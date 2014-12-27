// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "chrome/utility/importer/importer.h"
#include "base/json/json_file_value_serializer.h"

class GURL;
class BookmarkNode;
struct ImportedFaviconUsage;
struct ImportedBookmarkEntry;

namespace sql {
class Connection;
}

// Importer for Google Chrome.
// Google Chrome stores its persistent information in a new system called places.
// http://wiki.mozilla.org/Places
class ChromeImporter : public Importer {
 public:
  ChromeImporter();

  // Importer:
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) OVERRIDE;

 private:
  typedef std::map<int64, std::set<GURL> > FaviconMap;
  //std::vector<importer::URLKeywordInfo> UrlKeyWord;
  virtual ~ChromeImporter();

  void ImportBookmarks();//need to implement 
  void ImportPasswords();//no need to implement for all the browser. because Aviator can't allow user to import paasword from other browsers.  
  void ImportHistory();//no need to implement for all the browser. because Aviator can't allow user to import History from other browsers.
  void ImportSearchEngines();//no need to implement for all the browser. because Aviator can't allow user to import SearchEngines from other browsers.
  
  // Import the user's home page, unless it is set to default home page as
  // defined in browserconfig.properties.
  void ImportHomepage();
  void GetSearchEnginesXMLData(std::vector<std::string>* search_engine_data);

  // The struct stores the information about a bookmark item.
  struct BookmarkItem;
  typedef std::vector<BookmarkItem*> BookmarkList;
  typedef std::vector<BookmarkNode*>BookmarkNodeList;
 
  void ParseChromeBookmarks(const base::string16& toolbar_name,std::vector<ImportedBookmarkEntry>* bookmarks,FaviconMap&);
  void GetTopBookmark(BookmarkNode* node,int,int,BookmarkNodeList*);
  void makeBookMarkNodeToBookmarkItem(BookmarkNodeList*,size_t);
  void ConvertBookMarkNodeToBookMarkItem(BookmarkNode* node);
  void GetWholeBookmarkFolder(BookmarkList* list,size_t position,bool* empty_folder);
  int  returnParentId(int child_id);
  int getParentPosition(int parent_id);
  bool OpenDatabase(sql::Connection* db);
  // Loads the favicons given in the map from the database, loads the data,
  // and converts it into FaviconUsage structures.
  void LoadFavicons(sql::Connection* db,FaviconMap& favicon_map);
	void LoadFaviconData(sql::Connection* db,FaviconMap& favicon_map,std::vector<ImportedFaviconUsage>* favicons);
  base::FilePath source_path_;
  base::FilePath app_path_;
  base::FilePath bookmarks_pref_path_;	
  base::FilePath bookmarks_pref_src_path_;
  base ::FilePath favicon_database_path_;	
  BookmarkList list_;
  std::map<int,int> parentChildId_;
#if defined(OS_POSIX)
  // Stored because we can only access it from the UI thread.
  std::string locale_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ChromeImporter);
};

#endif  // CHROME_BROWSER_IMPORTER_CHROME_IMPORTER_H_
