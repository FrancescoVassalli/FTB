#include "Frannamespace.C"
using namespace std;

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
	//makeMarkerNice(&ehist, 1);
	ehist->SetMarkerSize(2);
	ehist->SetMinimum(0);
	ehist->SetMaximum(0.1);
	ehist->GetXaxis()->SetLimits(0,13);
	ehist->Draw("AP*");
	axisTitles(ehist,"Beam Energy GeV","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05,0);
	myText(.3,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05,0);
	myText(.24,.85,kRed,Form("Stochastic: %0.6f#pm%0.6f ",eA,errors[0]),.05,0);
	myText(.24,.8,kRed,Form("Constant: %0.6f#pm%0.6f",eB,errors[1]),.05,0);
}

void BeamAnalysis3()
{
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
	resolution(input[1].size(),queueToArray(input[0]),queueToArray(input[1]),queueToArray(input[3]),queueToArray(input[2]),queueToArray(input[4]));
}
