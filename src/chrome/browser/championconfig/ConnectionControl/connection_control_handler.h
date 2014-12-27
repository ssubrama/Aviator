// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_POLICY_CONNECTIO_CONTROL_HANDLER_H_
#define CHROME_BROWSER_POLICY_CONNECTIO_CONTROL_HANDLER_H_

#pragma once

#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "base/prefs/pref_change_registrar.h"  
#include "base/prefs/pref_member.h"
#include "base/prefs/pref_service.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "chrome/browser/championconfig/connectioncontrol/rule_parser.h" // champion : ntlm rule parser (amresh)

class HostContentSettingsMap;
class ProtocolHandlerRegistry;
class PrefRegistrySyncable;

namespace options { 
class ConnectionControlHandler : public options :: OptionsPageUIHandler,
                                 public content::NotificationObserver {

public:

  ConnectionControlHandler();
  virtual ~ConnectionControlHandler();

  // OptionsPageUIHandler implementation.
  virtual void GetLocalizedValues(base::DictionaryValue* localized_strings) OVERRIDE;

  //virtual void Initialize() OVERRIDE;
  
  virtual void InitializeHandler() OVERRIDE; 
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void updateRules();

  static void resetRulesToDefault(PrefService* prefService, base::ListValue &ruleList); // champion navneet test

  content::NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  void AddConnectionRules(const base:: ListValue* args);
  void EditConnectionRules(const base:: ListValue* args);
  void DeleteConnectionRules(const base :: ListValue* args);
  void InitLoadInDefaultBrowserCheckBox();
  void RuleEnabled(const base::ListValue* args);
  void RuleDisabled(const base::ListValue* args);
  void LoadBlockedURLInDefaultBrowser(const base:: ListValue* args);
  void SetLoadInDefaultBrowser(const base:: ListValue* args);
  static void RegisterUserPrefs(user_prefs::PrefRegistrySyncable* registry);
  void UpdatePrefsForNPCC();
  void InitBrowserList();
  void SetBlockedURLAlternateBrowserInPrefs(const base::ListValue* args);
  std::string GetBlockedURLAlternateBrowserInPrefs();
  void 	SetUpBlockedURLAlternateBrowser();
  bool IsSafariInstalled();
  bool IsGoogleChromeInstalled();
  bool IsFirefoxInstalled();
  
  /****************NTLM *****************************/
  void AddHttpNtlmRules(const base::ListValue* args);
  void EditHttpNtlmRules(const base::ListValue* args);
  void DeleteHttpNtlmRules(const base::ListValue* args);
  void updateNTLMRules();
  void NTLMRuleEnabled(const base::ListValue* args);
  void NTLMRuleDisabled(const base::ListValue* args);
  void UpdatePrefsNTLMList();
  bool LoadNTLMRules(const PrefService* prefServicePtr);
  RULEParser::RuleStore& GetRuleStore() { return ntlmRuleStore_; }
  /******************************************/
  		
private:
  /*Added by utpal*/
  const base::ListValue *total_connection_rulelist_;//contain all current sets of Rule 
  PrefService* pref_service_; //added by utpal for geting and setting peferences 
  //Once update i.e rule is getting update need to notify the observer 
  PrefChangeRegistrar connection_control_rule_registrar_;
  void CurrentRuleList(); //will contain all current rule list 
  /*End*/

  std::vector<std::string> rulearrayCCH_;
  
  struct RuleList {  
    bool editable;
    std::string rulestr;
    bool state;
  } rulelist;

  StringPrefMember alternate_browser_;
  /****************************NTLM**************************/
  PrefChangeRegistrar ntlm_control_rule_registrar_;
  void CurrentNtlmRuleList();

private:
  RULEParser::RuleStore ntlmRuleStore_;
  bool SetRuleAndReturnStatus(const base::ListValue* args, std::string* rule = NULL, size_t* index = NULL);
  /******************************/
  
  DISALLOW_COPY_AND_ASSIGN(ConnectionControlHandler);
};

} // namespace options

#endif //CHROME_BROWSER_POLICY_CONNECTIO_CONTROL_HANDLER_H_
