#include "fileWatcher.h"
using namespace std;


// Watches over a single file or directory and takes in the path and function to be performed on
// a new event
void CustomFileWatcher::watchOverSingleFileOrDir( std::string path, std::function<void()> func ) {
	UpdateListener* ul = new UpdateListener( func );
	efsw::WatchID	watchId = 	handleWatchID( fileWatcher.addWatch( path, ul ) );
	if ( watchId < 0 ) {
		fileWatcher.removeWatch( watchId );
	}
}

// Watches over a directory and sub directories and takes in the path and function to be
// performed on a new event
void CustomFileWatcher::watchOverDirAndSubDir( std::string path, std::function<void()> func ) {
	UpdateListener* ul = new UpdateListener( func );
	efsw::WatchID watchId = handleWatchID( fileWatcher.addWatch( path, ul, true ) );
	if ( watchId < 0 ) {
		fileWatcher.removeWatch( watchId );
	}
}

// Function to end watching files

void CustomFileWatcher::removeWatchbyId( efsw::WatchID watchId ) {
	fileWatcher.removeWatch( watchId);
	cout << "Watcher removed successfully" << endl;
}

// remove a watch by path

void CustomFileWatcher::removeWatchbyId( std::string path ) {
	fileWatcher.removeWatch( path );
	cout << "Watcher removed successfully" << endl;
}





// Ouputs the addition or error state of a watch

