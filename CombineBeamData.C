/* macro for managing the output of the BeamAnalysis files*/

void CombineBeamData(){
	cout<<"Enter a file: ";
	bool run = true;
	const int LINES = 6;
	queue<float> input[LINES];
	while(run){
		string filename;
		cin>>filename;
		ifstream inFile (filename.c_str()); //txt file containing the data from BeamAnalysis1
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
		cout<<"Continue? [Y/N]"<<std::endl;
		cin.clear();
		cin>>filename;
		std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
		if(filename[0]!='y'){
			run=false;
		}
		cin.clear();
	}
	ofstream outFile;
	outFile.open("PbGl_dataC.txt");
	if(outFile.is_open()) //read info out to txt file if it opens
	{	
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