@echo off
git pull
git merge dev
git add --all
git commit -m "auto commit"
git push
