// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#ifndef CHROME_BROWSER_UI_WEBUI_HELP_HELP_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_HELP_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/help/version_updater.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"
#if defined (OS_MACOSX)
#include "chrome/browser/mac/keystone_glue.h" // champion - update mechanism in about page
#endif
#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/version_loader.h"
#endif  // defined(OS_CHROMEOS)

namespace content {
class WebUIDataSource;
}

#if defined (OS_WIN)
class AviatorUpdater;
#endif
// WebUI message handler for the help page.
class HelpHandler : public content::WebUIMessageHandler,
                    public content::NotificationObserver {
 public:
  HelpHandler();
  virtual ~HelpHandler();

  // WebUIMessageHandler implementation.
  virtual void RegisterMessages() OVERRIDE;

  // Fills |source| with string values for the UI.
  void GetLocalizedValues(content::WebUIDataSource* source);

  // NotificationObserver implementation.
  virtual void Observe(int type, const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
#if defined (OS_MACOSX)
  bool IsAdminUser(); // champion added for To show setup update for all users button only admin user in help page                    
  void UpdateAviator();
  
  static bool isUpdateInProgress;
  static void setUpdateInProgress(bool bValue);
  static bool IsUpdateFilesExists(); // champion changed as static before not
#endif  
 private:
  // Initializes querying values for the page.
  void OnPageLoaded(const base::ListValue* args);

#if defined(OS_MACOSX)
  // Promotes the updater for all users.
  void PromoteUpdater(const base::ListValue* args);
#endif

  // Relaunches the browser. |args| must be empty.
  void RelaunchNow(const base::ListValue* args);

  // Opens the feedback dialog. |args| must be empty.
  void OpenFeedbackDialog(const base::ListValue* args);

  // Opens the help page. |args| must be empty.
  void OpenHelpPage(const base::ListValue* args);

#if defined(OS_CHROMEOS)
  // Sets the release track version.
  void SetChannel(const base::ListValue* args);

  // Performs relaunch and powerwash.
  void RelaunchAndPowerwash(const base::ListValue* args);
#endif

  // Callback method which forwards status updates to the page.
  void SetUpdateStatus(VersionUpdater::Status status, int progress,
                       const base::string16& fail_message);

#if defined(OS_MACOSX)
  // Callback method which forwards promotion state to the page.
  void SetPromotionState(VersionUpdater::PromotionState state);
#endif

#if defined(OS_CHROMEOS)
  // Callbacks from VersionLoader.
  void OnOSVersion(const std::string& version);
  void OnOSFirmware(const std::string& firmware);
  void OnCurrentChannel(const std::string& channel);
  void OnTargetChannel(const std::string& channel);
#endif

  // Specialized instance of the VersionUpdater used to update the browser.
  scoped_ptr<VersionUpdater> version_updater_;

#if defined(OS_WIN)
  // champion : our update handler (Amresh)
  scoped_refptr<AviatorUpdater> update_launcher_;
#endif
  // Used to observe notifications.
  content::NotificationRegistrar registrar_;

#if defined(OS_CHROMEOS)
  // Handles asynchronously loading the CrOS version info.
  chromeos::VersionLoader loader_;

  // Used to request the version.
  base::CancelableTaskTracker tracker_;
#endif  // defined(OS_CHROMEOS)

  // Used for callbacks.
  base::WeakPtrFactory<HelpHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(HelpHandler);
};

#endif  // CHROME_BROWSER_UI_WEBUI_HELP_HELP_HANDLER_H_
