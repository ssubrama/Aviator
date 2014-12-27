// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include <windows.h>
#include <tchar.h>

#include <string>
#include <fstream>
#include <tlhelp32.h>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/win/windows_version.h"
#include "chrome/app/client_util.h"
#include "chrome/browser/chrome_process_finder_win.h"
#include "chrome/browser/policy/policy_path_parser.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "chrome_elf/chrome_elf_main.h"
#include "components/startup_metric_utils/startup_metric_utils.h"
#include "content/public/common/result_codes.h"
#include "ui/gfx/win/dpi.h"

namespace {

void CheckSafeModeLaunch() {
  unsigned short k1 = ::GetAsyncKeyState(VK_CONTROL);
  unsigned short k2 = ::GetAsyncKeyState(VK_MENU);
  const unsigned short kPressedMask = 0x8000;
  if ((k1 & kPressedMask) && (k2 & kPressedMask))
    ::SetEnvironmentVariableA(chrome::kSafeModeEnvVar, "1");
}

// List of switches that it's safe to rendezvous early with. Fast start should
// not be done if a command line contains a switch not in this set.
// Note this is currently stored as a list of two because it's probably faster
// to iterate over this small array than building a map for constant time
// lookups.
const char* const kFastStartSwitches[] = {
  switches::kProfileDirectory,
  switches::kShowAppList,
};

bool IsFastStartSwitch(const std::string& command_line_switch) {
  for (size_t i = 0; i < arraysize(kFastStartSwitches); ++i) {
    if (command_line_switch == kFastStartSwitches[i])
      return true;
  }
  return false;
}

bool ContainsNonFastStartFlag(const CommandLine& command_line) {
  const CommandLine::SwitchMap& switches = command_line.GetSwitches();
  if (switches.size() > arraysize(kFastStartSwitches))
    return true;
  for (CommandLine::SwitchMap::const_iterator it = switches.begin();
       it != switches.end(); ++it) {
    if (!IsFastStartSwitch(it->first))
      return true;
  }
  return false;
}

bool AttemptFastNotify(const CommandLine& command_line) {
  if (ContainsNonFastStartFlag(command_line))
    return false;

  base::FilePath user_data_dir;
  if (!chrome::GetDefaultUserDataDirectory(&user_data_dir))
    return false;
  policy::path_parser::CheckUserDataDirPolicy(&user_data_dir);

  HWND chrome = chrome::FindRunningChromeWindow(user_data_dir);
  if (!chrome)
    return false;
  return chrome::AttemptToNotifyRunningChrome(chrome, true) ==
      chrome::NOTIFY_SUCCESS;
}

}  // namespace

//Champion - start for Update of AViator Auto Update  file and removal of au.lock in case AviatorAutoUpdate.exe did not delete it.


DWORD FindProcessId(const std::wstring& processName) 
{
  PROCESSENTRY32 processInfo;
  processInfo.dwSize = sizeof(processInfo);

  HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if ( processesSnapshot == INVALID_HANDLE_VALUE )
    return 0;

  Process32First(processesSnapshot, &processInfo);
  if ( !processName.compare(processInfo.szExeFile) )
  {
    CloseHandle(processesSnapshot);
    return processInfo.th32ProcessID;
  }

  while ( Process32Next(processesSnapshot, &processInfo) )
  {
    if ( !processName.compare(processInfo.szExeFile) )
    {
      CloseHandle(processesSnapshot);
      return processInfo.th32ProcessID;
    }
  }

  CloseHandle(processesSnapshot);
  return 0;
}

