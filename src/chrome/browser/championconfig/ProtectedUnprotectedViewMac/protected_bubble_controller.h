// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/championconfig/ProtectedUnprotectedViewMac/base_bubble_controller_aviator.h"

@class ProtectedBubbleController;

// Controller for the bookmark bubble.  The bookmark bubble is a
// bubble that pops up when clicking on the STAR next to the URL to
// add or remove it as a bookmark.  This bubble allows for editing of
// the bookmark in various ways (name, folder, etc.)
@interface ProtectedBubbleController : BaseBubbleControllerAviator {
 @private
  

  IBOutlet NSTextField* bigTitle_;   // "Protected Mode"
  IBOutlet NSTextField* nameTextField_;
  // "This window is protected. If you did not go into Protected mode deliberately, manually clearing cookies and cache will minimize your risk."  

}

@property(nonatomic, retain) IBOutlet NSTextField* nameTextField_;

// |node| is the bookmark node we edit in this bubble.
// |alreadyBookmarked| tells us if the node was bookmarked before the
//   user clicked on the star.  (if NO, this is a brand new bookmark).
// The owner of this object is responsible for showing the bubble if
// it desires it to be visible on the screen.  It is not shown by the
// init routine.  Closing of the window happens implicitly on dealloc.
- (id)initWithParentWindow:(NSWindow*)parentWindow;
        
@end


