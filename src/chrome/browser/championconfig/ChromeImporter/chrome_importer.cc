// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/chromeimporter/chrome_importer.h"
#include "chrome/common/importer/importer_data_types.h"
#include <set>
#include "base/file_util.h"

#include "base/files/file_enumerator.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/stl_util.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/common/importer/firefox_importer_utils.h"
#include "chrome/common/importer/firefox_importer_utils.h"
#include "chrome/common/importer/imported_bookmark_entry.h"
#include "chrome/common/importer/imported_favicon_usage.h"
#include "chrome/common/importer/importer_bridge.h"
#include "chrome/common/importer/importer_url_row.h"
#include "chrome/utility/importer/bookmark_html_reader.h"
#include "chrome/utility/importer/favicon_reencode.h"
#include "chrome/utility/importer/nss_decryptor.h"
#include "components/autofill/core/common/password_form.h"
#include "components/bookmarks/browser/bookmark_codec.h"
#include "components/bookmarks/browser/bookmark_index.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "grit/generated_resources.h"
#include "grit/components_strings.h"
#include "sql/connection.h"
#include "sql/statement.h"
#include "url/gurl.h"
namespace {

  enum BookmarkItemType {
    TYPE_BOOKMARK = 1,
    TYPE_FOLDER = 2,
    TYPE_SEPARATOR = 3,
    TYPE_DYNAMIC_CONTAINER = 4
  };

}  // namespace

struct ChromeImporter::BookmarkItem {
  int parent;
  int id;
  GURL url;
  base::string16 title;
  BookmarkItemType type;
  std::string keyword;
  base::Time date_added;
  int64 favicon;
  //int child;
  bool empty_folder;
};

typedef std::vector<ChromeImporter*> ChromeBookmarkList;	
ChromeImporter::ChromeImporter() {
#if defined(OS_POSIX) && !defined(OS_MACOSX)
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  locale_ = g_browser_process->GetApplicationLocale();
#endif

}

ChromeImporter::~ChromeImporter() {
}

void ChromeImporter::StartImport(
  const importer::SourceProfile& source_profile,
  uint16 items,
  ImporterBridge* bridge) {
    bridge_ = bridge;
    source_path_ = source_profile.source_path;
    app_path_ = source_profile.app_path;
	base::FilePath::StringType temp;
	base::FilePath::StringType temp1;	
    temp=source_path_.value();
    temp1=app_path_.value();
#if defined (OS_WIN)
    unsigned found = temp.find_last_of(L"/\\");
#endif
#if defined (OS_MACOSX)
    unsigned found = temp.find_last_of("/\\");
#endif

    base::FilePath::StringType bookmark_file_path=temp.substr(0,(found+1)); //+1 for taking / at the End 
    base::FilePath Bookmark_pref_path(bookmark_file_path);
    base ::FilePath Favicon_DB_pref_path(bookmark_file_path);		
    Bookmark_pref_path = Bookmark_pref_path.AppendASCII("Bookmarks");
    bookmarks_pref_path_ = Bookmark_pref_path;//assign to member variable;

    //Favicon DB path
    Favicon_DB_pref_path = Favicon_DB_pref_path.AppendASCII("Favicons");
    favicon_database_path_ = Favicon_DB_pref_path;//assign to member variable;

    // The order here is important!
    bridge_->NotifyStarted();
    if ((items & importer::HOME_PAGE) && !cancelled())
    {
      bridge_->NotifyItemStarted(importer::HOME_PAGE);
      ImportHomepage();  // Doesn't have a UI item.
      bridge_->NotifyItemEnded(importer::HOME_PAGE);
    }

    // Note history should be imported before bookmarks because bookmark import
    // will also import favicons and we store favicon for a URL only if the URL
    // exist in history or bookmarks.
    if ((items & importer::HISTORY) && !cancelled()) {
      bridge_->NotifyItemStarted(importer::HISTORY);
      ImportHistory();
      bridge_->NotifyItemEnded(importer::HISTORY);
    }

    if ((items & importer::FAVORITES) && !cancelled()) {
      bridge_->NotifyItemStarted(importer::FAVORITES);
      ImportBookmarks();
      bridge_->NotifyItemEnded(importer::FAVORITES);
    }
    if ((items & importer::SEARCH_ENGINES) && !cancelled()) {
      bridge_->NotifyItemStarted(importer::SEARCH_ENGINES);
      ImportSearchEngines();
      bridge_->NotifyItemEnded(importer::SEARCH_ENGINES);
    }
    if ((items & importer::PASSWORDS) && !cancelled()) {
      bridge_->NotifyItemStarted(importer::PASSWORDS);
      ImportPasswords();
      bridge_->NotifyItemEnded(importer::PASSWORDS);
    }
    bridge_->NotifyEnded();
}

