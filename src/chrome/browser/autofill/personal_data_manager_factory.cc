// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

#include "chrome/browser/autofill/personal_data_manager_factory.h"

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/webdata/web_data_service_factory.h"
#include "components/autofill/core/browser/personal_data_manager.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

namespace autofill {

// static
PersonalDataManager* PersonalDataManagerFactory::GetForProfile(
    Profile* profile) {
      //champion - To show autofill protected deatails listed -  navneet 
     if (profile->IsOffTheRecord()){
      return static_cast<PersonalDataManager*>(
      	GetInstance()->GetServiceForBrowserContext(profile->GetOriginalProfile(), true));
     } else
      return static_cast<PersonalDataManager*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

// static
PersonalDataManagerFactory* PersonalDataManagerFactory::GetInstance() {
  return Singleton<PersonalDataManagerFactory>::get();
}

PersonalDataManagerFactory::PersonalDataManagerFactory()
    : BrowserContextKeyedServiceFactory(
        "PersonalDataManager",
        BrowserContextDependencyManager::GetInstance()) {
  DependsOn(WebDataServiceFactory::GetInstance());
}

PersonalDataManagerFactory::~PersonalDataManagerFactory() {
}

KeyedService* PersonalDataManagerFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  Profile* profile = Profile::FromBrowserContext(context);
  PersonalDataManager* service =
      new PersonalDataManager(g_browser_process->GetApplicationLocale());
  service->Init(WebDataServiceFactory::GetAutofillWebDataForProfile(
                    profile, Profile::EXPLICIT_ACCESS),
             //   profile->GetPrefs(), // champion navneet test
             	profile->GetOriginalProfile()->GetPrefs(), // champion navneet test
                profile->IsOffTheRecord());
  return service;
}

content::BrowserContext* PersonalDataManagerFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextOwnInstanceInIncognito(context);
}

}  // namespace autofill
