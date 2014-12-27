// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_LOCK_DIALOG_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_LOCK_DIALOG_H_

#include "base/callback_forward.h"
#include "ui/gfx/native_widget_types.h"

namespace importer {

// This function is called by an ImporterHost, and presents the Firefox profile
// warning dialog. After closing the dialog, the ImportHost receives a callback
// with the message either to skip the import, or to continue the process.
void ShowImportLockDialog(gfx::NativeWindow parent,
                          const base::Callback<void(bool)>& callback);

void ShowImportLockChromeDialog(bool is_chrome, gfx::NativeWindow parent, const base::Callback<void(bool)>& callback); //champion import bookmarks
}  // namespace importer

#endif  // CHROME_BROWSER_IMPORTER_IMPORTER_LOCK_DIALOG_H_
