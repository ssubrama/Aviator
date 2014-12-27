// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#include "chrome/common/importer/firefox_importer_utils.h"

#include <shlobj.h>

#include "base/file_util.h"
#include "base/path_service.h"
#include "base/strings/string16.h"
#include "base/win/registry.h"

// NOTE: Keep these in order since we need test all those paths according
// to priority. For example. One machine has multiple users. One non-admin
// user installs Firefox 2, which causes there is a Firefox2 entry under HKCU.
// One admin user installs Firefox 3, which causes there is a Firefox 3 entry
// under HKLM. So when the non-admin user log in, we should deal with Firefox 2
// related data instead of Firefox 3.
static const HKEY kFireFoxRegistryPaths[] = {
  HKEY_CURRENT_USER,
  HKEY_LOCAL_MACHINE
};

static const wchar_t* kFirefoxPath = L"Software\\Mozilla\\Mozilla Firefox";
static const wchar_t* kIEPath = L"Software\\Microsoft\\Internet Explorer"; // champion
static const wchar_t* kCurrentVersion = L"CurrentVersion";

int GetCurrentFirefoxMajorVersionFromRegistry() {
  TCHAR ver_buffer[128];
  DWORD ver_buffer_length = sizeof(ver_buffer);
  int highest_version = 0;
  // When installing Firefox with admin account, the product keys will be
  // written under HKLM\Mozilla. Otherwise it the keys will be written under
  // HKCU\Mozilla.
  for (int i = 0; i < arraysize(kFireFoxRegistryPaths); ++i) {
    base::win::RegKey reg_key(kFireFoxRegistryPaths[i], kFirefoxPath,
                              KEY_READ);

    LONG result = reg_key.ReadValue(kCurrentVersion, ver_buffer,
                                    &ver_buffer_length, NULL);
    if (result != ERROR_SUCCESS)
      continue;
    highest_version = std::max(highest_version, _wtoi(ver_buffer));
  }
  return highest_version;
}

base::FilePath GetFirefoxInstallPathFromRegistry() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kFirefoxPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(),
                            KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer,
                                  &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\" + base::string16(buffer) + L"\\Main";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE,
                                      registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"Install Directory", buffer,
                                       &buffer_length, NULL);
  // champion : start
  if(result != ERROR_SUCCESS) {
  
    base::string16 registry_path = kFirefoxPath;
    wchar_t buffer[MAX_PATH];
    DWORD buffer_length = sizeof(buffer);
    base::win::RegKey reg_key(HKEY_CURRENT_USER, registry_path.c_str(), KEY_READ);
    LONG result = reg_key.ReadValue(kCurrentVersion, buffer, &buffer_length, NULL);
    if (result != ERROR_SUCCESS)
      return base::FilePath();

    registry_path += L"\\" + base::string16(buffer) + L"\\Main";
    buffer_length = sizeof(buffer);
    base::win::RegKey reg_key_directory(HKEY_CURRENT_USER,
                                        registry_path.c_str(), KEY_READ);
    result = reg_key_directory.ReadValue(L"Install Directory", buffer,
                                         &buffer_length, NULL);
  }
  // champion : end

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

//champion - check install path in HK_CURRENT_USER directory in REGEDIT - Dinesh
base::FilePath GetFfoxInstallPathFromRegistry() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kFirefoxPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_CURRENT_USER, registry_path.c_str(),
                            KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer,
                                  &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\" + base::string16(buffer) + L"\\Main";
  buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_CURRENT_USER,
                                      registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"Install Directory", buffer,
                                       &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetIEInstallPathFromRegistryLM() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kIEPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(),
                            KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer,
                                  &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\Main";
  // buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_LOCAL_MACHINE,
                                      registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"x86AppPath", buffer,
                                       &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetIEInstallPathFromRegistryCU() {
  // Detects the path that Firefox is installed in.
  base::string16 registry_path = kIEPath;
  wchar_t buffer[MAX_PATH];
  DWORD buffer_length = sizeof(buffer);
  base::win::RegKey reg_key(HKEY_LOCAL_MACHINE, registry_path.c_str(),
                            KEY_READ);
  LONG result = reg_key.ReadValue(kCurrentVersion, buffer,
                                  &buffer_length, NULL);
  if (result != ERROR_SUCCESS)
    return base::FilePath();

  registry_path += L"\\Main";
  // buffer_length = sizeof(buffer);
  base::win::RegKey reg_key_directory(HKEY_CURRENT_USER,
                                      registry_path.c_str(), KEY_READ);
  result = reg_key_directory.ReadValue(L"x86AppPath", buffer,
                                       &buffer_length, NULL);

  return (result != ERROR_SUCCESS) ? base::FilePath() : base::FilePath(buffer);
}

base::FilePath GetProfilesINI() {
  base::FilePath ini_file;
  // The default location of the profile folder containing user data is
  // under the "Application Data" folder in Windows XP, Vista, and 7.
  if (!PathService::Get(base::DIR_APP_DATA, &ini_file))
    return base::FilePath();

  ini_file = ini_file.AppendASCII("Mozilla");
  ini_file = ini_file.AppendASCII("Firefox");
  ini_file = ini_file.AppendASCII("profiles.ini");

  return base::PathExists(ini_file) ? ini_file : base::FilePath();
}
