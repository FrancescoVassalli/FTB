
using namespace std;

Scalar trendForced(const int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror){
	float ex[nMeanBins]; // make a zero array
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,ex,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[SIZE-1]);
	axisTitles(p_mean,"Beam Energy GeV","Mean PbGl");
	gNice();
	p_mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	p_mean->Fit(lin,"0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	//cout<<"C2/C1: "<<nonLinearFactor<<" / "<<linearFactor<<" = "<<nonLinearFactor/linearFactor<<endl;
	double linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	//cout<<"Ratio: "<<ratiouncertainty<<endl;
	p_mean->SetMarkerStyle(kOpenCircle);
	doubleZero(p_mean,mean[SIZE-1]+1000,energy[SIZE-1]+1);
	p_mean->Draw("AP");
	p_mean->GetXaxis()->SetLimits(0,energy[SIZE-1]+1);
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
	myText(.5,.27,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.05);
	myText(.5,.22,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	myText(.5,.37,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);
	return Scalar(linearFactor,linearError);
}

void BeamAnalysis2(){
	ifstream inFile ("PbGlA.txt"); //txt file containing the data from Part2A
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
	cout<< trendForced(input[5].size(),queueToArray(input[5]),queueToArray(input[1]),queueToArray(input[2]),queueToArray(input[3]),queueToArray(input[4]));
	/*ofstream outFile;
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
*/
}