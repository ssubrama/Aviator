// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h"

#include "base/file_util.h"
#include "base/path_service.h"
#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"

#include "net/base/escape.h"

#if defined(OS_WIN)
#include "base/win/registry.h"
#include "base/win/windows_version.h"
#endif
namespace ABF { 

static const wchar_t* kFirefoxPath = L"Software\\Mozilla\\Mozilla Firefox";
static const wchar_t* kIEPath = L"Software\\Microsoft\\Internet Explorer";
static const wchar_t* kChromePath = L"Software\\Wow6432Node\\Google\\Update\\ClientState"; // L"Software\\Mozilla\\Google Chrome";
static const wchar_t* kChromPath = L"Software\\Google\\Update\\ClientState";
static const wchar_t* kCurrentVersion = L"CurrentVersion";
static const wchar_t* kChromePath_Win7 = L"Software\\Wow6432Node\\Google\\Update\\ClientState\\{8A69D345-D564-463c-AFF1-A69D9E530F96}"; // 
static const wchar_t* kChromePath_XP = L"Software\\Google\\Update\\ClientState\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
static const wchar_t* kAviatorPath = L"SOFTWARE\\Wow6432Node\\WhiteHat\\Update"; 
const char kChromeSigninEffectiveSite[] = "chrome-extension://acfccoigjajmmgbhpfbjnpckhjjegnih";
const char kChromiumSyncService[] = "service=chromiumsync";


base::FilePath GetFirefoxInstallPathFromRegistry() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kFirefoxPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer, &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\" + base::string16(buffer) + L"\\Main";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"Install Directory", buffer, &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

std::string GetChromeInstalledPath() {
  std::string defaultBrowserPath;
  base::string16 strProgid;
  std::string strProogID_IE;

  if( base::win::GetVersion() >= base::win::VERSION_VISTA) {
    base::win::RegKey regkey(HKEY_CURRENT_USER,kChromePath_Win7, KEY_READ);
    
    regkey.ReadValue(L"LastInstallerSuccessLaunchCmdLine",&strProgid);
    strProogID_IE = base::UTF16ToUTF8(strProgid);

    if(!strProogID_IE.empty())
      return strProogID_IE;

    base::win::RegKey regkey1(HKEY_LOCAL_MACHINE,kChromePath_Win7, KEY_READ);
    regkey1.ReadValue(L"LastInstallerSuccessLaunchCmdLine",&strProgid);
    strProogID_IE=base::UTF16ToUTF8(strProgid);

    if(!strProogID_IE.empty())
      return strProogID_IE;
  } else {
    base::win::RegKey regkey(HKEY_CURRENT_USER,kChromePath_XP, KEY_READ);

    regkey.ReadValue(L"LastInstallerSuccessLaunchCmdLine",&strProgid);
    strProogID_IE=base::UTF16ToUTF8(strProgid);

    if(!strProogID_IE.empty())
      return strProogID_IE;

    base::win::RegKey regkey1(HKEY_LOCAL_MACHINE,kChromePath_XP, KEY_READ);

    regkey1.ReadValue(L"LastInstallerSuccessLaunchCmdLine",&strProgid);
    strProogID_IE=base::UTF16ToUTF8(strProgid);

    if(!strProogID_IE.empty())
      return strProogID_IE;
  }

  return std::string();
}

base::FilePath GetChromeInstallPathFromRegistry() {
  // Detects the path that Chrome is installed in.
  base::string16 registry_path = kChromePath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer, &buffer_length, NULL);
 
  registry_path += L"\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"LastInstallerSuccessLaunchCmdLine",buffer, &buffer_length, NULL);
  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetIEInstallPathFromRegistryLM() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kIEPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer, &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\Main";
  // buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"x86AppPath", buffer, &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetIEInstallPathFromRegistryCU() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kIEPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer, &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\Main";
  // buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"x86AppPath", buffer, &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

bool IsFirefoxDefaultBrowser() {
  bool ff_default = false;
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    base::string16 app_cmd;
    base::win::RegKey key(HKEY_CURRENT_USER, kRegVistaUrlPrefs, KEY_READ);
    if (key.Valid() && (key.ReadValue(L"Progid", &app_cmd) == ERROR_SUCCESS) && 
        app_cmd == L"FirefoxURL")
      ff_default = true;
  } else {
    base::string16 key_path(L"http");
    key_path.append(kRegShellOpen);
    base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);
    base::string16 app_cmd;
    if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) &&
        base::string16::npos != StringToLowerASCII(app_cmd).find(L"firefox"))
      ff_default = true;
  }
  
  return ff_default;
}

bool IsGoogleChromeDefaultBrowser() {
  bool ff_default = false;
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    base::string16 app_cmd;
    base::win::RegKey key(HKEY_CURRENT_USER, kRegVistaUrlPrefs, KEY_READ);
    if (key.Valid() && (key.ReadValue(L"Progid", &app_cmd) == ERROR_SUCCESS) &&
        app_cmd == L"ChromeHTML")
      ff_default = true;
  } else {
    base::string16 key_path(L"http");
    key_path.append(kRegShellOpen);
    base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);
    base::string16 app_cmd;
    if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) &&
        base::string16::npos != StringToLowerASCII(app_cmd).find(L"chrome"))
      ff_default = true;
  }
  return ff_default;
}

