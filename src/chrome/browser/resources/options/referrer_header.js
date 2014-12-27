// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('options', function() {

  var OptionsPage = options.OptionsPage;

  //////////////////////////////////////////////////////////////////////////////
  // ReferrerHeader class:

  /**
   * Encapsulated handling of content settings page.
   * @constructor
   */
  function ReferrerHeader() {
    this.activeNavTab = null;
    OptionsPage.call(this, 'referrer', loadTimeData.getString('referrerHeaderPageTabTitle'),
                     'referrer-header-page');
  }

  cr.addSingletonGetter(ReferrerHeader);

  ReferrerHeader.prototype = {
    __proto__: OptionsPage.prototype,

    initializePage: function() {
      OptionsPage.prototype.initializePage.call(this);
	  
	  $('ManageReferrerExceptionsButton').onclick = function(event) {
		
        OptionsPage.navigateToPage('ManageReferrerHeader');
		
      };
},
// manage exceptions page ---------------------------------------------------
	  
  };
  
  ReferrerHeader.updateReferrerHeaderRadios = function(enabled) {
    var selector = '#referrer-header-page input[type=radio][value=' +
        (enabled ? 'allow' : 'block') + '].referrer-radio';
    document.querySelector(selector).checked = true;
  };
  
  ReferrerHeader.setExceptions = function(type, list) {
    var exceptionsList =
        document.querySelector('div[contentType=' + type + ']' +
                               ' list[mode=normal]');
    exceptionsList.setExceptions(list);
  };

  ReferrerHeader.patternValidityCheckComplete =
      function(type, mode, pattern, valid) {
    var exceptionsList =
        document.querySelector('div[contentType=' + type + '] ' +
                               'list[mode=' + mode + ']');
    exceptionsList.patternValidityCheckComplete(pattern, valid);
  };
  // Export
  return {
    ReferrerHeader: ReferrerHeader
  };

});
