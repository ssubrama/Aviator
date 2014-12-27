// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/warn_before_quit/WarnBeforeQuit.h"
#include <string>
#include "base/memory/scoped_vector.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/base/ui_base_paths.h"
#include "ui/views/test/child_modal_window.cc"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/radio_button.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"
#include <Windows.h>
#include <CommCtrl.h>



static HWND ghParent =  NULL;
static HWND ghParentchild =  NULL;

namespace views {
namespace examples {

WarnBeforeQuit::WarnBeforeQuit(Operation operation,ui::ModalType modaltype)
      : example_shown_(new View),
       modal_type_(modaltype), 	
	  status_label_(new Label),
        operation_(operation),
        ok_button_(NULL),
        cancel_button_(NULL),
		ExampleBase("Checkbox"),count_(0)
		
        //disable_chk_button_(NULL) 
{
     
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
  /*  instance_ = NULL;
    if (operation_ == QUIT_ON_CLOSE)
      base::MessageLoopForUI::current()->Quit();*/
	//if(!window()->IsMaximized())
			//window()->Maximize();
	instance()->GetWidget()->Close();
	instance_ = NULL;
		
  }
// champion modal window - Dinesh
void WarnBeforeQuit :: closefunction()
{
	//delete instance();
}
ui::ModalType WarnBeforeQuit::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}

bool WarnBeforeQuit::IsClosed() const {
  return true;
}


// Overridden from View:
void WarnBeforeQuit::ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) {
    if (details.is_add && details.child == this)
      InitExamplesWindow();
}
//NativeAppWindow* WarnBeforeQuit::GetBaseWindow() {
//  return native_app_window_.get();
//}
void WarnBeforeQuit::ButtonPressed(Button* sender, const ui::Event& event) {
	
	Profile* profile = ProfileManager::GetLastUsedProfile();
    PrefService* prefs =profile->GetPrefs();
	
	if(sender!=ok_button_ && sender!=cancel_button_){
		Warnbeforequit2ndWindow(QUIT_ON_CLOSE);
			// open the second dialog 
	}
	else if(sender==ok_button_){
		WarnBeforeQuit::bQuitAviator=true;

		if(WarnBeforeQuit::checked_)	
		   prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,false);
		else
     	    prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,true);
		// update the pref value and close the browser
	  instance()->GetWidget()->Close();		  
		//::SendMessage(ghParent,WM_QUIT,0,0);
	//	instance_ = NULL;
	//	if (operation_ == QUIT_ON_CLOSE)
		base::MessageLoopForUI::current()->Quit();
	
	}
	else if(sender==cancel_button_)
	{
		// dont close the browser
		WarnBeforeQuit::bQuitAviator=false;

		if(WarnBeforeQuit::checked_)	
		   prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,false);
		else
     	    prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,true);
	
		
		//instance()->GetWidget()->CloseAllSecondaryWidgets();
		instance()->GetWidget()->Close();	
		//WindowClosing();
	//	base::MessageLoopForUI::current()->Run();
		//GetBaseWindow();
	    //instance_ = NULL;
		//Browser* browser_;
		//browser_->initial_show_state();
		//ui::BaseWindow* parent_window;
		//gfx::NativeWindow parent =
		/*if(!window()->IsMaximized())
			window()->Maximize();*/
	
	}


}
void WarnBeforeQuit::NotifyClick(const ui::Event& event)
{

	//ShowExamples2ndWindow(QUIT_ON_CLOSE);

}
	// Creates the layout within the examples window.
void WarnBeforeQuit::InitExamplesWindow() {
	set_background(Background::CreateStandardPanelBackground());
	GridLayout* layout = new GridLayout(this);
	SetLayoutManager(layout);

	ColumnSet* column_set = layout->AddColumnSet(0);
	
	column_set->AddPaddingColumn(0, 80);
	
	column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 1,GridLayout::USE_PREF, 0, 0);
	
	//column_set->AddPaddingColumn(0, 5);
	
	layout->AddPaddingRow(0, 25);
	
	layout->StartRow(0 /* no expand */, 0);
	
	quit_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
	quit_label_->SetEnabledColor(SK_ColorDKGRAY);
	layout->AddView(quit_label_);
	layout->AddPaddingRow(0, 15);
	// checkbox
	column_set = layout->AddColumnSet(1);
	column_set->AddPaddingColumn(0, 50);
	column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 1,GridLayout::USE_PREF, 0, 0);
	layout->StartRow(0 /* no expand */, 1);
	layout->AddView(disable_chk_button_);
	
	column_set = layout->AddColumnSet(2);
	column_set->AddPaddingColumn(0, 80);
	column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0, 0);
	
	layout->AddPaddingRow(0, 10);
	layout->StartRow(0 /* no expand */, 2);
		
	layout->AddView(ok_button_);

