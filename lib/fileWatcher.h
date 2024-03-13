#pragma once

#include <efsw/FileSystem.hpp>
#include <efsw/System.hpp>
#include <efsw/efsw.hpp>
#include <iostream>
#include <signal.h>
#include <string>
#include <functional>

// Wrapper for esfw
class CustomFileWatcher {

  public:

	// To use the interrupt approach
	CustomFileWatcher() : fileWatcher() { fileWatcher.watch(); }

	// To use the polling approach
	CustomFileWatcher( bool usePolling ) : fileWatcher( usePolling ) { fileWatcher.watch(); }

	// Watches over a single file or directory and takes in the path and function to be performed on
	// a new event

	void watchOverSingleFileOrDir( std::string path, std::function<void()> func );
	// Watches over a directory and sub directories and takes in the path and function to be
	// performed on a new event

	void watchOverDirAndSubDir( std::string path, std::function<void()> func );

	//remove a watch by watchId
	
	void removeWatchbyId( efsw::WatchID watchId );

	//remoca a watch by path

	void removeWatchbyId(  std::string path );

	  private:

	// instance of esfw FileWatcher function
	
	efsw::FileWatcher fileWatcher;




	// implements  the function to be executed whenever there is a new event
	class UpdateListener : public efsw::FileWatchListener {

	  public:
		// takes in function
		UpdateListener( std::function<void()> function ) : func( function ) {}

		std::string getActionName( efsw::Action action ) {
			switch ( action ) {
				case efsw::Actions::Add:
					return "Add";
				case efsw::Actions::Modified:
					return "Modified";
				case efsw::Actions::Delete:
					return "Delete";
				case efsw::Actions::Moved:
					return "Moved";
				default:
					return "Bad Action";	
			}
		}
		// overrides the esfw function which is exectuted whenever a new event is emmited

		void handleFileAction( efsw::WatchID watchid, const std::string& dir,
							   const std::string& filename, efsw::Action action,
							   std::string oldFilename = "" ) override {
			std::cout << "Watch ID " << watchid << " DIR ("
					  << dir + ") FILE (" +
							 ( oldFilename.empty() ? "" : "from file " + oldFilename + " to " ) +
							 filename + ") has event "
					  << getActionName( action ) << std::endl;
			func();
		}

	  private:
		std::function<void()> func;
	};

	// Ouputs the addition or error state of a watch

	efsw::WatchID handleWatchID( efsw::WatchID watchid ) {
		switch ( watchid ) {
			case efsw::Errors::FileNotFound:
			case efsw::Errors::FileRepeated:
			case efsw::Errors::FileOutOfScope:
			case efsw::Errors::FileRemote:
			case efsw::Errors::WatcherFailed:
			case efsw::Errors::Unspecified: {
				std::cout << efsw::Errors::Log::getLastErrorLog().c_str() << std::endl;
				break;
			}
			default: {
				std::cout << "Added WatchID: " << watchid << std::endl;
			}
		}

		return watchid;
	}
	// function to be executed on a new event


};

