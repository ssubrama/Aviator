// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "chrome/browser/net/net_pref_observer.h"

#include "base/prefs/pref_service.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/net/predictor.h"
#include "chrome/browser/prerender/prerender_manager.h"
#include "chrome/common/pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "content/public/browser/browser_thread.h"
#include "net/http/http_stream_factory.h"

#include "chrome/browser/championconfig/varsconfig.h" // champion : disable SPDY (amresh)

using content::BrowserThread;

NetPrefObserver::NetPrefObserver(PrefService* prefs,
                                 prerender::PrerenderManager* prerender_manager,
                                 chrome_browser_net::Predictor* predictor)
    : prerender_manager_(prerender_manager),
      predictor_(predictor) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(prefs);
  DCHECK(predictor);

  base::Closure prefs_callback = base::Bind(&NetPrefObserver::ApplySettings,
                                            base::Unretained(this));
  network_prediction_enabled_.Init(prefs::kNetworkPredictionEnabled, prefs,
                                   prefs_callback);
  spdy_disabled_.Init(prefs::kDisableSpdy, prefs, prefs_callback);

  ApplySettings();
}

NetPrefObserver::~NetPrefObserver() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
}

void NetPrefObserver::ApplySettings() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  predictor_->EnablePredictor(*network_prediction_enabled_);
  if (prerender_manager_)
    prerender_manager_->set_enabled(*network_prediction_enabled_);
  if (spdy_disabled_.IsManaged())
    net::HttpStreamFactory::set_spdy_enabled(!*spdy_disabled_);
}

// static
void NetPrefObserver::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(
      prefs::kNetworkPredictionEnabled,
      true,
      user_prefs::PrefRegistrySyncable::SYNCABLE_PREF);
  // champion : disable SPDY (amresh)
  if (DISABLE_SPDY)
    registry->RegisterBooleanPref(
      prefs::kDisableSpdy,
      true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  else
    registry->RegisterBooleanPref(
      prefs::kDisableSpdy,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  /*registry->RegisterBooleanPref(
      prefs::kDisableSpdy,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);*/
}
