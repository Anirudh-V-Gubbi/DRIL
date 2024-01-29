#include"../src/wrapper/fileWatcher.h"
static void function() {
	std::cout << "this is executing" << std::endl;
}
bool STOP = false;

int main( int argc, char** argv ) {

	std::string CurPath( efsw::System::getProcessPath() );

	CustomFileWatcher fileWatcher;

	std::cout << "Press ^C to exit demo" << std::endl;
	fileWatcher.watchOverDirAndSubDir( CurPath, function );

	while ( !STOP ) {
		efsw::System::sleep( 100 );
	}

	return 0;
}