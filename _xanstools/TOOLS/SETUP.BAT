@ECHO OFF

:: This setup batch file runs the python scripts which install relevant dependencies for you, since those were added to the repo by the foundation in 4.7
CD..
CD..
CD MISC
CD SCRIPTS

PYTHON "install_accesskit.py"
PYTHON "install_angle.py"
PYTHON "install_d3d12_sdk_windows.py"
PYTHON "install_winrt.py"

ECHO Finished! Press any key to quit . . .
PAUSE >NUL