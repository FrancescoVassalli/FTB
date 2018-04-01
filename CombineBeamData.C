/* macro for managing the output of the BeamAnalysis files*/

void CombineBeamData(int argc, char* argv[]){
	const int LINES = 5;
	queue<float> input[LINES];
	while(argc>1){
		ifstream inFile (argv[argc-1]); //txt file containing the data from BeamAnalysis1
		string intemp;
		stringstream ss;
		for (int i = 0; i < LINES; ++i)
		{
			inFile>>intemp;
			ss<<intemp;
			getline(ss,intemp,',');
			cout<<intemp<<":\n";
			while(getline(ss,intemp,',')){
				input[i].push(stof(intemp));
				cout<<intemp<<endl;
			}
			ss.clear();
		}
		argc--;
	}
	ofstream outFile;
	outFile.open("PbGl_dataC.txt");
	if(outFile.is_open()) //read info out to txt file if it opens
	{	
		outFile<<r[0]<<","<<r[1]<<"\n";
		for (int i = 0; i < LINES; ++i)
		{
			while(!input[i].empty()){
				outFile<<input[i].front()<<",";
				input[i].pop();
			}
			outFile<<"\n";
		}
	}
	else {cout<<"RED ALERT! RED ALERT! FAILED TO WRITE TO A TEXT FILE! I REPEAT! RED ALERT!"<<endl;}
}