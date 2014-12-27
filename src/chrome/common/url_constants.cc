// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/common/url_constants.h"

#include <algorithm>

#include "base/basictypes.h"
#include "content/public/common/url_constants.h"
#include "url/url_util.h"

// Champion artf414382 Build library on Mac	
#include "chrome/browser/championconfig/varsconfig.h" 

namespace chrome {

#if defined(OS_CHROMEOS)
const char kCrosScheme[] = "cros";
const char kDriveScheme[] = "drive";
#endif

// Add Chrome UI URLs as necessary, in alphabetical order.
// Be sure to add the corresponding kChromeUI*Host constant below.
// This is a WebUI page that lists other WebUI pages.
const char kChromeUIAboutURL[] = "aviator://about/";
const char kChromeUIAppsURL[] = "aviator://apps/";
const char kChromeUIAppListStartPageURL[] = "aviator://app-list/";
const char kChromeUIBookmarksURL[] = "aviator://bookmarks/";
const char kChromeUICertificateViewerURL[] = "aviator://view-cert/";
const char kChromeUICertificateViewerDialogURL[] = "aviator://view-cert-dialog/";
const char kChromeUIChromeSigninURL[] = "aviator://aviator-signin/";
const char kChromeUIChromeURLsURL[] = "aviator://aviator-urls/";
const char kChromeUICloudPrintResourcesURL[] = "aviator://cloudprintresources/";
const char kChromeUIComponentsURL[] = "aviator://components/";
const char kChromeUIConflictsURL[] = "aviator://conflicts/";
const char kChromeUIConstrainedHTMLTestURL[] = "aviator://constrained-test/";
const char kChromeUICrashesURL[] = "aviator://crashes/";
const char kChromeUICreditsURL[] = "aviator://credits/";
const char kChromeUIDevicesURL[] = "aviator://devices/";
const char kChromeUIDevToolsURL[] =
    "aviator-devtools://devtools/bundled/devtools.html";
const char kChromeUIDownloadsURL[] = "aviator://downloads/";
const char kChromeUIEditSearchEngineDialogURL[] = "aviator://editsearchengine/";
const char kChromeUIExtensionIconURL[] = "aviator://extension-icon/";
const char kChromeUIExtensionInfoURL[] = "aviator://extension-info/";
const char kChromeUIExtensionsFrameURL[] = "aviator://extensions-frame/";
const char kChromeUIExtensionsURL[] = "aviator://extensions/";
const char kChromeUIFaviconURL[] = "aviator://favicon/";
const char kChromeUIFeedbackURL[] = "aviator://feedback/";
const char kChromeUIFlagsURL[] = "aviator://flags/";
const char kChromeUIFlashURL[] = "aviator://flash/";
const char kChromeUIGCMInternalsURL[] = "aviator://gcm-internals/";
const char kChromeUIHelpFrameURL[] = "aviator://help-frame/";
const char kChromeUIHistoryURL[] = "aviator://history/";
const char kChromeUIHistoryFrameURL[] = "aviator://history-frame/";
const char kChromeUIIdentityInternalsURL[] = "aviator://identity-internals/";
const char kChromeUIInspectURL[] = "aviator://inspect/";
const char kChromeUIInstantURL[] = "aviator://instant/";
const char kChromeUIInvalidationsURL[] = "aviator://invalidations/";
const char kChromeUIIPCURL[] = "aviator://ipc/";
const char kChromeUIManagedUserPassphrasePageURL[] =
    "aviator://managed-user-passphrase/";
const char kChromeUIMemoryRedirectURL[] = "aviator://memory-redirect/";
const char kChromeUIMemoryURL[] = "aviator://memory/";
const char kChromeUIMetroFlowURL[] = "aviator://make-metro/";
const char kChromeUINaClURL[] = "aviator://nacl/";
const char kChromeUINetInternalsURL[] = "aviator://net-internals/";
const char kChromeUINewProfile[] = "aviator://newprofile/";
const char kChromeUINewTabURL[] = HOMEPAGE_URL ;//"aviator://newtab/"; // Champion : To open Default home page if new tab option is selected - (Navneet)
const char kChromeUIOmniboxURL[] = "aviator://omnibox/";
const char kChromeUIPerformanceMonitorURL[] = "aviator://performance/";
const char kChromeUIPluginsURL[] = "aviator://plugins/";
const char kChromeUIPolicyURL[] = "aviator://policy/";
const char kChromeUIProfileSigninConfirmationURL[] =
    "aviator://profile-signin-confirmation/";
const char kChromeUIUserManagerURL[] = "aviator://user-manager/";
const char kChromeUIPrintURL[] = "aviator://print/";
const char kChromeUIQuitURL[] = "aviator://quit/";
const char kChromeUIRestartURL[] = "aviator://restart/";
const char kChromeUISettingsURL[] = "aviator://settings/";
const char kChromeUISettingsFrameURL[] = "aviator://settings-frame/";
const char kChromeUISuggestions[] = "aviator://suggestions/";
const char kChromeUISuggestionsInternalsURL[] =
    "aviator://suggestions-internals/";
const char kChromeUISSLClientCertificateSelectorURL[] = "aviator://select-cert/";
const char kChromeUITermsURL[] = "aviator://terms/";
const char kChromeUIThemeURL[] = "aviator://theme/";
const char kChromeUIThumbnailURL[] = "aviator://thumb/";
const char kChromeUIThumbnailListURL[] = "aviator://thumbnails/";
const char kChromeUIUberURL[] = "aviator://aviator/";
const char kChromeUIUberFrameURL[] = "aviator://uber-frame/";
const char kChromeUIUserActionsURL[] = "aviator://user-actions/";
const char kChromeUIVersionURL[] = "aviator://version/";

const char kChromeUINewUnprotectedWindowURL[]=HOMEPAGE_URL; //"aviator://newtab/";//Added for champion for unprotected window
const char kChromeUIFeedbackURL1[]="www.whitehatsec.com/aviator/contactus/";//Added for champion for unprotected window
//std::string strHomePageURL(HOMEPAGE_URL);
//const char * kChromeUINewTabURL=strHomePageURL.c_str();

// CR_DEFINE_STATIC_LOCAL(std::string, strHomePageURL, HOMEPAGE_URL);
//strHomePageURL(HOMEPAGE_URL);

// champion : static build fix (amresh)
// It was reported to be available in web_contents_impl.cc
//const char kChromeUIHomePageURL1[]="https://www.whitehatsec.com/securebrowser";//strHomePageURL.c_str();  //Added for champion for Focus on Home Page

#if defined(OS_ANDROID)
const char kChromeUINativeNewTabURL[] = "aviator-native://newtab/";
const char kChromeUINativeBookmarksURL[] = "aviator-native://bookmarks/";
const char kChromeUINativeRecentTabsURL[] = "aviator-native://recent-tabs/";
const char kChromeUIWelcomeURL[] = "aviator://welcome/";
#endif

#if defined(OS_CHROMEOS)
const char kChromeUIActivationMessage[] = "aviator://activationmessage/";
const char kChromeUIBluetoothPairingURL[] = "aviator://bluetooth-pairing/";
const char kChromeUICertificateManagerDialogURL[] =
    "aviator://certificate-manager/";
const char kChromeUIChargerReplacementURL[] = "aviator://charger-replacement/";
const char kChromeUIChooseMobileNetworkURL[] =
    "aviator://choose-mobile-network/";
const char kChromeUIDiscardsURL[] = "aviator://discards/";
const char kChromeUIFirstRunURL[] = "aviator://first-run/";
const char kChromeUIIdleLogoutDialogURL[] = "aviator://idle-logout/";
const char kChromeUIImageBurnerURL[] = "aviator://imageburner/";
const char kChromeUIKeyboardOverlayURL[] = "aviator://keyboardoverlay/";
const char kChromeUILockScreenURL[] = "aviator://lock/";
const char kChromeUIMediaplayerURL[] = "aviator://mediaplayer/";
const char kChromeUIMobileSetupURL[] = "aviator://mobilesetup/";
const char kChromeUINfcDebugURL[] = "aviator://nfc-debug/";
const char kChromeUIOobeURL[] = "aviator://oobe/";
const char kChromeUIOSCreditsURL[] = "aviator://os-credits/";
const char kChromeUIProxySettingsURL[] = "aviator://proxy-settings/";
const char kChromeUIScreenlockIconURL[] = "aviator://screenlock-icon/";
const char kChromeUISetTimeURL[] = "aviator://set-time/";
const char kChromeUISimUnlockURL[] = "aviator://sim-unlock/";
const char kChromeUISlideshowURL[] = "aviator://slideshow/";
const char kChromeUISlowURL[] = "aviator://slow/";
const char kChromeUISystemInfoURL[] = "aviator://system/";
const char kChromeUITermsOemURL[] = "aviator://terms/oem";
const char kChromeUIUserImageURL[] = "aviator://userimage/";
#endif

#if defined(USE_AURA)
const char kChromeUIGestureConfigURL[] = "aviator://gesture/";
const char kChromeUIGestureConfigHost[] = "gesture";
const char kChromeUISalsaURL[] = "aviator://salsa/";
const char kChromeUISalsaHost[] = "salsa";
#endif

#if (defined(OS_LINUX) && defined(TOOLKIT_VIEWS)) || defined(USE_AURA)
const char kChromeUITabModalConfirmDialogURL[] =
    "aviator://tab-modal-confirm-dialog/";
#endif

#if defined(ENABLE_WEBRTC)
const char kChromeUIWebRtcLogsURL[] = "aviator://webrtc-logs/";
#endif

// Add Chrome UI hosts here, in alphabetical order.
// Add hosts to kChromePaths in browser_about_handler.cc to be listed by
// aviator://aviator-urls (about:about) and the built-in AutocompleteProvider.
const char kChromeUIAboutHost[] = "about";
const char kChromeUIBlankHost[] = "blank";
const char kChromeUIAppLauncherPageHost[] = "apps";
const char kChromeUIAppListStartPageHost[] = "app-list";
const char kChromeUIBookmarksHost[] = "bookmarks";
const char kChromeUICacheHost[] = "cache";
const char kChromeUICertificateViewerHost[] = "view-cert";
const char kChromeUICertificateViewerDialogHost[] = "view-cert-dialog";
const char kChromeUIChromeSigninHost[] = "aviator-signin"; //Added for champion
const char kChromeUIChromeURLsHost[] = "aviator-urls";     //Added for champion
const char kChromeUICloudPrintResourcesHost[] = "cloudprintresources";
const char kChromeUICloudPrintSetupHost[] = "cloudprintsetup";
const char kChromeUIConflictsHost[] = "conflicts";
const char kChromeUIConstrainedHTMLTestHost[] = "constrained-test";
const char kChromeUICrashesHost[] = "crashes";
const char kChromeUICrashHost[] = "crash";
const char kChromeUICreditsHost[] = "credits";
const char kChromeUIDefaultHost[] = "version";
const char kChromeUIDevicesHost[] = "devices";
const char kChromeUIDevToolsHost[] = "devtools";
const char kChromeUIDevToolsBundledPath[] = "bundled";
const char kChromeUIDevToolsRemotePath[] = "remote";
const char kChromeUIDNSHost[] = "dns";
const char kChromeUIDownloadsHost[] = "downloads";
const char kChromeUIDriveInternalsHost[] = "drive-internals";
const char kChromeUIEditSearchEngineDialogHost[] = "editsearchengine";
const char kChromeUIExtensionIconHost[] = "extension-icon";
const char kChromeUIExtensionInfoHost[] = "extension-info";
const char kChromeUIExtensionsFrameHost[] = "extensions-frame";
const char kChromeUIExtensionsHost[] = "extensions";
const char kChromeUIFaviconHost[] = "favicon";
const char kChromeUIFeedbackHost[] = "feedback";
const char kChromeUIFlagsHost[] = "flags";
const char kChromeUIFlashHost[] = "flash";
const char kChromeUIGCMInternalsHost[] = "gcm-internals";
const char kChromeUIHangHost[] = "hang";
const char kChromeUIHelpFrameHost[] = "help-frame";
const char kChromeUIHelpHost[] = "help";
const char kChromeUIHistoryHost[] = "history";
const char kChromeUIHistoryFrameHost[] = "history-frame";
const char kChromeUIIdentityInternalsHost[] = "identity-internals";
const char kChromeUIInspectHost[] = "inspect";
const char kChromeUIInstantHost[] = "instant";
const char kChromeUIInvalidationsHost[] = "invalidations";
const char kChromeUIIPCHost[] = "ipc";
const char kChromeUIKillHost[] = "kill";
const char kChromeUIManagedUserPassphrasePageHost[] = "managed-user-passphrase";
const char kChromeUIMemoryHost[] = "memory";
const char kChromeUIMemoryInternalsHost[] = "memory-internals";
const char kChromeUIMemoryRedirectHost[] = "memory-redirect";
const char kChromeUIMetroFlowHost[] = "make-metro";
const char kChromeUINaClHost[] = "nacl";
const char kChromeUINetExportHost[] = "net-export";
const char kChromeUINetInternalsHost[] = "net-internals";
const char kChromeUINewTabHost[] = "newtab";
const char kChromeUIOmniboxHost[] = "omnibox";
const char kChromeUIPasswordManagerInternalsHost[] =
    "password-manager-internals";
const char kChromeUIPerformanceMonitorHost[] = "performance";
const char kChromeUIPluginsHost[] = "plugins";
const char kChromeUIComponentsHost[] = "components";
const char kChromeUIPolicyHost[] = "policy";
const char kChromeUIProfileSigninConfirmationHost[] =
    "profile-signin-confirmation";
const char kChromeUIUserManagerHost[] = "user-manager";
const char kChromeUIPredictorsHost[] = "predictors";
const char kChromeUIPrintHost[] = "print";
const char kChromeUIProfilerHost[] = "profiler";
const char kChromeUIQuotaInternalsHost[] = "quota-internals";
const char kChromeUIQuitHost[] = "quit";
const char kChromeUIRestartHost[] = "restart";
const char kChromeUISettingsHost[] = "settings";
const char kChromeUISettingsFrameHost[] = "settings-frame";
const char kChromeUIShorthangHost[] = "shorthang";
const char kChromeUISignInInternalsHost[] = "signin-internals";
const char kChromeUISuggestionsHost[] = "suggestions";
const char kChromeUISuggestionsInternalsHost[] = "suggestions-internals";
const char kChromeUISSLClientCertificateSelectorHost[] = "select-cert";
const char kChromeUIStatsHost[] = "stats";
const char kChromeUISyncHost[] = "sync";
const char kChromeUISyncFileSystemInternalsHost[] = "syncfs-internals";
const char kChromeUISyncInternalsHost[] = "sync-internals";
const char kChromeUISyncResourcesHost[] = "syncresources";
const char kChromeUISystemInfoHost[] = "system";
const char kChromeUITaskManagerHost[] = "tasks";
const char kChromeUITermsHost[] = "terms";
const char kChromeUIThemeHost[] = "theme";
const char kChromeUIThumbnailHost[] = "thumb";
const char kChromeUIThumbnailHost2[] = "thumb2";
const char kChromeUIThumbnailListHost[] = "thumbnails";
const char kChromeUITouchIconHost[] = "touch-icon";
const char kChromeUITranslateInternalsHost[] = "translate-internals";
const char kChromeUIUberFrameHost[] = "uber-frame";
const char kChromeUIUberHost[] = "aviator"; // champion chrome to aviator
const char kChromeUIUserActionsHost[] = "user-actions";
const char kChromeUIVersionHost[] = "version";
const char kChromeUIWorkersHost[] = "workers";

const char kChromeUIScreenshotPath[] = "screenshots";
const char kChromeUIThemePath[] = "theme";

#if defined(OS_ANDROID)
const char kChromeUIWelcomeHost[] = "welcome";
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
const char kChromeUILinuxProxyConfigHost[] = "linux-proxy-config";
const char kChromeUISandboxHost[] = "sandbox";
#endif

#if defined(OS_CHROMEOS)
const char kChromeUIActivationMessageHost[] = "activationmessage";
const char kChromeUIAppLaunchHost[] = "app-launch";
const char kChromeUIBluetoothPairingHost[] = "bluetooth-pairing";
const char kChromeUICertificateManagerHost[] = "certificate-manager";
const char kChromeUIChargerReplacementHost[] = "charger-replacement";
const char kChromeUIChooseMobileNetworkHost[] = "choose-mobile-network";
const char kChromeUICryptohomeHost[] = "cryptohome";
const char kChromeUIDiscardsHost[] = "discards";
const char kChromeUIFirstRunHost[] = "first-run";
const char kChromeUIIdleLogoutDialogHost[] = "idle-logout";
const char kChromeUIImageBurnerHost[] = "imageburner";
const char kChromeUIKeyboardOverlayHost[] = "keyboardoverlay";
const char kChromeUILockScreenHost[] = "lock";
const char kChromeUILoginContainerHost[] = "login-container";
const char kChromeUILoginHost[] = "login";
const char kChromeUIMediaplayerHost[] = "mediaplayer";
const char kChromeUIMobileSetupHost[] = "mobilesetup";
const char kChromeUINfcDebugHost[] = "nfc-debug";
const char kChromeUINetworkHost[] = "network";
const char kChromeUIOobeHost[] = "oobe";
const char kChromeUIOSCreditsHost[] = "os-credits";
const char kChromeUIPowerHost[] = "power";
const char kChromeUIProvidedFileSystemsHost[] = "provided-file-systems";
const char kChromeUIProxySettingsHost[] = "proxy-settings";
const char kChromeUIRotateHost[] = "rotate";
const char kChromeUIScreenlockIconHost[] = "screenlock-icon";
const char kChromeUISetTimeHost[] = "set-time";
const char kChromeUISimUnlockHost[] = "sim-unlock";
const char kChromeUISlideshowHost[] = "slideshow";
const char kChromeUISlowHost[] = "slow";
const char kChromeUISlowTraceHost[] = "slow_trace";
const char kChromeUIUserImageHost[] = "userimage";

const char kChromeUIMenu[] = "menu";
const char kChromeUINetworkMenu[] = "network-menu";
const char kChromeUIWrenchMenu[] = "wrench-menu";

const char kEULAPathFormat[] = "/usr/share/chromeos-assets/eula/%s/eula.html";
const char kOemEulaURLPath[] = "oem";
const char kOnlineEulaURLPath[] =
    "https://www.google.com/intl/%s/chrome/eula_text.html";
#endif

#if (defined(OS_LINUX) && defined(TOOLKIT_VIEWS)) || defined(USE_AURA)
const char kChromeUITabModalConfirmDialogHost[] = "tab-modal-confirm-dialog";
#endif

#if defined(ENABLE_WEBRTC)
const char kChromeUIWebRtcLogsHost[] = "webrtc-logs";
#endif

// Option sub pages.
// Add sub page paths to kChromeSettingsSubPages in builtin_provider.cc to be
// listed by the built-in AutocompleteProvider.
const char kAutofillSubPage[] = "autofill";
const char kAlternateBrowserSubPage[]="alternateBrowser"; // champion : alternate browser
const char kClearBrowserDataSubPage[] = "clearBrowserData";
const char kContentSettingsExceptionsSubPage[] = "contentExceptions";
const char kContentSettingsSubPage[] = "content";
const char kCreateProfileSubPage[] = "createProfile";
const char kExtensionsSubPage[] = "extensions";
const char kHandlerSettingsSubPage[] = "handlers";
const char kImportDataSubPage[] = "importData";
const char kLanguageOptionsSubPage[] = "languages";
const char kSupervisedUserSettingsSubPage[] = "managedUser";
const char kManageProfileSubPage[] = "manageProfile";
const char kPasswordManagerSubPage[] = "passwords";
const char kResetProfileSettingsSubPage[] = "resetProfileSettings";
const char kSearchEnginesSubPage[] = "searchEngines";
const char kSearchSubPage[] = "search";
const char kSearchUsersSubPage[] = "search#Users";
const char kSyncSetupSubPage[] = "syncSetup";
#if defined(OS_CHROMEOS)
const char kInternetOptionsSubPage[] = "internet";
const char kBluetoothAddDeviceSubPage[] = "bluetooth";
const char kChangeProfilePictureSubPage[] = "changePicture";
#endif

// Extension sub pages.
const char kExtensionConfigureCommandsSubPage[] = "configureCommands";

const char kExtensionInvalidRequestURL[] = "chrome-extension://invalid/";
const char kExtensionResourceInvalidRequestURL[] =
    "chrome-extension-resource://invalid/";

const char kSyncGoogleDashboardURL[] =
    "https://www.google.com/settings/chrome/sync/";

const char kAutoPasswordGenerationLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ui_generate_password";
	
const char kPasswordManagerLearnMoreURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_password";
#else
    //"https://support.google.com/chrome/?p=settings_password";
#endif

const char kExtensionControlledSettingLearnMoreURL[] = AVIATOR_HELP_URL; //champion
   // "https://support.google.com/chrome/?p=ui_settings_api_extension";

//const char kSettingsApiLearnMoreURL[] = AVIATOR_HELP_URL; //champion
   // "https://support.google.com/chrome/?p=ui_settings_api_extension";

const char kChromeHelpViaKeyboardURL[] =
#if defined(OS_CHROMEOS)
#if defined(OFFICIAL_BUILD)
    "chrome-extension://honijodknafkokifofgiaalefdiedpko/main.html";
#else
    "https://support.google.com/chromeos/?p=help&ctx=keyboard";
#endif  // defined(OFFICIAL_BUILD
#else
    //"https://support.google.com/chrome/?p=help&ctx=keyboard";
    AVIATOR_HELP_URL;
#endif  // defined(OS_CHROMEOS)

const char kChromeHelpViaMenuURL[] =
#if defined(OS_CHROMEOS)
#if defined(OFFICIAL_BUILD)
    "chrome-extension://honijodknafkokifofgiaalefdiedpko/main.html";
#else
   // "https://support.google.com/chromeos/?p=help&ctx=menu";
#endif  // defined(OFFICIAL_BUILD
#else
    //"https://support.google.com/chrome/?p=help&ctx=menu";
	AVIATOR_HELP_URL;
#endif  // defined(OS_CHROMEOS)

const char kChromeHelpViaWebUIURL[] =
#if defined(OS_CHROMEOS)
#if defined(OFFICIAL_BUILD)
    "chrome-extension://honijodknafkokifofgiaalefdiedpko/main.html";
#else
   // "https://support.google.com/chromeos/?p=help&ctx=settings";
#endif  // defined(OFFICIAL_BUILD
#else
    //"https://support.google.com/chrome/?p=help&ctx=settings";
	AVIATOR_HELP_URL;
#endif  // defined(OS_CHROMEOS)

#if defined(OS_CHROMEOS)
const char kChromeAccessibilityHelpURL[] =
    "https://support.google.com/chromeos/?p=accessibility_menu";
const char kChromeAccessibilitySettingsURL[] =
    "/chromevox/background/options.html";
#endif  // defined(OS_CHROMEOS)

#if defined(ENABLE_ONE_CLICK_SIGNIN)
const char kChromeSyncLearnMoreURL[] = AVIATOR_HELP_URL; //champion
 
