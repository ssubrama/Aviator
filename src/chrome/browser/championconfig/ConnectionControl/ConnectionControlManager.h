// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef COMPONENTS_POLICY_CORE_BROWSER_CONNECTION_CONTROL_MANAGER_H_
#define COMPONENTS_POLICY_CORE_BROWSER_CONNECTION_CONTROL_MANAGER_H_

#include "chrome/browser/championconfig/connectioncontrol/rule_parser.h"
#include "base/values.h"
#include "base/prefs/pref_member.h"
#include "base/prefs/pref_service.h"
#include "components/pref_registry/pref_registry_syncable.h"

class PrefService;
namespace policy {

//typedef enum protocols { http = 0, https, ftp, ws, wss, gopher, file } protocol_tag; 
typedef std::vector<RULEParser::Rule> RuleObjList;
typedef  enum RuleListType_tag { allowprivate, denyprivate, allowpublic, denypublic } RuleListType;
  
class ConnectionControlManager {

private:  

  static std::vector<std::string> *rulearray;
  static std::vector<bool> *IsEnableVector;
  
  RuleObjList ruleobjlist;
  
	static bool championIsFirstRun;

    PrefService* pref_service_ = NULL;
	
	bool MatchPort(int portnum, RULEParser::Rule ruleobj) const;
	bool CompareURLwithRuleObj(const GURL& url, RULEParser::IP ip, bool HostIsIP, bool IsPublilc, int port, RULEParser::Rule ruleobj) const;
	bool URLExistsInRuleList(const GURL& url,RULEParser::IP ip, bool HostIsIP, bool IsPublilc, int port, RuleListType type) const;
	void updateRule(int opr, unsigned index, std::string Rule);
  void UpdatePrefsForNPCC(std::vector<std::string> strs);
	bool ISChamURLBlocked(const GURL& url) const;
	RULEParser::Rule::Protocol GetProtocalTag(const GURL& url) const;
    
    void SplitAndLoad(std::vector<std::string> strs);
	void LoadFromPref();
	
	bool IsFirefoxInstalled() const;
	bool IsGoogleChromeInstalled() const;
	bool IsSafariInstalled() const;
	
//	void UpdateToPref();
	
public:
    ConnectionControlManager(PrefService* pref_service);
    ConnectionControlManager();
	~ConnectionControlManager();

    const base::ListValue* RuleListArray;
	  static bool bUpdateNeeded;
    void updateEnable(int opr, unsigned index, bool bVal);
    void LoadRules();     
	bool IsUrlBlocked(const GURL& url);
 /*   static void AddRule(std::string rule, bool enable);
    static void EditRule(std::string rule, bool enabled, int index);
    static void DeleteRule(int index);
    static void EnableRule(int index);
    static void DisableRule(int index);
    static size_t GetRuleArraySize();
    static bool IsRuleEditable(int index);
    static bool IsRuleEnabled(int index);
    static void ClearRules();
    static std::string GetRuleAtIndex(int index);*/
	static void DecomposeRules(std::string temp, std::vector<std::string> *strs); 
	
//	  static bool IsPublicBlocked;
//    static bool IsPrivateBlocked;

  static RuleObjList AllowPrivateRuleList;
  static RuleObjList AllowPublicRuleList;
  static RuleObjList DenyPrivateRuleList;
  static RuleObjList DenyPublicRuleList;
  
  
  };

}  // namespace policy

#endif  // COMPONENTS_POLICY_CORE_BROWSER_CONNECTION_CONTROL_MANAGER_H_
