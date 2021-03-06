/*Made by Francesco Vassalli for analysis of PbGl detector for sPHENIX EMCal calibration effort
	June -October 
	This script takes the mean, sigma, mean error, and sigma error from Part2A.C's
	output files and plots the linearity and resolution. 
	*/
#include "Frannamespace.C"
#include <Scalar.h>

#include <TH1D.h>
#include <TChain.h>
#include <TF1.h>
#include <TLinearFitter.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TMap.h>
#include "TClonesArray.h"
#include "TObject.h"

#include <queue>
#include <iostream>
#include <sstream>
//#include "/Users/Chase/Documents/HeavyIonsResearch/FranTools/Bin/NiceHists.C" //for chase

namespace {
	int nameCount=0;
}

//holds the info for one run's data 
struct Data
{
	Scalar mean;
	Scalar sigma;
	int energy;
	Data operator /(Data& in){
		Scalar scale = in.mean/Scalar(in.energy);
		mean/=scale;
		sigma/=scale;
		return *this;
	}
	inline friend std::ostream& operator<<(std::ostream& os, Data const & tc) {
		return os <<"Data:"<<tc.energy<<"GeV \n\tmean:" << tc.mean <<"\t sigma:"<<tc.sigma;
	}
};

using namespace std;
using namespace Frannamespace;
queue<Data>* sortcombine(queue<Data>* d1, queue<Data>* d2);
queue<Data>* combineAllPoints(queue<Data>* temp);
void chiAnalysis(TGraphErrors* graph, TF1* lin,float* runNum);
void chiAnalysis(TGraphErrors* graph, TF1* lin);

const char* namer(int* count){
	std::string r= std::to_string(*count);
	*count=*count+1;
	return r.c_str();
}

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {
	TLatex l; //l.SetTextAlign(12); 
	l.SetTextSize(tsize); 
	l.SetNDC();
	l.SetTextColor(color);
	l.DrawLatex(x,y,text);
}	

queue<Data>* toDataQueue(TGraphErrors* graph){
	int SIZE=graph->GetN();
	double* energy = graph->GetX();
	double* y=graph->GetY();
	double* yu= graph->GetEY();
	queue<Data> *temp = new queue<Data>();
	for (int i = 0; i < SIZE; ++i)
	{
		Data dtemp;
		dtemp.energy=energy[i];
		dtemp.mean=Scalar(y[i],yu[i]);
		dtemp.sigma=Scalar(1,1);
		temp->push(dtemp);
	}
	return temp;
}

TGraphErrors* combineAllPoints(TGraphErrors* g1, TGraphErrors* g2){
	queue<Data> *temp= combineAllPoints(sortcombine(toDataQueue(g1),toDataQueue(g2)));
	int SIZE = temp->size();
	float energy[SIZE];
	float y[SIZE];
	float yu[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		energy[i] = (float)temp->front().energy;
		y[i] = temp->front().mean.value;
		yu[i] = temp->front().mean.uncertainty;
		temp->pop();
	}
	TGraphErrors *r = new TGraphErrors(SIZE,energy,y,nullptr,yu);
	delete temp;
	return r;
}

TGraphErrors* combineAllPoints(TGraphErrors* g1){
	queue<Data> *temp= combineAllPoints(toDataQueue(g1));
	int SIZE = temp->size();
	float energy[SIZE];
	float y[SIZE];
	float yu[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		//cout<<temp->front();
		energy[i] = (float)temp->front().energy;
		y[i] = temp->front().mean.value;
		//cout<<temp->front().mean;
		yu[i] = temp->front().mean.uncertainty;
		temp->pop();
	}
	TGraphErrors *r = new TGraphErrors(SIZE,energy,y,nullptr,yu);
	delete temp;
	return r;
}

TGraphErrors* unitConverter(TGraphErrors* graph){
	TF1* lin = (TF1*) graph->FindObject("lin");
	Scalar yInt(lin->GetParameter(1),lin->GetParError(1));
	Scalar scale(lin->GetParameter(0),lin->GetParError(0));
	double *values = graph->GetY();
	double *errors = graph->GetEY();
	const int kSIZE = graph->GetN();
	for (int i = 0; i < kSIZE; ++i)
	{
		Scalar temp(values[i],errors[i]); 
		temp-=yInt;
		temp/=scale;
		values[i]=temp.value;
		errors[i]=temp.uncertainty;
	}
	return graph;
}

float getPointCoVarience(TGraphErrors* data, int i, double slope);

