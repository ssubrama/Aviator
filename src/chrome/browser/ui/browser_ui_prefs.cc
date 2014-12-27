// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/browser/ui/browser_ui_prefs.h"

#include "base/prefs/pref_registry_simple.h"
#include "base/prefs/pref_service.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/translate/core/common/translate_pref_names.h"

#include "chrome/browser/championconfig/varsconfig.h" // champion 

namespace chrome {

void RegisterBrowserPrefs(PrefRegistrySimple* registry) {
  registry->RegisterIntegerPref(prefs::kOptionsWindowLastTabIndex, 0);
  registry->RegisterBooleanPref(prefs::kAllowFileSelectionDialogs, true);
  registry->RegisterIntegerPref(prefs::kShowFirstRunBubbleOption,
                             first_run::FIRST_RUN_BUBBLE_DONT_SHOW);
}

void RegisterBrowserUserPrefs(user_prefs::PrefRegistrySyncable* registry) {
  /*registry->RegisterBooleanPref(
      prefs::kHomePageChanged,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kHomePageIsNewTabPage,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kShowHomeButton,
      false,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); */
  // champion - homepage feature
  std::string homePageWithDisconnect = HOMEPAGE_URL;
    homePageWithDisconnect += "#searchEngine=Google"; 
  registry->RegisterStringPref(prefs::kHomePage,
      homePageWithDisconnect,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterBooleanPref(prefs::kHomePageIsNewTabPage,
      false,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  /* Champion artf414382 Build library on Mac	*/                             
  /* Champion artf360018 : ENABLE "Show Home button" true */
  if(SHOW_HOME_BUTTON) {
    //MessageBox("if part prefs::kShowHomeButton",prefs::kShowHomeButton,1);
    registry->RegisterBooleanPref(prefs::kShowHomeButton,
        true,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  } else {
    //MessageBox("else part prefs::kShowHomeButton",prefs::kShowHomeButton,1);
    registry->RegisterBooleanPref(prefs::kShowHomeButton,
        false,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  }
  //End of Champion
           
#if defined(OS_MACOSX)
  // This really belongs in platform code, but there's no good place to
  // initialize it between the time when the AppController is created
  // (where there's no profile) and the time the controller gets another
  // crack at the start of the main event loop. By that time,
  // StartupBrowserCreator has already created the browser window, and it's too
  // late: we need the pref to be already initialized. Doing it here also saves
  // us from having to hard-code pref registration in the several unit tests
  // that use this preference.
  registry->RegisterBooleanPref(
      prefs::kShowUpdatePromotionInfoBar,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
#endif
  registry->RegisterBooleanPref(
      prefs::kDeleteBrowsingHistory,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDeleteDownloadHistory,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDeleteCache,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDeleteCookies,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  /* Champion artf414382 Build library on Mac 	*/   
  /* Champion artf360025	: ENABLE "Clear cookies and other site data when I quit my browser"  -false +true || 
  advanced->clear browsing data->Clear saved passwords (its to be checked always ) */
  if(CLEAR_DATA_PASSWORD) {  
    registry->RegisterBooleanPref(
        prefs::kDeletePasswords,
        true,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  } else {
    registry->RegisterBooleanPref(
        prefs::kDeletePasswords,
        false,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); // default
  }
  /* Champion artf414382 Build library on Mac 	*/   
  /* Champion artf360025	: ENABLE "Clear cookies and other site data when I quit my browser"  -false +true  || advanced->clear browsing data->Clear saved Autofill form data (its to be checked always ) */    
  if(CLEAR_DATA_FORMDATA) {  
    registry->RegisterBooleanPref(
        prefs::kDeleteFormData,
        true,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); 
  } else {
    registry->RegisterBooleanPref(
        prefs::kDeleteFormData,
        false,
        user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); // default
  }
  registry->RegisterBooleanPref(
      prefs::kDeleteHostedAppsData,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); // champion -false +true
  registry->RegisterIntegerPref(
      prefs::kDeleteTimePeriod,
      4,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); // champion -0 +4
  registry->RegisterInt64Pref(
      prefs::kLastClearBrowsingDataTime,
      0,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterIntegerPref(prefs::kModuleConflictBubbleShown,
      0,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kCheckDefaultBrowser,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kWebAppCreateOnDesktop,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kWebAppCreateInAppsMenu,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kWebAppCreateInQuickLaunchBar,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kEnableTranslate,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterStringPref(
      prefs::kCloudPrintEmail,
      std::string(),
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kCloudPrintProxyEnabled,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kCloudPrintSubmitEnabled,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDevToolsDisabled,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kBrowserWindowPlacement,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kBrowserWindowPlacementPopup,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kPreferencesWindowPlacement,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
	  
  /* Champion artf360053:BUG Import data did not bring over Firefox passwords // -true +false */
   
  if(IMPORT_SAVED_PASSWORDS) { 
    registry->RegisterBooleanPref(
        prefs::kImportBookmarks,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);                          	
    registry->RegisterBooleanPref(
        prefs::kImportHistory,
        true,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
    registry->RegisterBooleanPref(
        prefs::kImportHomepage,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
    registry->RegisterBooleanPref(
        prefs::kImportSearchEngine,
        true,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
    registry->RegisterBooleanPref(
        prefs::kImportSavedPasswords,
        true,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  } else {
    registry->RegisterBooleanPref(
        prefs::kImportBookmarks,
        true,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);                          	
    registry->RegisterBooleanPref(
        prefs::kImportHistory,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
    registry->RegisterBooleanPref(
        prefs::kImportSearchEngine,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
    registry->RegisterBooleanPref(
        prefs::kImportSavedPasswords,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  } 
  registry->RegisterBooleanPref(
      prefs::kImportHomepage,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);  
  registry->RegisterBooleanPref(
      prefs::kEnableDoNotTrack,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);/*champion  -false + true - Default Enable for track */	

  // Dictionaries to keep track of default tasks in the file browser.
  registry->RegisterDictionaryPref(
      prefs::kDefaultTasksByMimeType,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kDefaultTasksBySuffix,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);

  // We need to register the type of these preferences in order to query
  // them even though they're only typically controlled via policy.
  registry->RegisterBooleanPref(
      prefs::kPluginsAllowOutdated,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  if(!CHAMPION_INTERNAL_BROWSER) { //Added for champion for always click to play 
    registry->RegisterBooleanPref(
        prefs::kPluginsAlwaysAuthorize,
        true,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  } else {
    registry->RegisterBooleanPref(
        prefs::kPluginsAlwaysAuthorize,
        false,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  }
  registry->RegisterBooleanPref(
      prefs::kClearPluginLSODataEnabled,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kHideWebStoreIcon,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
   #if defined(OS_MACOSX)
     registry->RegisterBooleanPref(prefs::kNotificationKeyChainAccess,
       false,
     user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);  /*champion - added for keycahin navneet test */ 
   #endif  // defined(OS_MACOSX)  
  registry->RegisterBooleanPref(prefs::kWarnBeforeQuitEnabled,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);  /*champion - added for warn_before_quit */	
  registry->RegisterBooleanPref(prefs::kRestartManual,
      false,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF); /*champion - added for Manual Restart */
   registry->RegisterBooleanPref(prefs::kDefaultSearchPopUp,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);  /*champion - Default Search Engine change Popup - Navneet */	
      // champion : Need to register only in mac (Amresh)
#ifdef OS_MACOSX
	  registry->RegisterStringPref(prefs::kDisplayAlternateBrowser,
      "Safari",
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);		
#endif      
#if !defined(OS_MACOSX)
  registry->RegisterBooleanPref(
      prefs::kFullscreenAllowed,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
#endif
}

void RegisterAppPrefs(const std::string& app_name, Profile* profile) {
  // We need to register the window position pref.
  //
  // TODO(mnissler): Use a separate pref name pointing to a single
  // dictionary instead. Also tracked as http://crbug.com/167256
  std::string window_pref(prefs::kBrowserWindowPlacement);
  window_pref.append("_");
  window_pref.append(app_name);
  PrefService* prefs = profile->GetPrefs();
  if (!prefs->FindPreference(window_pref.c_str())) {
    // TODO(joi): Do all registration up front.
    scoped_refptr<user_prefs::PrefRegistrySyncable> registry(
        static_cast<user_prefs::PrefRegistrySyncable*>(
            prefs->DeprecatedGetPrefRegistry()));
    registry->RegisterDictionaryPref(
        window_pref.c_str(), user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  }
}


}  // namespace chrome
