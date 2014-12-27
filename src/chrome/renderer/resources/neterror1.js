cr.define('neterror1', function() {
  'use strict';

  /**
   * Object for accessing localized strings.
   * @type {!LocalStrings}
   */
  /**
   * Be polite and insert translated hello world strings for the user on loading.
   */
  function initialize() {
 
	var state=0;
	
/* New error page hide fn for 28 start*/	
  var errCodeDisp = document.getElementById("error-code-disp");
  if(errCodeDisp.parentNode.id == "help-box-inner") {
      var errStatus = errCodeDisp.innerHTML;
      if(errStatus.indexOf("ERR_BLOCKED_BY_ADMINISTRATOR") != -1){
          document.getElementById("default_browser").style.display = "block";
          document.getElementById("error-code-disp").style.display = "none";
      }else{
          document.getElementById("default_browser").style.display = "none";
          document.getElementById("error-code-disp").style.display = "block";
      }
  }

/* New error page hide fn for 28 end*/		
    document.getElementById('default_browser').onclick = function(event) {
		var URL = templateData.summary.failedUrl;
		//alert(URL);
		var wnd = window.open(URL+':'+':Error138');
		
		setTimeout(function() {
		wnd.close();
		}, 100);
			return false;
	}

  }

  // Return an object with all of the exports.
  return {
    initialize: initialize,
  };
});

document.addEventListener('DOMContentLoaded', neterror.initialize);