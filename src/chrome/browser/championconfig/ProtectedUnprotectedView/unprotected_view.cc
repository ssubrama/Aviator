// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/protectedunprotectedview/unprotected_view.h"
#include "chrome/browser/championconfig/protectedunprotectedview/unprotected_bubble_view.h"

#include "base/metrics/histogram.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/ui/view_ids.h"
#include "grit/generated_resources.h"
#include "grit/theme_resources.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/browser.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

UnProtectedView::UnProtectedView(Browser* browser) : browser_(browser) {
  set_id(VIEW_ID_UNPROTECTED_BUTTON);
  SetToggled(false);
}

UnProtectedView::~UnProtectedView() {}

void UnProtectedView::SetToggled(bool on) {
  SetImage(ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(IDR_NOT_PROTECTED));
}

void UnProtectedView::OnMouseMoved(const ui::MouseEvent& event) {
	BrowserView* browser_view_ = BrowserView::GetBrowserViewForBrowser(browser_);
 
  if(browser_view_)
	  browser_view_->ShowUnProtectedBubble();
}

void UnProtectedView::OnMouseExited(const ui::MouseEvent& event) {
  UnProtectedBubbleView::Hide();
}


