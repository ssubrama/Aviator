// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/installer/util/helper.h"

#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/install_util.h"
#include "chrome/installer/util/installation_state.h"
#include "chrome/installer/util/util_constants.h"

namespace installer {

base::FilePath GetChromeInstallPath(bool system_install,
                                    BrowserDistribution* dist) {
  base::FilePath install_path;
#if defined(_WIN64)
  // TODO(wfh): Place Chrome binaries into DIR_PROGRAM_FILESX86 until the code
  // to support moving the binaries is added.
  int key =
      system_install ? base::DIR_PROGRAM_FILESX86 : base::DIR_LOCAL_APP_DATA;
#else
  int key = system_install ? base::DIR_PROGRAM_FILES : base::DIR_LOCAL_APP_DATA;
#endif
  /*if (PathService::Get(key, &install_path)) {
    install_path = install_path.Append(dist->GetInstallSubDir());
    install_path = install_path.Append(kInstallBinaryDir);
  }*/
  
  // champion : our data path differs from installation path pattern
  bool isDataDir (false);
  if (system_install) {
    PathService::Get(key, &install_path);
  } else {
    isDataDir = true;
    PathService::Get(base::DIR_LOCAL_APP_DATA, &install_path);
  }

  if (!install_path.empty()) {
    // champion : install path is now at whitehat\aviator (amresh)
    install_path = (isDataDir) ? 
                     install_path.Append(dist->GetInstallSubDir()) :
                     install_path.Append(dist->GetInstallSubDir2());
    install_path = install_path.Append(kInstallBinaryDir);
  }
  
  return install_path;
}

BrowserDistribution* GetBinariesDistribution(bool system_install) {
  BrowserDistribution* dist = BrowserDistribution::GetDistribution();
  ProductState state;

  // If we're part of a multi-install, we need to poll using the multi-installer
  // package's app guid rather than the browser's or Chrome Frame's app guid.
  // If we can't read the app's state from the registry, assume it isn't
  // multi-installed.
  if (state.Initialize(system_install, dist) && state.is_multi_install()) {
    return BrowserDistribution::GetSpecificDistribution(
        BrowserDistribution::CHROME_BINARIES);
  } else {
    return dist;
  }
}

std::wstring GetAppGuidForUpdates(bool system_install) {
  return GetBinariesDistribution(system_install)->GetAppGuid();
}

}  // namespace installer.
