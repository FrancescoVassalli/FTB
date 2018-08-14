// Made by Francesco Vassalli for analysis of PbGl detector for sPHENIX EMCal calibration effort
// 6/27/18 at the CU Boulder
// This script takes the mean, sigma, mean error, and sigma error from Part2A.C's
// 2 output files and plots the 1200V and 1100V data trendline together. 
// It will also plot the combined resolution of each data set
#include <Scalar.h>
#include <sstream>
#include "TH1D.h"
#include "TChain.h"
#include <iostream>
#include "TF1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
//#include "NicePlots.C"
#include "Frannamespace.C"
// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"
#include <queue>
//#include "/Users/Chase/Documents/HeavyIonsResearch/FranTools/Bin/NiceHists.C" //for chase

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {
  	TLatex l; //l.SetTextAlign(12); 
  	l.SetTextSize(tsize); 
  	l.SetNDC();
 	l.SetTextColor(color);
  	l.DrawLatex(x,y,text);
}	


using namespace std;
using namespace Frannamespace;


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
	myText(.5,.37,kRed,Form("Linear = %0.4f #pm %0.4f",linearFactor,linearError),.05);
	myText(.5,.32,kRed,Form("C2: %0.3f#pm %0.3f",nonLinearFactor,nonLinearError),.05);
	myText(.5,.17,kRed,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05);
	//canvas1->Close();
	//delete canvas1;
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
	inline friend std::ostream& operator<<(std::ostream& os, Data const & tc) {
       return os <<"Data:"<<tc.energy<<"GeV \n\tmean:" << tc.mean <<"\t sigma:"<<tc.sigma;
    }
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
	cout<<"Enter plot data size:"<<data->size()<<endl;
	const int SIZE = data->size();
	TCanvas *canvas1 = new TCanvas();
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
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,nullptr,meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[SIZE-1]);
	axisTitles(p_mean,"Beam Energy [GeV]","Measured Energy [GeV]");
	//gNice();
	p_mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	double polylinear = poly->GetParameter(0);
	double polylinearError = poly->GetParError(0);
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
	myText(.15,.86,kBlue,Form("Quad: E_{PbGl} = (%0.3f#pm %0.3f)*(E_{beam})^{2} + (%0.3f#pm %0.3f)*E_{beam}",nonLinearFactor,nonLinearError,polylinear,polylinearError),.04);
	myText(.15,.815,kBlue,Form("Quad: #chi^{2}/NDF: %0.2f",chi2/ndf),.04);
	myText(.15,.77,kRed,Form("Linear: E_{PbGl} = (%0.3f #pm %0.3f)*E_{beam}",linearFactor,linearError),.04);
	//myText(.15,.77,kRed,Form("Linear #chi^{2}: %0.2f NDF: %i",chi,ndf),.04);
	myText(.15,.725,kRed,Form("Linear: #chi^{2}/NDF: %0.2f",chi/ndf),.04);

}

queue<Data>* sortcombine(queue<Data>* d1, queue<Data>* d2){
	cout<<"Enter sort combine"<<endl;
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
	delete d1;
	delete d2;
	return rdata;
}
Data weightedAverage(queue<Data>* temp)
{
	Data tempdata;
	tempdata.energy = temp->front().energy;
	float meanD = 0;
	float sigmaD = 0;
	float meanError = 0;
	float sigmaError = 0;
	int size = temp->size();
	for(int i = 0; i < size; i++) //weighted average
	{
		//add up mean values
		tempdata.mean.value += (1/temp->front().mean.uncertainty)*(temp->front().mean.value); //adding means
		meanD += (1/temp->front().mean.uncertainty); //demoninator for mean
		meanError += pow(temp->front().mean.uncertainty,2); //add mean uncertainties is quadrature
		//add up sigma values
		tempdata.sigma.value += (1/temp->front().sigma.uncertainty)*(temp->front().sigma.value); //adding sigmas
		sigmaD += (1/temp->front().sigma.uncertainty); //demoninator for sigma
		sigmaError += pow(temp->front().sigma.uncertainty,2);//add sigma uncertainties is quadrature
		temp->pop();
	}
	tempdata.mean.value = tempdata.mean.value/meanD; //finish mean weighted average 
	tempdata.sigma.value = tempdata.sigma.value/sigmaD; //finish mean weighted average
	tempdata.mean.uncertainty = sqrt(meanError)/size; //set new uncertainties
	tempdata.sigma.uncertainty = sqrt(sigmaError)/size;
	return tempdata;
}

