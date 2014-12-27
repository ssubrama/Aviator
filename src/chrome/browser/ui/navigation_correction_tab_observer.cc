// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "chrome/browser/ui/navigation_correction_tab_observer.h"

#include "base/prefs/pref_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/google/google_profile_helper.h"
#include "chrome/browser/google/google_url_tracker_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/render_messages.h"
#include "components/google/core/browser/google_util.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "google_apis/google_api_keys.h"

// Champion artf414382 Build library on Mac	
#include "chrome/browser/championconfig/varsconfig.h" 

using content::RenderFrameHost;
using content::RenderViewHost;
using content::WebContents;

DEFINE_WEB_CONTENTS_USER_DATA_KEY(NavigationCorrectionTabObserver);

NavigationCorrectionTabObserver::NavigationCorrectionTabObserver(
    WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      profile_(Profile::FromBrowserContext(web_contents->GetBrowserContext())) {
  PrefService* prefs = profile_->GetPrefs();
  if (prefs) {
    pref_change_registrar_.Init(prefs);
    pref_change_registrar_.Add(
        prefs::kAlternateErrorPagesEnabled,
        base::Bind(&NavigationCorrectionTabObserver::OnEnabledChanged,
                   base::Unretained(this)));
  }

  GoogleURLTracker* google_url_tracker =
      GoogleURLTrackerFactory::GetForProfile(profile_);
  if (google_url_tracker) {
    google_url_updated_subscription_ = google_url_tracker->RegisterCallback(
        base::Bind(&NavigationCorrectionTabObserver::OnGoogleURLUpdated,
                   base::Unretained(this)));
  }
}

NavigationCorrectionTabObserver::~NavigationCorrectionTabObserver() {
}

// static
void NavigationCorrectionTabObserver::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* prefs) {
  if(WEBSERVICE_NAV_ERRORS)
    prefs->RegisterBooleanPref(prefs::kAlternateErrorPagesEnabled, // champion navigaton_errors                             false,
                               false,user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  else
    prefs->RegisterBooleanPref(prefs::kAlternateErrorPagesEnabled,
                               true,
                               user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
}

////////////////////////////////////////////////////////////////////////////////
// WebContentsObserver overrides

void NavigationCorrectionTabObserver::RenderViewCreated(
    RenderViewHost* render_view_host) {
  UpdateNavigationCorrectionInfo(render_view_host);
}

////////////////////////////////////////////////////////////////////////////////
// Internal helpers

void NavigationCorrectionTabObserver::OnGoogleURLUpdated(GURL old_url,
                                                         GURL new_url) {
  UpdateNavigationCorrectionInfo(web_contents()->GetRenderViewHost());
}

GURL NavigationCorrectionTabObserver::GetNavigationCorrectionURL() const {
  // Disable navigation corrections when the preference is disabled or when in
  // Incognito mode.
  if (!profile_->GetPrefs()->GetBoolean(prefs::kAlternateErrorPagesEnabled) ||
      profile_->IsOffTheRecord()) {
    return GURL();
  }

  return google_util::LinkDoctorBaseURL();
}

void NavigationCorrectionTabObserver::OnEnabledChanged() {
  UpdateNavigationCorrectionInfo(web_contents()->GetRenderViewHost());
}

void NavigationCorrectionTabObserver::UpdateNavigationCorrectionInfo(
    RenderViewHost* rvh) {
  RenderFrameHost* rfh = rvh->GetMainFrame();
  rfh->Send(new ChromeViewMsg_SetNavigationCorrectionInfo(
      rfh->GetRoutingID(),
      GetNavigationCorrectionURL(),
      google_util::GetGoogleLocale(g_browser_process->GetApplicationLocale()),
      google_util::GetGoogleCountryCode(
          google_profile_helper::GetGoogleHomePageURL(profile_)),
      google_apis::GetAPIKey(),
      google_util::GetGoogleSearchURL(
          google_profile_helper::GetGoogleHomePageURL(profile_))));
}
