// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_UTILS_H_
#define CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_UTILS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "build/build_config.h"

class GURL;
class TemplateURL;

namespace base {
  class DictionaryValue;
  class FilePath;
}

#if defined(OS_WIN)
// Detects which version of Chrome is installed from registry. Returns its
// major version, and drops the minor version. Returns 0 if failed. If there are
// indicators of both Chrome 2 and Chrome 3 it is biased to return the biggest
// version.
int GetCurrentChromeMajorVersionFromRegistry();

// Detects where Chrome lives. Returns an empty path if Chrome is not installed.
base::FilePath GetChromeInstallPathFromRegistry();
base::FilePath GetChromInstallPathFromRegistry();
base::FilePath GetAviatorInstallPathFromRegistry();      //Champion Dinesh for Aviator path
base::FilePath GetAviatorInstallPathFromRegistry32bit(); //Champion Dinesh for Aviator path
std::string GetAviatorInstallPathFromRegistry32bitW();   //Champion Dinesh for Aviator path
std::string GetChromeInstalledPath();

#endif  // OS_WIN

#if defined(OS_MACOSX)
// Get the directory in which the Chrome .dylibs live, we need to load these
// in order to decoded FF profile passwords.
// The Path is usuall FF App Bundle/Contents/Mac OS/
// Returns empty path on failure.
base::FilePath GetChromeDylibPath();
#endif  // OS_MACOSX

// Returns the path to the Chrome profile.
base::FilePath GetChromeProfilePath();
base::FilePath GetProfilesPreferencePath();
void ParsePreferenceFile(const base::FilePath& file, base::DictionaryValue* root);

// Returns true if we want to add the URL to the history. We filter out the URL
// with a unsupported scheme.
bool CanImportURL(const GURL& url);

// Parses the OpenSearch XML files in |xml_files| and populates |search_engines|
// with the resulting TemplateURLs.
void ParseSearchEnginesFromXMLFiles(const std::vector<base::FilePath>& xml_files,
                                    std::vector<TemplateURL*>* search_engines);

// Returns the home page set in Chrome in a particular profile.
GURL GetHomepageChrome(const base::FilePath& profile_path);

// Checks to see if this home page is a default home page, as specified by
// the resource file browserconfig.properties in the Chrome application
// directory.
bool IsDefaultHomepageChrome(const GURL& homepage, const base::FilePath& app_path);

// Parses the prefs found in the file |pref_file| and puts the key/value pairs
// in |prefs|. Keys are strings, and values can be strings, booleans or
// integers.  Returns true if it succeeded, false otherwise (in which case
// |prefs| is not filled).
// Note: for strings, only valid UTF-8 string values are supported. If a
// key/pair is not valid UTF-8, it is ignored and will not appear in |prefs|.
bool ParsePrefFile(const base::FilePath& pref_file, base::DictionaryValue* prefs);

// Parses the value of a particular Chrome preference from a string that is the
// contents of the prefs file.
std::string GetPrefsJsValueChrome(const std::string& prefs,
                            const std::string& pref_key);

// Returns the localized Chrome branding name.
// This is useful to differentiate between Chrome and Iceweasel.
// If anything goes wrong while trying to obtain the branding name,
// the function assumes it's Chrome.

base::FilePath GetAppInstallPath();
base::FilePath GetSafariAppInstallPath();

	
#endif  // CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_UTILS_H_
