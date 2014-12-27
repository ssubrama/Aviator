// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License.

/**
 * @fileoverview Base class for dialogs that require saving preferences on
 *     confirm and resetting preference inputs on cancel.
 */

cr.define('options', function() {
  /** @const */ var OptionsPage = options.OptionsPage;

  /**
   * Base class for settings dialogs.
   * @constructor
   * @param {string} name See OptionsPage constructor.
   * @param {string} title See OptionsPage constructor.
   * @param {string} pageDivName See OptionsPage constructor.
   * @param {HTMLInputElement} okButton The confirmation button element.
   * @param {HTMLInputElement} cancelButton The cancellation button element.
   * @extends {OptionsPage}
   */
  function SettingsDialog(name, title, pageDivName, okButton, cancelButton) {
    OptionsPage.call(this, name, title, pageDivName);
    this.okButton = okButton;
    this.cancelButton = cancelButton;
  }

  SettingsDialog.prototype = {
    __proto__: OptionsPage.prototype,

    /** @override */
    initializePage: function() {
      this.okButton.onclick = this.handleConfirm.bind(this);
      this.cancelButton.onclick = this.handleCancel.bind(this);
    },

    /**
     * Handles the confirm button by saving the dialog preferences.
     */
    handleConfirm: function() {
      OptionsPage.closeOverlay();

      var prefs = Preferences.getInstance();
      var els = this.pageDiv.querySelectorAll('[dialog-pref]');
      for (var i = 0; i < els.length; i++) {
        if (els[i].pref)
          prefs.commitPref(els[i].pref, els[i].metric);
      }
      location.reload(); // Champion:  Added to reset home page with default search engine when HOMEPAGE_URl is set as home page . Navneet
    },

    /**
     * Handles the cancel button by closing the overlay.
     */
    handleCancel: function() {
      OptionsPage.closeOverlay();

      var prefs = Preferences.getInstance();
      var els = this.pageDiv.querySelectorAll('[dialog-pref]');
      for (var i = 0; i < els.length; i++) {
        if (els[i].pref)
          prefs.rollbackPref(els[i].pref);
      }
    },
  };

  return {
    SettingsDialog: SettingsDialog
  };
});
