// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/connectioncontrol/connection_control_handler.h" // champion
#include "chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.h"                       // champion
#include "chrome/browser/championconfig/varsconfig.h" // champion navneet test

#include <map>
#include <string>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h" 
#include "base/command_line.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/notifications/desktop_notification_service.h"
#include "chrome/browser/notifications/desktop_notification_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "base/prefs/pref_service.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_pattern.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/user_metrics.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_source.h"
#include "content/public/browser/notification_types.h"
#include "content/public/common/content_switches.h"
#include "content/public/browser/web_ui.h"
#include "content/browser/web_contents/web_contents_view.h"
//#include "../chrome/grit/generated_resources.h"
//#include "../chrome/grit/locale_settings.h"
#include "ui/base/l10n/l10n_util.h"
#include "base/prefs/scoped_user_pref_update.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "base/file_util.h"
#include "chrome/common/importer/firefox_importer_utils.h"
#include "chrome/browser/championconfig/chromeimporter/chrome_importer_utils.h" // champion

#if defined(OS_WIN)
#include "chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h" // champion
#endif

typedef unsigned long DWORD;
using namespace std;

namespace options {
std::vector<std::string> rulelistUI() {
     CR_DEFINE_STATIC_LOCAL (std::vector<std::string>, rulelistUI, ());
      return rulelistUI;
}
  
ConnectionControlHandler::ConnectionControlHandler() {}

ConnectionControlHandler::~ConnectionControlHandler() {
  /*Added by utpal for confirm writing into preference file */
  //we are done going make null preference service pointer 
  if(pref_service_)
    pref_service_ = NULL;
  
  /*End of utpal*/
}

void ConnectionControlHandler::GetLocalizedValues(
    base::DictionaryValue* localized_strings) {
  DCHECK(localized_strings);
}

void ConnectionControlHandler ::InitLoadInDefaultBrowserCheckBox() {
Profile* profile = Profile::FromWebUI(web_ui());
bool bLoadInDefaultBrowser = false;
bLoadInDefaultBrowser = profile->GetPrefs()->GetBoolean(prefs::kLoadInDefaultBrowser);
  base::FundamentalValue handlers_enabled(bLoadInDefaultBrowser);
  web_ui()->CallJavascriptFunction("ConnectionControl.InitLoadInDefaultBrowser",handlers_enabled);
}

void ConnectionControlHandler::InitializeHandler() {
  Profile* profile = Profile::FromWebUI(web_ui());
  notification_registrar_.Add(
     this, chrome::NOTIFICATION_CONNECTION_CONTROL_HANDLER_CHANGED,
     content::Source<Profile>(profile));
}

void ConnectionControlHandler::InitializePage() {
  updateRules();
  InitLoadInDefaultBrowserCheckBox(); 
  InitBrowserList();
  /****************************NTLM***************************/
  LoadNTLMRules(pref_service_);
  updateNTLMRules();
}

bool ConnectionControlHandler::LoadNTLMRules(const PrefService* prefServicePtr) {
  if (!prefServicePtr)
    return false;

  const base::ListValue* total_ntlm_rulelist = prefServicePtr->GetList(prefs::kNtlmControlRules);
  if (!total_ntlm_rulelist)
    return false;

  RULEParser::RuleStrings allNtlmRules_;
  if (total_ntlm_rulelist->GetSize()) {
    std::string rule;
    for (size_t i = 0; i < total_ntlm_rulelist->GetSize(); i++) {
      total_ntlm_rulelist->GetString(i, &rule);
      allNtlmRules_.push_back(rule);
      rule.clear();
    }
    //put all the rules in RuleStore as a Rule object 
    ntlmRuleStore_.Reset(allNtlmRules_);
  }
  
  return true;
}

// champion: To reset connection control rules (Navneet)
void ConnectionControlHandler::resetRulesToDefault(PrefService* prefService, base::ListValue &rule_pref_list){
 // policy::ConnectionControlManager::ClearRules();
/*  std::vector<std::string> ruleListFromConfig;
  string rulesFromConfigFile(RULELIST_EDIT);
  policy::ConnectionControlManager::DecomposeRules(rulesFromConfigFile, &ruleListFromConfig);
  std::vector<std::string> ::iterator itRuleList;
  int ruleCount = 0;
  base::StringValue* strValue;
  //  base::ListValue rule_pref_list;
  for (itRuleList = ruleListFromConfig.begin(); itRuleList != ruleListFromConfig.end(); itRuleList++)
  {
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
  prefService->Set(prefs::kConnectionControlRules, rule_pref_list); */
  
#if defined(OS_WIN) 
    prefService->SetBoolean(ABF::kRulesMovedToPref, false); // champion
#endif
#if defined(OS_MACOSX) 
    prefService->SetBoolean(prefs::kRulesMovedToPref, false);
#endif  
    prefService->SetBoolean(prefs::kNPCCRulesUpdated, true);
/*  policy::ConnectionControlManager::LoadRules(); */
}

void ConnectionControlHandler::updateRules() {
  DCHECK(web_ui());
  std::vector<std::string> strs_edit;
  base::ListValue rule_list;
  base::StringValue* strValue;
  
  rulearrayCCH_.clear();
  
#if defined(OS_MACOSX) 
    const base::ListValue* RuleListArray = pref_service_->GetList(prefs::kConnectionControlRules);
#endif
#if defined(OS_WIN) 
    const base::ListValue* RuleListArray = pref_service_->GetList(ABF::kConnectionControlRules);
#endif
	base::ListValue* RuleListArray_new = const_cast<base::ListValue*> (RuleListArray);
    if (RuleListArray) { 
   int nSize = RuleListArray->GetSize();
    for (int i = 0; i < nSize; i++) {
      base::Value* value = NULL;
      if (RuleListArray_new->Get(i, &value)) {
        std::string url_text;
        if (value->GetAsString(&url_text)) {
		    strValue = base::StringValue::CreateStringValue(url_text);
		    rule_list.Insert(i, strValue);
     		rulearrayCCH_.push_back(url_text);
        }
      }
    }
  }
  web_ui()->CallJavascriptFunction("ConnectionControl.updateRules", rule_list);
}

void ConnectionControlHandler::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
   switch (type) {
    
    case chrome::NOTIFICATION_PREF_CHANGED: {
      std::string* pref_name =content::Details<std::string>(details).ptr();
      if (*pref_name == prefs::kDisplayAlternateBrowser) 
           SetUpBlockedURLAlternateBrowser();
      break;
    }

    case chrome::NOTIFICATION_REFERRER_HEADER_REGISTRY_CHANGED: {
      updateRules();
      break;
    }

    // champion : removed on balaji suggestion (amresh)
    // Observe no more available in OptionsPageUIHandler
    /*default:
      OptionsPageUIHandler::Observe(type, source, details);*/
  }
}

