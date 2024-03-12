call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 
dumpbin /exports "N:\DRIL\DRIL\docimp\bin\Debug\DRILTest.dll" > "N:\DRIL\DRIL\docimp\bin\Debug\output.txt"
dumpbin /symbols "N:\DRIL\DRIL\docimp\bin\Debug\DRILTest.dll" > "N:\DRIL\DRIL\docimp\bin\Debug\output2.txt" | findstr C:\"External\"