//calculates the electronic signal to GeV conversion and calls unitconverter to preform it
TGraphErrors* graphConvert(const int SIZE,float*energy, float* mean, float* meanerror,float* runNum,bool convert=true){
	TCanvas *tc = new TCanvas();
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,nullptr,meanerror); // how to set the uncertainty
	p_mean=combineAllPoints(p_mean);
	//the slope of this linear fit is the conversion factor
	TF1* lin = new TF1("lin","[0]*x+[1]",0,energy[SIZE-1]);
	p_mean->Fit(lin,"M0");
	p_mean->SetMarkerStyle(kDot);
	p_mean->Fit(lin,"M0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	double linearError = lin->GetParError(0);
	double bterm=lin->GetParameter(1);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	p_mean->SetTitle(";Beam Energy [GeV];ADC signal");
	p_mean->SetMarkerStyle(kOpenCircle);
	p_mean->SetMarkerStyle(27);
	p_mean->Clone()->Draw("AP");
	lin->Draw("same");
	myText(.15,.77,kRed,Form("E_{PbGl} = (%0.3f #pm %0.3f)*E_{beam}+%0.3f",linearFactor,linearError,bterm),.04);
	myText(.15,.725,kRed,Form("#chi^{2}: %0.2f",chi),.04);
	myText(.25,.725,kRed,Form("NDF: %i",ndf),.04);
	if (convert)
	{
		p_mean=unitConverter(p_mean);
	}
	return p_mean;
}

//deprecated way of finding unit convertion more similar to the 2016 analysis 
queue<Data>* pointScaling(queue<Data>* inData){
	const int kSIZE=inData->size();
	Data* points = queueToArray(*inData);
	int i=0;
	while (points[i].energy!=4&&i < kSIZE) i++;
	Data scale = points[i];
	i=0;
	delete inData;
	cout<<"Scaling with: \n \t"<<scale;
	queue<Data> *rdata = new queue<Data>;
	while(i<kSIZE){
		rdata->push(points[i]/scale);
		i++;
	}
	delete [] points;
	return rdata;
}

//calculates the resolution from the text file 
TGraphErrors* makeResolutionFromArrays(int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror,TGraphErrors* graph){
	float y[SIZE];
	float yu[SIZE];
	TF1* lin = (TF1*)graph->FindObject("lin");	
	Scalar yInt(lin->GetParameter(1),lin->GetParError(1));
	Scalar scale(lin->GetParameter(0),lin->GetParError(0));
	for (int i = 0; i < SIZE; ++i)
	{
		Scalar tmean(mean[i],meanerror[i]);
		Scalar tsigma(sigma[i],sigmaerror[i]);
		tmean/=scale;
		tsigma/=scale;
		Scalar temp = tsigma/tmean;
		y[i] = temp.value;
		yu[i]= temp.uncertainty;
	}
	return new TGraphErrors(SIZE,energy,y,nullptr,yu);
}

//reads to data out of the text files and calls the methods to get single voltage graphs in units of GeV
pair<TGraphErrors*, TGraphErrors*> singlefileConverter(string filename,bool removeF=false,bool convert=true){
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
		while(getline(ss,intemp,',')){   //loop to put data from each line into each queue at the same place in the arrays
			input[i].push(stof(intemp));
			/*if (i==5) //print the energies 
				{
				cout<<intemp<<endl;
				}*/
		}
		ss.clear();
	}
	const int SIZE = input[5].size();
	float* runNum = queueToArray(input[0]);
	float* energy = queueToArray(input[5]);
	float* mean = queueToArray(input[1]);
	float* meanError = queueToArray(input[2]);
	float* sigma = queueToArray(input[3]);
	float* sigmaError = queueToArray(input[4]);
	TGraphErrors* lin =graphConvert(SIZE,energy,mean,meanError,runNum,convert);
	string title = filename+";Beam Energy [GeV];Measured Energy [GeV]";
	lin->SetTitle(title.c_str());
	TGraphErrors* res = makeResolutionFromArrays(SIZE,energy,mean,sigma,meanError,sigmaError,lin);
	if (removeF)
	{
		lin->RemovePoint(0);
		res->RemovePoint(0);
	}
	title=filename+";resolution;Beam Energy [GeV]";
	res->SetTitle(title.c_str());
	return pair<TGraphErrors*,TGraphErrors*>(lin,res);
}

