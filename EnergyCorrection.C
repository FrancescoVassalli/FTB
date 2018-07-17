
struct Data
{
	Scalar mean;
	Scalar sigma;
	int energy;
};

Scalar linearity(const int SIZE,float*energy, float* mean, float* sigma, float* meanerror, float* sigmaerror){
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,zeroArray(SIZE,ex),meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	p_mean->Fit(lin,"0");
	return Scalar(lin->GetParameter(0),lin->GetParError(0));
}
Scalar linearity(queue<Data>* data){
	const int SIZE = data->size();
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
	TGraphErrors* p_mean = new TGraphErrors(SIZE,energy,mean,zeroArray(SIZE,ex),meanerror); // how to set the uncertainty
	TF1* lin = new TF1("lin","[0]*x",0,energy[SIZE-1]);
	p_mean->Fit(lin,"0");
	return Scalar(lin->GetParameter(0),lin->GetParError(0));
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

void plotCorrection(queue<Data> *data){
	const int SIZE = data->size();
	float x[SIZE];
	float y[SIZE];
	float yerror[SIZE];
	float xerror[SIZE];
	int i=0;
	while(!data->empty()){
		y[i] = (Scalar(data->front().energy)/data->front().mean).value;
		x[i] = data->front().mean.value;
		yerror[i] = (Scalar(data->front().energy)/data->front().mean).uncertainty;
		xerror[i] = data->front().mean.uncertainty;
		data->pop();
		i++;
	}
	TGraphErrors* p_mean = new TGraphErrors(SIZE,x,y,xerror,yerror); // how to set the uncertainty
	p_mean->Draw();
}

void EnergyCorrection(){
	queue<Data> *data = sortcombine(singlefileAnalysis("PbGlA12004x4.txt"),singlefileAnalysis("PbGlA11004x4.txt"));
	plotCorrection(data);

}