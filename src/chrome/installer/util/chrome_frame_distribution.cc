// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
//
// This file defines a specific implementation of BrowserDistribution class for
// Chrome Frame. It overrides the bare minimum of methods necessary to get a
// Chrome Frame installer that does not interact with Google Chrome or
// Chromium installations.
// champion : app name changes (amresh)

#include "chrome/installer/util/chrome_frame_distribution.h"

#include "base/strings/string_util.h"
#include "chrome/common/net/test_server_locations.h"
#include "chrome/installer/util/channel_info.h"
#include "chrome/installer/util/google_update_constants.h"
#include "chrome/installer/util/google_update_settings.h"
#include "chrome/installer/util/helper.h"
#include "chrome/installer/util/install_util.h"
#include "chrome/installer/util/l10n_string_util.h"
#include "chrome/installer/util/updating_app_registration_data.h"

#include "installer_util_strings.h"  // NOLINT

namespace {
const wchar_t kChromeFrameGuid[] = L"{8BA986DA-5100-405E-AA35-86F34A02ACBF}";
}

ChromeFrameDistribution::ChromeFrameDistribution()
    : BrowserDistribution(CHROME_FRAME,
          make_scoped_ptr(new UpdatingAppRegistrationData(kChromeFrameGuid))) {
}

base::string16 ChromeFrameDistribution::GetBaseAppName() {
  return L"Aviator Frame";
}

base::string16 ChromeFrameDistribution::GetBrowserProgIdPrefix() {
  NOTREACHED();
  return base::string16();
}

base::string16 ChromeFrameDistribution::GetBrowserProgIdDesc() {
  NOTREACHED();
  return base::string16();
}

base::string16 ChromeFrameDistribution::GetDisplayName() {
#if defined(GOOGLE_CHROME_BUILD)
  return L"Aviator Frame";
#else
  return L"Aviator Frame";
#endif
}

base::string16 ChromeFrameDistribution::GetShortcutName(
    ShortcutType shortcut_type) {
  NOTREACHED();
  return base::string16();
}

base::string16 ChromeFrameDistribution::GetInstallSubDir() {
  return L"WhiteHat\\Aviator\\Aviator Frame";
}

base::string16 ChromeFrameDistribution::GetPublisherName() {
  const base::string16& publisher_name =
      installer::GetLocalizedString(IDS_ABOUT_VERSION_COMPANY_NAME_BASE);
  return publisher_name;
}

base::string16 ChromeFrameDistribution::GetAppDescription() {
  return L"Aviator in a Frame.";
}

base::string16 ChromeFrameDistribution::GetLongAppDescription() {
  return L"Aviator in a Frame.";
}

std::string ChromeFrameDistribution::GetSafeBrowsingName() {
  return "aviatorframe";
}

std::string ChromeFrameDistribution::GetNetworkStatsServer() const {
  return chrome_common_net::kEchoTestServerLocation;
}

base::string16 ChromeFrameDistribution::GetUninstallLinkName() {
  return L"Uninstall Aviator Frame";
}

base::string16 ChromeFrameDistribution::GetUninstallRegPath() {
  return L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
         L"Aviator Frame";
}

base::string16 ChromeFrameDistribution::GetIconFilename() {
  return installer::kChromeExe;
}

int ChromeFrameDistribution::GetIconIndex(ShortcutType shortcut_type) {
  switch (shortcut_type) {
    case SHORTCUT_CHROME:
      return 0;
    default:
      NOTREACHED();
      return 0;
  }
}

BrowserDistribution::DefaultBrowserControlPolicy
    ChromeFrameDistribution::GetDefaultBrowserControlPolicy() {
  return DEFAULT_BROWSER_UNSUPPORTED;
}

bool ChromeFrameDistribution::CanCreateDesktopShortcuts() {
  return false;
}

bool ChromeFrameDistribution::GetCommandExecuteImplClsid(
    base::string16* handler_class_uuid) {
  return false;
}

void ChromeFrameDistribution::UpdateInstallStatus(bool system_install,
    installer::ArchiveType archive_type,
    installer::InstallStatus install_status) {
#if defined(GOOGLE_CHROME_BUILD)
  GoogleUpdateSettings::UpdateInstallStatus(system_install,
      archive_type, InstallUtil::GetInstallReturnCode(install_status),
      kChromeFrameGuid);
#endif
}
