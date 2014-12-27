#!/bin/bash

# Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
# in this file are licensed under the BSD-3-clause license (the "License") included in 
# the WhiteHat-LICENSE file included in the root directory of the distributed source code 
# archive. Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
# ANY KIND, either express or implied. See the License for the specific language governing 
# permissions and limitations under the License. 

# Copies a framework to its new home, "unversioning" it.
#
# Normally, frameworks are versioned bundles.  The contents of a framework are
# stored in a versioned directory within the bundle, and symbolic links
# provide access to the actual code and resources.  See
# http://developer.apple.com/mac/library/documentation/MacOSX/Conceptual/BPFrameworks/Concepts/FrameworkAnatomy.html
#
# The symbolic links usually found in frameworks create problems.  Symbolic
# links are excluded from code signatures.  That means that it's possible to
# remove or retarget a symbolic link within a framework without affecting the
# seal.  In Chrome's case, the outer .app bundle contains a framework where
# all application code and resources live.  In order for the signature on the
# .app to be meaningful, it encompasses the framework.  Because framework
# resources are accessed through the framework's symbolic links, this
# arrangement results in a case where the resources can be altered without
# affecting the .app signature's validity.
#
# Indirection through symbolic links also carries a runtime performance
# penalty on open() operations, although open() typically completes so quickly
# that this is not considered a major performance problem.
#
# To resolve these problems, the frameworks that ship within Chrome's .app
# bundle are unversioned.  Unversioning is simple: instead of using the
# original outer .framework directory as the framework that ships within the
# .app, the inner versioned directory is used.  Instead of accessing bundled
# resources through symbolic links, they are accessed directly.  In normal
# situations, the only hard-coded use of the versioned directory is by dyld,
# when loading the framework's code, but this is handled through a normal
# Mach-O load command, and it is easy to adjust the load command to point to
# the unversioned framework code rather than the versioned counterpart.
#
# The resulting framework bundles aren't strictly conforming, but they work
# as well as normal versioned framework bundles.
#
# An option to skip running install_name_tool is available. By passing -I as
# the first argument to this script, install_name_tool will be skipped. This
# is only suitable for copied frameworks that will not be linked against, or
# when install_name_tool will be run on any linker output when something is
# linked against the copied framework. This option exists to allow signed
# frameworks to pass through without subjecting them to any modifications that
# would break their signatures.

set -e

