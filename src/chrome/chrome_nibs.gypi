# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# This gypi file contains lists of XIB files that are used by Chromium. The
# lists are divided by those files that need to be run through the localizer
# tool and those that do not. A XIB should be listed in either one or the
# other, but not both.
{
  'variables': {
    'mac_translated_xibs': [
      'app/nibs/AvatarMenuItem.xib',
      'app/nibs/BookmarkAllTabs.xib',
      'app/nibs/BookmarkBar.xib',
      'app/nibs/BookmarkBubble.xib',
      'app/nibs/BookmarkEditor.xib',
      'app/nibs/BookmarkNameFolder.xib',
      'app/nibs/CollectedCookies.xib',
      'app/nibs/ContentBlockedCookies.xib',
      'app/nibs/ContentBlockedDownloads.xib',
      'app/nibs/ContentBlockedMIDISysEx.xib',
      'app/nibs/ContentBlockedMedia.xib',
      'app/nibs/ContentBlockedMixedScript.xib',
      'app/nibs/ContentBlockedPlugins.xib',
      'app/nibs/ContentBlockedPopups.xib',
      'app/nibs/ContentBlockedGeolocation.xib',
      'app/nibs/ContentBlockedSimple.xib',
      'app/nibs/ContentProtocolHandlers.xib',
      'app/nibs/CookieDetailsView.xib',
      'app/nibs/DownloadItem.xib',
      'app/nibs/DownloadShelf.xib',
      'app/nibs/EditSearchEngine.xib',
      'app/nibs/ExtensionInstallPrompt.xib',
      'app/nibs/ExtensionInstallPromptBundle.xib',
      'app/nibs/ExtensionInstallPromptNoWarnings.xib',
      'app/nibs/ExtensionInstallPromptWebstoreData.xib',
      'app/nibs/ExtensionInstalledBubble.xib',
      'app/nibs/FirstRunBubble.xib',
      'app/nibs/FirstRunDialog.xib',
      'app/nibs/FullscreenExitBubble.xib',
      'app/nibs/HttpAuthLoginSheet.xib',
      'app/nibs/HungRendererDialog.xib',
      'app/nibs/ImportProgressDialog.xib',
      'app/nibs/UnprotectedBubble.xib', # Champion Added for Unprotected Bubble (Balaji)
      'app/nibs/ProtectedBubble.xib',   # Champion Added for Protected   Bubble (Balaji)
      'app/nibs/MainMenu.xib',
      'app/nibs/OneClickSigninBubble.xib',
      'app/nibs/OneClickSigninDialog.xib',
      'app/nibs/Quit.xib', #Champion Added for Warn before Quit
      'app/nibs/QuitWarn.xib', #Champion Added for Warn before Quit
      'app/nibs/RestartAviator.xib', # Champion Added for Restart Aviator Browser for Update (Balaji)
      'app/nibs/SaveAccessoryView.xib',
      'app/nibs/TaskManager.xib',
      'app/nibs/Toolbar.xib',
      'app/nibs/WrenchMenu.xib',
    ],  # mac_translated_xibs
    'mac_untranslated_xibs': [
      'app/nibs/AboutIPC.xib',
      'app/nibs/ActionBoxMenuItem.xib',
      'app/nibs/BookmarkBarFolderWindow.xib',
      'app/nibs/ExtensionInstalledBubbleBundle.xib',
      'app/nibs/FindBar.xib',
      'app/nibs/GlobalErrorBubble.xib',
      'app/nibs/HungRendererDialog.xib',
      'app/nibs/InfoBar.xib',
      'app/nibs/Panel.xib',
      'app/nibs/SadTab.xib',
    ],  # mac_untranslated_xibs
    'mac_all_xibs': [
      '<@(mac_translated_xibs)',
      '<@(mac_untranslated_xibs)',
    ],  # mac_all_xibs
  },  # variables
}
