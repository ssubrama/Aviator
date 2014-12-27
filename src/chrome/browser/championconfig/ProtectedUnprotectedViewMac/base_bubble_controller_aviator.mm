// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#import "chrome/browser/championconfig/ProtectedUnprotectedViewMac/base_bubble_controller_aviator.h"

#include "base/logging.h"
#include "base/mac/bundle_locations.h"
#include "base/mac/mac_util.h"
#include "base/mac/scoped_nsobject.h"
#include "base/strings/string_util.h"
#import "chrome/browser/ui/cocoa/browser_window_controller.h"
#import "chrome/browser/ui/cocoa/info_bubble_view.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"

@implementation BaseBubbleControllerAviator

@synthesize parentWindow = parentWindow_;
@synthesize anchorPoint = anchor_;
@synthesize bubble = bubble_;
@synthesize shouldOpenAsKeyWindow = shouldOpenAsKeyWindow_;

- (id)initWithWindowNibPath:(NSString*)nibPath
               parentWindow:(NSWindow*)parentWindow
                 anchoredAt:(NSPoint)anchoredAt {
  nibPath = [base::mac::FrameworkBundle() pathForResource:nibPath
                                                   ofType:@"nib"];
  if ((self = [super initWithWindowNibPath:nibPath owner:self])) {
    parentWindow_ = parentWindow;
    anchor_ = anchoredAt;
    shouldOpenAsKeyWindow_ = YES;
  
  }
  return self;
}

- (void)awakeFromNib {
  // Check all connections have been made in Interface Builder.
  DCHECK([self window]);
  DCHECK(bubble_);
  DCHECK_EQ(self, [[self window] delegate]);
  [bubble_ setArrowLocation:info_bubble::kTopRight];
}

- (void)dealloc {
   [super dealloc];
}


// We want this to be a child of a browser window.  addChildWindow:
// (called from this function) will bring the window on-screen;
// unfortunately, [NSWindowController showWindow:] will also bring it
// on-screen (but will cause unexpected changes to the window's
// position).  We cannot have an addChildWindow: and a subsequent
// showWindow:. Thus, we have our own version.
- (void)showWindow:(id)sender {
/*  NSWindow* window = [self window];  // Completes nib load.
  [self updateOriginFromAnchor];
  [parentWindow_ addChildWindow:window ordered:NSWindowAbove];
  if (shouldOpenAsKeyWindow_)
    [window makeKeyAndOrderFront:self];
  else
    [window orderFront:nil];
  [self registerKeyStateEventTap];*/
}

- (void)close {

  [[[self window] parentWindow] removeChildWindow:[self window]];
  [super close];
}

@end  // BaseBubbleControllerAviator
