# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

#!/bin/bash

number=$(ps -Ac | grep Aviator | wc -l)

if [ $number -gt 0 ]
    then
        echo Running
else
    echo Updating Aviator...
    if [ -x "${1}/Aviator.app/Contents_update" ]; then
       mv "${1}/Aviator.app/Contents" "${1}/Aviator.app/Contents_old"
       mv "${1}/Aviator.app/Contents_update" "${1}/Aviator.app/Contents"
       rm -Rf "${1}/Aviator.app/Contents_old"
       launchctl unload "${2}/com.aviatorupdate.agent.plist"
       rm "${2}/com.aviatorupdate.agent.plist"    
    fi
fi
