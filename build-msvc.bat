set BUILD_CONFIG=%1

rmdir /Q /S build && mkdir build & cd build && cmake .. -DCMAKE_BUILD_TYPE=%BUILD_CONFIG% -G "Visual Studio 17 2022" && cd ..
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
msbuild build/ALL_BUILD.vcxproj