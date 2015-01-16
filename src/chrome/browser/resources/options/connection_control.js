// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

cr.define('options', function () {

  var OptionsPage = options.OptionsPage;
  var flag;
  var oRow;
  //////////////////////////////////////////////////////////////////////////////
  // ConnectionControl class:

  /**
   * Encapsulated handling of Connection Control page.
   * @constructor
   */
  function ConnectionControl() {
    this.activeNavTab = null;
    OptionsPage.call(this, 'NetworkPortConnection', 'options - Network Connection Control', 'connection-control-page');
  }


  cr.addSingletonGetter(ConnectionControl);

  ConnectionControl.prototype = {
    __proto__: OptionsPage.prototype,

    initializePage: function () {
      OptionsPage.prototype.initializePage.call(this);
      /*Code starts for ADD rule button for ravi request*/
      //document.getElementById('new').style.display = "none";
      $('new').onclick = function (event) {
        var a = window.outerHeight + 'px';
        $("addNtlmURL").style.display = 'none';
        document.getElementById('addPage').style.display = "block";
        document.getElementById('heading3').innerHTML = 'Add Rule';
        document.getElementById('rule').value = "OK";
        document.getElementById("protocol").selectedIndex = 0;
        document.getElementById("conn").selectedIndex = 0;
        document.getElementById("server").selectedIndex = 0;
        document.getElementById("portText").value = "";
        document.getElementById("valueText").value = "";
        document.getElementById('hideDiv').style.display = "none";
        document.getElementById('enable').checked = false;
        flag = 1;
        var table = document.getElementById('tableId');
        var rowsCount = table.rows.length;
        if (rowsCount > 8) {
          document.getElementById('connection-control-page').style.overflow = "auto";
        } else document.getElementById('connection-control-page').style.overflow = "visible";
      };
      /*Code ends for ADD rule button*/
      $('cancel').onclick = function (event) {
        document.getElementById('addPage').style.display = "none";

      };
      $('closeButton').onclick = function (event) {
        document.getElementById('addPage').style.display = "none";
      };

      /******** For refereshing the page after clicking the connection control close button*****/
      $('closeBtn').onclick = function (event) {
        OptionsPage.closeOverlay();
      };

 /*     $('default_chk').onclick = function (event) {
        var bool_default_chk = document.getElementById('default_chk').checked;
        chrome.send('SetLoadInDefaultBrowser', [bool_default_chk]);
      }; */

      /*** Added for Champion
      ***** alternate browser Options js start here***/
/*      $('blocked-url-browser').onchange = function () {
        var browserSelect = $('blocked-url-browser');
        var selectedIndex = browserSelect.selectedIndex;
        if (selectedIndex >= 0) {
          var selection = browserSelect.options[selectedIndex];
          chrome.send('setBlockedURLAlternateBrowserInPrefs', [String(selection.innerHTML)]);
        }
      };*/

      $('rule').onclick = function (event) {
        if (flag == 1) {
          var conn = document.getElementById("conn").selectedIndex;
          var connval = document.getElementById("conn")[conn].value;
          var ser = document.getElementById("server").selectedIndex;
          var serval = document.getElementById("server")[ser].value;
          var val = document.getElementById("valueText").value;
          var port = document.getElementById("portText").value;
          var proto = document.getElementById("protocol").selectedIndex;
          var protoval = document.getElementById("protocol")[proto].value;
          var enab = document.getElementById('enable').checked;
          var msg = connval + " " + protoval + " connection to " + serval;

          if ((proto == 0) || (ser == 0)) {
            alert('Please select a value for Protocol and Server');
            return false;
          }
          //addded for http=80 and https = 443 
          if (((protoval == "http") && (port == "443")) || ((protoval == "https") && (port == "80"))) {
            alert("Port value for http is 80 and https is 443");
            return false;
          }
          if (port == '' || port == null) {
            alert('Please enter a value for port');
            return false;
          }
          /*Added for port validation*/
          if (port > 65535 || port <= 0) {
            alert('Please enter a Valid value for port.Port should < 65535 ');
            return false;
          }
          if (ser == 1 || ser == 2 || ser == 3) {
            if (val == '' || val == null) {
              alert('Fields cannot be empty.');
              return false;
            } else if ((val != '' || val != null) && (port == '' || port == null)) {
              if (ser == 3) {
                var ipaddress = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/;
                if (val.match(ipaddress) == null) {
                  alert("Please enter valid IP Address");
                  return false;
                }
              } else if (ser == 2) {
                var hostname = /^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$/;
                if (val.match(hostname) == null) {
                  alert("Please enter valid HostName");
                  return false;
                }
              } else if (ser == 1) {
                var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
                if (val.match(domain) == null) {
                  alert("Please enter valid Domain");
                  return false;
                }
              }
              var msg = connval + " " + protoval + " connection to " + val;
            } else if ((val != '' || val != null) && (port != '' || port != null)) {
              if (ser == 3) {
                var ipaddress = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/;
                if (val.match(ipaddress) == null) {
                  alert("Please enter valid IP Address");
                  return false;
                }
              } else if (ser == 2) {
                var hostname = /^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$/;
                if (val.match(hostname) == null) {
                  alert("Please enter valid HostName");
                  return false;
                }
              } else if (ser == 1) {
                var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
                if (val.match(domain) == null) {
                  alert("Please enter valid Domain");
                  return false;
                }
              }
              var patt = /^(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0)((,)(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0))*$/g;
              if (port.match(patt) == null) {
                alert("Please enter valid port number");
                return false;
              }
              var msg = connval + " " + protoval + " connection on port " + port + " to " + val;
            }
          } else if (ser == 4 || ser == 5) {
            if (port == '' || port == null) {
              var msg = connval + " " + protoval + " connection to " + serval;
            } else {
              var patt = /^(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0)((,)(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0))*$/g;
              if (port.match(patt) == null) {
                alert("Please enter valid port number");
                return false;
              }
              var msg = connval + " " + protoval + " connection on port " + port + " to " + serval;
            }
          }

          var table = document.getElementById('tableId');
          var rowCount = table.rows.length;
          var b = msg.split(" ");
          var c = "";
          var d = "";
          var comma = /,/g;
          var digit_five = /^\d{1,5}$/;

          if (b[5] != undefined) {
            if (b[5].match(comma)) {
              var ports = b[5].split(",");
              var portlength = ports.length;
              for (i = 1; i < b.length; i++) {
                if (i == 5) { continue; }
                else { c += b[i] + " "; }
              }
              for (j = 1; j < rowCount; j++) {
                var str1 = table.rows[j].cells[1].innerHTML;
                var str2 = str1.split(" ");
                for (p = 1; p < str2.length; p++) {
                  if (p == 5) { continue; }
                  else { d += str2[p] + " "; }
                }
                if (str2[5] != undefined) {
                  if (str2[5].match(comma)) {
                    var tabports = str2[5].split(",");
                    var tabportlength = ports.length;
                    for (k = 0; k < portlength; k++) {
                      for (m = 0; m <= tabportlength; m++) {
                        if ((c == d) && b[0] != str2[0]) { // added for Contradict rule check
                          alert("Contradict rule");
                          return false;
                        } else if ((ports[k] == [m]) && (c == d)) {
                          alert("Rule already exists");
                          return false;
                        }
                      }
                    }
                  }
                  else {
                    for (k = 0; k < portlength; k++) {
                      if ((ports[k] == str2[5]) && (c == d)) {
                        alert("Rule already exists");
                        return false;
                      }
                    }
                  }
                } else { continue; }
                d = "";
              }

            } else if (b[5].match(digit_five)) {
              var ports = b[5].split(",");
              var portlength = ports.length;
              for (i = 1; i < b.length; i++) {
                if (i == 5) { continue; }
                else { c += b[i] + " "; }
              }
              for (j = 1; j < rowCount; j++) {
                var str1 = table.rows[j].cells[1].innerHTML;
                var str2 = str1.split(" ");
                for (p = 1; p < str2.length; p++) {
                  if (p == 5) { continue; }
                  else { d += str2[p] + " "; }
                }
                if (str2[5] != undefined) {
                  if (str2[5].match(comma)) {
                    var tabports = str2[5].split(",");
                    var tabportlength = ports.length;
                    for (m = 0; m <= tabportlength; m++) {
                      if ((c == d) && b[0] != str2[0]) { // added for Contradict rule check
                        alert("Contradict rule");
                        return false;
                      } else if ((tabports[m] == b[5]) && (c == d)) {
                        alert("Rule already exists");
                        return false;
                      }
                    }
                  } else {
                    var tabports = str2[5].split(",");
                    var tabportlength = ports.length;
                    for (k = 0; k < tabportlength; k++) {
                      if ((c == d) && b[0] != str2[0]) {  // added for Contradict rule check
                        alert("Contradict rule");
                        return false;
                      } else if ((tabports[k] == b[5]) && (c == d)) {
                        alert("Rule already exists");
                        return false;
                      }
                    }
                  }
                } else { continue; }
                d = "";
              }
            } else {
              for (i = 1; i < b.length; i++) {
                c += b[i] + " ";
              }
              for (j = 1; j < rowCount; j++) {
                var str1 = table.rows[j].cells[1].innerHTML;
                var str2 = str1.split(" ");
                var str3 = ""
                for (i = 1; i < str2.length; i++) {
                  str3 += str2[i] + " ";
                }
                if (c == str3) {
                  alert("Rule already exists");
                  return false;
                }

              }
            }
          } else {
            for (i = 1; i < b.length; i++) {
              c += b[i] + " ";
            }
            for (j = 1; j < rowCount; j++) {
              var str1 = table.rows[j].cells[1].innerHTML;
              var str2 = str1.split(" ");
              var str3 = ""
              for (i = 1; i < str2.length; i++) {
                str3 += str2[i] + " ";
              }
              if (c == str3) {
                alert("Rule already exists");
                return false;
              }
            }
          }
          var row = table.insertRow(rowCount);

          var cell1 = row.insertCell(0);
          cell1.innerHTML = "";

          var cell2 = row.insertCell(1);
          cell2.textContent = msg;

          var cell3 = row.insertCell(2);
          var element2 = document.createElement("span");
          element2.className = "blue edit";
          element2.setAttribute("onClick", "edit(this);");
          element2.innerHTML = 'Edit';
          cell3.appendChild(element2);

          var cell4 = row.insertCell(3);
          var element3 = document.createElement("span");
          element3.className = "blue";
          if (enab == true) {
            element3.innerHTML = 'Disable';
            row.className = "";
            cell1.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
          } else {
            element3.innerHTML = 'Enable';
            row.className = "disableTableRow";
            cell1.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
          }
          element3.setAttribute("onClick", "enable(this);");
          cell4.appendChild(element3);

          var cell5 = row.insertCell(4);
          var element4 = document.createElement("span");
          element4.className = "blue";
          element4.innerHTML = 'Delete';
          element4.setAttribute("onClick", "deleteRule(this);");
          cell5.appendChild(element4);
          document.getElementById('addPage').style.display = "none";
          chrome.send('AddConnectionRules', [enab, msg]);
        } else {
          var conn = document.getElementById("conn").selectedIndex;
          var connval = document.getElementById("conn")[conn].value;
          var ser = document.getElementById("server").selectedIndex;
          var serval = document.getElementById("server")[ser].value;
          var val = document.getElementById("valueText").value;
          var port = document.getElementById("portText").value;
          var proto = document.getElementById("protocol").selectedIndex;
          var protoval = document.getElementById("protocol")[proto].value;
          var enab = document.getElementById('enable').checked;
          var msg = connval + " " + protoval + " connection to " + serval;
          if ((proto == 0) || (ser == 0)) {
            alert('Please select a value for Protocol and Server');
            return false;
          }
          if (((protoval == "http") && (port == "443")) || ((protoval == "https") && (port == "80"))) {
            alert("Port value for http is 80 and https is 443");
            return false;
          }
          if (port == '' || port == null) {
            alert('Please enter a value for Port');
            return false;
          }
          if (ser == 1 || ser == 2 || ser == 3) {
            if (val == '' || val == null) {
              alert('Fields cannot be empty');
              return false;
            } else if ((val != '' || val != null) && (port == '' || port == null)) {
              if (ser == 3) {
                var ipaddress = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/;
                if (val.match(ipaddress) == null) {
                  alert("Please enter valid IP Address");
                  return false;
                }
              } else if (ser == 2) {
                var hostname = /^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$/;
                if (val.match(hostname) == null) {
                  alert("Please enter valid HostName");
                  return false;
                }
              } else if (ser == 1) {
                var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
                if (val.match(domain) == null) {
                  alert("Please enter valid Domain");
                  return false;
                }
              }

              var msg = connval + " " + protoval + " connection to " + val;
            } else if ((val != '' || val != null) && (port != '' || port != null)) {
              if (ser == 3) {
                var ipaddress = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/;
                if (val.match(ipaddress) == null) {
                  alert("Please enter valid IP Address");
                  return false;
                }
              } else if (ser == 2) {
                var hostname = /^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$/;
                if (val.match(hostname) == null) {
                  alert("Please enter valid HostName");
                  return false;
                }
              } else if (ser == 1) {
                var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
                if (val.match(domain) == null) {
                  alert("Please enter valid Domain");
                  return false;
                }
              }
              var patt = /^(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0)((,)(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0))*$/g;
              if (port.match(patt) == null) {
                alert("Please enter valid port number");
                return false;
              }
              var msg = connval + " " + protoval + " connection on port " + port + " to " + val;
            }
          } else if (ser == 4 || ser == 5) {
            if (port == '' || port == null) {
              var msg = connval + " " + protoval + " connection to " + serval;
            } else {
              var patt = /^(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0)((,)(6553[0-5]|655[0-2]\d|65[0-4]\d\d|6[0-4]\d{3}|[1-5]\d{4}|[1-9]\d{0,3}|0))*$/g;
              if (port.match(patt) == null) {
                alert("Please enter valid port number");
                return false;
              }
              var msg = connval + " " + protoval + " connection on port " + port + " to " + serval;
            }
          }


          var table = document.getElementById('tableId');
          var rowCount = table.rows.length;
          var b = msg.split(" ");
          var c = "";
          var d = "";
          var comma = /,/g;
          var digit_five = /^\d{1,5}$/;
          if (b[5] != undefined) {
            if (b[5].match(comma)) {
              var ports = b[5].split(",");
              var portlength = ports.length;
              for (i = 1; i < b.length; i++) {
                if (i == 5) { continue; }
                else { c += b[i] + " "; }
              }
              for (j = 1; j < rowCount; j++) {
                if (j == oRow.rowIndex) { continue; }
                else {
                  var str1 = table.rows[j].cells[1].innerHTML;
                  var str2 = str1.split(" ");
                  for (p = 1; p < str2.length; p++) {
                    if (p == 5) { continue; }
                    else { d += str2[p] + " "; }
                  }
                  if (str2[5] != undefined) {
                    if (str2[5].match(comma)) {
                      var tabports = str2[5].split(",");
                      var tabportlength = ports.length;
                      for (k = 0; k < portlength; k++) {
                        for (m = 0; m <= tabportlength; m++) {
                          if ((ports[k] == tabports[m]) && (c == d)) {
                            alert("Rule already exists");
                            return false;
                          }
                        }
                      }
                    } else {
                      for (k = 0; k < portlength; k++) {
                        if ((ports[k] == str2[5]) && (c == d)) {
                          alert("Rule already exists");
                          return false;
                        }
                      }
                    }
                  } else { continue; }
                  d = "";
                }
              }
            } else if (b[5].match(digit_five)) {
              var ports = b[5].split(",");
              var portlength = ports.length;
              for (i = 1; i < b.length; i++) {
                if (i == 5) { continue; }
                else { c += b[i] + " "; }
              }
              for (j = 1; j < rowCount; j++) {
                if (j == oRow.rowIndex) { continue; }
                else {
                  var str1 = table.rows[j].cells[1].innerHTML;
                  var str2 = str1.split(" ");
                  for (p = 1; p < str2.length; p++) {
                    if (p == 5) { continue; }
                    else { d += str2[p] + " "; }
                  }
                  if (str2[5] != undefined) {
                    if (str2[5].match(comma)) {
                      var tabports = str2[5].split(",");
                      var tabportlength = ports.length;
                      for (m = 0; m <= tabportlength; m++) {
                        if ((c == d) && b[0] != str2[0]) {
                          alert("Contradicst Rule");
                          return false;
                        } else if ((tabports[m] == b[5]) && (c == d)) {
                          alert("Rule already exists");
                          return false;
                        }
                      }
                    } else {
                      var tabports = str2[5].split(",");
                      var tabportlength = ports.length;
                      for (k = 0; k < tabportlength; k++) {
                        if ((c == d) && b[0] != str2[0]) {
                          alert("Contradicst Rule");
                          return false;
                        } else if ((tabports[k] == b[5]) && (c == d)) {
                          alert("Rule already exists");
                          return false;
                        }
                      }
                    }
                  } else { continue; }
                  d = "";
                }
              }
            } else {
              for (i = 1; i < b.length; i++) {
                c += b[i] + " ";
              }
              for (j = 1; j < rowCount; j++) {
                if (j == oRow.rowIndex) { continue; }
                else {
                  var str1 = table.rows[j].cells[1].innerHTML;
                  var str2 = str1.split(" ");
                  var str3 = ""
                  for (i = 1; i < str2.length; i++) {
                    str3 += str2[i] + " ";
                  }
                  if (c == str3) {
                    alert("Rule already exists");
                    return false;
                  }
                }
              }
            }
          } else {
            for (i = 1; i < b.length; i++) {
              c += b[i] + " ";
            }
            for (j = 1; j < rowCount; j++) {
              if (j == oRow.rowIndex) { continue; }
              else {
                var str1 = table.rows[j].cells[1].innerHTML;
                var str2 = str1.split(" ");
                var str3 = ""
                for (i = 1; i < str2.length; i++) {
                  str3 += str2[i] + " ";
                }
                if (c == str3) {
                  alert("Rule already exists");
                  return false;
                }
              }
            }
          }
          oRow.cells[1].textContent = msg;
          if (enab == true) {
            oRow.cells[3].innerHTML = '<span class="blue" onclick="enable(this);">Disable</span>';
            oRow.className = "";
            oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
          } else {
            oRow.cells[3].innerHTML = '<span class="blue" onclick="enable(this);">Enable</span>';
            oRow.className = "disableTableRow";
            oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
          }
          chrome.send('EditConnectionRules', [enab, oRow.rowIndex, msg]);
        }
        document.getElementById('addPage').style.display = "none";
      };

      $('server').onchange = function (event) {
        var val = String(event.target.options[event.target.selectedIndex].value);
        switch (val) {
          case 'Domain':
            document.getElementById('hideValue').innerText = 'Domain';
            break;
          case 'Hostname':
            document.getElementById('hideValue').innerText = 'Host';
            break;
          case 'IP Address':
            document.getElementById('hideValue').innerText = 'IP';
            break;
        }
        if (val == 'Domain' || val == 'Hostname' || val == 'IP Address') {
          document.getElementById('hideDiv').style.display = "block";
        } else
          document.getElementById('hideDiv').style.display = "none";
      };


      //---------------------- NTLM White URL ----------------------
      $('btnAddNtlmRule').onclick = function () {
        $("addPage").style.display = 'none';
        $("addNtlmURL").style.display = 'block';
        document.getElementById('ntlmheading3').innerHTML = 'Add URL';
        document.getElementById('btnNtlmURL').value = "OK";
        $("ntlmValueText").value = "";
        document.getElementById('ntlmEnable').checked = true;
        flag = 1;
        var table = document.getElementById('tblNtlmRules');
        var rowsCount = table.rows.length;
        if (rowsCount > 8) {
          document.getElementById('connection-control-page').style.overflow = "auto";
        } else document.getElementById('connection-control-page').style.overflow = "visible";
      };

      $('ntlmCloseButton').onclick = function (event) {
        document.getElementById('addNtlmURL').style.display = "none";
      };

      $('btnNtlmCancel').onclick = function (event) {
        document.getElementById('addNtlmURL').style.display = "none";
      };

      $('btnNtlmURL').onclick = function (event) {
        if (flag == 1) {
          var val = document.getElementById("ntlmValueText").value;
          var enab = document.getElementById('ntlmEnable').checked;
          var msg = "";

          if (val == '' || val == null) {
            alert('Fields cannot be empty.');
            return false;
          } else if ((val != '' || val != null)) {
            var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
            if (val.match(domain) == null) {
              alert("Please enter valid URL");
              return false;
            }
            msg = "Allow connection to " + val;
          }
          var table = document.getElementById('tblNtlmRules');
          var rowCount = table.rows.length;
          var b = msg.split(" ");
          var c = "";
          var d = "";
          var comma = /,/g;
          var digit_five = /^\d{1,5}$/;
          for (i = 1; i < b.length; i++) {
            c += b[i] + " ";
          }
          for (j = 1; j < rowCount; j++) {
            var str1 = table.rows[j].cells[1].innerHTML;
            var str2 = str1.split(" ");
            var str3 = ""
            for (i = 1; i < str2.length; i++) {
              str3 += str2[i] + " ";
            }
            if (c == str3) {
              alert("URL already exists");
              return false;
            }
          }

          var row = table.insertRow(rowCount);
          var cell0 = row.insertCell(0);
          var cell1 = row.insertCell(1);
          cell1.textContent = msg;

          var cell2 = row.insertCell(2);
          var element2 = document.createElement("span");
          element2.className = "blue edit";
          element2.setAttribute("onClick", "editNtlmURL(this);");
          element2.innerHTML = 'Edit';
          cell2.appendChild(element2);

          var cell3 = row.insertCell(3);
          var element3 = document.createElement("span");
          element3.className = "blue";
          if (enab == true) {
            element3.innerHTML = 'Disable';
            row.className = "";
            cell0.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
          }
          else {
            element3.innerHTML = 'Enable';
            row.className = "disableTableRow";
            cell0.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
          }
          element3.setAttribute("onClick", "enableNtlmURL(this);");
          cell3.appendChild(element3);

          var cell4 = row.insertCell(4);
          var element4 = document.createElement("span");
          element4.className = "blue";
          element4.innerHTML = 'Delete';
          element4.setAttribute("onClick", "deleteNtlmURL(this);");
          cell4.appendChild(element4);
          document.getElementById('addNtlmURL').style.display = "none";
          chrome.send('AddHttpNtlmRules', [enab, msg]);

        }
        else {
          var val = document.getElementById("ntlmValueText").value;
          var enab = document.getElementById('ntlmEnable').checked;
          var msg = "";



          if (val == '' || val == null) {
            alert('Fields cannot be empty');
            return false;
          }
          else if ((val != '' || val != null)) {
            var domain = /(^|\s)((ftp:\/\/|https?:\/\/)?[\w-]+((\.|\/)[\w-]+)+\.?(:\d+)?(\/\S*)?)/gi;
            if (val.match(domain) == null) {
              alert("Please enter valid URL");
              return false;

            }
            msg = "Allow connection to " + val;
          }

          var table = document.getElementById('tblNtlmRules');
          var rowCount = table.rows.length;
          var b = msg.split(" ");
          var c = "";
          var d = "";
          var comma = /,/g;
          var digit_five = /^\d{1,5}$/;

          for (i = 1; i < b.length; i++) {
            c += b[i] + " ";
          }
          for (j = 1; j < rowCount; j++) {
            if (j == oRow.rowIndex) { continue; }
            else {
              var str1 = table.rows[j].cells[1].innerHTML;
              var str2 = str1.split(" ");
              var str3 = ""
              for (i = 1; i < str2.length; i++) {
                str3 += str2[i] + " ";
              }
              if (c == str3) {
                alert("URL already exists");
                return false;
              }
            }
          }

          oRow.cells[1].textContent = msg;
          if (enab == true) {
            oRow.cells[3].innerHTML = '<span class="blue" onclick="enableNtlmURL(this);">Disable</span>';
            oRow.className = "";
            oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
          } else {
            oRow.cells[3].innerHTML = '<span class="blue" onclick="enableNtlmURL(this);">Enable</span>';
            oRow.className = "disableTableRow";
            oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
          }

          chrome.send('EditHttpNtlmRules', [enab, oRow.rowIndex, msg]);

        }
        document.getElementById('addNtlmURL').style.display = "none";

      };
    },
  };

  portValue = function (el) {
    var protocolIndex = el.selectedIndex;
    var protocolIndexOptions = el.options;
    protocolOptionValue = protocolIndexOptions[protocolIndex].text;
    switch (protocolOptionValue) {
      case 'http':
        document.getElementById('portText').value = '80';
        break;
      case 'https':
        document.getElementById('portText').value = '443';
        break;
      default:
        document.getElementById('portText').value = '';
        break;
    }
  };

  deleteRule = function (src) {
    oRow = src.parentElement.parentElement;
    document.getElementById('addPage').style.display = "none";
    var table = document.getElementById('tableId');
    var rowsCount = table.rows.length;
    if (rowsCount > 8) {
      document.getElementById('connection-control-page').style.overflow = "auto";
    } else { document.getElementById('connection-control-page').style.overflow = "visible"; }
    var r = confirm("Do you want to Delete the Rule");
    if (r == true) {
      var Index = oRow.rowIndex;
      document.getElementById('tableId').deleteRow(oRow.rowIndex);
      chrome.send('DeleteConnectionRules', [Index]);
    } else {
      return;
    }
  };

  edit = function (src) {
    oRow = src.parentElement.parentElement;
    var a = oRow.cells[1].innerText;
    var able = oRow.cells[3].innerText;
    //console.info("oRow.cells[3].innerText....." + able);
    document.getElementById('heading3').innerHTML = 'Edit Rule';
    document.getElementById('rule').value = "Update";
    flag = 2;
    document.getElementById('addPage').style.display = "block";
    if (able == 'Disable')
      document.getElementById('enable').checked = true;
    else
      document.getElementById('enable').checked = false;
    if (a.indexOf('Allow') != -1)
      document.getElementById("conn").selectedIndex = 0;
    if (a.indexOf('Allow') == -1)
      document.getElementById("conn").selectedIndex = 1;
    if (a.indexOf('-Select-') != -1)
      document.getElementById("protocol").selectedIndex = 0;
    if (a.indexOf('http') != -1)
      document.getElementById("protocol").selectedIndex = 1;
    if (a.indexOf('https') != -1)
      document.getElementById("protocol").selectedIndex = 2;
    if (a.indexOf('ftp') != -1)
      document.getElementById("protocol").selectedIndex = 3;
    if (a.indexOf('-Select-') != -1)
      document.getElementById("server").selectedIndex = 0;
    if (a.indexOf('Domain') != -1)
      document.getElementById("server").selectedIndex = 1;
    if (a.indexOf('Hostname') != -1)
      document.getElementById("server").selectedIndex = 2;
    if (a.indexOf('IP') != -1)
      document.getElementById("server").selectedIndex = 3;
    if (a.indexOf('Public') != -1)
      document.getElementById("server").selectedIndex = 4;
    if (a.indexOf('Private') != -1)
      document.getElementById("server").selectedIndex = 5;
    var b = /[0-9]{2,4}/;
    var pattip = /( port )/;
    var c = a.match(b);
    var ipmatch = a.match(pattip);
    var patt1 = a.split(" ");
    if (c != null && ipmatch != null)
      document.getElementById("portText").value = patt1[5];
    else
      document.getElementById("portText").value = "";
    var patt = /(to)/g;
    var patt2 = /([0-9]{1,3})+\./g;
    var patt3 = /[.]/;
    var len = patt1.length - 1;
    var va = patt1[len];
    if (a.match(patt) != null) {
      document.getElementById('hideDiv').style.display = "block";
      document.getElementById("valueText").value = a.substring(a.indexOf("to") + 3)
      if (va.match(patt2) != null) {
        document.getElementById("server").selectedIndex = 3;
        document.getElementById('hideValue').innerText = 'IP';
      } else if (va.match(patt3) != null) {
        document.getElementById('hideValue').innerText = 'Domain';
        document.getElementById("server").selectedIndex = 1;
      } else if ((a.indexOf('Public') != -1) || (a.indexOf('Private') != -1)) {
        document.getElementById('hideDiv').style.display = "none";
      } else {
        document.getElementById('hideValue').innerText = 'Host';
        document.getElementById("server").selectedIndex = 2;
      }
    } else
      document.getElementById('hideDiv').style.display = "none";
  };

  enable = function (src) {
    oRow = src.parentElement.parentElement;
    document.getElementById('addPage').style.display = "none";
    var a = oRow.cells[3].innerText;
    var Index = oRow.rowIndex;
    if (a == 'Enable') {
      oRow.cells[3].innerHTML = '<span class="blue" onclick="enable(this);">Disable</span>';
      oRow.className = "";
      oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
      chrome.send('RuleEnabled', [Index]);
    } else {
      oRow.cells[3].innerHTML = '<span class="blue" onclick="enable(this);">Enable</span>';
      oRow.className = "disableTableRow";
      oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
      chrome.send('RuleDisabled', [Index]);
    }
  };

  deleteNtlmURL = function (src) {
    oRow = src.parentElement.parentElement;
    document.getElementById('addNtlmURL').style.display = "none";
    var table = document.getElementById('tblNtlmRules');
    var rowsCount = table.rows.length;
    if (rowsCount > 8) {
      document.getElementById('connection-control-page').style.overflow = "auto";
    } else { document.getElementById('connection-control-page').style.overflow = "visible"; }
    var r = confirm("Do you want to Delete the URL");
    if (r == true) {
      var Index = oRow.rowIndex;
      var enab = oRow.cells[3].innerText;
      var msg = oRow.cells[1].innerText;
      var enabStatus = false;
      if (enab == 'Enable') {
        enabStatus = false;
      } else {
        enabStatus = true;
      }
      document.getElementById('tblNtlmRules').deleteRow(oRow.rowIndex);
      chrome.send('DeleteHttpNtlmRules', [enabStatus, Index, msg]);
    } else {
      return;
    }
  };

  enableNtlmURL = function (src) {
    oRow = src.parentElement.parentElement;
    document.getElementById('addNtlmURL').style.display = "none";
    var enab = oRow.cells[3].innerText;
    var Index = oRow.rowIndex;
    var msg = oRow.cells[1].innerText;
    if (enab == 'Enable') {
      oRow.cells[3].innerHTML = '<span class="blue" onclick="enableNtlmURL(this);">Disable</span>';
      oRow.className = "";
      oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
      chrome.send('NTLMRuleEnabled', [true, Index, msg]);
    } else {
      oRow.cells[3].innerHTML = '<span class="blue" onclick="enableNtlmURL(this);">Enable</span>';
      oRow.className = "disableTableRow";
      oRow.cells[0].innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
      chrome.send('NTLMRuleDisabled', [false, Index, msg]);
    }
  };

  editNtlmURL = function (src) {
    oRow = src.parentElement.parentElement;
    var a = oRow.cells[1].innerText;
    var able = oRow.cells[3].innerText;
    //console.info("oRow.cells[2].innerText....." + able);
    document.getElementById('ntlmheading3').innerHTML = 'Edit URL';
    document.getElementById('btnNtlmURL').value = "Update";
    flag = 2;
    document.getElementById('addNtlmURL').style.display = "block";
    if (able == 'Disable')
      document.getElementById('ntlmEnable').checked = true;
    else
      document.getElementById('ntlmEnable').checked = false;

    var patt1 = a.split(" ");
    var patt = /(to)/g;
    var patt2 = /([0-9]{1,3})+\./g;
    var patt3 = /[.]/;
    var len = patt1.length - 1;
    var va = patt1[len];
    if (a.match(patt) != null) {
      document.getElementById("ntlmValueText").value = a.substring(a.indexOf("to") + 3);
    }
  };

  /**** Added for CHAMPION: for showing the browser installed in system******/
  ConnectionControl.InitBrowserList = function (Browserlist, alternateBrowser) {
    var defaultOptionsValue = '';
/*    var browserSelect = document.getElementById("blocked-url-browser");
    for (i = 0; i <= Browserlist.length - 1; i++) {
      browserSelect.add(new Option(Browserlist[i], i), browserSelect.options[i]);
      if (browserSelect.options[i].innerHTML == alternateBrowser) {
        defaultOptionsValue = browserSelect.options[i].value;
      }
    }
    browserSelect.selectedIndex = defaultOptionsValue;*/

    // champion added for page scroll issue noted in rel 78002 start
    var table = document.getElementById('tableId');
    var rowsCount = table.rows.length;
    if (rowsCount > 8) {
      document.getElementById('connection-control-page').style.overflow = "auto";
    } else document.getElementById('connection-control-page').style.overflow = "visible";
  };

  // champion navneet test start
  ConnectionControl.deleteAllRows = function () {
    var table = document.getElementById('tableId');
    var row = table.rows.length;
    for (var counter = row - 1; counter > 0; counter--) {
      table.deleteRow(counter);
    }
  };

  ConnectionControl.resetAlternateBrowser = function () {
    var browserSelect = document.getElementById("blocked-url-browser");
    browserSelect.selectedIndex = 0;
  };
  // champion navneet test end

  ConnectionControl.updateRules = function (rulelist) {
    for (var i = 0; i < rulelist.length; i++) {
      var table = document.getElementById('tableId');
      var rowCount = table.rows.length;
      var row = table.insertRow(rowCount);
      a = rulelist[i];
      var b = a.split(';');
      var cell1 = row.insertCell(0);

      var cell2 = row.insertCell(1);
      cell2.textContent = b[1];

      var cell3 = row.insertCell(2);
      var element2 = document.createElement("span");
      element2.className = "blue edit";
      element2.setAttribute("onClick", "edit(this);");
      element2.innerHTML = 'Edit';
      cell3.appendChild(element2);

      var cell4 = row.insertCell(3);
      var element3 = document.createElement("span");
      element3.className = "blue";
      if (b[0] == 1) {
        element3.innerHTML = 'Disable';
        row.className = "";
        cell1.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
      } else {
        element3.innerHTML = 'Enable';
        row.className = "disableTableRow";
        cell1.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
      }
      element3.setAttribute("onClick", "enable(this);");
      cell4.appendChild(element3);

      var cell5 = row.insertCell(4);
      var element4 = document.createElement("span");
      element4.className = "blue";
      element4.innerHTML = 'Delete';
      element4.setAttribute("onClick", "deleteRule(this);");
      cell5.appendChild(element4);
    }
  };

  ConnectionControl.updateNtlmURL = function (urlList) {
    for (var i = 0; i < urlList.length; i++) {
      var table = document.getElementById('tblNtlmRules');
      var rowCount = table.rows.length;
      var row = table.insertRow(rowCount);
      a = urlList[i];
      var b = a.split(';');
      var cell0 = row.insertCell(0);
      var cell1 = row.insertCell(1);
      cell1.textContent = b[1];

      var cell2 = row.insertCell(2);
      var element2 = document.createElement("span");
      element2.className = "blue edit";
      element2.setAttribute("onClick", "editNtlmURL(this);");
      element2.innerHTML = 'Edit';
      cell2.appendChild(element2);

      var cell3 = row.insertCell(3);
      var element3 = document.createElement("span");
      element3.className = "blue";
      if (b[0] == 1) {
        element3.innerHTML = 'Disable';
        row.className = "";
        cell0.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAABJUlEQVQ4jc3SvU4CQRDA8f/MGYujskKxoUGztU+gBYXFhSWBeHbos2jtC9ho58culXSQmPgEFiS2xsrE2LO4NkguhASw0el2M7/JzszCX4esCrIsKydJci4iz865i5UKtFqtzRBCHzAAMUarv8XAWFXf15bBjUajGkIYANXJ1SjGmHvvnxa+oNlsbolIr4DHInLa7XbvAaRer5fSNL0RkT0RaTvnHos4xjgAdgu445y7/snRNE2PROQQKAMP1toDgCzLKjP4CzgpYoCkVqt9qmoOlIB1oG2MeVXVyxnc8d5fzbYoANbaHWAAVOaMIYhI7py7mzej6T+YFOkD20UMHHvvb+dhgOkWvPcvqroPvC2LAZLiYTgcfhhjejHGDRE5W4T/R3wD8UhnbycDUKsAAAAASUVORK5CYII='/>";
      } else {
        element3.innerHTML = 'Enable';
        row.className = "disableTableRow";
        cell0.innerHTML = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABblBMVEUAAABOTk5IS1dISlNGSVRHSVVMTExJSUlHSVZHSlZHSlRHSVRHSVRHS1ZHSlZGSlZAQEBEREBVVVVGSlZHSlZHSVFEREBEREBEREAzM2ZIS1dHSVZHSlVVVVVEREBEREBHSlZISldISEpEREBJSVVHSlZISVVEREBEREBFSlhHSlZHR09FSVZHS1ZHSlZIS1RHSVRFSkxHSVZHSVNJSVdHSlZHSlZHR1JCQj5HSVJHSFRHSlVHSFRHSlZGSlZMTFVGSVNHSVZHSlVHSFNFSlVHSlZHS1VGRk9GSlVHSlZHSlVHSlZKSlVHSlZHSldDTVZIS1ZISlZGSlZJSVVHSlZHSlZHR1BISlVLS1pGSlZHSlZHSlZISFBHSldHSVZGRl1IS1dHSlZHSlZJSVAAAABGS1ZHSlZGSVdIS1ZGSlZHSVNFSVBFSE9FSVVHSlVHSlZIS1dHR09HSVNHSlRHSlZGSlZFTlhAQFVHSlZHSlY6AfIDAAAAeHRSTlMADVN3d1ULB4j17tDQ8/J/BAEJwOZqIhoDBWrstgYCCaHdMx8/9ZMSCjT9SjvzzGX8OY/DOPHPFRvXisCO79IXm7LMfzDt1BmFvrKYLevXGqRv5Crp2RxgEZjn3B+N5QtV/vAiAXT7SVz21XhSPm/QUi+j7OiVGgzCAkS9AAAACXBIWXMAAABIAAAASABGyWs+AAAA1ElEQVQY02NgwAoYmZhZWNngXHYOTi5uHl4+fgEGBkEgX0hYRFRMXEJSSlpGVk4eKKCgqKQsLsHAoKJaoaauwcCgqaUN5uvo6ukbGAJVGBmD+SamZuYWllZAM6xtQHxNWzt7cWUHR6ARTs5Avourm7u4sriHJ1DAy5uBwcfXzx/IlwhQAAoEBkkGh4SGgfiMruFAgQjXyKjoGBCfITYuHuTQhMSkZBA/JTUtHexShozMrOyc3Lx8vgIGsEsZ5AqLiisrSkrLGORALmUQlNeQFyyPh7IAzX0j+XF7JnMAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTMtMDgtMTBUMDQ6NTk6NDEtMDU6MDBDR982AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDEzLTA4LTEwVDA0OjU5OjQxLTA1OjAwMhpnigAAAABJRU5ErkJggg=='/>";
      }
      element3.setAttribute("onClick", "enableNtlmURL(this);");
      cell3.appendChild(element3);

      var cell4 = row.insertCell(4);
      var element4 = document.createElement("span");
      element4.className = "blue";
      element4.innerHTML = 'Delete';
      element4.setAttribute("onClick", "deleteNtlmURL(this);");
      cell4.appendChild(element4);

    }
  };

/*  ConnectionControl.InitLoadInDefaultBrowser = function (handlersenabled) {
    document.getElementById('default_chk').checked = handlersenabled;
  }; */

  return {
    ConnectionControl: ConnectionControl
  };

});
