:: Delete temp folder "NGine vX.X/Projects/Temp/"
rmdir /s /q "%~dp0\Projects\Temp\"
rmdir /s /q "%~dp0\Projects\ipch\"

:: Delete N-Gine.lib files from "NGine vX.X/Lib/"
cd %~dp0\Lib\
del /F /Q "N-Gine_x86_Debug.lib"
del /F /Q "N-Gine_x86_Release.lib"
del /F /Q "N-Gine_x64_Debug.lib"
del /F /Q "N-Gine_x64_Release.lib"

:: Delete .exe .pdb .ilk & .txt files from "NGine vX.X/Build/"
cd %~dp0\Build\x86\
del /F /Q *.exe
cd %~dp0\Build\x64\
del /F /Q *.exe

:: Delete .sdf files from "NGine vX.X/Projects/"
cd %~dp0\Projects\
del /F /Q *.sdf
del /F /Q *.vsp
del /F /Q *.psess
del /F /Q *.opensdf
