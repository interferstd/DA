@echo off
set /p str=Enter commit name:
git add --all
git commit -m "%str%"
