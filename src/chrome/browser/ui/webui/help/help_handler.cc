// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/ui/webui/help/help_handler.h"

#include <string>


#include "base/basictypes.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/command_line.h"
#include "base/strings/string16.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/common/chrome_version_info.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/url_constants.h"
#include "components/google/core/browser/google_util.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/user_agent.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "grit/google_chrome_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "v8/include/v8.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"

#include "chrome/browser/championconfig/varsconfig.h"     //Added for Champion version
#if defined(OS_WIN)
#include "chrome/browser/championconfig/update/curl.h"    // Added for Champion About page update
#include "chrome/browser/championconfig/chromeimporter/chrome_importer_utils.h" // champion : to get aviator update path (Amresh)
#include "chrome/browser/championconfig/RestartUpdate/RestartforUpdate.h"       // champion : for delayed restart (Amresh)
#include "base/path_service.h"                                                  // champion : for server list file (Amresh)
#include "chrome/common/chrome_paths.h"                                         // champion : for server list file (Amresh)
#include "chrome/browser/championconfig/serverlistparser/server_list_parser.h"  // champion : for server list file (Amresh)
#include <Windows.h>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <time.h>
#include <tlhelp32.h>
#include <iostream>
#endif
#if defined(OS_MACOSX)
#include "chrome/browser/mac/obsolete_system.h"
#endif

#if defined(OS_CHROMEOS)
#include "base/files/file_util_proxy.h"
#include "base/i18n/time_formatting.h"
#include "base/prefs/pref_service.h"
#include "base/sys_info.h"
#include "chrome/browser/chromeos/login/users/user_manager.h"
#include "chrome/browser/chromeos/policy/browser_policy_connector_chromeos.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/help/help_utils_chromeos.h"
#include "chromeos/chromeos_switches.h"
#include "chromeos/dbus/dbus_thread_manager.h"
#include "chromeos/dbus/power_manager_client.h"
#include "content/public/browser/browser_thread.h"
#endif

using base::ListValue;
using content::BrowserThread;

// About page update Aviator starts here - champion


////////////////////////////////////

#if defined (OS_WIN) 
#if defined(ARCH_CPU_64_BITS)
  #pragma comment(lib, "x64/libcurl.lib")
#else
  #pragma comment(lib, "x86/libcurl.lib")
#endif
#endif
#if defined (OS_MACOSX)
#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include "base/file_util.h"

#include <unistd.h>
#include <sys/types.h>
#include <strings.h>
#include <pwd.h>
#include <grp.h>

bool bSetPromoteHide;
bool HelpHandler::isUpdateInProgress=false;
bool isAdminUser;
#include "base/threading/worker_pool.h"
#endif 

using namespace std; // champion
#if defined (OS_WIN)
// champion : update mechanism
static bool isUpdateRequired;
static std::string updatedVersionOnServer = CHAMPION_BUILD_NO;

#define XP "5.1"
#define AVIATORUPDATELOCATION aviatorUpdateLocation
#define AVIATORUPDATEPATH "path"

char* aviatorUpdateLocation;
bool isAdmin = true; 
// About page update Aviator ends here - champion
#endif
namespace {

// Returns the browser version as a string.
/*base::string16 BuildBrowserVersionString() {
  chrome::VersionInfo version_info;
  DCHECK(version_info.is_valid());

  std::string browser_version = version_info.Version();
  std::string version_modifier =
      chrome::VersionInfo::GetVersionStringModifier();
  if (!version_modifier.empty())
    browser_version += " " + version_modifier;

#if !defined(GOOGLE_CHROME_BUILD)
  browser_version += " (";
  browser_version += version_info.LastChange();
  browser_version += ")";
#endif //make change for aviator version () remove champion

#if defined(ARCH_CPU_64_BITS)
  browser_version += " (64-bit)";
#endif

  return base::UTF8ToUTF16(browser_version);
}*/

#if defined(OS_CHROMEOS)

// Returns message that informs user that for update it's better to
// connect to a network of one of the allowed types.
base::string16 GetAllowedConnectionTypesMessage() {
  if (help_utils_chromeos::IsUpdateOverCellularAllowed()) {
    return l10n_util::GetStringUTF16(IDS_UPGRADE_NETWORK_LIST_CELLULAR_ALLOWED);
  } else {
    return l10n_util::GetStringUTF16(
        IDS_UPGRADE_NETWORK_LIST_CELLULAR_DISALLOWED);
  }
}

// Returns true if the device is enterprise managed, false otherwise.
bool IsEnterpriseManaged() {
  policy::BrowserPolicyConnectorChromeOS* connector =
      g_browser_process->platform_part()->browser_policy_connector_chromeos();
  return connector->IsEnterpriseManaged();
}

// Returns true if current user can change channel, false otherwise.
bool CanChangeChannel() {
  bool value = false;
  chromeos::CrosSettings::Get()->GetBoolean(chromeos::kReleaseChannelDelegated,
                                            &value);

  // On a managed machine we delegate this setting to the users of the same
  // domain only if the policy value is "domain".
  if (IsEnterpriseManaged()) {
    if (!value)
      return false;
    // Get the currently logged in user and strip the domain part only.
    std::string domain = "";
    std::string user = chromeos::UserManager::Get()->GetLoggedInUser()->email();
    size_t at_pos = user.find('@');
    if (at_pos != std::string::npos && at_pos + 1 < user.length())
      domain = user.substr(user.find('@') + 1);
    policy::BrowserPolicyConnectorChromeOS* connector =
        g_browser_process->platform_part()->browser_policy_connector_chromeos();
    return domain == connector->GetEnterpriseDomain();
  } else if (chromeos::UserManager::Get()->IsCurrentUserOwner()) {
    // On non managed machines we have local owner who is the only one to change
    // anything. Ensure that ReleaseChannelDelegated is false.
    return !value;
  }
  return false;
}

#endif  // defined(OS_CHROMEOS)

}  // namespace
#if defined (OS_WIN)
// About page update Aviator starts here - champion

