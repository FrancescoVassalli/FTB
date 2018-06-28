// Made by Francesco Vassalli for analysis of PbGl detector for sPHENIX EMCal calibration effort
// 6/27/18 at the CU Boulder

/*#include "/Users/Chase/Documents/HeavyIonsResearch/FranTools/Bin/NiceHists.C" //for chase
void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {
  	TLatex l; //l.SetTextAlign(12); 
  	l.SetTextSize(tsize); 
  	l.SetNDC();
 	l.SetTextColor(color);
  	l.DrawLatex(x,y,text);
}	*/

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

struct Data
{
	Scalar mean;
	Scalar sigma;
	int energy;
};

queue<Data>* singlefileAnalysis(string filename){
	ifstream inFile (filename.c_str()); //txt file containing the data from Part2A
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
	inFile.close();
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
		cout<<"work"<<'\n';
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
	cout<<"Exit Combine"<<endl;
	delete d1;
	delete d2;
	return rdata;
}

void Part2B(){
	combinedplot(sortcombine(singlefileAnalysis("A1200.txt"),singlefileAnalysis("A1100.txt")));
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