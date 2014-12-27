# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# Intended to be included by chrome_syzygy.gyp. A variable 'dll_name' should
# be set to the base name of the DLL. This is used to generate the build steps
# for both chrome.dll and chrome_child.dll when in multiple dll mode.
{
  # Reorder or instrument the initial chrome DLL executable, placing the
  # optimized output and corresponding PDB file into the "syzygy"
  # subdirectory.
  # This target won't build in fastbuild, since there are no PDBs.
  'dependencies': [
    '<(DEPTH)/chrome/chrome.gyp:chrome_dll', # champion : app name change (amresh)
  ],
  'conditions': [
    ['syzyasan==0 and syzygy_optimize==1', {
      # Reorder chrome DLL executable.
      # If there's a matching chrome.dll-ordering.json file present in
      # the output directory, chrome.dll will be ordered according to
      # that, otherwise it will be randomized.
      'actions': [
        {
          'action_name': 'Reorder Chrome with Syzygy',
          'inputs': [
            '<(PRODUCT_DIR)/<(dll_name).dll',
            '<(PRODUCT_DIR)/<(dll_name).dll.pdb',
          ],
          'outputs': [
            '<(dest_dir)/<(dll_name).dll',
            '<(dest_dir)/<(dll_name).dll.pdb',
          ],
          'action': [
            'python',
            '<(DEPTH)/chrome/tools/build/win/syzygy_reorder.py',
            '--input_executable', '<(PRODUCT_DIR)/<(dll_name).dll',
            '--input_symbol', '<(PRODUCT_DIR)/<(dll_name).dll.pdb',
            '--destination_dir', '<(dest_dir)',
          ],
        },
      ],
    }],
    ['syzyasan==1 and syzygy_optimize==0', {
      # Instrument chrome DLL executable with SyzyAsan.
      'actions': [
        {
          'action_name': 'Instrument Chrome with SyzyAsan',
          'inputs': [
            '<(DEPTH)/chrome/tools/build/win/win-syzyasan-filter.txt',
            '<(PRODUCT_DIR)/<(dll_name).dll',
          ],
          'outputs': [
            '<(dest_dir)/<(dll_name).dll',
            '<(dest_dir)/<(dll_name).dll.pdb',
            '<(dest_dir)/win-syzyasan-filter-<(dll_name).txt.json',
          ],
          'action': [
            'python',
            '<(DEPTH)/chrome/tools/build/win/syzygy_instrument.py',
            '--mode', 'asan',
            '--input_executable', '<(PRODUCT_DIR)/<(dll_name).dll',
            '--input_symbol', '<(PRODUCT_DIR)/<(dll_name).dll.pdb',
            '--filter',
            '<(DEPTH)/chrome/tools/build/win/win-syzyasan-filter.txt',
            '--output-filter-file',
            '<(dest_dir)/win-syzyasan-filter-<(dll_name).txt.json',
            '--destination_dir', '<(dest_dir)',
          ],
        },
      ],
      'dependencies': [
        'copy_syzyasan_binaries',
      ],
    }],
  ],
}
