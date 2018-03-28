

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

float* trendForced(const int nMeanBins,float*meanBins, float* adc12, float* sigma, float* meanerror, float* sigmaerror){
	float ex[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		ex[i] = 0;
		cout<<meanBins[i]<<endl;
	}
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* mean = new TGraphErrors(nMeanBins,meanBins,adc12,ex,sigma); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,12);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,12);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	mean->Fit(lin,"0");
	lin->SetLineColor(kRed);
	float linearFactor = lin->GetParameter(0);
	cout<<"C2/C1: "<<nonLinearFactor<<" / "<<linearFactor<<" = "<<nonLinearFactor/linearFactor<<endl;
	float linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	cout<<"Ratio: "<<ratiouncertainty<<endl;
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

void trend(const int nMeanBins,float*meanBins, float* adc12, float* sigma, float* meanerror, float* sigmaerror){
	float ex[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		ex[i] = 0;
		cout<<meanBins[i]<<endl;
	}
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* mean = new TGraphErrors(nMeanBins,meanBins,adc12,ex,sigma); // how to set the uncertainty
	TF1* lin = new TF1("lin","[1]*x+[0]",0,12);
	TF1* poly = new TF1("poly","[2]*x*x+[1]*x+[0]",0,12);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(2);
	double nonLinearError = poly->GetParError(2);
	float chi2 = poly->GetChisquare();
	mean->Fit(lin,"0");
	lin->SetLineColor(kRed);
	float linearConstant = lin->GetParameter(0);
	float linearFactor = lin->GetParameter(1);
	cout<<"C2/C1: "<<nonLinearFactor<<" / "<<linearFactor<<" = "<<nonLinearFactor/linearFactor<<endl;
	float linearError = lin->GetParError(1);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	cout<<"Ratio: "<<ratiouncertainty<<endl;
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
	myText(.55,.42,kRed,Form("Linear Constant: %0.2f",linearConstant),.05);

}

void resolution(const int nMeanBins,float*meanBins, float* adc12, float* sigma, float* meanerror, float* sigmaerror){
	float ex[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		ex[i] = 0;
	}
	TCanvas *canvas1 = new TCanvas();
	float relativeE[nMeanBins];
	float relativeU[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		relativeE[i] = sigma[i]/adc12[i];
		relativeU[i]= errorDivide(sigma[i],sigmaerror[i],adc12[i],meanerror[i]);
		cout<<relativeU[i]<<endl;
	}
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,12);
	eF->SetLineColor(kRed);
	TGraphErrors* ehist = new TGraphErrors(nMeanBins,meanBins,relativeE,ex,relativeU);
	ehist->Fit(eF);
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	ehist->Draw("AP");
	axisTitles(ehist,"Beam Energy GeV","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05);
	myText(.24,.85,kRed,Form("Stochastic: %0.6f#pm%0.6f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Constant: %0.6f#pm%0.6f",eB,errors[1]),.05);
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
		cout<<intemp<<":\n";
		while(getline(ss,intemp,',')){
			input[i].push(stof(intemp));
			cout<<intemp<<endl;
		}
		ss.clear();
	}
	float *r = trendForced(input[1].size(),queueToArray(input[0]),queueToArray(input[1]),queueToArray(input[3]),queueToArray(input[2]),queueToArray(input[4]));
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