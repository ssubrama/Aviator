// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include <Cocoa/Cocoa.h>
#include <sys/param.h>
#import <AppKit/AppKit.h>
#include <CoreFoundation/CoreFoundation.h>

#include "chrome/common/importer/chrome_importer_utils.h"

#include "base/file_util.h"
#include "base/mac/foundation_util.h"
#include "base/path_service.h"

base::FilePath GetProfilesPreferencePath() {
  base::FilePath app_data_path;
  if (!PathService::Get(base::DIR_APP_DATA, &app_data_path)) {
    return base::FilePath();
  }
  base::FilePath pref_file =app_data_path.Append("Google");
  pref_file = pref_file.Append("Chrome");	
  pref_file =pref_file.Append("Default").Append("Preferences");
  if (!base::PathExists(pref_file)) {
    return base::FilePath();
  }
  return pref_file;
}

base::FilePath GetAppInstallPath(){			
 NSWorkspace* ws = [NSWorkspace sharedWorkspace];
 NSString *bundlePath = [ws absolutePathForAppBundleWithIdentifier:@"com.google.Chrome"];
 if (bundlePath) {
 const char* string = [bundlePath UTF8String];
 std::string str(string);
 base ::FilePath AppPath(str);		
 return AppPath;
 }
 else
 return base::FilePath();
}
  	
base::FilePath GetChromeDylibPath() {
  CFURLRef appURL = nil;
  if (LSFindApplicationForInfo(kLSUnknownCreator,
                              CFSTR("com.google.Chrome"),
                              NULL,
                              NULL,
                              &appURL) != noErr) {
    return base::FilePath();
  }
  NSBundle *ff_bundle =
      [NSBundle bundleWithPath:[base::mac::CFToNSCast(appURL) path]];
  CFRelease(appURL);
  NSString *ff_library_path =
      [[ff_bundle executablePath] stringByDeletingLastPathComponent];
  char buf[MAXPATHLEN];
  if (![ff_library_path getFileSystemRepresentation:buf maxLength:sizeof(buf)])
    return base::FilePath();
  return base::FilePath(buf);
}
