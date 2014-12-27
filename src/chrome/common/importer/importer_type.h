// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_COMMON_IMPORTER_IMPORTER_TYPE_H_
#define CHROME_COMMON_IMPORTER_IMPORTER_TYPE_H_

#include "build/build_config.h"

namespace importer {

// An enumeration of the type of importers that we support to import
// settings and data from (browsers, google toolbar and a bookmarks html file).
// NOTE: Numbers added so that data can be reliably cast to ints and passed
// across IPC.
enum ImporterType {
  TYPE_UNKNOWN         = -1,
#if defined(OS_WIN)
  TYPE_IE              = 0,
#endif
  // Value 1 was the (now deleted) Firefox 2 profile importer.
  TYPE_FIREFOX         = 2,
#if defined(OS_MACOSX)
  TYPE_SAFARI          = 3,
#endif
  // Value 4 was the (now deleted) Google Toolbar importer.
  TYPE_BOOKMARKS_FILE  = 5,  // Identifies a 'bookmarks.html' file.
  TYPE_CHROME 	=	6		// added for champion import bookmark for chrome 
};

}  // namespace importer


#endif  // CHROME_COMMON_IMPORTER_IMPORTER_TYPE_H_