RUN_INSTALL_NAME_TOOL=1
if [ $# -eq 3 ] && [ "${1}" = "-I" ] ; then
  shift
  RUN_INSTALL_NAME_TOOL=
fi

if [ $# -ne 2 ] ; then
  echo "usage: ${0} [-I] FRAMEWORK DESTINATION_DIR" >& 2
  exit 1
fi

# FRAMEWORK should be a path to a versioned framework bundle, ending in
# .framework.  DESTINATION_DIR is the directory that the unversioned framework
# bundle will be copied to.
#FILEPATH="$HOME/abcd.txt"
FRAMEWORK="${1}"
SCR_PATH="${1}"
#echo  "SCR_PATH=""$SCR_PATH" >> $FILEPATH
#( `cd ../../..` )
LOC=`pwd`
#echo  "LOC=""$LOC" >> $FILEPATH
FINAL="$LOC""/browser/championconfig"
#echo  "FINAL=""$FINAL" >> $FILEPATH
InternetPlugins="Internet Plug-Ins"
ExternalExtension="External Extensions"

if [ -d "$FRAMEWORK" ] ; then
    cd "$FRAMEWORK"
    if [ ! -d "$InternetPlugins" ] ; then
     mkdir "$InternetPlugins"
    fi 
    if [ ! -d "$ExternalExtension" ] ; then
     mkdir "$ExternalExtension"
    fi 
    #echo "Internet Plugin Directory=""$InternetPlugins" >> $FILEPATH
fi


#echo  "FRAMEWORK=""$FRAMEWORK" >> $FILEPATH
DESTINATION_DIR="${2}"
#echo  "DESTINATION_DIR=""$DESTINATION_DIR" >> $FILEPATH
#added by utpal for apple framework structure (versioned bundle) champion
COPYWITHSYMLINK="${1}"
FRMWORKSRCPATH="${2}"
#echo  "COPYWITHSYMLINK=""$COPYWITHSYMLINK" >> $FILEPATH
#echo  "FRMWORKSRCPATH=""$FRMWORKSRCPATH" >> $FILEPATH
#end

FRAMEWORK_NAME="$(basename "${FRAMEWORK}")"
if [ "${FRAMEWORK_NAME: -10}" != ".framework" ] ; then
  echo "${0}: ${FRAMEWORK_NAME} does not end in .framework" >& 2
  exit 1 
fi
FRAMEWORK_NAME_NOEXT="${FRAMEWORK_NAME:0:$((${#FRAMEWORK_NAME} - 10))}"
#echo  "FRAMEWORK_NAME=""$FRAMEWORK_NAME" >> $FILEPATH
# Find the current version.
VERSIONS="${FRAMEWORK}/Versions"
CURRENT_VERSION_LINK="${VERSIONS}/Current"
CURRENT_VERSION_ID="$(readlink "${VERSIONS}/Current")"
CURRENT_VERSION="${VERSIONS}/${CURRENT_VERSION_ID}"

# Make sure that the framework's structure makes sense as a versioned bundle.
if [ ! -e "${CURRENT_VERSION}/${FRAMEWORK_NAME_NOEXT}" ] ; then
  echo "${0}: ${FRAMEWORK_NAME} does not contain a dylib" >& 2
  exit 1
fi

DESTINATION="${DESTINATION_DIR}/${FRAMEWORK_NAME}"
#echo  "DESTINATION=""$DESTINATION" >> $FILEPATH

##added by utpal for apple framework structure (versioned bundle) champion
#Now copy the entaire version bundle within the framework by utpal for champion 
#default_apps shuold place inside Aviator Framework.framework otherwise disconnect will not install/load automatically default-extension disconnect 
DEFAULT_EXTENSION_PATH="${DESTINATION}/""Versions/A/Default Apps"
rsync -lra --exclude=*.{git,svn,pyc} "${COPYWITHSYMLINK}" "${FRMWORKSRCPATH}"

#echo  "DEFAULT_EXTENSION_PATH=""$DEFAULT_EXTENSION_PATH" >> $FILEPATH
if [ -d "$DEFAULT_EXTENSION_PATH" ];then
	#echo  "inside""$DEFAULT_EXTENSION_PATH" >> $FILEPATH below mv modify for getting waring and exit now it will work fine
	rsync -ra -exclude=*.{git,svn,pyc} "$DEFAULT_EXTENSION_PATH" "$DESTINATION"
	rm -rf "$DEFAULT_EXTENSION_PATH" 
fi
#end
#now we should provide the current version path to the linker so that it can link right(current) resoures for champion

#end

#<<COMMENT1
#block below code by utpalendu ray for app store framework format structure for champion
# Copy the versioned directory within the versioned framework to its
# destination location.
#mkdir -p "${DESTINATION_DIR}"
#below code i.e rsync command modified by utaplendu ray for automatically copy version bundle 
#rsync -rcC --delete --exclude Headers --exclude PrivateHeaders \
#    --include '*.so' "${CURRENT_VERSION}/" "${DESTINATION}"
#COMMENT1
if [[ -n "${RUN_INSTALL_NAME_TOOL}" ]]; then
  # Adjust the Mach-O LC_ID_DYLIB load command in the framework.  This does not
  # change the LC_LOAD_DYLIB load commands in anything that may have already
  # linked against the framework.  Not all frameworks will actually need this
  # to be changed.  Some frameworks may already be built with the proper
  # LC_ID_DYLIB for use as an unversioned framework.  Xcode users can do this
  # by setting LD_DYLIB_INSTALL_NAME to
  # $(DYLIB_INSTALL_NAME_BASE:standardizepath)/$(WRAPPER_NAME)/$(PRODUCT_NAME)
  # If invoking ld via gcc or g++, pass the desired path to -Wl,-install_name
  # at link time.
  FRAMEWORK_DYLIB="${DESTINATION}/${FRAMEWORK_NAME_NOEXT}"
  LC_ID_DYLIB_OLD="$(otool -l "${FRAMEWORK_DYLIB}" |
                         grep -A10 "^ *cmd LC_ID_DYLIB$" |
                         grep -m1 "^ *name" |
                         sed -Ee 's/^ *name (.*) \(offset [0-9]+\)$/\1/')"
  VERSION_PATH="/Versions/${CURRENT_VERSION_ID}/${FRAMEWORK_NAME_NOEXT}"
  LC_ID_DYLIB_NEW="$(echo "${LC_ID_DYLIB_OLD}" |
                     sed -Ee "s%${VERSION_PATH}$%/${FRAMEWORK_NAME_NOEXT}%")"

  if [ "${LC_ID_DYLIB_NEW}" != "${LC_ID_DYLIB_OLD}" ] ; then
    install_name_tool -id "${LC_ID_DYLIB_NEW}" "${FRAMEWORK_DYLIB}"
  fi
fi

#This below line added for coping All Aviator update copy into proper place
#added by utpal for log file
#FILE="/Users/whitehat/aa.txt"
#echo "-->${VERSION_PATH}" >> $FILEPATH
#echo "@@@@->${DESTINATION}" >> $FILEPATH
#echo "$CURRENT_VERSION"	>> $FILEPATH

DES="${DESTINATION}""/Resources/"
USERNAMEF=`id -nu`
SCRIPT="$FINAL"
PYTHONPATH=`which python`

PDFPLUGINDEST="${DESTINATION}""/Internet Plug-Ins/"
PDFPLUGINSRC="$FINAL""/PDFPlugin/"

ExternalExtensionDEST="${DESTINATION}""/External Extensions/"
ExternalExtensionSRC="$FINAL""/External Extensions/"
#echo "PDFPLUGINDEST=""$PDFPLUGINDEST" >> $FILEPATH
#echo "DES=""$DES" >> $FILEPATH
#echo "PYTHONPATH=$PYTHONPATH"	>> $FILEPATH
#echo "SCRIPT/copy_aviator_file.py=$SCRIPT/copy_aviator_file.py"	>> $FILEPATH
#echo "Allresource file=$SCRIPT/script"	>> $FILEPATH
#echo "Dest Location=$DES"	>> $FILEPATH
"$PYTHONPATH"	"$SCRIPT/copy_aviator_file.py" "-i $SCRIPT/script/" "-o $DES"
#echo "DES=""$DES" >> $FILEPATH

if [ -d "$PDFPLUGINDEST" ] ; then
    cp -r "$PDFPLUGINSRC" "$PDFPLUGINDEST"
    #echo "Internet Plugin Directory=""$InternetPlugins" >> $FILEPATH
fi

if [ -d "$ExternalExtensionDEST" ] ; then
    cp -r "$ExternalExtensionSRC" "$ExternalExtensionDEST"
    #echo "Internet Plugin Directory=""$InternetPlugins" >> $FILEPATH
fi

#"$PYTHONPATH"	"$SCRIPT/copy_aviator_pdfplugin.py" "-i $PDFPLUGINSRC" "-o $PDFPLUGINDEST"

#end of utpal