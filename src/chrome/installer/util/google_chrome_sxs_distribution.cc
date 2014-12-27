// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

//
// This file defines implementation of GoogleChromeSxSDistribution.
// champion : app name change (amresh)

#include "chrome/installer/util/google_chrome_sxs_distribution.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "chrome/common/chrome_icon_resources_win.h"
#include "chrome/installer/util/updating_app_registration_data.h"

#include "installer_util_strings.h"  // NOLINT

namespace {

const wchar_t kChromeSxSGuid[] = L"{4ea16ac7-fd5a-47c3-875b-dbf4a2008c20}";
const wchar_t kChannelName[] = L"canary";
const wchar_t kBrowserAppId[] = L"AviatorCanary";
const wchar_t kBrowserProgIdPrefix[] = L"AviatorSSHTM";
const wchar_t kBrowserProgIdDesc[] = L"Aviator Canary HTML Document";
const wchar_t kCommandExecuteImplUuid[] =
    L"{1BEAC3E3-B852-44F4-B468-8906C062422E}";

}  // namespace

GoogleChromeSxSDistribution::GoogleChromeSxSDistribution()
    : GoogleChromeDistribution(
          make_scoped_ptr(
              new UpdatingAppRegistrationData(kChromeSxSGuid))) {
}

base::string16 GoogleChromeSxSDistribution::GetBaseAppName() {
  return L"WhiteHat Aviator Canary";
}

base::string16 GoogleChromeSxSDistribution::GetShortcutName(
    ShortcutType shortcut_type) {
  switch (shortcut_type) {
    case SHORTCUT_CHROME_ALTERNATE:
      // This should never be called. Returning the same string as Google Chrome
      // preserves behavior, but it will result in a naming collision.
      NOTREACHED();
      return GoogleChromeDistribution::GetShortcutName(shortcut_type);
    case SHORTCUT_APP_LAUNCHER:
      return installer::GetLocalizedString(
          IDS_APP_LIST_SHORTCUT_NAME_CANARY_BASE);
    default:
      DCHECK_EQ(shortcut_type, SHORTCUT_CHROME);
      return installer::GetLocalizedString(IDS_SXS_SHORTCUT_NAME_BASE);
  }
}

base::string16 GoogleChromeSxSDistribution::GetStartMenuShortcutSubfolder(
    Subfolder subfolder_type) {
  switch (subfolder_type) {
    case SUBFOLDER_APPS:
      return installer::GetLocalizedString(
          IDS_APP_SHORTCUTS_SUBDIR_NAME_CANARY_BASE);
    default:
      DCHECK_EQ(subfolder_type, SUBFOLDER_CHROME);
      return GetShortcutName(SHORTCUT_CHROME);
  }
}

base::string16 GoogleChromeSxSDistribution::GetBaseAppId() {
  return kBrowserAppId;
}

base::string16 GoogleChromeSxSDistribution::GetBrowserProgIdPrefix() {
  return kBrowserProgIdPrefix;
}

base::string16 GoogleChromeSxSDistribution::GetBrowserProgIdDesc() {
  return kBrowserProgIdDesc;
}

base::string16 GoogleChromeSxSDistribution::GetInstallSubDir() {
  return GoogleChromeDistribution::GetInstallSubDir().append(
      installer::kSxSSuffix);
}

base::string16 GoogleChromeSxSDistribution::GetUninstallRegPath() {
  return GoogleChromeDistribution::GetUninstallRegPath().append(
      installer::kSxSSuffix);
}

BrowserDistribution::DefaultBrowserControlPolicy
    GoogleChromeSxSDistribution::GetDefaultBrowserControlPolicy() {
  return DEFAULT_BROWSER_OS_CONTROL_ONLY;
}

int GoogleChromeSxSDistribution::GetIconIndex(ShortcutType shortcut_type) {
  if (shortcut_type == SHORTCUT_APP_LAUNCHER)
    return icon_resources::kSxSAppLauncherIndex;
  DCHECK(shortcut_type == SHORTCUT_CHROME ||
         shortcut_type == SHORTCUT_CHROME_ALTERNATE) << shortcut_type;
  return icon_resources::kSxSApplicationIndex;
}

bool GoogleChromeSxSDistribution::GetChromeChannel(base::string16* channel) {
  *channel = kChannelName;
  return true;
}

bool GoogleChromeSxSDistribution::GetCommandExecuteImplClsid(
    base::string16* handler_class_uuid) {
  if (handler_class_uuid)
    *handler_class_uuid = kCommandExecuteImplUuid;
  return true;
}

bool GoogleChromeSxSDistribution::AppHostIsSupported() {
  return false;
}

bool GoogleChromeSxSDistribution::ShouldSetExperimentLabels() {
  return true;
}

bool GoogleChromeSxSDistribution::HasUserExperiments() {
  return true;
}

base::string16 GoogleChromeSxSDistribution::ChannelName() {
  return kChannelName;
}
