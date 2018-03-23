

//Here are a bunch of 1200 V PbGl runs: (edited)
//sphenix/user/dvp/testbeam/beam_00000558-0000.root (6 GeV Beam, 1200V) Tested - Chase/Franceso
//sphenix/user/dvp/testbeam/beam_00000551-0000.root (4 GeV beam),    Tested - Chase    (Imported)
//sphenix/user/dvp/testbeam/beam_00000563-0000.root (2 GeV beam),    Tested - Chase	   (Imported)
//sphenix/user/dvp/testbeam/beam_00000573-0000.root (12 GeV beam),   Tested - Fracesco (Imported)
//sphenix/user/dvp/testbeam/beam_00000567-0000.root (8 Gev Beam),    Tested - Chase    (Imported)

//This is a 1100V Run
//sphenix/user/dvp/testbeam/beam_00000544-0000.root (8 GeV beam),    Tested - Chase	   (Imported)


using namespace std;
#include "TLegend.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>
#include <string>


/////////////////////Franceso's included functions (for asthetic) //////////////////////////////////////////////
short colors[7]={kRed,kBlue,kGreen+2,kMagenta+3,kOrange+4,kCyan+1,kMagenta-7};
short styles[7]={kFullCircle,kOpenSquare,kFullTriangleUp,kFullDiamond,kFullCross,kFullStar,kOpenFourTrianglesX};
void makeBins(float* bins, int min, int nBins, float width){
	for(int i=0; i<=nBins;++i){
		bins[i] = min + width*i;
	}
}

