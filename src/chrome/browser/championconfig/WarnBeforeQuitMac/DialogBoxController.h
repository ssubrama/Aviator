// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import <Cocoa/Cocoa.h>

extern int iQuitRadioSelectedTag;

@interface DialogBoxController : NSWindowController
	<NSWindowDelegate>
{
	// outlets
	IBOutlet NSTextField*	textField ;
	IBOutlet NSImageView*	imageView ;
	IBOutlet NSTextField*	imageField;
  IBOutlet NSMatrix *nsmatQuitRadio;
  IBOutlet NSButtonCell *nsbcellQuitRadio_Yes;
  IBOutlet NSButtonCell *nsbcellQuitRadio_No;
   	
	// properties
//	NSNotificationCenter*	nc		  ;
	NSString*				imagePath ;
}
// actions
- (IBAction) closeButton:(id)sender;
//- (IBAction) imageFile:(id)sender;
- (IBAction) quitRadioAction:(id)sender;


// property getters/setters

- (void) setImagePath:(NSString*)aImagePath;

@end