void ConnectionControlHandler::RegisterMessages() {
  PrefService* pref_service1 = Profile::FromWebUI(web_ui())->GetPrefs();
  pref_service_ =  pref_service1;
  alternate_browser_.Init(prefs::kDisplayAlternateBrowser, pref_service1);//, this); //++ champion_ab

  web_ui()->RegisterMessageCallback("AddConnectionRules",
    base::Bind(&ConnectionControlHandler::AddConnectionRules,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("EditConnectionRules",
    base::Bind(&ConnectionControlHandler::EditConnectionRules,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("DeleteConnectionRules",
    base::Bind(&ConnectionControlHandler::DeleteConnectionRules,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("RuleEnabled",
    base::Bind(&ConnectionControlHandler::RuleEnabled,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("RuleDisabled",
    base::Bind(&ConnectionControlHandler::RuleDisabled,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("SetLoadInDefaultBrowser",
    base::Bind(&ConnectionControlHandler::SetLoadInDefaultBrowser,
    base::Unretained(this)));
  web_ui()->RegisterMessageCallback("setBlockedURLAlternateBrowserInPrefs",
    base::Bind(&ConnectionControlHandler::SetBlockedURLAlternateBrowserInPrefs,
    base::Unretained(this)));

   /***********************************NTLM **********************************/
    web_ui()->RegisterMessageCallback("AddHttpNtlmRules",
      base::Bind(&ConnectionControlHandler::AddHttpNtlmRules,
      base::Unretained(this)));
    web_ui()->RegisterMessageCallback("EditHttpNtlmRules",
      base::Bind(&ConnectionControlHandler::EditHttpNtlmRules,
      base::Unretained(this)));
    web_ui()->RegisterMessageCallback("DeleteHttpNtlmRules",
      base::Bind(&ConnectionControlHandler::DeleteHttpNtlmRules,
      base::Unretained(this)));
    web_ui()->RegisterMessageCallback("NTLMRuleEnabled",
      base::Bind(&ConnectionControlHandler::NTLMRuleEnabled,
      base::Unretained(this)));
    web_ui()->RegisterMessageCallback("NTLMRuleDisabled",
      base::Bind(&ConnectionControlHandler::NTLMRuleDisabled,
      base::Unretained(this)));
}

void ConnectionControlHandler:: CurrentRuleList() {}

void ConnectionControlHandler::AddConnectionRules(const base::ListValue* args) {
  /*Need to update the code 
  1.why should we add rule to urlBlocklist manager if so then why connection_control_handler is required 

  1.Add list as member variable of this class 
  2.Get the all list of rule from preference service at initialization using pref_services_->GetList(prefs::kConnectionControlRules).
  3.At any point if we modify/change the rule list use same member variable list 
  4.call UpdatePrefsForNPCC 
  5.notify urlBlocklistManager that rule list has been change reload the list.
    a) for notification derive URLBlacklistManager class from NotificationObserver 
       for override observer method
    b) take member variable of PrefChangeRegistrar registrar URLBlacklistManager. 
    c) in constructer call below two function 
        connection_control_rule_registrar_.Init(pref_service_);
        connection_control_rule_registrar_.Add(prefs::kConnectionControlRules,this);
    d) once any change happen to this prefs::kConnectionControlRules preference it will triger call urlBlocklistManager observer overridden function   
  */

  // Add rule     enable;rule --> 2 args.
  //int nSize = 0;  
  std::string rule;
  std::string tempStr;
  bool enabled = false;
  //bool editable = true;

  CHECK(args->GetBoolean(0, &enabled));
  CHECK(args->GetString(1, &rule));//  >GetList(0, &rulelist));
  if (enabled)
    tempStr.append("1;");
  else
    tempStr.append("0;");
  tempStr.append(rule);
  rulearrayCCH_.push_back(tempStr);

  UpdatePrefsForNPCC();
}

void ConnectionControlHandler::EditConnectionRules(const base::ListValue* args) {
  // Edit rule    enable;rule;index --> size of args 3.
  // int nSize = 0;

  double action = 0;
  int index = 0;
  std::string rule;
  std::string tempStr;
  bool enabled = false;

  CHECK(args->GetBoolean(0, &enabled));
  CHECK(args->GetDouble(1, &action));
  CHECK(args->GetString(2, &rule));//  >GetList(0, &rulelist));
  index = static_cast <int> (action);
//  policy::ConnectionControlManager::EditRule(rule,enabled,index);

  if (enabled)
    tempStr.append("1;");
  else
    tempStr.append("0;");
  tempStr.append(rule);
//  rulearrayCCH_.push_back(tempStr);
  rulearrayCCH_.at(index - 1) = tempStr;
  UpdatePrefsForNPCC();
}

void ConnectionControlHandler::DeleteConnectionRules(const base::ListValue* args) {
  double action = 0;
  unsigned index = 0;
  std::string rule;
  
  CHECK(args->GetDouble(0, &action));
  index = static_cast <int> (action);
  std::vector<std::string> :: iterator i; 
  std::vector<bool> :: iterator k;
  //policy::ConnectionControlManager::DeleteRule(index);

  rulearrayCCH_.erase(rulearrayCCH_.begin() + (index - 1));

  UpdatePrefsForNPCC();
}

void ConnectionControlHandler::RuleEnabled(const base::ListValue* args) {
  double action = 0;
  unsigned index = 0;
  std::string rule;
  std::string tempStr;
  
  CHECK(args->GetDouble(0, &action));
  index = static_cast <int> (action);
  
  tempStr = rulearrayCCH_.at(index - 1);
  rule.append("1;");
  rule.append(tempStr.substr(2));
  rulearrayCCH_.at(index - 1) = rule;
   
  UpdatePrefsForNPCC();
}

void ConnectionControlHandler::RuleDisabled(const base::ListValue* args) {
  // index --> size of args 1.

  std::string rule;
  std::string tempStr;
  
  double action = 0;
  unsigned index = 0;
  
  CHECK(args->GetDouble(0, &action));
  index = static_cast <int> (action);
  tempStr = rulearrayCCH_.at(index - 1);
  rule.append("0;");
  rule.append(tempStr.substr(2));
  rulearrayCCH_.at(index - 1) = rule;  
   
  UpdatePrefsForNPCC();
}

void ConnectionControlHandler::UpdatePrefsForNPCC() {
  DCHECK(web_ui());
  Profile* profile = Profile::FromWebUI(web_ui());
  
  ListPrefUpdate update(profile->GetPrefs(), prefs::kConnectionControlRules);
  base::ListValue* rule_pref_list = update.Get();
  DCHECK(rule_pref_list);
  if (rule_pref_list)
    rule_pref_list->Clear();
  std::string rulestr;
  base::StringValue* strValue;
  int i = 0;
  for (size_t j = 0; j < rulearrayCCH_.size(); j++) {
    rulestr.clear();
    rulestr.append(rulearrayCCH_.at(j));
    strValue = base::StringValue::CreateStringValue(rulestr); 
    rule_pref_list->Set(i, strValue);
    i++;
  }
  base::ListValue setruleList;
  setruleList.Swap(rule_pref_list);
  pref_service_->Set(prefs::kConnectionControlRules, setruleList);
  pref_service_->CommitPendingWrite();
  pref_service_->SetBoolean(prefs::kNPCCRulesUpdated, true);
}

void ConnectionControlHandler::RegisterUserPrefs(user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterListPref(prefs::kConnectionControlRules,
    user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
   registry->RegisterBooleanPref(prefs::kRulesMovedToPref,
    false,
     user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF); //champion 
  registry->RegisterBooleanPref(prefs::kLoadInDefaultBrowser,
    false,
    user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterStringPref(prefs::kDisplayAlternateBrowser,
    "",
    user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);  
  /**NTLM */
  registry->RegisterListPref(prefs::kNtlmControlRules,
    user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  // champion : moved from connection control manager to here
  registry->RegisterBooleanPref(prefs::kNPCCRulesUpdated,
   false,
  user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF); //champion 
}

void ConnectionControlHandler :: SetLoadInDefaultBrowser(const base::ListValue* args) {
  bool enabled =false;
  CHECK(args->GetBoolean(0,&enabled));
//  policy::ConnectionControlManager::bLoadInDefault = enabled;
  DCHECK(web_ui());
  Profile* profile = Profile::FromWebUI(web_ui());
  scoped_ptr<base::Value> value(base::Value :: CreateBooleanValue(enabled));
  profile->GetPrefs()->Set(prefs :: kLoadInDefaultBrowser,*value);
}

// champion : added for file check (amresh)
bool FileExists (const std::string& name) {
  ifstream f(name.c_str());
  if (f.good()) {
    f.close();
    return true;
  } else {
    f.close();
    return false;
  }   
}

// champion : added for dir check (amresh)
bool DirExists(const string& dirName_in) {
#if defined(OS_WIN)
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!
#endif
  return false;    // this is not a directory!
}

/*code for alternate browser for blocked urls start*/
void ConnectionControlHandler::InitBrowserList() {
  
  base::ListValue browser_list;
  std::string strBrowser;
  base::StringValue* strValue;
 // base::FilePath Ischromeinstalled = GetAppInstallPath(); //Champion - Dinesh
  long i= 0;
  browser_list.Clear();
  
#if defined(OS_MACOSX)
  base::FilePath Ischromeinstalled = GetAppInstallPath(); //Champion - Dinesh
  base::FilePath IsFirefoxAppinstalled = GetFirefoxAppInstallPath(); 
  base::FilePath IsSafariAppinstalled = GetSafariAppInstallPath();
  if(IsSafariInstalled()) {    
    strBrowser.assign("Safari");
    strValue = base::StringValue::CreateStringValue(strBrowser);
  }
  
  if (!Ischromeinstalled.empty()) { //champion - Dinesh 
    strBrowser.assign("Google Chrome");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }

  if (!IsFirefoxAppinstalled.empty()) {  
    strBrowser.assign("Mozilla Firefox");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }
  
  if (!IsSafariAppinstalled.empty()) {     
    strBrowser.assign("Safari");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }
#endif

#if defined(OS_WIN)
  std::string strProgramPath = "C:";
  std::string strCheckFolderFor64bitWin = "C:\\Program Files (x86)";
  if (DirExists (strCheckFolderFor64bitWin))
    strProgramPath += "\\Program Files (x86)\\";
  else
    strProgramPath += "\\Program Files\\";
  std::string strIEPath = strProgramPath + "Internet Explorer\\iexplore.exe";
  std::string strFFPath = strProgramPath + "Mozilla Firefox\\firefox.exe";
  std::string strGCPath = strProgramPath + "Google\\Chrome\\Application\\chrome.exe";
  //champion - check install path in HK_CURRENT_USER directory in REGEDIT - Dinesh
  base::FilePath strFireFoxLPath = GetFirefoxInstallPathFromRegistry();
  base::FilePath strChromeLPath =  GetChromeInstallPathFromRegistry();
  if (FileExists (strIEPath)) {
    strBrowser.assign("Internet Explorer");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }//champion - fixed for by defaut opening IE but showing firefox - Navneet
  if (FileExists (strFFPath) || !strFireFoxLPath.empty()) {
    strBrowser.assign("Firefox");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }
  
  if (FileExists (strGCPath) || !strChromeLPath.empty()) {
    strBrowser.assign("Google Chrome");
    strValue = base::StringValue::CreateStringValue(strBrowser);
    browser_list.Insert(i, strValue);
    i++;
  }


#endif
  
  scoped_ptr<base::Value> strselectedbrowservalue(base::Value::CreateStringValue(GetBlockedURLAlternateBrowserInPrefs()));
  web_ui()->CallJavascriptFunction("ConnectionControl.InitBrowserList",browser_list,*strselectedbrowservalue);
  web_ui()->CallJavascriptFunction("AlternateBrowser.InitBrowserList",browser_list,*strselectedbrowservalue);
}

#if defined(OS_MACOSX)
bool ConnectionControlHandler::IsSafariInstalled() {
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

bool ConnectionControlHandler::IsGoogleChromeInstalled() {
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

bool ConnectionControlHandler::IsFirefoxInstalled() {
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
void ConnectionControlHandler::SetBlockedURLAlternateBrowserInPrefs(const base::ListValue* args) {
  std::string strSelectedBrowser;
  CHECK(args->GetString(0, &strSelectedBrowser));

  DCHECK(web_ui());
  Profile* profile = Profile::FromWebUI(web_ui());  
  scoped_ptr<base::Value> value(base::Value::CreateStringValue(strSelectedBrowser));
  profile->GetPrefs()->Set(prefs::kDisplayAlternateBrowser, *value);
}

std::string ConnectionControlHandler::GetBlockedURLAlternateBrowserInPrefs() {
  DCHECK(web_ui());
  Profile* profile = Profile::FromWebUI(web_ui());  
  std::string strAlternateBrowserInPrefs=profile->GetPrefs()->GetString(prefs::kDisplayAlternateBrowser);

  return strAlternateBrowserInPrefs;
}

void ConnectionControlHandler::SetUpBlockedURLAlternateBrowser() {
  PrefService* pref_service = Profile::FromWebUI(web_ui())->GetPrefs();
  alternate_browser_.Init(prefs::kDisplayAlternateBrowser, pref_service);//, this); //++ champion_ab
  base::StringValue alternate_browser_value(alternate_browser_.GetValue());
  web_ui()->CallJavascriptFunction("AlternateBrowser.setUpBlockedURLAlternateBrowser", alternate_browser_value);
}

void ConnectionControlHandler::updateNTLMRules() {
  DCHECK(web_ui());
  base::ListValue rule_list;
  base::StringValue* strValue;
  for (size_t j = 0; j < ntlmRuleStore_.Size(); j++) {
    const RULEParser::Rule* rule = ntlmRuleStore_[j];
    strValue = base::StringValue::CreateStringValue(rule->ToString());
    rule_list.Insert(j, strValue);
  }
  web_ui()->CallJavascriptFunction("ConnectionControl.updateNtlmURL", rule_list);
}

bool ConnectionControlHandler::SetRuleAndReturnStatus(const base::ListValue* args, std::string* rule, size_t* index) {
  if (!args)
    return false;

  size_t listSize = args->GetSize();
  if (!listSize)
    return false;

  bool isEnabled = false;
  double ruleIndex = -1;

  CHECK(args->GetBoolean(0, &isEnabled));
  if (index) {
    CHECK(args->GetDouble(listSize - 2, &ruleIndex));
    *index = (size_t)(ruleIndex - 1);
  }
  if (rule) {
    CHECK(args->GetString(listSize - 1, rule));
    if (isEnabled)
      *rule = "1;" + *rule;
    else
      *rule = "0;" + *rule;
  }

  return isEnabled;
}

void ConnectionControlHandler::AddHttpNtlmRules(const base::ListValue* args) {
  std::string rule;
  SetRuleAndReturnStatus(args, &rule);
  ntlmRuleStore_.Add(rule);
  UpdatePrefsNTLMList();
}

void ConnectionControlHandler::EditHttpNtlmRules(const base::ListValue* args) {
  std::string rule;
  size_t index = -1;
  SetRuleAndReturnStatus(args, &rule, &index);
  bool isRemoved = ntlmRuleStore_.Remove(index);
  if (isRemoved){
    if (index == ntlmRuleStore_.Size())
      ntlmRuleStore_.Add(rule);
    else
      ntlmRuleStore_.Insert(index, rule);
    UpdatePrefsNTLMList();
  }
}

void ConnectionControlHandler::DeleteHttpNtlmRules(const base::ListValue* args) {
  size_t index = -1;
  SetRuleAndReturnStatus(args, NULL, &index);
  ntlmRuleStore_.Remove(index);
  UpdatePrefsNTLMList();
}

void ConnectionControlHandler::UpdatePrefsNTLMList() {
  Profile* profile = Profile::FromWebUI(web_ui());
  
  ListPrefUpdate update(profile->GetPrefs(), prefs::kNtlmControlRules);
  base::ListValue* rule_pref_list = update.Get();

  DCHECK(rule_pref_list);
  if (rule_pref_list)
    rule_pref_list->Clear();
  std::string rulestr;
  base::StringValue* strValue;
  for (size_t j = 0; j < ntlmRuleStore_.Size(); j++){
    const RULEParser::Rule* rule = ntlmRuleStore_[j];
    strValue = base::StringValue::CreateStringValue(rule->ToString());
    rule_pref_list->Set(j, strValue);
  }
  base::ListValue setruleList;
  setruleList.Swap(rule_pref_list);
  pref_service_->Set(prefs::kNtlmControlRules, setruleList);
  //below call ensure write into preference file
  pref_service_->CommitPendingWrite();
}

void ConnectionControlHandler::NTLMRuleEnabled(const base::ListValue* args) {
  size_t index = -1;
  SetRuleAndReturnStatus(args, NULL, &index);
  RULEParser::Rule* rule = ntlmRuleStore_[index];
  if (rule) {
    rule->Edit(RULEParser::Rule::ENABLED);
    ntlmRuleStore_.IndexRules();
    UpdatePrefsNTLMList();
  }
}

void ConnectionControlHandler::NTLMRuleDisabled(const base::ListValue* args) {
  size_t index = -1;
  SetRuleAndReturnStatus(args, NULL, &index);
  RULEParser::Rule* rule = ntlmRuleStore_[index];
  if (rule) {
    rule->Edit(RULEParser::Rule::DISABLED);
    ntlmRuleStore_.IndexRules();
    UpdatePrefsNTLMList();
  }
}

} // namepsace options
/*code for alternate browser for blocked urls end*/
