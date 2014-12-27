// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/chromeimporter/chrome_profile_lock.h"

#include "base/files/file_path.h"
#include "base/threading/thread_restrictions.h"

// static
#if defined(OS_MACOSX)
const base::FilePath::CharType* ChromeProfileLock::kLockFileName =
    FILE_PATH_LITERAL("LOCK");
const base::FilePath::CharType* ChromeProfileLock::kOldLockFileName =
    FILE_PATH_LITERAL("LOCK");
#elif defined(OS_POSIX)
const base::FilePath::CharType* ChromeProfileLock::kLockFileName =
    FILE_PATH_LITERAL("LOCK");
const base::FilePath::CharType* ChromeProfileLock::kOldLockFileName =
    FILE_PATH_LITERAL("LOCK");
#else
const base::FilePath::CharType* ChromeProfileLock::kLockFileName =
    FILE_PATH_LITERAL("LOCK");
#endif

#if defined(OS_WIN)
ChromeProfileLock::ChromeProfileLock(const base::FilePath& path) {
  Init();

  //champion -bookmark issue - Dinesh
  base::FilePath newpath;
  std::wstring checkstring=path.value();
  unsigned found = checkstring.find(L"Default");
  std::wstring srcpathchange = checkstring.substr(0,found);

  srcpathchange.append(kLockFileName);
  lock_file_=newpath.Append(srcpathchange);
  Lock();
}

#elif defined (OS_MACOSX)
ChromeProfileLock::ChromeProfileLock(const base::FilePath& path) {
  std::string strfullpath=path.value(); //("/Users/photoninfotech/Library/Application Support/Google/Chrome/Default/Preferences");
    strfullpath.replace(strfullpath.length()-12,12,"/");
    strfullpath.append("Extension Rules/LOCK");
    lock_file_=base::FilePath::FromUTF8Unsafe(strfullpath);
}
#endif
ChromeProfileLock::~ChromeProfileLock() {
  // Because this destructor happens in first run on the profile import thread,
  // with no UI to jank, it's ok to allow deletion of the lock here.
  base::ThreadRestrictions::ScopedAllowIO allow_io;
  Unlock();
}
