// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "chrome/browser/pepper_broker_infobar_delegate.h"

#include "base/prefs/pref_service.h"
#include "chrome/browser/content_settings/host_content_settings_map.h"
#include "chrome/browser/content_settings/tab_specific_content_settings.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/plugins/plugin_finder.h"
#include "chrome/browser/plugins/plugin_metadata.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/infobars/core/infobar.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/plugin_service.h"
#include "content/public/browser/user_metrics.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/referrer.h"
#include "content/public/common/webplugininfo.h"
#include "grit/generated_resources.h"
#include "grit/theme_resources.h"
#include "net/base/net_util.h"
#include "ui/base/l10n/l10n_util.h"


// static
void PepperBrokerInfoBarDelegate::Create(
    content::WebContents* web_contents,
    const GURL& url,
    const base::FilePath& plugin_path,
    const base::Callback<void(bool)>& callback) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  // TODO(wad): Add ephemeral device ID support for broker in guest mode.
  if (profile->IsGuestSession()) {
    callback.Run(false);
    return;
  }

  TabSpecificContentSettings* tab_content_settings =
      TabSpecificContentSettings::FromWebContents(web_contents);

  HostContentSettingsMap* content_settings =
      profile->GetHostContentSettingsMap();
  ContentSetting setting =
      content_settings->GetContentSetting(url, url,
                                          CONTENT_SETTINGS_TYPE_PPAPI_BROKER,
                                          std::string());

  if (setting == CONTENT_SETTING_ASK) {
    content::RecordAction(
        base::UserMetricsAction("PPAPI.BrokerInfobarDisplayed"));
    InfoBarService* infobar_service =
        InfoBarService::FromWebContents(web_contents);
    infobar_service->AddInfoBar(ConfirmInfoBarDelegate::CreateInfoBar(
        scoped_ptr<ConfirmInfoBarDelegate>(new PepperBrokerInfoBarDelegate(
            url, plugin_path,
            profile->GetPrefs()->GetString(prefs::kAcceptLanguages),
            content_settings, tab_content_settings, callback))));
    return;
  }

  bool allowed = (setting == CONTENT_SETTING_ALLOW);
  content::RecordAction(allowed ?
      base::UserMetricsAction("PPAPI.BrokerSettingAllow") :
      base::UserMetricsAction("PPAPI.BrokerSettingDeny"));
  tab_content_settings->SetPepperBrokerAllowed(allowed);
  callback.Run(allowed);
}

PepperBrokerInfoBarDelegate::PepperBrokerInfoBarDelegate(
    const GURL& url,
    const base::FilePath& plugin_path,
    const std::string& languages,
    HostContentSettingsMap* content_settings,
    TabSpecificContentSettings* tab_content_settings,
    const base::Callback<void(bool)>& callback)
    : ConfirmInfoBarDelegate(),
      url_(url),
      plugin_path_(plugin_path),
      languages_(languages),
      content_settings_(content_settings),
      tab_content_settings_(tab_content_settings),
      callback_(callback) {
}

PepperBrokerInfoBarDelegate::~PepperBrokerInfoBarDelegate() {
  if (!callback_.is_null())
    callback_.Run(false);
}

int PepperBrokerInfoBarDelegate::GetIconID() const {
  return IDR_INFOBAR_PLUGIN_INSTALL;
}

base::string16 PepperBrokerInfoBarDelegate::GetMessageText() const {
  content::PluginService* plugin_service =
      content::PluginService::GetInstance();
  content::WebPluginInfo plugin;
  bool success = plugin_service->GetPluginInfoByPath(plugin_path_, &plugin);
  DCHECK(success);
  scoped_ptr<PluginMetadata> plugin_metadata(
      PluginFinder::GetInstance()->GetPluginMetadata(plugin));
  return l10n_util::GetStringFUTF16(IDS_PEPPER_BROKER_MESSAGE,
                                    plugin_metadata->name(),
                                    net::FormatUrl(url_.GetOrigin(),
                                                   languages_));
}

base::string16 PepperBrokerInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return l10n_util::GetStringUTF16((button == BUTTON_OK) ?
      IDS_PEPPER_BROKER_ALLOW_BUTTON : IDS_PEPPER_BROKER_DENY_BUTTON);
}

bool PepperBrokerInfoBarDelegate::Accept() {
  DispatchCallback(true);
  return true;
}

bool PepperBrokerInfoBarDelegate::Cancel() {
  DispatchCallback(false);
  return true;
}

base::string16 PepperBrokerInfoBarDelegate::GetLinkText() const {
  return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
}
 
 // AVIATOR_HELP_URL; // champion balaji

bool PepperBrokerInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  GURL learn_more_url("https://www.whitehatsec.com/aviator/help/");
  InfoBarService::WebContentsFromInfoBar(infobar())->OpenURL(
      content::OpenURLParams(
          learn_more_url, content::Referrer(),
          (disposition == CURRENT_TAB) ? NEW_FOREGROUND_TAB : disposition,
          content::PAGE_TRANSITION_LINK, false));
  return false;
}

void PepperBrokerInfoBarDelegate::DispatchCallback(bool result) {
  content::RecordAction(result ?
      base::UserMetricsAction("PPAPI.BrokerInfobarClickedAllow") :
      base::UserMetricsAction("PPAPI.BrokerInfobarClickedDeny"));
  callback_.Run(result);
  callback_ = base::Callback<void(bool)>();
  content_settings_->SetContentSetting(
      ContentSettingsPattern::FromURLNoWildcard(url_),
      ContentSettingsPattern::Wildcard(),
      CONTENT_SETTINGS_TYPE_PPAPI_BROKER,
      std::string(), result ? CONTENT_SETTING_ALLOW : CONTENT_SETTING_BLOCK);
  tab_content_settings_->SetPepperBrokerAllowed(result);
}
