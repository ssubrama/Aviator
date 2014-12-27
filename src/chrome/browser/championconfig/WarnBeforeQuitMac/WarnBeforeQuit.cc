// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/warnbeforequit/WarnBeforeQuit.h"

#include "ui/aura/window.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/radio_button.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"
#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"
#include "chrome/browser/lifetime/application_lifetime.h"

using namespace base;

static gfx::NativeWindow gptrNativeWnd;
static gfx::NativeWindow gptrParentchild;
static Browser* browser_ = NULL;
static bool* checkStockCloseType;

namespace views {

WarnBeforeQuit::WarnBeforeQuit(Operation operation,ui::ModalType modaltype)
      : window_shown_(new View),
        modal_type_(modaltype),   
        status_label_(new Label),
        operation_(operation),
        ok_button_(NULL),
        cancel_button_(NULL),
        count_(0) {
     
  instance_ = this;

  disable_chk_button_ = new Checkbox(ASCIIToUTF16("Disable this warning in future."));
  disable_chk_button_->set_listener(this);

  ok_button_ = new views::LabelButton(this, ASCIIToUTF16("Ok"));
  ok_button_->SetStyle(views::Button::STYLE_BUTTON);

  status_label_ = new Label(ASCIIToUTF16(""));

  quit_label_ = new Label(ASCIIToUTF16("Do you really want to quit?"));
  cancel_button_ = new views::LabelButton(this, ASCIIToUTF16("Cancel"));
  cancel_button_->SetStyle(views::Button::STYLE_BUTTON);
}

WarnBeforeQuit::~WarnBeforeQuit() {}

// Prints a message in the status area, at the bottom of the window.
void WarnBeforeQuit::SetStatus(const std::string& status) {
  status_label_->SetText(UTF8ToUTF16(status));
}

void WarnBeforeQuit::WindowClosing() {
  /*if (browser_ && browser_->IsCloseAllBrowser())
    WarnBeforeQuit::closeAll = true;*/
  instance()->GetWidget()->Close();
  instance_ = NULL;
}

// champion modal window - Dinesh
void WarnBeforeQuit :: closefunction() {}

ui::ModalType WarnBeforeQuit::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}

bool WarnBeforeQuit::IsClosed() const {
  return true;
}

// Overridden from View:
void WarnBeforeQuit::ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) {
  if (details.is_add && details.child == this)
    InitWindow();

}

void WarnBeforeQuit::ButtonPressed(Button* sender, const ui::Event& event) {
  Profile* profile = ProfileManager::GetLastUsedProfile();
  PrefService* prefs =profile->GetPrefs();
  
  if(sender!=ok_button_ && sender != cancel_button_) {
    // open the second dialog 
    Warnbeforequit2ndWindow(QUIT_ON_CLOSE);
    } else if(sender==ok_button_) {
    WarnBeforeQuit::bQuitAviator = true;

    if(WarnBeforeQuit::checked_)  
      prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,false);
    else
      prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,true);
    instance()->GetWidget()->Close();

    if (WarnBeforeQuit::bQuitAviator && browser_) {
      BrowserView* browser_view_ = BrowserView::GetBrowserViewForBrowser(browser_);
      (*checkStockCloseType) = false;
      browser_view_->GetTabStipController()->CloseSingleTab(); 
      browser_view_->CanClose();
      WarnBeforeQuit::bQuitAviator = false;
    }
  } else if(sender ==   cancel_button_) {
    // dont close the browser
    WarnBeforeQuit::bQuitAviator = false;

    if(WarnBeforeQuit::checked_)  
      prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled, false);
    else
      prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled, true);
  
    (*checkStockCloseType) = true;
    instance()->GetWidget()->Close();  
  }
}

void WarnBeforeQuit::NotifyClick(const ui::Event& event) {}

