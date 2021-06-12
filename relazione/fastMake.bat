cls
@echo Started: %date% %time%;
@echo off

powershell.exe xelatex.exe -interaction=batchmode -output-driver='xdvipdfmx -z0' .\relazioneCG.tex;

@echo on
@echo Completed: %date% %time%;