size_t write_data(void *ptr, size_t size, size_t nmemb, char *stream);

DWORD FindProcessId(const std::wstring& processName) {
  PROCESSENTRY32 processInfo;
  processInfo.dwSize = sizeof(processInfo);

  HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if (processesSnapshot == INVALID_HANDLE_VALUE)
    return 0;

  Process32First(processesSnapshot, &processInfo);
  if (!processName.compare(processInfo.szExeFile)) {
    CloseHandle(processesSnapshot);
    return processInfo.th32ProcessID;
  }

  while (Process32Next(processesSnapshot, &processInfo)) {
    if (!processName.compare(processInfo.szExeFile)) {
      CloseHandle(processesSnapshot);
      return processInfo.th32ProcessID;
    }
  }

  CloseHandle(processesSnapshot);
  return 0;
}

// champion : worker thread to handle update related task (Amresh)
class AviatorUpdater : public base::RefCountedThreadSafe<AviatorUpdater> {
public:
  AviatorUpdater(content::WebUI* webUI);
  void InvokeUpdate() {
    BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE, base::Bind(
      &AviatorUpdater::UpdateAviator, this));
  }

  static void updateRequired(string version);
  static float listFileVersion;
  static std::string strHash;
  static std::string strListFile;
  static CURLcode curlResponse;

private:
  friend class base::RefCountedThreadSafe<AviatorUpdater>;
  friend class HelpHandler;

  virtual ~AviatorUpdater();

  bool RunApplicationSilently(const std::string& strAppName, PROCESS_INFORMATION& pInfo) const;
  std::string GetUserDataPath() const;
  void RunAgainstServerList(CURL* curl, bool& didRun);
  void UpdateAviator();
  bool CanUpdateAgainstURL(CURL* curl, char* url);
  void getAviatorRegistryPath() const;
  string getUpdatePathFromRegistry();
  bool IsExtractionCompleted() const;
  bool CanRequestRestart() const;

  content::WebUI* webUI_;
  Browser* browser_;
  string AviatorMSIFile;
  string ExctractPath;
  AutoUpdater::ServerListParser* slp_;
  char curlError[MAX_PATH];
#if USE_CERT
  string certificateFile;
#endif

  DISALLOW_COPY_AND_ASSIGN(AviatorUpdater);
};

float AviatorUpdater::listFileVersion = 0.0;
std::string AviatorUpdater::strListFile = "";
std::string AviatorUpdater::strHash = "";
CURLcode AviatorUpdater::curlResponse = CURLE_COULDNT_CONNECT;

AviatorUpdater::AviatorUpdater(content::WebUI* webUI)
  : webUI_(webUI), browser_(0), slp_(0) {
#if USE_CERT
  certificateFile = "";
#endif
  memset(curlError, '\0', sizeof(char) * MAX_PATH);
  browser_ = chrome::FindBrowserWithWebContents(webUI_->GetWebContents());
  try {
    slp_ = new AutoUpdater::ServerListParser(GetUserDataPath() + "\\ServerList.lst");
  } catch (exception e) {
    ::MessageBoxA(GetActiveWindow(), e.what(), "Server List Error!", MB_OK);
  }
  string AviatorUpdatePath = GetAviatorInstallPathFromRegistry32bitW();
  if (!AviatorUpdatePath.empty() || AviatorUpdatePath != "ERROR") {
    std::string checkstring = AviatorUpdatePath;
    unsigned found = checkstring.find("Aviator");
    AviatorMSIFile.append(checkstring.substr(0, found));
    ExctractPath = AviatorMSIFile + "Update";
    AviatorMSIFile.append("Update\\Aviator.msi");
  }
}

AviatorUpdater::~AviatorUpdater() {
  if (slp_)
    delete slp_;
}

std::string AviatorUpdater::GetUserDataPath() const {
  base::FilePath user_data_path;
  base::FilePath::StringType strPath;
  if (!PathService::Get(chrome::DIR_USER_DATA, &user_data_path))
    strPath = base::FilePath().value();

  strPath = user_data_path.value();
  return base::WideToUTF8(strPath.c_str());
}

void AviatorUpdater::updateRequired(string version) {
  char* upgradeVersion = new char[version.length() + 1];
  strcpy(upgradeVersion, version.c_str());
  if (version.find("&lt;") != string::npos) {
    char* vers = strtok(upgradeVersion, "&lt;");
    vers = strtok(NULL, "&lt;");
    updatedVersionOnServer = vers;
    if (atof(CHAMPION_BUILD_NO) < atof(vers)) {
      isUpdateRequired = true;
    } else {
      isUpdateRequired = false;
    }
  } else if (version.find("&gt;") != string::npos) {
    char* vers = strtok(upgradeVersion, "&gt;");
    vers = strtok(NULL, "&gt;");
    updatedVersionOnServer = vers;
    if (atof(CHAMPION_BUILD_NO) > atof(vers)) {
      isUpdateRequired = true;
    } else {
      isUpdateRequired = false;
    }
  }
  
  AutoUpdater::Trim(updatedVersionOnServer);
}

