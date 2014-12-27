// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 


#pragma once

#include <string>
#include <hash_map>

using namespace std;
using namespace stdext;

class ChampionConfigure
{
private:
   
    static ChampionConfigure *single;

	/*bool defaultIncognitoOption;
	bool predictionService;
	bool webserviceNavErrors;*/

	string GetStringvalue (string key);
    int GetIntvalue (string key);
    int GetBooleanValue (string Key);
	hash_map<string, int> VarsConfig_hash;
	typedef pair <string, int> Int_Pair;
    hash_map<string, int> ::iterator Vars_Iterator;
	void OpenConfigFile(string path);
    ChampionConfigure()
    {
		VarsConfig_Init();
        /*defaultIncognitoOption = INCOGNITO_MODE;// this has to be read from Property file??
		predictionService = PREDICTION_SERVICE;
		webserviceNavErrors = WEBSERVICE_NAVERROS;*/
		//1. read the propoerty file heres
		//2. Populate them into a data structure (HashTable preferred);
		//3. Add the compile time settings like the above defaultIncognitoOption also into hastable
		//4. getPredictionService Methoid Body will call GetBooleanValue("predictionService") and return the boolean as you have returned
	//	VarsConfig_hash["defaultIncognitoOption"] = 1;
	//	VarsConfig_hash["predictionService"] = 0;
//		VarsConfig_hash["webserviceNavErrors"] = 0;
    }

	
	
public:
	/*hash_map<string, int> VarsConfig_hash;
	typedef pair <string, int> Int_Pair;
    hash_map<string, int> ::iterator Vars_Iterator;
	void VarsConfig_Init();
	void OpenConfigFile(string path);*/
	void VarsConfig_Init();
    static ChampionConfigure* getInstance();
    ~ChampionConfigure();
  	int IsPrivateBrowsingOn();
	int IsNavigatetorErrorOn();
	int IsPredictionServiceOn();
	int IsBlockThirdPartyCookies();
	int IsShowHomeButton();
	int IsClearCookies();
	int IsClearBrowsingDataPref();
	int IsSyncOption();
	int IsThemeSelection();
	int IsGooleInstant();
	int IsCloudPrint();
	int IsSigninToChampion();
	int IsSynOptionFromWrench();
	int IsDeveloperTool();
	int IsEncryptedGoogleSearch();
    int IsReportToanIssue();
	int IsHelpOption();
	int IsDisableBookmark();
	int IsTranslateToEnglish();
	int IsInspectElement();
	int IsDeveloperToolFromRightclick();
};

