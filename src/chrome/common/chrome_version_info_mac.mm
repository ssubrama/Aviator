// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/common/chrome_version_info.h"

#import <Foundation/Foundation.h>

#include "base/basictypes.h"
#include "base/mac/bundle_locations.h"
#include "base/strings/sys_string_conversions.h"

namespace chrome {

// static
std::string VersionInfo::GetVersionStringModifier() {

return std::string();
	// //make change for aviator version () remove champion

/*
#if defined(GOOGLE_CHROME_BUILD)
  // Use the main Chrome application bundle and not the framework bundle.
  // Keystone keys don't live in the framework.
  NSBundle* bundle = base::mac::OuterBundle();
  NSString* channel = [bundle objectForInfoDictionaryKey:@"KSChannelID"];

  // Only ever return "", "unknown", "beta", "dev", or "canary" in a branded
  // build.
  if (![bundle objectForInfoDictionaryKey:@"KSProductID"]) {
    // This build is not Keystone-enabled, it can't have a channel.
    channel = @"unknown";
  } else if (!channel) {
    // For the stable channel, KSChannelID is not set.
    channel = @"";
  } else if ([channel isEqual:@"beta"] ||
             [channel isEqual:@"dev"] ||
             [channel isEqual:@"canary"]) {
    // do nothing.
  } else {
    channel = @"unknown";
  }

  return base::SysNSStringToUTF8(channel);
#else
  return std::string();
#endif*/
}

// static
VersionInfo::Channel VersionInfo::GetChannel() {
#if defined(GOOGLE_CHROME_BUILD)
  std::string channel = GetVersionStringModifier();
  if (channel.empty()) {
    return CHANNEL_STABLE;
  } else if (channel == "beta") {
    return CHANNEL_BETA;
  } else if (channel == "dev") {
    return CHANNEL_DEV;
  } else if (channel == "canary") {
    return CHANNEL_CANARY;
  }
#endif

  return CHANNEL_UNKNOWN;
}

}  // namespace chrome
