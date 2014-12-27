// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

// Purpose - For parsing NTLM rule.

#ifndef CHROME_BROWSER_CHAMPIONCONFIG_NTLMCONTROL_RULE_PARSER_H
#define CHROME_BROWSER_CHAMPIONCONFIG_NTLMCONTROL_RULE_PARSER_H

#include <string>
#include <vector>
#include "url/gurl.h"
#include "base/files/file_path.h"

#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wdangling-else"
#pragma GCC diagnostic ignored "-Wexit-time-destructors"
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif

namespace RULEParser {

class IP;
 
// flags to convert string into lower or upper, used in SplitStringToString
enum Convert {TOLOWER, TOUPPER, NONE};

bool detect_private_network(std::string domain_name, IP* ip_addr);

// splits string, can skip spaces and can trim from both ends
void SplitStringToString(const std::string& strData,
  const char chToken,
  std::vector<std::string>& storage,
  Convert type = Convert::NONE,
  bool skipSpace = true,
  bool trim = true);

// splits string and convert them into int, max allowed depth is 10
void SplitStringToInt(const std::string& strData,
  const char chToken,
  std::vector<int>& storage,
  size_t maxDepth=10);

// class IP declaration
class IP {
protected:
  struct IPAddress {
    int blockA;
    int blockB;
    int blockC;
    int blockD;
  };

public:
  enum Status {
    INVALID,
    VALID
  };

  IP(const std::string& ip);
  IP();
  ~IP() {}

  static bool IsValid(const std::string& ip);

  const IPAddress& GetAdress() const { return const_cast<const IPAddress&>(ipAddress); }
  bool IsValid() const;
  bool Set(const std::string& ip);
  void Reset();
  const std::string ToString() const;
  bool IsPrivate() const;
  void Init(const std::string ip);

  IP * operator = (const IP& other);
  bool operator == (const IP& other);
  bool operator != (const IP& other);
  bool operator == (const IP& other) const;
  bool operator != (const IP& other) const;

private:
  bool IsInRange(const IP& rangeOne, const IP& rangeTwo) const;

  IPAddress ipAddress;
  Status status;
};

// Private IPs range
const IP privateRangeOneStart("10.0.0.0");
const IP privateRangeOneEnd("10.255.255.255");
const IP privateRangeTwoStart("172.16.0.0");
const IP privateRangeTwoEnd("172.31.255.255");
const IP privateRangeThreeStart("192.168.0.0");
const IP privateRangeThreeEnd("192.168.255.255");

typedef std::vector<std::string> RuleStrings;

// class Rule declaration
class Rule {
public:
  enum State {
    DISABLED,
    ENABLED,
    UNKNOWN
  };
  enum Status {
    DENIED,
    ALLOWED,
    INVALID
  };
  enum Type {
    NTLM,
    CONNECTIONCONTROL
  };
  enum Protocol {
    HTTP,
    HTTPS,
    FTP,
    WS, 
    WSS, 
    GOPHER, 
    FILE,
    NONE
  };

  Rule(const std::string& rule, Type rType=Type::NTLM);
  ~Rule();

  static bool IsValid(const std::string& rule, Type rType=Rule::NTLM);

  void Reset(const std::string& rule, Type rType = Type::NTLM);
  bool Edit(const std::string newHost);
  bool Edit(State newState, const std::string newHost = "");
  bool Edit(Status newStatus, const std::string newHost = "");
  bool Edit(Protocol newProtocol, const std::string newHost = "");
  bool Edit(int port, const std::string newHost = "");
  bool Edit(Protocol newProtocol, int port, const std::string newHost = "");
  bool Edit(State newState, Status newStatus, const std::string newHost = "");
  bool IsValid() const;
  bool IsEnabled() const;
  bool IsAllowed() const;
  void ReverseState();
  void ReverseStatus();
  const std::string GetHost() const { return const_cast<const std::string&>(Host); }
  const std::string GetDomain() const { return const_cast<const std::string&>(Domain); }
  const IP& GetIP() const { return const_cast<const IP&>(ip); }
  Status GetStatus() const { return ruleStatus; }
  State GetState() const { return ruleState; }
  Type GetType() const { return ruleType; }
  Protocol GetProtocol() const { return ruleProtocol; }
  int GetPort() const { return Port; }
  const std::string ToString() const;
  const std::string ToRule() const;

