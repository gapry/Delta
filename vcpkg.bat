REM Copyright (c) 2025 Gapry.
REM Licensed under the MIT License.
REM See LICENSE file in the project root for full license information.

@echo off

if not exist vcpkg (
  git clone https://github.com/microsoft/vcpkg.git
)

call .\vcpkg\bootstrap-vcpkg.bat

call .\vcpkg\vcpkg.exe install fmt