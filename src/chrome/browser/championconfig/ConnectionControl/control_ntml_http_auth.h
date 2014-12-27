// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef CHROME_BROWSER_CHAMPIONCONIFG_CONTROL_NTLM_HTTP_AUTH_H_
#define CHROME_BROWSER_CHAMPIONCONIFG_CONTROL_NTLM_HTTP_AUTH_H_

#include "build/build_config.h"
#include <string>
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/prefs/pref_service.h"

#include<vector>
#include<list>
#include<iostream>

namespace options{
  class ConnectionControlHandler;
}

extern PrefService* g_pref_service_for_ntlm; // champion : ntlm (utpal)

namespace RULEParser {
  class Rule;
  typedef std::vector<Rule> RuleStorage;
}

namespace NTLMControl {
  class PrefService;
  
  class ControlNtlmHttpAuth {
  public:
    ControlNtlmHttpAuth();
    ~ControlNtlmHttpAuth();
    void setIpAddress(const std::string&);
    void setPortNumber(uint16);
    void setHost(std::string& host);
    void setRequestUrl(std::string &);
    void setDomainName(std::string &);
    bool IsUrlAllowed();
    size_t getHttpNtlmwhilteListRuleSize();
    size_t getHttpNtlmblackListRuleSize();
  private:
    const std::string getIpAddress();
    const std::string getPortNumber();
    void OnUpdateExceptionList(options::ConnectionControlHandler* ccHandler);
    std::string ipaddress_;
    std::string authSchme_;
    uint16 portNo_;
    std::string host_;
    std::string requestUrl_;
    std::string domainName_;
    bool isPublic_;
    bool isAuthEnable_;
    //store the all allow list 
    std::list<std::string> whilteList_;
    //store the all block list 
    std::list<std::string> blackList_;
    //ip address part
    std::vector<std::string> ipParts_;
    //content::NotificationRegistrar registrar_;
    RULEParser::RuleStorage* whiteListedRules_;
  };
}

#endif //CHROME_BROWSER_CHAMPIONCONIFG_CONTROL_NTLM_HTTP_AUTH_H_
