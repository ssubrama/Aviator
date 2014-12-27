// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

//#import "QuitDialog.h"

#include "chrome/browser/championconfig/Warn_before_quit/QuitDialog.h"
#include "chrome/common/pref_names.h" 
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "base/prefs/pref_service.h"

// NSString* nIconPathChanged = @"iconPathChanged" ;
bool bQuit=false; 


/*int MessageBox12345( const char* header, const char* message, unsigned long message_type )
{
    
    CFStringRef header_ref      = CFStringCreateWithCString( NULL, header,     strlen(header)    );
    CFStringRef message_ref  = CFStringCreateWithCString( NULL, message,  strlen(message) );
    
    CFOptionFlags result;
    
    
    CFUserNotificationDisplayAlert( 0,kCFUserNotificationNoteAlertLevel, NULL,NULL,
                                   NULL,header_ref,message_ref,NULL,CFSTR("Cancel"),
                                   NULL,&result );
    
    
    CFRelease( header_ref );
    CFRelease( message_ref );
    
    if( result == kCFUserNotificationDefaultResponse )
        return 1; //IDOK;
    else
        return 0; //IDCANCEL;
    
}*/

@implementation QuitDialog

static QuitDialog* _sharedMySingleton = nil;
 
+(QuitDialog*)sharedMySingleton
{
	@synchronized([QuitDialog class])
	{
		if (!_sharedMySingleton)
			[[self alloc] init];
 
		return _sharedMySingleton;
	}
 
	return nil;
}
 
+(id)alloc
{
	@synchronized([QuitDialog class])
	{
		NSAssert(_sharedMySingleton == nil, @"Attempted to allocate a second instance of a singleton.");
		_sharedMySingleton = [super alloc];
		return _sharedMySingleton;
	}
 
	return nil;
}

- (id) init
{
	//self = [super initWithWindowNibName:@"DialogBox"];
	self = [super initWithWindowNibName:@"Quit"];
	NSLog(@"QuitDialog init self = %@",self);
	
/*	nc = [[NSNotificationCenter defaultCenter]retain] ;
	[nc addObserver:self
		   selector:@selector(iconPathChanged:)
			   name:nIconPathChanged
			 object:nil
	];*/
	imagePath = nil;
	//MessageBox12("QuitDialog","Init",1);
	return self ;
}

- (void) dealloc
{
 //	[nc release];
	[imagePath release];
	[super dealloc];
}


/*- (void) dealloc
{
	[nc release];
	[modalDialog    release];
	[dockImageView  release];
	[imagePath		release];
	[super dealloc];
}*/

- (void) setImagePath:(NSString*) aImagePath
{
	/*@try {*/
		[aImagePath retain] ;
	//	[imagePath release];
		//NSString*	  iconPath	= [NSString stringWithFormat:@"%@/product_logo_64_1.jpg",[[NSBundle mainBundle]resourcePath]] ;
		//imagePath = aImagePath;
		NSString*	  iconPath	= [NSString stringWithFormat:@"%@/quit.icns",[[NSBundle mainBundle]resourcePath]] ;
	  imagePath =iconPath;
		NSImage* image = [[NSImage alloc] initWithContentsOfFile:imagePath];
		[imageView setImage:image];
		//NSImage *image = [NSImage imageNamed: @"product_logo_64.png"];
		//if ( image ) {
		//	[NSApp setApplicationIconImage: image];
		//	[imageView setImage:image];
		
		
			
		//}
	/*}
	@catch (NSException * e) {
		NSLog(@"e = %@",e);
	}
	@finally {
		//
	}*/
	
}

/* - (void) iconPathChanged:(NSNotification*)note
{
	NSLog(@"dialogs.mm iconPathChanged %@",note);
	[self setImagePath: [note object]] ;

}



- (void) openPanelDidEnd:(NSOpenPanel *)op
			 returnCode:(int)returnCode
			contextInfo:(void *)ci
{
	if (returnCode == NSOKButton) {
		NSString* path = [op filename];
		[nc postNotificationName:nIconPathChanged object:path];
	}
} 


- (IBAction) imageFile:(id)sender
{

	NSOpenPanel *op = [NSOpenPanel openPanel];
	
	[op beginSheetForDirectory: imagePath
						  file: [imagePath lastPathComponent]
						 types: [NSImage imageFileTypes]
				modalForWindow: [self window]
				 modalDelegate: self
				didEndSelector: @selector(openPanelDidEnd:returnCode:contextInfo:)
				   contextInfo: nil
	];
	
	NSLog(@"imageFile pressed currentString = %@",imagePath);
} */


- (void) createModalDialog
{
	if ( !modalDialog ) { 
	//MessageBox12("QuitDialog","createModalDialog",1);
		NSLog(@"showSheet alloc init");
		modalDialog = [[DialogBoxController alloc]init] ;
		
		if ( modalDialog ) {
			// I've we dont do this, things are happy
			// I think it has something to do the default size of the box
			[modalDialog showWindow:self];
			[[modalDialog window]setDelegate:modalDialog];
			[modalDialog close];
		}
		if ( modalDialog ) {
			[modalDialog setImagePath:imagePath];
		}
	}
}

- (IBAction) showModal:(id)sender
{
	
	NSButton *btn = (NSButton *)sender;
  int i=(int)btn.state;
  if(i==1)
  {
  	
		NSLog(@"showModal");
		//MessageBox12345("QuitDialog","checked",1);
		[self createModalDialog];
	
		if ( modalDialog ) {
				NSLog(@"showModal runModalForWindow");
				int result = [NSApp runModalForWindow:[modalDialog window]];
			NSLog(@"result = %d",result) ;
		}
	}
	
	
}
/*- (IBAction) closeButton:(id)sender 
{
	NSLog(@"closeButton");
    
	int result = 12345 ;
	[NSApp stopModalWithCode:result]; // not convinced this does anything
	[NSApp endSheet:[self window] returnCode:result];
	
	[[self window] orderOut:sender];
	[[self window] close] ;
}*/

- (IBAction) okButtonAction:(id)sender 
{
	 Profile* profile = ProfileManager::GetLastUsedProfile();
   PrefService* prefs =profile->GetPrefs();
 
	int i= butDisable.state;
	if(i==1)
	
	   prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,false);
	
	else
	
	    prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,true);
	 
	
	NSLog(@"closeButton");
  bQuit=true;  
	int result = 12345 ;
	[NSApp stopModalWithCode:result]; // not convinced this does anything
	[NSApp endSheet:[self window] returnCode:result];
	
	[[self window] orderOut:sender];
	[[self window] close] ;
}

- (IBAction) cancelButtonAction:(id)sender 
{
	int i= butDisable.state;
	
	 Profile* profile = ProfileManager::GetLastUsedProfile();
   PrefService* prefs =profile->GetPrefs();
	
	if(i==1)
	
	   prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,false);
	
	else

	    prefs->SetBoolean(prefs::kWarnBeforeQuitEnabled,true);
	 
	NSLog(@"closeButton");
    bQuit=false;
	int result = 12345 ;
	[NSApp stopModalWithCode:result]; // not convinced this does anything
	[NSApp endSheet:[self window] returnCode:result];
	
	[[self window] orderOut:sender];
	[[self window] close] ;
}



@end


