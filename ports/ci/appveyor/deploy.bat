set SOURCES_DIR=%CD%
mkdir ..\webcamoid-shadow-build
cd ..\webcamoid-shadow-build

set BUILD_DIR=%CD%
C:\Python36\python.exe %SOURCES_DIR%\ports\deploy\deploy.py

cd %SOURCES_DIR%
