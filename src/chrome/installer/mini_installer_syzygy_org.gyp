# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 
{
  'variables': {
    'version_py': '<(DEPTH)/build/util/version.py',
    'version_path': '<(DEPTH)/chrome/VERSION',
    'lastchange_path': '<(DEPTH)/build/util/LASTCHANGE',
    # 'branding_dir' is set in the 'conditions' section at the bottom.
    'msvs_use_common_release': 0,
    'msvs_use_common_linker_extras': 0,
  },
  'includes': [
    '../../build/win_precompile.gypi',
  ],
  'conditions': [
    # This target won't build in fastbuild, since there are no PDBs.
    ['OS=="win" and fastbuild==0', {
      'conditions': [
        ['chrome_multiple_dll==0', {
          'targets': [
            {
              'target_name': 'mini_installer_syzygy',
              'type': 'executable',
              'product_name': 'mini_installer',

              'variables': {
                'chrome_dll_project': [
                  '../chrome_syzygy.gyp:chrome_dll_syzygy',
                ],
                'chrome_dll_path': [
                  '<(PRODUCT_DIR)/syzygy/Aviator.dll',
                ],
                'output_dir': '<(PRODUCT_DIR)/syzygy',
              },
              # Bulk of the build configuration comes from here.
              'includes': [ 'mini_installer.gypi', ],
            },
          ],
        }, {
          'targets': [
            {
              'target_name': 'mini_installer_syzygy',
              'type': 'executable',
              'product_name': 'mini_installer',

              'variables': {
                'chrome_dll_project': [
                  '../chrome_syzygy.gyp:chrome_dll_syzygy',
                  '../chrome_syzygy.gyp:chrome_child_dll_syzygy',
                ],
                'chrome_dll_path': [
                  '<(PRODUCT_DIR)/syzygy/Aviator.dll', # champion : app name change (amresh)
                  '<(PRODUCT_DIR)/syzygy/Aviator_child.dll',
                ],
                'output_dir': '<(PRODUCT_DIR)/syzygy',
              },
              # Bulk of the build configuration comes from here.
              'includes': [ 'mini_installer.gypi', ],
            },
          ],
        }],
      ],
    },{
      'targets': [],
    }],
    [ 'branding == "Chrome"', {
      'variables': {
         'branding_dir': '../app/theme/google_chrome',
      },
    }, {  # else branding!="Chrome"
      'variables': {
         'branding_dir': '../app/theme/chromium',
      },
    }],
  ],
}
