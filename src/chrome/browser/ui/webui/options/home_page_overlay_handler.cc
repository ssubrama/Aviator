// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/ui/webui/options/home_page_overlay_handler.h"

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/values.h"
#include "chrome/browser/autocomplete/autocomplete_classifier.h"
#include "chrome/browser/autocomplete/autocomplete_controller.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_result.h"
#include "chrome/browser/profiles/profile.h"
#include "components/metrics/proto/omnibox_event.pb.h"
#include "content/public/browser/web_ui.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "chrome/common/pref_names.h" //Champion:Added for reset button in homepage. Navneet
#include "base/prefs/pref_service.h" //Champion:Added for reset button in homepage. Navneet
#include "chrome/browser/championconfig/varsconfig.h"  //Champion:Added for reset button in homepage. Navneet 

namespace options {

HomePageOverlayHandler::HomePageOverlayHandler() {
}

HomePageOverlayHandler::~HomePageOverlayHandler() {
}

void HomePageOverlayHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "requestAutocompleteSuggestionsForHomePage",
      base::Bind(&HomePageOverlayHandler::RequestAutocompleteSuggestions,
                 base::Unretained(this)));
			 
  //Champion : Added for reset button in homepage. Navneet 
  web_ui()->RegisterMessageCallback(
      "resetHomePageURL",
      base::Bind(&HomePageOverlayHandler::ResetHomePage,
                 base::Unretained(this)));
}

//Champion : Added for reset button in homepage. Navneet 
void HomePageOverlayHandler::ResetHomePage( const base::ListValue* args ){
  Profile* profile = Profile::FromWebUI(web_ui());
  std::string defaultSearchEngine = profile->GetPrefs()->GetString(prefs::kDefaultSearchProviderName);
  std::string loadUrl = HOMEPAGE_URL;
  if (defaultSearchEngine == "Yahoo! (Disconnect)" ){
    loadUrl.append("#searchEngine=Yahoo");
  } else if (defaultSearchEngine == "DuckDuckGo (Disconnect)" ) {
    loadUrl.append("#searchEngine=DuckDuckGo");
  } else if (defaultSearchEngine == "Bing (Disconnect) " ) {
    loadUrl.append("#searchEngine=Bing");
  } else if (defaultSearchEngine == "Google (Disconnect)") {
    loadUrl.append("#searchEngine=Google");
  }
  profile->GetPrefs()->SetString(prefs::kHomePage, loadUrl);
}

void HomePageOverlayHandler::InitializeHandler() {
  Profile* profile = Profile::FromWebUI(web_ui());
  autocomplete_controller_.reset(new AutocompleteController(profile, this,
      AutocompleteClassifier::kDefaultOmniboxProviders));
}

void HomePageOverlayHandler::GetLocalizedValues(
    base::DictionaryValue* localized_strings) {
  RegisterTitle(localized_strings, "homePageOverlay",
                IDS_OPTIONS_HOMEPAGE_TITLE);
}

void HomePageOverlayHandler::RequestAutocompleteSuggestions(
    const base::ListValue* args) {
  base::string16 input;
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(args->GetString(0, &input));

  autocomplete_controller_->Start(AutocompleteInput(
      input, base::string16::npos, base::string16(), GURL(),
      metrics::OmniboxEventProto::INVALID_SPEC, true, false, false, true));
}

void HomePageOverlayHandler::OnResultChanged(bool default_match_changed) {
  const AutocompleteResult& result = autocomplete_controller_->result();
  base::ListValue suggestions;
  OptionsUI::ProcessAutocompleteSuggestions(result, &suggestions);
  web_ui()->CallJavascriptFunction(
      "HomePageOverlay.updateAutocompleteSuggestions", suggestions);
}

}  // namespace options
