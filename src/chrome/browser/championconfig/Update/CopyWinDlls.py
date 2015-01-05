#!/usr/bin/env python

# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 


import os
import shutil
import getopt
import sys

def GetOsName():    
    """This function return os name i.e windows or unix or mac 
    """
    this_sys = os.name
    return this_sys

def _CopyAll(srcpath,destpath):
  """This function will copy all required aviator auto update files in proper place
  no need to copy manually.It will copy proper place based on running build mode.  
  """
  #To check All Resource file is present or not
  srcpath=srcpath.lstrip().rstrip()
  destpath=destpath.lstrip().rstrip()
  if os.path.exists(str(srcpath)):
    src_files = os.listdir(str(srcpath))
    for file_name in src_files:
      full_file_name = os.path.join(srcpath, file_name)
      full_file_name=str(full_file_name.lstrip().rstrip())
      if (os.path.isfile(full_file_name)):
        shutil.copy(full_file_name, destpath)


def Main(argv):
  '''
  sfile=''
  dfile=''
  try:
    myopts, args = getopt.getopt(sys.argv[1:],"i:o:")
  except getopt.GetoptError as e:
    print (str(e))
    print("Usage: %s -i input -o output" % sys.argv[0])
    sys.exit(2)
 
  for o, a in myopts:
    if o == '-i':
        sfile=a
    elif o == '-o':
        dfile=a
        
  if len(args) > 0:
    print >>sys.stderr, parser.get_usage()
    return 1
  '''
  path = os.path.abspath(os.path.split(__file__)[0])
  print os.path.split(path)[0]
  os_name=''
  os_name=GetOsName()
  if os_name == "nt":
    # print "Windows"
    _CopyAll(sfile.lstrip().rstrip(),dfile.lstrip().rstrip())
  else:
    pass
   
if __name__ == '__main__':
  sys.exit(Main(sys.argv[1:]))			
