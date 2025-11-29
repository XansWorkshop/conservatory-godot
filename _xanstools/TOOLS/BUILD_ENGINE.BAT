@ECHO OFF
CD..
SET XANSTOOLS_FOLDER=%CD%/
CD..
SET CONSERVATORY_GODOT_FOLDER=%CD%/
ECHO This should be the _xanstools directory...
ECHO %XANSTOOLS_FOLDER%
ECHO This should be the Godot project directory...
ECHO %CONSERVATORY_GODOT_FOLDER%
ECHO If these directories are correct, you may continue.
ECHO.

ECHO What are you building this fork for? Press...
ECHO [1] for The Conservatory (Release)
ECHO [2] for The Conservatory (Debug)
ECHO [3] for The Conservatory (Release, Double Precision)
ECHO [4] for The Conservatory (Debug, Double Precision)
ECHO [5] for The Conservatory (Release, Runtime Memory Metrics)
ECHO [6] for The Conservatory (Release, Double Precision, Runtime Memory Metrics)
CHOICE /c 123456 /n
ECHO.
if %ERRORLEVEL%==6 GOTO TYPE_CONSERVATORY_RELEASE_DOUBLE_RUNTIME_METRICS
if %ERRORLEVEL%==5 GOTO TYPE_CONSERVATORY_RELEASE_RUNTIME_METRICS
if %ERRORLEVEL%==4 GOTO TYPE_CONSERVATORY_DEBUG_DOUBLE
if %ERRORLEVEL%==3 GOTO TYPE_CONSERVATORY_RELEASE_DOUBLE
if %ERRORLEVEL%==2 GOTO TYPE_CONSERVATORY_DEBUG
if %ERRORLEVEL%==1 GOTO TYPE_CONSERVATORY_RELEASE
GOTO ASKEXP

:: godot.<platform>.<target>[.dev][.double].<arch>[.<extra_suffix>][.<ext>]

:TYPE_GENERAL_EXPORT
SET BUILD_NAME=xansworkshop
SET PROFILE="module_mono_enabled=yes" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_DEBUG_DOUBLE
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_debug_double.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.dev.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
GOTO ASKEXP

:TYPE_CONSERVATORY_DEBUG
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_debug.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.dev.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_RELEASE_DOUBLE
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_double.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
GOTO ASKEXP

:TYPE_CONSERVATORY_RELEASE
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_RELEASE_DOUBLE_RUNTIME_METRICS
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_double_memtracking.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
GOTO ASKEXP

:TYPE_CONSERVATORY_RELEASE_RUNTIME_METRICS
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_memtracking.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:ASKEXP
ECHO Would you like to build export templates as well? (Press Y (Yes) or N (No))
CHOICE /n
ECHO.
SET CHS=%ERRORLEVEL%

@ECHO ON
scons platform=windows "accesskit_sdk_path=%XANSTOOLS_FOLDER%MODULES/accesskit-c-0.17.0" "mesa_libs=%XANSTOOLS_FOLDER%MODULES/d3d12" "angle_libs=%XANSTOOLS_FOLDER%MODULES/ANGLE" %PROFILE%
@ECHO OFF

if %ERRORLEVEL%==0 GOTO CARRYON
ECHO An error occurred and so the build process was stopped.
ECHO Press any key to quit.
PAUSE >nul
EXIT

:CARRYON
IF %CHS%==2 GOTO GLUE
IF %CHS%==1 GOTO EXPORT_TEMPLATES
:EXPORT_TEMPLATES
:: Build export templates
ECHO Build Templates...

ECHO Making Windows Debug Build Template...
scons platform=windows target=template_debug %PROFILE%

ECHO Making Windows Release Build Template...
scons platform=windows target=template_release %PROFILE%

:GLUE
ECHO Glue...

:: Generate glue sources
CD bin
%EXNAME% --headless --generate-mono-glue "../modules/mono/glue"
:: Build .NET assemblies

CD..
SET LOCAL_NUGET=%USERPROFILE%\LocalNuget
ECHO %LOCAL_NUGET%
PAUSE
python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin --godot-platform=windows %DOUBLE% --no-deprecated --push-nupkgs-local "%LOCAL_NUGET%"

ECHO.
ECHO Done! Press any key to quit.
PAUSE >nul