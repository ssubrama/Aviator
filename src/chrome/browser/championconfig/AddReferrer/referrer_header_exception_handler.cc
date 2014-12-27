// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/championconfig/addreferrer/referrer_header_exception_handler.h"

#include <map>
#include <string>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/command_line.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/content_settings_details.h"
#include "chrome/browser/content_settings/content_settings_utils.h"
#include "chrome/browser/content_settings/host_content_settings_map.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "chrome/browser/notifications/desktop_notification_service.h"
#include "chrome/browser/notifications/desktop_notification_service_factory.h"
#include "chrome/browser/profiles/profile.h"
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
#include "grit/generated_resources.h"
#include "grit/locale_settings.h"
#include "ui/base/l10n/l10n_util.h"


ReferrerHeaderExceptionHandler::ReferrerHeaderExceptionHandler() {}

ReferrerHeaderExceptionHandler::~ReferrerHeaderExceptionHandler() {}

void ReferrerHeaderExceptionHandler::GetLocalizedValues(
    base::DictionaryValue* localized_strings) {
  DCHECK(localized_strings);

  static OptionsStringResource resources[] = {    
    // Referrer header Exception.
    { "referrerheaderExceptionPage", IDS_OPTIONS_REFERRER_HEADER_EXCEPTION },    
    { "referrerheaderExceptionsTitle", IDS_OPTIONS_REFERRER_HEADER_EXCEPTION_TAB }
	
  };

  RegisterStrings(localized_strings, resources, arraysize(resources));
  /*RegisterTitle(localized_strings, "referrerheaderExceptionPage",
                IDS_OPTIONS_REFERRER_HEADER_TITLE);*/
}

void ReferrerHeaderExceptionHandler::InitializePage() {
  /* notification_registrar_.Add(
      this, chrome::NOTIFICATION_PROFILE_CREATED,
      content::NotificationService::AllSources());
  notification_registrar_.Add(
      this, chrome::NOTIFICATION_PROFILE_DESTROYED,
      content::NotificationService::AllSources());

  UpdateHandlersEnabledRadios();
  UpdateAllExceptionsViewsFromModel();
  notification_registrar_.Add(
      this, chrome::NOTIFICATION_CONTENT_SETTINGS_CHANGED,
      content::NotificationService::AllSources());
  notification_registrar_.Add(
      this, chrome::NOTIFICATION_DESKTOP_NOTIFICATION_SETTINGS_CHANGED,
      content::NotificationService::AllSources());
  Profile* profile = Profile::FromWebUI(web_ui_);
  notification_registrar_.Add(
      this, chrome::NOTIFICATION_PROTOCOL_HANDLER_REGISTRY_CHANGED,
      content::Source<Profile>(profile));

  PrefService* prefs = profile->GetPrefs();
  pref_change_registrar_.Init(prefs);
  pref_change_registrar_.Add(prefs::kGeolocationContentSettings, this);*/
}
