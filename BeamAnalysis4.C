
using namespace std;

void plotByEnergy(int SIZE, float* mean, float* sigma, float* inputEnergy){
	TCanvas *tc = new TCanvas();
	float ex[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		ex[i] = 0;
	}
	TGraphErrors *measure = new TGraphErrors(SIZE,inputEnergy,means,ex,sigma);
	axisTitles(measure,"Beam Energy GeV","Measured Energy");
	TF1* lin = new TF1("lin","[0]*x",0,12);
	measure->Fit(lin);
	gNice();
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	lin->SetLineColor(kRed);
	measure->SetMarkerStyle(kOpenCircle);
	doubleZero(measure,means[SIZE-1]+2,inputEnergy[SIZE-1]+1);
	measure->Draw("AP");
	measure->GetXaxis()->SetLimits(0,inputEnergy[SIZE-1]+1);
	myText(.5,.22,kRed,Form("#chi^{2}/NDF: %0.2f",chi/ndf),.05);
}

queue<float>* adcToEnergy(float linearFactor, float linearFactorError, int SIZE, float* means, float* inputEnergy, float* sigma){
	errorDivideArray(SIZE,means,sigma,linearFactor,linearFactorError);
	queue<float> *r = new queue<float>[3];
	for (int i = 0; i < SIZE; ++i)
	{
		r[0].push(means[i]);
		r[1].push(sigma[i]);
		r[2].push(inputEnergy[i]);
	}
	return r;
}


void BeamAnalysis4(){
	ifstream inFile ("PbGl_dataC.txt"); //txt file containing the names of files to process
	queue<string> files;
	string intemp;
	while(getline(inFile,intemp)){
		files.push(intemp);
	}
	inFile.close();
	const int LINES =5;
	float *linearFactor[files.size()];
	float *linearFactorError[files.size()];
	int count=0;
	queue<float> totalinput[3];
	while(!files.empty()){
		queue<float> input[LINES];
		queue<float> *tempenergy;
		inFile.open(files.front().c_str());
		files.pop();
		stringstream ss;
		intemp="";
		inFile>>intemp;
		ss<<intemp;
		getline(ss,intemp,',');
		linearFactor[count]= stof(intemp);
		getline(ss,intemp,',');
		linearFactorError[count] = stof(intemp);
		ss.clear();
		for (int i = 0; i < LINES; ++i)
		{
			inFile>>intemp;
			ss<<intemp;
			while(getline(ss,intemp,',')){
				input[i].push(stof(intemp));
				//cout<<intemp<<endl;
			}
			ss.clear();
		}
		inFile.close();
		tempenergy=adcToEnergy(linearFactor[count], linearFactorError[count], input[1].size(),queueToArray(input[1]),queueToArray(input[0]),queueToArray(input[3]));
		while(!tempenergy[0].empty()){
			totalinput[0].push(tempenergy[0].front());
			totalinput[1].push(tempenergy[1].front());
			totalinput[2].push(tempenergy[2].front());
			tempenergy[0].pop();
			tempenergy[1].pop();
			tempenergy[2].pop();
		}
		count++;
	}
	plotByEnergy(totalinput[0].size(),queueToArray(totalinput[0]),queueToArray(totalinput[1]),queueToArray(totalinput[2]));
}