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

@class UnprotectedBubbleController;

// Controller for the Unprotected bubble.  
@interface UnprotectedBubbleController : BaseBubbleControllerAviator {
 @private
  

  IBOutlet NSTextField* bigTitle_;   // "Not Protected Mode"
  IBOutlet NSTextField* nameTextField_;
  // "This window is not protected. If you did not go into unprotected mode deliberately, manually clearing cookies and cache will minimize your risk."  

}

@property(nonatomic, retain) IBOutlet NSTextField* nameTextField_;

// The owner of this object is responsible for showing the bubble if
// it desires it to be visible on the screen.  It is not shown by the
// init routine.  Closing of the window happens implicitly on dealloc.
- (id)initWithParentWindow:(NSWindow*)parentWindow;

@end

