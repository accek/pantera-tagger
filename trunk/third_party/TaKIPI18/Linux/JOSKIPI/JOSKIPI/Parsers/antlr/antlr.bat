@echo off
cd "C:\Users\KeMbi\Documents\Polibuda\tagger\backup\JOSKIPI\JOSKIPI\JOSKIPI\Parsers\antlr\"
set "CP=C:\Program Files\antlr\277\lib\antlr.jar";%CLASSPATH%
set cmd=java -classpath "%CP%" antlr.Tool joskipi.g
echo JOSKIPI:
%cmd%
set CP=
@pause