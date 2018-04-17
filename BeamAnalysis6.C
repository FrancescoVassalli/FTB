#include "Frannamespace.C"
#include "TLine.h"

using namespace std;
//using namespace Frannamespace;


void superArraySorter4000(float* energies, float* mean, float* meanError, int SIZE)
{
	for (int i = 0; i < SIZE-1; i++) 
	{  
	   for (int j = 0; j < SIZE-i-1; j++) 
	   {
	       if (energies[j] > energies[j+1])
	       {
	          oleSwitcheroo(&energies[j],&energies[j+1]);
	          oleSwitcheroo(&mean[j],&mean[j+1]);
	          oleSwitcheroo(&meanError[j],&meanError[j+1]);
	         
	       }
	   }
	}
}

queue<TBox*>* getSystematicBoxes(int SIZE,float* means, float* meanerror, float* inputEnergy){
	int fghfker=0;
	superArraySorter4000(inputEnergy,means,meanerror,SIZE);
	queue<queue<float>> groups = breakArray(means,*sameValueIndices(SIZE, inputEnergy));
	queue<float> averagevalues = averageList(groups);
	const unsigned int nGroups = groups.size();
	float *groupX = valuesAt(inputEnergy,*sameValueIndices(SIZE,inputEnergy));
	float systematics[nGroups];
	cout<<"Systematics: "<<'\n';
	while(!groups.empty()){
		systematics[fghfker] = systematicError<float>(groups.front());
		cout<<systematics[fghfker++]<<'\n';
		groups.pop();
	}
	queue<int> goodBoxes = arrayNonZero(systematics,nGroups);
	TBox **boxes = new TBox*[nGroups];
	fghfker=0;
	while(!averagevalues.empty()){
		cout<<"X: "<<groupX[fghfker]<<'\n';
		boxes[fghfker] = new TBox(groupX[fghfker]-.5,averagevalues.front()-systematics[fghfker],groupX[fghfker]+.5,averagevalues.front()+systematics[fghfker]);
		averagevalues.pop();
		boxes[fghfker]->SetLineColor(kAzure+3);
		boxes[fghfker]->SetFillColor(kAzure+3);
		boxes[fghfker]->SetFillStyle(1);
		fghfker++;
	}
	queue<TBox*> *r = new queue<TBox*>();
	//cout<<"Length: "<<nGroups<<'\n';
	while(!goodBoxes.empty()){
		//cout<<goodBoxes.front()<<'\n';
		r->push(boxes[goodBoxes.front()]);
		goodBoxes.pop();
	}
	return r;
}

