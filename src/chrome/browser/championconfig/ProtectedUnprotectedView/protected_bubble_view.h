// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/combobox/combobox_listener.h"

namespace views {
  class LabelButton;
  class Textfield;
}

// ProtectedBubbleView is a view intended to be used as the content of an
// Bubble. ProtectedBubbleView provides views for unstarring and editing the
// bookmark it is created with. Don't create a ProtectedBubbleView directly,
// instead use the static Show method.
class ProtectedBubbleView : public views::BubbleDelegateView{
public:
  static void ShowBubble(views::View* anchor_view );
  static bool IsShowing();
  static void Hide();

  virtual ~ProtectedBubbleView();

  // views::WidgetDelegate method.
  virtual void WindowClosing() OVERRIDE;

protected:
  // views::BubbleDelegateView method.
  virtual void Init() OVERRIDE;

private:
  // Creates a ProtectedBubbleView.
  ProtectedBubbleView(views::View* anchor_view );

  // Returns the title to display.
  base::string16 GetTitle();

  // Overridden from views::View:
  virtual gfx::Size GetMinimumSize() const OVERRIDE;

  // The bookmark bubble, if we're showing one.
  static ProtectedBubbleView* protected_bubble_;

  // Textfield showing the title of the bookmark.
  views::Textfield* title_tf_;

  // Protected sync promo view, if displayed.
  views::View* sync_promo_view_;

  DISALLOW_COPY_AND_ASSIGN(ProtectedBubbleView);
};

#endif  // CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PROTECTED_BUBBLE_VIEW_H_