void updateAviatorAutoUpdate() {
	
	char* aviatorUpdateLocation="SOFTWARE\\Wow6432Node\\WhiteHat\\Update"; 
	DWORD dwType;
	bool isAdmin = true;
    	char  szVersion[255];
    	DWORD dwDataSize = 255;
    	memset(szVersion, 0, 255);
    	HKEY hkeyDXVer;
	long lResult2 = RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);
	if (ERROR_SUCCESS != lResult2){
		aviatorUpdateLocation="SOFTWARE\\WhiteHat\\Update"; 
    if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer)){
			  isAdmin = false;
    }
	}
	if (RegOpenKeyExA(HKEY_CURRENT_USER, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer)!=ERROR_SUCCESS){
		RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);
	}
	if (RegQueryValueExA(hkeyDXVer, "path", NULL, &dwType, (BYTE*)szVersion, &dwDataSize)!=ERROR_SUCCESS){
		
		return ;
	}
	std::string ver(szVersion);
	bool copyRequired = false;
	unsigned pos = (unsigned)ver.find("Update");        
	std::string localUpdate = ver.substr(0,pos); 
	localUpdate.append("Application\\AviatorAutoUpdate.exe");
	std::ifstream reader (localUpdate.c_str());
	if ( reader.is_open()){
		reader.close();
		copyRequired=true;
	}
	if (FindProcessId(L"AviatorAutoUpdate.exe")){
		return ;
	}
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	std::string version;
	long long minor = osvi.dwMinorVersion;
	long long major = osvi.dwMajorVersion;
	version.append(std::to_string(major));
	version.append(".");
	version.append(std::to_string(minor));
	std::string lock;
/*	if (atof("5.1") == atof(version.c_str()) ){
		lock.append("C:\\Documents and Settings\\");
		lock.append(getenv("USERNAME"));
		lock.append("\\Local Settings\\Application Data\\Aviator\\User Data\\");
		lock.append("au.lock");
		std::ifstream lockReader (lock.c_str());
	if (lockReader.is_open()){
		lockReader.close();
			std::string deleteCommand;
			deleteCommand.append("del /f \"");
			deleteCommand.append(lock);
			deleteCommand.append(" \"");
			system(deleteCommand.c_str());
		}
	}
	else{
		lock.append("C:\\Users\\");
		lock.append(getenv("USERNAME"));
		lock.append("\\AppData\\Local\\Aviator\\User Data\\");
		lock.append("au.lock");
		std::ifstream lockReader (lock.c_str());
		if (lockReader.is_open()){
			lockReader.close();
			std::string deleteCommand;
			deleteCommand.append("del /f \"");
			deleteCommand.append(lock);
			deleteCommand.append(" \"");
			system(deleteCommand.c_str());
			
		}
	}*/
	std::string com;
	if (!copyRequired){
		return ;
	}
	//unsigned pos = ver.find("Update");        
	std::string path = ver.substr(0,pos); 
	path.append("Application\\patch.exe");
	com.append("powershell.exe -windowstyle hidden -c  start ");
	if (isAdmin && atof("5.1") != atof(version.c_str())){
		com.append(" -Verb runAs ");  
	}
	com.append("'");
	com.append(path);
	com.append(" ' -windowstyle hidden ");
  WinExec(com.c_str(),SW_HIDE);
	//system(com.c_str());
}

//Champion - End for Update of AViator Auto Update  file and removal of au.lock in case AviatorAutoUpdate.exe did not delete it.

#if !defined(ADDRESS_SANITIZER)
int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prev, wchar_t*, int) {
#else
// The AddressSanitizer build should be a console program as it prints out stuff
// on stderr.
int main() {
  HINSTANCE instance = GetModuleHandle(NULL);
#endif
  startup_metric_utils::RecordExeMainEntryTime();

  // Signal Chrome Elf that Chrome has begun to start.
  SignalChromeElf();

  // Initialize the commandline singleton from the environment.
  CommandLine::Init(0, NULL);
  // The exit manager is in charge of calling the dtors of singletons.
  base::AtExitManager exit_manager;

  // We don't want to set DPI awareness on Vista because we don't support
  // DirectWrite there. GDI fonts are kerned very badly, so better to leave
  // DPI-unaware and at effective 1.0. See also ShouldUseDirectWrite().
  if (base::win::GetVersion() > base::win::VERSION_VISTA)
    gfx::EnableHighDPISupport();

  if (AttemptFastNotify(*CommandLine::ForCurrentProcess()))
    return 0;

  CheckSafeModeLaunch();
  
  // Champion - Update of Aviator Auto Update file and removal of au.lock in case AviatorAutoUpdate.exe did not delete it.
  updateAviatorAutoUpdate();

  // Load and launch the chrome dll. *Everything* happens inside.
  MainDllLoader* loader = MakeMainDllLoader();
  int rc = loader->Launch(instance);
  loader->RelaunchChromeBrowserWithNewCommandLineIfNeeded();
  delete loader;
  return rc;
}
