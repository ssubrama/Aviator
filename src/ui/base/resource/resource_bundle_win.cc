// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "ui/base/resource/resource_bundle_win.h"

#include "base/logging.h"
#include "base/path_service.h"
#include "base/strings/utf_string_conversions.h"
#include "skia/ext/image_operations.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/resource/resource_data_dll_win.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_source.h"
#include "ui/gfx/win/dpi.h"

namespace ui {

namespace {

HINSTANCE resources_data_dll;

HINSTANCE GetCurrentResourceDLL() {
  if (resources_data_dll)
    return resources_data_dll;
  return GetModuleHandle(NULL);
}

base::FilePath GetResourcesPakFilePath(const std::string& pak_name) {
  base::FilePath path;
  if (PathService::Get(base::DIR_MODULE, &path))
    return path.AppendASCII(pak_name.c_str());

  // Return just the name of the pack file.
  return base::FilePath(base::ASCIIToUTF16(pak_name));
}

}  // namespace

void ResourceBundle::LoadCommonResources() {
  // As a convenience, add the current resource module as a data packs.
  data_packs_.push_back(new ResourceDataDLL(GetCurrentResourceDLL()));

  if (IsScaleFactorSupported(SCALE_FACTOR_100P)) {
    AddDataPackFromPath(
        GetResourcesPakFilePath("Aviator_100_percent.pak"), // champion : app name changes (amresh)
        SCALE_FACTOR_100P);
  }
  if (IsScaleFactorSupported(SCALE_FACTOR_200P)) {
    DCHECK(gfx::IsHighDPIEnabled());
    AddDataPackFromPath(
        GetResourcesPakFilePath("Aviator_200_percent.pak"), // champion : app name changes (amresh)
        SCALE_FACTOR_200P);
  }
}

gfx::Image& ResourceBundle::GetNativeImageNamed(int resource_id, ImageRTL rtl) {
  // Flipped image is not used on Windows.
  DCHECK_EQ(rtl, RTL_DISABLED);

  // Windows only uses SkBitmap for gfx::Image, so this is the same as
  // GetImageNamed.
  return GetImageNamed(resource_id);
}

void SetResourcesDataDLL(HINSTANCE handle) {
  resources_data_dll = handle;
}

HICON LoadThemeIconFromResourcesDataDLL(int icon_id) {
  return ::LoadIcon(GetCurrentResourceDLL(), MAKEINTRESOURCE(icon_id));
}

}  // namespace ui;
