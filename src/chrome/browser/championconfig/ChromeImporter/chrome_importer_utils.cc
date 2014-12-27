// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "chrome/browser/championconfig/chromeimporter/chrome_importer_utils.h" //This whole file is added for champion 
#include "chrome/common/chrome_paths_internal.h"                                //this is for getting profile path of chrome		
#include <algorithm>
#include <map>
#include <string>

#include "base/file_util.h"
#include "base/ini_parser.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "base/strings/string_util.h"
#include "url/gurl.h"

#include<string>

base::FilePath GetChromeProfilePath() {
  base::FilePath profile_path = GetProfilesPreferencePath();
  return profile_path;
}

void ParsePreferenceFile(const base::FilePath& file, base::DictionaryValue* root) {
  // Reads the whole INI file.
  std::string content;
  base::ReadFileToString(file, &content);
  ReplaceSubstringsAfterOffset(&content, 0, "\r\n", "\n");
  std::vector<std::string> lines;
  base::SplitString(content, '\n', &lines);

  // Parses the file.
  root->Clear();
  std::string current_section;
  for (size_t i = 0; i < lines.size(); ++i) {
    std::string line = lines[i];
    if (line.empty()) {
      // Skips the empty line.
      continue;
    }
    if (line[0] == '#' || line[0] == ';') {
      // This line is a comment.
      continue;
    }
    if (line[0] == '[') {
      // It is a section header.
      current_section = line.substr(1);
      size_t end = current_section.rfind(']');
      if (end != std::string::npos)
        current_section.erase(end);
    } else {
      std::string key, value;
      size_t equal = line.find('=');
      if (equal != std::string::npos) {
        key = line.substr(0, equal);
        value = line.substr(equal + 1);
        // Checks whether the section and key contain a '.' character.
        // Those sections and keys break DictionaryValue's path format,
        // so we discard them.
        if (current_section.find('.') == std::string::npos &&
            key.find('.') == std::string::npos)
          root->SetString(current_section + "." + key, value);
      }
    }
  }
}

// champion : already in chrome_importer.cc (amresh)
/*bool CanImportURL(const GURL& url) {
  const char* kInvalidSchemes[] = {"wyciwyg", "place", "about", "chrome"};

  // The URL is not valid.
  if (!url.is_valid())
    return false;

  // Filter out the URLs with unsupported schemes.
  for (size_t i = 0; i < arraysize(kInvalidSchemes); ++i) {
    if (url.SchemeIs(kInvalidSchemes[i]))
      return false;
  }

  return true;
}*/

void ParseSearchEnginesFromXMLFiles(
    const std::vector<base::FilePath>& xml_files,
    std::vector<TemplateURL*>* search_engines) {
  /* DCHECK(search_engines);

  typedef std::map<std::string, TemplateURL*> SearchEnginesMap;
  SearchEnginesMap search_engine_for_url;
  std::string content;
  // The first XML file represents the default search engine in Chrome 3, so we
  // need to keep it on top of the list.
  SearchEnginesMap::const_iterator default_turl = search_engine_for_url.end();
  for (std::vector<base::FilePath>::const_iterator file_iter =
           xml_files.begin(); file_iter != xml_files.end(); ++file_iter) {
    base::ReadFileToString(*file_iter, &content);
    ChromeURLParameterFilter param_filter;
    TemplateURL* template_url = TemplateURLParser::Parse(NULL, true,
        content.data(), content.length(), &param_filter);
    if (template_url) {
      SearchEnginesMap::iterator iter =
          search_engine_for_url.find(template_url->url());
      if (iter == search_engine_for_url.end()) {
        iter = search_engine_for_url.insert(
            std::make_pair(template_url->url(), template_url)).first;
      } else {
        // We have already found a search engine with the same URL.  We give
        // priority to the latest one found, as GetSearchEnginesXMLFiles()
        // returns a vector with first Chrome default search engines and then
        // the user's ones.  We want to give priority to the user ones.
        delete iter->second;
        iter->second = template_url;
      }
      if (default_turl == search_engine_for_url.end())
        default_turl = iter;
    }
    content.clear();
  }

  // Put the results in the |search_engines| vector.
  for (SearchEnginesMap::iterator t_iter = search_engine_for_url.begin();
       t_iter != search_engine_for_url.end(); ++t_iter) {
    if (t_iter == default_turl)
      search_engines->insert(search_engines->begin(), default_turl->second);
    else
      search_engines->push_back(t_iter->second);
  }*/
}

std::string ReadPrefsJsValueChrome(const base::FilePath& profile_path,
                             const std::string& pref_key) {
  std::string content;
  return GetPrefsJsValueChrome(content, pref_key);
}

GURL GetHomepageChrome(const base::FilePath& profile_path) {			
  std::string home_page_list;
  ReadPrefsJsValueChrome(profile_path, "homepage");
  size_t seperator = home_page_list.find_first_of('|');

  if (seperator == std::string::npos)
    return GURL(home_page_list);

  return GURL(home_page_list.substr(0, seperator));
}