/* 
1.no need to implement for importing all settings of the browser. because Aviator can't allow user to import History from other browsers.
2.For Time being leave this implementation but whould implementation for future 
*/

void ChromeImporter::ImportHistory() {

}
bool ChromeImporter :: OpenDatabase(sql::Connection* db)
{
  return db->Open(this->favicon_database_path_);
}

void ChromeImporter::ImportBookmarks() 
{

  // Import favicons.
  std::vector<base::string16> path;	
  std::vector<TemplateURL*> template_urls;
  //base::string16 toolbar_name =bridge_->GetLocalizedString(IDS_BOOKMARK_BAR_FOLDER_NAME);
  base::string16 toolbar_name(base::ASCIIToUTF16("Bookmarks bar")); 
  BookmarkList list;
  std::vector<ImportedBookmarkEntry> bookmarks;
  FaviconMap favicon_map;

  base::FilePath &file = bookmarks_pref_path_;
  bookmarks_pref_src_path_ = 	bookmarks_pref_path_;		
  bool bookmark_file_exists=true;	
  if (!base::PathExists(file))
  {

    //if Main bookmark preference file not exist then check for backup file if not then nothing information there
    file = bookmarks_pref_path_.AppendASCII(".bak");	
    if (!base::PathExists(file))
      bookmark_file_exists=false;
    else
    {
      bookmark_file_exists = true;
      bookmarks_pref_src_path_ =file;	
    }
  }
  //Now read the bookmark file.this is nothing but JSON file
  if(bookmark_file_exists)//if file present then only going read bookmarks file else no need to read at all
  {	

    std::string content;
    base::ReadFileToString(file, &content);
    std::string key ,value;
  }
  //  FaviconMap favicon_map;		
  if (bookmark_file_exists) 
    ParseChromeBookmarks(toolbar_name, &bookmarks,favicon_map);

  if (!bookmarks.empty() && !cancelled()) 
  {
    const base::string16& first_folder_name = bridge_->GetLocalizedString(IDS_BOOKMARK_GROUP_FROM_CHROME);
    bridge_->AddBookmarks(bookmarks, first_folder_name);
  }
  sql::Connection db;
  if (!OpenDatabase(&db))
    return;

  LoadFavicons(&db, favicon_map);
  // Write favicons into profile.			
  if (!favicon_map.empty() && !cancelled()) {
    std::vector<ImportedFaviconUsage> favicons;
    LoadFaviconData(&db, favicon_map, &favicons);
    bridge_->SetFavicons(favicons);
  }
}

void ChromeImporter::LoadFaviconData(sql::Connection* db,FaviconMap& favicon_map,std::vector<ImportedFaviconUsage>* favicons)
{
  //SELECT i.url,d.image_data FROM favicons i JOIN favicon_bitmaps d ON i.id = d.icon_id WHERE i.id =2; 	
  const char* query ="SELECT i.url,d.image_data "
    "FROM favicons i JOIN favicon_bitmaps d "
    "ON i.id = d.icon_id  "
    "WHERE i.id = ?;";

  sql::Statement s(db->GetUniqueStatement(query));

  /* if (!s.is_valid())
  return; */

  for (FaviconMap::const_iterator i = favicon_map.begin();i != favicon_map.end(); ++i) 
  {
    s.BindInt64(0, i->first);
    if (s.Step()) {
      ImportedFaviconUsage usage;

      usage.favicon_url = GURL(s.ColumnString(0));
      if (!usage.favicon_url.is_valid())
        continue;  // Don't bother importing favicons with invalid URLs.

      std::vector<unsigned char> data;
      s.ColumnBlobAsVector(1, &data);
      if (data.empty())
        continue;  // Data definitely invalid.

      if (!importer::ReencodeFavicon(&data[0], data.size(), &usage.png_data))
        continue;  // Unable to decode.

      usage.urls = i->second;
      favicons->push_back(usage);
    }
    s.Reset(true);
  }
}