bool AviatorUpdater::IsExtractionCompleted() const {
  if (AutoUpdater::DirExist(ExctractPath) &&
      AutoUpdater::DirExist(ExctractPath + "\\program files\\Whitehat\\Aviator\\Application") &&
      AutoUpdater::DirExist(ExctractPath + "\\program files\\Whitehat\\Aviator\\Update") &&
      AutoUpdater::FileExist(AviatorMSIFile))
    return true;

  return false;
}

bool AviatorUpdater::CanRequestRestart() const {
  if (IsExtractionCompleted() && FindProcessId(L"AviatorAutoUpdate.exe"))
    return true;

  if (!FindProcessId(L"AviatorAutoUpdate.exe"))
    return false;
  
  long timeToWait(3600), timeElapsed(0);
  while (timeElapsed != timeToWait) {
    try {
      if (browser_->IsAttemptingToCloseBrowser())
        return false;
      webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus", base::StringValue("updating"),
        base::StringValue(""));
    } catch (...) { return false; }

    Sleep(500);
    timeElapsed++;
    if (!FindProcessId(L"AviatorAutoUpdate.exe"))
      return false;

    if (IsExtractionCompleted())
      return true;
  }

  return false;
}

// return value based on read xml stream data and provided key
std::string GetValue(const std::string& version, const std::string& strKey) {
  std::string strValue = "";
  std::string strStartNode = "<" + strKey + ">";
  std::string strEndNode = "</" + strKey + ">";
  size_t startPos = version.find(strStartNode);
  size_t endPos = version.find(strEndNode);
  if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos) {
    startPos += strStartNode.size();
    strValue = version.substr(startPos, endPos - startPos);
    AutoUpdater::Trim(strValue);
  }

  return strValue;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, char *stream) {
  stream = (char*)ptr;
  string version;
  string localUpdate;
  version.append(stream);

  try {
    AviatorUpdater::listFileVersion = (float)atof(GetValue(version, "Listversion").c_str());
    AviatorUpdater::strListFile = GetValue(version, "Listfile");
    AviatorUpdater::strHash = GetValue(version, "Listhash");
    localUpdate = GetValue(version, "Predicate");
  } catch (std::exception) {
    localUpdate = GetValue(version, "Predicate");
  }

  AviatorUpdater::updateRequired(localUpdate);
  return (localUpdate.size()) ? version.size() : 0;
}

bool AviatorUpdater::CanUpdateAgainstURL(CURL* curl, char* url) {
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlError);

#if USE_CERT
  // without certificate curl will not allow connection (secure connection)
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
  curl_easy_setopt(curl, CURLOPT_CAINFO, certificateFile.c_str());
#else
  // without certificate, curl will allow connection (unsecure connection)
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
#endif

  curlResponse = curl_easy_perform(curl);
  return (curlResponse == CURLE_OK);
}

bool AviatorUpdater::RunApplicationSilently(const std::string& strAppName, PROCESS_INFORMATION& ProcessInfo) const {
  STARTUPINFO StartupInfo;
  std::wstring strApplication(strAppName.begin(), strAppName.end());

  memset(&StartupInfo, 0, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(STARTUPINFO);
  StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
  StartupInfo.wShowWindow = SW_HIDE;

  if (!CreateProcess(NULL, (LPWSTR)strApplication.c_str(), NULL, NULL, FALSE, 
      CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo))
    return false;

  return true;
}

void AviatorUpdater::RunAgainstServerList(CURL* curl, bool& didRun) {
  const AutoUpdater::ServerList& sl = slp_->GetServerList();
  if (sl.size()) {
    for (size_t i = 0; i < sl.size(); i++) {
      char* url = const_cast<char *>(sl[i].c_str());
      if (!CanUpdateAgainstURL(curl, url))
        continue;
      else {
        didRun = true;
        break;
      }
    }
  } else
    didRun = false;
}

void AviatorUpdater::UpdateAviator() {
  // champion : decide whether per user or system level installation 
  // and read updater location from registry (Amresh)
  getAviatorRegistryPath();
  std::string updaterLocation = getUpdatePathFromRegistry();
  if (updaterLocation == "ERROR") {
    // champion : installation corrupt or missing, let user know update is not possible (Amresh)
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("failed"),
      base::StringValue("Unable to locate Aviator installation!"));
    return;
  }

  // champion : let user know update check in progress (Amresh)
  webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
    base::StringValue("checking"),
    base::StringValue("Checking for updates..."));
  // champion : request restart if updates already available or wait till in progress (Amresh)
  if (CanRequestRestart()) {
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("nearly_updated"),
      base::StringValue(""));
    return;
  }

  // champion : push certificate info for ssl connection if enabled (Amresh)
#if USE_CERT
  unsigned pos = getUpdatePathFromRegistry().find("Update");
  certificateFile = getUpdatePathFromRegistry().substr(0, pos);
  certificateFile.append("Application\\");
  certificateFile.append(CERTIFICATE);
  slp_->SetCertificateFile(certificateFile);
