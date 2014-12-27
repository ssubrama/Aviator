# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 
{
  'variables': {
    'chromium_code': 1,
  },
  'includes': [
    '../build/win_precompile.gypi',
  ],
  'targets': [
    {
      'target_name': 'test_registrar',
      'type': 'executable',
      'dependencies': [
        '../base/base.gyp:base',
        # Chrome is the default viewer process currently used by the tests.
        # TODO(robertshield): Investigate building a standalone metro viewer
        # process.
        '../chrome/chrome.gyp:chrome', # champion : branding (amresh)
        'win8.gyp:test_registrar_constants',
      ],
      'sources': [
        'test/test_registrar.cc',
        'test/test_registrar.rc',
        'test/test_registrar.rgs',
        'test/test_registrar_resource.h',
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
        },
      },
    },
  ],
}
