// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/championconfig/addreferrer/referrer_header_handler.h"

#include <map>
#include <string>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/command_line.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/ui/webui/options/content_settings_handler.h"
#include "chrome/browser/content_settings/content_settings_details.h"
#include "chrome/browser/content_settings/content_settings_utils.h"
#include "chrome/browser/content_settings/host_content_settings_map.h"
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
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_source.h"
#include "content/public/browser/notification_types.h"
#include "content/public/common/content_switches.h"
#include "grit/generated_resources.h"
#include "grit/locale_settings.h"
#include "ui/base/l10n/l10n_util.h"
#include "../../../../../net/http/http_request_headers.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui.h"
#include <stdio.h>

using namespace std;
using namespace base;

bool options::ReferrerHeaderHandler::Allways_Or_Never = false;

bool options::ReferrerHeaderHandler::GetReferrer() {
	return options::ReferrerHeaderHandler::Allways_Or_Never;
}

namespace options {

ReferrerHeaderHandler::ReferrerHeaderHandler() {}
 
ReferrerHeaderHandler::~ReferrerHeaderHandler() {}

void ReferrerHeaderHandler::GetLocalizedValues(
  base::DictionaryValue* localized_strings) {
  DCHECK(localized_strings);

  // Referrer header.
  static OptionsStringResource resources[] = {    
     { "referrerHeader_tab_label", IDS_OPTIONS_REFERRER_HEADER_TAB },    
     { "referrerHeader_always_send", IDS_OPTIONS_REFERRER_HEADER_TAB_OPTION_ALWAYS },
     { "referrerHeader_never_send", IDS_OPTIONS_REFERRER_HEADER_TAB_OPTION_NEVER },
     { "ManageReferrerExceptionsButton", IDS_OPTIONS_REFERRER_HEADER_TAB_MANAGE_BUTTON }	
  };

  RegisterStrings(localized_strings, resources, arraysize(resources));
  RegisterTitle(localized_strings, "referrerHeaderPage",
                IDS_OPTIONS_REFERRER_HEADER_TITLE);

  // Register titles for each of the individual settings whose exception
  // dialogs will be processed by |ReferrerHeadersHandler|.
  RegisterTitle(localized_strings, "referrerHeader",
                IDS_OPTIONS_REFERRER_HEADER_TAB);
} 

void ReferrerHeaderHandler::InitializeHandler()
{
		updateReferrerHeaderRadios();
		Profile* profile = Profile::FromWebUI(web_ui());
  	notification_registrar_.Add(
      this, chrome::NOTIFICATION_REFERRER_HEADER_REGISTRY_CHANGED,
      content::Source<Profile>(profile));
}
void ReferrerHeaderHandler::InitializePage() {
	updateReferrerHeaderRadios();
}

void ReferrerHeaderHandler::updateReferrerHeaderRadios() {
  DCHECK(web_ui());
  Profile* profile = Profile::FromWebUI(web_ui());          
  bool enabled = profile->GetPrefs()->GetBoolean(prefs::kAllwaysOrNeverReferrersHeader);
	Allways_Or_Never=enabled;	
  base::FundamentalValue handlers_enabled(enabled);

  web_ui()->CallJavascriptFunction("ReferrerHeader.updateReferrerHeaderRadios",
      handlers_enabled);
      ReferrerHeaderHandler::Allways_Or_Never = enabled;
}


void ReferrerHeaderHandler::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_REFERRER_HEADER_REGISTRY_CHANGED: {
      updateReferrerHeaderRadios();
      break;
    }
    // champion : removed on balaji suggestion (amresh)
    // Observe no more available in OptionsPageUIHandler
    /* default:
      OptionsPageUIHandler::Observe(type, source, details);*/
  }
}

void ReferrerHeaderHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback("setReferrerHeaderEnabled",
      base::Bind(&ReferrerHeaderHandler::setReferrerHeaderEnabled,
                 base::Unretained(this)));
  /* web_ui()->RegisterMessageCallback("RemoveException",
      base::Bind(&ReferrerHeaderHandler::RemoveException,
                 base::Unretained(this)));
  web_ui()->RegisterMessageCallback("SetException",
      base::Bind(&ReferrerHeaderHandler::SetException,
                 base::Unretained(this)));
  web_ui()->RegisterMessageCallback("CheckExceptionPatternValidity",
      base::Bind(&ReferrerHeaderHandler::CheckExceptionPatternValidity,
                 base::Unretained(this)));*/
}

int Checkvalue(std::vector<std ::string>& temvector, std::string key) {
	std::vector<std:: string>::iterator itpurge;

	for(itpurge=temvector.begin();itpurge!=temvector.end();++itpurge) {
		std ::string tempkey(*itpurge);
		std ::string temprefkey(key);
		unsigned int value=temprefkey.find(tempkey);

		if ( value != std::string::npos )	
			return 1;
	}

	return 0;
} 

bool ReferrerHeaderHandler::GetReferrerHeader() {
 return Allways_Or_Never;
}

void ReferrerHeaderHandler::SetException(const ListValue* args) {
  size_t arg_i = 0;
  std::string type_string;
  CHECK(args->GetString(arg_i++, &type_string));
  std::string mode;
  CHECK(args->GetString(arg_i++, &mode));
  std::string pattern;
  CHECK(args->GetString(arg_i++, &pattern));
  std::string setting;
  CHECK(args->GetString(arg_i++, &setting));

}
void ReferrerHeaderHandler::setReferrerHeaderEnabled(const ListValue* args) {
  // bool enabled = true;
  bool allwaysornever=true;
  CHECK(args->GetBoolean(0, &allwaysornever));

  // Added for purge reffer 
  if(allwaysornever) {
	  Allways_Or_Never=true;	  
	  allwaysornever=true;
  } else {
	  Allways_Or_Never=false;
	  allwaysornever=false;	  
  }
  // End

  Profile* profile = Profile::FromWebUI(web_ui());   
  scoped_ptr<base::Value> value(Value::CreateBooleanValue(allwaysornever));
	scoped_ptr<base::Value> valueallwaysornever(Value::CreateBooleanValue(allwaysornever));
  profile->GetPrefs()->Set(prefs::kAllwaysOrNeverReferrersHeader, *valueallwaysornever);
}

//Added for purge reffer 
void ReferrerHeaderHandler::CheckExceptionPatternValidity(const ListValue* args) {
  size_t arg_i = 0;
  Value* type;
  ListValue* args1=const_cast<ListValue*>(args);
  CHECK(args1->Get(arg_i++, &type));
  std::string mode_string;
  CHECK(args->GetString(arg_i++, &mode_string));
  std::string pattern_string;
  CHECK(args->GetString(arg_i++, &pattern_string));
  ContentSettingsPattern pattern =ContentSettingsPattern::FromString(pattern_string);
	scoped_ptr<Value> mode_value(Value::CreateStringValue(mode_string));
  scoped_ptr<Value> pattern_value(Value::CreateStringValue(pattern_string));
  scoped_ptr<Value> valid_value(Value::CreateBooleanValue(pattern.IsValid()));
  {  
    web_ui()->CallJavascriptFunction(
        "ReferrerHeader.patternValidityCheckComplete",
        *type,
        *mode_value.get(),
        *pattern_value.get(),
        *valid_value.get());
  }
}

} // end of namespace options
