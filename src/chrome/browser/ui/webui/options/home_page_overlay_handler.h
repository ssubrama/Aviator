// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_HOME_PAGE_OVERLAY_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_HOME_PAGE_OVERLAY_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class AutocompleteController;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace options {

class HomePageOverlayHandler : public OptionsPageUIHandler,
                               public AutocompleteControllerDelegate {
 public:
  HomePageOverlayHandler();
  virtual ~HomePageOverlayHandler();
  void ResetHomePage(const base::ListValue* args); //Champion : Added for reset button in homepage. Navneet 

  // OptionsPageUIHandler implementation
  virtual void GetLocalizedValues(base::DictionaryValue*) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  // AutocompleteControllerDelegate implementation.
  virtual void OnResultChanged(bool default_match_changed) OVERRIDE;

 private:
  void RequestAutocompleteSuggestions(const base::ListValue* args);

  scoped_ptr<AutocompleteController> autocomplete_controller_;

  DISALLOW_COPY_AND_ASSIGN(HomePageOverlayHandler);
};

}  // namespace options

#endif  // CHROME_BROWSER_UI_WEBUI_OPTIONS_HOME_PAGE_OVERLAY_HANDLER_H_
