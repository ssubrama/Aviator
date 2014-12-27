// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.h"
#include "chrome/browser/championconfig/connectioncontrol/rule_parser.h" // champion : ntlm rule parser (amresh)
#include "chrome/browser/championconfig/varsconfig.h"
//#include "../chrome/grit/generated_resources.h"

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
//#include "chrome/browser/ui/webui/options/browser_options_handler.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"

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

using namespace std;

  policy::RuleObjList policy::ConnectionControlManager::AllowPrivateRuleList;
  policy::RuleObjList policy::ConnectionControlManager::AllowPublicRuleList;
  policy::RuleObjList policy::ConnectionControlManager::DenyPrivateRuleList;
  policy::RuleObjList policy::ConnectionControlManager::DenyPublicRuleList;
  
namespace policy {

ConnectionControlManager::ConnectionControlManager() {}

ConnectionControlManager::ConnectionControlManager(PrefService* pref_service) {
 pref_service_ = pref_service;
#if defined(OS_WIN) 
  pref_service_->SetBoolean(ABF::kNPCCRulesUpdated, true);
#endif

#if defined(OS_MACOSX) 
 pref_service_->SetBoolean(prefs::kNPCCRulesUpdated, true);
#endif
}

ConnectionControlManager::~ConnectionControlManager() {}

#if defined(OS_MACOSX)   
void ConnectionControlManager::LoadInDefaultBrowserMac(const GURL& url, int length) const {

    std::string m_stralternatebrowser_;	     
  	
  	m_stralternatebrowser_=pref_service_->GetString(prefs::kDisplayAlternateBrowser);
  
    if (m_stralternatebrowser_.compare("Google Chrome")==0)
      if (!IsGoogleChromeInstalled())
        m_stralternatebrowser_ = "Safari";
    
    if (m_stralternatebrowser_.compare("Firefox") == 0)
      if (!IsFirefoxInstalled())
        m_stralternatebrowser_ = "Safari";
          
		if (m_stralternatebrowser_.empty())
    		m_stralternatebrowser_ = "Safari";
    		
		std::string spec(url.spec());
    std::string geturl(url.host());
    bool bDonotBlock = false;
    char temp[1024]={'0'};
    int len=spec.size();
    length=spec.length();
    std ::vector<std ::string> myvector;
        
        myvector.push_back(std::string("translate.googleapis.com"));
        myvector.push_back(std::string("api.mixpanel.com"));
        myvector.push_back(std::string("mxr.mozilla.org"));
        myvector.push_back(std::string("trackalyzer.com"));
        myvector.push_back(std::string("secure.eloqua.com"));
        myvector.push_back(std::string("ssl.google-analytics.com")); 
        if (!spec.compare((length-9), 9, ":Error138"))
        {
        
            spec.copy(temp, length-10);
            temp[length]='\0';
            
        }
        else
        {
            spec.copy(temp, spec.size());
            temp[len] = '\0';
          
        }
        std::string callurl("open ");  
        std::string tempHost(temp);
        callurl=callurl+ tempHost;
	 	if (pref_service_->GetBoolean(prefs::kLoadInDefaultBrowser))
	 	 {
	 	   for(int i=0; i < (int)myvector.size(); i++)
		     if(url.DomainCompare(myvector[i].c_str()))
			 {
				bDonotBlock = true;
				break;
			 }
		 }
	
		 	
	if (!bDonotBlock)
	 {
		
		if(m_stralternatebrowser_=="Google Chrome")
			{
			   m_stralternatebrowser_.assign("Google");
			   m_stralternatebrowser_.append("\\ Chrome");
		   }
			
			// champion
		if (ShellIntegration::IsFirefoxDefaultBrowser()) 
			{
			  m_stralternatebrowser_.assign("firefox");
			}
			
		 	else if(ShellIntegration::IsSafariDefaultBrowser() ){
  				 m_stralternatebrowser_.assign("Safari");
			}
			else if(ShellIntegration::IsGoogleChromeDefaultBrowser()) {
			    m_stralternatebrowser_.clear();
   				m_stralternatebrowser_.assign("Google");
			    m_stralternatebrowser_.append("\\ Chrome");
			}			
			// champion
			std::string strsiteandbrowser;
			strsiteandbrowser.assign("open -a /Applications/");
	 		strsiteandbrowser.append(m_stralternatebrowser_);
			strsiteandbrowser.append(".app ");
			strsiteandbrowser.append(tempHost);			
			system(strsiteandbrowser.c_str());     
	}
} 

bool ConnectionControlManager::IsSafariInstalled() const {
  bool result = false;

   //champion : windows build manipulation (there is no popen in windows, its related to mac)
  char buffer[128] = "\0";
  FILE * f = popen("mdfind kMDItemCFBundleIdentifier == com.apple.Safari", "r");
  fgets(buffer, 128, f);
  if(buffer[0] != '\0')
    result = true;

  pclose(f); 


  return result;
}

bool ConnectionControlManager::IsGoogleChromeInstalled() const {
  bool result = false;
  //champion : windows build manipulation (there is no popen in windows, its related to mac)
  char buffer[128] = "\0";
  FILE * f = popen("mdfind kMDItemCFBundleIdentifier == com.google.Chrome", "r");
  fgets(buffer, 128, f);
  if(buffer[0] != '\0')
    result = true;
  pclose(f); 

  return result;
}

bool ConnectionControlManager::IsFirefoxInstalled() const {
  bool result = false;

  char buffer[128] = "\0";
  FILE * f = popen("mdfind kMDItemCFBundleIdentifier == org.mozilla.firefox", "r");
  fgets(buffer, 128, f);
  if(buffer[0] != '\0')
    result = true;
  pclose(f); 
 return result;
}
#endif

#if defined(OS_WIN) 
void ConnectionControlManager::LoadInDefaultBrowserWin(const GURL& url, int length) const {
  std::string urlValue = url.spec();
  std::size_t found = urlValue.find("::Error138");
  if (found != std::string::npos) {
    // champion : Added to stop multiple tabs in alternate browser when load by default is checked ( Navneet )
    urlValue = urlValue.erase(urlValue.length() - 10, urlValue.length() - 1);
    if (pref_service_->GetBoolean(ABF::kLoadInDefaultBrowser))     
      return;
  }
  std::string strCommand = "";

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
}
#endif

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
  
#if defined(OS_WIN) 
  if (pref_service_->GetBoolean(ABF::kNPCCRulesUpdated)) {
#endif

#if defined(OS_MACOSX) 
    if (pref_service_->GetBoolean(prefs::kNPCCRulesUpdated)) {
#endif
  //  ClearRules();
		LoadRules();
#if defined(OS_WIN) 
    pref_service_->SetBoolean(ABF::kNPCCRulesUpdated, false);
#endif
    
#if defined(OS_MACOSX) 
    pref_service_->SetBoolean(prefs::kNPCCRulesUpdated, false);
#endif
		}
//  policy::ConnectionControlManager::bUpdateNeeded = false;	
  if (length >= 9 && !spec.compare((length - 9), 9, ":Error138")) {
#if defined(OS_WIN)   
    LoadInDefaultBrowserWin(url, length);
#endif    
#if defined(OS_MACOSX) 
    LoadInDefaultBrowserMac(url, length);
#endif    
    return true;
  }
if (ISChamURLBlocked(url)) {
#if defined(OS_WIN) 
    if (pref_service_->GetBoolean(ABF::kLoadInDefaultBrowser)) {
         LoadInDefaultBrowserWin(url, length);
#endif

#if defined(OS_MACOSX) 
    if (pref_service_->GetBoolean(prefs::kLoadInDefaultBrowser)) {
         LoadInDefaultBrowserMac(url, length);
#endif
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
	  
	if (!IsUserURLPublic) // private URL
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
  	for (k = policy::ConnectionControlManager::AllowPrivateRuleList.begin(); k != policy::ConnectionControlManager::AllowPrivateRuleList.end(); k++) {
			RULEParser::Rule ruleobj(*k);
			if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj)) {
				return true;
				}
		}		
		break;
	case denyprivate:
		for (k = policy::ConnectionControlManager::DenyPrivateRuleList.begin(); k != policy::ConnectionControlManager::DenyPrivateRuleList.end(); k++) {
			RULEParser::Rule ruleobj(*k);
			if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj)) {
				return true;
				}
		}		
		break;
	case allowpublic:
		for (k =policy::ConnectionControlManager::AllowPublicRuleList.begin(); k != policy::ConnectionControlManager::AllowPublicRuleList.end(); k++) {
			RULEParser::Rule ruleobj(*k);
			if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj)) { 
				return true;
				}
		}		
		break;
	case denypublic:
		for (k = policy::ConnectionControlManager::DenyPublicRuleList.begin(); k != policy::ConnectionControlManager::DenyPublicRuleList.end(); k++) {
			RULEParser::Rule ruleobj(*k);
			if (CompareURLwithRuleObj(url, ip, HostIsIP, IsPublilc, port, ruleobj)) {
				return true;
				}
		}		
		break;
	}
	return false;
}

