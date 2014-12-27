// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_CHAMPIONCONFIG_ALTERNATE_BROWSER_FINDER_H_
#define CHROME_BROWSER_CHAMPIONCONFIG_ALTERNATE_BROWSER_FINDER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "google_apis/gaia/gaia_urls.h"

namespace base {
class FilePath;
}
 
namespace ABF {
  const char kLoadInDefaultBrowser[] = "profile.LoadInDefaultBrowser";
  const char kDisplayAlternateBrowser[] = "profile.display_alternate_browser";
  const char kConnectionControlRules[] = "profile.ConnectionControlRules";
  const char kRulesMovedToPref[] = "profile.RulesMovedToPref";
  const char kNPCCRulesUpdated[] = "profile.NPCCRulesUpdated";
  const wchar_t kRegShellOpen[] = L"\\shell\\open\\command";
  const wchar_t kRegVistaUrlPrefs[] = L"Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice";
    
  base::FilePath GetFirefoxInstallPathFromRegistry();
  base::FilePath GetChromeInstallPathFromRegistry();
  base::FilePath GetIEInstallPathFromRegistryLM();
  base::FilePath GetIEInstallPathFromRegistryCU();
  std::string GetChromeInstalledPath();
  std::string GetDefaultBrowserForAviator();
  bool DirExists(const std::string& dirName_in);
  bool IsWebBasedSigninFlowURL(const GURL& url);
  bool IsFirefoxDefaultBrowser();
  bool IsGoogleChromeDefaultBrowser();
  bool IsAviatorDefaultBrowser();
  bool IsIEDefaultBrowser(); 
}

#endif  //CHROME_BROWSER_CHAMPIONCONFIG_ALTERNATE_BROWSER_FINDER_H_
