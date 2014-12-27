#!/usr/bin/env python
# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 
# champion : app name change 

import glob
import optparse
import os
import shutil
import subprocess
import sys

def reorder_imports(input_dir, output_dir, architecture):
  """Run swapimports.exe on the initial Aviator.exe, and write to the output
  directory. Also copy over any related files that might be needed
  (pdbs, manifests etc.).
  """

  input_image = os.path.join(input_dir, 'Aviator.exe')
  output_image = os.path.join(output_dir, 'Aviator.exe')

  swap_exe = os.path.join(
    __file__,
    '..\\..\\..\\third_party\\syzygy\\binaries\\exe\\swapimport.exe')

  args = [swap_exe, '--input-image=%s' % input_image,
      '--output-image=%s' % output_image, '--overwrite', '--no-logo']

  if architecture == 'x64':
    args.append('--x64');

  args.append('Aviator_elf.dll');

  subprocess.call(args)

  for fname in glob.iglob(os.path.join(input_dir, 'Aviator.exe.*')):
    shutil.copy(fname, os.path.join(output_dir, os.path.basename(fname)))
  return 0


def main(argv):
  usage = 'reorder_imports.py -i <input_dir> -o <output_dir> -a <target_arch>'
  parser = optparse.OptionParser(usage=usage)
  parser.add_option('-i', '--input', help='reorder Aviator.exe in DIR',
      metavar='DIR')
  parser.add_option('-o', '--output', help='write new Aviator.exe to DIR',
      metavar='DIR')
  parser.add_option('-a', '--arch', help='architecture of build (optional)',
      default='ia32')
  opts, args = parser.parse_args()

  if not opts.input or not opts.output:
    parser.error('Please provide and input and output directory')
  return reorder_imports(opts.input, opts.output, opts.arch)

if __name__ == "__main__":
  sys.exit(main(sys.argv[1:]))
