#!/bin/bash

# declare associative arrays
declare -A gitList
declare -A svnList
declare -A modules

# exit codes
EC_CANT_CREATE_PATH=1
EC_CANT_EXPORT=2
EC_CANT_UPDATE=3

# git
gitList[ src_highlite ]="git://git.savannah.gnu.org/src-highlite.git"
gitList[ src_highlite_qt ]="git://srchiliteqt.git.sourceforge.net/gitroot/srchiliteqt/srchiliteqt"
gitList[ fresh ]="git://github.com/pasnox/fresh.git"
# svn
svnList[ ctags ]="https://ctags.svn.sourceforge.net/svnroot/ctags/trunk"
# modules
modules[ git ]="${!gitList[*]}"
modules[ svn ]="${!svnList[*]}"

#################################### START FUNCTIONS#####################################

# avoid the print of pushd stack
pushd()
{
    builtin pushd "$@" > /dev/null;
}

# $1 folder name
thirdPartyFolderFilePath()
{
    _thirdPartyName="3rdparty"
    
    if [ ! -d "$_thirdPartyName" ] ; then
        mkdir -p "$_thirdPartyName" || exit $EC_CANT_CREATE_PATH
    fi
    
    echo "$_thirdPartyName/$1"
}

# $1 system
# $2 uri
# $3 folder
systemExport()
{
    if [ "$1" == "git" ] ; then
        if [ -z "$3" ] ; then
            git clone "$2" || exit $EC_CANT_EXPORT
        else
            git clone "$2" "$3" || exit $EC_CANT_EXPORT
        fi
    elif [ "$1" == "svn" ] ; then
        if [ -z "$3" ] ; then
            svn co -q "$2" || exit $EC_CANT_EXPORT
        else
            svn co -q "$2" "$3" || exit $EC_CANT_EXPORT
        fi
    fi
}

# $1 system
# $2 folder
systemUpdate()
{
    if [ "$1" == "git" ] ; then
        if [ -z "$2" ] ; then
            git pull || exit $EC_CANT_UPDATE
        else
            git pull "$2" || exit $EC_CANT_UPDATE
        fi
    elif [ "$1" == "svn" ] ; then
        if [ -z "$2" ] ; then
            svn update || exit $EC_CANT_UPDATE
        else
            svn update "$2" || exit $EC_CANT_UPDATE
        fi
    fi
}

# $1 system
# $2 name
# $3 uri
moduleExport()
{
    echo "--- Updating module $2..."
    
    folder=`thirdPartyFolderFilePath "$2.$1"`
    
    if [ ! -d "$folder" ] ; then
        systemExport "$1" "$3" "$folder"
    fi
    
    pushd "$folder"
    systemUpdate "$1"
    pushd
}

#################################### END FUNCTIONS#####################################

for system in ${!modules[*]}
do
    for name in ${modules[ $system ]}
    do
        uri=($(eval echo \${"$system"List[ $name ]}))
        moduleExport "$system" "$name" "$uri"
    done
done
