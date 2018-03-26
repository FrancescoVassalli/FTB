

using namespace std;

void BeamAnalysis2(){
	const int nMeanBins = 5;
	float meanBins[nMeanBins+1] = {0,2,4,6,8,12};
	float adc12[nMeanBins] = {1392,2830,4145,5445,7517};
	float sigma[nMeanBins] = {87,123,149,172,194};
	TCanvas *canvas1 = new TCanvas();
	TH1F* mean = new TH1F("mean","",nMeanBins,meanBins);
	for (int i = 1; i <= nMeanBins; ++i)
	{
		mean->SetBinContent(i,adc12[i-1]);
		mean->SetBinError(i,sigma[i-1]);
	}
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
	mean->Draw("pe");
	myText(.43,.24,kRed,Form("Linear #Chi^2: %0.2f NDF: %i",chi,ndf),.05);
	myText(.43,.19,kRed,Form("#Chi^2/NDF: %0.2f",chi/ndf),.05);
	myText(.43,.34,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.43,.29,kRed,Form("C2/C1: %0.4f",nonLinearFactor),.05);
	TH1F* ehist = new TH1F("ehist","",nMeanBins,meanBins);
	for (int i = 1; i <= nMeanBins; ++i)
	{
		ehist->SetBinContent(i,sigma[i-1]/adc12[i-1]);
		ehist->SetBinError(i,errorDivide(sigma[i-1],0,adc12[i-1],sigma[i-1]));
	}
	//canvas1->Clear("D");
	ehist->Fit(eF);
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	ehist->Draw("p");
	axisTitles(ehist,"Beam Energy GeV","#sigma/mean");
	chi = eF->GetChisquare();
	ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#Chi^2:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#Chi^2/NDF:%0.2f",chi/ndf),.05);
	myText(.3,.8,kRed,Form("Stochastic: %0.2f#pm%0.3f Constant: %0.2f#pm%0.3f",eA,errors[0],eB,errors[1]),.05);

}