#endif

  CURL* curl = curl_easy_init();
  char* url = MANIFESTFILE;
  bool serverInListResponded = false;
  // champion : not sure why it would happen but still, let user know update failed (Amresh)
  if (!curl) {
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("failed"),
      base::StringValue("Unknown error."));
    return;
  }

  // champion : attempt to access urls from server list file (Amresh)
  RunAgainstServerList(curl, serverInListResponded);
  if (serverInListResponded && !strListFile.size()) {
    CanUpdateAgainstURL(curl, url);
    curlResponse = CURLE_OK;
  }

  // champion : if failed on server list urls, attempt on main server (Amresh)
  if (!serverInListResponded)
    serverInListResponded = CanUpdateAgainstURL(curl, url);

  // champion : a rare chance urls of server list was hit but server list file info missing
  // if happens, give last chance to hit main server and attempt downloading server list file (Amresh)
  if (curlResponse == CURLE_OK) {
    if (listFileVersion > slp_->GetVersion() || (strHash.size() && (slp_->GetListHash() != strHash)))
      slp_->CopyAndReset(curl, strListFile, strHash);
  } else {
    // champion : something went wrong (i.e. no connection, certificate issue, etc..), show error with info (Amresh)
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("failed"),
      base::StringValue(curlError));
    curl_easy_cleanup(curl);
    return;
  }

  curlResponse = CURLE_COULDNT_CONNECT;
  serverInListResponded = false;
  curl_easy_cleanup(curl);
  // champion : check if update available on server (Amresh)
  if (!isUpdateRequired) {
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("updated"),
      base::StringValue(" "));
    return;
  }

  OSVERSIONINFO osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  long long minor = osvi.dwMinorVersion;
  long long major = osvi.dwMajorVersion;
  std::string version = to_string(major) + "." + to_string(minor);
  bool isXP(false);
  std::string strMsg = "An updated version " + updatedVersionOnServer + " found on server.";
  PROCESS_INFORMATION pi;
  string updaterCommand;

  // champion : we need to prompt for UAC on systems other than XP and hence
  // prepare launch command based XP or non XP OS and lauch updater (Amresh)
  memset(&pi, 0, sizeof(PROCESS_INFORMATION));
  webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
    base::StringValue("failed"),
    base::StringValue(strMsg.c_str()));
  if (atof(XP) == atof(version.c_str())) {
    updaterCommand = updaterLocation;
    isXP = true;
  } else {
    std::string nonXPCommand = "powershell.exe -windowstyle hidden -c start ";
    if (isAdmin)
      nonXPCommand += " -Verb runAs ";

    nonXPCommand += ("'" + updaterLocation + " ' -windowstyle hidden ");
    updaterCommand = nonXPCommand;
  }
  
  // champion : updater is expected to run but if not, fire error and exit (Amresh)
  if (!RunApplicationSilently(updaterCommand, pi)) {
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
        base::StringValue("failed"),
        base::StringValue("Aviator Silent Updater failed to launch."));
    return;
  }

  // champion : we need to wait for user to decide on prompted UAC and hence wait
  // till user posts his choice (non XP OS) (Amresh)
  if (!isXP) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
  }

  // champion : by now update is already in progress
  // request relaunch when update is ready (Amresh)
  if (CanRequestRestart()) {
    webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
      base::StringValue("nearly_updated"),
      base::StringValue(""));
  } else {
    try {
      if (browser_->IsAttemptingToCloseBrowser())
        return;
      // champion : something went wrong with updater, display error and exit (Amresh)
      webUI_->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
        base::StringValue("failed"),
        base::StringValue("Aviator Silent Updater failed, check updater log for details."));
    } catch (...) { }
  }

  // champion : we don't need to wait for updater command to complete as
  // for XP, updater command is updater itself and hence waiting will cause
  // a stuck when hit relaunch button (Amresh)
  if (isXP) {
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
  }
}


void AviatorUpdater::getAviatorRegistryPath() const {
  aviatorUpdateLocation = "SOFTWARE\\Wow6432Node\\WhiteHat\\Update";
  char  szVersion[255];
  DWORD dwDataSize = 255;
  memset(szVersion, 0, 255);
  HKEY hkeyDXVer;
  long lResult2 = RegOpenKeyExA(HKEY_LOCAL_MACHINE, AVIATORUPDATELOCATION, 0, KEY_READ, &hkeyDXVer);
  if (ERROR_SUCCESS != lResult2){
    aviatorUpdateLocation = "SOFTWARE\\WhiteHat\\Update";
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, AVIATORUPDATELOCATION, 0, KEY_READ, &hkeyDXVer) != ERROR_SUCCESS){
      isAdmin = false;
    }
  }
}

/* To get the update path from regedit so that app will irrespective of windows version */
string AviatorUpdater::getUpdatePathFromRegistry() {
  DWORD dwType;
  char  szVersion[255];
  DWORD dwDataSize = 255;
  memset(szVersion, 0, 255);
  HKEY hkeyDXVer;
  if (RegOpenKeyExA(HKEY_CURRENT_USER, AVIATORUPDATELOCATION, 0, KEY_READ, &hkeyDXVer) != ERROR_SUCCESS)
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, AVIATORUPDATELOCATION, 0, KEY_READ, &hkeyDXVer) != ERROR_SUCCESS)
      return "ERROR";
  if (RegQueryValueExA(hkeyDXVer, AVIATORUPDATEPATH, NULL, &dwType, (BYTE*)szVersion, &dwDataSize) != ERROR_SUCCESS)
    return "ERROR";

  return szVersion;
}
// About page update Aviator functions definitions ends here - champion
#endif
HelpHandler::HelpHandler()
    : version_updater_(VersionUpdater::Create()),
      weak_factory_(this) {
}

HelpHandler::~HelpHandler() {
}

