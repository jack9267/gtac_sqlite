@if [%CMAKE_GENERATOR%] == [] echo Insufficient parameters! && pause && exit

mkdir CMake.tmp
cd CMake.tmp
mkdir %CMAKE_GENERATOR%
cd %CMAKE_GENERATOR%
mkdir %CMAKE_GENERATOR_PLATFORM%
cd %CMAKE_GENERATOR_PLATFORM%
mkdir %CMAKE_GENERATOR_TOOLSET%
cd %CMAKE_GENERATOR_TOOLSET%

cmake ../../../.. -G %CMAKE_GENERATOR% -A %CMAKE_GENERATOR_PLATFORM% -T %CMAKE_GENERATOR_TOOLSET% -DCMAKE_INSTALL_PREFIX="../../../.." %1
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Debug --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Release --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config RelWithDebInfo --target install
if %ERRORLEVEL% neq 0 pause && goto eof
pause

:eof
cd ../../../..
