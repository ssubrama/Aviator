# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 
{
  'action_name': 'repack_Aviator_resources_100_percent',
  'variables': {
    'pak_inputs': [
      '<(SHARED_INTERMEDIATE_DIR)/components/component_resources_100_percent.pak',
      '<(SHARED_INTERMEDIATE_DIR)/ui/ui_resources/ui_resources_100_percent.pak',
      '<(grit_out_dir)/renderer_resources_100_percent.pak',
      '<(grit_out_dir)/theme_resources_100_percent.pak',
    ],
    'pak_output': '<(SHARED_INTERMEDIATE_DIR)/repack/Aviator_100_percent.pak',
    'conditions': [
      ['OS != "ios"', {
        'pak_inputs': [
          '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_resources_100_percent.pak',
        ],
      }],
      ['use_ash==1', {
        'pak_inputs': [
          '<(SHARED_INTERMEDIATE_DIR)/ash/resources/ash_resources_100_percent.pak',
        ],
      }],
    ],
  },
  'includes': [ '../build/repack_action.gypi' ],
}
