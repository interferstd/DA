@echo off
set /p str=Enter chapter:
git checkout "dev"
git merge "master"
git merge "%str%a"
git merge "%str%d"
git add --all
git commit -m "%str%"
git push