// Creates the layout within the window.
void WarnBeforeQuit::InitWindow() {
  set_background(Background::CreateStandardPanelBackground());
  GridLayout* layout = new GridLayout(this);

  SetLayoutManager(layout);

  ColumnSet* column_set = layout->AddColumnSet(0);
  
  column_set->AddPaddingColumn(0, 80);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 1,GridLayout::USE_PREF, 0, 0);
  
  layout->AddPaddingRow(0, 25);
  layout->StartRow(0, 0);
  
  quit_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  quit_label_->SetEnabledColor(SK_ColorDKGRAY);

  layout->AddView(quit_label_);
  layout->AddPaddingRow(0, 15);
  
  column_set = layout->AddColumnSet(1);
  column_set->AddPaddingColumn(0, 50);
  column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 1,GridLayout::USE_PREF, 0, 0);

  layout->StartRow(0, 1);
  layout->AddView(disable_chk_button_);
  
  column_set = layout->AddColumnSet(2);
  column_set->AddPaddingColumn(0, 80);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0, 0);
  
  layout->AddPaddingRow(0, 10);
  layout->StartRow(0, 2);
  layout->AddView(ok_button_);

  column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0 ,0 );
  layout->AddView(cancel_button_);
}

// static
WarnBeforeQuit* WarnBeforeQuit::instance_ = NULL;
Checkbox* WarnBeforeQuit::disable_chk_button_= NULL;
bool WarnBeforeQuit::bQuitAviator = false;
bool WarnBeforeQuit::checked_ = false;
bool WarnBeforeQuit::checkclose = false;
bool WarnBeforeQuit::closeAll = false;

static int ConsolePosX = 400;
static int ConsolePosY = 300;

bool Warnbeforequit(Operation operation, gfx::NativeWindow ptrNativeWnd, Browser* browser, bool& checkStockCType) {
  browser_ = browser;
  checkStockCloseType = &checkStockCType;
  gptrNativeWnd = ptrNativeWnd;
  if (WarnBeforeQuit::instance()) {
    WarnBeforeQuit::instance()->GetWidget()->Activate();
    // WarnBeforeQuit::checkclose = false; 
  } else {
    gfx::Rect aviatorRect = ptrNativeWnd->bounds();
    int xPos = aviatorRect.width() / 2;
    int yPos = aviatorRect.height() / 2;
    ConsolePosX = (xPos >= 400) ? xPos : ConsolePosX;
    ConsolePosY = (yPos >= 300) ? yPos : ConsolePosY;
    Widget::CreateWindowWithParentAndBounds(new WarnBeforeQuit(operation, ui::MODAL_TYPE_CHILD), ptrNativeWnd,
                                            gfx::Rect(ConsolePosX - 150, ConsolePosY - 100, 300, 200))->Show();
  }
  return WarnBeforeQuit::bQuitAviator;
}

void LogStatus(const std::string& string) {
  WarnBeforeQuit::instance()->SetStatus(string);
}

///////////////////////////////////////////////////////////////////////////////////// champion second window 

WarnBeforeQuit2::WarnBeforeQuit2(Operation operation)
      : window_shown_(new View),
        status_label_(new Label),
        operation_(operation),
        ok_button_(NULL),
        cancel_button_(NULL),
        disable_chk_button_(NULL),
        radiobutton(NULL) {
     
  instance1_ = this;
  radiobutton = new views::RadioButton(ASCIIToUTF16("Yes, Disable this Warning"),0);
  radiobutton->set_listener(this);
  //radiobutton->SetEnabled(true);

  radiobutton1 = new views::RadioButton(ASCIIToUTF16("No, Keep the Warning"),0);
  radiobutton1->set_listener(this);

  ok_button_ = new views::LabelButton(this, ASCIIToUTF16("Ok"));
  ok_button_->SetStyle(views::Button::STYLE_BUTTON);

  // disable_chk_button_->set_listener(this);
  status_label_ = new Label(ASCIIToUTF16(""));
  quit_label_ = new Label(ASCIIToUTF16("Are you certain?"));

  // you will lose your tabs,cokkies and cache,you can change this option later in settings."));
  Text_label_= new Label(ASCIIToUTF16("By disabling the popup if you inadvertently close Aviator "));
  Text_label1_= new Label(ASCIIToUTF16("you will lose your tabs, cookies and cache. You can change "));
  Text_label2_= new Label(ASCIIToUTF16("this option later in Settings."));
}

