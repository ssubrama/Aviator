// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/RestartUpdate/RestartforUpdate.h"
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

#include "chrome/installer/util/product.h"
#include <iostream>  
#include <fstream> 

using namespace base;

#define LOCKFILENAME "rb.lock"
#define WINDOWVERSIONXP "5.1" 

static gfx::NativeWindow gptrNativeWnd;
static gfx::NativeWindow gptrParentchild;
static Browser* Rbrowser_ = NULL;
static bool* checkStockCloseType;

namespace views {

///////////////////////////////////////////////////////////////////////////////////// champion Update Mech window 

UpdateMechwindow::UpdateMechwindow(Operation operation)
      : window_shown_(new View),
        status_label_(new Label),
        operation_(operation),
        ok_button_(NULL),
        cancel_button_(NULL),
        disable_chk_button_(NULL),
        radiobutton(NULL) {
     
  instanceU_=this;
  ok_button_ = new views::LabelButton(this, ASCIIToUTF16("Dismiss"));
   ok_button_->SetStyle(views::Button::STYLE_BUTTON);
  //ok_button_->SetSize(gfx::Size(200,50));
  cancel_button_ = new views::LabelButton(this, ASCIIToUTF16("Restart Aviator Now"));
   cancel_button_->SetStyle(views::Button::STYLE_BUTTON);
  // disable_chk_button_->set_listener(this);
  status_label_ = new Label(ASCIIToUTF16(""));
  quit_label_ = new Label(ASCIIToUTF16("Restart Aviator Now?"));

  // you will lose your tabs,cokkies and cache,you can change this option later in settings."));
  Text_label_= new Label(ASCIIToUTF16("Aviator has automatically updated itself to the Latest"));
  Text_label1_= new Label(ASCIIToUTF16("version. Please restart the browser for the changes to"));
  Text_label2_= new Label(ASCIIToUTF16("take effect. Remember to complete any downloads in"));
  Text_label3_= new Label(ASCIIToUTF16("progress save any information you need before restarting."));
}

UpdateMechwindow::~UpdateMechwindow() {}

// Prints a message in the status area, at the bottom of the window.
void UpdateMechwindow::SetStatus(const std::string& status) {
  status_label_->SetText(UTF8ToUTF16(status));
}

void UpdateMechwindow::WindowClosing() {
  instanceU()->GetWidget()->Close();
  instanceU_ = NULL;
}

void UpdateMechwindow :: closefunction() {
  instanceU_ = NULL;
  if (operation_ == QUIT_ON_CLOSE)
    MessageLoopForUI::current()->Quit();
}

// Overridden from View:
void UpdateMechwindow::ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) {
  if (details.is_add && details.child == this)
    InitWindow();
}

int UpdateMechwindow::launchBrowserFromBrowser(){
	char* aviatorUpdateLocation="SOFTWARE\\Wow6432Node\\WhiteHat\\Update"; 
	DWORD dwType;
	//bool isAdmin = true;
    char  szVersion[255];
    DWORD dwDataSize = 255;
    memset(szVersion, 0, 255);
    HKEY hkeyDXVer;
	long lResult2 = RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);
	if (ERROR_SUCCESS != lResult2){
		aviatorUpdateLocation="SOFTWARE\\WhiteHat\\Update"; 
		if(ERROR_SUCCESS!=RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer)){
			//isAdmin = false;
		}
	}
	if (RegOpenKeyExA(HKEY_CURRENT_USER, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer)!=ERROR_SUCCESS){
		RegOpenKeyExA(HKEY_LOCAL_MACHINE, aviatorUpdateLocation, 0, KEY_READ, &hkeyDXVer);
	}
	if (RegQueryValueExA(hkeyDXVer, "path", NULL, &dwType, (BYTE*)szVersion, &dwDataSize)!=ERROR_SUCCESS){
			return -1;
	}

	//cout<<"szVersion is "<<szVersion<<endl;
	std::string ver(szVersion);
	unsigned pos = ver.find("Update");        
	std::string localUpdate = ver.substr(0,pos); 
	localUpdate.append("Application\\Aviator.exe");

	std::string launchCommand;

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
    if (atof(WINDOWVERSIONXP)== atof(version.c_str())){
      launchCommand.append("cmd.exe /c ping 1.1.1.1 -n 1 -w 20000 > nul & \"");
    }
    else {
      launchCommand.append("cmd.exe /c timeout 10 & \"");
    }

	launchCommand.append(localUpdate);
	launchCommand.append("\"");

	std::wstring wUpdateCommand ;
	wUpdateCommand.assign(launchCommand.begin(), launchCommand.end());

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	HANDLE hprocessorig = OpenProcess(SYNCHRONIZE, true, GetCurrentProcessId()); 
	//cin.ignore();
	BOOL result = CreateProcess(NULL,(LPWSTR)wUpdateCommand.c_str(), 
							   NULL,
                               NULL, FALSE, 
                               CREATE_NO_WINDOW,//NULL, 
                               NULL, NULL, &si,&pi);
	if (!result){
			return -1;
		}
	CloseHandle(hprocessorig);
	return 0;
}
void UpdateMechwindow::ButtonPressed(Button* sender, const ui::Event& event) {
  Profile* profile = ProfileManager::GetLastUsedProfile();
  PrefService* prefs =profile->GetPrefs();

  if(sender==ok_button_){
    UpdateMechwindow::dismiss=true;
    instanceU()->GetWidget()->Close();
  } else if (sender == cancel_button_){

    launchBrowserFromBrowser();

    BrowserView* browser_view_ = BrowserView::GetBrowserViewForBrowser(Rbrowser_);
    WarnBeforeQuit::closeAll = true;
    checkformanualrestart=true;    
    prefs->SetBoolean(prefs::kRestartManual, true);//we will set  kRestartManual false 
    MessageLoopForUI::current()->Quit();
    instanceU()->GetWidget()->Close();
  }
}

