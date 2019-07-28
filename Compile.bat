@if [%2] == [] echo Insufficient parameters! && pause && exit

mkdir CMake.tmp
cd CMake.tmp
mkdir %1
cd %1
mkdir %2
cd %2

cmake ../../.. -G %1 -T %2 -DCMAKE_INSTALL_PREFIX="../../.." %3
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Debug --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config Release --target install
if %ERRORLEVEL% neq 0 pause && goto eof
cmake --build . --config RelWithDebInfo --target install
if %ERRORLEVEL% neq 0 pause && goto eof
pause

:eof
cd ../../..
