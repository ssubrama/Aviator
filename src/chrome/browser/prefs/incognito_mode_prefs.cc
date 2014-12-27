// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/prefs/incognito_mode_prefs.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "base/prefs/pref_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "chrome/browser/championconfig/varsconfig.h" // champion lib

#if defined(OS_WIN)
#include "base/win/metro.h"
#endif  // OS_WIN

#if defined(OS_ANDROID)
#include "chrome/browser/android/chromium_application.h"
#endif  // OS_ANDROID

#if defined(OS_WIN)
namespace {

bool g_parental_control_on = false;

} // empty namespace
#endif  // OS_WIN

// static
bool IncognitoModePrefs::IntToAvailability(int in_value,
                                           Availability* out_value) {
  if (in_value < 0 || in_value >= AVAILABILITY_NUM_TYPES) {
    *out_value = ENABLED;
    return false;
  }
  *out_value = static_cast<Availability>(in_value);
  return true;
}

// static
IncognitoModePrefs::Availability IncognitoModePrefs::GetAvailability(
    const PrefService* pref_service) {
  DCHECK(pref_service);
  int pref_value = pref_service->GetInteger(prefs::kIncognitoModeAvailability);
  Availability result = IncognitoModePrefs::ENABLED;
  bool valid = IntToAvailability(pref_value, &result);
  DCHECK(valid);
  if (ArePlatformParentalControlsEnabled()) {
    if (result == IncognitoModePrefs::FORCED)
      LOG(ERROR) << "Ignoring FORCED incognito. Parental control logging on";
    return IncognitoModePrefs::DISABLED;
  }
  return result;
}

// static
void IncognitoModePrefs::SetAvailability(PrefService* prefs,
                                         const Availability availability) {
  prefs->SetInteger(prefs::kIncognitoModeAvailability, availability);
}

// static
void IncognitoModePrefs::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
#if defined (OS_MACOSX)      
	  /* Champion artf414382 Build library on Mac 	*/  
  /* Champion artf360007 : The Default mode should be Private Browsing */ 
  
 /* registry->RegisterIntegerPref(
      prefs::kIncognitoModeAvailability,
      IncognitoModePrefs::ENABLED,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF); */
  if(DEFAULT_INCOGNITO_OPTION)  
     registry->RegisterIntegerPref(
        prefs::kIncognitoModeAvailability,
        IncognitoModePrefs::FORCED,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);  
   else  
     registry->RegisterIntegerPref(
        prefs::kIncognitoModeAvailability,
        IncognitoModePrefs::ENABLED,
        user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF); 
#endif
#if !defined (OS_MACOSX)
 registry->RegisterIntegerPref(
        prefs::kIncognitoModeAvailability,
      IncognitoModePrefs::ENABLED,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
#endif 
}

// static
bool IncognitoModePrefs::ShouldLaunchIncognito(
    const CommandLine& command_line,
    const PrefService* prefs) {
#if defined (OS_MACOSX)
   //Added for champion any link should open incognito mode by default  	
   if(DEFAULT_INCOGNITO_OPTION)
   	{	
  		return (bool)(IncognitoModePrefs::FORCED);
   	}
   else
   {
    Availability incognito_avail = GetAvailability(prefs);
  	return incognito_avail != IncognitoModePrefs::DISABLED &&
         (command_line.HasSwitch(switches::kIncognito) ||
          incognito_avail == IncognitoModePrefs::FORCED);
}
#endif
#if !defined (OS_MACOSX)
Availability incognito_avail = GetAvailability(prefs);
  return incognito_avail != IncognitoModePrefs::DISABLED &&
         (command_line.HasSwitch(switches::kIncognito) ||
          incognito_avail == IncognitoModePrefs::FORCED);
#endif
}

// static
bool IncognitoModePrefs::CanOpenBrowser(Profile* profile) {
#if defined (OS_MACOSX)
 return true;
 #endif // champion added for while open a browser on unprotected mode by clicking on wrench menu button
  switch (GetAvailability(profile->GetPrefs())) {
    case IncognitoModePrefs::ENABLED:
      return true;

    case IncognitoModePrefs::DISABLED:
      return !profile->IsOffTheRecord();

    case IncognitoModePrefs::FORCED:
      return profile->IsOffTheRecord();

    default:
      NOTREACHED();
      return false;
  }
}

// static
bool IncognitoModePrefs::ArePlatformParentalControlsEnabled() {
#if defined(OS_WIN)
  // Disable incognito mode windows if parental controls are on. This is only
  // for Windows Vista and above.
  return base::win::IsParentalControlActivityLoggingOn();
#elif defined(OS_ANDROID)
  return chrome::android::ChromiumApplication::AreParentalControlsEnabled();
#else
  return false;
#endif
}

#if defined(OS_WIN)
void IncognitoModePrefs::InitializePlatformParentalControls() {
  g_parental_control_on = base::win::IsParentalControlActivityLoggingOn();
}
#endif // OS_WIN

bool IncognitoModePrefs::ArePlatformParentalControlsEnabledCached() {
#if defined(OS_WIN)
  return g_parental_control_on;
#elif defined(OS_ANDROID)
  return chrome::android::ChromiumApplication::AreParentalControlsEnabled();
#else
  return false;
#endif
}