//	column_set->AddPaddingColumn(0,10);
	column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0 ,0 );
	layout->AddView(cancel_button_);
	

}

// static
WarnBeforeQuit* WarnBeforeQuit::instance_ = NULL;
Checkbox* WarnBeforeQuit::disable_chk_button_=NULL;
bool WarnBeforeQuit::bQuitAviator=false;
bool WarnBeforeQuit::checked_=false;
//void WarnBeforeQuit :: checkfunction()
//{
//	WarnBeforeQuit::disable_chk_button_->SetChecked(!true);
//
//}
bool Warnbeforequit(Operation operation, HWND hDlgParent /*,BrowserCommandController* parentObj */) {
	ghParent = hDlgParent;
	//Parent = parentObj;
  if (WarnBeforeQuit::instance()) {
    WarnBeforeQuit::instance()->GetWidget()->Activate();
	 
  } else {
		 // RECT dlgRect;
		  RECT rectScreen;
		  int ConsolePosX = 500;
		  int ConsolePosY = 300;
		  HWND hwndScreen=GetDesktopWindow ();
		  if (hwndScreen) {
			  GetWindowRect(hwndScreen,&rectScreen); 
			 // GetWindowRect (hDlg, &dlgRect);

			  ConsolePosX = ((rectScreen.right-rectScreen.left)/2 -150);
			  ConsolePosY = ((rectScreen.bottom-rectScreen.top)/2- 100 ); 
		  }

		//  MoveWindow(hDlg, ConsolePosX, ConsolePosY, dlgRect.right - dlgRect.left, dlgRect.bottom - dlgRect.top, true);
	  	  Widget::CreateWindowWithParentAndBounds(new WarnBeforeQuit(operation, ui::MODAL_TYPE_CHILD), (gfx::NativeWindow)hDlgParent,
                                   gfx::Rect(ConsolePosX,ConsolePosY, 300, 200))->Show();
  //  Widget::CreateWindowWithBounds(new WarnBeforeQuit(operation,MODAL_TYPE_WINDOW),gfx::Rect(0, 0, 300, 180))->Show();
 //ChildModalParent* obj=new ChildModalParent();
 //obj->ShowChild();
	
}
  return WarnBeforeQuit::bQuitAviator;
}

void LogStatus(const std::string& string) {
  WarnBeforeQuit::instance()->SetStatus(string);
}

//champion second window 

WarnBeforeQuit2::WarnBeforeQuit2(Operation operation)
      : example_shown_(new View),
        status_label_(new Label),
        operation_(operation),
        ok_button_(NULL),
        cancel_button_(NULL),
		ExampleBase("RadioButton"),
        disable_chk_button_(NULL),
		radiobutton(NULL) {
     
    instance1_ = this;
	radiobutton = new views::RadioButton(ASCIIToUTF16("Yes, Diasble this Warning"),0);
	radiobutton->set_listener(this);
	//radiobutton->SetEnabled(true);


	radiobutton1 = new views::RadioButton(ASCIIToUTF16("No, Keep the Warning"),0);
	radiobutton1->set_listener(this);

	ok_button_ = new views::LabelButton(this, ASCIIToUTF16("Ok"));
	ok_button_->SetStyle(views::Button::STYLE_BUTTON);

	// disable_chk_button_->set_listener(this);
	status_label_ = new Label(ASCIIToUTF16(""));
	quit_label_ = new Label(ASCIIToUTF16("Are you certain?"));
	Text_label_= new Label(ASCIIToUTF16("By disabling the popup if you in advertently close Aviator "));//you will lose your tabs,cokkies and cache,you can change this option later in settings."));
	Text_label1_= new Label(ASCIIToUTF16("you will lose your tabs,cokkies and cache, you can change "));
	Text_label2_= new Label(ASCIIToUTF16("this option later in settings."));
}

WarnBeforeQuit2::~WarnBeforeQuit2() {}

  // Prints a message in the status area, at the bottom of the window.
void WarnBeforeQuit2::SetStatus(const std::string& status) {
	status_label_->SetText(UTF8ToUTF16(status));
}

