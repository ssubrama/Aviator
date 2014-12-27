// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "control_ntml_http_auth.h"
#include "chrome/browser/chrome_notification_types.h"
#include "content/public/browser/notification_service.h"
#include "chrome/browser/championconfig/connectioncontrol/connection_control_handler.h"
using namespace std;

namespace NTLMControl {

ControlNtlmHttpAuth::ControlNtlmHttpAuth() : ipaddress_(std::string()), authSchme_(std::string()),
  isPublic_(false), isAuthEnable_(false), whiteListedRules_(0) {}
  
ControlNtlmHttpAuth::~ControlNtlmHttpAuth() {}

//all public method 
void ControlNtlmHttpAuth::setIpAddress(const std::string& ip) {
  if (!ip.empty())
    ipaddress_ = const_cast<std::string&>(ip);
}

void ControlNtlmHttpAuth::setPortNumber(uint16 port) {
  if (port)
    portNo_ = port;
}

void ControlNtlmHttpAuth::setHost(std::string& host) {
  if (!host.empty())
    host_ = host;
}

void ControlNtlmHttpAuth::setRequestUrl(std::string& url) {
  if (url.size())
    requestUrl_ = url;
}

void ControlNtlmHttpAuth::setDomainName(std::string& domain) {
  if (domain.size())
    domainName_ = domain;
}

size_t ControlNtlmHttpAuth::getHttpNtlmwhilteListRuleSize() {
  if (whilteList_.size() > 0)
    return whilteList_.size();
	
  return 0;
}

size_t ControlNtlmHttpAuth::getHttpNtlmblackListRuleSize() {
  if (blackList_.size() > 0)
    return blackList_.size();
	
  return 0;
}

//start private method
void ControlNtlmHttpAuth::OnUpdateExceptionList(options::ConnectionControlHandler* ccHandler) {
  whiteListedRules_ = &ccHandler->GetRuleStore().GetFilteredRules();
}

bool ControlNtlmHttpAuth::IsUrlAllowed() {
  options::ConnectionControlHandler* ccHandler(0);
  
  if (g_pref_service_for_ntlm) {
    ccHandler = new options::ConnectionControlHandler();
    ccHandler->LoadNTLMRules(g_pref_service_for_ntlm);
    RULEParser::RuleStore& ruleStore = ccHandler->GetRuleStore();
    whiteListedRules_ = &ruleStore.GetFilteredRules();

    RULEParser::IP ip(ipaddress_);

    if (!whiteListedRules_)
      return false;

    if (!whiteListedRules_->size())
      return false;

    const std::string pvtNetRule = "1;Allow connection to Private Network";
    const std::string pubNetRule = "1;Allow connection to Public Network";

    if (ruleStore.IsPrivateNetworkAllowed(pvtNetRule) && ruleStore.IsPublicNetworkAllowed(pubNetRule))
      return true;
    else if (ruleStore.IsPrivateNetworkAllowed(pvtNetRule) && ip.IsPrivate())
      return true;
    else if (ruleStore.IsPublicNetworkAllowed(pubNetRule) && !ip.IsPrivate())
      return true;
    else if (ruleStore.HostIsURL(requestUrl_))
      return true;
    else if (ruleStore.DomainIsAllowed(domainName_))
      return true;
  }

  return false;
}

} // namespace ntmlcontrol
