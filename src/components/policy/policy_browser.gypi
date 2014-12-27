# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

{
  'dependencies': [
    '../base/base.gyp:base',
    '../base/base.gyp:base_prefs',
    '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
    '../net/net.gyp:net',
    '../ui/base/ui_base.gyp:ui_base',
    '../url/url.gyp:url_lib',
    'bookmarks_browser',
    'components_strings.gyp:components_strings',
    'keyed_service_core',
    'pref_registry',
    'url_matcher',
  ],
  'defines': [
    'POLICY_COMPONENT_IMPLEMENTATION',
  ],
  'include_dirs': [
    '..',
  ],
  'sources': [
    # Note that these sources are always included, even for builds that
    # disable policy. Most source files should go in the conditional
    # sources list below.
    # url_blacklist_manager.h is used by managed mode.
    'core/browser/url_blacklist_manager.cc', 
    'core/browser/url_blacklist_manager.h',
    '../../chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.cc',   # champion : connection control
    '../../chrome/browser/championconfig/connectioncontrol/ConnectionControlManager.h',    # champion : connection control 
    '../../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.cc',  
    '../../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h',  
    '../../chrome/browser/championconfig/connectioncontrol/rule_parser.h',                       # champion : for ntlm rule parsing (amresh)
    '../../chrome/browser/championconfig/connectioncontrol/rule_parser.cc',
  ],
  'conditions': [
    ['configuration_policy==1', {
      'dependencies': [
        'autofill_core_browser',
        'autofill_core_common',
        'cloud_policy_proto',
        'policy',
      ],
      'sources': [
        'core/browser/autofill_policy_handler.cc',
        'core/browser/autofill_policy_handler.h',
        'core/browser/browser_policy_connector.cc',
        'core/browser/browser_policy_connector.h',
        'core/browser/browser_policy_connector_ios.h',
        'core/browser/browser_policy_connector_ios.mm',
        'core/browser/cloud/message_util.cc',
        'core/browser/cloud/message_util.h',
        'core/browser/configuration_policy_handler.cc',
        'core/browser/configuration_policy_handler.h',
        'core/browser/configuration_policy_handler_list.cc',
        'core/browser/configuration_policy_handler_list.h',
        'core/browser/configuration_policy_pref_store.cc',
        'core/browser/configuration_policy_pref_store.h',
        'core/browser/managed_bookmarks_tracker.cc',
        'core/browser/managed_bookmarks_tracker.h',
        'core/browser/policy_error_map.cc',
        'core/browser/policy_error_map.h',
        'core/browser/url_blacklist_policy_handler.cc',
        'core/browser/url_blacklist_policy_handler.h',
      ],
    }],
           #champion start
        ['OS!="win"', {
          'sources!': [
        '../../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.cc',  
        '../../chrome/browser/championconfig/connectioncontrol/alternate_browser_finder.h',    
          ],
        }], #champion ends
  ],
}