bool IsAviatorDefaultBrowser() {
  bool ff_default = false;
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    base::string16 app_cmd;
    base::win::RegKey key(HKEY_CURRENT_USER, kRegVistaUrlPrefs, KEY_READ);
    if (key.Valid() && (key.ReadValue(L"Progid", &app_cmd) == ERROR_SUCCESS) &&
        base::string16::npos != StringToLowerASCII(app_cmd).find(L"aviator"))
      ff_default = true;
  } else {
    base::string16 key_path(L"http");
    key_path.append(kRegShellOpen);
    base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);
    base::string16 app_cmd;
    if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) &&
        base::string16::npos != StringToLowerASCII(app_cmd).find(L"aviator"))
      ff_default = true;
  }
  return ff_default;
}

bool IsIEDefaultBrowser() {
  bool ff_default = false;
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    base::string16 app_cmd;
    base::win::RegKey key(HKEY_CURRENT_USER,
                          kRegVistaUrlPrefs, KEY_READ);
    if (key.Valid() && (key.ReadValue(L"Progid", &app_cmd) == ERROR_SUCCESS) &&
        app_cmd == L"IE.HTTP")
      ff_default = true;
  } else {
    base::string16 key_path(L"http");
    key_path.append(kRegShellOpen);
    base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);
    base::string16 app_cmd;
    if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) &&
        base::string16::npos != StringToLowerASCII(app_cmd).find(L"iexplore"))
      ff_default = true;
  }
  return ff_default;
}

bool IsWebBasedSigninFlowURL(const GURL& url) {
  GURL effective(kChromeSigninEffectiveSite);
  if (url.SchemeIs(effective.scheme().c_str()) &&
      url.host() == effective.host()) {
    return true;
  }

  GURL service_login(GaiaUrls::GetInstance()->service_login_url());
  if (url.GetOrigin() != service_login.GetOrigin())
    return false;

  // Any login UI URLs with signin=chromiumsync should be considered a web
  // URL (relies on GAIA keeping the "service=chromiumsync" query string
  // fragment present even when embedding inside a "continue" parameter).
  return net::UnescapeURLComponent(url.query(),
                                   net::UnescapeRule::URL_SPECIAL_CHARS)
             .find(kChromiumSyncService) != std::string::npos;
}

std::string GetDefaultBrowserForAviator() {
  if( base::win::GetVersion() >= base::win::VERSION_VISTA) {
    std::string defaultBrowserPath;
    base::string16 strProgid;
    std::string strProogID_IE;

    base::win::RegKey regkey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\shell\\Associations\\UrlAssociations\\http\\UserChoice", KEY_READ);

    regkey.ReadValue(L"Progid",&strProgid);
    strProogID_IE=base::UTF16ToUTF8(strProgid);

    size_t pos=strProogID_IE.find("IE.HTTP");

    if(pos!=std::string::npos) { //default browser IE
      base::FilePath strIEMachinePath = GetIEInstallPathFromRegistryLM();
      base::FilePath strIEUserPath = GetIEInstallPathFromRegistryCU();

      std::string strProgramPath = "C:";
      std::string strIEPath;
      std::string strCheckFolderFor64bitWin = "C:\\Program Files (x86)";

      if (DirExists (strCheckFolderFor64bitWin))
        strProgramPath += "\\Program Files (x86)\\";
      else
        strProgramPath += "\\Program Files\\"; 

      if(!strIEMachinePath.empty())
        strIEPath = base::WideToUTF8(strIEMachinePath.value());
      else if(!strIEUserPath.empty())
        strIEPath = base::WideToUTF8(strIEUserPath.value());
      else
        strIEPath = strProgramPath + "Internet Explorer\\iexplore.exe ";

      return strIEPath;
    } else { //default browser not IE
      base::string16 key_path(L"http\\shell\\open\\command");
      base::string16 app_cmd;
      std ::string browser_name;
      std::string strBrowser;

      base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);

      if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) )
        browser_name= base::WideToUTF8(app_cmd);

      std::string strTempBrowser=browser_name;

      int pos = strTempBrowser.find("exe");

      if(pos!=-1) {
        strBrowser=strTempBrowser.erase(pos+3,strTempBrowser.length()-1);
        strBrowser=strBrowser.substr(1,strTempBrowser.length()-1);
      }
      
      return strBrowser;
    }
  } else {
    base::string16 key_path(L"http\\shell\\open\\command");
    base::string16 app_cmd;
    std ::string browser_name;
    std::string strBrowser;

    base::win::RegKey key(HKEY_CLASSES_ROOT, key_path.c_str(), KEY_READ);

    if (key.Valid() && (key.ReadValue(L"", &app_cmd) == ERROR_SUCCESS) )
      browser_name= base::WideToUTF8(app_cmd);

    std::string strTempBrowser=browser_name;

    int pos = strTempBrowser.find("exe");
    //::MessageBoxA(0,strTempBrowser.c_str(),(LPCSTR)"Temp Browser",1);
    if(pos!=-1) {
      strBrowser=strTempBrowser.erase(pos+3,strTempBrowser.length()-1);
      strBrowser=strBrowser.substr(1,strTempBrowser.length()-1);
    }
    
    return strBrowser;
  }
}

// champion : added for dir check (amresh)
bool DirExists(const std::string& dirName_in) {
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!
    
  return false;    // this is not a directory!
}

} // namespace ABF