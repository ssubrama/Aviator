// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/extensions/extension_uninstall_dialog.h"

#import <Cocoa/Cocoa.h>

#include <string>

#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "extensions/common/extension.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/base/l10n/l10n_util_mac.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia_util_mac.h"

namespace {

// The Cocoa implementation of ExtensionUninstallDialog. This has a less
// complex life cycle than the Views and GTK implementations because the
// dialog blocks the page from navigating away and destroying the dialog,
// so there's no way for the dialog to outlive its delegate.
class ExtensionUninstallDialogCocoa
    : public extensions::ExtensionUninstallDialog {
 public:
  ExtensionUninstallDialogCocoa(
      Profile* profile,
      Browser* browser,
      Delegate* delegate);
  virtual ~ExtensionUninstallDialogCocoa() OVERRIDE;

 private:
  virtual void Show() OVERRIDE;
};

ExtensionUninstallDialogCocoa::ExtensionUninstallDialogCocoa(
    Profile* profile,
    Browser* browser,
    extensions::ExtensionUninstallDialog::Delegate* delegate)
    : extensions::ExtensionUninstallDialog(profile, browser, delegate) {
}

ExtensionUninstallDialogCocoa::~ExtensionUninstallDialogCocoa() {}

void ExtensionUninstallDialogCocoa::Show() {
//Added for champion plug_in bar	
	std::string name=extension_->name();
	std::string strPluginType="";
	if(name == "Disconnect")
	 strPluginType="security";
/*	else if (name == "PDF Viewer")
	 strPluginType="bundled";
	if(name == "PDF Viewer" || name == "Disconnect") */
	if(name == "Disconnect"){
  	  NSAlert* alert = [[[NSAlert alloc] init] autorelease];
	  NSButton* cancelButton = [alert addButtonWithTitle:l10n_util::GetNSString(IDS_CANCEL)];
	  [cancelButton setKeyEquivalent:@"\r"];
	  NSString* msg=l10n_util::GetNSStringF(IDS_EXTENSION_UNINSTALL_CANCEL_MESSAGE,base::UTF8ToUTF16(extension_->name()),base::UTF8ToUTF16(strPluginType));
	  [alert setMessageText:msg]; // champion Balaji
	  [alert setAlertStyle:NSCriticalAlertStyle];
	  [alert setIcon:gfx::NSImageFromImageSkia(icon_)];
	
	  if ([alert runModal] == NSAlertFirstButtonReturn)
	  	   delegate_->ExtensionUninstallCanceled();
	  else
	  	  delegate_->ExtensionUninstallCanceled();
	  	  /*cancel both the case*/	
	}
	else {  
  NSAlert* alert = [[[NSAlert alloc] init] autorelease];

  NSButton* continueButton = [alert addButtonWithTitle:l10n_util::GetNSString(
      IDS_EXTENSION_PROMPT_UNINSTALL_BUTTON)];
  NSButton* cancelButton = [alert addButtonWithTitle:l10n_util::GetNSString(
      IDS_CANCEL)];
  // Default to accept when triggered via aviator://extensions page.
  if (triggering_extension_) {
    [continueButton setKeyEquivalent:@""];
    [cancelButton setKeyEquivalent:@"\r"];
  }

  [alert setMessageText:base::SysUTF8ToNSString(GetHeadingText())];
  [alert setAlertStyle:NSWarningAlertStyle];
  [alert setIcon:gfx::NSImageFromImageSkia(icon_)];

  if ([alert runModal] == NSAlertFirstButtonReturn)
    delegate_->ExtensionUninstallAccepted();
  else
    delegate_->ExtensionUninstallCanceled();
}
}
}  // namespace

// static
extensions::ExtensionUninstallDialog*
extensions::ExtensionUninstallDialog::Create(Profile* profile,
                                             Browser* browser,
                                             Delegate* delegate) {
  return new ExtensionUninstallDialogCocoa(profile, browser, delegate);
}
