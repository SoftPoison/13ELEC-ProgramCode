@echo off
echo Deleting old .bin files
del *.bin
echo Compiling...
particle compile photon
pause