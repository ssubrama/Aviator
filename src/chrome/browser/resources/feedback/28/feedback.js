// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


// Constants.
/** @const */ var FEEDBACK_LANDING_PAGE ='https://www.whitehatsec.com/aviator/contactus/'; //block below for aviator feedback landing page champion
   // 'https://www.google.com/support/chrome/go/feedback_confirmation';
/** @const */ var MAX_ATTACH_FILE_SIZE = 3 * 1024 * 1024;

var selectedThumbnailDivId = '';
var selectedThumbnailId = '';
var selectedImageUrl;

var savedThumbnailIds = [];
savedThumbnailIds['current-screenshots'] = '';
savedThumbnailIds['saved-screenshots'] = '';

var categoryTag = '';
var filePath = '';
var forceDisableScreenshots = false;

// Globals to manage reading data from the attach a file option.
var attachFileBinaryData = '';
var lastReader = null;

var isdescriptionTextEmpty;
var ispageUrlTextEmpty;
var isuserEmailTextEmpty;
var warningMessage;


/**
 * Returns the base filename for a given path. Handles only Unix style paths.
 * @param {string} path The path to return the basename for.
 * @return {string} Basename for the path.
 */
function getBaseName(path) {
  lastSeparator = path.lastIndexOf('/');
  if (lastSeparator == -1)
    return '';
  else
    return path.substr(lastSeparator + 1);
}

/**
 * Selects an image thumbnail in the specified div.
 * @param {string} divId The id of the div to search in.
 * @param {string} thumbnailId The id of the thumbnail to search for.
 */
function selectImage(divId, thumbnailId) {
  var thumbnailDivs = $(divId).children;
  selectedThumbnailDivId = divId;
  if (thumbnailDivs.length == 0) {
    $(divId).hidden = true;
    return;
  }

  for (var i = 0; i < thumbnailDivs.length; i++) {
    thumbnailDivs[i].className = 'image-thumbnail-container';

    // If the the current div matches the thumbnail id provided,
    // or there is no thumbnail id given, and we're at the first thumbnail.
    if (thumbnailDivs[i].id == thumbnailId || (!thumbnailId && !i)) {
      thumbnailDivs[i].classList.add('image-thumbnail-container-selected');
      selectedThumbnailId = thumbnailDivs[i].id;
      savedThumbnailIds[divId] = thumbnailId;
    }
  }
}

/**
 * Adds an image thumbnail to the specified div.
 * @param {string} divId The id of the div to add a screenshot to.
 * @param {string} screenshot The URL of the screenshot being added.
 */
function addScreenshot(divId, screenshot) {
  var thumbnailDiv = document.createElement('div');
  thumbnailDiv.className = 'image-thumbnail-container';

  thumbnailDiv.id = divId + '-thumbnailDiv-' + $(divId).children.length;
  thumbnailDiv.onclick = function() {
    selectImage(divId, thumbnailDiv.id);
  };

  var innerDiv = document.createElement('div');
  innerDiv.className = 'image-thumbnail';

  var thumbnail = document.createElement('img');
  thumbnail.id = thumbnailDiv.id + '-image';
  // We add the ?+timestamp to make sure the image URLs are unique
  // and Chrome does not load the image from cache.
  thumbnail.src = screenshot + '?' + Date.now();
  innerDiv.appendChild(thumbnail);

  thumbnailDiv.appendChild(innerDiv);
  $(divId).appendChild(thumbnailDiv);

  if (!selectedThumbnailId)
    selectImage(divId, thumbnailDiv.id);
}

/**
 * Enables screenshots.
 */
function enableScreenshots() {
  if (forceDisableScreenshots)
    return;
  $('screenshot-row').hidden = false;
}

/**
 * Reads the selected file when the user selects a file.
 * @param {event} evtFileSelected The on changed event for the file input box.
 */
