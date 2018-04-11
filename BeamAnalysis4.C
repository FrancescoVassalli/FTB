
using namespace std;

void plotByEnergy(int SIZE, float* means, float* meanerror, float* inputEnergy,const unsigned int nFiles){
	TCanvas *tc = new TCanvas();
	float ex[SIZE];
	int peakInput=0;
	int tempenergy=inputEnergy[0];
	int fileBeginIndex[nFiles+1];
	int fileBeginIndexCounter=1;
	fileBeginIndex[0]=0;
	for (int i = 0; i < SIZE; ++i)
	{
		ex[i] = 0;
		if(peakInput<inputEnergy[i]){
			peakInput=inputEnergy[i];
		}
		if(tempenergy>inputEnergy[i]){
			fileBeginIndex[fileBeginIndexCounter++]=i;
		}
		tempenergy=inputEnergy[i];
	}
	fileBeginIndex[fileBeginIndexCounter]=SIZE-1;
	peakInput++;
	TGraphErrors *measure = new TGraphErrors(SIZE,inputEnergy,means,ex,meanerror);
	axisTitles(measure,"Beam Energy GeV","Measured Energy");
	TF1* lin = new TF1("lin","[0]*x",0,peakInput);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,peakInput);
	measure->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	measure->Fit(lin,"0");
	gNice();
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	lin->SetLineColor(kRed);
	measure->SetMarkerStyle(kOpenCircle);
	
	TGraphErrors** plotgraphs = new TGraphErrors*[nFiles];
	for (unsigned i = 0; i < nFiles; ++i)
	{
		
		plotgraphs[i] = new TGraphErrors(fileBeginIndex[i+1]-fileBeginIndex[i],partialArray(inputEnergy,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(means,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(ex,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(meanerror,fileBeginIndex[i],fileBeginIndex[i+1]));
	}
	doubleZero(plotgraphs[0],peakInput,peakInput);
	plotgraphs[0]->SetMarkerSize(2);
	plotgraphs[0]->SetMarkerStyle(kOpenCircle);
	plotgraphs[0]->Draw("AP");
	plotgraphs[1]->SetMarkerStyle(kOpenTriangleDown);
	for (unsigned i = 1; i < nFiles; ++i)
	{
		plotgraphs[i]->SetMarkerSize(2);
		plotgraphs[i]->Draw("P");
	}
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
	plotgraphs[0]->GetXaxis()->SetLimits(0,peakInput);
	myText(.5,.30,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	myText(.5,.18,kRed,Form("C2: %0.3f#pm %0.5f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.24,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);
	myMarkerText(.25,.9,kBlack,kOpenTriangleDown,"1100V",2,.05);
	myMarkerText(.25,.8,kBlack,kOpenCircle,"1200V",2,.05);
}

queue<float>* adcToEnergy(float linearFactor, float linearFactorError, int SIZE, float* means, float* inputEnergy, float* meanerror){
	errorDivideArray(SIZE,means,meanerror,linearFactor,linearFactorError);
	queue<float> *r = new queue<float>[3];
	for (int i = 0; i < SIZE; ++i)
	{
		r[0].push(means[i]);
		r[1].push(meanerror[i]);
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
	const unsigned int nFiles = files.size();
	const int LINES =5;
	float linearFactor[files.size()];
	float linearFactorError[files.size()];
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
		cout<<intemp<<std::endl;
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
				/*if(i==2)
					cout<<intemp<<endl;*/
			}
			ss.clear();
		}
		inFile.close();
		tempenergy=adcToEnergy(linearFactor[count], linearFactorError[count], input[1].size(),queueToArray(input[1]),queueToArray(input[0]),queueToArray(input[2]));
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
	plotByEnergy(totalinput[0].size(),queueToArray(totalinput[0]),queueToArray(totalinput[1]),queueToArray(totalinput[2]),nFiles);
}