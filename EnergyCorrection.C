
struct Data
{
	Scalar mean;
	Scalar sigma;
	int energy;
};

namespace {
	const int kDOMAINMAX=17;
	const int kANABELMAX=25;
}

Scalar linearity(const int kSIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror){
	TGraphErrors* p_mean = new TGraphErrors(kSIZE,energy,mean,zeroArray(kSIZE),meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[kSIZE-1]);
	p_mean->Fit(lin,"0");
	return Scalar(lin->GetParameter(0),lin->GetParError(0));
}
TGraphErrors* makePlots(queue<Data> data, TGraphErrors* lin, TGraphErrors* quad){
	const int kSIZE = data.size();
	float energy[kSIZE];
	float mean[kSIZE];
	float meanerror[kSIZE];
	int i=0;
	while(!data.empty()){
		energy[i] = data.front().energy;
		mean[i] = data.front().mean.value;
		meanerror[i] = data.front().mean.uncertainty;
		data.pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(kSIZE,energy,mean,zeroArray(kSIZE),meanerror); // how to set the uncertainty
	TF1* linFit = new TF1("lin","[0]*x",0,energy[kSIZE-1]);
	TF1* quadraticFit = new TF1("poly","[1]*x*x+[0]*x",0,energy[kSIZE-1]);
	p_mean->Fit(linFit,"0");
	lin->SetPoint(0,0,0);
	quad->SetPoint(0,0,0);
	for (int i = 0; i < kANABELMAX; ++i)
	{
		lin->SetPoint(i,i+1,0);
		quad->SetPoint(i,i+1,0);
	}
	(TVirtualFitter::GetFitter())->GetConfidenceIntervals(lin);
	p_mean->Fit(quadraticFit,"0");
	(TVirtualFitter::GetFitter())->GetConfidenceIntervals(quad);
	Scalar ltemp,qtemp;
	double screwthis;
	double s;
	for (int i = 0; i < kANABELMAX; ++i)
	{
		lin->GetPoint(i,screwthis,s);
		ltemp.value = (float) s ;
		ltemp.uncertainty=lin->GetErrorY(i);
		quad->GetPoint(i,screwthis,s);
		qtemp.value = (float) s ;
		qtemp.uncertainty=quad->GetErrorY(i);
		ltemp/=(float) (i+1);
		qtemp/=(float) (i+1);
		lin->SetPoint(i,i+1,ltemp.value);
		lin->SetPointError(i,0,ltemp.uncertainty);
		quad->SetPoint(i,i+1,qtemp.value);
		quad->SetPointError(i,0,qtemp.uncertainty);
	}
	return p_mean;

}
void linearity(queue<Data> data){
	const int kSIZE = data.size();
	float energy[kSIZE];
	float mean[kSIZE];
	float meanerror[kSIZE];
	int i=0;
	while(!data.empty()){
		energy[i] = data.front().energy;
		mean[i] = data.front().mean.value;
		meanerror[i] = data.front().mean.uncertainty;
		data.pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(kSIZE,energy,mean,zeroArray(kSIZE),meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[kSIZE-1]);
	p_mean->Fit(lin,"0");
}
Poly2 quadratic(queue<Data> data){
	const int kSIZE = data.size();
	float energy[kSIZE];
	float mean[kSIZE];
	float meanerror[kSIZE];
	int i=0;
	while(!data.empty()){
		energy[i] = data.front().energy;
		mean[i] = data.front().mean.value;
		meanerror[i] = data.front().mean.uncertainty;
		data.pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(kSIZE,energy,mean,nullptr,meanerror); // how to set the uncertainty
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energy[kSIZE-1]);
	p_mean->Fit(poly,"0");
	return Poly2(Scalar(0.),Scalar(poly->GetParameter(0),poly->GetParError(0)),Scalar(poly->GetParameter(1),poly->GetParError(1)));
}

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
	Scalar slope = linearity(input[5].size(),queueToArray(input[5]),queueToArray(input[1]),queueToArray(input[2]),queueToArray(input[3]),queueToArray(input[4]));
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

queue<Data>* sortcombine(queue<Data>* d1, queue<Data>* d2){
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
	queue<Data>* newData = new queue<Data>();
	queue<Data>* currentpoints = new queue<Data>(); //queue for 'to be combined' particles
	Data tempdata;
	for (int i = 0; i < 7; ++i)
	{
		tempdata.mean = temp->front().mean;
		tempdata.sigma= temp->front().sigma;
		tempdata.energy = temp->front().energy;
		currentpoints->push(temp->front()); //put first point in queue where 'to be combined' particles are stored
		cout<<"Energy"<<temp->front().energy<<endl;
		temp->pop();
		for (int j = 0; j < temp->size(); ++j)
		{
			if(temp->front().energy == tempdata.energy)
			{
				currentpoints->push(temp->front()); //put next point in queue where 'to be combined' particles are stored
				temp->pop();
			}
			else{break;} //break if no more same energy points, they are sorted so this will work correctly
		}
		newData->push(combinePoint(currentpoints));
		while(!currentpoints->empty())
		{
			currentpoints->pop();
		}
	}
	return newData;
}

void plotCorrection(queue<Data> *data,Scalar m){
	TCanvas *tc = new TCanvas();
	const int kSIZE = data->size();
	float x[kSIZE];
	float y[kSIZE];
	float yerror[kSIZE];
	float xerror[kSIZE];
	int i=0;
	TLine *trend = new TLine(0,m.value,kDOMAINMAX,m.value);
	TBox *band = new TBox(0,m.value-m.uncertainty,kDOMAINMAX,m.value+m.uncertainty);
	band->SetFillColorAlpha(kBlue,.40);
	band->SetLineColor(kBlue);
	trend->SetLineColor(kRed);
	while(!data->empty()){
		y[i] = (data->front().mean/Scalar(data->front().energy)).value;
		x[i] = data->front().energy;
		yerror[i] = (data->front().mean/Scalar(data->front().energy)).uncertainty;
		xerror[i] = 0;//data->front().mean.uncertainty;
		data->pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(i,x,y,xerror,yerror);
	p_mean->Draw();
	band->Draw("same");
	trend->Draw("same");
	tc->Print("plot1.pdf");
}
void plotCorrection(queue<Data>* data, TGraphErrors *lin, TGraphErrors *quad){
	const int kSIZE = data->size();
	float x[kSIZE];
	float y[kSIZE];
	float yerror[kSIZE];
	float xerror[kSIZE];
	int i=0;
	while(!data->empty()){
		y[i] = (data->front().mean/Scalar(data->front().energy)).value;
		x[i] = data->front().energy;
		yerror[i] = (data->front().mean/Scalar(data->front().energy)).uncertainty;
		xerror[i] = 0;//data->front().mean.uncertainty;
		data->pop();
		i++;
	}
	TGraphErrors *p_data= new TGraphErrors(i,x,y,xerror,yerror);
	TGraphErrors *base = new TGraphErrors(kANABELMAX);
	TCanvas *tc = new TCanvas();
	lin->SetFillColorAlpha(kBlue,.4);
	lin->SetLineColor(kBlue);
	quad->SetLineColor(kPink+3);
	quad->SetFillColorAlpha(kPink+3,.4);
	axisTitles(quad,"Beam Energy","Measured E/Beam E");

	quad->Draw("ACE3");
	lin->Draw("same CE3");
	p_data->Draw("same P");

	//tc->Print("plot1.pdf");
}
void plotCorrection(queue<Data> *data){
	TCanvas *tc = new TCanvas();
	const int kSIZE = data->size();
	float x[kSIZE];
	float y[kSIZE];
	float yerror[kSIZE];
	float xerror[kSIZE];
	int i=0;
	while(!data->empty()){
		y[i] = (data->front().mean/Scalar(data->front().energy)).value;
		x[i] = data->front().energy;
		yerror[i] = (data->front().mean/Scalar(data->front().energy)).uncertainty;
		xerror[i] = 0;//data->front().mean.uncertainty;
		data->pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(kSIZE,x,y,xerror,yerror); // how to set the uncertainty
	p_mean->Draw();
}

void EnergyCorrection(){
	queue<Data> *data = combineAllPoints(sortcombine(singlefileAnalysis("PbGlA12004x4.txt"),singlefileAnalysis("PbGlA11004x4.txt")));
	TGraphErrors *lin = new TGraphErrors(kANABELMAX);
	TGraphErrors *quad = new TGraphErrors(kANABELMAX);
	makePlots(*data,lin,quad);
	//lin->Draw();
	plotCorrection(data,lin,quad);

}