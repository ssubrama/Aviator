// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/protectedunprotectedview/protected_promo_view.h"

#include "base/strings/string16.h"
#include "grit/generated_resources.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/font.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/layout_constants.h"

namespace {
  // Background color of the promo.
  const SkColor kBackgroundColor = SkColorSetRGB(245, 245, 245);
  // Color of the top border of the promo.
  const SkColor kBorderColor = SkColorSetRGB(229, 229, 229);
  // Width of the top border of the promo.
  const int kBorderWidth = 1;
  // Color of the text of the promo.
  const SkColor kTextColor = SkColorSetRGB(102, 102, 102);
}  // namespace

ProtectedPromoView::ProtectedPromoView(){
  set_background(views::Background::CreateSolidBackground(kBackgroundColor));
  SetBorder (views::Border::CreateSolidSidedBorder(kBorderWidth, 0, 0, 0, kBorderColor));
  size_t offset;
  base::string16 link_text = l10n_util::GetStringUTF16( IDS_PROTECTED_BUBBLE_DESCRIPTION_TEXT_PROMO_LINK);
  base::string16 promo_text = l10n_util::GetStringFUTF16(
      IDS_PROTECTED_BUBBLE_DESCRIPTION_TEXT_PROMO_MESSAGE, link_text, &offset);

  promo_label = new views::StyledLabel(promo_text, this);
  promo_label->SetDisplayedOnBackgroundColor(kBackgroundColor);

  views::StyledLabel::RangeStyleInfo link_style = views::StyledLabel::RangeStyleInfo::CreateForLink();
  link_style.font_style = gfx::Font::NORMAL;
  //promo_label->AddStyleRange(gfx::Range(offset, offset + link_text.length()), link_style);

  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 10, 10, 0));
  AddChildView(promo_label);
}

void ProtectedPromoView::StyledLabelLinkClicked(const gfx::Range& range, int event_flags) {}
