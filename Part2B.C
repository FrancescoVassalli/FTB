// Made by Francesco Vassalli for analysis of PbGl detector for sPHENIX EMCal calibration effort
// 6/27/18 at the CU Boulder
// This script takes the mean, sigma, mean error, and sigma error from Part2A.C's
// 2 output files and plots the 1200V and 1100V data trendline together. 
// It will also plot the combined resolution of each data set
#include <queue>
#include "/Users/Chase/Documents/HeavyIonsResearch/FranTools/Bin/NiceHists.C" //for chase
void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {
  	TLatex l; //l.SetTextAlign(12); 
  	l.SetTextSize(tsize); 
  	l.SetNDC();
 	l.SetTextColor(color);
  	l.DrawLatex(x,y,text);
}	

using namespace std;




Scalar trendForced(const int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror){
	float *ex;
	ex= zeroArray(SIZE,ex);
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,ex,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[SIZE-1]);
	axisTitles(p_mean,"Beam Energy GeV","Mean PbGl");
	//gNice();
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
	//doubleZero(p_mean,mean[SIZE-1]+1000,energy[SIZE-1]+1);
	p_mean->Draw("AP");
	//p_mean->GetXaxis()->SetLimits(0,energy[SIZE-1]+1);
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
Scalar trendForcedQuiet(const int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror){
	float *ex;
	ex= zeroArray(SIZE,ex);
	TCanvas *canvas1 = new TCanvas();
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,ex,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[SIZE-1]);
	axisTitles(p_mean,"Beam Energy GeV","Mean PbGl");
	//gNice();
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
	//doubleZero(p_mean,mean[SIZE-1]+1000,energy[SIZE-1]+1);
	//p_mean->Draw("AP");
	//p_mean->GetXaxis()->SetLimits(0,energy[SIZE-1]+1);
	//poly->SetLineColor(kBlue);
	//poly->Draw("same");
	//lin->Draw("same");
	//myText(.5,.27,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.05);
	//myText(.5,.22,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	//myText(.5,.37,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	//myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	//myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);
	return Scalar(linearFactor,linearError);
}

struct Data
{
	Scalar mean;
	Scalar sigma;
	int energy;
};

queue<Data>* singlefileAnalysis(string filename){
	ifstream inFile (filename.c_str()); //txt file containing the data from Part2A
	cout<<"Opened file!"<<endl;
	const int LINES = 6;
	queue<float> input[LINES]; //create array of queues
	string intemp;
	stringstream ss;
	for (int i = 0; i < LINES; ++i) //loop over each beam files data
	{
		inFile>>intemp;
		ss<<intemp;
		getline(ss,intemp,',');
		//cout<<intemp<<":\n";
		while(getline(ss,intemp,',')){   //loop to put data from each line into each queue at the same place in the arrays
			input[i].push(stof(intemp));
			//cout<<intemp<<endl;
		}
		ss.clear();
	}
	//inFile.close();
	queue<Data>* rdata = new queue<Data>();
	//convert each of the queues into an array resulting in arrays of GeV, mean, sigma, mean error, and sigma error
	Scalar slope = trendForced(input[5].size(),queueToArray(input[5]),queueToArray(input[1]),queueToArray(input[2]),queueToArray(input[3]),queueToArray(input[4]));
	//convert the arrays to energy 
	Data temp;
	while (!input[5].empty())
	{
		temp.mean = Scalar(input[1].front(),input[2].front())/slope; //these Q's might be empty b/c of the queueToarray
		temp.sigma = Scalar(input[3].front(),input[4].front())/slope;
		temp.energy = input[5].front();
		rdata->push(temp);
		input[1].pop();
		input[2].pop();
		input[3].pop();
		input[4].pop();
		input[5].pop();
	}
	//return all the arrays in terms of energy 
	cout<<slope;
	return rdata;
}

void combinedplot(queue<Data>* data){
	cout<<"Enter plot"<<endl;
	const int SIZE = data->size();
	float *ex;
	ex= zeroArray(SIZE,ex);
	TCanvas *canvas1 = new TCanvas();
	cout<<"The main canvas"<<endl;
	float energy[SIZE];
	float mean[SIZE];
	float meanerror[SIZE];
	int i=0;
	while(!data->empty()){
		energy[i] = data->front().energy;
		mean[i] = data->front().mean.value;
		meanerror[i] = data->front().mean.uncertainty;
		data->pop();
		i++;
	}
	cout<<"data arrays made"<<endl;
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,ex,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[SIZE-1]);
	axisTitles(p_mean,"Beam Energy GeV","Mean PbGl");
	//gNice();
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
	//doubleZero(p_mean,mean[SIZE-1]+1000,energy[SIZE-1]+1);
	p_mean->Draw("AP");
	//p_mean->GetXaxis()->SetLimits(0,energy[SIZE-1]+1);
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
	myText(.5,.27,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.05);
	myText(.5,.22,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05);
	myText(.5,.37,kRed,Form("C1 = %0.4f #pm %0.2f",linearFactor,linearError),.05);
	myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);

}

