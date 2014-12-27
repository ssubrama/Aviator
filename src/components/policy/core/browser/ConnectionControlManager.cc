// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
#include "components/policy/core/browser/ConnectionControlManager.h"
#include "chrome/browser/championconfig/NTLMControl/rule_parser.h" // champion : ntlm rule parser (amresh)
#include "chrome/browser/championconfig/varsconfig.h"
#include "../chrome/grit/generated_resources.h"

#include "components/policy/core/browser/url_blacklist_manager.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/first_run/first_run.h"
// champion end

#include "base/prefs/pref_service.h"
#include "net/url_request/url_request.h"
#include "url/url_parse.h"


//champion
#include "base/file_util.h"
#include "base/strings/string_util.h"

#include "chrome/browser/shell_integration.h"
#include "chrome/browser/ui/webui/options/browser_options_handler.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"

#if defined(OS_MACOSX)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

// #include "chrome/browser/app_controller_mac.h"
#endif

#if defined(OS_WIN)
#include <windows.h>
#include <winsock2.h>
#include "chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h" // champion
#endif

#include "chrome/browser/ui/startup/default_browser_prompt.h" 
#include<stdlib.h> //For system call
#include "chrome/common/url_constants.h"  

bool policy::ConnectionControlManager::bLoadInDefault = false;
bool policy::ConnectionControlManager::bUpdateNeeded = false;
std::vector<std::string>* policy::ConnectionControlManager::rulearray = NULL;
std::vector<bool>*  policy::ConnectionControlManager::IsEnableVector = NULL;


