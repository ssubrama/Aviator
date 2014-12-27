// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_REFERRER_HEADING_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_REFERRER_HEADING_HANDLER_H_
#pragma once

//#include "chrome/browser/plugin_data_remover_helper.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class HostContentSettingsMap;
class ProtocolHandlerRegistry;
class WebUI;

//class content::WebUI;
enum ReferrerSetting {
  REFERRER_SETTING_DEFAULT = 0,
  REFERRER_SETTING_ALLOW,
  REFERRER_SETTING_BLOCK,
  REFERRER_SETTING_ASK,          //FUTURE USE 
  REFERRER_SETTING_SESSION_ONLY, //FUTURE USE
  REFERRER_SETTING_NUM_SETTINGS  //FUTURE USE
};
 
namespace options  {
	
class ReferrerHeaderHandler : public options ::OptionsPageUIHandler,
                              public content::NotificationObserver {
 public:
  ReferrerHeaderHandler();
  virtual ~ReferrerHeaderHandler();

  // OptionsPageUIHandler implementation.
  virtual void GetLocalizedValues(base::DictionaryValue* localized_strings) OVERRIDE;

  //virtual void Initialize() OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void updateReferrerHeaderRadios();
  void setReferrerHeaderEnabled(const base::ListValue* args);
  // void setImportList(const ListValue* args);

  // Called to decide whether a given pattern is valid, or if it should be
  // rejected. Called while the user is editing an exception pattern.
  void CheckExceptionPatternValidity(const base::ListValue* args);

  // Changes the value of an exception. Called after the user is done editing an
  // exception.
  void SetException(const base::ListValue* args);

  // Removes the given row from the table. The first entry in |args| is the
  // content type, and the rest of the arguments depend on the content type
  // to be removed.
  void RemoveException(const base::ListValue* args);
  void UpdateNotificationExceptionsView();

  static bool GetReferrer(); // Returns current referrer status
  static bool GetReferrerHeader();

  //Added for purge_referrer
  static bool Allways_Or_Never;

  content::NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;
  std::vector<std::string> AllowVector ;
	std::vector<std::string> BlockVector;

  //End
  DISALLOW_COPY_AND_ASSIGN(ReferrerHeaderHandler);
};

} // namepsace options

#endif  // CHROME_BROWSER_UI_WEBUI_OPTIONS_CONTENT_SETTINGS_HANDLER_H_
