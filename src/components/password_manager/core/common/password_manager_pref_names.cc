// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "components/password_manager/core/common/password_manager_pref_names.h"

namespace password_manager {

namespace prefs {

#if defined(OS_WIN)
// Whether the password was blank, only valid if OS password was last changed
// on or before the value contained in kOsPasswordLastChanged.
const char kOsPasswordBlank[] = "password_manager.os_password_blank";

// The number of seconds since epoch that the OS password was last changed.
const char kOsPasswordLastChanged[] =
    "password_manager.os_password_last_changed";
#endif

// Boolean controlling whether the password manager allows to retrieve passwords
// in clear text.
const char kPasswordManagerAllowShowPasswords[] =
    "profile.password_manager_allow_show_passwords";

// Boolean that is true if the password manager is on (will record new
// passwords and fill in known passwords).
const char kPasswordManagerEnabled[] = "profile.password_manager_enabled";

// A list of numbers. Each number corresponds to one of the domains monitored
// for save-password-prompt breakages. That number is a random index into
// the array of groups containing the monitored domain. That group should be
// used for reporting that domain.
const char kPasswordManagerGroupsForDomains[] =
    "profile.password_manager_groups_for_domains";
    
// Boolean that is true if the password manager is on (will record new
// passwords and fill in known passwords incognito).
const char kPasswordManagerIncognitoEnabled[] = 
    "profile.password_manager_incognito_enabled"; // added for champion_pwd

#if !defined(OS_MACOSX) && !defined(OS_CHROMEOS) && defined(OS_POSIX)
// The local profile id for this profile.
const char kLocalProfileId[] = "profile.local_profile_id";
#endif

}  // namespace prefs

}  // namespace password_manager
