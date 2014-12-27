// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "../../extensions/window_controller.h"
#include "../../../../ui/views/controls/image_view.h"
#include "../../../../ui/views/controls/textfield/textfield.h"
#include <string>

using namespace std;

extern int iQuitRadioSelectedTag;

public class DialogBoxController : public WindowController {
public:
	DialogBoxController ();
	void setImagePath (string aImagePath);
private:
	TextField   textField;
	ImageView   imageView;
	TextField   imageField;
    //IBOutlet    NSMatrix *nsmatQuitRadio;
    //IBOutlet NSButtonCell *nsbcellQuitRadio_Yes;
    //IBOutlet NSButtonCell *nsbcellQuitRadio_No;
	string				imagePath;
};
// actions
//- (IBAction) closeButton:(id)sender;
//- (IBAction) imageFile:(id)sender;
//- (IBAction) quitRadioAction:(id)sender;


// property getters/setters

