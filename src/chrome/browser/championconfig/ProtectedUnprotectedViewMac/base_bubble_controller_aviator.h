// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

@class InfoBubbleView;

// Base class for bubble controllers. Manages a xib that contains an
// InfoBubbleWindow which contains an InfoBubbleView. Contains code to close
// the bubble window on clicks outside of the window, and the like.
// To use this class:
// 1. Create a new xib that contains a window. Change the window's class to
//    InfoBubbleWindow. Give it a child view that autosizes to the window's full
//    size, give it class InfoBubbleView. Make the controller the window's
//    delegate.
// 2. Create a subclass of BaseBubbleController.
// 3. Change the xib's File Owner to your subclass.
// 4. Hook up the File Owner's |bubble_| to the InfoBubbleView in the xib.
@interface BaseBubbleControllerAviator : NSWindowController<NSWindowDelegate> {
 @private
  NSWindow* parentWindow_;  // weak
  NSPoint anchor_;
  IBOutlet InfoBubbleView* bubble_;  // to set arrow position
  // The controlled window should be the key window when it's opened. True by
  // default.
  bool shouldOpenAsKeyWindow_;
}

@property(nonatomic, readonly) NSWindow* parentWindow;
// The point in base screen coordinates at which the bubble should open and the
// arrow tip points.
@property(nonatomic, assign) NSPoint anchorPoint;
@property(nonatomic, readonly) InfoBubbleView* bubble;
@property(nonatomic, assign) bool shouldOpenAsKeyWindow;

// Creates a bubble. |nibPath| is just the basename, e.g. @"FirstRunBubble".
// |anchoredAt| is in screen space. You need to call -showWindow: to make the
// bubble visible. It will autorelease itself when the user dismisses the
// bubble.
// This is the designated initializer.
- (id)initWithWindowNibPath:(NSString*)nibPath
               parentWindow:(NSWindow*)parentWindow
                 anchoredAt:(NSPoint)anchoredAt;

@end