void makeMarkerNice(TH1F** h, int n){
	for (int i = 1; i < n; ++i)
	{
		(*h)->SetMarkerStyle(styles[i-1]);
		(*h)->SetMarkerColor(colors[i-1]);
		h++;
	}
}
void makeLineColors(TH1F** h, int n){
	for (int i = 1; i < n; ++i)
	{
		(*h)->SetLineColor(colors[i-1]);
		h++;
	}
}
void makeLegendPoint(TLegend* tl, TH1F** h, int n, std::string *titles){
	for (int i = 0; i < n; ++i)
	{
		tl->AddEntry((*h++),titles++->c_str(),"p");
	}
}
void makeLegendLine(TLegend* tl, TH1F** h, int n, std::string *titles){
	for (int i = 0; i < n; ++i)
	{
		tl->AddEntry((*h++),titles++->c_str(),"l");
	}
}
void makeNiceHist(TH1* h){
	h->SetMarkerStyle(kFullCircle);
}
void axisTitles(TH1F* h,std::string x, std::string y){
	h->GetYaxis()->SetTitle(y.c_str());
	h->GetXaxis()->SetTitle(x.c_str());
}
void smallBorders(){
	gPad->SetBottomMargin(.1);
	gPad->SetTopMargin(.1);
}
void axisTitleSize(TH1F* h,float s){
	h->GetYaxis()->SetTitleSize(s);
	h->GetXaxis()->SetTitleSize(s);
}
void axisLabelSize(TH1F* h,float s){
	h->GetYaxis()->SetLabelSize(s);
	h->GetXaxis()->SetLabelSize(s);
}
void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text,Float_t msize,Double_t tsize)
{
	//  Double_t tsize=0.032;
	TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
	marker->SetMarkerColor(color);  marker->SetNDC();
	marker->SetMarkerStyle(mstyle);
	marker->SetMarkerSize(msize);
	marker->Draw();

	TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
	l.SetNDC();
	l.DrawLatex(x,y,text);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void recursiveGaus(TH1F* h, TF1* gaus, float* data, int lazyMan){
    h->Fit(gaus,"","",data[0]-2*data[1],data[0]+2*data[1]);
    if(data[0]!=gaus->GetParameter(1)){
    	if(lazyMan == 100){return;}
        data[0] = gaus->GetParameter(1);
        data[1] = gaus->GetParameter(2);
        lazyMan++;
        recursiveGaus(h,gaus,data,lazyMan);
    }
    else{
        data[0] = gaus->GetParameter(1);
        data[1] = gaus->GetParameter(2);
        return;
    }
}

///////////Franceso's class to pull the appropriate data from the tree (what a champ) //////////////////////////
class DiffADC
{
public:
	DiffADC(float* peak, float* pedestal){ 
		this->peak = peak;
		this->pedestal = pedestal;
	}
	~DiffADC(){
		
	}
	int pmp(int i){
		return peak[i] - pedestal[i];
	}
	bool pass(){
		return pmp(172)<100&&pmp(173)<100&&pmp(174)<150&&pmp(175)<200&&pmp(160)>1000&&pmp(171)>100;
	}
	float getGoodPbGl(){
		if(pass()){
			return pmp(171);
		}
		else{
			return -1;
		}
	}
	float getCerenkov(){
		return pmp(160);
	}
	float getPbGl(){
		return pmp(171);
	}
	/*veto #0 is 172*/
	float getVeto(int i){
		return pmp(veto0+i);
	}
	float gethHodo(int i){
		return pmp(hHodo0+i);
	}
	float getvHodo(int i){
		return pmp(vHodo0+i);
	}
private:
	float* peak;
	float* pedestal;
	const int veto0 = 172;
	const int hHodo0 = 104;
	const int vHodo0 = 96;
	
};


///////////Analysis of delta ADC peak-pedestal data, Note that I changed this to work with the function below//////////////
void BeamAnalysis(TTree *maintree, float* mygaus2){
	//TFile *input = new TFile("beam_00000567-0000.root"); 
	//TTree *maintree = (TTree*) input->Get("W");

	const int SIZE = 192;
	Long64_t LENGTH = maintree->GetEntries();
	const int VETOSIZE =4;
	const int HODOSIZE = 8;
	float mypeak[SIZE];
	float mypedestal[SIZE];
	int runnumber[SIZE];
	maintree->SetBranchAddress("peak",&mypeak);
	maintree->SetBranchAddress("pedestal",&mypedestal);
	maintree->SetBranchAddress("runnumber",&runnumber);
	TH1F* ht = new TH1F("temp","",500,200,10000);
	queue<float> veto[4];
	queue<float> hHodo[8];
	queue<float> vHodo[8];
	queue<float> cerenkov;
	queue<float> goodPbGl;
	float tempglass;
	for (Long64_t i = 0; i < LENGTH; ++i)
	{
		maintree->GetEntry(i);
		DiffADC myPs(&mypeak[0],&mypedestal[0]);
		/* get the veto only use this segment if you need to know more than the PbGl 
		From CHASE - For some reason the code would not run with this commented out */

		for (int j = 0; j < VETOSIZE; ++j)
		{
			veto[j].push(myPs.getVeto(j));
		}
		for (int j = 0; j < HODOSIZE; ++j)
		{
			hHodo[j].push(myPs.gethHodo(j));
			vHodo[j].push(myPs.getvHodo(j));
		}
		cerenkov.push(myPs.getCerenkov());
		tempglass = myPs.getGoodPbGl();
		
		if(tempglass>0){
			goodPbGl.push(tempglass);
			ht->Fill(tempglass);
		}
	}
	ht->Sumw2();
	TCanvas *tc = new TCanvas();
	gStyle->SetOptStat(0);
	TLegend *tl = new TLegend(.15,.7,.4,.85);
	tl->AddEntry(ht,"PbGl 8 Gev Beam","l");

	makeNiceHist(ht);
	axisTitles(ht,"#Delta ADC","Count");
	axisTitleSize(ht,.03);
	axisLabelSize(ht,.03);
	smallBorders();

	int maxbin = ht->GetMaximumBin();
	float gausLowBound = ht->GetBinLowEdge(maxbin);
	float temp = gausLowBound*.7;
	float gausUpbound = gausLowBound +temp;
	gausLowBound -= temp; 
	TF1* gaus = new TF1("gaus","gaus",gausLowBound,gausUpbound);

	gaus->SetLineColor(kRed);
	ht->Fit(gaus,"R");       //“R” Use the range specified in the function range
	float mygaus[2];
	mygaus[0] = gaus->GetParameter(1); //mean
	mygaus[1] = gaus->GetParameter(2); //sigma

	int lazyMan = 0;
	recursiveGaus(ht, gaus, mygaus, lazyMan);

	ht->GetXaxis()->SetRangeUser(mygaus[0]-(5*mygaus[1]),mygaus[0]+5*mygaus[1]);\

	tl->AddEntry((TObject*)0,Form("Mean: %0.2f", mygaus[0]),"l");
	tl->AddEntry((TObject*)0,Form("Sigma: %0.2f", mygaus[1]),"l");

	ht->Draw("p");
	tl->Draw();


	mygaus[0] = gaus->GetParameter(1); //mean
	mygaus[1] = gaus->GetParameter(2); //sigma
	mygaus2[0] = mygaus[0];
	mygaus2[1] = mygaus2[1];
	mygaus2[2] = runnumber[0];
}


/////////////////////Implementing BeamAnalysis on an arbitrary number of root files///////////////////////////
void BeamAnalysis1()
{
	int runcount = 6;
	int counter = 0;
	ifstream inFile ("PbGl_Runs.txt"); //txt file containing the beam files

	string someRuns[runcount];

	while(inFile>>someRuns[counter])
	{
		cout<<"Run: "<<someRuns[counter]<<endl;
		counter++;
	}
	inFile.close();

	float mygaus2[3]; //temp array to contain sigma and mean
	int whichRuns[runcount-1]; //array of run rumbers
	float allmeans[runcount-1]; //array of means
	float allsigma[runcount-1]; //array of sigma 

	for(int i = 1; i < runcount; i++)
	{
		TFile *input = new TFile(someRuns[i].c_str()); //read in tree so it can be passed to BeamAnalysis1
		TTree *maintree = (TTree*) input->Get("W");

		BeamAnalysis(maintree, mygaus2); //BeamAnalysis modifies array to contain run number, mean, and sigma
		allmeans[i] = mygaus2[0];
		allsigma[i] = mygaus2[1];
		whichRuns[i] = mygaus2[2];
	}

	ofstream outFile;
	outFile.open("PbGl_data1.txt");
	if(outFile.is_open()) //read info out to txt file if it opens
	{
		cout<<"File Opened!"<<endl;

		outFile << "RunNumber";
		for(int i = 1; i < runcount; i++) //enter all run numbers into txt file
		{
			outFile << ","<< whichRuns[i];
		}
		outFile << "\n";
		outFile << "Mean";
		for(int i = 1; i < runcount; i++) //enter all mean values into txt file
		{
			outFile << ","<< allmeans[i];
		}
		outFile << "\n";
		outFile << "Sigma";
		for(int i = 1; i < runcount; i++) //enter all signma values into txt file
		{
			outFile << ","<< allsigma[i];
		}
		outFile << "\n";

		outFile.close();
	}
	else {cout<<"RED ALERT! RED ALERT! FAILED TO WRITE TO A TEXT FILE! I REPEAT! RED ALERT!"<<endl;}

}