void WarnBeforeQuit2::WindowClosing() {
    /*instance1_ = NULL;
    if (operation_ == QUIT_ON_CLOSE)
      base::MessageLoopForUI::current()->Quit();*/
	instance1()->GetWidget()->Close();
	instance1_ = NULL;
  

}
void WarnBeforeQuit2 :: closefunction()
{

	instance1_ = NULL;
    if (operation_ == QUIT_ON_CLOSE)
      base::MessageLoopForUI::current()->Quit();

}
// Overridden from View:
void WarnBeforeQuit2::ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) {
    if (details.is_add && details.child == this)
      InitExamplesWindow();
}

void WarnBeforeQuit2::ButtonPressed(Button* sender, const ui::Event& event) {
	if(sender==radiobutton){
		//WarnBeforeQuit::checkfunction();
		WarnBeforeQuit::checked_=true;
		WarnBeforeQuit::disable_chk_button_->SetChecked(WarnBeforeQuit::checked_);	
		// open the second dialog 
	}
	else if(sender==radiobutton1)
	{
		WarnBeforeQuit::checked_=false;
		WarnBeforeQuit::disable_chk_button_->SetChecked(WarnBeforeQuit::checked_);	
		// open the second dialog 
	
	}
	else if(sender==ok_button_)
	{
		instance1()->GetWidget()->Close();
	}

}

  // Creates the layout within the examples window.
void WarnBeforeQuit2::InitExamplesWindow() {
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
	
	//column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);

	//column_set->AddPaddingColumn(0, 20);

	//column_set->AddPaddingColumn(0, 5);
	
	layout->AddPaddingRow(0, 25);
	
	layout->StartRow(0 /* no expand */, 0);
	
	quit_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
	quit_label_->SetEnabledColor(SK_ColorDKGRAY);
	layout->AddView(quit_label_);
	layout->AddPaddingRow(0, 15);
	// checkbox
	layout->StartRow(0 /* no expand */, 1);
	Text_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
	Text_label_->SetEnabledColor(SK_ColorDKGRAY);
	layout->AddView(Text_label_);
	//layout->AddPaddingRow(0, 8);
	layout->StartRow(0 /* no expand */, 2);
	Text_label1_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
	Text_label1_->SetEnabledColor(SK_ColorDKGRAY);
	layout->AddView(Text_label1_);

	layout->StartRow(0 /* no expand */, 3);
	Text_label2_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
	Text_label2_->SetEnabledColor(SK_ColorDKGRAY);
	layout->AddView(Text_label2_);

	layout->AddPaddingRow(0, 10);
	layout->StartRow(0 /* no expand */, 4);
	layout->AddView(radiobutton);
	layout->StartRow(0 /* no expand */, 5);
	layout->AddView(radiobutton1);

	layout->AddPaddingRow(0, 10);
	layout->StartRow(0 /* no expand */, 6);
	layout->AddView(ok_button_);

//	column_set->AddPaddingColumn(0,10);
	
	

}
ui::ModalType WarnBeforeQuit2::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}

void WarnBeforeQuit2::NotifyClick(const ui::Event& event)
{
	 ::MessageBox(NULL, L"Hello", L"Caption", MB_OKCANCEL);
	//disable_chk_button_->SetChecked(false);

}
// static
WarnBeforeQuit2* WarnBeforeQuit2::instance1_ = NULL;

void Warnbeforequit2ndWindow(Operation operation) {
	ghParentchild = (HWND)WarnBeforeQuit::instance()->GetWidget()->GetNativeWindow();
  if (WarnBeforeQuit2::instance1()) {
    WarnBeforeQuit2::instance1()->GetWidget()->Activate();
  } else {

	  	  RECT rectScreen;
		  int ConsolePosX = 500;
		  int ConsolePosY = 300;
		  HWND hwndScreen=GetDesktopWindow ();
		  if (hwndScreen) {
			  GetWindowRect(hwndScreen,&rectScreen); 
			 // GetWindowRect (hDlg, &dlgRect);

			  ConsolePosX = ((rectScreen.right-rectScreen.left)/2 -150);
			  ConsolePosY = ((rectScreen.bottom-rectScreen.top)/2- 100 ); 
		  }

    Widget::CreateWindowWithParentAndBounds(new WarnBeforeQuit2(operation),(gfx::NativeWindow)ghParentchild,
                                   gfx::Rect(ConsolePosX, ConsolePosY, 300, 230))->Show();
  }
}




}  // namespace examples
}  // namespace views
