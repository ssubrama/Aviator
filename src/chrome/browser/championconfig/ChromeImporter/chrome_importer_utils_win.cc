// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/chromeimporter/chrome_importer_utils.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/windows_version.h"

#include <shlobj.h>

#include "base/file_util.h"
#include "base/path_service.h"
#include "base/strings/string16.h"
#include "base/win/registry.h"
#include <string>
#include<fstream>
#include<iostream>

#if defined(OS_WIN)
#include <windows.h>
#include<WinBase.h>
#endif

// NOTE: Keep these in order since we need test all those paths according
// to priority. For example. One machine has multiple users. One non-admin
// user installs Chrome 2, which causes there is a Chrome2 entry under HKCU.
// One admin user installs Chrome 3, which causes there is a Chrome 3 entry
// under HKLM. So when the non-admin user log in, we should deal with Chrome 2
// related data instead of Chrome 3.
static const HKEY kChromeRegistryPaths[] = {
  HKEY_CURRENT_USER,
  HKEY_LOCAL_MACHINE
};

// Google registeries
static const wchar_t* kChromePath_Win7 = L"Software\\Wow6432Node\\Google\\Update\\ClientState\\{8A69D345-D564-463c-AFF1-A69D9E530F96}"; // L"Software\\Mozilla\\Google Chrome";
static const wchar_t* kChromePath_XP = L"Software\\Google\\Update\\ClientState\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
static const wchar_t* kChromePath = L"Software\\Wow6432Node\\Google\\Update\\ClientState"; // L"Software\\Mozilla\\Google Chrome";
static const wchar_t* kChromPath = L"Software\\Google\\Update\\ClientState";
static const wchar_t* kCurrentVersion = L"CurrentVersion";
static const wchar_t* kAviatorPath = L"SOFTWARE\\Wow6432Node\\WhiteHat\\Update"; 
static const wchar_t* kAviatorPath32bit = L"SOFTWARE\\WhiteHat\\Update"; 

int GetCurrentChromeMajorVersionFromRegistry() {
  TCHAR ver_buffer[128];
  DWORD ver_buffer_length = sizeof(ver_buffer);
  int highest_version = 0;

  // When installing Chrome with admin account, the product keys will be
  // written under HKLM\Mozilla. Otherwise it the keys will be written under
  // HKCU\Mozilla.
  for (int i = 0; i < arraysize(kChromeRegistryPaths); ++i) {
    base::win::RegKey reg_key(kChromeRegistryPaths[i], kChromePath, KEY_READ);
    LONG result = reg_key.ReadValue(kCurrentVersion, ver_buffer, &ver_buffer_length, NULL);

    if (result != ERROR_SUCCESS)
      continue;

    highest_version = std::max(highest_version, _wtoi(ver_buffer));
  }

  return highest_version;
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
  base::string16 registry_path = L"Software\\Wow6432Node\\Google\\Update\\ClientState";
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  registry_path += L"\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE,
                                      registry_path.c_str(), KEY_READ);
  LONG result = reg_key_directory.ReadValue(L"LastInstallerSuccessLaunchCmdLine",buffer,
                                       &buffer_length, NULL);

  if(result!=ERROR_SUCCESS){
  base::string16 registry_path = L"Software\\Google\\Update\\ClientState";
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
  registry_path += L"\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"LastInstallerSuccessLaunchCmdLine", buffer, &buffer_length, NULL);
  }
  
  if(result!=ERROR_SUCCESS){
  base::string16 registry_path = L"Software\\Google\\Update\\ClientState";
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  registry_path += L"\\{8A69D345-D564-463c-AFF1-A69D9E530F96}";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"LastInstallerSuccessLaunchCmdLine", buffer, &buffer_length, NULL);
  }
  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetAviatorInstallPathFromRegistry() {
  // champion Detects the path that Aviator is installed in.
  base::string16 registry_path = kAviatorPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(L"path", buffer, &buffer_length, NULL);

  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"path",buffer, &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetAviatorInstallPathFromRegistry32bit() {
  // champion Detects the path that Aviator is installed in.
  base::string16 registry_path = kAviatorPath32bit;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
  LONG result = reg_key.ReadValue(L"path", buffer, &buffer_length, NULL);

  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE, registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"path",buffer, &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

std::string GetAviatorInstallPathFromRegistry32bitW() {
  char* aviatorUpdateLocation = "SOFTWARE\\Wow6432Node\\WhiteHat\\Update"; 
	DWORD dwType;
  base::FilePath path;
  char szVersion[255];
  DWORD dwDataSize = 255;
  memset(szVersion, 0, 255);
  HKEY hkeyDXVer;
	long lResult2 = RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);

	if (ERROR_SUCCESS != lResult2)
		aviatorUpdateLocation="SOFTWARE\\WhiteHat\\Update"; 
	
	if (RegOpenKeyExA(HKEY_CURRENT_USER, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer)!=ERROR_SUCCESS)
		RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);

	if (RegQueryValueExA(hkeyDXVer, "path", NULL, &dwType, (BYTE*)szVersion, &dwDataSize)!=ERROR_SUCCESS)
	  return szVersion;

  return std::string(szVersion);
}
// champion : added for dir check
bool DirExists(const std::string& dirName_in) {
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}

bool exists(const std::string& filePath) {
	return true;
	/*//This will get the file attributes bitlist of the file
	DWORD fileAtt = GetFileAttributesA(filePath.c_str());

	//If an error occurred it will equal to INVALID_FILE_ATTRIBUTES
	if(fileAtt == INVALID_FILE_ATTRIBUTES)
		return false;
	if( fileAtt & FILE_ATTRIBUTE_DIRECTORY )
		return false;
	else
		return true;*/
}

bool isChromeInstall() {
	std::string strProgramPath = "C:";
	std::string strCheckFolderFor64bitWin = "C:\\Program Files (x86)";
	
	if (DirExists (strCheckFolderFor64bitWin))
		strProgramPath += "\\Program Files (x86)\\";
	else
		strProgramPath += "\\Program Files\\";
    
	std::string m_stralternatebrowser_ = "";
	std::string strGCPath = "\"" + strProgramPath + "Google\\Chrome\\Application\\chrome.exe\"";
	
	if(exists(strGCPath))
		return true;

  return false;
}

base::FilePath GetProfilesPreferencePath() {
  base::FilePath Preference_file_path;
  // The default location of the profile folder containing user data is
  // under the "Application Data" folder in Windows XP, Vista, and 7.
  
  // first we should check chrome is installed or not 
  if(!isChromeInstall())
	  return base::FilePath();

  if (!PathService::Get(base::DIR_LOCAL_APP_DATA, &Preference_file_path))
    return base::FilePath();

  Preference_file_path = Preference_file_path.AppendASCII("Google");
  Preference_file_path = Preference_file_path.AppendASCII("Chrome");
  Preference_file_path = Preference_file_path.AppendASCII("User Data");
  Preference_file_path = Preference_file_path.AppendASCII("Default");
  Preference_file_path = Preference_file_path.AppendASCII("Preferences");
  
  return base::PathExists(Preference_file_path) ? Preference_file_path : base::FilePath();
}