void ChromeImporter::LoadFavicons(sql::Connection* db,FaviconMap& favicon_map) {
  const char* query = "SELECT icon_id, page_url FROM icon_mapping;";
  sql::Statement s(db->GetUniqueStatement(query));
  while (s.Step() && !cancelled()) 
  {
    int64 icon_id = s.ColumnInt64(0);
    GURL url = GURL(s.ColumnString(1));
    (favicon_map)[icon_id].insert(url);
  }
}

void ChromeImporter :: GetTopBookmark(BookmarkNode* node,int parent_id,int folder_id,BookmarkNodeList* nodeList){

  //scan the root and put each indivisual node inside vector
  //node type is BOOKMARK_BAR.This can be FOLDER type also
  //first set the parent id and node id
  parentChildId_[folder_id] = parent_id;
  nodeList->push_back(node);
}

// Returns true if |url| has a valid scheme that we allow to import. We
// filter out the URL with a unsupported scheme.
bool CanImportURL(const GURL& url) {
  // The URL is not valid.
  if (!url.is_valid())
    return false;

  // Filter out the URLs with unsupported schemes.
  const char* const kInvalidSchemes[] = {"wyciwyg", "place", "about", "chrome","aviator"};
  for (size_t i = 0; i < arraysize(kInvalidSchemes); ++i) {
    if (url.SchemeIs(kInvalidSchemes[i]))
      return false;
  }
  return true;
}

void ChromeImporter::ConvertBookMarkNodeToBookMarkItem(BookmarkNode* node)
{
  BookmarkItem* item= new BookmarkItem;
  item->id=node->id();
  int parent_id=returnParentId(item->id);
  item->parent=parent_id;
  item->url=GURL(node->url());
  item->title=node->GetTitle();

  if(node->type() == 0)
    item->type=TYPE_BOOKMARK;
  else if(node->type() == 1)
    item->type =TYPE_FOLDER;
  item->date_added=node->date_added();
  item->favicon=0;
  item->empty_folder=node->child_count()?true:false;
  list_.push_back(item);
}

void ChromeImporter::makeBookMarkNodeToBookmarkItem(BookmarkNodeList *node,size_t pos)
{
    BookmarkNodeList temp_list;
    BookmarkNode*temp =(*node)[pos];
    temp_list.push_back(temp);
    if(temp->is_folder())
    {
      for(int i=0;i<temp->child_count();i++)
        GetTopBookmark(temp->GetChild(i),temp->id(),temp->GetChild(i)->id(),node);
    }
    /*else
    {
      ConvertBookMarkNodeToBookMarkItem((*node)[pos]);
    }*/
}


int ChromeImporter ::returnParentId(int child_id)
{
  std::map<int,int>::iterator it=parentChildId_.find(child_id);
   if (it == parentChildId_.end()) {
    return -1;
  }
   else
     return it->second;

}

