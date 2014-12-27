// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "base/debug/debugger.h"

#include <stdlib.h>
#include <windows.h>

namespace base {
namespace debug {

bool BeingDebugged() {
  return ::IsDebuggerPresent() != 0;
}

void BreakDebugger() {
  if (IsDebugUISuppressed())
    _exit(1);
  //__debugbreak(); // champion : no stopping
#if defined(NDEBUG)
  _exit(1);
#endif
}

}  // namespace debug
}  // namespace base
