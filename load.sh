@echo off
git add --all
git commit -m "dev"
git push
git checkout "master"
git merge "dev"
git add --all
git commit -m "auto"
git push
git checkout "lexd"
git merge "dev"
git add --all
git commit -m "lex"
git push
git checkout "lexa"
git merge "dev"
git add --all
git commit -m "lex"
git push
git checkout "semd"
git merge "dev"
git add --all
git commit -m "syn"
git push
git checkout "sema"
git merge "dev"
git add --all
git commit -m "syn"
git push
git checkout "syna"
git merge "dev"
git add --all
git commit -m "syn"
git push
git checkout "synd"
git merge "dev"
git add --all
git commit -m "syn"
git push
git checkout "asma"
git merge "dev"
git add --all
git commit -m "asm"
git push
git checkout "asmd"
git merge "dev"
git add --all
git commit -m "asm"
git push
git checkout "dev"