function onFileSelected(evtFileSelected) {
  var file = evtFileSelected.target.files[0];
  if (!file) {
    // User canceled file selection.
    $('attach-file-checkbox').checked = false;
    attachFileBinaryData = null;
    return;
  }

  if (file.size > MAX_ATTACH_FILE_SIZE) {
    $('attach-error').hidden = false;

    // Clear our selected file.
    $('attach-file').value = '';
    attachFileBinaryData = null;
    $('attach-file-checkbox').checked = false;

    return;
  }

  $('attach-error').hidden = true;

  // Abort an existing file read operation if one exists.
  if (lastReader) {
    lastReader.abort();
    lastReader = null;
  }

  var reader = new FileReader();
  reader.onloadend = function(evtLoadEnd) {
    if (evtLoadEnd.target.readyState == FileReader.DONE) {
      attachFileBinaryData = evtLoadEnd.target.result;
      lastReader = null;
      // Check the checkbox so we do send this file. Users can uncheck the
      // box if they don't want to send the file.
      $('attach-file-checkbox').checked = true;
      $('reading-file').hidden = true;
      $('send-report-button').disabled = false;
    }
  };

  lastReader = reader;
  reader.readAsBinaryString(file);
  $('reading-file').hidden = false;
  $('send-report-button').disabled = true;
}

/**
 * Sends the report; after the report is sent, we need to be redirected to
 * the landing page, but we shouldn't be able to navigate back, hence
 * we open the landing page in a new tab and sendReport closes this tab.
 * @return {boolean} True if the report was sent.
 */
function sendReport() {
  
  if(validateEmptyFileds())
  {
 	 if(warningMessage!="")
  	{
    	var resul=confirm(warningMessage);
    	if(!resul)
      		return false;
   	}
  }
  
  var imagePath = '';
  if ($('screenshot-checkbox').checked && selectedThumbnailId)
    imagePath = $(selectedThumbnailId + '-image').src;
  var pageUrl = $('page-url-text').value;
  if (!$('page-url-checkbox').checked)
    pageUrl = '';
  var userEmail = $('user-email-text').value;
  if (!$('user-email-checkbox').checked)
    userEmail = '';
  
 /* 
 //if ($('description-text').value.length == 0) {
   // alert(loadTimeData.getString('no-description'));
   // return false;
  //}

  var imagePath = '';
  if ($('screenshot-checkbox').checked && selectedThumbnailId)
    imagePath = $(selectedThumbnailId + '-image').src;
  var pageUrl = $('page-url-text').value;
  if (!$('page-url-checkbox').checked)
    pageUrl = '';
  var userEmail = $('user-email-text').value;
  if (!$('user-email-checkbox').checked)
    userEmail = '';

  var reportArray = [pageUrl,
                     categoryTag,
                     $('description-text').value,
                     userEmail,
                     imagePath];

  // Add chromeos data if it exists.
  if ($('sys-info-checkbox')) {
    reportArray = reportArray.concat([String($('sys-info-checkbox').checked)]);
  } 

  if ($('attach-file-checkbox') &&
      $('attach-file-checkbox').checked) {
    if (attachFileBinaryData) {
      reportArray = reportArray.concat(
          [$('attach-file').files[0].name, btoa(attachFileBinaryData)]);
    }
  } else if ($('attach-file-custom-checkbox') &&
             $('attach-file-custom-checkbox').checked) {
    if (filePath)
      reportArray = reportArray.concat([filePath, '']);
  } */

  // open the landing page in a new tab, sendReport will close this one.
  //window.open(FEEDBACK_LANDING_PAGE, '_blank'); champion
  //chrome.send('sendReport', reportArray);champion
  var problem;
  if ($('description-text').value.length != 0)
  	problem=$('description-text').value;
  else
    problem='';
  
  var postData = "";
  postData = postData + "&problem=" + problem;
  if (!$('page-url-checkbox').checked) {
 		postData = postData + "&include_url=no";
 	} else {
 		postData = postData + "&include_url=yes";
 	}
  postData = postData + "&url=" + pageUrl;
 	if (!$('user-email-checkbox').checked) {
 		postData = postData + "&include_email=no";
 	} else {
 		postData = postData + "&include_email=yes";
 	}
 	postData = postData + "&email=" + userEmail;
 	if (navigator.appVersion.indexOf("Mac")!=-1) {
 	postData = postData + "&aviator_version=MAC_2.6";
 	}
 	if (navigator.appVersion.indexOf("Win")!=-1) {
 	postData = postData + "&aviator_version=WIN_2.6";
 	}
 	if (!$('screenshot-checkbox').checked) {
 		postData = postData + "&include_screenshot=no";
 		postData = postData + "&screenshot_base64=";
 	} else {
 		postData = postData + "&include_screenshot=yes";
 		postData = postData + "&screenshot_base64=" + document.getElementById("screenshot-code").innerHTML;
 	}
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4) {
  		window.location = "https://www.whitehatsec.com/aviator/contactus/";
    }
	}
	xmlhttp.open("POST","https://staging.aviatorbrowser.com/cgi-bin/feedback.py",true); 
	//xmlhttp.open("POST","https://updates.aviatorbrowser.com/cgi-bin/feedback.py",true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send(postData);
}

