call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 
dumpbin /exports "C:\Users\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll" > "C:\Users\DRIL\examples\OpenGL\bin\debug\output.txt"
dumpbin /symbols "C:\Users\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll" > "C:\Users\DRIL\examples\OpenGL\bin\debug\output2.txt" 