void HelpHandler::GetLocalizedValues(content::WebUIDataSource* source) {
  struct L10nResources {
    const char* name;
    int ids;
  };

  static L10nResources resources[] = {
    { "aboutTitle", IDS_ABOUT_TITLE },
#if defined(OS_CHROMEOS)
    { "aboutProductTitle", IDS_PRODUCT_OS_NAME },
#else
    { "aboutProductTitle", IDS_PRODUCT_NAME },
#endif
    { "aboutProductDescription", IDS_ABOUT_PRODUCT_DESCRIPTION },
    { "relaunch", IDS_RELAUNCH_BUTTON },
#if defined(OS_CHROMEOS)
    { "relaunchAndPowerwash", IDS_RELAUNCH_AND_POWERWASH_BUTTON },
#endif
    { "productName", IDS_PRODUCT_NAME },
    { "updateCheckStarted", IDS_UPGRADE_CHECK_STARTED },
    { "upToDate", IDS_UPGRADE_UP_TO_DATE },
    { "updating", IDS_UPGRADE_UPDATING },
#if defined(OS_CHROMEOS)
    { "updatingChannelSwitch", IDS_UPGRADE_UPDATING_CHANNEL_SWITCH },
#endif
    { "updateAlmostDone", IDS_UPGRADE_SUCCESSFUL_RELAUNCH },
#if defined(OS_CHROMEOS)
    { "successfulChannelSwitch", IDS_UPGRADE_SUCCESSFUL_CHANNEL_SWITCH },
#endif
    { "getHelpWithChrome", IDS_GET_HELP_USING_CHROME },
    { "reportAnIssue", IDS_REPORT_AN_ISSUE },
#if defined(OS_CHROMEOS)
    { "platform", IDS_PLATFORM_LABEL },
    { "firmware", IDS_ABOUT_PAGE_FIRMWARE },
    { "showMoreInfo", IDS_SHOW_MORE_INFO },
    { "hideMoreInfo", IDS_HIDE_MORE_INFO },
    { "channel", IDS_ABOUT_PAGE_CHANNEL },
    { "stable", IDS_ABOUT_PAGE_CHANNEL_STABLE },
    { "beta", IDS_ABOUT_PAGE_CHANNEL_BETA },
    { "dev", IDS_ABOUT_PAGE_CHANNEL_DEVELOPMENT },
    { "channel-changed", IDS_ABOUT_PAGE_CHANNEL_CHANGED },
    { "currentChannelStable", IDS_ABOUT_PAGE_CURRENT_CHANNEL_STABLE },
    { "currentChannelBeta", IDS_ABOUT_PAGE_CURRENT_CHANNEL_BETA },
    { "currentChannelDev", IDS_ABOUT_PAGE_CURRENT_CHANNEL_DEV },
    { "currentChannel", IDS_ABOUT_PAGE_CURRENT_CHANNEL },
    { "channelChangeButton", IDS_ABOUT_PAGE_CHANNEL_CHANGE_BUTTON },
    { "channelChangeDisallowedMessage",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_DISALLOWED_MESSAGE },
    { "channelChangePageTitle", IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_TITLE },
    { "channelChangePagePowerwashTitle",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_POWERWASH_TITLE },
    { "channelChangePagePowerwashMessage",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_POWERWASH_MESSAGE },
    { "channelChangePageDelayedChangeTitle",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_DELAYED_CHANGE_TITLE },
    { "channelChangePageUnstableTitle",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_UNSTABLE_TITLE },
    { "channelChangePagePowerwashButton",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_POWERWASH_BUTTON },
    { "channelChangePageChangeButton",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_CHANGE_BUTTON },
    { "channelChangePageCancelButton",
      IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_CANCEL_BUTTON },
    { "webkit", IDS_WEBKIT },
    { "userAgent", IDS_ABOUT_VERSION_USER_AGENT },
    { "commandLine", IDS_ABOUT_VERSION_COMMAND_LINE },
    { "buildDate", IDS_ABOUT_VERSION_BUILD_DATE },
#endif
#if defined(OS_MACOSX)
    { "promote", IDS_ABOUT_CHROME_PROMOTE_UPDATER },
    { "learnMore", IDS_LEARN_MORE },
#endif
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(resources); ++i) {
    source->AddString(resources[i].name,
                      l10n_util::GetStringUTF16(resources[i].ids));
  }

#if defined(OS_MACOSX)
  source->AddString("updateObsoleteSystem",
                    ObsoleteSystemMac::LocalizedObsoleteSystemString());
  source->AddString("updateObsoleteSystemURL",
                    chrome::kMac32BitDeprecationURL);
#endif

  /* champion - about page version */

  std ::string championversion(CHAMPION_VERSION_NO);
  std::string championBuilNo(CHAMPION_BUILD_NO); //Champion Change Build number
  championversion+=" ("+championBuilNo+")";

// champion : add 64 bit info for x64 build (Amresh)  
#if defined(ARCH_CPU_64_BITS)
  championversion += " (64-bit)";
#endif
  
  source->AddString(   // original chrome code commeted to include champion build number
      "browserVersion",
      l10n_util::GetStringFUTF16(IDS_ABOUT_PRODUCT_VERSION,
                                base::UTF8ToUTF16(championversion)));
  /*source->AddString(  // original chrome code commented to include champion build number
      "browserVersion",
      l10n_util::GetStringFUTF16(IDS_ABOUT_PRODUCT_VERSION,
                                 BuildBrowserVersionString()));*/
  /* champion - about page version */
  
  base::Time::Exploded exploded_time;
  base::Time::Now().LocalExplode(&exploded_time);
  source->AddString(
      "productCopyright",
       l10n_util::GetStringFUTF16(IDS_ABOUT_VERSION_COPYRIGHT,
                                  base::IntToString16(exploded_time.year)));

  base::string16 license = l10n_util::GetStringFUTF16(
      IDS_ABOUT_VERSION_LICENSE,
      base::ASCIIToUTF16(chrome::kChromiumProjectURL),
      base::ASCIIToUTF16(chrome::kChromeUICreditsURL));
  source->AddString("productLicense", license);

#if defined(OS_CHROMEOS)
  base::string16 os_license = l10n_util::GetStringFUTF16(
      IDS_ABOUT_CROS_VERSION_LICENSE,
      base::ASCIIToUTF16(chrome::kChromeUIOSCreditsURL));
  source->AddString("productOsLicense", os_license);

  base::string16 product_name = l10n_util::GetStringUTF16(IDS_PRODUCT_OS_NAME);
  source->AddString(
      "channelChangePageDelayedChangeMessage",
      l10n_util::GetStringFUTF16(
          IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_DELAYED_CHANGE_MESSAGE,
          product_name));
  source->AddString(
      "channelChangePageUnstableMessage",
      l10n_util::GetStringFUTF16(
          IDS_ABOUT_PAGE_CHANNEL_CHANGE_PAGE_UNSTABLE_MESSAGE,
          product_name));

  if (CommandLine::ForCurrentProcess()->
      HasSwitch(chromeos::switches::kDisableNewChannelSwitcherUI)) {
    source->AddBoolean("disableNewChannelSwitcherUI", true);
  }
#endif

  base::string16 tos = l10n_util::GetStringFUTF16(
      IDS_ABOUT_TERMS_OF_SERVICE, base::UTF8ToUTF16(chrome::kChromeUITermsURL));
  source->AddString("productTOS", tos);

  source->AddString("webkitVersion", content::GetWebKitVersion());

  source->AddString("jsEngine", "V8");
  source->AddString("jsEngineVersion", v8::V8::GetVersion());

  source->AddString("userAgentInfo", GetUserAgent());

  CommandLine::StringType command_line =
      CommandLine::ForCurrentProcess()->GetCommandLineString();
  source->AddString("commandLineInfo", command_line);
}

