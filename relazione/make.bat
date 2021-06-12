cls
@echo Started: %date% %time%;
@echo off

powershell.exe xelatex.exe -synctex=1 -interaction=batchmode --no-pdf  .\relazioneCG.tex;
powershell.exe bibtex .\relazioneCG;
powershell.exe xelatex.exe -synctex=1 -interaction=batchmode -output-driver='xdvipdfmx -z0' .\relazioneCG.tex;

@echo on
@echo Completed: %date% %time%;