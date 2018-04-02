#!/bin/bash
RUN=1
FRONT="PbGl_data"
BACK=".txt"
FILENUMBER=2
PRINT=""
while [ $RUN -eq 1 ]; do
	NAME=$FRONT$FILENUMBER$BACK
	if [ -e $NAME ]; 
	then
		let "FILENUMBER++"
	else
		RUN=0
	fi
done
>PbGl_dataC.txt
FILE2=$((FILENUMBER-1))
while [[ $FILENUMBER -ge 3 ]]; do
	mv /home/user/Dropbox/Nagel/FLTBAnalysis/PbGl_data{$FILE2,$FILENUMBER}.txt
	echo $"$FRONT$FILENUMBER$BACK">>PbGl_dataC.txt
	let "FILENUMBER--"
	FILE2=$((FILENUMBER-1))
done