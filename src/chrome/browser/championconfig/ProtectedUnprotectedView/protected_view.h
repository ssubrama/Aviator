// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_VIEW_H_

#include "ui/views/controls/image_view.h"

class Browser;

class ProtectedView : public views::ImageView {
public:
  ProtectedView(Browser* browser);

  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual ~ProtectedView();

  void SetToggled(bool on);

private:
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(ProtectedView);
};

#endif  // CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_STAR_VIEW_H_
