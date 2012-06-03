#!/usr/bin/python

import sys
import os
import platform
import shlex
import subprocess
import tarfile
import urllib2
import shutil

class RepositoryInitializer:
    # errors
    EC_OK = 0
    EC_CANT_CREATE_PATH = 1
    EC_CANT_EXPORT = 2
    EC_CANT_UPDATE = 3

    # the modules to clone / check out
    modules = {
        'git':
            {
                'document-properties-discover': 'git://github.com/pasnox/document-properties-discover.git',
                'source-highlight': 'git://git.savannah.gnu.org/src-highlite.git',
                'source-highlight-qt': 'git://srchiliteqt.git.sourceforge.net/gitroot/srchiliteqt/srchiliteqt',
                'fresh': 'git://github.com/pasnox/fresh.git',
                'kate': 'git://anongit.kde.org/kate'
            },
        'svn':
            {
                'ctags': 'https://ctags.svn.sourceforge.net/svnroot/ctags/trunk'
            },
        'tar':
            {
                'qscintilla2': 'http://www.riverbankcomputing.co.uk/static/Downloads/QScintilla2/QScintilla-gpl-2.6.1.tar.gz',
                'qfreedesktopmime': 'http://qt-apps.org/CONTENT/content-files/86454-qfreedesktopmime-1.0.1.tar.bz2'
            }
    }

    def __init__(self):
        if not platform.system() == 'Windows':
            os.environ[ 'PATH' ] = '/opt/local/bin:/opt/bin:%s' % ( os.environ[ 'PATH' ] )

    def execute(self, command, workingDirectory = None, showError = False, showExecInfo = False, usePipe = False):
        if showExecInfo:
            if workingDirectory:
                print ' - From: %s' % ( workingDirectory )
            print ' - Executing: %s' % command
        args = shlex.split( command )
        if usePipe:
            runner = subprocess.Popen( args, cwd = workingDirectory, stdout = subprocess.PIPE, stderr= subprocess.STDOUT, shell = False )
        else:
            runner = subprocess.Popen( args, cwd = workingDirectory, shell = False )
        output, error = runner.communicate()
        if runner.returncode != 0:
            if showError:
                print ' - Exit Code: %i' % ( runner.returncode )
                print output
            return False
        return True

    def thirdPartyFolderFilePath(self, folderName):
        thirdPartyName = '3rdparty'
        if not os.path.exists( thirdPartyName ):
            os.makedirs( thirdPartyName )
            if not os.path.exists( thirdPartyName ):
                return RepositoryInitializer.EC_CANT_CREATE_PATH
        return '%s/%s' % ( thirdPartyName, folderName )

    def systemUpdate(self, cvs, folder = None):
        if cvs == "git":
            if folder:
                if not self.execute( 'git pull "%s"' % ( folder ) ):
                    return RepositoryInitializer.EC_CANT_UPDATE
            else:
                if not self.execute( 'git pull' ):
                    return RepositoryInitializer.EC_CANT_UPDATE
        elif cvs == "svn":
            if folder:
                if not self.execute( 'svn update "%s"' % ( folder ) ):
                    return RepositoryInitializer.EC_CANT_UPDATE
            else:
                if not self.execute( 'svn update' ):
                    return RepositoryInitializer.EC_CANT_UPDATE
        else:
            print 'Already up-to-date.'
        return RepositoryInitializer.EC_OK

    def systemExport(self, cvs, uri, folder):
        if cvs == "git":
            if folder:
                if not self.execute( 'git clone "%s" "%s"' % ( uri, folder ) ):
                    return RepositoryInitializer.EC_CANT_EXPORT
            else:
                if not self.execute( 'git clone "%s"' % ( uri ) ):
                    return RepositoryInitializer.EC_CANT_EXPORT
        elif cvs == "svn":
            if folder:
                if not self.execute( 'svn co -q "%s" "%s"' % ( uri, folder ) ):
                    return RepositoryInitializer.EC_CANT_EXPORT
            else:
                if not self.execute( 'svn co -q "%s"' % ( uri ) ):
                    return RepositoryInitializer.EC_CANT_EXPORT
        elif cvs == 'tar':
            pathName = os.path.dirname( folder )
            fileName = os.path.basename( uri )
            baseName = os.path.splitext( os.path.splitext( fileName )[ 0 ] )[ 0 ]
            extractedPathName = '%s/%s' % ( pathName, baseName )
            try:
                tarGzData = urllib2.urlopen( uri )
                output = open( fileName , 'wb' )
                output.write( tarGzData.read() )
                output.close()
                tar = tarfile.open( fileName )
                # check baseName exist in archive else fix extractedPathName
                for name in tar.getnames():
                    if not name.startswith( baseName ):
                        extractedPathName = '%s/%s' % ( pathName, os.path.dirname( name +'/' ) )
                    break
                tar.extractall( pathName )
                tar.close()
                os.remove( fileName )
                os.rename( extractedPathName, folder )
            except:
                if os.path.exists( fileName ):
                    os.remove( fileName )
                if os.path.exists( extractedPathName ):
                    shutil.rmtree( extractedPathName )
                return RepositoryInitializer.EC_CANT_EXPORT
        return RepositoryInitializer.EC_OK

    def moduleExport(self, cvs, module, uri):
        print '--- Updating module %s...' % ( module )
        
        folder = self.thirdPartyFolderFilePath( '%s.%s' % ( module, cvs ) )
        
        if folder == RepositoryInitializer.EC_CANT_CREATE_PATH:
            return folder
        
        if not os.path.isdir( folder ):
            result = self.systemExport( cvs, uri, folder )
            if result != RepositoryInitializer.EC_OK:
                return result
        
        cwd = os.getcwd();
        os.chdir( folder )
        result = self.systemUpdate( cvs );
        os.chdir( cwd )
        return result

    def run(self):
        for cvs in RepositoryInitializer.modules.keys():
            cvsModules = RepositoryInitializer.modules[ cvs ]
            for module in cvsModules.keys():
                uri = cvsModules[ module ]
                result = self.moduleExport( cvs, module, uri )
                if result != RepositoryInitializer.EC_OK:
                    return result
        return RepositoryInitializer.EC_OK

def main(argv=None):
    if argv is None:
        argv = sys.argv

    initializer = RepositoryInitializer()
    return initializer.run()

if __name__ == "__main__":
    result = main()
    print 'Exit Code: %i' % ( result )
    sys.exit( result )