//does the full analysis for the linearity with just 1 file double file is a misnomer but it allows overloading
TGraphErrors* doubleFileAnalysis(TGraphErrors* g1){
	TCanvas* tc = new TCanvas();
	TF1* lin = new TF1("lin","[0]*x",0,g1->GetXaxis()->GetBinUpEdge(g1->GetXaxis()->GetLast()));
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,g1->GetXaxis()->GetBinUpEdge(g1->GetXaxis()->GetLast()));
	axisTitles(g1,"Beam Energy [GeV]","Measured Energy [GeV]");
	g1->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	double polylinear = poly->GetParameter(0);
	double polylinearError = poly->GetParError(0);
	float chi2 = poly->GetChisquare();
	g1->Fit(lin,"M0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	double linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	double ratiouncertainty = errorDivide(nonLinearFactor,nonLinearError,linearFactor,linearError);
	g1->SetMarkerStyle(kOpenCircle);
	g1->Draw("AP");
	g1->SetMarkerStyle(27);
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
	myText(.15,.86,kBlue,Form("Quad: E_{PbGl} = (%0.3f#pm %0.3f)*(E_{beam})^{2} + (%0.3f#pm %0.3f)*E_{beam}",nonLinearFactor,nonLinearError,polylinear,polylinearError),.04);
	myText(.15,.815,kBlue,Form("Quad: #chi^{2}/NDF: %0.2f",chi2/ndf),.04);
	myText(.15,.77,kRed,Form("Linear: E_{PbGl} = (%0.3f #pm %0.3f)*E_{beam}",linearFactor,linearError),.04);
	myText(.15,.725,kRed,Form("Linear: #chi^{2}: %0.2f",chi),.04);
	myText(.25,.725,kRed,Form("Linear: NDF:%i",ndf),.04);
	cout<<"Returning combined double"<<endl;
	return g1;
}

//combine, plot, and fit the points from two linearity graphs 
TGraphErrors* doubleFileAnalysis(TGraphErrors* i1, TGraphErrors *i2,bool runChi=false){
	TCanvas* tc = new TCanvas();
	TGraphErrors* g1= combineAllPoints(i1,i2);//probably memory leaks 
	//fit the linearity plot with a linear and quadratic fit  
	TF1* lin = new TF1("lin","[0]*x",0,g1->GetXaxis()->GetBinUpEdge(g1->GetXaxis()->GetLast()));
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,g1->GetXaxis()->GetBinUpEdge(g1->GetXaxis()->GetLast()));
	axisTitles(g1,"Beam Energy [GeV]","Measured Energy [GeV]");
	g1->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	double polylinear = poly->GetParameter(0);
	double polylinearError = poly->GetParError(0);
	float chi2 = poly->GetChisquare();
	int pndf = poly->GetNDF();
	g1->Fit(lin,"M0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	double linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int lndf = lin->GetNDF();
	g1->SetMarkerStyle(kOpenCircle);
	g1->Draw("AP");
	g1->SetMarkerStyle(27);
	poly->SetLineColor(kBlue);
	lin->Draw("same");
	//if the nonlinearity disagrees with 0 then display it 
	if (TMath::Abs(nonLinearFactor)-nonLinearError>0)
	{
		myText(.15,.86,kBlue,Form("Quad: E_{PbGl} = (%0.4f#pm %0.4f)*(E_{beam})^{2} + (%0.3f#pm %0.3f)*E_{beam}",nonLinearFactor,nonLinearError,polylinear,polylinearError),.04);
		myText(.15,.815,kBlue,Form("Quad: #chi^{2}/NDF: %0.2f",chi2/pndf),.04);
		poly->Draw("same");
	}
	myText(.15,.77,kRed,Form("E_{PbGl} = (%0.3f #pm %0.3f)*E_{beam}",linearFactor,linearError),.04);
	myText(.15,.725,kRed,Form("#chi^{2}: %0.2f",chi),.04);
	myText(.25,.725,kRed,Form("NDF: %i",lndf),.04);
	if (runChi)
	{
		chiAnalysis(g1,lin);
	}
	return g1;
}


TGraphErrors* resolution(TGraphErrors* ehist);
TGraphErrors* doubleFileAnalysisResolution(TGraphErrors* i1, TGraphErrors *i2){
	return resolution(combineAllPoints(i1,i2));
}

//combines two queues keeping them ordered by energy 
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
		sigmaError += pow(temp->front().sigma.uncertainty,2);//add sigma uncertainties in quadrature
		temp->pop();
	}
	tempdata.mean.value = tempdata.mean.value/meanD; //finish mean weighted average 
	tempdata.sigma.value = tempdata.sigma.value/sigmaD; //finish mean weighted average
	tempdata.mean.uncertainty = sqrt(meanError)/size; //set new uncertainties
	tempdata.sigma.uncertainty = sqrt(sigmaError)/size;
	return tempdata;
}

