// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_IMPORTER_CHROME_PROFILE_LOCK_H__
#define CHROME_BROWSER_IMPORTER_CHROME_PROFILE_LOCK_H__

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"

class ChromeProfileLock {
public:
  explicit ChromeProfileLock(const base::FilePath& path);
  ~ChromeProfileLock();

  // Locks and releases the profile.
  void Lock();
  void Unlock();

  // Returns true if we lock the profile successfully.
  bool HasAcquired();

private:
  static const base::FilePath::CharType* kLockFileName;
  static const base::FilePath::CharType* kOldLockFileName;

  void Init();

  // Full path of the lock file in the profile folder.
  base::FilePath lock_file_;

  // The handle of the lock file.
#if defined(OS_WIN)
  HANDLE lock_handle_;
#elif defined(OS_POSIX)
  int lock_fd_;

  // On Posix systems Firefox apparently first tries to put a fcntl lock
  // on a file and if that fails, it does a regular exculsive open on another
  // file. This variable contains the location of this other file.
  base::FilePath old_lock_file_;

  // Method that tries to put a fcntl lock on file specified by |lock_file_|.
  // Returns false if lock is already held by another process. true in all
  // other cases.
  bool LockWithFcntl();
#endif

  DISALLOW_COPY_AND_ASSIGN(ChromeProfileLock);
};

#endif  // CHROME_BROWSER_IMPORTER_FIREFOX_PROFILE_LOCK_H__
