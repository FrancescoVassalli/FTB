
using namespace std;

void adcToEnergy(float linearFactor, float linearFactorError, int SIZE, float* means, float* inputEnergy, float* sigma){
	errorDivideArray(SIZE,means,sigma,linearFactor,linearFactorError);
	TCanvas *tc = new TCanvas();
	float ex[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		ex[i] = 0;
	}
	TGraphErrors *measure = new TGraphErrors(SIZE,inputEnergy,means,ex,sigma);
	axisTitles(measure,"Beam Energy GeV","Measured Energy");
	TF1* lin = new TF1("lin","[0]*x",0,12);
	measure->Fit(lin);
	gNice();
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	lin->SetLineColor(kRed);
	measure->SetMarkerStyle(kOpenCircle);
	doubleZero(measure,means[SIZE-1]+2,inputEnergy[SIZE-1]+1);
	measure->Draw("AP");
	measure->GetXaxis()->SetLimits(0,inputEnergy[SIZE-1]+1);
	myText(.5,.22,kRed,Form("#chi^{2}/NDF: %0.2f",chi/ndf),.05);
}


void BeamAnalysis4(){
	ifstream inFile ("PbGl_data2.txt"); //txt file containing the data from BeamAnalysis2
	const int LINES =5;
	queue<float> input[LINES];
	string intemp;
	stringstream ss;
	inFile>>intemp;
	ss<<intemp;
	float linearFactor;
	getline(ss,intemp,',');
	//cout<<intemp<<endl;
	linearFactor= stof(intemp);
	float linearFactorError;
	getline(ss,intemp,',');
	//cout<<intemp<<endl;
	linearFactorError = stof(intemp);
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
	adcToEnergy(linearFactor, linearFactorError, input[1].size(),queueToArray(input[1]),queueToArray(input[0]),queueToArray(input[3]));
	
}