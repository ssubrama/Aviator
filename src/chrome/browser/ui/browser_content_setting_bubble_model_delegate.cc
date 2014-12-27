// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_dialogs.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/common/url_constants.h"
#include "components/google/core/browser/google_util.h"

// Champion artf414382 Build library on Mac	
#include "chrome/browser/championconfig/varsconfig.h" 

// The URL for when the user clicks "learn more" on the mixed scripting page
// icon bubble.
const char kInsecureScriptHelpUrl[] = AVIATOR_HELP_URL; // champion balaji
// "https://support.google.com/chrome/bin/answer.py?answer=1342714";

BrowserContentSettingBubbleModelDelegate::
BrowserContentSettingBubbleModelDelegate(Browser* browser) : browser_(browser) {
}

BrowserContentSettingBubbleModelDelegate::
~BrowserContentSettingBubbleModelDelegate() {
}

void BrowserContentSettingBubbleModelDelegate::ShowCollectedCookiesDialog(
    content::WebContents* web_contents) {
  chrome::ShowCollectedCookiesDialog(web_contents);
}

void BrowserContentSettingBubbleModelDelegate::ShowContentSettingsPage(
    ContentSettingsType type) {
  switch (type) {
    case CONTENT_SETTINGS_TYPE_MIXEDSCRIPT:
      // We don't (yet?) implement user-settable exceptions for mixed script
      // blocking, so bounce to an explanatory page for now.
      chrome::AddSelectedTabWithURL(browser_,
                                    GURL(kInsecureScriptHelpUrl),
                                    content::PAGE_TRANSITION_LINK);
      return;
    case CONTENT_SETTINGS_TYPE_PROTOCOL_HANDLERS:
      chrome::ShowSettingsSubPage(browser_, chrome::kHandlerSettingsSubPage);
      return;
    default:
      chrome::ShowContentSettings(browser_, type);
      return;
  }
}

void BrowserContentSettingBubbleModelDelegate::ShowLearnMorePage(
    ContentSettingsType type) {
  if (type != CONTENT_SETTINGS_TYPE_PLUGINS)
    return;
  chrome::AddSelectedTabWithURL(browser_,
                                GURL(chrome::kBlockedPluginLearnMoreURL),
                                content::PAGE_TRANSITION_LINK);
}