WarnBeforeQuit2::~WarnBeforeQuit2() {}

// Prints a message in the status area, at the bottom of the window.
void WarnBeforeQuit2::SetStatus(const std::string& status) {
  status_label_->SetText(UTF8ToUTF16(status));
}

void WarnBeforeQuit2::WindowClosing() {
  instance1()->GetWidget()->Close();
  instance1_ = NULL;
}

void WarnBeforeQuit2 :: closefunction() {
  instance1_ = NULL;
  if (operation_ == QUIT_ON_CLOSE)
    MessageLoopForUI::current()->Quit();
}

// Overridden from View:
void WarnBeforeQuit2::ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) {
  if (details.is_add && details.child == this)
    InitWindow();
}

void WarnBeforeQuit2::ButtonPressed(Button* sender, const ui::Event& event) {
  if(sender==radiobutton){
    WarnBeforeQuit::checked_=true;
    WarnBeforeQuit::disable_chk_button_->SetChecked(WarnBeforeQuit::checked_);  
  } else if(sender==radiobutton1) {
    WarnBeforeQuit::checked_=false;
    WarnBeforeQuit::disable_chk_button_->SetChecked(WarnBeforeQuit::checked_);  
  } else if(sender==ok_button_) 
    instance1()->GetWidget()->Close();
}

// Creates the layout within the window.
void WarnBeforeQuit2::InitWindow() {
  set_background(Background::CreateStandardPanelBackground());
  GridLayout* layout = new GridLayout(this);
  SetLayoutManager(layout);

  ColumnSet* column_set = layout->AddColumnSet(0);

  column_set->AddPaddingColumn(0, 120);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  column_set = layout->AddColumnSet(1);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  column_set = layout->AddColumnSet(2);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  column_set = layout->AddColumnSet(3);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  column_set = layout->AddColumnSet(4);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  column_set = layout->AddColumnSet(5);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 140);
  column_set = layout->AddColumnSet(6);
  column_set->AddPaddingColumn(0, 140);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  
  layout->AddPaddingRow(0, 25);
  layout->StartRow(0, 0);
  
  quit_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  quit_label_->SetEnabledColor(SK_ColorDKGRAY);

  layout->AddView(quit_label_);
  layout->AddPaddingRow(0, 15);
  layout->StartRow(0, 1);
  
  Text_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  Text_label_->SetEnabledColor(SK_ColorDKGRAY);

  layout->AddView(Text_label_);
  layout->StartRow(0, 2);
  
  Text_label1_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  Text_label1_->SetEnabledColor(SK_ColorDKGRAY);
  
  layout->AddView(Text_label1_);
  layout->StartRow(0, 3);

  Text_label2_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  Text_label2_->SetEnabledColor(SK_ColorDKGRAY);
  
  layout->AddView(Text_label2_);
  layout->AddPaddingRow(0, 10);
  layout->StartRow(0, 4);
  layout->AddView(radiobutton);
  layout->StartRow(0, 5);
  layout->AddView(radiobutton1);
  layout->AddPaddingRow(0, 10);
  layout->StartRow(0, 6);
  layout->AddView(ok_button_);
 
}

ui::ModalType WarnBeforeQuit2::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}

void WarnBeforeQuit2::NotifyClick(const ui::Event& event) {
  ::MessageBox(NULL, L"Hello", L"Caption", MB_OK  );
}

// static
WarnBeforeQuit2* WarnBeforeQuit2::instance1_ = NULL;

void Warnbeforequit2ndWindow(Operation operation) {
  gptrParentchild = WarnBeforeQuit::instance()->GetWidget()->GetNativeWindow();

  if (WarnBeforeQuit2::instance1())
    WarnBeforeQuit2::instance1()->GetWidget()->Activate();
  else
    Widget::CreateWindowWithParentAndBounds(new WarnBeforeQuit2(operation),gptrParentchild,
                                            gfx::Rect(ConsolePosX - 150, ConsolePosY - 115, 300, 230))->Show();
}

}  // namespace views
