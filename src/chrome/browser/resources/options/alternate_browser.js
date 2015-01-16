// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
//in this file are licensed under the BSD-3-clause license (the "License") included in 
//the WhiteHat-LICENSE file included in the root directory of the distributed source code 
//archive. Unless required by applicable law or agreed to in writing, software distributed
//under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


cr.define('options', function() {

	var OptionsPage = options.OptionsPage;
	//////////////////////////////////////////////////////////////////////////////
	// AlternateBrowser class:

	/**
	 * Encapsulated handling of Connection Control page.
	 * @constructor
	 */
	 	
	 /*function SearchEngineManager() {
    this.activeNavTab = null;
    OptionsPage.call(this, 'searchEngines',
                     loadTimeData.getString('searchEngineManagerPageTabTitle'),
                     'search-engine-manager-page');
  }*/
	
	function AlternateBrowser() {
		this.activeNavTab = null;
		OptionsPage.call(this, 'alternateBrowser','', 'alternateBrowser');
	}

	
	cr.addSingletonGetter(AlternateBrowser); 

	AlternateBrowser.prototype = {
		__proto__ : OptionsPage.prototype,
 
		initializePage : function() {
			OptionsPage.prototype.initializePage.call(this);
			$('blocked-url-browser-popup').onchange = function(){
					var browserSelect = $('blocked-url-browser-popup'); 
		    	var selectedIndex = browserSelect.selectedIndex;
		    	if(selectedIndex >= 0){  
			    	var selection = browserSelect.options[selectedIndex];			    
	    		//chrome.send('setBlockedURLAlternateBrowserInPrefs',[String(selection.value)]);
	    		chrome.send('setBlockedURLAlternateBrowserInPrefs',[String(selection.innerHTML)]);
	    		
    			} 
			}; 
			$('alternate-browser-confirm').onclick = function() {
					var browserSelect = $('blocked-url-browser-popup'); 
		    	var selectedIndex = browserSelect.selectedIndex;
		    	if(selectedIndex >= 0){  
			    	var selection = browserSelect.options[selectedIndex];	
			    	//alert(String(selection.innerHTML));		    
	    		 chrome.send('setBlockedURLAlternateBrowserInPrefs',[String(selection.innerHTML)]);
    			} 
    			
    			/******** For refereshing the page after clicking the connection control close button*****/
    				 setTimeout("console.log('***************');location.reload(true);",100);
    				 //location.reload(true);
             OptionsPage.closeOverlay();
      };
		},
	};
	
	/**** Added for CHAMPION: for showing the browser installed in system******/
	AlternateBrowser.InitBrowserList = function(Browserlist, alternateBrowser){
			var defaultOptionsValue= '';
			 var browserSelect = document.getElementById("blocked-url-browser-popup");   
				for(i=0;i<Browserlist.length;i++){
					browserSelect.add(new Option(Browserlist[i], i), browserSelect.options[i]);
					
					if(browserSelect.options[i].innerHTML == alternateBrowser){
						defaultOptionsValue = browserSelect.options[i].value;					
					}
				}
				//browserSelect.selectedIndex.value = alternateBrowser;	
				browserSelect.selectedIndex = defaultOptionsValue;	
				//OptionsPage.navigateToPage('alternateBrowser');    
	};
	
	/**** Added for CHAMPION: for showing the browser installed in system******/
	AlternateBrowser.setUpBlockedURLAlternateBrowser = function(alternateBrowser)
	{ 
		//alert("alternateBrowser is "+alternateBrowser);
			var defaultOptionsValue= '';
			 var browserSelect = document.getElementById("blocked-url-browser-popup");   
					browserSelect.selectedIndex = alternateBrowser;	
	};
	AlternateBrowser.SetAsdDefaultBrowserClicked = function() // 10-12-2012
	{ 
		 
			OptionsPage.navigateToPage('alternateBrowser');    
	};
	
	return {
		AlternateBrowser : AlternateBrowser
	};

});
