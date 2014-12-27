// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('options', function() {

  //////////////////////////////////////////////////////////////////////////////
  // ReferrerHeaderRadio class:

  // Define a constructor that uses an input element as its underlying element.
  var ReferrerHeaderRadio = cr.ui.define('input');

  ReferrerHeaderRadio.prototype = {
    __proto__: HTMLInputElement.prototype,

    /**
     * Initialization function for the cr.ui framework.
     */
    decorate: function() {
      this.type = 'radio';
      var self = this;

     this.addEventListener('change',
          function(e) {
          	//alert("referrer radio changed "); 
            chrome.send('setReferrerHeaderEnabled', [this.value == 'allow']);
          });
    },
  };

  // Export
  return {
    ReferrerHeaderRadio: ReferrerHeaderRadio    
  };

});