void HelpHandler::RegisterMessages() {
  registrar_.Add(this, chrome::NOTIFICATION_UPGRADE_RECOMMENDED,
                 content::NotificationService::AllSources());

  web_ui()->RegisterMessageCallback("onPageLoaded",
      base::Bind(&HelpHandler::OnPageLoaded, base::Unretained(this)));
  web_ui()->RegisterMessageCallback("relaunchNow",
      base::Bind(&HelpHandler::RelaunchNow, base::Unretained(this)));
  web_ui()->RegisterMessageCallback("openFeedbackDialog",
      base::Bind(&HelpHandler::OpenFeedbackDialog, base::Unretained(this)));
  web_ui()->RegisterMessageCallback("openHelpPage",
      base::Bind(&HelpHandler::OpenHelpPage, base::Unretained(this)));
#if defined(OS_CHROMEOS)
  web_ui()->RegisterMessageCallback("setChannel",
      base::Bind(&HelpHandler::SetChannel, base::Unretained(this)));
  web_ui()->RegisterMessageCallback("relaunchAndPowerwash",
      base::Bind(&HelpHandler::RelaunchAndPowerwash, base::Unretained(this)));
#endif
#if defined(OS_MACOSX)
  web_ui()->RegisterMessageCallback("promoteUpdater",
      base::Bind(&HelpHandler::PromoteUpdater, base::Unretained(this)));
#endif
}

void HelpHandler::Observe(int type, const content::NotificationSource& source,
                          const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_UPGRADE_RECOMMENDED: {
      // A version update is installed and ready to go. Refresh the UI so the
      // correct state will be shown.
      // champion : we are not using google updater (Amresh)
      /*version_updater_->CheckForUpdate(
          base::Bind(&HelpHandler::SetUpdateStatus, base::Unretained(this))
#if defined(OS_MACOSX)
          , base::Bind(&HelpHandler::SetPromotionState, base::Unretained(this))
#endif
          );*/
      break;
    }
    default:
      NOTREACHED();
  }
}

#if defined(OS_MACOSX)
// for about page update - champion
void HelpHandler::UpdateAviator() {
  version_updater_->CheckForUpdate(
    base::Bind(&HelpHandler::SetUpdateStatus, base::Unretained(this))
#if defined(OS_MACOSX)
    , base::Bind(&HelpHandler::SetPromotionState, base::Unretained(this))
#endif
    );
}

void HelpHandler::setUpdateInProgress(bool bValue) {
  HelpHandler::isUpdateInProgress = bValue;
}
#endif

