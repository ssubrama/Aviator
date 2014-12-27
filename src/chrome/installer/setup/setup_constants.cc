// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.
// champion : app name change 

#include "chrome/installer/setup/setup_constants.h"

namespace installer {

// Elements that make up install paths.
const wchar_t kChromeArchive[] = L"Aviator.7z";
const wchar_t kChromeCompressedArchive[] = L"Aviator.packed.7z";
const wchar_t kVisualElements[] = L"VisualElements";
const wchar_t kVisualElementsManifest[] = L"VisualElementsManifest.xml";
const wchar_t kWowHelperExe[] = L"wow_helper.exe";

// Sub directory of install source package under install temporary directory.
const wchar_t kInstallSourceDir[] = L"source";
const wchar_t kInstallSourceChromeDir[] = L"Aviator-bin"; // champion : branding (amresh)

const wchar_t kMediaPlayerRegPath[] =
    L"Software\\Microsoft\\MediaPlayer\\ShimInclusionList";

// Key names for values passed via the GoogleUpdateUntrustedData environment
// variable.
const char kInstallFromWebstore[] = "install-from-webstore";

}  // namespace installer