namespace policy {

ConnectionControlManager::ConnectionControlManager(PrefService* pref_service) {
 pref_service_ = pref_service; 
 LoadRules();
}
void ConnectionControlManager::LoadInDefaultBrowser(const GURL& url, int length) const {
  std::string urlValue = url.spec();
  std::size_t found = urlValue.find("::Error138");
  if (found != std::string::npos) {
    // champion : Added to stop multiple tabs in alternate browser when load by default is checked ( Navneet )
    if (policy::ConnectionControlManager::bLoadInDefault)
      return;
    urlValue = urlValue.erase(urlValue.length() - 10, urlValue.length() - 1);
  }
  std::string strCommand = "";

#if defined(OS_WIN)
  base::FilePath strFireFoxMachinePath = ABF::GetFirefoxInstallPathFromRegistry();
  base::FilePath strChromeMachinePath = ABF::GetChromeInstallPathFromRegistry();

  base::FilePath strIEMachinePath = ABF::GetIEInstallPathFromRegistryLM();
  base::FilePath strIEUserPath = ABF::GetIEInstallPathFromRegistryCU();

  std::string strProgramPath = "C:";
  std::string strFFPath = "";
  std::string strGCPath = "";
  std::string strIEPath = "";
  std::string m_stralternatebrowser_ = "";

  std::string strCheckFolderFor64bitWin = "C:\\Program Files (x86)";
  if (ABF::DirExists(strCheckFolderFor64bitWin))
    strProgramPath += "\\Program Files (x86)\\";
  else
    strProgramPath += "\\Program Files\\";

  strIEPath = strProgramPath + "Internet Explorer\\iexplore.exe ";

  if (!strIEMachinePath.empty())
    strIEPath = base::WideToUTF8(strIEMachinePath.value());
  else if (!strIEUserPath.empty())
    strIEPath = base::WideToUTF8(strIEUserPath.value());
  else
    std::string strIEPath = strProgramPath + "Internet Explorer\\iexplore.exe ";

  if (!strFireFoxMachinePath.empty())
    strFFPath = base::WideToUTF8(strFireFoxMachinePath.value()) + "\\" + "firefox.exe ";
  else
    strFFPath = strProgramPath + "Mozilla Firefox\\firefox.exe ";

  if (!ABF::GetChromeInstalledPath().empty())
    strGCPath = ABF::GetChromeInstalledPath();
  else
    strGCPath = strProgramPath + "Google\\Chrome\\Application\\chrome.exe ";

  std::string strOperaPath = strProgramPath + "Opera\\launcher.exe ";
  std::string strDefaultBrowser = ABF::GetDefaultBrowserForAviator();

  m_stralternatebrowser_ = pref_service_->GetString(ABF::kDisplayAlternateBrowser);

  if (!ABF::IsAviatorDefaultBrowser()) {
    if (ABF::IsFirefoxDefaultBrowser())
      strCommand = strFFPath + " " + urlValue;
    else if (ABF::IsGoogleChromeDefaultBrowser())
      strCommand = strGCPath + " " + urlValue;
    else if (ABF::IsIEDefaultBrowser())
      strCommand = strIEPath + " " + urlValue;
    else
      strCommand = strIEPath + " " + urlValue;
  } else if (ABF::IsAviatorDefaultBrowser()) {
    if (m_stralternatebrowser_ == "Google Chrome")
      strCommand = strGCPath + " " + urlValue;
    else if (m_stralternatebrowser_ == "Firefox")
      strCommand = strFFPath + " " + urlValue;
    else if (m_stralternatebrowser_ == "Opera")
      strCommand = strOperaPath + " " + urlValue;
    else
      strCommand = strIEPath + " " + urlValue;
  } else {
    strCommand = strIEPath + " " + urlValue;
  }

  if (urlValue != "about:blank")
    WinExec(strCommand.c_str(), SW_SHOW);
#endif
}

/*
// returns true if IP is in Private reserved IP range.
// returns FALSE for the public IP
bool Validate_ip_range(IP ip_add) {
  if (ip_add.intIP.part1 == TYPE_A) {
    if (((ip_add.intIP.part2 >= 0) && (ip_add.intIP.part2 <= 255))) {
      if (((ip_add.intIP.part3 >= 0) && (ip_add.intIP.part3 <= 255)) && ((ip_add.intIP.part4 >= 0) && (ip_add.intIP.part4 <= 255)))
        return true;
    } else
      return false;
  } else if (ip_add.intIP.part1 == TYPE_B) {
    if (((ip_add.intIP.part2 >= 16) && (ip_add.intIP.part2 <= 31))) {
      if (((ip_add.intIP.part3 >= 0) && (ip_add.intIP.part3 <= 255)) && ((ip_add.intIP.part4 >= 0) && (ip_add.intIP.part4 <= 255)))
        return true;
    } else
      return false;
  } else if (ip_add.intIP.part1 == TYPE_C) {
    if ((ip_add.intIP.part2) == 168) {
      if (((ip_add.intIP.part3 >= 0) && (ip_add.intIP.part3 <= 255)) && ((ip_add.intIP.part4 >= 0) && (ip_add.intIP.part4 <= 255)))
        return true;
    } else
      return false;
  } else
    return false;

  return false;
}
*/


void ConnectionControlManager::ClearRules() { // champion : to clear rules (navneet)
  if (policy::ConnectionControlManager::rulearray)
    policy::ConnectionControlManager::rulearray->clear();
  if (policy::ConnectionControlManager::IsEnableVector)
    policy::ConnectionControlManager::IsEnableVector->clear();
}

void ConnectionControlManager::AddRule(std::string rule, bool enabled) {
  policy::ConnectionControlManager::rulearray->push_back(rule);
  policy::ConnectionControlManager::IsEnableVector->push_back(enabled);
}

void ConnectionControlManager::EditRule(std::string rule, bool enabled, int index) {
  policy::ConnectionControlManager::rulearray->at(index - 1) = rule;
  policy::ConnectionControlManager::IsEnableVector->at(index - 1) = enabled;
}

void ConnectionControlManager::DeleteRule(int index) {
  policy::ConnectionControlManager::rulearray->erase(policy::ConnectionControlManager::rulearray->begin() + (index - 1));
  policy::ConnectionControlManager::IsEnableVector->erase(policy::ConnectionControlManager::IsEnableVector->begin() + (index - 1));
}

void ConnectionControlManager::EnableRule(int index) {
  policy::ConnectionControlManager::IsEnableVector->at(index - 1) = true;
}

void ConnectionControlManager::DisableRule(int index) {
  policy::ConnectionControlManager::IsEnableVector->at(index - 1) = false;
}

size_t ConnectionControlManager::GetRuleArraySize() {
  return policy::ConnectionControlManager::rulearray->size();
}

bool ConnectionControlManager::IsRuleEnabled(int index) {
  return policy::ConnectionControlManager::IsEnableVector->at(index);
}

std::string ConnectionControlManager::GetRuleAtIndex(int index) {
  return policy::ConnectionControlManager::rulearray->at(index);
}

RULEParser::Rule::Protocol ConnectionControlManager::GetProtocalTag(const GURL& url) const {
	RULEParser::Rule::Protocol tempProtocol;
    if(!url.scheme().compare("HTTP"))
      tempProtocol = RULEParser::Rule::Protocol::HTTP;
    else if (!url.scheme().compare("HTTPS"))
      tempProtocol = RULEParser::Rule::Protocol::HTTPS;
    else if (!url.scheme().compare("FTP"))
      tempProtocol = RULEParser::Rule::Protocol::FTP;
    else if (!url.scheme().compare("WS")) 
      tempProtocol = RULEParser::Rule::Protocol::WS; 
    else if (!url.scheme().compare("WSS")) 
      tempProtocol = RULEParser::Rule::Protocol::WSS;
    else if (!url.scheme().compare("GOPHER")) 
      tempProtocol = RULEParser::Rule::Protocol::GOPHER; 
    else if (!url.scheme().compare("FILE")) 
      tempProtocol = RULEParser::Rule::Protocol::FILE; 
    else // to handle default case
      tempProtocol = RULEParser::Rule::Protocol::HTTP;
	return  tempProtocol; 
}
bool ConnectionControlManager::IsUrlBlocked(const GURL& url) {
std::string spec(url.spec());
  int length = spec.length();
  if (policy::ConnectionControlManager::bUpdateNeeded)
	LoadRules();
  policy::ConnectionControlManager::bUpdateNeeded = false;	
  if (length >= 9 && !spec.compare((length - 9), 9, ":Error138")) {
    LoadInDefaultBrowser(url, length);
    return true;
  }
  if (ISChamURLBlocked(url)) {
    if (policy::ConnectionControlManager::bLoadInDefault) {
      LoadInDefaultBrowser(url, length);
    }
    return true;
  }
return false;
}
bool ConnectionControlManager::ISChamURLBlocked(const GURL& url) const {
  /* Champion - Load Blocked URLs in default browser */
    std::string forbookmark="aviator";
    std::string chromeinternal="chrome";
    std::string bookmarkmagaer="chrome-extension";

	if(url.SchemeIs(chromeinternal.c_str()) || url.SchemeIs(bookmarkmagaer.c_str()) 
		|| url.SchemeIs(forbookmark.c_str()))
	  return false;

	bool IsIP = url.HostIsIPAddress();
    RULEParser::IP UrlIP;
    
	std::string host(url.host());
	int int_port = url.EffectiveIntPort();
	bool IsUserURLPublic = false;

	if (!(host.compare("plus.google.com"))) // Don't allow plus.google.com host to detect private network function as this host response is very slow and affects other TABs loading.
		IsUserURLPublic = true;
	else
		IsUserURLPublic = !RULEParser::detect_private_network(host, &UrlIP); // return TRUE for the private networks else FALSE for Public
	  
	if (!IsUserURLPublic)
	{
		if ((RULEParser::Rule::GetIsBlockPrivate()) && URLExistsInRuleList(url, UrlIP, IsIP, IsUserURLPublic, int_port, allowprivate))
			return false; // Since this URL is available in private allow list, we are allowing this URL
		if (RULEParser::Rule::GetIsBlockPrivate())
			return true;
		if (URLExistsInRuleList(url, UrlIP, IsIP, IsUserURLPublic, int_port, denyprivate)) 
			return true;
	}
	else
	{
		if ((RULEParser::Rule::GetIsBlockPublic()) && URLExistsInRuleList(url, UrlIP, IsIP, IsUserURLPublic, int_port, allowpublic))
			return false; // Since this URL is available in public allow list, we are allowing this URL
		if (RULEParser::Rule::GetIsBlockPublic())
			return true;
		if (URLExistsInRuleList(url, UrlIP, IsIP, IsUserURLPublic, int_port, denypublic)) 
			return true;
	}
  /* match the URL entered by the user - ends */
  return false;
}

bool policy::ConnectionControlManager::URLExistsInRuleList(const GURL& url, RULEParser::IP ip, bool HostIsIP, bool IsPublilc, int port, RuleListType type) const
{	
    RuleObjList ::const_iterator k;
	
	switch (type)
	{
	case allowprivate:
		//ruleobjlist = AllowPrivateRuleList;
	for (k = AllowPrivateRuleList.begin(); k != AllowPrivateRuleList.end(); k++) {
		RULEParser::Rule ruleobj(*k);
		if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj))
			return true;
	}		
		break;
	case denyprivate:
//		ruleobjlist = DenyPrivateRuleList;
	for (k = DenyPrivateRuleList.begin(); k != DenyPrivateRuleList.end(); k++) {
		RULEParser::Rule ruleobj(*k);
		if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj))
			return true;
	}		
		break;
	case allowpublic:
