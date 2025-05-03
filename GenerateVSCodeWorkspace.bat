@echo off

set UE_PATH=D:\Epic Games\UE_5.5

set PROJECT_PATH=%cd%

set PROJECT_FILE=Delta.uproject

"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%PROJECT_PATH%\%PROJECT_FILE%" -game -engine -vscode

echo Visual Studio Code project files generated successfully.

pause
