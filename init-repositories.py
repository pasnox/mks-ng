#!/usr/bin/python

import sys
import os
import stat
import fileinput

class RepositoryInitializer:
    # errors
    EC_CANT_CREATE_PATH = 1
    EC_CANT_EXPORT = 2
    EC_CANT_UPDATE = 3
    # the modules to clone / check out
    modules = {
        'git':
            {
                'src_highlite': 'git://git.savannah.gnu.org/src-highlite.git',
                'src_highlite_qt': 'git://srchiliteqt.git.sourceforge.net/gitroot/srchiliteqt/srchiliteqt',
                'fresh': 'git://github.com/pasnox/fresh.git'
            },
        'svn':
            {
                'ctags': 'https://ctags.svn.sourceforge.net/svnroot/ctags/trunk'
            }
    }
    
    def __init__(self):
        os.environ[ 'PATH' ] = '/opt/local/bin:/opt/bin:%s' % ( os.environ[ 'PATH' ] )

    # $1 system
    # $2 uri
    # $3 folder
    def systemExport(self, cvs, uri, folder)
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

    def moduleExport(self, cvs, module, uri):
        print '--- Updating module %s...' % ( module )
        
        folder = '' #`thirdPartyFolderFilePath "$2.$1"`
        
        if not os.path.isdir( folder ):
            systemExport( cvs, uri, folder )
        
        cwd = os.getcwd();
        os.chdir( folder )
        systemUpdate( cvs );
        os.chdir( cwd )
    
    def run(self):
        for cvs in RepositoryInitializer.modules.keys():
            cvsModules = RepositoryInitializer.modules[ cvs ]
            for module in cvsModules.keys():
                uri = cvsModules[ module ]
                moduleExport( cvs, module, uri )

def main(argv=None):
    if argv is None:
        argv = sys.argv

    initializer = RepositoryInitializer()
    return initializer.run()

if __name__ == "__main__":
    sys.exit( main() )


    
'''
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


'''
