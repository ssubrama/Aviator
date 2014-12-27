# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

import os
import sys


def IsAvailable():
  _winreg = None
  if sys.platform == 'win32':
    import _winreg
  elif sys.platform == 'cygwin':
    try:
      import cygwinreg as _winreg
    except ImportError:
      pass  # If not available, be safe and write 0.

  if not _winreg:
    return False

  try:
    val = _winreg.QueryValue(_winreg.HKEY_CURRENT_USER,
                             'Software\\WhiteHat\\Aviator\\split_link_installed')
    return os.path.exists(val)
  except WindowsError:
    pass

  return False


def main():
  # Can be called from gyp to set variable.
  if IsAvailable():
    sys.stdout.write('1')
  else:
    print >> sys.stderr, "Couldn't find split_link installation."
    print >> sys.stderr, ('Run python tools\\win\\split_link\\'
                          'install_split_link.py from an elevated Visual '
                          'Studio Command Prompt to install.')
    sys.stdout.write('0')
    return 1


if __name__ == '__main__':
  sys.exit(main())