void HelpHandler::OnPageLoaded(const base::ListValue* args) {
#if defined(OS_CHROMEOS)
  // Version information is loaded from a callback
  loader_.GetVersion(
      chromeos::VersionLoader::VERSION_FULL,
      base::Bind(&HelpHandler::OnOSVersion, base::Unretained(this)),
      &tracker_);
  loader_.GetFirmware(
      base::Bind(&HelpHandler::OnOSFirmware, base::Unretained(this)),
      &tracker_);

  web_ui()->CallJavascriptFunction(
      "help.HelpPage.updateEnableReleaseChannel",
      base::FundamentalValue(CanChangeChannel()));

  base::Time build_time = base::SysInfo::GetLsbReleaseTime();
  base::string16 build_date = base::TimeFormatFriendlyDate(build_time);
  web_ui()->CallJavascriptFunction("help.HelpPage.setBuildDate",
                                   base::StringValue(build_date));
#endif  // defined(OS_CHROMEOS)
  // champion : create threader weak ptr and invoke updater to do job (Amresh)
   #if defined(OS_WIN)
  if (!update_launcher_)
    update_launcher_ = new AviatorUpdater(web_ui());
  update_launcher_->InvokeUpdate();
  #endif
  // champion : we are not using google updater (Amresh)
  /*version_updater_->CheckForUpdate(
      base::Bind(&HelpHandler::SetUpdateStatus, base::Unretained(this))
#if defined(OS_MACOSX)
      , base::Bind(&HelpHandler::SetPromotionState, base::Unretained(this))
#endif
      );*/
     // champion - added for about page update 
#if defined(OS_MACOSX)
  SetPromotionState(VersionUpdater::PROMOTE_ENABLED);
  base::string16 message;
  SetUpdateStatus(VersionUpdater::CHECKING, 0, message);
  UpdateAviator();
  setUpdateInProgress(true);
#endif	  
  // champion - added for about page update  end
#if defined(OS_MACOSX)
  web_ui()->CallJavascriptFunction(
      "help.HelpPage.setObsoleteSystem",
      base::FundamentalValue(ObsoleteSystemMac::Is32BitObsoleteNowOrSoon() &&
                             ObsoleteSystemMac::Has32BitOnlyCPU()));
  web_ui()->CallJavascriptFunction(
      "help.HelpPage.setObsoleteSystemEndOfTheLine",
      base::FundamentalValue(ObsoleteSystemMac::Is32BitObsoleteNowOrSoon() &&
                             ObsoleteSystemMac::Is32BitEndOfTheLine()));
#endif

#if defined(OS_CHROMEOS)
  web_ui()->CallJavascriptFunction(
      "help.HelpPage.updateIsEnterpriseManaged",
      base::FundamentalValue(IsEnterpriseManaged()));
  // First argument to GetChannel() is a flag that indicates whether
  // current channel should be returned (if true) or target channel
  // (otherwise).
  version_updater_->GetChannel(true,
      base::Bind(&HelpHandler::OnCurrentChannel, weak_factory_.GetWeakPtr()));
  version_updater_->GetChannel(false,
      base::Bind(&HelpHandler::OnTargetChannel, weak_factory_.GetWeakPtr()));
#endif
}

#if defined(OS_MACOSX)
void HelpHandler::PromoteUpdater(const base::ListValue* args) {
  version_updater_->PromoteUpdater();
  // champion - added for about page update

  if (bSetPromoteHide) {
    std::string state_str;
    state_str = "hidden";
    scoped_ptr<base::Value> state_value(base::Value::CreateStringValue(state_str));
    web_ui()->CallJavascriptFunction("help.HelpPage.setPromotionState", *state_value);
  }
  bSetPromoteHide = false;
  // champion   
}
#endif

void HelpHandler::RelaunchNow(const base::ListValue* args) {
  DCHECK(args->empty());
#if defined (OS_WIN)
  // champion : we need delayed start (Amresh)
  //version_updater_->RelaunchBrowser();
  //views::UpdateMechDialog(ADT::QUIT_ON_CLOSE, chrome::FindBrowserWithWebContents(web_ui()->GetWebContents()));
  views::UpdateMechwindow::LaunchWithoutPrompt();
#endif
 
#if defined (OS_MACOSX)
   Profile* profile = ProfileManager::GetLastUsedProfile();
   PrefService* prefs =profile->GetPrefs();
   prefs->SetBoolean(prefs::kAviatorRestarted, true);
   version_updater_->RelaunchBrowser();
#endif
}

void HelpHandler::OpenFeedbackDialog(const base::ListValue* args) {
  DCHECK(args->empty());
  Browser* browser = chrome::FindBrowserWithWebContents(
      web_ui()->GetWebContents());
  chrome::OpenFeedbackDialog(browser);
}

void HelpHandler::OpenHelpPage(const base::ListValue* args) {
  DCHECK(args->empty());
  Browser* browser = chrome::FindBrowserWithWebContents(
      web_ui()->GetWebContents());
  chrome::ShowHelp(browser, chrome::HELP_SOURCE_WEBUI);
}

#if defined(OS_CHROMEOS)

void HelpHandler::SetChannel(const base::ListValue* args) {
  DCHECK(args->GetSize() == 2);

  if (!CanChangeChannel()) {
    LOG(WARNING) << "Non-owner tried to change release track.";
    return;
  }

  base::string16 channel;
  bool is_powerwash_allowed;
  if (!args->GetString(0, &channel) ||
      !args->GetBoolean(1, &is_powerwash_allowed)) {
    LOG(ERROR) << "Can't parse SetChannel() args";
    return;
  }

  version_updater_->SetChannel(base::UTF16ToUTF8(channel),
                               is_powerwash_allowed);
  if (chromeos::UserManager::Get()->IsCurrentUserOwner()) {
    // Check for update after switching release channel.
    version_updater_->CheckForUpdate(base::Bind(&HelpHandler::SetUpdateStatus,
                                                base::Unretained(this)));
  }
}

void HelpHandler::RelaunchAndPowerwash(const base::ListValue* args) {
  DCHECK(args->empty());

  if (IsEnterpriseManaged())
    return;

  PrefService* prefs = g_browser_process->local_state();
  prefs->SetBoolean(prefs::kFactoryResetRequested, true);
  prefs->CommitPendingWrite();

  // Perform sign out. Current chrome process will then terminate, new one will
  // be launched (as if it was a restart).
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->RequestRestart();
}

#endif  // defined(OS_CHROMEOS)