//		ruleobjlist = AllowPublicRuleList;
	for (k =AllowPublicRuleList.begin(); k != AllowPublicRuleList.end(); k++) {
		RULEParser::Rule ruleobj(*k);
		if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj))
			return true;
	}		
		break;
	case denypublic:
//		ruleobjlist = DenyPublicRuleList;
	for (k = DenyPublicRuleList.begin(); k != DenyPublicRuleList.end(); k++) {
		RULEParser::Rule ruleobj(*k);
		if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj))
			return true;
	}		
		break;
	}

	return false;
}

bool ConnectionControlManager::MatchPort(int portnum, RULEParser::Rule ruleobj) const {
  std::vector<int> ::iterator k;
  for (k = ruleobj.GetPorts().begin(); k != ruleobj.GetPorts().end(); k++) {
    if (*k == portnum)
      return true;
  }
  return false;
}

// Compare the current browsing URL with Rule Object 
bool ConnectionControlManager::CompareURLwithRuleObj(const GURL& url, RULEParser::IP ip, bool HostIsIP, bool IsPublilc, int port, RULEParser::Rule ruleobj) const {
	RULEParser::Rule::Protocol pTag = GetProtocalTag(url);
    if (pTag != ruleobj.GetProtocol()) 
      return false;
	if (HostIsIP || ruleobj.IsDomainIP()) 
	{
	 if (ip != ruleobj.GetIP())
	    return false;
	}
    else 
	{
	if (!url.DomainCompare(ruleobj.GetDomain().c_str()))
		return false;
	}		
	if (!MatchPort(port, ruleobj))
         return false;
    return true; // URL matches with Rule object
}
/*
void ConnectionControlManager::updateRule(int opr, unsigned index, string Rule) {
  switch (opr) {
  case 1: // Add rule
    policy::ConnectionControlManager::rulearray->push_back(Rule);
    break;
  case 2: // Edit
    policy::ConnectionControlManager::rulearray->at(index) = Rule;
    break;
  case 3: // delete
    std::vector<std::string> ::iterator it;
    for (it = policy::ConnectionControlManager::rulearray->begin(); it != policy::ConnectionControlManager::rulearray->end(); it++) {
      if (index == it->npos)
        policy::ConnectionControlManager::rulearray->erase(it);
    }
    break;
  }
}

void ConnectionControlManager::updateEnable(int opr, unsigned index, bool bVal) {
  switch (opr) {
  case 1: // Add editable or not
    policy::ConnectionControlManager::IsEnableVector->push_back(bVal);
    break;
  case 2: // Edit
    policy::ConnectionControlManager::IsEnableVector->at(index) = bVal;
    break;
  case 3: // delete
    policy::ConnectionControlManager::IsEnableVector->erase(policy::ConnectionControlManager::IsEnableVector->begin() + index);
    break;
  }
}
*/

