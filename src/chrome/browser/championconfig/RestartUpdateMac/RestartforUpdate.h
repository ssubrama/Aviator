// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_CHAMPIONCONFIG_RESTARTUPDATE_RESTARTUPDATE_H_
#define CHROME_BROWSER_CHAMPIONCONFIG_RESTARTUPDATE_RESTARTUPDATE_H_

#include "base/strings/utf_string_conversions.h"
#include "ui/views/controls/button/label_button.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/championconfig/aviatordatatypes.h"

using namespace ADT;

namespace views {
class Checkbox;
class RadioButton;
class NativeAppWindow;

class UpdateMechwindow : public WidgetDelegateView,
                         public ButtonListener {

public:
  UpdateMechwindow(Operation operation);
  virtual ~UpdateMechwindow();
  // Prints a message in the status area, at the bottom of the window.
  void SetStatus(const std::string& status);
  static UpdateMechwindow* instanceU() { return instanceU_; }
  virtual void NotifyClick(const ui::Event& event);
  void closefunction();
  static bool dismiss;
  static bool checkformanualrestart;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  int launchBrowserFromBrowser();
 private:
  // Overridden from WidgetDelegateView:
  virtual bool CanResize() const OVERRIDE { return false; }
  virtual bool CanMaximize() const OVERRIDE { return false; }
  virtual base::string16 GetWindowTitle() const OVERRIDE { return base::ASCIIToUTF16("Restart Aviator"); }
  virtual View* GetContentsView() OVERRIDE { return this; }
  virtual void WindowClosing() OVERRIDE;

  // Overridden from View:
  virtual void ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) OVERRIDE ;
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  // Creates the layout within the window.
  void InitWindow();

  static UpdateMechwindow* instanceU_;
 
  View* window_shown_;
  Label* status_label_;
  Label* quit_label_;
  Label* Text_label_; 
  Label* Text_label1_; 
  Label* Text_label2_;
  Label* Text_label3_;
  LabelButton* ok_button_;
  LabelButton* cancel_button_;
  Checkbox* disable_chk_button_;
  views::RadioButton* radiobutton;
  views::RadioButton* radiobutton1; 
  views::ButtonListener* button_listener_;

  const Operation operation_;

  DISALLOW_COPY_AND_ASSIGN(UpdateMechwindow);
};

// Shows a window with the views in it.
void UpdateMechDialog(Operation operation,Browser* browser_);

}  // namespace views

#endif  // CHROME_BROWSER_CHAMPIONCONFIG_RESTARTUPDATE_RESTARTUPDATE_H_