void HelpHandler::SetUpdateStatus(VersionUpdater::Status status,
                                  int progress, const base::string16& message) {
  // Only UPDATING state should have progress set.
  DCHECK(status == VersionUpdater::UPDATING || progress == 0);

  std::string status_str;
  switch (status) {
  case VersionUpdater::CHECKING:
    status_str = "checking";
    break;
  case VersionUpdater::UPDATING:
    status_str = "updating";
    break;
  case VersionUpdater::NEARLY_UPDATED:
    status_str = "nearly_updated";
    break;
  case VersionUpdater::UPDATED:
    status_str = "updated";
    break;
  case VersionUpdater::FAILED:
  case VersionUpdater::FAILED_OFFLINE:
  case VersionUpdater::FAILED_CONNECTION_TYPE_DISALLOWED:
    status_str = "failed";
    break;
  case VersionUpdater::DISABLED:
    status_str = "disabled";
    break;
  }

  web_ui()->CallJavascriptFunction("help.HelpPage.setUpdateStatus",
                                   base::StringValue(status_str),
                                   base::StringValue(message));

  if (status == VersionUpdater::UPDATING) {
    web_ui()->CallJavascriptFunction("help.HelpPage.setProgress",
                                     base::FundamentalValue(progress));
  }

#if defined(OS_CHROMEOS)
  if (status == VersionUpdater::FAILED_OFFLINE ||
      status == VersionUpdater::FAILED_CONNECTION_TYPE_DISALLOWED) {
    base::string16 types_msg = GetAllowedConnectionTypesMessage();
    if (!types_msg.empty()) {
      web_ui()->CallJavascriptFunction(
          "help.HelpPage.setAndShowAllowedConnectionTypesMsg",
          base::StringValue(types_msg));
    } else {
      web_ui()->CallJavascriptFunction(
          "help.HelpPage.showAllowedConnectionTypesMsg",
          base::FundamentalValue(false));
    }
  } else {
    web_ui()->CallJavascriptFunction(
        "help.HelpPage.showAllowedConnectionTypesMsg",
        base::FundamentalValue(false));
  }
#endif  // defined(OS_CHROMEOS)
}
// champion - added code for update mechanism in about page
// static
#if defined(OS_MACOSX)
bool HelpHandler::IsUpdateFilesExists() {
  // path/Library/Aviator
  uid_t current_user_id = getuid();
  struct passwd *pwentry = getpwuid(current_user_id);
  std::string HomeDir = pwentry->pw_dir;
  std::string strAgentPath1 = "/Library/LaunchAgents/com.aviator.agent.plist";
  std::string strAgentPath = HomeDir + strAgentPath1;
  base::FilePath enginePath("/Library/Aviator/AviatorEngine");
  base::FilePath ticketPath("/Library/Aviator/AviatorTicket");
  base::FilePath scriptPath("/Library/Aviator/UpdateAviator.sh");
  base::FilePath daemonPath("/Library/LaunchDaemons/com.aviator.daemon.plist");
  base::FilePath agentPath(strAgentPath);
  //base::FilePath agentPath("/Library/LaunchAgents/com.aviator.agent.plist");

  if (base::PathExists(enginePath) &&
      base::PathExists(ticketPath) &&
      base::PathExists(scriptPath) &&
      base::PathExists(daemonPath) &&
      base::PathExists(agentPath))
    return true;
  else
    return false;

}


void HelpHandler::SetPromotionState(VersionUpdater::PromotionState state) {
  std::string state_str;
  state = VersionUpdater::PROMOTE_HIDDEN;	// champion - set hidden always as per Robert's request on 14th Oct 2013
  switch (state) {
    case VersionUpdater::PROMOTE_HIDDEN:
      state_str = "hidden";
      break;
    case VersionUpdater::PROMOTE_ENABLED:
      state_str = "enabled";
      break;
    case VersionUpdater::PROMOTE_DISABLED:
      state_str = "disabled";
      break;
  }

  if (isAdminUser) { // champion - update mechanism in about page
    web_ui()->CallJavascriptFunction("help.HelpPage.setPromotionState",
      base::StringValue(state_str));
  }
}

// champion - update mechanism in about page
bool HelpHandler::IsAdminUser() { // champion added for To show setup update for all users button only admin user in help page  
  //Is Admin user?

  uid_t current_user_id = getuid();
  struct passwd *pwentry = getpwuid(current_user_id);

  //    struct group *grentry = getgrgid(getgid());
  //    printf("My Current Group Name is %s\n", grentry->gr_name);

  struct group *admin_group = getgrnam("admin");
  while (*admin_group->gr_mem != NULL) {
    if (strcmp(pwentry->pw_name, *admin_group->gr_mem) == 0) {
      return true; // Admin user; no need to copy the 
    }
    admin_group->gr_mem++;
  }
  return false;
}
#endif  // defined(OS_MACOSX)

#if defined(OS_CHROMEOS)
void HelpHandler::OnOSVersion(const std::string& version) {
  web_ui()->CallJavascriptFunction("help.HelpPage.setOSVersion",
                                   base::StringValue(version));
}

void HelpHandler::OnOSFirmware(const std::string& firmware) {
  web_ui()->CallJavascriptFunction("help.HelpPage.setOSFirmware",
                                   base::StringValue(firmware));
}

void HelpHandler::OnCurrentChannel(const std::string& channel) {
  web_ui()->CallJavascriptFunction(
      "help.HelpPage.updateCurrentChannel", base::StringValue(channel));
}

void HelpHandler::OnTargetChannel(const std::string& channel) {
  web_ui()->CallJavascriptFunction(
      "help.HelpPage.updateTargetChannel", base::StringValue(channel));
}

#endif // defined(OS_CHROMEOS)
