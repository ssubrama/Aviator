// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.
// champion : app name change

#ifndef CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_
#define CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_

namespace mini_installer {

// Various filenames
const wchar_t kSetupName[] = L"setup.exe";
const wchar_t kChromePrefix[] = L"Aviator";
const wchar_t kSetupPrefix[] = L"setup";

// setup.exe command line arguments
const wchar_t kCmdInstallArchive[] = L" --install-archive";
const wchar_t kCmdUpdateSetupExe[] = L" --update-setup-exe";
const wchar_t kCmdNewSetupExe[] = L" --new-setup-exe";

// Temp directory prefix that this process creates
const wchar_t kTempPrefix[] = L"CR_";
// Google Update will use the full installer if this suffix is found in the ap
// value.
const wchar_t kFullInstallerSuffix[] = L"-full";
// ap value tag for a multi-install product.
const wchar_t kMultiInstallTag[] = L"-multi";

// The resource types that would be unpacked from the mini installer.
// 'BN' is uncompressed binary and 'BL' is LZ compressed binary.
const wchar_t kBinResourceType[] = L"BN";
const wchar_t kLZCResourceType[] = L"BL";
const wchar_t kLZMAResourceType[] = L"B7";

// Registry key to get uninstall command
const wchar_t kApRegistryValueName[] = L"ap";
// Registry key that tells Chrome installer not to delete extracted files.
const wchar_t kCleanupRegistryValueName[] = L"ChromeInstallerCleanup";
// Registry key to get uninstall command
const wchar_t kUninstallRegistryValueName[] = L"UninstallString";

// Paths for the above registry keys
#if defined(GOOGLE_CHROME_BUILD)
// The concatenation of this plus the Google Update GUID is the app registry
// key.
const wchar_t kApRegistryKeyBase[] = L"Software\\WhiteHat\\Aviator\\Update\\ClientState\\";
const wchar_t kUninstallRegistryKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WhtieHat\\Aviator";
const wchar_t kCleanupRegistryKey[] = L"Software\\WhiteHat\\Aviator";
#else
const wchar_t kApRegistryKeyBase[] = L"Software\\WhiteHat\\Aviator";
const wchar_t kUninstallRegistryKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WhiteHat\\Aviator";
const wchar_t kCleanupRegistryKey[] = L"Software\\WhiteHat\\Aviator";
#endif

// One gigabyte is the biggest resource size that it can handle.
const int kMaxResourceSize = 1024*1024*1024;

// This is the file that contains the list of files to be linked in the
// executable. This file is updated by the installer generator tool chain.
const wchar_t kManifestFilename[] = L"packed_files.txt";

}  // namespace mini_installer

#endif  // CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_

