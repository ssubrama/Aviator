// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import "chrome/browser/championconfig/ProtectedUnprotectedViewMac/protected_decoration.h"

#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/command_updater.h"
#import "chrome/browser/ui/cocoa/omnibox/omnibox_view_mac.h"
#include "grit/generated_resources.h"
#include "grit/theme_resources.h"
#include "ui/base/l10n/l10n_util_mac.h"

//

#import "chrome/browser/ui/cocoa/browser_window_controller.h"

#import "base/mac/mac_util.h"
#include "base/run_loop.h"
#include "base/strings/utf_string_conversions.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_window.h"
#import "chrome/browser/ui/cocoa/profiles/avatar_button_controller.h"
#include "chrome/browser/ui/cocoa/browser_window_cocoa.h"
#import "chrome/browser/ui/cocoa/browser_window_controller_private.h"
#import "chrome/browser/ui/cocoa/fast_resize_view.h"
#import "chrome/browser/ui/cocoa/infobars/infobar_container_controller.h"
#import "chrome/browser/ui/cocoa/nsview_additions.h"
#include "chrome/browser/ui/search/search_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/browser/web_contents.h"
#import "testing/gtest_mac.h"

//

namespace {

// The info-bubble point should look like it points to the point
// between the star's lower tips.  The popup should be where the
// Omnibox popup ends up (2px below field).  Determined via Pixie.app
// magnification.
const CGFloat kStarPointYOffset = 2.0;

}  // namespace
//- (void)mouseMoved:(NSEvent*)event ;


ProtectedDecoration::ProtectedDecoration(CommandUpdater* command_updater)
    : command_updater_(command_updater) {
  SetVisible(true);
  SetStarred(false);
}

ProtectedDecoration::ProtectedDecoration(Browser* browser)
    : browser_(browser) {
  SetVisible(true);
  SetStarred(false);
}


ProtectedDecoration::~ProtectedDecoration() {
}

void ProtectedDecoration::SetStarred(bool starred) {
  starred_ = starred;
  const int image_id = IDR_PROTECTED;//starred ? IDR_STAR_LIT : IDR_STAR;
//  const int tip_id = IDS_TOOLTIP_PROTECTED;
  SetImage(OmniboxViewMac::ImageForResource(image_id));
 // tooltip_.reset([l10n_util::GetNSStringWithFixup(tip_id) retain]);
}

NSPoint ProtectedDecoration::GetBubblePointInFrame(NSRect frame) {
  const NSRect draw_frame = GetDrawRectInFrame(frame);
  return NSMakePoint(NSMidX(draw_frame),(NSMaxY(draw_frame) - kStarPointYOffset));
}

bool ProtectedDecoration::AcceptsMousePress() {
	return true;
}

bool ProtectedDecoration::OnMousePressed(NSRect frame,NSPoint location) {
                                                      
  return true;
}


bool ProtectedDecoration::OnMouseMoved(NSRect frame)// champion_Protected
{
if(!isShowProtectedBubble)
{
  BrowserWindowController* controller_ = [BrowserWindowController browserWindowControllerForWindow:browser_->window()->GetNativeWindow()];
  isShowProtectedBubble = true;
  [controller_ showProtectedBubble];  
  controller_ = nil; 
 }
 return false;
}

NSString* ProtectedDecoration::GetToolTip() {
  return tooltip_.get();
}
