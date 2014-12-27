// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PROTECTED_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PROTECTED_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/location_bar/image_decoration.h"
#import "chrome/browser/championconfig/ProtectedUnprotectedViewMac/protected_bubble_controller.h"
#include "chrome/browser/ui/browser.h"
class CommandUpdater;

// Star icon on the right side of the field.

class ProtectedDecoration : public ImageDecoration {
 public:
  explicit ProtectedDecoration(CommandUpdater* command_updater);
  explicit ProtectedDecoration(Browser* browser);
  virtual ~ProtectedDecoration();

  // Sets the image and tooltip based on |starred|.
  void SetStarred(bool starred);

  // Returns true if the star is lit.
  bool starred() const { return starred_; }

  // Get the point where the bookmark bubble should point within the
  // decoration's frame.
  virtual NSPoint GetBubblePointInFrame(NSRect frame) OVERRIDE;

  // Implement |LocationBarDecoration|.
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame,NSPoint location) OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;
  virtual bool OnMouseMoved(NSRect frame) OVERRIDE;// champion_protected
  private:
  // For bringing up bookmark bar.
  CommandUpdater* command_updater_;  // Weak, owned by Browser.
  
  Browser* browser_; // champion_protected

  // The string to show for a tooltip.
  base::scoped_nsobject<NSString> tooltip_;

  // Whether the star icon is lit.
  bool starred_;
  
  ProtectedBubbleController* protectedBubbleController_;
  
  //void mouseMoved:(NSEvent*)event;

  DISALLOW_COPY_AND_ASSIGN(ProtectedDecoration);
};



#endif  // CHROME_BROWSER_UI_COCOA_LOCATION_BAR_STAR_DECORATION_H_