function loadImage() {
    var input, file, fr, img;

    input = document.getElementById('imgfile');
    if (!input) {
   
    	return true;
    }
    else if (!input.files) {
    	return true;
    } else if (!input.files[0]) {
    	return true;
    } else {
		    var filenameSplit = input.value.split(".");
		  	if (filenameSplit[filenameSplit.length - 1].toUpperCase() != "PNG") {
		  		alert("File type not supported. Only PNG files can be attached.");
		  		input.value = "";
		      return true;
		    }
		    
		    file = input.files[0];
		    fr = new FileReader();
		    fr.onload = createImage;
		    fr.readAsDataURL(file);
    }
    function createImage() {
        img = new Image();
        img.onload = imageLoaded;
        img.src = fr.result;
    }

    function imageLoaded() {
        var canvas = document.createElement("canvas");
        canvas.style.visibility = "hidden";
        canvas.width = img.width;
        canvas.height = img.height;
        var ctx = canvas.getContext("2d");
        ctx.drawImage(img,0,0);
        document.getElementById("screenshot-code").innerHTML = canvas.toDataURL("image/png").replace("data:image/png;base64,", "");
    }
}

/**
 * Click listener for the cancel button.
 * @param {Event} e The click event being handled.
 */
function cancel(e) {
  chrome.send('cancel');
  e.preventDefault();
}

/**
 * Select the current screenshots div, restoring the image that was
 * selected when we had this div open previously.
 */
function currentSelected() {
  // TODO(rkc): Change this to use a class instead.
  $('current-screenshots').hidden = false;
  if ($('saved-screenshots'))
    $('saved-screenshots').hidden = true;

  if (selectedThumbnailDivId != 'current-screenshots')
    selectImage('current-screenshots',
                savedThumbnailIds['current-screenshots']);
}

/**
 * Select the saved screenshots div, restoring the image that was
 * selected when we had this div open previously.
 */
function savedSelected() {
  if ($('saved-screenshots').childElementCount == 0) {
    // setupSavedScreenshots will take care of changing visibility
    chrome.send('refreshSavedScreenshots');
  } else {
    $('current-screenshots').hidden = true;
    $('saved-screenshots').hidden = false;
    if (selectedThumbnailDivId != 'saved-screenshots')
      selectImage('saved-screenshots', savedThumbnailIds['saved-screenshots']);
  }
}

/**
 * Change the type of screenshot we're showing to the user from
 * the current screenshot to saved screenshots
 */
function changeToSaved() {
  $('screenshot-label-current').hidden = true;
  $('screenshot-label-saved').hidden = false;

  // Change the link to say "go to original"
  $('screenshot-link-tosaved').hidden = true;
  $('screenshot-link-tocurrent').hidden = false;

  savedSelected();
}