//combines all points within an energy 
Data combinePoint(queue<Data>* temp)
{
	Data tempdata;
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

//there are multiple points at each energy so I do a weighted avergae to make it look nice 
queue<Data>* combineAllPoints(queue<Data>* temp)
{
	//cout<<"Enter combineAll"<<'\n';
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
		//cout<<bigI<<"-"<<nextPoint<<'\n';
		//cout<<"Energy"<<working[bigI].energy<<endl;
		rdata->push(combinePoint(currentpoints));
		bigI=nextPoint;
		delete currentpoints;
	}
	delete temp;
	return rdata;
}

//there are multiple points at each energy so I do a weighted avergae to make it look nice 
int combineAllPoints(Data* working, int SIZE)
{
	//cout<<"Enter combineAll"<<'\n';
	const int kTotalPoints = SIZE;
	int bigI=0;
	queue<Data>* rdata = new queue<Data>();
	while(bigI < kTotalPoints)
	{
		int nextPoint=bigI;
		queue<Data>* currentpoints = new queue<Data>(); //queue to be combined
		while(nextPoint<=kTotalPoints&&working[nextPoint].energy==working[bigI].energy){
			currentpoints->push(working[nextPoint]);
			nextPoint++;
		} 
		//cout<<"Energy"<<working[bigI].energy<<'\n';
		//cout<<bigI<<"-"<<nextPoint<<'\n';
		rdata->push(combinePoint(currentpoints));
		bigI=nextPoint;
		delete currentpoints;
	}
	delete [] working;
	SIZE = rdata->size();
	working=queueToArray(*rdata);
	return SIZE;
}

//mostly deprecated I don't use the queues anymore 
TGraphErrors* resolution(queue<Data>* temp){
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
	eF->SetParLimits(0,0,1000);
	eF->SetParLimits(0,0,1000);
	TGraphErrors* ehist = new TGraphErrors(SIZE,energy,relativeE,nullptr,relativeU);
	ehist->Fit(eF,"MBI");
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
	return ehist;
}

//fits the stochastic function to the graph and plots it 
TGraphErrors* resolution(TGraphErrors* ehist){
	TCanvas *canvas1 = new TCanvas();
	//the stochastic funtion
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,
		ehist->GetXaxis()->GetBinUpEdge(ehist->GetXaxis()->GetLast()));
	eF->SetLineColor(kRed);
	eF->SetParLimits(0,0,1000);
	eF->SetParLimits(1,0,1000);
	ehist->Fit(eF,"MBI");
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	ehist->SetMarkerSize(1);
	ehist->SetMarkerStyle(21);
	ehist->SetMarkerColor(kRed);
	ehist->SetLineColor(kRed);
	ehist->GetXaxis()->SetLimits(0,ehist->GetXaxis()->GetBinUpEdge(ehist->GetXaxis()->GetLast())+1);
	gPad->SetTicks();
	ehist->Draw("AP");
	cout<<"here"<<endl;
	TF1* fit2016 = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,
		ehist->GetXaxis()->GetBinUpEdge(ehist->GetXaxis()->GetLast()));
	fit2016->FixParameter(0,.05);
	fit2016->FixParameter(1,.0244);
	fit2016->SetLineColor(kBlue);
	fit2016->Draw("same");
	ehist->SetTitle(";Beam Energy GeV;#sigma/mean");
	const int kN2016=10;
	double y2016[] = {.0556,.0448,.0395,.034,.03,.0318,.0308,.0297,.0301,.02567};
	double ye2016[]={.003,.0025,.0012,.002,.0015,.0014,.0008,.001,.0004,.001};
	double x2016[]={1,2,3,4,6,6,8,8,12,16};
	TGraphErrors *points2016 = new TGraphErrors(kN2016,x2016,y2016,nullptr,ye2016);
	points2016->SetMarkerColor(kBlue);
	points2016->SetLineColor(kBlue);
	points2016->SetMarkerSize(1);
	points2016->SetMarkerStyle(21);
	points2016->Draw("Psame");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
	myText(.3,.75,kRed,Form("#chi^{2}:%0.2f",chi),.05);
	myText(.4,.75,kRed,Form("NDF:%i",ndf),.05);
	myText(.24,.85,kRed,Form("Stochastic: %0.3f#pm%0.3f ",eA,errors[0]),.05);
	myText(.24,.8,kRed,Form("Constant: %0.3f#pm%0.3f",eB,errors[1]),.05);
	TLegend *tl = new TLegend(.75,.75,.95,.95);
	tl->AddEntry(fit2016,"2016","l");
	tl->AddEntry(eF,"2018","l");
	tl->Draw();
	return ehist;
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

