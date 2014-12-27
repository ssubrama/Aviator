// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/ui/views/importer/import_lock_dialog_view.h"

#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/importer/importer_lock_dialog.h"
#include "content/public/browser/user_metrics.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "grit/locale_settings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/layout_constants.h"
#include "ui/views/widget/widget.h"

using base::UserMetricsAction;
bool checkbool =false; //champion- Import bookmark title -  Dinesh

namespace importer {

void ShowImportLockDialog(gfx::NativeWindow parent,
                          const base::Callback<void(bool)>& callback) {
  ImportLockDialogView::Show(false, parent, callback);
  content::RecordAction(UserMetricsAction("ImportLockDialogView_Shown"));
}

//added for champion
void ShowImportLockChromeDialog(bool is_chrome, gfx::NativeWindow parent, const base::Callback<void(bool)>& callback) {
  checkbool=is_chrome;
	ImportLockDialogView::Show(is_chrome,parent, callback);
  content::RecordAction(UserMetricsAction("ImportLockDialogView_Shown"));
  checkbool=false;//Champion -Dinesh
}

}  // namespace importer

// static
void ImportLockDialogView::Show(bool is_chrome, gfx::NativeWindow parent,
                                const base::Callback<void(bool)>& callback) {
  if(!is_chrome)
    views::DialogDelegate::CreateDialogWidget(new ImportLockDialogView(callback,is_chrome), NULL, NULL)->Show();
  else
    views::DialogDelegate::CreateDialogWidget(new ImportLockDialogView(callback,is_chrome), NULL, NULL)->Show();
}

ImportLockDialogView::ImportLockDialogView(
    const base::Callback<void(bool)>& callback)
    : description_label_(NULL),
      callback_(callback) {
  description_label_ = new views::Label(
      l10n_util::GetStringUTF16(IDS_IMPORTER_LOCK_TEXT));
  description_label_->SetMultiLine(true);
  description_label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  AddChildView(description_label_);
}

// champion utpal
ImportLockDialogView::ImportLockDialogView(
    const base::Callback<void(bool)>& callback,bool is_chrome=false)
    : description_label_(NULL),
      callback_(callback) {
  if(!is_chrome) {
    description_label_ = new views::Label(l10n_util::GetStringUTF16(IDS_IMPORTER_LOCK_TEXT));
    description_label_->SetMultiLine(true);
    description_label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    AddChildView(description_label_);
  } else {
	description_label_ = new views::Label(l10n_util::GetStringUTF16(IDS_IMPORTER_CHROME_LOCK_TEXT));
	description_label_->SetMultiLine(true);
	description_label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
	AddChildView(description_label_);
  }
}

ImportLockDialogView::~ImportLockDialogView() {
}

gfx::Size ImportLockDialogView::GetPreferredSize() const {
  return gfx::Size(views::Widget::GetLocalizedContentsSize(
      IDS_IMPORTLOCK_DIALOG_WIDTH_CHARS,
      IDS_IMPORTLOCK_DIALOG_HEIGHT_LINES));
}

void ImportLockDialogView::Layout() {
  gfx::Rect bounds(GetLocalBounds());
  bounds.Inset(views::kButtonHEdgeMargin, views::kPanelVertMargin);
  description_label_->SetBoundsRect(bounds);
}

base::string16 ImportLockDialogView::GetDialogButtonLabel(
    ui::DialogButton button) const {
  return l10n_util::GetStringUTF16((button == ui::DIALOG_BUTTON_OK) ?
      IDS_IMPORTER_LOCK_OK : IDS_IMPORTER_LOCK_CANCEL);
}

base::string16 ImportLockDialogView::GetWindowTitle() const {
  //champion- Import bookmark title -  Dinesh
  if(checkbool){
    return l10n_util::GetStringUTF16(IDS_IMPORTER_LOCK_TITLEFORCHROME);
  } else
    return l10n_util::GetStringUTF16(IDS_IMPORTER_LOCK_TITLE);
}

bool ImportLockDialogView::Accept() {
  base::MessageLoop::current()->PostTask(FROM_HERE,
                                         base::Bind(callback_, true));
  return true;
}

bool ImportLockDialogView::Cancel() {
  base::MessageLoop::current()->PostTask(FROM_HERE,
                                         base::Bind(callback_, false));
  return true;
}
