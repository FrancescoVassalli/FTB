

using namespace std;

void BeamAnalysis2(){
	const int nMeanBins = 5;
	float meanBins[nMeanBins] = {2,4,6,8,12};
	float adc12[nMeanBins] = {1392,2830,4145,5445,7517};
	float sigma[nMeanBins] = {87,123,149,172,194};
	float ex[nMeanBins]= {0,0,0,0,0};
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* mean = new TGraphErrors(nMeanBins,meanBins,adc12,ex,sigma); // how to set the uncertainty
	TF1* lin = new TF1("lin","[1]*x+[0]",0,12);
	TF1* poly = new TF1("poly","[2]*x*x+[1]*x+[0]",0,12);
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,12);
	eF->SetLineColor(kRed);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(2)/poly->GetParameter(1);
	double nonLinearError = poly->GetParError(2);
	mean->Fit(lin);
	lin->SetLineColor(kRed);
	float linearFactor = lin->GetParameter(1);
	float linearError = lin->GetParError(1);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	cout<<"Ratio: "<<ratiouncertainty<<endl;
	mean->SetMarkerStyle(kOpenCircle);
	mean->Draw("AP");
	myText(.43,.24,kRed,Form("Linear #Chi^2: %0.2f NDF: %i",chi,ndf),.05);
	myText(.43,.19,kRed,Form("#Chi^2/NDF: %0.2f",chi/ndf),.05);
	myText(.43,.34,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.43,.29,kRed,Form("C2/C1: %0.3f#pm %0.3f",nonLinearFactor/linearFactor,ratiouncertainty),.05);

}