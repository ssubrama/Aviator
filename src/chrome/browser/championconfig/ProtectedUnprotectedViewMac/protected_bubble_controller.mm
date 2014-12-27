// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import "chrome/browser/championconfig/ProtectedUnprotectedViewMac/protected_bubble_controller.h"

#include "base/mac/bundle_locations.h"
#include "base/mac/mac_util.h"
#include "base/strings/sys_string_conversions.h"
#import "chrome/browser/ui/cocoa/browser_window_controller.h"
#import "chrome/browser/ui/cocoa/info_bubble_view.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util_mac.h"

@implementation ProtectedBubbleController
 
@synthesize nameTextField_=nameTextField_;

- (id)initWithParentWindow:(NSWindow*)parentWindow{
                   
  if ((self = [super initWithWindowNibPath:@"ProtectedBubble"
                              parentWindow:parentWindow
                                anchoredAt:NSZeroPoint])) {
    
   
  }
  return self;
}

- (void)awakeFromNib {
  [super awakeFromNib];
 }

// Override -[BaseBubbleControllerAviator showWindow:] to tweak bubble location and
// set up UI elements.
- (void)showWindow:(id)sender {
    
  NSWindow* window = [self window];  // Force load the NIB.
  NSWindow* parentWindow = self.parentWindow;
  BrowserWindowController* bwc =[BrowserWindowController browserWindowControllerForWindow:parentWindow];
  [bwc lockBarVisibilityForOwner:self withAnimation:NO delay:NO];

  InfoBubbleView* bubble = self.bubble;
  [bubble setArrowLocation:info_bubble::kTopRight];

  // Insure decent positioning even in the absence of a browser controller,
  // which will occur for some unit tests.
  NSPoint arrowTip = bwc ? [bwc protectedBubblePoint] : NSMakePoint([window frame].size.width, [window frame].size.height);
  arrowTip = [parentWindow convertBaseToScreen:arrowTip];
     
  NSPoint bubbleArrowTip = [bubble arrowTip];
  bubbleArrowTip = [bubble convertPoint:bubbleArrowTip toView:nil];
  arrowTip.y -= bubbleArrowTip.y;
  arrowTip.x -= bubbleArrowTip.x;
  arrowTip.y -=13;
  
  [window setFrameOrigin:arrowTip];
   
  NSString* title =l10n_util::GetNSString(IDS_PROTECTED_BUBBLE);
  [bigTitle_ setStringValue:title];
  
  NSString* info =l10n_util::GetNSString(IDS_PROTECTED_BUBBLE_TITLE_TEXT);
  [nameTextField_ setStringValue:info];
  
  [parentWindow addChildWindow:window ordered:NSWindowAbove];
  [window makeKeyAndOrderFront:self];
   
}

- (void)close {
  [[BrowserWindowController browserWindowControllerForWindow:self.parentWindow]
      releaseBarVisibilityForOwner:self withAnimation:YES delay:NO];

  [super close];
}

// The controller is the delegate of the window so it receives did resign key
// notifications. When key is resigned mirror Windows behavior and close the
// window.
- (void)windowDidResignKey:(NSNotification*)notification {
  NSWindow* window = [self window];
  DCHECK_EQ([notification object], window);
  if ([window isVisible]) {
    // If the window isn't visible, it is already closed, and this notification
    // has been sent as part of the closing operation, so no need to close.
    [self close];
  }
}

@end  // ProtectedBubbleController

