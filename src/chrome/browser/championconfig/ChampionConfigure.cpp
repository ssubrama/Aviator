// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include <fstream>
#include "ChampionConfigure.h"

using namespace std;

 string trim( string & source, string delims = " \t\r\n") 
 {
   string result(source);
   string::size_type index = result.find_last_not_of(delims);
   if(index !=  string::npos)
    result.erase(++index);

   index = result.find_first_not_of(delims);
   if(index !=  string::npos)
     result.erase(0, index);
   else
     result.erase();
   return result;
}
 /*
 int& ChampionConfigure :: operator[](string index)
 {
	 return VarsConfig_hash[index];
 }*/
 void ChampionConfigure :: OpenConfigFile(string path)
 {
   
   ifstream file(path.c_str());
   string line;
   string name;
   string value;
   int keyvalue;
   string inSection;
  int posEqual;
  while ( getline(file,line))
  {

    if (! line.length()) continue;

    if (line[0] == '#') continue;
	if (line[0] == ' ') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection=trim(line.substr(1,line.find(']')-1));
      continue;
    }

    posEqual=line.find('=');
    name  = trim(line.substr(0,posEqual)).c_str();
	//name=inSection+'/'+name;
    value = trim(line.substr(posEqual+1)).c_str();
	printf("%s--->%s ",name ,value);
	value=value.c_str();
	if(value == "1")
		keyvalue=1;
	else
		keyvalue=0;
	//VarsConfig_hash[name]=value;
	VarsConfig_hash.insert( Int_Pair(std::string(name),keyvalue) );
  /*  for(Vars_Iterator = VarsConfig_hash.begin( ); Vars_Iterator != VarsConfig_hash.end( ); Vars_Iterator++)
		  printf("%s--->%d ",(Vars_Iterator ->first) ,(Vars_Iterator->second)); */
    
  }
}
 
ChampionConfigure* ChampionConfigure::single = NULL;
ChampionConfigure::~ChampionConfigure()
{
    if(single)
		delete single;
	else
		;

}

ChampionConfigure* ChampionConfigure::getInstance()
{
    if(single == NULL)    
        single = new ChampionConfigure();              
	return single;
}

void ChampionConfigure::VarsConfig_Init()
{
	/*bool rtnBool = false;

	
	rtnBool = true;
	return rtnBool;*/
	OpenConfigFile("..\\config.txt");
}

int ChampionConfigure::GetBooleanValue (string key)
{
	/*for(Vars_Iterator = VarsConfig_hash.begin( ); Vars_Iterator != VarsConfig_hash.end( ); Vars_Iterator++)
	{
		 //printf("%s--->%d ",(Vars_Iterator ->first) ,(Vars_Iterator->second)); 
		string temp=Vars_Iterator->first;
		//name  = trim(line.substr(0,posEqual)).c_str();
		int pos=temp.find('/');
		int totallen=temp.length();
		string tempkey=trim(temp.substr((pos+1),(totallen-1))).c_str();
		//temp=trim(temp.substr(0,pos)).c_str();
		if( key == tempkey )
			//key=temp+'/'+ key;
			return GetIntvalue(temp);
		else
		  // throw "InvalidKey";
		  continue;
	  
	}
	return 0;*/
	Vars_Iterator = VarsConfig_hash.find(key);
	if ( Vars_Iterator != VarsConfig_hash.end( ) )
	    return VarsConfig_hash[key];
		//return (Vars_Iterator->second);
	else
		throw "InvalidKey";
	return 0; 
}

int ChampionConfigure::IsPrivateBrowsingOn()
{
	return GetBooleanValue ("defaultIncognitoOption");
}

int ChampionConfigure::IsNavigatetorErrorOn()
{
	return GetBooleanValue ("webserviceNavErrors");
}
int ChampionConfigure :: IsPredictionServiceOn()
{
	return GetBooleanValue ("predictionService");
}
int ChampionConfigure ::IsBlockThirdPartyCookies()
{
	return GetBooleanValue ("blockthirdpartycookies");
}
int ChampionConfigure :: IsClearBrowsingDataPref()
{
	return GetBooleanValue ("clearbrowsingdatapref");
}
int ChampionConfigure ::IsShowHomeButton()
{
	return GetBooleanValue ("showhomebutton");         
}
int ChampionConfigure ::IsClearCookies()
{
	return GetBooleanValue ("clearcookies");
}
int ChampionConfigure ::IsSyncOption()
{
	return GetBooleanValue ("syncoption");
}
int ChampionConfigure ::IsGooleInstant()
{
	return GetBooleanValue ("googleinstant");
}
int ChampionConfigure ::IsThemeSelection()
{
	return GetBooleanValue ("themesection");
}
int ChampionConfigure ::IsCloudPrint()
{
	return GetBooleanValue ("cloudprint");
}
int ChampionConfigure :: IsSigninToChampion()
{
	return GetBooleanValue ("Signintochampion");
}
int ChampionConfigure ::IsSynOptionFromWrench()
{
	return GetBooleanValue ("syncoptionfromwrench");
}
int ChampionConfigure :: IsDeveloperTool()
{
	return GetBooleanValue ("Developertool");
}
int ChampionConfigure :: IsEncryptedGoogleSearch()
{
	return GetBooleanValue ("encryptedgoogleserch");
}
int ChampionConfigure :: IsReportToanIssue()
{
	return GetBooleanValue ("reportanissue");
}
int ChampionConfigure :: IsHelpOption()
{
	return GetBooleanValue ("helpoption");
}
int ChampionConfigure ::IsDisableBookmark()
{
	return GetBooleanValue ("disablebookmark");
}
int ChampionConfigure :: IsTranslateToEnglish()
{
	return GetBooleanValue ("translatetoenglish");
}
int ChampionConfigure :: IsInspectElement()
{
	return GetBooleanValue ("inspectelement");
}
int ChampionConfigure ::IsDeveloperToolFromRightclick()
{
	return GetBooleanValue ("Developertoolfromright");
}
string ChampionConfigure::GetStringvalue (string key)
{
	/*Check if HashTable has the key prosent
	if yes then return the value and before returning typecaset as string
	else
	throw exception 
	*/
	 Vars_Iterator = VarsConfig_hash.find(key);
		if ( Vars_Iterator != VarsConfig_hash.end( ) )
			return (Vars_Iterator->first);
		else
			throw "InvalidKey";
		return "5";
		
}


int ChampionConfigure::GetIntvalue(string key)
{
	/*
	Check if HashTable has the key prosent
	if yes then return the value and before returning typecaset as Int
	else
	throw exception 
	*/
	    Vars_Iterator = VarsConfig_hash.find(key);
		if ( Vars_Iterator != VarsConfig_hash.end( ) )
			return (Vars_Iterator->second);
		else
			throw "InvalidKey";
		return 5;

}
