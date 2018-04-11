
/* dividing the mean and the sigma by the linear factor and propogating that error.  Need to check with Ron that those errors are right*/
queue<float>* adcToEnergy(float linearFactor, float linearFactorError, int SIZE, float* means, float* meanerror, float* inputEnergy, float* sigma, float* sigmaerror){
	errorDivideArray(SIZE,means,meanerror,linearFactor,linearFactorError);
	errorDivideArray(SIZE,sigma,sigmaerror,linearFactor,linearFactorError);
	queue<float> *r = new queue<float>[5];
	for (int i = 0; i < SIZE; ++i)
	{
		r[1].push(means[i]);
		r[2].push(meanerror[i]);
		r[3].push(sigma[i]);
		r[4].push(sigmaerror[i]);
		r[0].push(inputEnergy[i]);
	}
	return r;
}

void resolution(const int nMeanBins,float*inputEnergy, float* outEnergy, float* sigma, float* meanerror, float* sigmaerror,const unsigned int nFiles){
	float ex[nMeanBins];
	int peakInput=0;
	int tempenergy=inputEnergy[0];
	int fileBeginIndex[nFiles+1];
	int fileBeginIndexCounter=1;
	fileBeginIndex[0]=0;
	for (int i = 0; i < nMeanBins; ++i)
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
	fileBeginIndex[fileBeginIndexCounter]=nMeanBins-1;
	peakInput++;
	TCanvas *canvas1 = new TCanvas();
	float relativeE[nMeanBins];
	float relativeU[nMeanBins];
	for (int i = 0; i < nMeanBins; ++i)
	{
		relativeE[i] = sigma[i]/outEnergy[i];
		//cout<<relativeE[i]<< ": ";
		cout<<sigma[i]<<": "<<sigmaerror[i]<<", "<<outEnergy[i]<<" : "<<meanerror[i]<<'\n';
		relativeU[i]= errorDivide(sigma[i],sigmaerror[i],outEnergy[i],meanerror[i]);
		//cout<<relativeU[i]<<'\n';
	}
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,peakInput);
	TF1* old = new TF1("old","TMath::Sqrt(.02*.02+.014*.014+(.05*.05)/x)",0, peakInput);
	old->SetLineColor(kBlue);
	eF->SetLineColor(kRed);
	TGraphErrors* ehist = new TGraphErrors(nMeanBins,inputEnergy,relativeE,ex,relativeU);
	ehist->Fit(eF);
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);

	TGraphErrors** plotgraphs = new TGraphErrors*[nFiles];
	for (unsigned i = 0; i < nFiles; ++i)
	{
		
		plotgraphs[i] = new TGraphErrors(fileBeginIndex[i+1]-fileBeginIndex[i],partialArray(inputEnergy,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(relativeE,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(ex,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(relativeU,fileBeginIndex[i],fileBeginIndex[i+1]));
	}
	plotgraphs[0]->SetMarkerSize(2);
	plotgraphs[0]->SetMinimum(0);
	plotgraphs[0]->SetMaximum(0.1);
	plotgraphs[0]->GetXaxis()->SetLimits(0,peakInput);
	plotgraphs[0]->SetMarkerStyle(kOpenCircle);
	plotgraphs[0]->Draw("AP");
	plotgraphs[1]->SetMarkerStyle(kOpenTriangleDown);
	for (unsigned i = 1; i < nFiles; ++i)
	{
		plotgraphs[i]->SetMarkerSize(2);
		plotgraphs[i]->Draw("P");
	}
	old->Draw("same");
	eF->Draw("same");
	axisTitles(ehist,"Beam Energy GeV","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.5,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05,0);
	myText(.5,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05,0);
	myText(.5,.85,kRed,Form("Stochastic: %0.5f#pm%0.5f ",eA,errors[0]),.05,0);
	myText(.5,.8,kRed,Form("Constant: %0.5f#pm%0.5f",eB,errors[1]),.05,0);
	myText(.2,.2,kBlue,"2016",.05,0);
	myMarkerText(.7,.25,kBlack,kOpenTriangleDown,"1100V",2,.05);
	myMarkerText(.7,.2,kBlack,kOpenCircle,"1200V",2,.05);
}


void BeamAnalysis5(){
	ifstream inFile ("PbGl_dataC.txt"); //txt file containing the names of files to process
	queue<string> files;
	string intemp;
	while(getline(inFile,intemp)){
		files.push(intemp);
	}
	inFile.close();
	const unsigned int filecount = files.size();
	const int LINES =5;
	float linearFactor[files.size()];
	float linearFactorError[files.size()];
	int count=0;
	queue<float> totalinput[5];
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
				//cout<<intemp<<endl;
			}
			ss.clear();
		}
		inFile.close();
		tempenergy=adcToEnergy(linearFactor[count], linearFactorError[count], input[1].size(),queueToArray(input[1]),queueToArray(input[2]),queueToArray(input[0]),queueToArray(input[3]),queueToArray(input[4]));
		while(!tempenergy[0].empty()){
			totalinput[0].push(tempenergy[0].front()); //input energy
			totalinput[1].push(tempenergy[1].front()); //mean
			totalinput[2].push(tempenergy[2].front()); //meanerror
			totalinput[3].push(tempenergy[3].front()); //sigma
			totalinput[4].push(tempenergy[4].front()); //sigmaerror
			tempenergy[0].pop();
			tempenergy[1].pop();
			tempenergy[2].pop();
			tempenergy[3].pop();
			tempenergy[4].pop();
		}
		count++;
	}
	resolution(totalinput[0].size(),queueToArray(totalinput[0]),queueToArray(totalinput[1]),queueToArray(totalinput[3]),queueToArray(totalinput[2]),queueToArray(totalinput[4]),filecount);
}