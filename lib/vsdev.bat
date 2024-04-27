call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 
dumpbin /exports "G:\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll" > "G:\DRIL\examples\OpenGL\bin\debug\output.txt"
dumpbin /symbols "G:\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.lib" > "G:\DRIL\examples\OpenGL\bin\debug\output2.txt" 


