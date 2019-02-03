@echo off 
set /p str=Enter branch to merge: 
git merge "%str%"