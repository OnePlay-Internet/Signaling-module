rmdir /Q /S build && mkdir build & cd build && cmake .. && cd ..
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
msbuild build/ALL_BUILD.vcxproj