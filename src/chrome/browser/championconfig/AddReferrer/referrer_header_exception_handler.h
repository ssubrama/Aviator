// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_REFERRER_HEADING_EXCEPTION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_REFERRER_HEADING_EXCEPTION_HANDLER_H_
#pragma once

#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class HostContentSettingsMap;
class ProtocolHandlerRegistry;

class ReferrerHeaderExceptionHandler : public options :: OptionsPageUIHandler {
public:
  ReferrerHeaderExceptionHandler();
  virtual ~ReferrerHeaderExceptionHandler();

  // OptionsPageUIHandler implementation.
  virtual void GetLocalizedValues(base::DictionaryValue* localized_strings) OVERRIDE;

  virtual void InitializePage() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ReferrerHeaderExceptionHandler);
};

#endif  // CHROME_BROWSER_UI_WEBUI_OPTIONS_REFERRER_HEADING_EXCEPTION_HANDLER_H_
