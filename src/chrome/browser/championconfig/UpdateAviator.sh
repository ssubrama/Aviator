# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# 1. Need to check whether update is required?
# 2. If yes, runt he update.

# ./AviatorEngine dryrunticket -store /Library/Aviator/TicketStore
# ./AviatorEngine runticket -store /Library/Aviator/TicketStore
# Determine directory where this script is running from
#script_dir=$(dirname $(echo $0 | sed -e "s,^\([^/]\),$(pwd)/\1,"))

NEWLOG="************************ Aviator Auto Update *******************"
ENDLOG="------------------------ End -----------------------------------"
CUSRNAME=`id -nu` 
LOG_ENABLED=yes
LOG_FILE="/Users/$CUSRNAME/Library/Logs/AviatorAutoUpdate.log"
DATE=`date`" $LOGDATEFORMAT"
TIME=`date +"%r"`
option_enabled () {
    VAR="$1"
    VAR_VALUE=$(eval echo \$$VAR)
    if [[ "$VAR_VALUE" == "y" ]] || [[ "$VAR_VALUE" == "yes" ]]
    then
        return 1
    else
        return 0
    fi
}

function has_value {
		
		if [ -z "$1" ];then
			return 0
		else
			return 1
		fi	
}

function defined {
    [[ ${!1-X} == ${!1-Y} ]]
}

function init() {
			
		if [ -f "${LOG_FILE}" ]; then
				log "$NEWLOG"
				log "Aviator Auto Update Started"
		else
				touch "$LOG_FILE"
				log "$NEWLOG" 
				log "Aviator Auto Update Started"
		fi
									
}

log () {
				
        LOG_MESSAGE="$1" 
        STATE="$2"

        if has_value "$LOG_MESSAGE"
        then
            LOG_STRING="$DATE"": - ""$STATE ""$LOG_MESSAGE"
        else
            LOG_STRING="$DATE"": - ""$STATE ""$LOG_MESSAGE"
        fi

        if option_enabled "$LOG_ENABLED"
        then
            echo "$LOG_STRING" >> "$LOG_FILE"
        fi
       
}
script_dir=''

function findUpdateUrl(){
	
	log "Going to Download File"
	#server list file name
	SERVERLISTFILE=ServerList.lst
	
	#check file is present locally or not
	if [ -f "$SERVERLISTFILE" ];
	then
		log "File present but checking for updated serverlist file to Download File" "$SERVERLISTFILE" 
		downloadServerList
	else
		log "File Not present Locally.Need to Download File" "$SERVERLISTFILE"
		downloadServerList
	fi
	"$agent" runticket -store "${ticket}"
	log "Executing Aviator Engine for downloading SeverList file"
}

function downloadServerList(){
	
	log "Creatring Temporary ticket file for serverlist file"
	#first serverlist ticket into existing Aviatorticket file
	createServerListTicketFile
	
	#agent file
	local agent="$script_dir"/AviatorEngine
	
	#Ticket file
	local ticket="$script_dir"/AviatorServerTicket
	
	if [ ! -f "$agent" ]; then
    log "Can't figure out how to update now."
    exit 1
	fi
	#download serverList file
	result=$("$agent" dryrunticket -store "${ticket}")
  if [[ $result = *com.whitehatsec.serverlist* ]]
  then
  	log "Updated ServerList File Present .Start to download "
		"$agent" runticket -store "${ticket}"
		if [ "$?" -eq 0 ]
		then
			log "Successfully download serverlist file from live server..."
		else
			log "Failed to download serverlist file from live server..."
		fi	
	fi
}

function createServerListTicketFile(){
  #Ticket file
	TICKETPATH="$script_dir""/AviatorServerTicket"
	PRODUCTID="com.whitehatsec.serverlist"
	VERSION="1.00"
	URL="https://whitehat.photoninfotech.com/whitehat/update.plist"
	PATH="$script_dir"
	
	#COMPOSE COMMAND FOR ADDING TICKET FOR EACH URL
		
	CMD="$agent"" add  -store ""$TICKETPATH"" -productid ""$PRODUCTID"" -version ""$VERSION"" -url ""$URL"" -xcpath ""$PATH" 
	log "Creating command for download file" $CMD
	$CMD
}

#Main entry point of this script
init

script_dir="${1}"
log "Reading Installation information " $script_dir
agent="$script_dir"/AviatorEngine
log "Locating Aviator Engine" $agent

if [ ! -x "$agent" ]; then
    log "AviatorEngine missing. Can't figure out how to update now"
    exit 1
fi


ticket="$script_dir"/AviatorTicket
log "Loading Ticket file information from " $ticket
#get update server url list for live server 
findUpdateUrl