Data combinePoint(queue<Data>* temp)
{
	Data tempdata;
	int tempenergy = temp->front().energy;
	int size = temp->size();
	if(temp->size() == 1) //only one point, return this point
	{
		tempdata.mean = temp->front().mean;
		tempdata.sigma= temp->front().sigma;
		tempdata.energy = temp->front().energy;
		return tempdata;
	}
	else
	{
		return weightedAverage(temp);
	}
}

queue<Data>* combineAllPoints(queue<Data>* temp)
{
	cout<<"Enter combineAll"<<'\n';
	const int kTotalPoints = temp->size();
	queue<Data>* rdata = new queue<Data>();
	Data *working = queueToArray(*temp);
	int bigI=0;
	while(bigI < kTotalPoints)
	{
		int nextPoint=bigI;
		queue<Data>* currentpoints = new queue<Data>(); //queue to be combined
		while(nextPoint<=kTotalPoints&&working[nextPoint].energy==working[bigI].energy){
			currentpoints->push(working[nextPoint]);
			nextPoint++;
		} 
		cout<<bigI<<"-"<<nextPoint<<'\n';
		cout<<"Energy"<<working[bigI].energy<<endl;
		rdata->push(combinePoint(currentpoints));
		bigI=nextPoint;
		delete currentpoints;
	}
	return rdata;
}

void resolution(queue<Data>* temp){
	int SIZE = temp->size();
	float energy[SIZE];
	float relativeE[SIZE];
	float relativeU[SIZE];
	int loopi = 0;
	while (!temp->empty())
	{
		energy[loopi] = temp->front().energy;
		Scalar res = temp->front().sigma/temp->front().mean;
		relativeE[loopi] = res.value;
		relativeU[loopi]= res.uncertainty;
		loopi++;
		temp->pop();
	}
	TCanvas *canvas1 = new TCanvas();
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,energy[SIZE-1]);
	eF->SetLineColor(kRed);
	TGraphErrors* ehist = new TGraphErrors(SIZE,energy,relativeE,nullptr,relativeU);
	ehist->Fit(eF,"M");
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	//makeMarkerNice(&ehist, 1);
	ehist->SetMarkerSize(2);
	ehist->SetMarkerStyle(41);
	ehist->SetMarkerColor(4);
	ehist->SetLineColor(4);
	ehist->GetXaxis()->SetLimits(0,energy[SIZE-1]+1);
	gPad->SetTicks();
	ehist->Draw("AP");
	axisTitles(ehist,"Beam Energy GeV","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05);
	myText(.24,.85,kRed,Form("Stochastic: %0.6f#pm%0.6f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Constant: %0.6f#pm%0.6f",eB,errors[1]),.05);
}

void resolutionAnabel(queue<Data>* notCombined) //doing resolution vs 1/sqrt(E)
{
	queue<Data>* temp = combineAllPoints(notCombined); //combined points
	//queue<Data>* temp = notCombined; //not combined points
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

	for (int i = 0; i < SIZE; ++i)
	{
		energy[i] = 1/sqrt(energy[i]);
		
	}

	TF1* eF = new TF1("eF","[0]*x+[1]",0,1);
	eF->SetLineColor(kRed);
	eF->SetRange(0,1);
	TGraphErrors* ehist = new TGraphErrors(SIZE,energy,relativeE,ex,relativeU);
	ehist->Fit(eF,"M");
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	//makeMarkerNice(&ehist, 1);
	ehist->SetMarkerSize(2);
	ehist->SetMarkerStyle(41);
	ehist->SetMarkerColor(4);
	ehist->SetLineColor(4);
	ehist->SetMinimum(0);
	ehist->SetMaximum(0.1);
	ehist->GetXaxis()->SetLimits(0,1);
	gPad->SetTicks();
	ehist->Draw("AP");
	axisTitles(ehist,"1/#sqrt{GeV}","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f NDF:%i",chi,ndf),.05);
	myText(.3,.7,kRed,Form("#chi^{2}/NDF:%0.2f",chi/ndf),.05);
	myText(.24,.85,kRed,Form("Slope: %0.6f#pm%0.6f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Intercept: %0.6f#pm%0.6f",eB,errors[1]),.05);
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
	//resolutionAnabel(rdata);
}

void Part2B(){
	singlefileAnalysis("PbGlA10004x4.txt");
	//queue<Data>* data =sortcombine(sortcombine(singlefileAnalysis("PbGlA12004x4.txt"),singlefileAnalysis("PbGlA11004x4.txt")),singlefileAnalysis("PbGlA10004x4.txt"));
	/*while(!data->empty()){
		cout<<data->front();
		data->pop();
	}*/
	//combinedplot(data);
	//resolution(data);
	//combinedResolution("PbGlA12004x4.txt","PbGlA11004x4.txt");
}