/**
 * Change the type of screenshot we're showing to the user from
 * the saved screenshots to the current screenshots
 */
function changeToCurrent() {
  $('screenshot-label-current').hidden = false;
  $('screenshot-label-saved').hidden = true;

  // Change the link to say "go to saved"
  $('screenshot-link-tosaved').hidden = false;
  $('screenshot-link-tocurrent').hidden = true;

  currentSelected();
}

///////////////////////////////////////////////////////////////////////////////
// Document Functions:
/**
 * Window onload handler, sets up the page.
 */
function load() {
  if ($('attach-file'))
    $('attach-file').addEventListener('change', onFileSelected);

//  if ($('sysinfo-url')) {
  //  $('sysinfo-url').onclick = function(event) {
 //     chrome.send('openSystemTab');
 //   };
//  }

//<if expr="pp_ifdef('chromeos')">
//  $('screenshot-link-tosaved').onclick = changeToSaved;
//  $('screenshot-link-tocurrent').onclick = changeToCurrent;
//</if>
  $('send-report-button').onclick = sendReport;
  $('cancel-button').onclick = cancel;

  // Set default values for the possible parameters, and then parse the actual
  // values from the URL href.
  var parameters = {
    'description': '',
    'categoryTag': '',
    'customPageUrl': '',
    'filePath': '',
  };

  var loc = window.location;
  // Split the query string into an array of parameters.
  var query = loc.search.substr(1).split('&');
  // If we have a query in the hash.
  if (loc.hash.indexOf('?') >= 0) {
    // Remove the hash and split this query into parameters too.
    query = query.concat(loc.hash.substr(loc.hash.indexOf('?') + 1).split('&'));
  }
  for (var i = 0; i < query.length; i++) {
    // Decode and store each parameter value.
    parameter = query[i].split('=');
    parameters[parameter[0]] = decodeURIComponent(parameter[1]);
  }

  // Set the initial description text.
  $('description-text').textContent = parameters['description'];
  // If a page url is spcified in the parameters, override the default page url.
  if (parameters['customPageUrl'] != '') {
    $('page-url-text').value = parameters['customPageUrl'];
    // and disable the page image, since it doesn't make sense on a custom url.
    $('screenshot-checkbox').checked = false;
    forceDisableScreenshots = true;
  }

  // Pick up the category tag (for most cases this will be an empty string)
  categoryTag = parameters['categoryTag'];

  // Pick up the file path for the attached file (only user for this at the
  // moment is the quick office extension).
  filePath = parameters['filePath'];

  if (filePath != '') {
    var baseName = getBaseName(filePath);
    if (baseName) {
      // Don't let the user choose another file, we were invoked by an
      // extension already providing us the file, this report should only
      // attach that file, or no file at all.
      $('attach-file-container').hidden = true;

      // Set our filename and unhide the "Attach this file" span.
      $('attach-file-custom-name').textContent = baseName;
      $('attach-file-custom-container').hidden = false;
      // No screenshots if we're being invoked by an extension - screenshot was
      // never taken.
      $('screenshot-checkbox').checked = false;
      forceDisableScreenshots = true;
    } else {
      filePath = '';
    }
  }

  chrome.send('getDialogDefaults');
  chrome.send('refreshCurrentScreenshot');
}

function setupCurrentScreenshot(screenshot) {
  addScreenshot('current-screenshots', screenshot);
}

function setupSavedScreenshots(screenshots) {
  if (screenshots.length == 0) {
    $('saved-screenshots').textContent =
        loadTimeData.getString('no-saved-screenshots');

    // Make sure we make the display the message.
    $('current-screenshots').hidden = true;
    $('saved-screenshots').hidden = false;

    // In case the user tries to send now; fail safe, do not send a screenshot
    // at all versus sending the current screenshot.
    selectedThumbnailDivId = '';
    selectedThumbnailId = '';
  } else {
    $('saved-screenshots').textContent = '';
    for (i = 0; i < screenshots.length; ++i)
      addScreenshot('saved-screenshots', screenshots[i]);

    // Now that we have our screenshots, try selecting the saved screenshots
    // again.
    savedSelected();
  }
}

