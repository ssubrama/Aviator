// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

// This defines an enumeration of IDs that can uniquely identify a view within
// the scope of a container view.

#ifndef CHROME_BROWSER_UI_VIEW_IDS_H_
#define CHROME_BROWSER_UI_VIEW_IDS_H_

enum ViewID {
  VIEW_ID_NONE = 0,

  // BROWSER WINDOW VIEWS
  // ------------------------------------------------------

  // Views which make up the skyline. These are used only
  // on views.
  VIEW_ID_MINIMIZE_BUTTON,
  VIEW_ID_MAXIMIZE_BUTTON,
  VIEW_ID_RESTORE_BUTTON,
  VIEW_ID_CLOSE_BUTTON,
  VIEW_ID_WINDOW_ICON,
  VIEW_ID_WINDOW_TITLE,
  VIEW_ID_AVATAR_LABEL,
  VIEW_ID_AVATAR_BUTTON,
  VIEW_ID_NEW_AVATAR_BUTTON,

  // Tabs within a window/tab strip, counting from the left.
  VIEW_ID_TAB_0,
  VIEW_ID_TAB_1,
  VIEW_ID_TAB_2,
  VIEW_ID_TAB_3,
  VIEW_ID_TAB_4,
  VIEW_ID_TAB_5,
  VIEW_ID_TAB_6,
  VIEW_ID_TAB_7,
  VIEW_ID_TAB_8,
  VIEW_ID_TAB_9,
  VIEW_ID_TAB_LAST,

  // ID for any tab. Currently only used on views.
  VIEW_ID_TAB,

  VIEW_ID_TAB_STRIP,

  // Toolbar & toolbar elements.
  VIEW_ID_TOOLBAR = 1000,
  VIEW_ID_BACK_BUTTON,
  VIEW_ID_FORWARD_BUTTON,
  VIEW_ID_RELOAD_BUTTON,
  VIEW_ID_HOME_BUTTON,
  VIEW_ID_STAR_BUTTON,
  VIEW_ID_APP_MENU,
  VIEW_ID_BROWSER_ACTION_TOOLBAR,
  VIEW_ID_BROWSER_ACTION,
  VIEW_ID_FEEDBACK_BUTTON,
  VIEW_ID_OMNIBOX,
  VIEW_ID_SCRIPT_BUBBLE,
  VIEW_ID_PROTECTED_BUTTON,   //champion : added for protected
  VIEW_ID_UNPROTECTED_BUTTON, //champion : added for unprotected

  //champion iconbar
  VIEW_ID_ICONBAR=2000,
  
  VIEW_ID_TRANSLATE_BUTTON,

  // The Bookmark Bar.
  VIEW_ID_BOOKMARK_BAR,
  VIEW_ID_OTHER_BOOKMARKS,
  VIEW_ID_MANAGED_BOOKMARKS,
  // Used for bookmarks/folders on the bookmark bar.
  VIEW_ID_BOOKMARK_BAR_ELEMENT,

  // Find in page.
  VIEW_ID_FIND_IN_PAGE_TEXT_FIELD,

  // Tab Container window.
  VIEW_ID_TAB_CONTAINER,

  // Docked dev tools.
  VIEW_ID_DEV_TOOLS_DOCKED,

  // The contents split.
  VIEW_ID_CONTENTS_SPLIT,

  // The Infobar container.
  VIEW_ID_INFO_BAR_CONTAINER,

  // The Download shelf.
  VIEW_ID_DOWNLOAD_SHELF,

// TODO(dbeam): change the zoom decoration to an NSImageView on Mac so IDs work.
#if !defined(OS_MACOSX)
  // Zoom button in location bar.
  VIEW_ID_ZOOM_BUTTON,
#endif

// TODO(npentrel): change the passwords decoration to an NSImageView.
#if !defined(OS_MACOSX)
  // The manage passwords icon in the location bar.
  VIEW_ID_MANAGE_PASSWORDS_ICON_BUTTON,
#endif

  // The omnibox icon to do voice-based search.
  VIEW_ID_MIC_SEARCH_BUTTON,

  // Used in chrome/browser/ui/gtk/view_id_util_browsertest.cc
  // If you add new ids, make sure the above test passes.
  VIEW_ID_PREDEFINED_COUNT,

  // Plus button on location bar.
  VIEW_ID_ACTION_BOX_BUTTON,
};

#endif  // CHROME_BROWSER_UI_VIEW_IDS_H_
