# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# This file contains resources for the ui_unittests test bundle.
# See chrome_dll_bundle.gypi for a description of the techniques here.
# champion : app name changes (amresh)
{
  'product_name': 'ui_unittests Framework',
  'variables': {
    'grit_out_dir': '<(SHARED_INTERMEDIATE_DIR)/ui',

    # There is no executable in the mock framework, and so nothing to strip.
    'mac_strip': 0,
  },
  'mac_bundle': 1,
  'xcode_settings': {
    'CHROMIUM_BUNDLE_ID': 'com.google.ChromiumUITests',
    'DYLIB_COMPATIBILITY_VERSION': '1.0.0',
    'DYLIB_CURRENT_VERSION': '1.0.0',
    'DYLIB_INSTALL_NAME_BASE': '@executable_path/../Versions/1.0.0.0',
    'LD_DYLIB_INSTALL_NAME':
        '$(DYLIB_INSTALL_NAME_BASE:standardizepath)/$(WRAPPER_NAME)/$(PRODUCT_NAME)',
    'INFOPLIST_FILE': 'base/test/framework-Info.plist',
  },
  'mac_bundle_resources': [
    'base/test/framework-Info.plist',
    '<(PRODUCT_DIR)/ui_test.pak',
    '<!@pymod_do_main(repack_locales -o -p <(OS) -g <(grit_out_dir) -s <(SHARED_INTERMEDIATE_DIR) -x <(SHARED_INTERMEDIATE_DIR) <(locales))',
  ],
  'mac_bundle_resources!': [
    'base/test/framework-Info.plist',
  ],
  'postbuilds': [
    {
      'postbuild_name': 'Symlink Resources',
      'action': [
        'ln',
        '-fns',
        'Versions/A/Resources',
        '${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/Resources'
      ],
    },
    {
      # Resource bundle pak names are hardcoded. This allows ui_test.pak to be
      # found while running the ResourceBundle tests.
      'postbuild_name': 'Symlink chrome_100_percent for test',
      'action': [
        'ln',
        '-fns',
        'ui_test.pak',
        '${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/Versions/A/Resources/chrome_100_percent.pak'
      ],
    },
  ],
}
