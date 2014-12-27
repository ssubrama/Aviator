// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import <Cocoa/Cocoa.h>
#include "chrome/browser/championconfig/Warn_before_quit/QuitDialog.h" // Champion Added for warn before quit

@class QuitDialog;

@interface DialogsDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
    QuitDialog* objQuitDialog;
   // NSWindow *window1;
}

@property (assign) IBOutlet NSWindow *window;
//@property (assign) IBOutlet NSWindow *window1;  

@end
