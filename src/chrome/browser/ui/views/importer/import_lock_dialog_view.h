// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ui/views/view.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class Label;
}

// ImportLockDialogView asks the user to shut down Firefox before starting the
// profile import.
class ImportLockDialogView : public views::DialogDelegateView {
 public:
  static void Show(bool is_chrome, gfx::NativeWindow parent, 
                   const base::Callback<void(bool)>& callback); //update show function for champion import bookmarks

 private:
  explicit ImportLockDialogView(const base::Callback<void(bool)>& callback);
  explicit ImportLockDialogView(const base::Callback<void(bool)>& callback, bool is_chrome); //champion for import bookmarks
  virtual ~ImportLockDialogView();

  // views::View:
  virtual gfx::Size GetPreferredSize() const OVERRIDE;
  virtual void Layout() OVERRIDE;

  // views::DialogDelegate:
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

 private:
  views::Label* description_label_;

  // Called with the result of the dialog.
  base::Callback<void(bool)> callback_;

  DISALLOW_COPY_AND_ASSIGN(ImportLockDialogView);
};

#endif  // CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_
