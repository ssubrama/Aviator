// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "chrome/browser/mac/master_prefs.h"

#include "base/file_util.h"
#include "base/mac/foundation_util.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_version_info.h"

namespace {

#if defined(GOOGLE_CHROME_BUILD)
// This should be NSApplicationSupportDirectory, but it has already been
// released using NSLibraryDirectory.
const NSSearchPathDirectory kSearchPath = NSLibraryDirectory;
const char kMasterPreferencesDirectory[] = "Google";
const char kMasterPreferencesFileName[] = "Google Chrome Master Preferences";
#else
const NSSearchPathDirectory kSearchPath = NSApplicationSupportDirectory;
const char kMasterPreferencesDirectory[] = "Aviator"; //added for champion +Aviator -Chromium
const char kMasterPreferencesFileName[] = "Aviator Master Preferences"; //added for champion +Aviator -Chromium
#endif  // GOOGLE_CHROME_BUILD

}  // namespace


namespace master_prefs {

base::FilePath MasterPrefsPath() {
#if defined(GOOGLE_CHROME_BUILD)
  // Don't load master preferences for the canary.
  chrome::VersionInfo::Channel channel = chrome::VersionInfo::GetChannel();
  if (channel == chrome::VersionInfo::CHANNEL_CANARY)
    return base::FilePath();
#endif  // GOOGLE_CHROME_BUILD

  // On official builds, try
  //~/Library/Application Support/Google/Chrome/Google Chrome Master Preferences
  // On chromium builds, try
  //~/Library/Application Support/Chromium/Chromium Master Preferences
  // This intentionally doesn't use eventual --user-data-dir overrides.
  base::FilePath user_application_support_path;
  if (chrome::GetDefaultUserDataDirectory(&user_application_support_path)) {
    user_application_support_path =
        user_application_support_path.Append(kMasterPreferencesFileName);
    if (base::PathExists(user_application_support_path))
      return user_application_support_path;
  }

  // On official builds, try /Library/Google/Google Chrome Master Preferences
  // On chromium builds, try
  // /Library/Application Support/Chromium/Chromium Master Preferences
  base::FilePath search_path;
  if (!base::mac::GetLocalDirectory(kSearchPath, &search_path))
    return base::FilePath();

  return search_path.Append(kMasterPreferencesDirectory)
                    .Append(kMasterPreferencesFileName);
}

}  // namespace master_prefs