void ConnectionControlManager::LoadRules() {
  std::string temp;

bool IsMovedToPref = false; 
#if defined(OS_WIN) 
  IsMovedToPref = pref_service_->GetBoolean(ABF::kRulesMovedToPref);
#endif
#if defined(OS_MACOSX) 
  if (pref_service_->IsManagedPreference(prefs::kRulesMovedToPref))
 	 IsMovedToPref = pref_service_->GetBoolean(prefs::kRulesMovedToPref);
#endif
  if (IsMovedToPref) { 
	LoadFromPref();
  }
  else{
    // unpack RULELIST_EDIT from varsconfig.h. This happens when the browser is installed first time
   temp = RULELIST_EDIT;
   std::vector<std::string> strs;
   strs.clear();
#if defined(OS_WIN) 
    pref_service_->SetBoolean(ABF::kRulesMovedToPref, true); // champion
#endif
#if defined(OS_MACOSX) 
    pref_service_->SetBoolean(prefs::kRulesMovedToPref, true);
#endif  
    DecomposeRules(temp, &strs);
    AddToRuleArray(&strs);
    SplitAndLoad(strs);
    // champion : to add rules to pref file for first launch start (navneet)
  
  }
}

void ConnectionControlManager::DecomposeRules(std::string temp, std::vector<std::string> *strs) {
  size_t pos = temp.find('|');
  size_t initialPos = 0;

  strs->clear();
  if (temp.length() > 0) {
    // Decompose statement 
    while (pos != std::string::npos) {
      strs->push_back(temp.substr(initialPos, pos - initialPos));
      initialPos = pos + 1;
      pos = temp.find('|', initialPos);
    }
    // Add the last one 
    size_t istrs = pos < temp.size() ? pos : temp.size();//std::min(pos,(int)temp.size());                 
    strs->push_back(temp.substr(initialPos, istrs - initialPos));//old
  }
}