  Rule* operator = (const Rule& other);
  bool operator == (const Rule& other);
  bool operator != (const Rule& other);
  bool operator == (const Rule& other) const;
  bool operator != (const Rule& other) const;
  // bool parseCCRule(const base::FilePath::StringType rulestr);
  void parseCCRule(const std::string  rulestr);
  std::vector<int> GetPorts() { return m_vPort; }
  Protocol GetProtocol() { return ruleProtocol; }
  bool IsDomainIP() { return DomainIsIP; }
  IP GetIP() { return ip; }
  std::string GetDomain() { return Domain; }
  bool GetPrivate() { return IsPrivate; }
  static bool GetIsBlockPrivate() { return IsBlockPrivate;}
  static bool GetIsBlockPublic() { return IsBlockPublic; }

 
 static void SetIsBlockPrivate(bool BlockPrivate) { IsBlockPrivate = BlockPrivate; }
 static void SetIsBlockPublic(bool BlockPublic) { IsBlockPublic = BlockPublic; }
 
private:
  void Parse(const std::string& rule);
  bool IsValidConnectionControlRule(const RuleStrings& ruleParts, size_t& hostStartIndex);
  bool IsValidNTLMRule(const RuleStrings& ruleParts, size_t& hostStartIndex);
  void UpdateDomain();
  void GetProperty(const std::string strs);
  
  State ruleState;
  static RuleStrings ruleBody;
  Status ruleStatus;
  Type ruleType;
  Protocol ruleProtocol;
  int Port;
  std::vector<int> m_vPort;
  std::string Host;
  std::string Domain;
  GURL DomainURL;
  IP ip;
  bool IsPrivate;
  bool DomainIsIP;
  static bool IsBlockPrivate;
  static bool IsBlockPublic;
};

typedef std::vector<Rule> RuleStorage;
typedef std::vector<std::string>::iterator StringIterator;
typedef std::vector<Rule>::iterator RuleIterator;

// class RuleStore declaration
class RuleStore {
public:
  RuleStore(const RuleStrings& rules);
  RuleStore();
  ~RuleStore();

  bool Reset(const RuleStrings& rules);
  Rule* FindRule(const Rule& rule);
  Rule* FindRule(const std::string& rule);
  const Rule* FindRule(const Rule& rule) const;
  const Rule* FindRule(const std::string& rule) const;
  bool Add(const Rule& rule);
  bool Add(const std::string& rule);
  bool Insert(size_t at, const Rule& rule);
  bool Insert(size_t at, const std::string& rule);
  bool Remove();
  bool Remove(size_t at);
  bool Remove(const Rule& rule);
  bool Remove(const std::string& rule);
  bool IsEmpty() { return (ruleStorage.size() == 0); }
  bool IsPrivateNetworkAllowed(const std::string strPvtNetRule) const;
  bool IsPublicNetworkAllowed(const std::string strPubNetRule) const;
  bool HostIsURL(const std::string& URL);
  bool DomainIsAllowed(const std::string& domain);
  size_t Size() const { return ruleStorage.size(); }
  size_t EnabledListSize() const { return enabledRules.size(); }
  size_t DisabledListSize() const { return disabledRules.size(); }
  void IndexRules();
  const RuleStrings& GetInvalidRules() const { return const_cast<const RuleStrings&>(invalidRules); }
  RuleStorage& GetEnabledRules() { return enabledRules; }
  RuleStorage& GetDisabedRules() { return disabledRules; }
  RuleStorage& GetFilteredRules(Rule::State state=Rule::ENABLED, Rule::Status status = Rule::ALLOWED);

  RuleStore* operator=(const RuleStore& other);
  Rule* operator[](const size_t at);
  const Rule* operator[](const size_t at) const;
  
private:
  void StoreRules(const RuleStrings& rules);
  void RemoveFromTypedStore(const Rule& rule);
  void AddIntoTypedStore(const Rule& rule);

  RuleStorage ruleStorage;
  RuleStorage enabledRules;
  RuleStorage disabledRules;
  RuleStorage filteredRules;
  RuleStrings invalidRules;
  
  // NPCC Rule Obj list
  RuleStorage PrivateAllowRuleList;
  RuleStorage PrivateDenyRuleList;
  RuleStorage PublicAllowRuleList;
  RuleStorage PublicDenyRuleList;
};

} // namespace RULEParser

#endif // CHROME_BROWSER_CHAMPIONCONFIG_NTLMCONTROL_RULE_PARSER_H
