// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include <string>
#include "base/strings/string16.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "chrome/browser/championconfig/show_disconnect_search_popup.h"

int showDefaultSearchPopUp::showPopup(int value){

	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:@"Change default search engine"];
	
	if (value == 1 ) // Having default search engine any one of the supported search engines
	{
	[alert addButtonWithTitle:@"OK"];
	
	[alert setInformativeText:@"To improve privacy, Aviator omnibox searches will go through Disconnect Search."];
	[alert setAlertStyle:NSWarningAlertStyle];
	
	if ([alert runModal] == NSAlertFirstButtonReturn) {
    	// OK clicked, change to DuckDuCKGo + disconnect
		return 0;
	}
	}
	else { // Having default search engine which is not supported by Disconnect.
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setInformativeText:@"To improve privacy, Aviator omnibox searches will go through Disconnect Search. Click Ok if you want to use Disconnect Search else click Cancel to retain your old search engine."];
	[alert setAlertStyle:NSWarningAlertStyle];
	if ([alert runModal] == NSAlertFirstButtonReturn) {
    	// OK clicked, change to DuckDuCKGo + disconnect
		return 1;
	}
	else{
		// Cancel clicked. Retain old search engine
		return 2;
	}
	
	}
	return 0;
}
