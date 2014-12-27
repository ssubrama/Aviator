// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_
#define UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_
//#include "chrome/browser/extensions/api/app_current_window_internal/app_current_window_internal_api.h"
#include <string>
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/ui/browser.h"

#include "chrome/browser/ui/browser_window.h"
#include "ui/views/examples/views_examples_export.h"
#include "ui/views/examples/example_base.h"

#include "ui/views/controls/label.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "base/basictypes.h"

#include "base/compiler_specific.h"
#include "base/strings/stringprintf.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

//class BrowserCommandController;

namespace views {
class Checkbox;
class RadioButton;
//class BrowserWindow;
//class BaseWindow;
//class NativeAppWindow;
namespace examples {

enum Operation {
  DO_NOTHING_ON_CLOSE = 0,
  QUIT_ON_CLOSE,
};

//extern bool bQuitAviator;

class WarnBeforeQuit : public ExampleBase,
	                           public WidgetDelegateView,
							   public ButtonListener
							   							    {
 public:
  explicit WarnBeforeQuit(Operation operation,ui::ModalType modaltype);
      
  virtual ~WarnBeforeQuit();

  // Prints a message in the status area, at the bottom of the window.
  void SetStatus(const std::string& status);
  
  static WarnBeforeQuit* instance() { return instance_; }
  // void SetChecked(bool checked) ;
  int count_;
  ui::ModalType modal_type_;
  void SetChecked(bool checked) { checked_ = checked; }
  //bool checked() const { return checked_; }
 static void checkfunction();
 static  bool checked_;
 static Checkbox* disable_chk_button_;
 static bool bQuitAviator;
 void closefunction();
 bool IsClosed() const;
 //  NativeAppWindow* GetBaseWindow();
//  scoped_ptr<NativeAppWindow> native_app_window_; 
 // The BrowserFrame that hosts this view.

 // BrowserFrame* frame_;



  // The Browser object we are associated with.

  //scoped_ptr<Browser> browser_;
  //Browser* browser_;
//  Browser* browser() { return browser_.get(); }
 ui::BaseWindow* parent_window;
 ui::BaseWindow* window() const { return parent_window; }
 private:
  // Overridden from WidgetDelegateView:
  virtual bool CanResize() const OVERRIDE { return false; }
  virtual bool CanMaximize() const OVERRIDE { return false ; }
  virtual string16 GetWindowTitle() const OVERRIDE { return ASCIIToUTF16("Quit Aviator"); }
  virtual View* GetContentsView() OVERRIDE { return this; }
  virtual void WindowClosing() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual void NotifyClick(const ui::Event& event);
 
 //bool checked() const { return checked_; }
  // Overridden from View:
  virtual void ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) OVERRIDE ;
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  virtual void CreateExampleView(View* parent) {}
     // Creates the layout within the examples window.
  void InitExamplesWindow();

  static WarnBeforeQuit* instance_;
 
  View* example_shown_;
  
  Label* status_label_;
  Label* quit_label_;
  LabelButton* ok_button_;
  LabelButton* cancel_button_;
  
  views::ButtonListener* button_listener_;

  const Operation operation_;

  DISALLOW_COPY_AND_ASSIGN(WarnBeforeQuit);
};

//champion second window-dinesh

class WarnBeforeQuit2 : public ExampleBase, 
	                           public WidgetDelegateView,
							   public ButtonListener{
 public:
  WarnBeforeQuit2(Operation operation);
      
  virtual ~WarnBeforeQuit2();

  // Prints a message in the status area, at the bottom of the window.
  void SetStatus(const std::string& status);

  static WarnBeforeQuit2* instance1() { return instance1_; }
	 virtual void NotifyClick(const ui::Event& event);
	void closefunction();
	virtual ui::ModalType GetModalType() const OVERRIDE;
 private:
  // Overridden from WidgetDelegateView:
  virtual bool CanResize() const OVERRIDE { return false; }
  virtual bool CanMaximize() const OVERRIDE { return false; }
  virtual string16 GetWindowTitle() const OVERRIDE { return ASCIIToUTF16("Quit Aviator"); }
  virtual View* GetContentsView() OVERRIDE { return this; }
  virtual void WindowClosing() OVERRIDE;

  // Overridden from View:
  virtual void ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) OVERRIDE ;
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  virtual void CreateExampleView(View* parent) {}

  

  // Creates the layout within the examples window.
  void InitExamplesWindow();

  static WarnBeforeQuit2* instance1_;
 
  View* example_shown_;
  
  Label* status_label_;
  Label* quit_label_;
  Label* Text_label_; 
  Label* Text_label1_; 
  Label* Text_label2_;
  LabelButton* ok_button_;
  LabelButton* cancel_button_;
  Checkbox* disable_chk_button_;
   views::RadioButton* radiobutton;	
  views::RadioButton* radiobutton1; 
  views::ButtonListener* button_listener_;

  const Operation operation_;

  DISALLOW_COPY_AND_ASSIGN(WarnBeforeQuit2);
};


// Shows a window with the views examples in it.
bool Warnbeforequit(Operation operation, HWND hDlgParent/*, chrome::BrowserCommandController* const parentObj*/);
void Warnbeforequit2ndWindow(Operation operation);

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_
