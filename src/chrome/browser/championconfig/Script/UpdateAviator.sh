# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# ./AviatorEngine dryrunticket -store /Library/Aviator/TicketStore
# ./AviatorEngine runticket -store /Library/Aviator/TicketStore


#!/bin/bash
#

# Determine directory where this script is running from
#script_dir=$(dirname $(echo $0 | sed -e "s,^\([^/]\),$(pwd)/\1,"))
script_dir="${1}"
agent="$script_dir"/AviatorEngine

if [ ! -x "$agent" ]; then
    echo "Can't figure out how to update now."
    exit 1
fi

# checking whether update is required
ticket="$script_dir"/AviatorTicket

number=$(ps -Ac | grep AviatorEngine | wc -l)

if [ $number -eq 0 ]
then
  result=$("$agent" dryrunticket -store "${ticket}")
  if [[ $result = *com.whitehatsec.aviator* ]]
  then
      echo "Update is available for Aviator"
     "$agent" runticket -store "${ticket}"
  fi
fi

# if update is required, run this step to update Aviator



