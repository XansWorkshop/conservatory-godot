@ECHO OFF

:: YOU MAY NEED TO RUN SETUP.BAT BEFORE THIS - SEE INSTRUCTIONS

:: https://docs.godotengine.org/en/stable/engine_details/development/compiling/compiling_for_windows.html#installing-direct3d-12-requirements
:: https://github.com/godotengine/godot-accesskit-c-static/releases
:: https://github.com/godotengine/godot-angle-static/releases

:: Set this to nothing (or 0) if you are NOT using System.Numerics.Vector* types in C# as a supplement to Godot's types.
:: As a note, this requires a LOT of work to switch on and off, as it involves going in and modifying the GodotSharp C# project files (all of them!)
:: to (un)define the applicable compiler directive. Everything was put under a static branch though.
:: But to make things even more complex, even if you do disable this, The Conservatory is hardcoded to use float now. It used to not be, but realistically
:: double is just too slow and too precise to be useful. That, and every CPU can optimize float (SSE), but only some can optimize double (AVX).
SET USING_SYSTEM_NUMERICS_VECTORS=1

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:GET_OPTION
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

:DOUBLE_NOT_SUPPORTED
ECHO Currently, The Conservatory's C# API is using System.Numerics for its spatial types.
ECHO This dramatically optimizes the performance of Vector2/3/4 types, but does not
ECHO support double precision.
ECHO.
ECHO For this reason, double precision is not supported at this time. 
ECHO Press any key to make a new selection . . .
PAUSE >NUL
CLS
GOTO GET_OPTION

:: godot.<platform>.<target>[.dev][.double].<arch>[.<extra_suffix>][.<ext>]

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_DEBUG_DOUBLE
IF %USING_SYSTEM_NUMERICS_VECTORS%==1 GOTO DOUBLE_NOT_SUPPORTED
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_debug_double.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.dev.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
SET BUILDER=msvc
GOTO ASKEXP

:TYPE_CONSERVATORY_DEBUG
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_debug.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.dev.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
SET BUILDER=msvc
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_RELEASE_DOUBLE
IF %USING_SYSTEM_NUMERICS_VECTORS%==1 GOTO DOUBLE_NOT_SUPPORTED
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_double.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
SET BUILDER=llvm
GOTO ASKEXP

:TYPE_CONSERVATORY_RELEASE
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
SET BUILDER=llvm
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:TYPE_CONSERVATORY_RELEASE_DOUBLE_RUNTIME_METRICS
IF %USING_SYSTEM_NUMERICS_VECTORS%==1 GOTO DOUBLE_NOT_SUPPORTED
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_double_memtracking.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.double.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=--precision=double
SET BUILDER=llvm
GOTO ASKEXP

:TYPE_CONSERVATORY_RELEASE_RUNTIME_METRICS
SET BUILD_NAME=conservatory_edition
SET PROFILE="profile=%XANSTOOLS_FOLDER%TOOLS/scons_profiles/conservatory_release_memtracking.py" "extra_suffix=%BUILD_NAME%"
SET EXNAME=godot.windows.editor.x86_64.%BUILD_NAME%.mono.exe
SET DOUBLE=
SET BUILDER=llvm
GOTO ASKEXP

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:ASKEXP
ECHO Would you like to build export templates as well? (Press Y (Yes) or N (No))
CHOICE /n
ECHO.
SET CHS=%ERRORLEVEL%

@ECHO ON
scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" %PROFILE%
::scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "angle_libs=%LOCALAPPDATA%\Godot\build_deps\angle-x86_64-%BUILDER%" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" %PROFILE%
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
scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" target=template_debug %PROFILE%
::scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "angle_libs=%LOCALAPPDATA%\Godot\build_deps\angle-x86_64-%BUILDER%" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" target=template_debug %PROFILE%

ECHO Making Windows Release Build Template...
scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" target=template_release %PROFILE%
::scons platform=windows arch=x86_64 "accesskit_sdk_path=%LOCALAPPDATA%\Godot\build_deps\accesskit" "angle_libs=%LOCALAPPDATA%\Godot\build_deps\angle-x86_64-%BUILDER%" "mesa_libs=%LOCALAPPDATA%\Godot\build_deps\mesa-x86_64-%BUILDER%" "pix=%LOCALAPPDATA%\Godot\build_deps\pix" target=template_release %PROFILE%

:GLUE
ECHO Glue...

:: Generate glue sources
CD bin
%EXNAME% --verbose --headless --generate-mono-glue "../modules/mono/glue"
:: Build .NET assemblies

CD..
SET LOCAL_NUGET=%USERPROFILE%\LocalNuget
SET PYTHONIOENCODING="UTF-8"
IF NOT %USING_SYSTEM_NUMERICS_VECTORS%==1 GOTO BUILD_ASSEMBLIES
python ./modules/mono/build_scripts/xans_comical_vector_axis_hack.py "%CONSERVATORY_GODOT_FOLDER%"
:BUILD_ASSEMBLIES
python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin --godot-platform=windows %DOUBLE% --no-deprecated --push-nupkgs-local "%LOCAL_NUGET%"

ECHO.
ECHO Done! Press any key to quit.
PAUSE >nul