int ChromeImporter ::getParentPosition(int parent_id){

  int i=0;
  int count =list_.size();
  int pos=-1;
  for(i=0;i<count;i++)
  {
    BookmarkItem *temp=list_[i];
    if(temp->id == parent_id)
    {
      return i;
    }
  }
    return pos;
}
void ChromeImporter::ParseChromeBookmarks(const base::string16& toolbar_name,std::vector<ImportedBookmarkEntry>* bookmarks,FaviconMap& favicon_map)
{
  JSONFileValueSerializer serializer(bookmarks_pref_src_path_);
  scoped_ptr<base::Value> root(serializer.Deserialize(NULL, NULL));
  int toolbar_folder_id = -1;
  int other_folder_id = -1;
  int mobile_folder_id = -1;
  int parent_id=-1;
  //int invalid_folder_id = -1;
  if (root.get())
  {
    int64 max_node_id = 0;
    BookmarkNode* bb_node = new BookmarkNode(GURL(""));
    BookmarkNode* other_folder_node = new BookmarkNode(GURL(""));
    BookmarkNode* mobile_folder_node = new BookmarkNode(GURL(""));
    bookmarks::BookmarkCodec codec;
    parent_id=-1;
    codec.Decode(bb_node, other_folder_node,mobile_folder_node, &max_node_id, *root.get());
    BookmarkNodeList nodeList;
    toolbar_folder_id =bb_node->id();
    other_folder_id = other_folder_node->id();
    mobile_folder_id =mobile_folder_node->id();
    GetTopBookmark(bb_node,parent_id,toolbar_folder_id,&nodeList);
    GetTopBookmark(other_folder_node,parent_id,other_folder_id,&nodeList);
    GetTopBookmark(mobile_folder_node,parent_id,mobile_folder_id,&nodeList);
    
    for(size_t i=0;i<nodeList.size();i++)
      makeBookMarkNodeToBookmarkItem(&nodeList,i);
   
    for(size_t i=0;i<nodeList.size();i++)
      ConvertBookMarkNodeToBookMarkItem(nodeList[i]);
    
    // Find the bookmark path by tracing their links to parent folders.
    std::vector<base::string16> path;
    for (size_t i = 0; i < list_.size(); ++i)
    {
      BookmarkItem* item = list_[i];
      if (item->type == TYPE_FOLDER)
      {
         // Folders are added implicitly on adding children, so we only explicitly
        if(!item->id && item->title.length() == 0 )
          continue;

        //folder without name does not any value skip it
        if(item->title.length() == 0 )
          continue;

        // no need to add empty folders.
        if (!item->empty_folder)
         continue;
      } 
      else if (item->type == TYPE_BOOKMARK)
      {
         // Import only valid bookmarks
        if (!CanImportURL(item->url))
          continue;
      } 
      else 
        continue;

      
      BookmarkItem* child = item;
      bool found_path = false;
      bool is_in_toolbar = false;
      BookmarkItem* currentparent=NULL;
      path.clear();
      while (child->parent >= 0) 
      {
        int parent_id= returnParentId(child->id);
        if(parent_id == 0 )
          break;
        else
        {
          int pos=getParentPosition(parent_id);
          if(pos == -1)
            break;
          currentparent = list_[ pos ];
        }
        if (currentparent->id != 0)
        {
          // To avoid excessive nesting, omit the name for the bookmarks menu
          // folder.
          path.insert(path.begin(), currentparent->title);
         }
        if (currentparent->id == toolbar_folder_id)
          is_in_toolbar = true;
        if (currentparent->id == toolbar_folder_id || currentparent->id == other_folder_id || currentparent->id == mobile_folder_id) 
        {
          // We've reached a root node, hooray!
          found_path = true;
          break;
        }
        child = currentparent;
      }

      if (!found_path )
        continue;
      
      //This need to consider
	  #if defined (OS_WIN)
      base::string16 av=L"Aviator";
      if(!item->title.compare(av))
        continue;
      #endif
      ImportedBookmarkEntry entry;
      entry.creation_time = item->date_added;
      entry.title = item->title;
      entry.url = item->url;
      entry.path=path;
      entry.in_toolbar = is_in_toolbar;
      entry.is_folder = item->type == TYPE_FOLDER;
      bookmarks->push_back(entry);
    }
  }
 }

/* 
1.no need to implement for all the browser. because Aviator can't allow user to import History from other browsers.
2.For Time being leave this implementation but whould implementation for future 
*/
void ChromeImporter::ImportPasswords() {

}
/* 
1.no need to implement for all the browser. because Aviator can't allow user to import History from other browsers.
2.For Time being leave this implementation but whould implementation for future 
*/
void ChromeImporter::ImportSearchEngines() {

}

void ChromeImporter::ImportHomepage() {
  GURL home_page = GetHomepage(source_path_);
  if (home_page.is_valid() && !IsDefaultHomepage(home_page, app_path_)) {
    bridge_->AddHomePage(home_page);
  }
}