queue<Data>* sortcombine(queue<Data>* d1, queue<Data>* d2){
	cout<<"Enter combine"<<endl;
	queue<Data> *rdata = new queue<Data>();
	while (!d1->empty()||!d2->empty())
	{
		if(!d1->empty()^!d2->empty()) //if one of them is empty put non empty on end of list
		{
			if(d1->empty()) //if queue one empty
			{
				while(!d2->empty())
				{
					rdata->push(d2->front());
					d2->pop();
				}
			}
			else //if queue 2 empty
			{
				while(!d1->empty())
				{
					rdata->push(d1->front());
					d1->pop();
				}
			}
		}
		else //if both queues are full
		{
			int currentEnergy = min(d1->front().energy,d2->front().energy);
			while(d1->front().energy==currentEnergy){
				rdata->push(d1->front());
				d1->pop();
			}
			while(d2->front().energy==currentEnergy){
				rdata->push(d2->front());
				d2->pop();
			}
		}
	}
	cout<<"Exit Combine"<<endl;
	delete d1;
	delete d2;
	return rdata;
}
//trendForced(const int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror)
//trendForced(input[5].size(),queueToArray(input[5]),queueToArray(input[1]),queueToArray(input[2]),queueToArray(input[3]),queueToArray(input[4]))

void resolution(queue<Data>* temp){
	int SIZE = temp->size();
	float mean[SIZE]; 
	float sigma[SIZE]; 
	float meanerror[SIZE]; 
	float sigmaerror[SIZE];
	float energy[SIZE];
	float ex[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		ex[i] = 0;
		mean[i] = temp->front().mean.value;
		meanerror[i] = temp->front().mean.uncertainty;
		sigma[i] = temp->front().sigma.value;
		sigmaerror[i] = temp->front().sigma.uncertainty;
		energy[i] = temp->front().energy;
		temp->pop();
	}
	TCanvas *canvas1 = new TCanvas();
	float relativeE[SIZE];
	float relativeU[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		relativeE[i] = sigma[i]/energy[i];
		relativeU[i]= errorDivide(sigma[i],sigmaerror[i],energy[i],meanerror[i]);
		//cout<<relativeU[i]<<endl;
	}
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,mean[SIZE-1]);
	eF->SetLineColor(kRed);
	TGraphErrors* ehist = new TGraphErrors(SIZE,mean,relativeE,ex,relativeU);
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
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05);
	myText(.24,.85,kRed,Form("Stochastic: %0.6f#pm%0.6f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Constant: %0.6f#pm%0.6f",eB,errors[1]),.05);
}

void combinedResolution(string filename1, string filename2){
	cout<<"Started Resolution!"<<endl;
	ifstream inFile1 (filename1.c_str()); //txt file containing the first data file from Part2A
	cout<<"Opened file2!"<<endl;
	const int LINES = 6;
	queue<float> input1[LINES]; //create array of queues
	string intemp1;
	stringstream ss1;
	for (int i = 0; i < LINES; ++i) //loop over each beam files data
	{
		inFile1>>intemp1;
		ss1<<intemp1;
		getline(ss1,intemp1,',');
		//cout<<intemp<<":\n";
		while(getline(ss1,intemp1,',')){   //loop to put data from each line into each queue at the same place in the arrays
			input1[i].push(stof(intemp1));
			//cout<<intemp<<endl;
		}
		ss1.clear();
	}
	Scalar slope1 = trendForcedQuiet(input1[5].size(),queueToArray(input1[5]),queueToArray(input1[1]),queueToArray(input1[2]),queueToArray(input1[3]),queueToArray(input1[4]));
	queue<Data>* rdata1= new queue<Data>();
	Data temp1;
	cout<<"Reading Into Data Queue"<<endl;
	while (!input1[5].empty())
	{
		temp1.mean = Scalar(input1[1].front(),input1[2].front())/slope1; //these Q's might be empty b/c of the queueToarray
		temp1.sigma = Scalar(input1[3].front(),input1[4].front())/slope1;
		temp1.energy = input1[5].front();
		rdata1->push(temp1);
		input1[1].pop();
		input1[2].pop();
		input1[3].pop();
		input1[4].pop();
		input1[5].pop();
		//cout<<"work"<<i<<'\n';
	}
	//inFile1.close();
	ifstream inFile2 (filename2.c_str()); //txt file containing the 2nd data file from Part2A
	cout<<"Opened file3!"<<endl;
	queue<float> input2[LINES]; //create array of queues
	string intemp2;
	stringstream ss2;
	for (int i = 0; i < LINES; ++i) //loop over each beam files data
	{
		inFile2>>intemp2;
		ss2<<intemp2;
		getline(ss2,intemp2,',');
		//cout<<intemp<<":\n";
		while(getline(ss2,intemp2,',')){   //loop to put data from each line into each queue at the same place in the arrays
			input2[i].push(stof(intemp2));
			//cout<<intemp<<endl;
		}
		ss2.clear();
	}
	Scalar slope2 = trendForcedQuiet(input2[5].size(),queueToArray(input2[5]),queueToArray(input2[1]),queueToArray(input2[2]),queueToArray(input2[3]),queueToArray(input2[4]));
	queue<Data>* rdata2 = new queue<Data>();
	Data temp2;
	while (!input2[5].empty())
	{
		temp2.mean = Scalar(input2[1].front(),input2[2].front())/slope2; //these Q's might be empty b/c of the queueToarray
		temp2.sigma = Scalar(input2[3].front(),input2[4].front())/slope2;
		temp2.energy = input2[5].front();
		rdata2->push(temp2);
		input2[1].pop();
		input2[2].pop();
		input2[3].pop();
		input2[4].pop();
		input2[5].pop();
		//cout<<"work"<<i<<'\n';
	}
	//inFile2.close();
	queue<Data>* rdata = sortcombine(rdata1, rdata2);
	resolution(rdata);
}

void Part2B(){
	combinedplot(sortcombine(singlefileAnalysis("PbGlA1200.txt"),singlefileAnalysis("PbGlA1100.txt")));
	combinedResolution("PbGlA1200.txt","PbGlA1100.txt");
}





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