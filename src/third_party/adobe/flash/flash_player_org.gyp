# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

{
  'variables': {
    'flapper_version_h_file%': 'flapper_version.h',
    'flapper_binary_files%': [],
    'conditions': [
      [ 'branding == "Aviator"', { # champion : flash player fix (Balaji)
        'conditions': [
          [ 'OS == "linux" and target_arch == "ia32"', {
            'flapper_version_h_file%': 'symbols/ppapi/linux/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/linux/libpepflashplayer.so',
              'binaries/ppapi/linux/manifest.json',
            ],
          }],
          [ 'OS == "linux" and target_arch == "x64"', {
            'flapper_version_h_file%': 'symbols/ppapi/linux_x64/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/linux_x64/libpepflashplayer.so',
              'binaries/ppapi/linux_x64/manifest.json',
            ],
          }],
          [ 'OS == "mac" and target_arch == "ia32"', {
            'flapper_version_h_file%': 'symbols/ppapi/mac/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/mac/PepperFlashPlayer.plugin',
              'binaries/ppapi/mac/manifest.json',
            ],
          }],
          [ 'OS == "mac" and target_arch == "x64"', {
            'flapper_version_h_file%': 'symbols/ppapi/mac_64/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/mac_64/PepperFlashPlayer.plugin',
              'binaries/ppapi/mac_64/manifest.json',
            ],
          }],
          [ 'OS == "win" and target_arch == "ia32"', {
            'flapper_version_h_file%': 'symbols/ppapi/win/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/win/pepflashplayer.dll',
              'binaries/ppapi/win/manifest.json',
            ],
          }],
          [ 'OS == "win" and target_arch == "x64"', {
            'flapper_version_h_file%': 'symbols/ppapi/win_x64/flapper_version.h',
            'flapper_binary_files%': [
              'binaries/ppapi/win_x64/pepflashplayer.dll',
              'binaries/ppapi/win_x64/manifest.json',
            ],
          }],
        ],
      }],
    ],
  },
  # Always provide a target, so we can put the logic about whether there's
  # anything to be done in this file (instead of a higher-level .gyp file).
  'targets': [
    {
      # GN version: //third_party/adobe/flash:flapper_version_h
      'target_name': 'flapper_version_h',
      'type': 'none',
      'copies': [{
        'destination': '<(SHARED_INTERMEDIATE_DIR)',
        'files': [ '<(flapper_version_h_file)' ],
      }],
    },
    {
      # GN version: //third_party/adobe/flash:flapper_binaries
      'target_name': 'flapper_binaries',
      'type': 'none',
      'copies': [{
        'destination': '<(PRODUCT_DIR)/PepperFlash',
        'files': [ '<@(flapper_binary_files)' ],
      }],
    },
  ],
}
