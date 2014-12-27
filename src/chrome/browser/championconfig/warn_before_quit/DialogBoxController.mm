// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import "DialogBoxController.h"
#import "QuitDialog.h"

@implementation DialogBoxController

// NSString* nIconPathChanged1 = @"iconPathChanged" ;
int iQuitRadioSelectedTag=10; // no
 
int MessageBox13( const char* header, const char* message, unsigned long message_type )
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
    
}


- (void) dealloc
{
	//[nc release];
	[imagePath release];
	[super dealloc];
}

/*- (void) iconPathChanged:(NSNotification*)note
{
	NSLog(@"%s iconPathChanged %@",__FILE__,note);
	[self setImagePath:[note object]];
}*/

/*if (abs(windowHeight-[imageFromBundle size].width) > abs(windowWidth-[imageFromBundle size].height)) {

        frame.size.height = windowWidth;
        frame.size.width = 1+windowWidth*(([imageFromBundle size].width/[imageFromBundle size].height-(10*pow([imageFromBundle size].width, 2))/(pow([imageFromBundle size].height, 2)* windowHeight)));

    } else {

        frame.size.width = windowHeight;
        frame.size.height = ((windowHeight/[imageFromBundle size].width)*[imageFromBundle size].height);

    }

    [MyWindow setFrame: frame
             display: YES
             animate: YES]; */

- (id) init
{
	//self = [super initWithWindowNibName:@"DialogBox"];
	self = [super initWithWindowNibName:@"QuitWarn"];
	NSLog(@"DialogBoxController init self = %@",self);
	
 /*	nc = [[NSNotificationCenter defaultCenter]retain] ;
	[nc addObserver:self
		   selector:@selector(iconPathChanged:)
			   name:nIconPathChanged1
			 object:nil
	]; */
	imagePath = nil;
	
	return self ;
}

- (void) setImagePath: (NSString*) aImagePath
{
	
		[aImagePath retain];
		//[imagePath release];
		imagePath = aImagePath;
		NSImage* image = [[NSImage alloc] initWithContentsOfFile:imagePath];
	   [imageView setImage:image];
	
			
	
	
}

- (BOOL) windowShouldClose:(id)sender
{
	NSLog(@"window should close");
	[self closeButton:sender];
	return YES;
}

- (IBAction) closeButton:(id)sender 
{
	NSLog(@"closeButton");

	int result = 12345 ;
	[NSApp stopModalWithCode:result]; // not convinced this does anything
	[NSApp endSheet:[self window] returnCode:result];
	
	[[self window] orderOut:sender];
	[[self window] close] ;
}

- (IBAction) quitRadioAction:(id)sender
{
   
    @try {
        
          
        NSMatrix *btncell=(NSMatrix *)sender;
        iQuitRadioSelectedTag = (int) btncell.selectedTag;
        NSLog(@"button tag: %d",iQuitRadioSelectedTag);
        
        if(iQuitRadioSelectedTag==10)
        {
        	  [[QuitDialog sharedMySingleton]->butDisable setState:0];

          
        }
        else
        {
            //[objQuitDialog->butDisable setState:1];
            [[QuitDialog sharedMySingleton]->butDisable setState:1];
            	
        }
            
        
    }
    @catch (NSException *exception) {
        NSLog(@"Exception --> %@",[exception reason]);
    }  
    
}
@end