void ConnectionControlManager ::AddToRuleArray(std::vector<std::string> *strs){

    base::ListValue rule_pref_list;
    std::vector<std::string> ::iterator itRuleList;
    int ruleCount = 0;
    base::StringValue* strValue;
    //  base::ListValue rule_pref_list;
    for (itRuleList = strs->begin(); itRuleList != strs->end(); itRuleList++) {
      std::string tempStr(*itRuleList);
      strValue = base::StringValue::CreateStringValue(tempStr);
      rule_pref_list.Set(ruleCount++, strValue);
      if (!tempStr.substr(0, 1).compare("0")){
        policy::ConnectionControlManager::AddRule(tempStr.substr(2), false);
      }
      else{
        policy::ConnectionControlManager::AddRule(tempStr.substr(2), true);
      }
    }


}
void ConnectionControlManager::SplitAndLoad(std::vector<std::string> strs) {
  bool bVal = false;
  std::vector<std::string> ::iterator i;
  
  AllowPrivateRuleList.clear();
  AllowPublicRuleList.clear();
  DenyPrivateRuleList.clear();
  DenyPublicRuleList.clear();
  
  for (i = strs.begin(); i != strs.end(); i++) {
    std::string tempStr(*i);
    if (!tempStr.substr(0, 1).compare("0"))
      bVal = false;
    else
      bVal = true; 
	std::string strtemp = tempStr.substr(2);
	RULEParser::Rule ruleobj(strtemp, RULEParser::Rule::Type::CONNECTIONCONTROL);  
    
    if(ruleobj.GetStatus() == RULEParser::Rule::Status::ALLOWED && ruleobj.GetPrivate()){
      AllowPrivateRuleList.push_back(ruleobj);
    }
    if(ruleobj.GetStatus() == RULEParser::Rule::Status::ALLOWED && !ruleobj.GetPrivate()){
       AllowPublicRuleList.push_back(ruleobj);
    }
    if(ruleobj.GetStatus() == RULEParser::Rule::Status::DENIED && ruleobj.GetPrivate()){
      DenyPrivateRuleList.push_back(ruleobj);
    }
    if(ruleobj.GetStatus() == RULEParser::Rule::Status::DENIED && !ruleobj.GetPrivate()){
      DenyPublicRuleList.push_back(ruleobj);
    }
  }
}

void ConnectionControlManager::LoadFromPref() {
  std::vector<std::string> strs_edit;
  strs_edit.clear();
#if defined(OS_MACOSX) 
    RuleListArray = pref_service_->GetList(prefs::kConnectionControlRules);
#endif
#if defined(OS_WIN) 
    RuleListArray = pref_service_->GetList(ABF::kConnectionControlRules);
#endif
	base::ListValue* RuleListArray_new = const_cast<base::ListValue*> (RuleListArray);
    if (RuleListArray) { // Champion: It should load rules for first run always. (Navneet)
   int nSize = RuleListArray->GetSize();
    for (int i = 0; i < nSize; i++) {
      base::Value* value = NULL;
      if (RuleListArray_new->Get(i, &value)) {
        std::string url_text;
        if (value->GetAsString(&url_text)) {
          strs_edit.push_back(url_text);
        }
      }
    }
    SplitAndLoad(strs_edit);
  }
}

void ConnectionControlManager::UpdateToPref() {

    base::ListValue rule_pref_list;
    std::vector<std::string> ::iterator itRuleList;
    int ruleCount = 0;
    base::StringValue* strValue;
for (itRuleList = policy::ConnectionControlManager::rulearray->begin(); itRuleList != policy::ConnectionControlManager::rulearray->end(); itRuleList++)
    {
      std::string tempStr(*itRuleList);
      strValue = base::StringValue::CreateStringValue(tempStr);
      rule_pref_list.Set(ruleCount++, strValue);
    }
   if (pref_service_){
#if defined(OS_WIN) 
    pref_service_->Set(ABF::kConnectionControlRules, rule_pref_list);
#endif
#if defined(OS_MACOSX) 
    pref_service_->Set(prefs::kConnectionControlRules, rule_pref_list);
#endif
   }
 }
} // namespace policy
