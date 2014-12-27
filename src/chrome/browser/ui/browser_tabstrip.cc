// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/ui/browser_tabstrip.h"

#include "base/command_line.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_navigator.h"
#include "chrome/browser/ui/tab_contents/core_tab_helper.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"

// champion : headers start
#if defined(OS_WIN)
#include "chrome/browser/ui/views/frame/browser_view.h"       
#include "chrome/browser/championconfig/RestartUpdate/RestartforUpdate.h"      // champion : auto focus on addressbar (balaji)
#endif
#include "chrome/browser/ui/views/location_bar/location_bar_view.h" // champion : auto focus on addressbar (balaji)

#include "components/policy/core/browser/url_blacklist_manager.h"
#include "chrome/browser/championconfig/varsconfig.h"
#include "components/url_fixer/url_fixer.h"
#include "chrome/common/pref_names.h"
#include "chrome/browser/profiles/profile_impl.h"
#include "base/prefs/pref_service.h"
#include <fstream>

#include "chrome/browser/championconfig/chromeimporter/chrome_importer_utils.h" // champion : Aviator path  (Dinesh)
#define WINDOWVERSIONXP "5.1" 
// champion : ends

namespace chrome {

//const char kChromeUIHomePageURL[]="https://www.whitehatsec.com/securebrowser";   
const char kChromeUIHomePageURL[]=HOMEPAGE_URL;//champion - to open new tab with home page - navneet 

// champion : check if given file exists (amresh)
#if defined(OS_WIN)
bool FileExists (const std::wstring& name) {
    std::ifstream f(name.c_str());
    if (f.is_open()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

// champion : check if given file exists (amresh)
bool FileExistsString (const std::string& name) {
   std::ifstream f(name.c_str());
    if (f.is_open()) {
         f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}
#endif
void AddTabAt(Browser* browser, const GURL& url, int idx, bool foreground) {
  // Time new tab page creation time.  We keep track of the timing data in
  // WebContents, but we want to include the time it takes to create the
  // WebContents object too.
  GURL home_page(url_fixer::FixupURL(browser->profile()->GetPrefs()->GetString(prefs::kHomePage),std::string()));
  if (browser->profile()->GetPrefs()->GetBoolean(prefs::kHomePageIsNewTabPage)) {
    base::TimeTicks new_tab_start_time = base::TimeTicks::Now();
    chrome::NavigateParams params(browser, GURL(chrome::kChromeUIHomePageURL/*chrome::kChromeUINewUnprotectedWindowURL*/),
      content::PAGE_TRANSITION_TYPED);
    params.disposition = foreground ? NEW_FOREGROUND_TAB : NEW_BACKGROUND_TAB;
    params.tabstrip_index = idx;
    chrome::Navigate(&params);
    CoreTabHelper* core_tab_helper = 
      CoreTabHelper::FromWebContents(params.target_contents);
    core_tab_helper->set_new_tab_start_time(new_tab_start_time);
  } else {
    base::TimeTicks new_tab_start_time = base::TimeTicks::Now();	
    chrome::NavigateParams params(browser, home_page, 
      content::PAGE_TRANSITION_TYPED);
    params.disposition = foreground ? NEW_FOREGROUND_TAB : NEW_BACKGROUND_TAB;
    params.tabstrip_index = idx;
    chrome::Navigate(&params);
    CoreTabHelper* core_tab_helper = 
      CoreTabHelper::FromWebContents(params.target_contents);
    core_tab_helper->set_new_tab_start_time(new_tab_start_time);
    // champion : auto focus on addressbar (balaji)
   #if defined(OS_WIN)
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
    if (browser_view) {
      LocationBarView* location_bar = browser_view->GetLocationBarView();
      if (location_bar)
        location_bar->FocusLocation(true);
    }
    #endif
    // champion : ends
  }
 #if defined(OS_WIN)
  std::string Aviatorpath = GetAviatorInstallPathFromRegistry32bitW();
  std::string Aviatorpathchag;

  if (!views::UpdateMechwindow::dismiss) {
    if (!Aviatorpath.empty()){
      std::string checkstring = Aviatorpath;
      unsigned found = checkstring.find("Aviator");
      Aviatorpathchag.append(checkstring.substr(0, found));
      Aviatorpathchag.append("Update\\Aviator.msi");
    }
    if (FileExistsString(Aviatorpathchag)) {
      views::UpdateMechDialog(ADT::QUIT_ON_CLOSE, browser);
    }
	 
  }
  #endif 
}

content::WebContents* AddSelectedTabWithURL(
    Browser* browser,
    const GURL& url,
    content::PageTransition transition) {
  chrome::NavigateParams params(browser, url, transition);
  params.disposition = NEW_FOREGROUND_TAB;
  Navigate(&params);
  // champion : auto focus on addressbar (balaji)
    #if defined(OS_WIN)
  BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
  if (browser_view) {
    LocationBarView* location_bar = browser_view->GetLocationBarView();
    if (location_bar)
      location_bar->FocusLocation(true);
  }
   #endif
  // champion : ends
  return params.target_contents;
}

void AddWebContents(Browser* browser,
                    content::WebContents* source_contents,
                    content::WebContents* new_contents,
                    WindowOpenDisposition disposition,
                    const gfx::Rect& initial_pos,
                    bool user_gesture,
                    bool* was_blocked) {
  // No code for this yet.
  DCHECK(disposition != SAVE_TO_DISK);
  // Can't create a new contents for the current tab - invalid case.
  DCHECK(disposition != CURRENT_TAB);

  chrome::NavigateParams params(browser, new_contents);
  params.source_contents = source_contents;
  params.disposition = disposition;
  params.window_bounds = initial_pos;
  params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  // At this point, we're already beyond the popup blocker. Even if the popup
  // was created without a user gesture, we have to set |user_gesture| to true,
  // so it gets correctly focused.
  params.user_gesture = true;
  Navigate(&params);
}

void CloseWebContents(Browser* browser,
                      content::WebContents* contents,
                      bool add_to_history) {
  int index = browser->tab_strip_model()->GetIndexOfWebContents(contents);
  if (index == TabStripModel::kNoTab) {
    NOTREACHED() << "CloseWebContents called for tab not in our strip";
    return;
  }

  browser->tab_strip_model()->CloseWebContentsAt(
      index,
      add_to_history ? TabStripModel::CLOSE_CREATE_HISTORICAL_TAB
                     : TabStripModel::CLOSE_NONE);
}

}  // namespace chrome
