
cr.define('options.ReferrerHeader', function() {

  var OptionsPage = options.OptionsPage;

  //////////////////////////////////////////////////////////////////////////////
  // ReferrerImportHelpHeader class:

  /**
   * Encapsulated handling of content settings page.
   * @constructor
   */
  function ReferrerImportHelpHeader() {
    this.activeNavTab = null;
    OptionsPage.call(this, 'referrerImportHelpHeader', 'Import Help',
                     'referrer-import-help-header-manager');
  }

  cr.addSingletonGetter(ReferrerImportHelpHeader);

  ReferrerImportHelpHeader.prototype = {
    __proto__: OptionsPage.prototype,

    initializePage: function() {
      OptionsPage.prototype.initializePage.call(this);
	  
	  $('btnImportHelpOk').onclick = function(event) { 
		   OptionsPage.closeOverlay();
	  };
	  
	},
// manage exceptions page ---------------------------------------------------
	  
  }; 
  // Export
  return {
    ReferrerImportHelpHeader: ReferrerImportHelpHeader
  };

});