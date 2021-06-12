cls
@echo Started: %date% %time%;
@echo off

powershell.exe xelatex.exe -interaction=batchmode --no-pdf  .\relazioneCG.tex;
powershell.exe bibtex .\relazioneCG;
powershell.exe xelatex.exe -interaction=batchmode -output-driver='xdvipdfmx -z0' .\relazioneCG.tex;
DEL *.aux;
DEL *.blg;
DEL *.bbl;
DEL *.log;
DEL *.brf;
DEL *.nlo;
DEL *.dvi;
DEL *.ps;
DEL *.lof;
DEL *.toc;
DEL *.fls;
DEL *.pdfsync;
DEL *.synctex.gz;
DEL *.xdy;
DEL *.xml;
DEL *.glo;
DEL *.idx;
DEL *.nlo;
DEL *.bcf;
DEL *.mw;
DEL *.xdv;
DEL *-blx.bib;

@echo on
@echo Completed: %date% %time%;