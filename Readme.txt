BeamAnalysis1 - reads data from the PbGl_Runs.txt which should hold the file paths to the beam data  you want to process deliminated by line breaks.  It outputs PbGl_data1.txt which should contain the run# mean meanerror sigma sigmaerror. Only one voltage should be processed at a time. for unkown voltages just run a single file and figure out what the voltage is later 
BeamAnalysis2 - reads PbGl_data1.txt to produce a trendline 
BeamAnalysis3 - reads PbGl_data1.txt to produce a resolution \
BeamAnalysis4 - reads PbGl_dataC.txt to produce a trendline that is independent of voltage
BeamAnalysis5 - reads PbGl_dataC.txt to produce a resolution that is independent of voltage
BeamAnalysis6 - calculation of the systematic uncertainty on the trend
BeamAnalysis7 - calculates systematic uncertainty on the resolution
PbGl_data1.txt - data ouput from BeamAnalysis1
PbGl_data2.txt - out put from BeamAnalysis 2 should be the same as the data1.txt but with the linear trend and its error added on top rename these files after creating them and add their names to the PbGl_dataC.txt
PbGl_dataC.txt - contains the filenames of the outputs from BeamAnalysis2
CombineShell.sh - run with bash to move the data2.txt into data3.txt and move data3.txt to data4.txt and so on. The filenames for the Beam2 outputs are then saved to dataC
Combination.C - removed because the shell script is a better way to do this imo
----------------------------------------------------------------------------------------------------
After this point new calibrated analysis was made. 
Part2A.C - does what BeamAna1 did but outputs to PbGlA.txt with the beam energy intead of the numentries.  Set the input files in the main. More useful if only a single set of voltage files is used
Part2B.C - does what BeamAna2 did 
PbGlA.txt - records run,mean,meanU,sigmaU,energy more useful if only a single set of voltage files are used 
----------------------------------------------------------------------------------------------------
John Haggerty Pointed me to the new data.
vassalli@rssh.rcf.bnl.gov