bool IsDefaultHomepageChrome(const GURL& homepage, const base::FilePath& app_path) {
  /*if (!homepage.is_valid())
    return false;

  std::string default_homepages =
      ReadBrowserConfigProp(app_path, "homepage_is_newtabpage");

  size_t seperator = default_homepages.find_first_of('|');
  if (seperator == std::string::npos)
    return homepage.spec() == GURL(default_homepages).spec();

  // Crack the string into separate homepage urls.
  std::vector<std::string> urls;
  base::SplitString(default_homepages, '|', &urls);

  for (size_t i = 0; i < urls.size(); ++i) {
    if (homepage.spec() == GURL(urls[i]).spec())
      return true;
  }*/

  return false;
}

bool ParsePrefFile(const base::FilePath& pref_file, base::DictionaryValue* prefs) {
  // The string that is before a pref key.
  const std::string kUserPrefString = "user_pref(\"";
  std::string contents;
  if (!base::ReadFileToString(pref_file, &contents))
    return false;

  std::vector<std::string> lines;
  Tokenize(contents, "\n", &lines);

  for (std::vector<std::string>::const_iterator iter = lines.begin();
       iter != lines.end(); ++iter) {
    const std::string& line = *iter;
    size_t start_key = line.find(kUserPrefString);

    if (start_key == std::string::npos)
      continue;  // Could be a comment or a blank line.

    start_key += kUserPrefString.length();
    size_t stop_key = line.find('"', start_key);

    if (stop_key == std::string::npos) {
      LOG(ERROR) << "Invalid key found in Chrome pref file '" <<
          pref_file.value() << "' line is '" << line << "'.";
      continue;
    }

    std::string key = line.substr(start_key, stop_key - start_key);
    size_t start_value = line.find(',', stop_key + 1);

    if (start_value == std::string::npos) {
      LOG(ERROR) << "Invalid value found in Chrome pref file '" <<
          pref_file.value() << "' line is '" << line << "'.";
      continue;
    }

    size_t stop_value = line.find(");", start_value + 1);

    if (stop_value == std::string::npos) {
      LOG(ERROR) << "Invalid value found in Chrome pref file '" <<
          pref_file.value() << "' line is '" << line << "'.";
      continue;
    }

    std::string value = line.substr(start_value + 1, stop_value - start_value - 1);
    TrimWhitespace(value, base::TRIM_ALL, &value);
    // Value could be a boolean.
    bool is_value_true = LowerCaseEqualsASCII(value, "true");

    if (is_value_true || LowerCaseEqualsASCII(value, "false")) {
      prefs->SetBoolean(key, is_value_true);
      continue;
    }

    // Value could be a string.
    if (value.size() >= 2U &&
        value[0] == '"' && value[value.size() - 1] == '"') {
      value = value.substr(1, value.size() - 2);
      // ValueString only accept valid UTF-8.  Simply ignore that entry if it is
      // not UTF-8.
      if (base::IsStringUTF8(value))
        prefs->SetString(key, value);
      else
        VLOG(1) << "Non UTF8 value for key " << key << ", ignored.";
      continue;
    }

    // Or value could be an integer.
    int int_value = 0;
    if (base::StringToInt(value, &int_value)) {
      prefs->SetInteger(key, int_value);
      continue;
    }

    LOG(ERROR) << "Invalid value found in Chrome pref file '"
               << pref_file.value() << "' value is '" << value << "'.";
  }

  return true;
}

std::string GetPrefsJsValueChrome(const std::string& content,
                            const std::string& pref_key) {
  // This file has the syntax: user_pref("key", value);
  std::string search_for = std::string("user_pref(\"") + pref_key +
                           std::string("\", ");
  size_t prop_index = content.find(search_for);
  if (prop_index == std::string::npos)
    return std::string();

  size_t start = prop_index + search_for.length();
  size_t stop = std::string::npos;

  if (start != std::string::npos) {
    // Stop at the last ')' on this line.
    stop = content.find("\n", start + 1);
    stop = content.rfind(")", stop);
  }

  if (start == std::string::npos || stop == std::string::npos ||
      stop < start) {
    LOG(WARNING) << "Chrome property " << pref_key << " could not be parsed.";
    return std::string();
  }

  // String values have double quotes we don't need to return to the caller.
  if (content[start] == '\"' && content[stop - 1] == '\"') {
    ++start;
    --stop;
  }

  return content.substr(start, stop - start);
}

base::string16 GetChromeImporterName(const base::FilePath& app_path) {
  const base::FilePath app_ini_file = app_path.AppendASCII("application.ini");
  std::string branding_name;
  if (base::PathExists(app_ini_file)) {
    std::string content;
    base::ReadFileToString(app_ini_file, &content);
    std::vector<std::string> lines;
    base::SplitString(content, '\n', &lines);
    const std::string name_attr("Name=");
    bool in_app_section = false;
    for (size_t i = 0; i < lines.size(); ++i) {
      TrimWhitespace(lines[i], base::TRIM_ALL, &lines[i]);
      if (lines[i] == "[App]") {
        in_app_section = true;
      } else if (in_app_section) {
        if (lines[i].find(name_attr) == 0) {
          branding_name = lines[i].substr(name_attr.size());
          break;
        } else if (lines[i].length() > 0 && lines[i][0] == '[') {
          // No longer in the [App] section.
          break;
        }
      }
    }
  }

  StringToLowerASCII(&branding_name);

  if (branding_name.find("iceweasel") != std::string::npos)
    return l10n_util::GetStringUTF16(IDS_IMPORT_FROM_ICEWEASEL);

  return l10n_util::GetStringUTF16(IDS_IMPORT_FROM_FIREFOX);
}
