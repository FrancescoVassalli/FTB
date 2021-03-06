

using namespace std;

/* run 
mean
meanerror
sigma 
sigmaerror*/

float runToEnergy(float run){
	int r;
	int s = (int) run;
	switch (s){
		case 558:
			r=6;
			break;
		case 551:
			r= 4;
			break;
		case 563:
			r= 2;
			break;
		case 573:
			r= 12;
			break;
		case 567:
			r= 8;
			break;
		case 544:
			r= 8;
			break;
		default:
			r=-1;
			break;
	}
	return r;
}
float* runToEnergy(queue<float> runs){
	float *energies = new float[runs.size()];
	for (unsigned i = 0; i < runs.size(); ++i)
	{
		energies[i] = runToEnergy(runs.front());
		runs.pop();
	}
	return energies;
}

float* trendForced(const int nMeanBins,float*meanBins, float* adc12, float* sigma, float* meanerror, float* sigmaerror, float* counts){
	float ex[nMeanBins];
	//float *uncertainty = sigmaNtoUncertainty(nMeanBins, sigma, counts, sigmaerror);
	for (int i = 0; i < nMeanBins; ++i)
	{
		ex[i] = 0;
		//cout<<sigma[i]<<" : "<<counts[i]<<" = "<<uncertainty[i]<<'\n';
	}
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* mean = new TGraphErrors(nMeanBins,meanBins,adc12,ex,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,meanBins[nMeanBins-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,meanBins[nMeanBins-1]);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	mean->Fit(lin,"0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	//cout<<"C2/C1: "<<nonLinearFactor<<" / "<<linearFactor<<" = "<<nonLinearFactor/linearFactor<<endl;
	double linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	//cout<<"Ratio: "<<ratiouncertainty<<endl;
	mean->SetMarkerStyle(kOpenCircle);
	doubleZero(mean,adc12[nMeanBins-1]+1000,meanBins[nMeanBins-1]+1);
	mean->Draw("AP");
	mean->GetXaxis()->SetLimits(0,meanBins[nMeanBins-1]+1);
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
	myText(.5,.27,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.05);
	myText(.5,.22,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	myText(.5,.37,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);
	float *r = new float[2];
	r[0] = linearFactor;
	r[1] = linearError;
	return r;
}

void BeamAnalysis2(){
	ifstream inFile ("/home/user/Dropbox/Nagel/FLTBAnalysis/PbGl_data1_1200.txt"); //txt file containing the data from BeamAnalysis1
	const int LINES = 6;
	queue<float> input[LINES];
	string intemp;
	stringstream ss;
	for (int i = 0; i < LINES; ++i)
	{
		inFile>>intemp;
		ss<<intemp;
		getline(ss,intemp,',');
		//cout<<intemp<<":\n";
		while(getline(ss,intemp,',')){
			input[i].push(stof(intemp));
			//cout<<intemp<<endl;
		}
		ss.clear();
	}
	float *r = trendForced(input[1].size(),queueToArray(input[0]),queueToArray(input[1]),queueToArray(input[3]),queueToArray(input[2]),queueToArray(input[4]),queueToArray(input[5]));
	ofstream outFile;
	outFile.open("PbGl_data2.txt");
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