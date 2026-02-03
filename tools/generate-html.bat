REM change the current directory to the batch file directory, run antora (acoc -> html)
cd /D "%~dp0"
npx antora generate --to-dir ../docs ../doc/antora-playbook.yml
pause