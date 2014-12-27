// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
// Purpose - For parsing update server list file.

#ifndef CHROME_BROWSER_CHAMPIONCONFIG_SERVERLISTPARSER_SERVER_LIST_PARSER_H
#define CHROME_BROWSER_CHAMPIONCONFIG_SERVERLISTPARSER_SERVER_LIST_PARSER_H

#include <vector>
#include <string>
#include "chrome/browser/championconfig/update/curl.h"
#include "chrome/browser/championconfig/varsconfig.h"

namespace AutoUpdater {

typedef std::vector<std::string> ServerList;
typedef std::vector<std::string>::iterator ServerListIterator;

enum ConvertType { LOWER = 0, UPPER, NONE };

std::string Trim(std::string& strData);
std::string Convert(std::string& strData, ConvertType type);
bool FileExist(const std::string& strFile);
bool DirExist(const std::string& strPath);
size_t write_list_file(void *ptr, size_t size, size_t nmemb, FILE* stream);

// Declaration of class ServerListParser
class ServerListParser {
public:
  ServerListParser(const std::string& strServerListFile);
  ~ServerListParser();

  void ResetListFile(const std::string& strServerListFile);
  bool CopyAndReset(CURL* curl, const std::string& strDownloadLocation, const std::string& strHash);
  const ServerList& GetServerList() const { return serverList; }
  float GetVersion() const { return listFileVersion; }
  bool Exists() const { return listFileFound; }
  std::string GetListHash() const { return GetHash(strListFile); }
#if USE_CERT
  void SetCertificateFile(const std::string& strCertFile) { certificateFile = strCertFile; }
#endif

private:
  void Initialize();
  bool HasValue(const std::string& strSource, const std::string& strKey, std::string& strValue) const;
  std::string GetPath() const;
  std::string Download(CURL* curl, const std::string& strLocation, const std::string& strHash) const;
  std::string GetHash(const std::string& strFile) const;
  void RandomizeURLList();
  void PrepareRandomIndexes(size_t listSize, std::vector<int>& indexList) const;
  void PrepareURL(std::string& strURL) const;

  bool listFileFound;
  float listFileVersion;
  ServerList serverList;
  std::string strListFile;
#if USE_CERT
  std::string certificateFile;
#endif
};

} // namespace AutoUpdater

#endif // CHROME_BROWSER_CHAMPIONCONFIG_HOMEPAGECACHER_HOME_PAGE_CACHER_H