bool ConnectionControlManager::MatchPort(int portnum, RULEParser::Rule ruleobj) const {
  std::vector<int> ::iterator k;
  std::vector<int> tempPort;
  tempPort = ruleobj.GetPorts();
  for (k = tempPort.begin(); k != tempPort.end(); k++) {
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
    if (pTag == RULEParser::Rule::Protocol::FILE)
      return true; // Allow local files to open
	if (HostIsIP || ruleobj.IsDomainIP()) 
	{
	 if (ip != ruleobj.GetIP())
	    return false;
	}
  else 
	{
	if (!url.DomainCompare(ruleobj.GetDomain().c_str())) {
		return false;
		}
	}		
	if (!MatchPort(port, ruleobj))
         return false;
    return true; // URL matches with Rule object
}

void ConnectionControlManager::LoadRules() {
   std::string temp;

bool IsMovedToPref = false; 
#if defined(OS_WIN) 
  IsMovedToPref = pref_service_->GetBoolean(ABF::kRulesMovedToPref);
#endif
#if defined(OS_MACOSX) 
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
   DecomposeRules(temp, &strs);
   UpdatePrefsForNPCC(strs);
   SplitAndLoad(strs);

#if defined(OS_WIN) 
  pref_service_->SetBoolean(ABF::kRulesMovedToPref, true);
#endif
#if defined(OS_MACOSX) 
	 pref_service_->SetBoolean(prefs::kRulesMovedToPref, true);
#endif  
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

void ConnectionControlManager::SplitAndLoad(std::vector<std::string> strs) {
  bool bVal = false;
  std::vector<std::string> ::iterator i;
  
  RULEParser::Rule::SetIsBlockPrivate(false);
  RULEParser::Rule::SetIsBlockPublic(false);
  
  policy::ConnectionControlManager::AllowPrivateRuleList.clear();
  policy::ConnectionControlManager::AllowPublicRuleList.clear();
  policy::ConnectionControlManager::DenyPrivateRuleList.clear();
  policy::ConnectionControlManager::DenyPublicRuleList.clear();
  
 // ConnectionControlManager::IsPublicBlocked = false;
 // ConnectionControlManager::IsPrivateBlocked = false;
  
  for (i = strs.begin(); i != strs.end(); i++) {
    std::string tempStr(*i);
    if (!tempStr.substr(0, 1).compare("0"))
      bVal = false;
    else
      bVal = true;   
		std::string strtemp = tempStr.substr(2);

		RULEParser::Rule ruleobj(tempStr, RULEParser::Rule::Type::CONNECTIONCONTROL);
    
	  if(bVal && ruleobj.GetStatus() == RULEParser::Rule::Status::ALLOWED && ruleobj.GetPrivate())
  	    policy::ConnectionControlManager::AllowPrivateRuleList.push_back(ruleobj);
  	if (bVal && ruleobj.GetStatus() == RULEParser::Rule::Status::ALLOWED && !ruleobj.GetPrivate())
      policy::ConnectionControlManager::AllowPublicRuleList.push_back(ruleobj);
 	  if (bVal && ruleobj.GetStatus() == RULEParser::Rule::Status::DENIED && ruleobj.GetPrivate())
  	    policy::ConnectionControlManager::DenyPrivateRuleList.push_back(ruleobj);
    if (bVal && ruleobj.GetStatus() == RULEParser::Rule::Status::DENIED && !ruleobj.GetPrivate())
       policy::ConnectionControlManager::DenyPublicRuleList.push_back(ruleobj);
  }
//  ConnectionControlManager::IsPublicBlocked = RULEParser::Rule::GetIsBlockPublic(); 
//  ConnectionControlManager::IsPrivateBlocked =  RULEParser::Rule::GetIsBlockPrivate();
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
void ConnectionControlManager::UpdatePrefsForNPCC(std::vector<std::string> strs) {
  base::ListValue setruleList;
  std::string rulestr;
  base::StringValue* strValue;
  // bool enable = false;
  int i = 0;
  for (size_t j = 0; j < strs.size(); j++) {
    rulestr.clear();
    rulestr.append(strs.at(j));
    strValue = base::StringValue::CreateStringValue(rulestr);

    setruleList.Set(i, strValue);
    i++;
  }
#if defined(OS_MACOSX) 
    pref_service_->Set(prefs::kConnectionControlRules, setruleList);
#endif
#if defined(OS_WIN) 
   pref_service_->Set(ABF::kConnectionControlRules, setruleList);
#endif  

}
} // namespace policy