void plotByEnergy(int SIZE, float* means, float* meanerror, float* inputEnergy,const unsigned int nFiles){
	TCanvas *tc = new TCanvas();
	float ex[SIZE];
	int peakInput=0;
	int tempenergy=inputEnergy[0];
	int fileBeginIndex[nFiles+1];
	int fileBeginIndexCounter=1;
	fileBeginIndex[0]=0;
	for (int i = 0; i < SIZE; ++i)
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
	fileBeginIndex[fileBeginIndexCounter]=SIZE;
	peakInput++;
	TGraphErrors *measure = new TGraphErrors(SIZE,inputEnergy,means,ex,meanerror);
	axisTitles(measure,"Beam Energy (GeV)","Measured Energy(GeV)");
	TF1* lin = new TF1("lin","[0]*x",0,peakInput);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,peakInput);
	TF1* old = new TF1("lin"," x*(1-0.00058)",0,peakInput);
	measure->Fit(poly);
	old->SetLineColor(kBlue);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	measure->Fit(lin,"0");
	gNice();
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	lin->SetLineColor(kRed);
	measure->SetMarkerStyle(kOpenCircle);
	
	TGraphErrors** plotgraphs = new TGraphErrors*[nFiles];
	for (unsigned i = 0; i < nFiles; ++i)
	{
		plotgraphs[i] = new TGraphErrors(fileBeginIndex[i+1]-fileBeginIndex[i],partialArray(inputEnergy,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(means,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(ex,fileBeginIndex[i],fileBeginIndex[i+1]),partialArray(meanerror,fileBeginIndex[i],fileBeginIndex[i+1]));
	}
	doubleZero(plotgraphs[0],peakInput,peakInput);
	plotgraphs[0]->SetMarkerSize(2);
	plotgraphs[0]->SetMarkerStyle(kOpenCircle);
	plotgraphs[0]->Draw("AP");
	plotgraphs[1]->SetMarkerStyle(kOpenTriangleDown);
	axisTitles(plotgraphs[0],"Beam Energy (GeV)","Measured Energy #pm #sigma/#sqrt{3}  (GeV)");
	for (unsigned i = 1; i < nFiles; ++i)
	{
		plotgraphs[i]->SetMarkerSize(2);
		plotgraphs[i]->Draw("P");
	}
	poly->SetLineColor(kPink-7);
	//poly->Draw("same");        //quadratic fit removed
	old->Draw("same");
	lin->Draw("same");
	queue<TBox*>*boxes = getSystematicBoxes(SIZE, means, meanerror, inputEnergy);
	cout<<&boxes->front()<<endl;
	while (!boxes->empty())
	{
		boxes->front()->Draw("same");
		boxes->pop();
	}
	plotgraphs[0]->GetXaxis()->SetLimits(0,peakInput);
	//myText(.5,.30,kRed,Form("Linear #chi^{2}/NDF: %0.2f",chi/ndf),.05,0);
	//myText(.5,.24,kRed,Form("C2: %0.3f#pm %0.5f",nonLinearFactor,nonLinearError),.05,0);
	//myText(.5,.24,kPink-7,Form("Quad #chi^{2}/NDF: %0.2f",chi2/ndf),.05,0); //quadratic chi^2 removed

	myText(.25,.73,kBlack,"2016",.05,0);
	TF1 *oldData = new TF1("oldData","13.75",2,3); //key line for the old 2016 data 
	oldData->SetLineColor(kBlue);
	oldData->Draw("same");

	myText(.25,.68,kBlack,"2018",.05,0);
	//cout<<"slopey shit"<<fabs(1-(lin->GetParameter(0)))<<endl;
	myText(.5,.30,kBlack,"E_{measured} = E_{in}*1.0205013 ",.05,0);
	TF1 *newData = new TF1("newData","12.75",2,3); //key line for the our data 
	newData->SetLineColor(kRed);
	newData->Draw("same");

	myMarkerText(.25,.85,kBlack,kOpenTriangleDown,"1100V",2,.05);
	myMarkerText(.25,.8,kBlack,kOpenCircle,"1200V",2,.05);
}

queue<float>* adcToEnergy(float linearFactor, float linearFactorError, int SIZE, float* means, float* inputEnergy, float* meanerror){
	errorDivideArray(SIZE,means,meanerror,linearFactor,linearFactorError);
	queue<float> *r = new queue<float>[3];
	for (int i = 0; i < SIZE; ++i)
	{
		r[0].push(means[i]);
		r[1].push(meanerror[i]);
		r[2].push(inputEnergy[i]);
	}
	return r;
}


void BeamAnalysis6(){
	ifstream inFile ("PbGl_dataC.txt"); //txt file containing the names of files to process
	queue<string> files;
	string intemp;
	while(getline(inFile,intemp)){
		files.push(intemp);
	}
	inFile.close();
	const unsigned int nFiles = files.size();
	const int LINES =5;
	float linearFactor[files.size()];
	float linearFactorError[files.size()];
	int count=0;
	queue<float> totalinput[3];
	while(!files.empty()){
		queue<float> input[LINES];
		queue<float> *tempenergy;
		inFile.open(files.front().c_str());
		files.pop();
		stringstream ss;
		intemp="";
		inFile>>intemp;
		//cout<<intemp<<std::endl;
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
				/*if(i==2)
					cout<<intemp<<endl;*/
			}
			ss.clear();
		}
		inFile.close();
		tempenergy=adcToEnergy(linearFactor[count], linearFactorError[count], input[1].size(),queueToArray(input[1]),queueToArray(input[0]),queueToArray(input[2]));
		while(!tempenergy[0].empty()){
			totalinput[0].push(tempenergy[0].front());
			totalinput[1].push(tempenergy[1].front());
			totalinput[2].push(tempenergy[2].front());
			tempenergy[0].pop();
			tempenergy[1].pop();
			tempenergy[2].pop();
		}
		count++;
	}
	plotByEnergy(totalinput[0].size(),queueToArray(totalinput[0]),queueToArray(totalinput[1]),queueToArray(totalinput[2]),nFiles);
}