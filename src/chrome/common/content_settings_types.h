// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_COMMON_CONTENT_SETTINGS_TYPES_H_
#define CHROME_COMMON_CONTENT_SETTINGS_TYPES_H_

// A particular type of content to care about.  We give the user various types
// of controls over each of these.
enum ContentSettingsType {
  // "DEFAULT" is only used as an argument to the Content Settings Window
  // opener; there it means "whatever was last shown".
  CONTENT_SETTINGS_TYPE_DEFAULT = -1,
  CONTENT_SETTINGS_TYPE_COOKIES = 0,
  CONTENT_SETTINGS_TYPE_IMAGES,
  CONTENT_SETTINGS_TYPE_JAVASCRIPT,
  CONTENT_SETTINGS_TYPE_PLUGINS,
  CONTENT_SETTINGS_TYPE_POPUPS,
  CONTENT_SETTINGS_TYPE_GEOLOCATION,
  CONTENT_SETTINGS_TYPE_NOTIFICATIONS,
  CONTENT_SETTINGS_TYPE_AUTO_SELECT_CERTIFICATE,
  CONTENT_SETTINGS_TYPE_FULLSCREEN,
  CONTENT_SETTINGS_TYPE_MOUSELOCK,
  CONTENT_SETTINGS_TYPE_MIXEDSCRIPT,
  CONTENT_SETTINGS_TYPE_MEDIASTREAM,  // Default setting for MIC and CAMERA.
  CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC,
  CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA,
  CONTENT_SETTINGS_TYPE_PROTOCOL_HANDLERS,
  CONTENT_SETTINGS_TYPE_PPAPI_BROKER,
  CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS,
  CONTENT_SETTINGS_TYPE_MIDI_SYSEX,
#if defined(OS_WIN)
  CONTENT_SETTINGS_TYPE_METRO_SWITCH_TO_DESKTOP,
#elif defined(OS_ANDROID) || defined(OS_CHROMEOS)
  CONTENT_SETTINGS_TYPE_PROTECTED_MEDIA_IDENTIFIER,
#endif
#if defined(OS_ANDROID)
  CONTENT_SETTINGS_TYPE_APP_BANNER,
#endif
  CONTENT_SETTINGS_TYPE_REFERRER,   // champion
  CONTENT_SETTINGS_NUM_TYPES,
};

#endif  // CHROME_COMMON_CONTENT_SETTINGS_TYPES_H_
