/* Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
in this file are licensed under the BSD-3-clause license (the "License") included in 
the WhiteHat-LICENSE file included in the root directory of the distributed source code 
archive. Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
 ANY KIND, either express or implied. See the License for the specific language governing 
 permissions and limitations under the License. 
 */

#include "components/autofill/core/common/autofill_constants.h"

#include "build/build_config.h"

// Champion artf414382 Build library on Mac	
#include "chrome/browser/championconfig/varsconfig.h" 

namespace autofill {

const char kHelpURL[] =AVIATOR_HELP_URL; //champion
/*#if defined(OS_CHROMEOS)
    "https://support.google.com/chromeos/?p=settings_autofill";
#else
    "https://support.google.com/chrome/?p=settings_autofill";
#endif */

const size_t kRequiredAutofillFields = 3;

}  // namespace autofill
