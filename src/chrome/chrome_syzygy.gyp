# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 
{
  'variables': {
    'dest_dir': '<(PRODUCT_DIR)/syzygy',
  },
  'conditions': [
    ['syzyasan==1', {
      'variables': {
        'syzygy_exe_dir': '<(DEPTH)/third_party/syzygy/binaries/exe',
      },
      # Copy the SyzyASan runtime and logger to the syzygy directory.
      'targets': [
        {
          'target_name': 'copy_syzyasan_binaries',
          'type': 'none',
          'outputs': [
            '<(dest_dir)/agent_logger.exe',
            '<(dest_dir)/syzyasan_rtl.dll',
            '<(dest_dir)/syzyasan_rtl.dll.pdb',
          ],
          'copies': [
            {
              'destination': '<(dest_dir)',
              'files': [
                '<(syzygy_exe_dir)/agent_logger.exe',
                '<(syzygy_exe_dir)/syzyasan_rtl.dll',
                '<(syzygy_exe_dir)/syzyasan_rtl.dll.pdb',
              ],
            },
          ],
        },
      ],
    }],
    ['OS=="win" and fastbuild==0', {
      'conditions': [
        ['syzygy_optimize==1 or syzyasan==1', {
          'variables': {
            'dll_name': 'Aviator',
          },
          'targets': [
            {
              'target_name': 'chrome_dll_syzygy',
              'type': 'none',
              'sources' : [],
              'includes': [
                'chrome_syzygy.gypi',
              ],
            },
          ],
        }],
        ['chrome_multiple_dll==1', {
          'conditions': [
            ['syzyasan==1 or syzygy_optimize==1', {
              'variables': {
                'dll_name': 'Aviator_child',
              },
              'targets': [
                {
                  'target_name': 'chrome_child_dll_syzygy',
                  'type': 'none',
                  # For the official SyzyASan builds just copy Aviator_child.dll
                  # to the Syzygy directory.
                  'conditions': [
                    ['syzyasan==1 and buildtype=="Official"', {
                      'dependencies': [
                        'chrome_child_dll_syzygy_copy'
                      ],
                    }],
                  ],
                  # For the official SyzyASan builds also put an instrumented
                  # version of chrome_child.dll into syzygy/instrumented.
                  'variables': {
                    'conditions': [
                      ['syzyasan==1 and buildtype=="Official"', {
                        'dest_dir': '<(PRODUCT_DIR)/syzygy/instrumented',
                      }],
                    ],
                  },
                  'sources' : [],
                  'includes': [
                    'chrome_syzygy.gypi',
                  ],
                },
              ],
            }],
            # For the official SyzyASan builds just copy chrome_child.dll to the
            # Syzygy directory.
            ['syzyasan==1 and buildtype=="Official"', {
              'targets': [
              {
                'target_name': 'chrome_child_dll_syzygy_copy',
                'type': 'none',
                'inputs': [
                  '<(PRODUCT_DIR)/Aviator_child.dll',
                  '<(PRODUCT_DIR)/aviator_child.dll.pdb',
                ],
                'outputs': [
                  '<(dest_dir)/Aviator_child.dll',
                  '<(dest_dir)/aviator_child.dll.pdb',
                ],
                'copies': [
                  {
                    'destination': '<(dest_dir)',
                    'files': [
                      '<(PRODUCT_DIR)/Aviator_child.dll',
                      '<(PRODUCT_DIR)/aviator_child.dll.pdb',
                    ],
                  },
                ],
              }],
            }],
          ],
        }],
      ],
    }],
  ],
}
