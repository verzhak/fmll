#!/bin/bash

for x in ../various/dll/*def
do
	
	/usr/i486-mingw32/bin/dlltool -l `echo $x | sed 's/.*\/\([^\/]*\)\.def/\1\.lib/'` -d $x

done

