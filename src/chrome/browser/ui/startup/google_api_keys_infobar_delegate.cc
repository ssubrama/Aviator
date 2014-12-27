// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/ui/startup/google_api_keys_infobar_delegate.h"

#include "chrome/browser/infobars/infobar_service.h"
#include "components/infobars/core/infobar.h"
#include "content/public/browser/web_contents.h"
#include "google_apis/google_api_keys.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"


// static
void GoogleApiKeysInfoBarDelegate::Create(InfoBarService* infobar_service) {
  if (google_apis::HasKeysConfigured())
    return;

  infobar_service->AddInfoBar(ConfirmInfoBarDelegate::CreateInfoBar(
      scoped_ptr<ConfirmInfoBarDelegate>(new GoogleApiKeysInfoBarDelegate())));
}

GoogleApiKeysInfoBarDelegate::GoogleApiKeysInfoBarDelegate()
    : ConfirmInfoBarDelegate() {
}

GoogleApiKeysInfoBarDelegate::~GoogleApiKeysInfoBarDelegate() {
}

base::string16 GoogleApiKeysInfoBarDelegate::GetMessageText() const {
  return l10n_util::GetStringUTF16(IDS_MISSING_GOOGLE_API_KEYS);
}

int GoogleApiKeysInfoBarDelegate::GetButtons() const {
  return BUTTON_NONE;
}

base::string16 GoogleApiKeysInfoBarDelegate::GetLinkText() const {
  return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
}

bool GoogleApiKeysInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  InfoBarService::WebContentsFromInfoBar(infobar())->OpenURL(
      content::OpenURLParams(
          GURL("https://www.whitehatsec.com/aviator/help/"), // champion HELP URL Redirect to aviator Help Page
          content::Referrer(),
          (disposition == CURRENT_TAB) ? NEW_FOREGROUND_TAB : disposition,
          content::PAGE_TRANSITION_LINK, false));
  return false;
}
