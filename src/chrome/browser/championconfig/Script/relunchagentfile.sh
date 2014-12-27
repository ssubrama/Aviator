# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

#!/bin/bash

ARGCOUNT=1
E_WRONGARGS=70
E_FILE_NOT_EXIST=71

if [ $# -ne "$ARGCOUNT" ]
then
# Filename arg must be passed to script.
  echo "Usage: `basename $0` Agent_filename_path"
  exit $E_WRONGARGS
fi

if [ ! -e "$1" ]
then
  echo "File \""$1"\" does not exist."
  exit $E_FILE_NOT_EXIST
fi
# AGENTPLISTPATH="$argv[1]"

# checking whether agent for time interval update is running already or not.
result=$(launchctl list | grep com.aviator.keystone.agent | wc -l)

if [ $result -eq 0 ]
then 
    `launchctl load "${1}"`
fi    