// launch without prompting users from about page (Amresh)
void UpdateMechwindow::LaunchWithoutPrompt() {
  Profile* profile = ProfileManager::GetLastUsedProfile();
  PrefService* prefs = profile->GetPrefs();

  launchBrowserFromBrowser();
  WarnBeforeQuit::closeAll = true;
  prefs->SetBoolean(prefs::kRestartManual, true);//we will set  kRestartManual false 
  MessageLoopForUI::current()->Quit();
 // Sleep(5000);

}

// Creates the layout within the window.
void UpdateMechwindow::InitWindow() {
  set_background(Background::CreateStandardPanelBackground());
  GridLayout* layout = new GridLayout(this);
  SetLayoutManager(layout);

  ColumnSet* column_set = layout->AddColumnSet(0);
  
  column_set->AddPaddingColumn(0, 120);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER,0,GridLayout::USE_PREF, 0, 0);
  
  layout->AddPaddingRow(0, 25);
  layout->StartRow(0, 0);
  
  quit_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  layout->AddView(quit_label_);
  layout->AddPaddingRow(0, 15);
  
  column_set = layout->AddColumnSet(1);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  layout->StartRow(0, 1);
  layout->AddView(Text_label_);
  column_set = layout->AddColumnSet(2);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  layout->StartRow(0, 2);
  layout->AddView(Text_label1_);
  
  column_set = layout->AddColumnSet(3);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  layout->StartRow(0, 3);
  layout->AddView(Text_label2_);
  
  column_set = layout->AddColumnSet(4);
  column_set->AddPaddingColumn(0, 20);
  column_set->AddColumn(GridLayout::FILL, GridLayout::CENTER, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 20);
  layout->StartRow(0, 4);
  layout->AddView(Text_label3_);

  column_set = layout->AddColumnSet(5);
  column_set->AddPaddingColumn(0, 60);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 30);
  layout->AddPaddingRow(0, 10);
  layout->StartRow(0, 5);
  layout->AddView(ok_button_);

  column_set->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,GridLayout::USE_PREF, 0 ,0 );
  layout->AddView(cancel_button_);
}
ui::ModalType UpdateMechwindow::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}
void UpdateMechwindow::NotifyClick(const ui::Event& event) {
  ::MessageBox(NULL, L"Hello", L"Caption", MB_OK  );
}
//static
UpdateMechwindow* UpdateMechwindow::instanceU_ = NULL;
bool UpdateMechwindow::dismiss=false;
bool UpdateMechwindow::checkformanualrestart=false;
static int ConsolePosX = 400;
static int ConsolePosY = 300;
void UpdateMechDialog(Operation operation,Browser* browser) {
  Rbrowser_=browser;
  gfx::NativeWindow parent = browser->window()->GetNativeWindow();
  if (UpdateMechwindow::instanceU())
    UpdateMechwindow::instanceU()->GetWidget()->Activate();
  else
  {
   
    gfx::Rect aviatorRect = parent->bounds();
    int xPos = aviatorRect.width() / 2;
    int yPos = aviatorRect.height() / 2;
    ConsolePosX = (xPos >= 400) ? xPos : ConsolePosX;
    ConsolePosY = (yPos >= 300) ? yPos : ConsolePosY;
    Widget::CreateWindowWithParentAndBounds(new UpdateMechwindow(operation), parent,
                                           gfx::Rect(ConsolePosX - 150, ConsolePosY - 115, 350, 200))->Show();
  }
}

}  // namespace views