    //"http://support.google.com/chrome/bin/answer.py?answer=165139";

const char kChromeSyncMergeTroubleshootingURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/answer/1181420#merge";
#endif  // defined(ENABLE_ONE_CLICK_SIGNIN)

const char kChromeEnterpriseSignInLearnMoreURL[] = AVIATOR_HELP_URL; //champion
 // "http://support.google.com/chromeos/bin/answer.py?hl=en&answer=1331549";

const char kResetProfileSettingsLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    // "https://support.google.com/chrome/?p=ui_reset_settings";

const char kAutomaticSettingsResetLearnMoreURL[] = AVIATOR_HELP_URL; //champion
   // "https://support.google.com/chrome/?p=ui_automatic_settings_reset";

const char kSupervisedUserManagementURL[] = AVIATOR_HELP_URL; //champion
   //"https://www.chrome.com/manage";

const char kSupervisedUserManagementDisplayURL[] = AVIATOR_HELP_URL; //champion
   // "www.chrome.com/manage";

const char kSettingsSearchHelpURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_search_help";
#else
    //"https://support.google.com/chrome/?p=settings_search_help";
#endif

const char kOmniboxLearnMoreURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_omnibox";
#else
    //"https://support.google.com/chrome/?p=settings_omnibox";
#endif

const char kPageInfoHelpCenterURL[] =AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=ui_security_indicator";
#else
    //"https://support.google.com/chrome/?p=ui_security_indicator";
#endif

const char kCrashReasonURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=e_awsnap";
#else
    //"https://support.google.com/chrome/?p=e_awsnap";
#endif

const char kKillReasonURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=e_deadjim";
#else
    //"https://support.google.com/chrome/?p=e_deadjim";
#endif

const char kPrivacyLearnMoreURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_privacy";
#else
    //"https://support.google.com/chrome/?p=settings_privacy";
#endif

const char kDoNotTrackLearnMoreURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_do_not_track";
#else
    //"https://support.google.com/chrome/?p=settings_do_not_track";
#endif

#if defined(OS_CHROMEOS)
const char kAttestationForContentProtectionLearnMoreURL[] =
    "https://support.google.com/chromebook/?p=verified_access";

const char kEnhancedPlaybackNotificationLearnMoreURL[] =
    "https://support.google.com/chromebook/?p=enhanced_playback";
#endif

const char kChromiumProjectURL[] = "http://www.chromium.org/";

const char kLearnMoreReportingURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ui_usagestat";

const char kOutdatedPluginLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ib_outdated_plugin";

const char kBlockedPluginLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ib_blocked_plugin";

const char kHotwordLearnMoreURL[] = AVIATOR_HELP_URL; //champion
   // "https://support.google.com/chrome/?p=ui_hotword_search";

const char kLearnMoreRegisterProtocolHandlerURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ib_protocol_handler";

const char kSyncLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    // "https://support.google.com/chrome/?p=settings_sign_in";

const char kDownloadScanningLearnMoreURL[] = AVIATOR_HELP_URL; //champion
   // "https://support.google.com/chrome/?p=ib_download_blocked";

const char kDownloadInterruptedLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ui_download_errors";

const char kSyncEverythingLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=settings_sync_all";

const char kCloudPrintLearnMoreURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_cloud_print";
#else
    //"https://support.google.com/chrome/?p=settings_cloud_print";
#endif

const char kCloudPrintNoDestinationsLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    //"https://www.google.com/cloudprint/learn/";

const char kAppLauncherHelpURL[] = AVIATOR_HELP_URL; //champion
  //  "https://support.google.com/chrome_webstore/?p=cws_app_launcher";

const char kSyncEncryptionHelpURL[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    //"https://support.google.com/chromeos/?p=settings_encryption";
#else
    //"https://support.google.com/chrome/?p=settings_encryption";
#endif

const char kSyncErrorsHelpURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=settings_sync_error";

#if defined(OS_CHROMEOS)
const char kNaturalScrollHelpURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chromeos/?p=simple_scrolling";
#endif

#if defined(OS_CHROMEOS)
const char kLearnMoreEnterpriseURL[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chromeos/bin/answer.py?answer=2535613";
#endif

const char kRemoveNonCWSExtensionURL[] = AVIATOR_HELP_URL; //champion
    /*"https://support.google.com/chrome/answer/2811969?"
    "p=ui_remove_non_cws_extensions&rd=1";*/

const char kCorruptExtensionURL[] = AVIATOR_HELP_URL;
    //"https://support.google.com/chrome/?p=settings_corrupt_extension";

const char kNotificationsHelpURL[] = AVIATOR_HELP_URL; //champion
  //  "https://support.google.com/chrome/?p=ui_notifications";

const char kNotificationWelcomeLearnMoreURL[] = AVIATOR_HELP_URL; //champion
    // "https://support.google.com/chrome/?p=ib_google_now_welcome";

// Add hosts here to be included in aviator://aviator-urls (about:about).
// These hosts will also be suggested by BuiltinProvider.
const char* const kChromeHostURLs[] = {
  kChromeUICacheHost,
  kChromeUIChromeURLsHost,
  kChromeUIComponentsHost,
  kChromeUICrashesHost,
  kChromeUICreditsHost,
  kChromeUIDNSHost,
  kChromeUIFlagsHost,
  kChromeUIHistoryHost,
  kChromeUIInvalidationsHost,
  kChromeUIMemoryHost,
  kChromeUIMemoryInternalsHost,
  kChromeUINetInternalsHost,
  kChromeUINewTabHost,
  kChromeUIOmniboxHost,
  kChromeUIPasswordManagerInternalsHost,
  kChromeUIPredictorsHost,
  kChromeUIProfilerHost,
  kChromeUISignInInternalsHost,
  kChromeUIStatsHost,
  kChromeUISyncInternalsHost,
  kChromeUITermsHost,
  kChromeUITranslateInternalsHost,
  kChromeUIUserActionsHost,
  kChromeUIVersionHost,
  content::kChromeUIAccessibilityHost,
  content::kChromeUIAppCacheInternalsHost,
  content::kChromeUIBlobInternalsHost,
  content::kChromeUIGpuHost,
  content::kChromeUIHistogramHost,
  content::kChromeUIIndexedDBInternalsHost,
  content::kChromeUIMediaInternalsHost,
  content::kChromeUINetworkViewCacheHost,
  content::kChromeUIServiceWorkerInternalsHost,
  content::kChromeUITracingHost,
  content::kChromeUIWebRTCInternalsHost,
#if defined(OS_ANDROID)
  kChromeUIWelcomeHost,
#else
  kChromeUIAppLauncherPageHost,
  kChromeUIBookmarksHost,
  kChromeUIDownloadsHost,
  kChromeUIFlashHost,
  kChromeUIGCMInternalsHost,
  kChromeUIHelpHost,
  kChromeUIInspectHost,
  kChromeUIIPCHost,
  kChromeUIPluginsHost,
  kChromeUIQuotaInternalsHost,
  kChromeUISettingsHost,
  kChromeUISystemInfoHost,
  kChromeUIUberHost,
#endif
#if defined(OS_ANDROID) || defined(OS_IOS)
  kChromeUINetExportHost,
#endif
#if defined(OS_CHROMEOS)
  kChromeUICertificateManagerHost,
  kChromeUIChooseMobileNetworkHost,
  kChromeUICryptohomeHost,
  kChromeUIDiscardsHost,
  kChromeUIDriveInternalsHost,
  kChromeUIFirstRunHost,
  kChromeUIImageBurnerHost,
  kChromeUIKeyboardOverlayHost,
  kChromeUILoginHost,
  kChromeUINetworkHost,
  kChromeUIOobeHost,
  kChromeUIOSCreditsHost,
  kChromeUIPowerHost,
  kChromeUIProxySettingsHost,
  kChromeUITaskManagerHost,
#endif
#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  kChromeUILinuxProxyConfigHost,
  kChromeUISandboxHost,
#endif
#if defined(OS_WIN)
  kChromeUIConflictsHost,
#endif
#if !defined(DISABLE_NACL)
  kChromeUINaClHost,
#endif
#if defined(ENABLE_CONFIGURATION_POLICY)
  kChromeUIPolicyHost,
#endif
#if defined(ENABLE_EXTENSIONS)
  kChromeUIExtensionsHost,
#endif
#if defined(ENABLE_FULL_PRINTING)
  kChromeUIPrintHost,
#endif
#if defined(ENABLE_SERVICE_DISCOVERY)
  kChromeUIDevicesHost,
#endif
#if defined(ENABLE_WEBRTC)
  kChromeUIWebRtcLogsHost,
#endif
};
const size_t kNumberOfChromeHostURLs = arraysize(kChromeHostURLs);

const char* const kChromeDebugURLs[] = {
  content::kChromeUICrashURL,
  content::kChromeUIDumpURL,
  content::kChromeUIKillURL,
  content::kChromeUIHangURL,
  content::kChromeUIShorthangURL,
  content::kChromeUIGpuCleanURL,
  content::kChromeUIGpuCrashURL,
  content::kChromeUIGpuHangURL,
  content::kChromeUIPpapiFlashCrashURL,
  content::kChromeUIPpapiFlashHangURL,
  chrome::kChromeUIQuitURL,
  chrome::kChromeUIRestartURL
};
const int kNumberOfChromeDebugURLs =
    static_cast<int>(arraysize(kChromeDebugURLs));

const char kChromeNativeScheme[] = "aviator-native"; // champion chrome to aviator

const char kChromeSearchScheme[] = "chrome-search";
const char kChromeSearchLocalNtpHost[] = "local-ntp";
const char kChromeSearchLocalNtpUrl[] =
    "chrome-search://local-ntp/local-ntp.html";
const char kChromeSearchRemoteNtpHost[] = "remote-ntp";

const char kChromeSearchMostVisitedHost[] = "most-visited";
const char kChromeSearchMostVisitedUrl[] = "chrome-search://most-visited/";

const char kDomDistillerScheme[] = "chrome-distiller";

// Google SafeSearch query parameters.
const char kSafeSearchSafeParameter[] = "safe=active";
const char kSafeSearchSsuiParameter[] = "ssui=on";

const char kMediaAccessLearnMoreUrl[] = AVIATOR_HELP_URL; //champion
    //"https://support.google.com/chrome/?p=ib_access_cam_mic";

const char kLanguageSettingsLearnMoreUrl[] = AVIATOR_HELP_URL; //champion
#if defined(OS_CHROMEOS)
    // "https://support.google.com/chromebook/answer/1059490";
#else
   //  "https://support.google.com/chrome/topic/1678461";
#endif

#if defined(OS_MACOSX)
const char kMac32BitDeprecationURL[] = AVIATOR_HELP_URL; //champion
#if !defined(ARCH_CPU_64_BITS)
    // "https://support.google.com/chrome/?p=ui_mac_32bit_support";
#else
    "";
#endif
#endif

// TODO(tengs): Replace with real URL when ready.
const char kEasyUnlockLearnMoreUrl[] =
    "https://support.google.com/chromebook/?p=easy_unlock";
const char kEasyUnlockManagementUrl[] = "https://chrome.com";

}  // namespace chrome
