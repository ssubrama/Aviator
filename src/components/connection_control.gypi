# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

{
  'targets': [
    {
      'target_name': 'ruleparser',
      'type': 'shared_library',
      'sources': [
      '../chrome/browser/championconfig/connectioncontrol/rule_parser.h',                       # champion : for ntlm rule parsing (amresh)
      '../chrome/browser/championconfig/connectioncontrol/rule_parser.cc',                      # champion : for ntlm rule parsing (amresh)    
      '../chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.cc',   # champion : connection control
      '../chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.h',    # champion : connection control    
      '../chrome/browser/championconfig/connectioncontrol/connection_control_handler.cc', # champion : moved our files to championconfig (amresh)
      '../chrome/browser/championconfig/connectioncontrol/connection_control_handler.h',  # champion : moved our files to championconfig (amresh)
      '../chrome/browser/championconfig/connectioncontrol/control_ntml_http_auth.h',            # champion : for ntlm disable (utpal)
      '../chrome/browser/championconfig/connectioncontrol/control_ntml_http_auth.cc',           # champion : for ntlm disable (utpal)     
        ],
        'include_dirs': [
            '<(DEPTH)',
        ],
          'dependencies': [
            '../base/base.gyp:base',
            '../url/url.gyp:url_lib',
            '../skia/skia.gyp:skia',
          ],
          'conditions': [
          ['OS=="win"', {
          'sources' : [
    		  '../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.cc',   # champion : connection control
		      '../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h',    # champion : connection control 
          ],
        }],
        ],
    },      
  ],
}
