// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_PROMO_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_PROMO_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/styled_label.h"
#include "ui/views/controls/styled_label_listener.h"
#include "ui/views/view.h"

// Bookmark sync promo displayed at the bottom of the bookmark bubble.
class ProtectedPromoView : public views::StyledLabelListener,
  public views::View {
public:
  // |delegate| is not owned by ProtectedPromoView.
  explicit ProtectedPromoView();

private:
  // views::StyledLabelListener:
  virtual void StyledLabelLinkClicked(const gfx::Range& range, int event_flags) OVERRIDE;
  views::StyledLabel* promo_label;
  
  DISALLOW_COPY_AND_ASSIGN(ProtectedPromoView);
};

#endif  // CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_PROMO_VIEW_H_
