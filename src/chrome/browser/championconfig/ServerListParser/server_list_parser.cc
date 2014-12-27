// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
// Purpose - For parsing update server list file.

#include <sys/stat.h>
#include <direct.h>
#include <algorithm>
#include <fstream>

#include "chrome/browser/championconfig/varsconfig.h"
#include "chrome/browser/championconfig/serverlistparser/sha1.h"
#include "chrome/browser/championconfig/serverlistparser/server_list_parser.h" 

#if defined(ARCH_CPU_64_BITS)
#pragma comment(lib, "x64/libcurl.lib")
#else
#pragma comment(lib, "x86/libcurl.lib")
#endif


namespace AutoUpdater {

////////////////////////////////////////////// Global functions ////////////////////////////////////////////////

// utility function to trim string from both sides
std::string Trim(std::string& strData) {
  if (strData.size()) {
    strData.erase(0, strData.find_first_not_of(' '));         //prefixing spaces
    strData.erase(strData.find_last_not_of(' ') + 1);         //suffixing spaces
  }

  return strData;
}

// utility function to convert string
std::string Convert(std::string& strData, ConvertType type) {
  switch (type) {
    case ConvertType::LOWER:
      std::transform(strData.begin(), strData.end(), strData.begin(), ::tolower);
      break;
    case ConvertType::UPPER:
      std::transform(strData.begin(), strData.end(), strData.begin(), ::toupper);
      break;
    default:
      break;
  }

  return strData;
}

// writes list file to temp location
size_t write_list_file(void *ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

// return true if given file exists else false
bool FileExist(const std::string& strFile) {
  struct stat buffer;
  return (stat(strFile.c_str(), &buffer) == 0);
}

// returns true if given path exists else false
bool DirExist(const std::string& strPath) {
  struct stat info;

  if (stat(strPath.c_str(), &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}


///////////////////////////////////// Definition of class ServerListParser /////////////////////////////////////

// ServerListParser constructor
ServerListParser::ServerListParser(const std::string& strServerListFile)
  : listFileFound(false), listFileVersion(0.0), strListFile(strServerListFile) {
#if USE_CERT
  certificateFile = "";
#endif
  if (FileExist(strListFile))
    listFileFound = true;

  Initialize();
}

// ServerListParser destructor
ServerListParser::~ServerListParser() {}

// resets list file to specified file
void ServerListParser::ResetListFile(const std::string & strServerListFile) {
  strListFile = strServerListFile;
  if (FileExist(strListFile))
    listFileFound = true;

  Initialize();
}

// copies specified file to server list file location and resets object
bool ServerListParser::CopyAndReset(CURL* curl, const std::string& strLocation, const std::string& strHash) {
  std::string strDownloadedFile = Download(curl, strLocation, strHash);
  if (!FileExist(strDownloadedFile))
    return false;

  if (FileExist(strListFile)) {
    std::string strListFileBackup = strListFile + ".bak";
    if (FileExist(strListFileBackup) && std::remove(strListFileBackup.c_str()))
      return false;

    if (std::rename(strListFile.c_str(), strListFileBackup.c_str()))
      return false;
  }

  std::fstream inFile(strDownloadedFile, std::fstream::in | std::fstream::binary);
  if (!inFile.is_open())
    return false;

  std::fstream outFile(strListFile, std::fstream::out | std::fstream::trunc | std::fstream::binary);
  if (!outFile.is_open()) {
    inFile.close();
    return false;
  }

  inFile << std::noskipws;
  std::istream_iterator<unsigned char> beginIn(inFile);
  std::istream_iterator<unsigned char> end;
  std::ostream_iterator<char> beginOut(outFile);

  std::copy(beginIn, end, beginOut);
  inFile.close();
  outFile.close();
  std::remove(strDownloadedFile.c_str());
  Initialize();

  return true;
}

/////////////////////////////////// ServerListParser class private definitions /////////////////////////////////

// reads server list file and initializes values
void ServerListParser::Initialize() {
  std::fstream inFile(strListFile, std::fstream::in);
  if (!inFile.is_open()) {
    std::fstream outFile(strListFile, std::fstream::out);
    if (!outFile.is_open())
      throw std::exception::exception("Server list file missing, attempt to create failed.");

    outFile << "# all items should start with a new line items may" << std::endl;
    outFile << "# contain space at beginning and end, urls should" << std::endl;
    outFile << "# start with http://, https://, sftp:// or ftp://" << std::endl << std::endl;
    outFile << "Version = 1.0" << std::endl << std::endl;
    outFile.close();

    inFile.open(strListFile, std::fstream::in);
    if (!inFile.is_open())
      throw std::exception::exception("Failed to open server list file for unknown reason.");
  }

  serverList.clear();
  listFileVersion = 0.0;
  listFileFound = true;
  std::string strCurrentLine;
  while (getline(inFile, strCurrentLine)) {
    Trim(strCurrentLine);
    if (!strCurrentLine.size() || strCurrentLine[0] == '#')
      continue;

    std::string strValue;
    if (listFileVersion == 0.0 && HasValue(strCurrentLine, "version", strValue)) {
      try {
        listFileVersion = (float)std::atof(strValue.c_str());
      } catch (std::exception) { continue; }
    } else if (strCurrentLine.size() > 7) {
      if (strCurrentLine.substr(0, 6) == "ftp://" ||
          strCurrentLine.substr(0, 7) == "sftp://" ||
          strCurrentLine.substr(0, 7) == "http://" ||
          strCurrentLine.substr(0, 8) == "https://") {
        PrepareURL(strCurrentLine);
        serverList.push_back(strCurrentLine);
      }
    } else
      continue;
  }

  RandomizeURLList();
  inFile.close();
}

// takes source string, finds value against specified key, sets found value into strValue and returns true if succeeds
bool ServerListParser::HasValue(const std::string& strSource, const std::string& strKey, std::string& strValue) const {
  std::string strData = strSource;
  Convert(strData, ConvertType::LOWER);
  size_t keyLen = strKey.size();
  if (strData.substr(0, keyLen) != strKey)
    return false;

  strValue = "";
  if (strData.substr(0, strKey.size()) == strKey) {
    size_t startIndex = strData.rfind("=");
    if (startIndex == std::string::npos || startIndex == strData.size())
      return false;;

    strValue = strData.substr(startIndex + 1, strData.size());
    Trim(strValue);
    return (strValue.size() != 0);
  }

  return false;
}

// returns path of server list file
std::string ServerListParser::GetPath() const {
  size_t nameStartIndex = strListFile.rfind('\\');
  std::string strPath = ".\\";
  if (nameStartIndex != std::string::npos)
    strPath = strListFile.substr(0, nameStartIndex);

  return strPath;
}

// downloads file from specified location, saves to temp file at user data location,
// compares hash value read from server with download file and returns downloadeded
// file location if passes all validation
std::string ServerListParser::Download(CURL* curl, const std::string& strLocation, const std::string& strHash) const {
  std::string strTempFile = GetPath() + "\\~tmpList.lst";
  if (FileExist(strTempFile))
    std::remove(strTempFile.c_str());

  FILE* fp = fopen(strTempFile.c_str(), "wb");
  if (fp == NULL)
    return "";

  curl_easy_setopt(curl, CURLOPT_URL, strLocation.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_list_file);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

#if USE_CERT
  // without certificate curl will not allow connection (secure connection)
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
  curl_easy_setopt(curl, CURLOPT_CAINFO, certificateFile.c_str());
#else
  // without certificate, curl will allow connection (insecure connection)
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
#endif

  CURLcode res = curl_easy_perform(curl);
  switch (res) {
    case CURLE_COULDNT_CONNECT:
    case CURLE_COULDNT_RESOLVE_HOST:
    case CURLE_REMOTE_ACCESS_DENIED:
    case CURLE_COULDNT_RESOLVE_PROXY:
    case CURLE_WRITE_ERROR:
#if USE_CERT
    case CURLE_SSL_CACERT:
    case CURLE_SSL_CERTPROBLEM:
#endif
      std::remove(strTempFile.c_str());
      fclose(fp);
      return "";
  }

  fclose(fp);
  std::string strDownlodedHash = GetHash(strTempFile);
  if (strDownlodedHash == strHash && res == CURLE_OK)
    return strTempFile;
  else
    std::remove(strTempFile.c_str());

  return "";
}

// returns hash of specified file
std::string ServerListParser::GetHash(const std::string& strFile) const {
  if (!FileExist(strFile))
    return "";

  CSHA1 sha1;
  std::wstring strReport;
  std::wstring wide_string(strFile.begin(), strFile.end());
  if (sha1.HashFile(wide_string.c_str())) {
    sha1.Final();
    sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
    std::string hashValue(strReport.begin(), strReport.end());
    return hashValue;
  }

  return "";
}

// Randomizes url list
void ServerListParser::RandomizeURLList() {
  size_t listSize = serverList.size();
  if (!listSize)
    return;

  std::vector<int> randomIndexes;
  PrepareRandomIndexes(listSize, randomIndexes);
  ServerList randomList;
  for (size_t currentIndex = 0; currentIndex < listSize; currentIndex++)
    randomList.push_back(serverList[randomIndexes[currentIndex]]);

  std::copy(randomList.begin(), randomList.end(), serverList.begin());
}

// prepares random indexes from 0 to listSize - 1 and pushes into indexList
void ServerListParser::PrepareRandomIndexes(size_t listSize, std::vector<int>& indexList) const {
  indexList.clear();
  srand((size_t)time(NULL));
  indexList.push_back(rand() % listSize);
  size_t indexCount = 1;
  while (indexCount != listSize) {
    int index = rand() % listSize;
    std::vector<int>::iterator foundValue = std::find(indexList.begin(), indexList.end(), index);
    if (foundValue != indexList.end())
      continue;

    indexList.push_back(index);
    indexCount++;
  }
}

// prepares url for windows
void ServerListParser::PrepareURL(std::string& strURL) const {
  size_t urlLen = strURL.size();
  std::string strExt = ".xml";
  std::copy(strURL.begin() + urlLen - 4, strURL.end(), strExt.begin());
  if (strExt == ".xml")
    return;

  strURL += (strURL[urlLen - 1] == '/') ? "" : "/";
  strURL += SUBURL;
}

//////////////////////////////////////// end of class ServerListParser /////////////////////////////////////////

} // namespace AutoUpdater
