// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#ifndef COMPONENTS_PASSWORD_MANAGER_CORE_COMMON_PASSWORD_MANAGER_PREF_NAMES_H_
#define COMPONENTS_PASSWORD_MANAGER_CORE_COMMON_PASSWORD_MANAGER_PREF_NAMES_H_

#include "base/basictypes.h"

namespace password_manager {

namespace prefs {

// Alphabetical list of preference names specific to the PasswordManager
// component. Keep alphabetized, and document each in the .cc file.
#if defined(OS_WIN)
extern const char kOsPasswordBlank[];
extern const char kOsPasswordLastChanged[];
#endif
extern const char kPasswordManagerAllowShowPasswords[];
extern const char kPasswordManagerEnabled[];
extern const char kPasswordManagerGroupsForDomains[];
extern const char kPasswordManagerIncognitoEnabled[]; // champion : moved from pref_names.h (Amresh)

#if !defined(OS_MACOSX) && !defined(OS_CHROMEOS) && defined(OS_POSIX)
extern const char kLocalProfileId[];
#endif

}  // namespace prefs

}  // namespace password_manager

#endif  // COMPONENTS_PASSWORD_MANAGER_CORE_COMMON_PASSWORD_MANAGER_PREF_NAMES_H_
