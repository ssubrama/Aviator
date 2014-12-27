// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_MAC_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_MAC_H_

#import <AppKit/AppKit.h>

#include "base/compiler_specific.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/webui/help/version_updater.h"

class AviatorUpdater; // champion

@class KeystoneObserver;

// OS X implementation of version update functionality, used by the WebUI
// About/Help page.
class VersionUpdaterMac : public VersionUpdater {
 public:
  // VersionUpdater implementation.
  virtual void CheckForUpdate(const StatusCallback& status_callback,
                              const PromoteCallback& promote_callback) OVERRIDE;
  virtual void PromoteUpdater() const OVERRIDE;
  virtual void RelaunchBrowser() const OVERRIDE;

  // Process status updates received from Keystone. The dictionary will contain
  // an AutoupdateStatus value as an intValue at key kAutoupdateStatusStatus. If
  // a version is available (see AutoupdateStatus), it will be present at key
  // kAutoupdateStatusVersion.
  void UpdateStatus(NSDictionary* status);

  // champion
  bool UpdateCheck();
  void GetUpdate();
  
    // Callback used to communicate update status to the client.
  StatusCallback status_callback_;
  
 protected:
  friend class VersionUpdater;

  // Clients must use VersionUpdater::Create().
  VersionUpdaterMac();
  virtual ~VersionUpdaterMac();

 private:
  // Update the visibility state of promote button.
  void UpdateShowPromoteButton();
  
  scoped_refptr<AviatorUpdater> update_launcher_; // champion


  // Callback used to show or hide the promote UI elements.
  PromoteCallback promote_callback_;

  // The visible state of the promote button.
  bool show_promote_button_;

  // The observer that will receive keystone status updates.
  base::scoped_nsobject<KeystoneObserver> keystone_observer_;

  DISALLOW_COPY_AND_ASSIGN(VersionUpdaterMac);
};

#endif  // CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_MAC_H_

