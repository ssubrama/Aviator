// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "content/public/common/url_constants.h"

namespace content {

// Before adding new chrome schemes please check with security@chromium.org.
// There are security implications associated with introducing new schemes.
const char kChromeDevToolsScheme[] = "aviator-devtools"; // champion -aviator + chrome
const char kChromeUIScheme[] = "aviator";
const char kGuestScheme[] = "aviator-guest";
const char kMetadataScheme[] = "metadata";
const char kSwappedOutScheme[] = "swappedout";
const char kViewSourceScheme[] = "view-source";

const char kAboutSrcDocURL[] = "about:srcdoc";

const char kChromeUIAppCacheInternalsHost[] = "appcache-internals";
const char kChromeUIIndexedDBInternalsHost[] = "indexeddb-internals";
const char kChromeUIAccessibilityHost[] = "accessibility";
const char kChromeUIBlobInternalsHost[] = "blob-internals";
const char kChromeUIBrowserCrashHost[] = "inducebrowsercrashforrealz";
const char kChromeUIGpuHost[] = "gpu";
const char kChromeUIHistogramHost[] = "histograms";
const char kChromeUIMediaInternalsHost[] = "media-internals";
const char kChromeUINetworkViewCacheHost[] = "view-http-cache";
const char kChromeUIResourcesHost[] = "resources";
const char kChromeUIServiceWorkerInternalsHost[] = "serviceworker-internals";
const char kChromeUITcmallocHost[] = "tcmalloc";
const char kChromeUITracingHost[] = "tracing";
const char kChromeUIWebRTCInternalsHost[] = "webrtc-internals";

const char kChromeUICrashURL[] = "aviator://crash";
const char kChromeUIDumpURL[] = "aviator://crashdump";
const char kChromeUIGpuCleanURL[] = "aviator://gpuclean";
const char kChromeUIGpuCrashURL[] = "aviator://gpucrash";
const char kChromeUIGpuHangURL[] = "aviator://gpuhang";
const char kChromeUIHangURL[] = "aviator://hang";
const char kChromeUIKillURL[] = "aviator://kill";
const char kChromeUIPpapiFlashCrashURL[] = "aviator://ppapiflashcrash";
const char kChromeUIPpapiFlashHangURL[] = "aviator://ppapiflashhang";
const char kChromeUIServiceWorkerInternalsURL[] =
    "aviator://serviceworker-internals";

// This error URL is loaded in normal web renderer processes, so it should not
// have a aviator:// scheme that might let it be confused with a WebUI page.
const char kUnreachableWebDataURL[] = "data:text/html,chromewebdata";

const char kChromeUINetworkViewCacheURL[] = "aviator://view-http-cache/";
const char kChromeUIShorthangURL[] = "aviator://shorthang";

// This URL is loaded when a page is swapped out and replaced by a page in a
// different renderer process.  It must have a unique origin that cannot be
// scripted by other pages in the process.
const char kSwappedOutURL[] = "swappedout://";

}  // namespace content
