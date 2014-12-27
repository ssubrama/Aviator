// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_WEBUI_FEEDBACK_UI_H_
#define CHROME_BROWSER_UI_WEBUI_FEEDBACK_UI_H_

#include <string>

#include "base/basictypes.h"
#include "ui/web_dialogs/web_dialog_ui.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_ui_controller.h"
#include "ui/base/layout.h"
#include "chrome/browser/ui/browser.h"

namespace base {
  class FilePath;
}

//class FeedbackUI : public ui::WebDialogUI {
class FeedbackUI :public content::WebUIController {
public:
  explicit FeedbackUI(content::WebUI* web_ui);
  virtual ~FeedbackUI();
  // explicit FeedbackUI(content::WebUI* web_ui);

#if defined(OS_CHROMEOS)
  static void GetMostRecentScreenshots(
    const base::FilePath& filepath,
    std::vector<std::string>* saved_screenshots,
    size_t max_saved);
#endif

private:
  DISALLOW_COPY_AND_ASSIGN(FeedbackUI);
  base::WeakPtrFactory<FeedbackUI> weak_factory_;
};

#endif  // CHROME_BROWSER_UI_WEBUI_FEEDBACK_UI_H_

namespace chromefeedback {
  void ShowFeedbackPage1(Browser* browser,
    const std::string& description_template,
    const std::string& category_tag);
}
