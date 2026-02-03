REM change the current directory to the batch file directory, run mrdocs (to generate adoc files), then antora (acoc -> html)
cd /D "%~dp0"
mrdocs ../doc/mrdocs.yml
npx antora generate --to-dir ../docs ../doc/antora-playbook.yml
pause