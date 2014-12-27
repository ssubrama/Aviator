// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/protectedunprotectedview/protected_bubble_view.h"
#include "chrome/browser/championconfig/protectedunprotectedview/protected_promo_view.h" // champion

#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/app/chrome_command_ids.h"

#include "chrome/browser/ui/sync/sync_promo_ui.h"
#include "content/public/browser/user_metrics.h"
#include "grit/generated_resources.h"
#include "grit/theme_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/views/bubble/bubble_frame_view.h"
#include "ui/views/controls/button/label_button.h"

#include "ui/views/controls/label.h"
#include "ui/views/controls/link.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/layout/layout_constants.h"
#include "ui/views/widget/widget.h"

#include "chrome/browser/ui/views/bookmarks/bookmark_bubble_view.h" // champion

using base::UserMetricsAction;
using views::ColumnSet;
using views::GridLayout;

namespace {

// Minimum width of the the bubble.
const int kMinBubbleWidth = 150;

// Width of the border of a button.
const int kControlBorderWidth = 2;

// Background color of the promo.
const SkColor kBackgroundColor = SkColorSetRGB(245, 245, 245);

// Color of the top border of the promo.
const SkColor kBorderColor = SkColorSetRGB(229, 229, 229);

// Width of the top border of the promo.
const int kBorderWidth = 1;

// Color of the text of the promo.
const SkColor kTextColor = SkColorSetRGB(102, 102, 102); 

}  // namespace

ProtectedBubbleView* ProtectedBubbleView::protected_bubble_ = NULL;

// static
void ProtectedBubbleView::ShowBubble(views::View* anchor_view) {
  if (IsShowing())
    return;

  protected_bubble_ = new ProtectedBubbleView(anchor_view);
  views::BubbleDelegateView::CreateBubble(protected_bubble_)->Show();
 
}

// static
bool ProtectedBubbleView::IsShowing() {
  return protected_bubble_ != NULL;
}

void ProtectedBubbleView::Hide() {
  if (IsShowing()){
    views::Widget *pWidget = protected_bubble_->GetWidget();
    if (pWidget)
      pWidget->Close();
  }
}

ProtectedBubbleView::~ProtectedBubbleView() {}

void ProtectedBubbleView::WindowClosing() {
  // We have to reset |bubble_| here, not in our destructor, because we'll be
  // destroyed asynchronously and the shown state will be checked before then.
  DCHECK_EQ(protected_bubble_, this);
  protected_bubble_ = NULL;
}

void ProtectedBubbleView::Init() {
  views::Label* title_label = new views::Label(l10n_util::GetStringUTF16(IDS_PROTECTED_BUBBLE));
  ui::ResourceBundle* rb = &ui::ResourceBundle::GetSharedInstance();
  // champion : SetFont and GetFont no more exist (amresh)
  //title_label->SetFont(rb->GetFont(ui::ResourceBundle::MediumFont));
  title_label->SetFontList(rb->GetFontList(ui::ResourceBundle::MediumFont));
  
  GridLayout* layout = new GridLayout(this);
  SetLayoutManager(layout);

  // Column sets used in the layout of the bubble.
  enum ColumnSetID {
    TITLE_COLUMN_SET_ID,
    CONTENT_COLUMN_SET_ID,
    SYNC_PROMO_COLUMN_SET_ID
  };

  ColumnSet* cs = layout->AddColumnSet(TITLE_COLUMN_SET_ID);
  cs->AddPaddingColumn(0, views::kButtonHEdgeMarginNew);
  cs->AddColumn(GridLayout::CENTER, GridLayout::CENTER, 0, GridLayout::USE_PREF, 0, 0);
  cs->AddPaddingColumn(0, views::kButtonHEdgeMarginNew);

  layout->StartRow(0, TITLE_COLUMN_SET_ID);
  layout->AddView(title_label);
  layout->AddPaddingRow(0, views::kRelatedControlHorizontalSpacing);

  cs = layout->AddColumnSet(SYNC_PROMO_COLUMN_SET_ID);
  cs->AddColumn(GridLayout::FILL,
                  GridLayout::FILL,
                  1,
                  GridLayout::USE_PREF,
                  0,
                  0);
    layout->StartRow(0, SYNC_PROMO_COLUMN_SET_ID);

    sync_promo_view_ = new ProtectedPromoView();
    layout->AddView(sync_promo_view_);
    
  AddAccelerator(ui::Accelerator(ui::VKEY_RETURN, ui::EF_NONE));
}

ProtectedBubbleView::ProtectedBubbleView(views::View* anchor_view ) : BubbleDelegateView (anchor_view, views::BubbleBorder::TOP_RIGHT) {
  const SkColor background_color = GetNativeTheme()->GetSystemColor(ui::NativeTheme::kColorId_DialogBackground);
  set_color(background_color);
  set_background(views::Background::CreateSolidBackground(background_color));
  set_margins(gfx::Insets(views::kPanelVertMargin, 0, 0, 0));
  // Compensate for built-in vertical padding in the anchor view's image.
  gfx::Insets inst(2, 0, 2, 0);
  set_anchor_view_insets(inst);
}

base::string16 ProtectedBubbleView::GetTitle() {
  return base::string16();
}

gfx::Size ProtectedBubbleView::GetMinimumSize() const {
  gfx::Size size(views::BubbleDelegateView::GetPreferredSize());
  size.SetToMax(gfx::Size(kMinBubbleWidth, 0));
  return size;
}
