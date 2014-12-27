# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# This file contains settings for ../chrome/chrome.gyp that other gyp files
# also use.
{
  'variables': {
    # TODO: remove this helper when we have loops in GYP
    'apply_locales_cmd': ['python', '<(DEPTH)/build/apply_locales.py'],

    'conditions': [
      ['OS=="mac"', {
        'conditions': [
          ['branding=="Chrome"', {
            'mac_bundle_id': 'com.google.Chrome',
            'mac_creator': 'rimZ',
            # The policy .grd file also needs the bundle id.
            'grit_defines': ['-D', 'mac_bundle_id=com.google.Chrome'], #champion
          }, {  # else: branding!="Chrome"
            'mac_bundle_id': 'com.whitehatsec.aviator',
            'mac_creator': 'Cr24',
            # The policy .grd file also needs the bundle id.
            'grit_defines': ['-D', 'mac_bundle_id=com.whitehatsec.aviator'], #champion
          }],  # branding
        ],  # conditions
      }],  # OS=="mac"
    ],  # conditions
  },  # variables
}
