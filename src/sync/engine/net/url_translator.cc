// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

// Contains the definition of a few helper functions used for generating sync
// URLs.

#include "sync/engine/net/url_translator.h"

#include "base/basictypes.h"
#include "base/port.h"
#include "net/base/escape.h"

using std::string;

namespace syncer {

namespace {
// Parameters that the server understands. (here, a-Z)
const char kParameterClient[] = "client";
const char kParameterClientID[] = "client_id";

#if defined(GOOGLE_CHROME_BUILD)
const char kClientName[] = "Google Chrome";
#else
const char kClientName[] = "Aviator"; // champion : app name changes (amresh)
#endif  // defined(GOOGLE_CHROME_BUILD)
}

// Convenience wrappers around CgiEscapePath().
string CgiEscapeString(const char* src) {
  return CgiEscapeString(string(src));
}

string CgiEscapeString(const string& src) {
  return net::EscapeUrlEncodedData(src, true);
}

// This method appends the query string to the sync server path.
string MakeSyncServerPath(const string& path, const string& query_string) {
  string result = path;
  result.append("?");
  result.append(query_string);
  return result;
}

string MakeSyncQueryString(const string& client_id) {
  string query;
  query += kParameterClient;
  query += "=" + CgiEscapeString(kClientName);
  query += "&";
  query += kParameterClientID;
  query += "=" + CgiEscapeString(client_id);
  return query;
}

}  // namespace syncer