//makes a plot of how far each point is from a fit in chi2
void chiAnalysis(TGraphErrors* graph, TF1* fit){
	const int NDF = fit->GetNDF();
	TCanvas *canvas1 = new TCanvas();
	const int SIZE = graph->GetN();
	double *gx = graph->GetX();
	double *gy = graph->GetY();
	double* gye = graph->GetEY();
	//use the same x values and recalculate the y to be chi2 distance 
	double y[SIZE]; 
	double yu[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		//calculating distance
		Scalar point(gy[i],gye[i]);
		Scalar residual = (point - ((float)fit->Eval(gx[i])))*(point - ((float)fit->Eval(gx[i])));
		residual/=(gye[i])*(gye[i]);
		residual/=(float)NDF;
		y[i] = residual.value;
		yu[i] = residual.uncertainty;
	}
	TGraphErrors* p_mean = new TGraphErrors(SIZE,graph->GetX(),y,graph->GetEX(),yu);
	p_mean->Draw("AP");
	p_mean->SetMarkerStyle(27);
	p_mean->SetTitle("Fit Comparison;point x;#Chi^{2}/NDF contribution");
	gPad->SetTickx();
	gPad->SetTicky();
}

//makes a plot of how far each point is from a fit in chi2
void chiAnalysis(TGraphErrors* graph, TF1* fit, float* runNum=nullptr){
	//see function above for additional comments 
	const int NDF = fit->GetNDF();
	TCanvas *canvas1 = new TCanvas();
	const int SIZE = graph->GetN();
	double *gx = graph->GetX();
	double *gy = graph->GetY();
	double* gye = graph->GetEY();
	double y[SIZE];
	double yu[SIZE];
	double labely[SIZE];
	double labelx[SIZE];
	string *labels = new string[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		Scalar point(gy[i],gye[i]);
		Scalar residual = (point - ((float)fit->Eval(gx[i])));//*(point - ((float)fit->Eval(gx[i])));
		residual/=(gye[i]);
		residual/=TMath::Sqrt((float)NDF);
		y[i] = residual.value;
		yu[i] = residual.uncertainty;
	}
	TGraphErrors* p_mean = new TGraphErrors(SIZE,graph->GetX(),y,graph->GetEX(),yu);
	gPad->SetTickx();
	gPad->SetTicky();
	p_mean->Draw("AP");
	p_mean->SetMarkerStyle(27);
	//adds labels to the points 
	for (int i = 0; i < SIZE; ++i)
	{
		labely[i] = .8*TMath::Abs(y[i])/14+.1;
		labelx[i] = .8*gx[i]/9+.075;
		if(runNum!=nullptr)labels[i] = to_string((int)runNum[i]);
		myText(labelx[i],labely[i],kBlue,labels[i].c_str(),.04);
	}
}

void Part2B(){
	pair<TGraphErrors*,TGraphErrors*> lin1 =singlefileConverter("PbGl1100.txt");
	pair<TGraphErrors*,TGraphErrors*> lin1new =singlefileConverter("PbGl1100new.txt");
	pair<TGraphErrors*,TGraphErrors*> lin2 =singlefileConverter("PbGl1200.txt");
	pair<TGraphErrors*,TGraphErrors*> lin2new =singlefileConverter("PbGl1200new.txt",false);
	//pair<TGraphErrors*,TGraphErrors*> lin0 =singlefileConverter("PbGl1000.txt");
	pair<TGraphErrors*,TGraphErrors*> lin0new =singlefileConverter("PbGl1000new.txt");	
	//doubleFileAnalysis(lin0.first,lin0new.first);
	doubleFileAnalysis(doubleFileAnalysis(lin2.first,lin2new.first),doubleFileAnalysis(doubleFileAnalysis(lin1.first,lin1new.first),lin0new.first));
	doubleFileAnalysisResolution(doubleFileAnalysisResolution(lin1new.second,lin1.second),doubleFileAnalysisResolution(doubleFileAnalysisResolution(lin2new.second,lin2.second),lin0new.second));
	//doubleFileAnalysis(lin1.first,lin2.first);
	//doubleFileAnalysisResolution(lin1.second,lin2.second);
	//singlefileConverter("PbGl1200new.txt",false,false);
}
