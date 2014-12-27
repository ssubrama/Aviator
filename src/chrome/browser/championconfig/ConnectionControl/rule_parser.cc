// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

// Purpose - For parsing NTLM rule.

#include "rule_parser.h"

#include <sstream>
#include <algorithm>
#include <string>

#if defined(OS_WIN)
#include <windows.h>
#include <winsock2.h>
#include "chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h" // champion
#endif

#if defined (OS_MACOSX)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#endif // OS_MAC

namespace RULEParser {

bool RULEParser::Rule::IsBlockPrivate = false;
bool RULEParser::Rule::IsBlockPublic = false;
  
/////////////////////////////////////////// inline functions /////////////////////////////////////////////

// utility function to trim string from both sides
inline std::string TrimString(std::string& strData) {
  if (strData.size()) {
    strData.erase(0, strData.find_first_not_of(' '));         //prefixing spaces
    strData.erase(strData.find_last_not_of(' ') + 1);         //surfixing spaces
  }

  return strData;
}

// check if given charactor is numeric
inline bool IsNumeric(const char ch) {
  const std::string strNumeric = "0123456789";
  if (strNumeric.find(ch, 0) == std::string::npos)
    return false;

  return true;
}

bool detect_private_network(std::string domain_name, IP* ip_addr) {
  struct hostent *remoteHost;
  struct in_addr addr;
  std::string tempStrIP;
  bool private_net = false;

  remoteHost = gethostbyname(domain_name.c_str());
  IP ip;
  if (!(remoteHost == NULL)) {
    int i = 0;
    if (remoteHost->h_addrtype == AF_INET) {
      while (remoteHost->h_addr_list[i] != 0) {
        if (private_net)
           break;

        addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
        tempStrIP = std::string(inet_ntoa(addr));
        ip.Init(tempStrIP);
        private_net = ip.IsPrivate();
      }
    }
  }

  ip_addr = &ip;
  return private_net;
}

bool isIPAddress(std::string lastStr) {
  int nSize = lastStr.size();
  for (int i = 0; i < nSize; i++)
    if (lastStr[i] == '.' || (lastStr[i] >= 48 && lastStr[i] <= 57))
      continue;
    else
      return false;

  return true;
}
/////////////////////////////////////////// Global functions /////////////////////////////////////////////

// utility function to split string into strings
void SplitStringToString(const std::string& strData, 
                 const char chToken, 
                 std::vector<std::string>& storage, 
                 Convert type,
                 bool skipSpace,
                 bool trim) {
  if (!strData.size())
    return;

  std::istringstream f(strData);
  std::string part;
  while (getline(f, part, chToken)) {
    switch (type) {
      case Convert::TOLOWER:
        std::transform(part.begin(), part.end(), part.begin(), ::tolower);
        break;
      case Convert::TOUPPER:
        std::transform(part.begin(), part.end(), part.begin(), ::toupper);
        break;
      case Convert::NONE:
        break;
    }

    if (trim)
      TrimString(part);

    if (!skipSpace)
      storage.push_back(part);
    else if (skipSpace && part.size())
      storage.push_back(part);
  }
}

// utility function to split string into ints, max allowed depth is 10
void SplitStringToInt(const std::string& strData,
  const char chToken,
  std::vector<int>& storage,
  size_t maxDepth) {
  if (!strData.size() || maxDepth > 10)
    return;

  std::istringstream f(strData);
  std::string part;
  while (getline(f, part, chToken)) {
    size_t partSize = part.size();

    if (partSize && partSize <= maxDepth && IsNumeric(part[0])) {
      TrimString(part);
      int data = 0;
      try {
        data = atoi(part.c_str());
      } catch(std::exception) {}
      storage.push_back(data);
    } else
      return;
  }
}

////////////////////////////////////// IP class public definitions ///////////////////////////////////////

// class IP constructor
IP::IP(const std::string& ip) {
  Reset();
  if (IsValid(ip))
    Init(ip);
}

// class IP constructor
IP::IP() { Reset(); }

// check for validity
bool IP::IsValid() const {
  return (status == IP::VALID);
}

// Sets IP class
bool IP::Set(const std::string& ip) {
  if (IsValid(ip)) {
    Init(ip);
    return true;
  }

  return false;
}

// Resets IP class to defaults
void IP::Reset() {
  ipAddress.blockA = ipAddress.blockB = ipAddress.blockC = ipAddress.blockD = 0;
  status = IP::INVALID;
}

// static function to validate IP
bool IP::IsValid(const std::string& ip) {
  std::vector<int> blocks;
  SplitStringToInt(ip, '.', blocks, 3);
  if (blocks.size() == 4)
    if (blocks[0] == 255 &&
        blocks[1] == 255 &&
        blocks[2] == 255 &&
        blocks[3] == 255)
      return false;
    else if (blocks[0] > 0 && blocks[0] <= 255 &&
        blocks[1] >= 0 && blocks[1] <= 255 &&
        blocks[2] >= 0 && blocks[2] <= 255 &&
        blocks[3] >= 0 && blocks[3] <= 255)
      return true;
    else
      return false;

  return false;
}

// convert IP object to string
const std::string IP::ToString() const {
  if (!IsValid())
    return "Invalid";
std::string strIP; 
#if defined(OS_WIN)
  strIP = std::to_string(ipAddress.blockA) + "." +
  std::to_string(ipAddress.blockB) + "." +
  std::to_string(ipAddress.blockC) + "." +
  std::to_string(ipAddress.blockD);
#endif
#if defined(OS_MACOSX) 
  std::ostringstream StreamIP(strIP);
  StreamIP << ipAddress.blockA << "." << ipAddress.blockB << "." << ipAddress.blockC << "." << ipAddress.blockD;
  strIP = StreamIP.str();
#endif
  return strIP;
}

// Checks IP if in private range
bool IP::IsPrivate() const {
  return (IsInRange(privateRangeOneStart, privateRangeOneEnd) ||
    IsInRange(privateRangeTwoStart, privateRangeTwoEnd) ||
    IsInRange(privateRangeThreeStart, privateRangeThreeEnd));
}

// assignment operator for IP class
IP * IP::operator=(const IP& other) {
  ipAddress.blockA = other.ipAddress.blockA;
  ipAddress.blockB = other.ipAddress.blockB;
  ipAddress.blockC = other.ipAddress.blockC;
  ipAddress.blockD = other.ipAddress.blockD;

  return this;
}

// operator == for IP class
bool IP::operator==(const IP& other) {
  return (ipAddress.blockA == other.ipAddress.blockA &&
    ipAddress.blockB == other.ipAddress.blockB &&
    ipAddress.blockC == other.ipAddress.blockC &&
    ipAddress.blockD == other.ipAddress.blockD);
}

// operator != for IP class
bool IP::operator!=(const IP& other) {
  return !(*this == other);
}

// operator == for IP class
bool IP::operator==(const IP& other) const {
  return (ipAddress.blockA == other.ipAddress.blockA &&
    ipAddress.blockB == other.ipAddress.blockB &&
    ipAddress.blockC == other.ipAddress.blockC &&
    ipAddress.blockD == other.ipAddress.blockD);
}

// operator != for IP class
bool IP::operator!=(const IP& other) const {
  return !(*this == other);
}

/////////////////////////////////////// IP class private definitions /////////////////////////////////////

// splits and inits data
void IP::Init(const std::string ip) {
  std::vector<int> blocks;
  SplitStringToInt(ip, '.', blocks, 3);

  if (blocks.size() == 4)
    if (blocks[0] == 255 &&
        blocks[1] == 255 &&
        blocks[2] == 255 &&
        blocks[3] == 255)
      status = IP::INVALID;
    else if (blocks[0] > 0 && blocks[0] <= 255 &&
        blocks[1] >= 0 && blocks[1] <= 255 &&
        blocks[2] >= 0 && blocks[2] <= 255 &&
        blocks[3] >= 0 && blocks[3] <= 255)
        status = IP::VALID;
    else
      status = IP::INVALID;

  ipAddress.blockA = blocks[0];
  ipAddress.blockB = blocks[1];
  ipAddress.blockC = blocks[2];
  ipAddress.blockD = blocks[3];
}

// called by IsPrivate to find if IP is private
bool IP::IsInRange(const IP& rangeOne, const IP& rangeTwo) const {
  if (status == IP::INVALID)
    return false;

  if ((ipAddress.blockA != rangeOne.ipAddress.blockA) && 
      (ipAddress.blockA != rangeTwo.ipAddress.blockA))
    return false;

  if ((ipAddress.blockB < rangeOne.ipAddress.blockB) || 
      (ipAddress.blockB > rangeTwo.ipAddress.blockB))
    return false;

  if ((ipAddress.blockC < rangeOne.ipAddress.blockC) || 
      (ipAddress.blockC > rangeTwo.ipAddress.blockC))
    return false;

  if ((ipAddress.blockD < rangeOne.ipAddress.blockD) || 
      (ipAddress.blockD > rangeTwo.ipAddress.blockD))
    return false;

  return true;
}
///////////////////////////////////////////// End of IP class ////////////////////////////////////////////

/////////////////////////////////////// Rule public definitions //////////////////////////////////////

// static
RuleStrings Rule::ruleBody;

// NTLRule class constructor
Rule::Rule(const std::string& rule, Type rType) {
  ruleBody.clear();
  ruleBody.push_back("connection");
  switch (rType) {
    case Type::CONNECTIONCONTROL:
      ruleType = Type::CONNECTIONCONTROL;
      ruleBody.push_back("on");
      break;
    case Type::NTLM: default:
      ruleType = Type::NTLM;
      ruleBody.push_back("to");
      break;
  }

  ruleStatus = Status::INVALID;
  ruleState = State::UNKNOWN;
  ruleProtocol = Protocol::NONE;
  Host = "";
  Domain = "";
  Port = 0;
  ip.Reset();
  if (ruleType == Type::NTLM && IsValid(rule, rType))
    Parse(rule);
  else
   RULEParser::Rule::parseCCRule(rule);  
}

Rule :: ~Rule(){ } 

// static function to validate NTLM rule
bool Rule::IsValid(const std::string& rule, Type rType) {
  std::vector<std::string> blocks;
  SplitStringToString(rule, ';', blocks);
  if (blocks.size() != 2)
    return false;

  if (blocks[0] == "0" || blocks[0] == "1");
  else
    return false;

  std::vector<std::string> ruleParts;
  SplitStringToString(blocks[1], ' ', ruleParts);
  ruleBody.clear();
  ruleBody.push_back("connection");

  switch (rType) {
    case CONNECTIONCONTROL:
      ruleBody.push_back("on");
      if ((ruleParts.size() > 7) &&
        (ruleParts[0] == "Allow" || ruleParts[0] == "Deny") &&
        (ruleParts[1] == "http" || ruleParts[1] == "https" || ruleParts[1] == "ftp") &&
        (ruleParts[2] == ruleBody[0]) &&
        (ruleParts[3] == ruleBody[1]) &&
        (ruleParts[4] == "port") &&
        (ruleParts[6] == "to"))
      return true;
    case NTLM: default:
      ruleBody.push_back("to");
      if ((ruleParts.size() > 3) &&
          (ruleParts[0] == "Allow" || ruleParts[0] == "Deny") &&
          (ruleParts[1] == ruleBody[0]) &&
          (ruleParts[2] == ruleBody[1]))
        return true;
  }

  return false;
}

// resets rule
void Rule::Reset(const std::string& rule, Type rType) {
  ruleBody.clear();
  ruleBody.push_back("connection");
  switch (rType) {
  case Type::CONNECTIONCONTROL:
    ruleType = Type::CONNECTIONCONTROL;
    ruleBody.push_back("on");
    break;
  case Type::NTLM: default:
    ruleType = Type::NTLM;
    ruleBody.push_back("to");
    break;
  }

  ruleStatus = Status::INVALID;
  ruleState = State::UNKNOWN;
  ruleProtocol = Protocol::NONE;
  Host = "";
  Domain = "";
  Port = 0;
  ip.Reset();
  if (IsValid(rule, rType))
    Parse(rule);
}

// edit rule with new host
bool Rule::Edit(const std::string newHost) {
  if (!newHost.size())
    return false;

  Host = newHost;
  UpdateDomain();

  return true;
}

// edit rule with new state and new host
bool Rule::Edit(State newState, const std::string newHost) {
  if (newState == State::UNKNOWN)
    return false;

  ruleState = newState;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// edit rule with new state and new host
bool Rule::Edit(Status newStatus, const std::string newHost) {
  if (newStatus == Status::INVALID)
    return false;

  ruleStatus = newStatus;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// extended edit for connection control rule
bool Rule::Edit(Protocol newProtocol, const std::string newHost) {
  if (ruleType != Type::CONNECTIONCONTROL)
    return false;

  if (newProtocol == Protocol::NONE)
    return false;

  ruleProtocol = newProtocol;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// extended edit for connection control rule
bool Rule::Edit(int port, const std::string newHost) {
  if (ruleType != Type::CONNECTIONCONTROL)
    return false;

  if (port <= 0)
    return false;

  Port = port;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// extended edit for connection control rule
bool Rule::Edit(Protocol newProtocol, int port, const std::string newHost) {
  if (ruleType != Type::CONNECTIONCONTROL)
    return false;

  if (newProtocol == Protocol::NONE || port <= 0)
    return false;

  ruleProtocol = newProtocol;
  Port = port;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// edit rule with new state, new status and new host
bool Rule::Edit(State newState, Status newStatus, const std::string newHost) {
  if (newState == State::UNKNOWN || newStatus == Status::INVALID)
    return false;

  ruleState = newState;
  ruleStatus = newStatus;
  if (newHost.size()) {
    Host = newHost;
    UpdateDomain();
  }

  return true;
}

// Checks rules validity
bool Rule::IsValid() const {
  if (ruleType == Type::CONNECTIONCONTROL)
    return (ruleState != State::UNKNOWN && ruleStatus != Status::INVALID && ruleProtocol != Protocol::NONE && Port != 0);
  else
    return (ruleState != State::UNKNOWN && ruleStatus != Status::INVALID);
}

// Checks if rule is enabled
bool Rule::IsEnabled() const {
  return (ruleState == State::ENABLED);
}

// Checks if rule is allowed
bool Rule::IsAllowed() const {
  return (ruleStatus == Status::ALLOWED);
}

// Reverses rule state
void Rule::ReverseState() {
  if (ruleState == State::ENABLED)
    ruleState = State::DISABLED;
  else if (ruleState == State::DISABLED)
    ruleState = State::ENABLED;
}

// Reverses rule status
void Rule::ReverseStatus() {
  if (ruleStatus == Status::ALLOWED)
    ruleStatus = Status::DENIED;
  else if (ruleStatus == Status::DENIED)
    ruleStatus = Status::ALLOWED;
}

// convert Rule object to string
const std::string Rule::ToString() const {
  if (!IsValid())
    return "Unknown";

  std::string strRule = (ruleState == State::ENABLED) ? "1;" : "0;";
  return strRule + ToRule();
}

// get NTLM rule string
const std::string Rule::ToRule() const {
  if (!IsValid())
    return "Unknown";

  std::string strRule = (ruleStatus == Status::ALLOWED) ? "Allow " : "Deny ";
  if (ruleType == Type::CONNECTIONCONTROL) {
    switch (ruleProtocol) {
      case HTTP:
        strRule += "http ";
        break;
      case HTTPS:
        strRule += "https ";
        break;
      case FTP:
        strRule += "ftp ";
        break;
      case WS:
        strRule += "WS ";
        break;
      case WSS:
        strRule += "WSS ";
        break;
      case GOPHER:
        strRule += "GOPHER ";
        break; 
      case FILE:
        strRule += "FILE ";
        break; 
      case NONE:
        break;
    }

    strRule += ruleBody[0] + " " + ruleBody[1] + " ";
#if defined(OS_WIN)
    strRule += "port " + std::to_string(Port) + " to ";
#endif
#if defined(OS_MACOSX)
    std::ostringstream StrPort;
    StrPort << Port;
    strRule += "port " + StrPort.str() + " to ";
#endif
  } else
   strRule += ruleBody[0] + " " + ruleBody[1] + " ";

   return strRule + Host;
}

// assignment operator for rule class
Rule * Rule::operator = (const Rule& other) {
  ruleState = other.ruleState;
  ruleStatus = other.ruleStatus;
  ruleType = other.ruleType;
  ruleProtocol = other.ruleProtocol;
  Port = other.Port;
  Host = other.Host;
  Domain = other.Domain;
  ip = other.ip;

  return this;
}

// operator == for rule class
bool Rule::operator == (const Rule& other) {
  if (ruleState != other.ruleState)
    return false;
  if (ruleStatus != other.ruleStatus)
    return false;
  if (Port != other.Port)
    return false;
  if (ruleType != other.ruleType)
    return false;
  if (ruleProtocol != other.ruleProtocol)
    return false;
  if (Host != other.Host)
    return false;
  if (ip != other.ip)
    return false;
  if (Domain != other.Domain)
    return false;

  return true;
}

// operator != for rule class
bool Rule::operator != (const Rule& other) {
  return !(*this == other);
}

bool Rule::operator == (const Rule& other) const {
  if (ruleState != other.ruleState)
    return false;
  if (ruleStatus != other.ruleStatus)
    return false;
  if (Port != other.Port)
    return false;
  if (ruleType != other.ruleType)
    return false;
  if (ruleProtocol != other.ruleProtocol)
    return false;
  if (Host != other.Host)
    return false;
  if (ip != other.ip)
    return false;
  if (Domain != other.Domain)
    return false;  
  return true;
}

// operator != for rule class
bool Rule::operator != (const Rule& other) const {
  return !(*this == other);
}

/********** connection control ********************/
void RULEParser::Rule::parseCCRule(const std::string rulestr) {
  GetProperty(rulestr);
}

void Rule::GetProperty(const std::string rule) {
	bool bVal = false;
	DomainIsIP = false;
	std::vector<std::string> strs;
  strs.clear();
  if (!rule.substr(0, 1).compare("1"))
      bVal = true;    
  std::string strtemp = rule.substr(2);
	
  size_t pos = strtemp.find(' ');
  size_t initialPos = 0;

  // Decompose statement 
  while (pos != std::string::npos) {
    strs.push_back(strtemp.substr(initialPos, pos - initialPos + 1));
    initialPos = pos + 1;
    pos = strtemp.find(' ', initialPos);
  }

  // Add the last one 
  size_t istrs = pos < strtemp.size() ? pos : strtemp.size();
  strs.push_back(strtemp.substr(initialPos, (istrs - initialPos + 1)));
  std::vector<std::string> ::iterator i;
  std::string lastStr;

  /* Creating the Network Rule objs - starts */
  bool bPortval = false;
  bool donotlook = false;
  for (i = strs.begin(); i != strs.end(); i++) {
    std::string tempStr(*i);
    lastStr = tempStr;
    if (!tempStr.compare("Allow ")) {
      ruleStatus = Status::ALLOWED;
      continue;
    }

    if (!tempStr.compare("Deny ")) {
      ruleStatus = Status::DENIED;
      continue;
    }

    if (!tempStr.compare("http ")) {
      ruleProtocol = Protocol::HTTP;
      continue;
    } else if (!tempStr.compare("https ")) {
      ruleProtocol = Protocol::HTTPS;
      continue;
    } else if (!tempStr.compare("ftp ")) {
      ruleProtocol = Protocol::FTP;
      continue;
    } else if (!tempStr.compare("file ")) {
      ruleProtocol = Protocol::FILE;
      continue;
    } else if (!tempStr.compare("gopher ")) {
      ruleProtocol = Protocol::GOPHER;
      continue;
    } else if (!tempStr.compare("ws ")) {
      ruleProtocol = Protocol::WS;
      continue;
    } else if (!tempStr.compare("wss ")) {
      ruleProtocol = Protocol::WSS;
      continue;
    } else
      ruleProtocol = Protocol::HTTP;

    if (!tempStr.compare("port ")) {
      bPortval = true;
      continue;
    }

    if (bPortval) {
      std::vector<int> portval;
      portval.clear();
      size_t pos = tempStr.find(',');
      size_t initialPos = 0;
      std::string portStr;
      while (pos != std::string::npos) {
        portStr = tempStr.substr(initialPos, pos - initialPos + 1);
        portval.push_back(atoi(portStr.c_str()));
        initialPos = pos + 1;
        pos = tempStr.find(',', initialPos);
      }

      // Add the last one
      size_t iportstr = pos < tempStr.size() ? pos : tempStr.size();//std::min(pos,(int)tempStr.size());
      portStr = tempStr.substr(initialPos, (iportstr - initialPos + 1));
      portval.push_back(atoi(portStr.c_str()));
      m_vPort = portval; 
      bPortval = false;
      continue;
    }

    if (!tempStr.compare("Public ")) {
      if (bVal)
      RULEParser::Rule::IsBlockPublic = true;
      else
      RULEParser::Rule::IsBlockPublic = false;
      donotlook = true;
      continue;
    }

    if (!tempStr.compare("Private ")) {
      if (bVal)
      	 RULEParser::Rule::IsBlockPrivate = true;
      else
    	  RULEParser::Rule::IsBlockPrivate = false;
      donotlook = true;
      continue;
    }
  }

  if (!donotlook){
    if (!lastStr.compare(0, 7, "http://")) {
      Domain = lastStr.substr(7);
    } else if (!lastStr.compare(0, 8, "https://")) {
      Domain = lastStr.substr(8);
    } else if (!lastStr.compare(0, 6, "ftp://")) {
      Domain = lastStr.substr(6);
    } else if (!lastStr.compare(0, 9, "gopher://")) {
      Domain = lastStr.substr(9);
    } else if (!lastStr.compare(0, 5, "ws://")) {
      Domain = lastStr.substr(5);
    } else if (!lastStr.compare(0, 6, "wss://")) {
      Domain = lastStr.substr(6);
    } else {
      Domain = lastStr;
    }

    //IP ip;
    if (IP::IsValid(Domain)) {
      DomainIsIP = true;

      ip.Init(Domain);
      IsPrivate = ip.IsPrivate();
    } else {
      IsPrivate = detect_private_network(Domain, &ip);
    }
  }
}

// parses NTLM rule
void Rule::Parse(const std::string& rule) {
  std::vector<std::string> blocks;
  SplitStringToString(rule, ';', blocks);
  if (blocks[0] == "0")
    ruleState = State::DISABLED;
  else if (blocks[0] == "1")
    ruleState = State::ENABLED;
  else {
    ruleState = State::UNKNOWN;
    return;
  }

  std::vector<std::string> ruleParts;
  SplitStringToString(blocks[1], ' ', ruleParts);

  size_t hostStartIndex = 0;
  if (ruleType == Type::CONNECTIONCONTROL && !IsValidConnectionControlRule(ruleParts, hostStartIndex))
    return;
  else if (ruleType == Type::NTLM && !IsValidNTLMRule(ruleParts, hostStartIndex))
    return;
  
  if (!hostStartIndex)
    return;

  for (size_t i = hostStartIndex; i < ruleParts.size(); i++)
    if (i < ruleParts.size() - 1)
      Host = ruleParts[i] + " ";
    else
      Host += ruleParts[i];

  UpdateDomain();
  if (IP::IsValid(Host))
    ip.Set(Host);
}

// connection control rule string validation
bool Rule::IsValidConnectionControlRule(const RuleStrings& ruleParts, size_t& hostStartIndex) {
  if ((ruleParts.size() > 7) &&
      (ruleParts[0] == "Allow" || ruleParts[0] == "Deny") &&
      (ruleParts[1] == "http" || ruleParts[1] == "https" || ruleParts[1] == "ftp") &&
      (ruleParts[2] == ruleBody[0]) &&
      (ruleParts[3] == ruleBody[1]) &&
      (ruleParts[4] == "port") &&
      (ruleParts[6] == "to")) {
    if (ruleParts[0] == "Allow")
      ruleStatus = Status::ALLOWED;
    else if (ruleParts[0] == "Deny")
      ruleStatus = Status::DENIED;
    else {
      ruleState = State::UNKNOWN;
      ruleStatus = Status::INVALID;
      ruleProtocol = Protocol::NONE;
      return false;
    }

    if (ruleParts[1] == "http")
      ruleProtocol = Protocol::HTTP;
    else if (ruleParts[1] == "https")
      ruleProtocol = Protocol::HTTPS;
    else if (ruleParts[1] == "ftp")
      ruleProtocol = Protocol::FTP;
    else {
      ruleState = State::UNKNOWN;
      ruleStatus = Status::INVALID;
      ruleProtocol = Protocol::NONE;
      return false;
    }

    try {
      Port = std::atoi(ruleParts[5].c_str());
      if (Port <= 0) {
        ruleState = State::UNKNOWN;
        ruleStatus = Status::INVALID;
        ruleProtocol = Protocol::NONE;
        Port = 0;
        return false;
      }
    } catch (std::exception) {
      ruleState = State::UNKNOWN;
      ruleStatus = Status::INVALID;
      ruleProtocol = Protocol::NONE;
      Port = 0;
      return false;
    }

    hostStartIndex = 7;
  } else
    return false;

  return true;
}

// ntlm rule string validation
bool Rule::IsValidNTLMRule(const RuleStrings& ruleParts, size_t& hostStartIndex) {
  if ((ruleParts.size() > 3) &&
      (ruleParts[0] == "Allow" || ruleParts[0] == "Deny") &&
      (ruleParts[1] == "connection") &&
      (ruleParts[2] == "to")) {
    if (ruleParts[0] == "Allow")
      ruleStatus = Status::ALLOWED;
    else if (ruleParts[0] == "Deny")
      ruleStatus = Status::DENIED;
    else {
      ruleState = State::UNKNOWN;
      ruleStatus = Status::INVALID;
      return false;
    }

    hostStartIndex = 3;
  } else
    return false;

  return true;
}

// updates rule's domain
void Rule::UpdateDomain() {
  if (IP::IsValid(Host) || Host == "Private Network" || Host == "Public Network")
    Domain = "";
  else {
    RuleStrings blocks;
    SplitStringToString(Host, '/', blocks);

    if (!blocks.size())
      return;

    if (blocks.size() == 1) {
      if (blocks[0].find("www.") != std::string::npos)
        Domain = blocks[0].substr(4, blocks[0].size());
      else
        Domain = blocks[0];
    } else {
#if defined (OS_WIN) 
    RuleStrings protocols = { { "http:" }, { "https:" }, { "ftp:" } };
#endif
#if defined (OS_MACOSX)
      RuleStrings protocols;
      protocols.push_back("http:");
      protocols.push_back("https:");
      protocols.push_back("ftp:");
#endif    
      StringIterator it = std::find(protocols.begin(), protocols.end(), blocks[0]);
      if (it != protocols.end())
        if (blocks[1].find("www.") != std::string::npos)
          Domain = blocks[1].substr(4, blocks[1].size());
        else
          Domain = blocks[1];
    } 
  }
}

///////////////////////////////////////// end of Rule class //////////////////////////////////////////

///////////////////////////////////// end of RuleStorage class ///////////////////////////////////////
RuleStore::RuleStore() {}
RuleStore::~RuleStore() {}

// Constructor for rule storage
RuleStore::RuleStore(const RuleStrings& rules) {
  if (rules.size())
    StoreRules(rules);
}

// resets rule storage
bool RuleStore::Reset(const RuleStrings& rules) {
  if (rules.size()) {
    ruleStorage.clear();
    StoreRules(rules);
    return true;
  }

  return false;
}

// find object based rule and return reference of object
Rule* RuleStore::FindRule(const Rule& rule) {
  if (IsEmpty())
    return NULL;

  RuleIterator it = std::find(ruleStorage.begin(), ruleStorage.end(), rule);
  if (it != ruleStorage.end())
    return &(*it);

  return NULL;
}

// find string based rule and return reference of object
Rule* RuleStore::FindRule(const std::string& rule) {
  if (IsEmpty())
    return NULL;

  Rule newRule(rule);
  RuleIterator it = std::find(ruleStorage.begin(), ruleStorage.end(), newRule);
  if (it != ruleStorage.end())
    return &(*it);

  return NULL;
}

// find object based rule and return reference of object
const Rule* RuleStore::FindRule(const Rule& rule) const {
  if (!ruleStorage.size())
    return NULL;

  for (size_t i = 0; i < ruleStorage.size(); i++) {
    const Rule& currentRule = ruleStorage[i];
    if (rule == currentRule)
      return &currentRule;
  }

  return NULL;
}

// find string based rule and return reference of object
const Rule* RuleStore::FindRule(const std::string& rule) const {
  if (!ruleStorage.size())
    return NULL;

  Rule newRule(rule);
  for (size_t i = 0; i < ruleStorage.size(); i++) {
    const Rule& currentRule = ruleStorage[i];
    if (newRule == currentRule)
      return &currentRule;
  }

  return NULL;
}

// adds new rule to storage
bool RuleStore::Add(const Rule& rule) {
  if (!rule.IsValid())
    return false;
  
  ruleStorage.push_back(rule);
  AddIntoTypedStore(rule);

  return true;
}

// adds new rule to storage
bool RuleStore::Add(const std::string& rule) {
  if (!Rule::IsValid(rule))
    return false;

  Rule newRule(rule);
  ruleStorage.push_back(newRule);
  AddIntoTypedStore(newRule);

  return true;
}

// inserts new rule to storage at given place
bool RuleStore::Insert(size_t at, const Rule& rule) {
  if (!rule.IsValid() || at < 0 || at > ruleStorage.size() - 1)
    return false;

  RuleIterator it = ruleStorage.begin();
  ruleStorage.insert(it + at, rule);
  AddIntoTypedStore(rule);
  
  return true;
}

// inserts new rule to stroage at give place
bool RuleStore::Insert(size_t at, const std::string& rule) {
  if (!Rule::IsValid(rule) || at < 0 || at > ruleStorage.size() - 1)
    return false;

  Rule newRule(rule);
  RuleIterator it = ruleStorage.begin();
  ruleStorage.insert(it + at, newRule);
  AddIntoTypedStore(newRule);
  
  return true;
}

// removes last rule
bool RuleStore::Remove() {
  if (IsEmpty())
    return false;

  RuleIterator it = ruleStorage.end() - 1;
  RemoveFromTypedStore(*it);
  ruleStorage.erase(it);

  return true;
}

// removes rule from given place
bool RuleStore::Remove(size_t at) {
  if (IsEmpty() || at < 0 || at > ruleStorage.size() - 1)
    return false;

  RuleIterator it = ruleStorage.begin();
  RemoveFromTypedStore(*(it + at));
  ruleStorage.erase(it + at);

  return true;
}

// removes given rule
bool RuleStore::Remove(const Rule& rule) {
  if (IsEmpty() || !rule.IsValid())
    return false;

  RuleIterator it = std::find(ruleStorage.begin(), ruleStorage.end(), rule);
  if (it == ruleStorage.end())
    return false;

  ruleStorage.erase(it);
  RemoveFromTypedStore(rule);

  return true;
}

// removes given rule
bool RuleStore::Remove(const std::string& rule) {
  if (IsEmpty() || !Rule::IsValid(rule))
    return false;

  Rule newRule(rule);
  RuleIterator it = std::find(ruleStorage.begin(), ruleStorage.end(), newRule);
  if (it == ruleStorage.end())
    return false;

  ruleStorage.erase(it);
  RemoveFromTypedStore(newRule);

  return true;
}

// checks if private network rule is allowed
bool RuleStore::IsPrivateNetworkAllowed(const std::string strPvtNetRule) const {
  if (!ruleStorage.size())
    return false;

  const Rule pvtNets(strPvtNetRule);
  if (!pvtNets.IsValid() || pvtNets.GetHost() != "Private Network")
    return false;

  if (FindRule(pvtNets))
    return true;

  return false;
}

// checks if public network rule is allowed
bool RuleStore::IsPublicNetworkAllowed(const std::string strPubNetRule) const {
  if (!ruleStorage.size())
    return false;

  const Rule pubNets(strPubNetRule);
  if (!pubNets.IsValid() || pubNets.GetHost() != "Public Network")
    return false;

  if (FindRule(pubNets))
    return true;

  return false;
}

// checks if specified url is a host
bool RuleStore::HostIsURL(const std::string& URL) {
  if (!ruleStorage.size())
    return false;

  for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++) {
    std::string strHost = it->GetHost();
    if (strHost == URL)
      return true;
  }

  return false;
}

// checks if specified domain is allowed
bool RuleStore::DomainIsAllowed(const std::string& domain) {
  if (!ruleStorage.size())
    return false;

  for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++) {
    std::string strDomain = it->GetDomain();
    if (strDomain == domain)
      return true;
  }

  return false;
}

// updates enabled and disabled rule storage
void RuleStore::IndexRules() {
  if (!ruleStorage.size())
    return;

  enabledRules.clear();
  disabledRules.clear();
  for (size_t i = 0; i < ruleStorage.size(); i++) {
    if (ruleStorage[i].IsEnabled())
      enabledRules.push_back(ruleStorage[i]);
    else
      disabledRules.push_back(ruleStorage[i]);
  }
}

// return filtered list storage
RuleStorage& RuleStore::GetFilteredRules(Rule::State state, Rule::Status status) {
  filteredRules.clear();
  if (state == Rule::ENABLED && status == Rule::ALLOWED) {
    for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++)
      if ((*it).IsEnabled() && (*it).IsAllowed())
        filteredRules.push_back(*it);
  } else if (state == Rule::ENABLED && status == Rule::DENIED) {
    for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++)
      if ((*it).IsEnabled() && !(*it).IsAllowed())
        filteredRules.push_back(*it);
  } else if (state == Rule::DISABLED && status == Rule::ALLOWED) {
    for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++)
      if (!(*it).IsEnabled() && (*it).IsAllowed())
        filteredRules.push_back(*it);
  } else {
    for (RuleIterator it = ruleStorage.begin(); it < ruleStorage.end(); it++)
      if (!(*it).IsEnabled() && !(*it).IsAllowed())
        filteredRules.push_back(*it);
  }

  return filteredRules;
}

// assignment operator for RuleStore
RuleStore* RuleStore::operator = (const RuleStore& other) {
  ruleStorage.clear();
  enabledRules.clear();
  disabledRules.clear();
  invalidRules.clear();
  filteredRules.clear();

  std::copy(other.ruleStorage.begin(), other.ruleStorage.end(), ruleStorage.begin());
  std::copy(other.enabledRules.begin(), other.enabledRules.end(), enabledRules.begin());
  std::copy(other.disabledRules.begin(), other.disabledRules.end(), disabledRules.begin());
  std::copy(other.invalidRules.begin(), other.invalidRules.end(), invalidRules.begin());
  std::copy(other.filteredRules.begin(), other.filteredRules.end(), filteredRules.begin());

  return this;
}

// array operator for rule storage
Rule* RuleStore::operator [] (const size_t nAt) {
  if (nAt < 0 || nAt >= ruleStorage.size())
    return NULL;

  return &ruleStorage[nAt];
}

// array operator for rule storage
const Rule* RuleStore::operator [] (const size_t nAt) const {
  if (nAt < 0 || nAt >= ruleStorage.size())
    return NULL;

  return &ruleStorage[nAt];
}

// stores rules into storage
void RuleStore::StoreRules(const RuleStrings& rules) {
  enabledRules.clear();
  disabledRules.clear();
  invalidRules.clear();
  for (size_t i = 0; i < rules.size(); i++) {
    if (!Rule::IsValid(rules[i])) {
      invalidRules.push_back(rules[i]);
      continue;
    } else {
      Rule rule(rules[i]);
      ruleStorage.push_back(rule);
      AddIntoTypedStore(rule);
    }
  }
}

// remove from enabled or disabled based on rule status
void RuleStore::RemoveFromTypedStore(const Rule& rule) {
  RuleIterator itEnabled = std::find(enabledRules.begin(), enabledRules.end(), rule);
  if (itEnabled != enabledRules.end())
    enabledRules.erase(itEnabled);
  else {
    RuleIterator itDisabled = std::find(disabledRules.begin(), disabledRules.end(), rule);
    if (itDisabled != disabledRules.end())
      disabledRules.erase(itDisabled);
  }
}

// add in enabled or disabled based on rule status
void RuleStore::AddIntoTypedStore(const Rule& rule) {
  if (rule.IsEnabled())
    enabledRules.push_back(rule);
  else
    disabledRules.push_back(rule);
}

} // namespace RULEParser
