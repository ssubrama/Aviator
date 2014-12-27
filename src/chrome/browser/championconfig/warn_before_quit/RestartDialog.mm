// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
#import "chrome/browser/championconfig/Warn_before_quit/RestartDialog.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"

#include "chrome/browser/lifetime/application_lifetime.h" 

#if defined(OS_MACOSX) // Champion Restart Aviator for taking update (Balaji)
#include "chrome/browser/app_controller_mac.h"
#include "chrome/browser/policy/url_blacklist_manager.h"
#endif

@implementation RestartDialog

static RestartDialog* _sharedMySingleton = nil;
 
+(RestartDialog*)sharedMySingleton
{
	@synchronized([RestartDialog class])
	{
		if (!_sharedMySingleton)
			[[self alloc] init];
 
		return _sharedMySingleton;
	}
 
	return nil;
}
 
+(id)alloc
{
	@synchronized([RestartDialog class])
	{
		NSAssert(_sharedMySingleton == nil, @"Attempted to allocate a second instance of a singleton.");
		_sharedMySingleton = [super alloc];
		return _sharedMySingleton;
	}
 
	return nil;
}

- (id) init
{
	self = [super initWithWindowNibName:@"RestartAviator"];
	imagePath = nil;
	return self ;
}

- (void) dealloc
{
 	[super dealloc];
}

- (void) setImagePath:(NSString*) aImagePath
{
  [aImagePath retain] ;
  NSString*	  iconPath	= [NSString stringWithFormat:@"%@/quit.icns",[[NSBundle mainBundle]resourcePath]] ;
  imagePath =iconPath;
  NSImage* image = [[NSImage alloc] initWithContentsOfFile:imagePath];
  [imageView setImage:image];
		
	
}

- (void)windowWillClose:(NSNotification*)notification
{
    [NSApp stopModalWithCode:true];
   
}

- (IBAction) cancelButtonAction:(id)sender  // Restart Button
{  
    brestartAviatorToTakeUpdate=true;   // Champion Restart Aviator for take update (Balaji)
   	[self.window close] ;
	chrome::AttemptRestart();
	[self.window close] ; 
	
}

- (IBAction) okButtonAction:(id)sender  // Dismiss Button
{ 
   bshowAviatorRestartDialogAgain_URL=true;
   bshowAviatorRestartDialogAgain=true; // Champion Restart Aviator for take update (Balaji)
   [self.window close] ;
}

@end


