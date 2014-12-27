// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/utility/importer/importer_creator.h"

#include "base/logging.h"
#include "chrome/utility/importer/bookmarks_file_importer.h"
#include "chrome/utility/importer/firefox_importer.h"
#include "chrome/browser/championconfig/chromeimporter/chrome_importer.h"			//added for champion import bookmarks
#if defined(OS_WIN)
#include "chrome/utility/importer/ie_importer_win.h"
#endif

#if defined(OS_MACOSX)
#include <CoreFoundation/CoreFoundation.h>

#include "base/mac/foundation_util.h"
#include "chrome/utility/importer/safari_importer.h"
#endif

namespace importer {

Importer* CreateImporterByType(ImporterType type) {
  switch (type) {
#if defined(OS_WIN)
    case TYPE_IE:
      return new IEImporter();
#endif
    case TYPE_BOOKMARKS_FILE:
      return new BookmarksFileImporter();
    case TYPE_FIREFOX:
      return new FirefoxImporter();
	/*import_bookmarks-should add here because it should work all the platform */  
	case TYPE_CHROME:
    	return new ChromeImporter(); //added for champion import bookmarks
	/*End of champion import_bookmarks*/		
#if defined(OS_MACOSX)
    case TYPE_SAFARI:
      return new SafariImporter(base::mac::GetUserLibraryPath());
#endif
    default:
      NOTREACHED();
      return NULL;
  }
}

}  // namespace importer
