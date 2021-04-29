@echo off

rem Path to the Unreal Engine directory
set UE_PATH=D:\Epic Games\UE_5.5

rem Use the current directory as the project path
set PROJECT_PATH=%cd%

rem Set the project file name (replace YourProject with the actual project name)
set PROJECT_FILE=Delta.uproject

rem Generate Visual Studio Code project files
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%PROJECT_PATH%\%PROJECT_FILE%" -game -engine -vscode

echo Visual Studio Code project files generated successfully.
pause
