@echo off
rem Digimon World 3 Launcher
setlocal
cd /d "%~dp0"
if not exist ".venv\Scripts\python.exe" (
    echo First run: creating the launcher's Python environment...
    python -m venv .venv || goto :nopython
)
rem A venv from an older checkout can be missing a dependency, so ask it rather than assume it.
".venv\Scripts\python.exe" -c "import PySide6, tomlkit, numpy" 2>nul || (
    echo Installing the launcher's dependencies, this runs once...
    ".venv\Scripts\python.exe" -m pip install --upgrade pip
    ".venv\Scripts\python.exe" -m pip install -r requirements.txt || goto :nodeps
)
".venv\Scripts\python.exe" main.py %*
if errorlevel 1 pause
exit /b
:nopython
echo.
echo Python 3 is required. Install it from https://www.python.org/downloads/ and run this again.
pause
exit /b 1
:nodeps
echo.
echo Could not install the dependencies in requirements.txt. Check your internet connection and try again.
pause
exit /b 1
