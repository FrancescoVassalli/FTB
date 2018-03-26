using namespace std;

void BeamAnalysis3(){
	const int nMeanBins = 5;
	float meanBins[nMeanBins] = {2,4,6,8,12};
	float adc12[nMeanBins] = {1392,2830,4145,5445,7517};
	float sigma[nMeanBins] = {87,123,149,172,194};
	float ex[nMeanBins]= {0,0,0,0,0};
	TCanvas *canvas1 = new TCanvas();
	float relativeE[nMeanBins];
	float relativeU[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		relativeE[i] = sigma[i]/adc12[i];
		relativeU[i]= errorDivide(sigma[i],0,adc12[i],sigma[i]);
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
	myText(.3,.75,kRed,Form("#Chi^2:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#Chi^2/NDF:%0.2f",chi/ndf),.05);
	myText(.24,.85,kRed,Form("Stochastic: %0.6f#pm%0.6f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Constant: %0.6f#pm%0.6f",eB,errors[1]),.05);
}