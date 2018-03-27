

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

void trend(const int nMeanBins,float*meanBins, float* adc12, float* sigma, float* meanerror, float* sigmaerror){
	float ex[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		ex[i] = 0;
	}
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* mean = new TGraphErrors(nMeanBins,meanBins,adc12,ex,sigma); // how to set the uncertainty
	TF1* lin = new TF1("lin","[1]*x+[0]",0,12);
	TF1* poly = new TF1("poly","[2]*x*x+[1]*x+[0]",0,12);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(2)/poly->GetParameter(1);
	double nonLinearError = poly->GetParError(2);
	float chi2 = poly->GetChisquare();
	mean->Fit(lin);
	lin->SetLineColor(kRed);
	float linearFactor = lin->GetParameter(1);
	cout<<"C2/C1: "<<nonLinearFactor<<" / "<<linearFactor<<" = "<<nonLinearFactor/linearFactor<<endl;
	float linearError = lin->GetParError(1);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	cout<<"Ratio: "<<ratiouncertainty<<endl;
	mean->SetMarkerStyle(kOpenCircle);
	mean->Draw("AP");
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	myText(.5,.27,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.05);
	myText(.5,.22,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	myText(.5,.37,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);

}

void BeamAnalysis2(){
	int counter = 0;
	ifstream inFile ("PbGl_data1.txt"); //txt file containing the data from BeamAnalysis1
	const int LINES = 5;
	queue<float> input[LINES];
	string intemp;
	stringstream ss;
	for (int i = 0; i < LINES; ++i)
	{
		inFile>>intemp;
		ss<<intemp;
		getline(ss,intemp,',');
		while(getline(ss,intemp,',')){
			input[i].push(stof(intemp));
			cout<<intemp<<endl;
		}
		ss.clear();
	}
	float *energies = runToEnergy(input[0]);
	trend(input[1].size(),energies,queueToArray(input[1]),queueToArray(input[3]),queueToArray(input[2]),queueToArray(input[4]));
}