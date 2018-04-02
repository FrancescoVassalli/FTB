BeamAnalysis1 - reads data from the PbGl_Runs.txt which should hold the file paths to the beam data deliminated by line breaks.  It outputs PbGl_data1.txt which should contain the run# mean meanerror sigma sigmaerror.  
BeamAnalysis2 - reads PbGl_data1.txt to produce a trendline 
BeamAnalysis3 - reads PbGl_data1.txt to produce a resolution \
BeamAnalysis4 - reads PbGl_dataC.txt to produce a trendline that is independent of voltage
PbGl_data1.txt - data ouput from BeamAnalysis1
PbGl_data2.txt - out put from BeamAnalysis 2 should be the same as the data1.txt but with the linear trend and its error added on top rename these files after creating them and add their names to the PbGl_dataC.txt
PbGl_dataC.txt - contains the filenames of the outputs from BeamAnalysis2