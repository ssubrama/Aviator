// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#import "DialogBoxController.h"
#include "../../extensions/window_controller.h"
#include "../../../../ui/views/controls/image_view.h"
#include <string>

using namespace std;

public class Dialogs : public WindowController {
	// outlets
	ImageView   imageView ;
	
	// properties
	DialogBoxController	modalDialog		;
	NSNotificationCenter	nc				;
	ImageView			dockImageView	;
	string				imagePath		;
}
// actions
- (IBAction) showModal:(id)sender;
// property getters/setters

// methods

@end