function setupDialogDefaults(defaults) {
  // Current url.
  if ($('page-url-text').value == '')
    $('page-url-text').value = defaults.currentUrl;
  if (defaults.currentUrl == '')
    $('page-url-checkbox').checked = false;
  // User e-mail.
  $('user-email-text').value = defaults.userEmail;
  $('user-email-checkbox').checked = defaults.emailCheckboxDefault;

  document.documentElement.classList.toggle('launcher-layout',
                                            defaults.launcherFeedback);

  if (!defaults.disableScreenshots)
    enableScreenshots();

  if (defaults.useSaved) {
    $('screenshot-link-tosaved').hidden = false;
  }
}

function validateEmptyFileds()
{
  warningMessage="";
  var isScreenShotAdded=document.getElementById('imgfile').files.length;
 if ($('description-text').value=='')
    isdescriptionTextEmpty=true;
 else
   isdescriptionTextEmpty=false;
   
 if($('page-url-text').value=='')
 	 ispageUrlTextEmpty=true;
 else
 	ispageUrlTextEmpty=false;
 
 if($('user-email-text').value=='')
  	isuserEmailTextEmpty=true;
 else
 	isuserEmailTextEmpty=false;
 
 if( isdescriptionTextEmpty && ispageUrlTextEmpty && isuserEmailTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - URL\n - Email\n - Include this screenshot\n Still do you want to continue?.");

 else if( isdescriptionTextEmpty && ispageUrlTextEmpty && isuserEmailTextEmpty)
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - URL\n - Email\n Still do you want to continue?.");
 
 else if( isdescriptionTextEmpty && ispageUrlTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - URL\n - Include this screenshot\n Still do you want to continue?.");
  
 else if( isdescriptionTextEmpty && isuserEmailTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - Email\n - Include this screenshot\n Still do you want to continue?.");

 else if( ispageUrlTextEmpty && isuserEmailTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - URL\n - Email\n - Include this screenshot\n Still do you want to continue?.");
 
 else if( isdescriptionTextEmpty && ispageUrlTextEmpty )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - URL\n Still do you want to continue?.");

 else if( isdescriptionTextEmpty && isuserEmailTextEmpty )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - Email\n Still do you want to continue?.");

 else if( isdescriptionTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Tell us what is happening\n - Include this screenshot\n Still do you want to continue?.");
 
 else if(ispageUrlTextEmpty && isuserEmailTextEmpty )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - URL\n - Email\n Still do you want to continue?.");
 
 else if(ispageUrlTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - URL\n - Include this screenshot\n Still do you want to continue?.");
 
 else if(isuserEmailTextEmpty && !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following fields are Empty:\n - Email\n - Include this screenshot\n Still do you want to continue?.");		

 else if( isdescriptionTextEmpty)
 	warningMessage=warningMessage.concat("The following fields is Empty:\n - Tell us what is happening\n Still do you want to continue?.");

 else if(ispageUrlTextEmpty)
 	warningMessage=warningMessage.concat("The following field is Empty:\n - URL\n Still do you want to continue?.");
 
 else if(isuserEmailTextEmpty )
 	warningMessage=warningMessage.concat("The following field is Empty:\n - Email\n Still do you want to continue?.");
 
 else if( !isScreenShotAdded )
 	warningMessage=warningMessage.concat("The following field is Empty:\n - Include this screenshot\n Still do you want to continue?.");
  
 if(warningMessage!=null)
   	return true;
 else
  	return false;
 	
 }

window.addEventListener('DOMContentLoaded', load);
