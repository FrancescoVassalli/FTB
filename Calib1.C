//Here are a bunch of 1200 V PbGl runs: (edited)
//sphenix/user/dvp/testbeam/beam_00000558-0000.root (6 GeV Beam, 1200V) Tested - Chase/Franceso
//sphenix/user/dvp/testbeam/beam_00000551-0000.root (4 GeV beam),    Tested - Chase    (Imported)
//sphenix/user/dvp/testbeam/beam_00000563-0000.root (2 GeV beam),    Tested - Chase	   (Imported)
//sphenix/user/dvp/testbeam/beam_00000573-0000.root (12 GeV beam),   Tested - Francesco (Imported)
//sphenix/user/dvp/testbeam/beam_00000567-0000.root (8 Gev Beam),    Tested - Chase    (Imported)

//This is a 1100V Run
//sphenix/user/dvp/testbeam/beam_00000544-0000.root (8 GeV beam)

//Additional Runs
/*
	/sphenix/user/dvp/testbeam/beam_00000631-0000.root -  8 GeV -bad run?
	/sphenix/user/dvp/testbeam/beam_00000632-0000.root - 12 GeV -bad run?
	/sphenix/user/dvp/testbeam/beam_00000652-0000.root - 16 GeV - 1100V
	/sphenix/user/dvp/testbeam/beam_00000653-0000.root - 16 GeV - 1000V
	/sphenix/user/dvp/testbeam/beam_00000654-0000.root - 24 GeV - 1000V
	/sphenix/user/dvp/testbeam/beam_00000655-0000.root - 24 GeV - 1100V - bad run
	/sphenix/user/dvp/testbeam/beam_00000687-0000.root - 28 GeV - no record
	
	/sphenix/user/dvp/testbeam/beam_00000776-0000.root - 2 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000777-0000.root - 2 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000809-0000.root - 6 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000810-0000.root - 4 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000816-0000.root - 2 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000829-0000.root - 6 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000830-0000.root - 2 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000849-0000.root - 2 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000859-0000.root - 5 GeV - 1200V
	/sphenix/user/dvp/testbeam/beam_00000900-0000.root - 1 GeV - 1200V

*/


using namespace std;

#include "TLegend.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>
#include <string>


/////////////////////Franceso's included functions (for asthetic) //////////////////////////////////////////////
namespace fhist{
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
}
namespace atlasStyle{
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

float runToEnergy(float run){
    int r;
    int s = (int) run;
    switch (s){
        case 558:
            r=6;
            break;
        case 551:
            r= 4;
            break;
        case 563:
            r= 2;
            break;
        case 573:
            r= 12;
            break;
        case 567:
            r= 8;
            break;
        case 776:
        	r=2;
        	break;
    	case 777:
    		r=2;
    		break;
		case 809:
			r=6;
			break;
		case 810:
			r=4;
			break;
		case 816:
			r=2;
			break;
		case 829:
			r=6;
			break;
		case 830:
			r=2;
			break;
		case 849:
			r=2;
			break;
		case 859:
			r=5;
			break;
		case 900:
			r=1;
			break;
		case 631:
			r=8;
			break;
		case 544:
			r= 8;
			break;
		case 652:
			r= 16;
			break;
		case 653:
			r= 16;
			break;
		case 654:
			r= 24;
			break;
		case 687:
			r= 28;
			break;
		case 572:
			r= 12;
			break;
		case 574:
			r= 12;
			break;
		case 577:
			r= 2;
			break;
		case 578:
			r= 2;
			break;
		case 579:
			r= 2;
			break;	
		case 580:
			r=2;
			break;
        default:
            r=-1;
            break;
    }
    return r;
}

void oleSwitcheroo(float* xp, float* yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void superArraySorter9000(float* energies, float* mean, float* meanError, float* sigma, float* sigmaError,float* numEntries,int SIZE)
{
	int i, j;

	for (i = 0; i < SIZE-1; i++) 
	{  
	   for (j = 0; j < SIZE-i-1; j++) 
	   {
	       if (energies[j] > energies[j+1])
	       {
	          oleSwitcheroo(&energies[j],&energies[j+1]);
	          oleSwitcheroo(&mean[j],&mean[j+1]);
	          oleSwitcheroo(&meanError[j],&meanError[j+1]);
	          oleSwitcheroo(&sigma[j],&sigma[j+1]);
	          oleSwitcheroo(&sigmaError[j],&sigmaError[j+1]);
	          oleSwitcheroo(&numEntries[j],&numEntries[j+1]);
	       }
	   }
	}
}

#ifndef OfficalBeamData_h
#define OfficalBeamData_h 
class OfficalBeamData
{
public:
	OfficalBeamData(){}
	OfficalBeamData(TTree *orange, int beamVoltage, int beamEnergy,string name) : SIZE(orange->GetEntries()), beamVoltage(beamVoltage), beamEnergy(beamEnergy){ 
		if (SIZE==0)
		{
			cout<<"Error Data Size is 0"<<endl;
		}
		else{
			TH1F *pbglPlot = new TH1F(name.c_str(),"",100,0,20);

			double cerenkovEnergies[3]; // only need the [1] value 
	 		orange->SetBranchAddress("TOWER_CALIB_C2.energy", &cerenkovEnergies);
	 		double vetoEnergy[4]; //need all the values 
	 		orange->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", &vetoEnergy);
	 		double hodoVerticalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", &hodoVerticalEnergy);
			double hodoHorizontalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", &hodoHorizontalEnergy);
			double pbglTemp[1];
			orange->SetBranchAddress("TOWER_CALIB_PbGL.energy", &pbglTemp);
			for (int i = 1; i < 5; ++i)
			{
				cout<<"Event"<<i<<'\n';
                cout<<"C:"<<cerenkovEnergies[1]<<'\n';
                cout<<"V:"<<vetoEnergy[0]<<','<<vetoEnergy[1]<<','<<vetoEnergy[2]<<','<<vetoEnergy[3]<<'\n';

				if(i%1000==0) cout<<i<<" events processed"<<'\n';
				orange->GetEntry(i);
				if (passCuts(cerenkovEnergies[1],vetoEnergy,hodoVerticalEnergy,hodoHorizontalEnergy))
				{
					pbglEnergy.push(pbglTemp[0]);
					pbglPlot->Fill(pbglTemp[0]);
					cout<<pbglTemp<<'\n';
				}	
			}
			pbglPlot->Sumw2();
		}
	}
	~OfficalBeamData(){}
	inline bool passCuts(double cerenkov, double* veto, double* vhodo, double* hhodo){
		return cerenkov>CERENKOVcut && noVeto(veto),passHodo(vhodo),passHodo(hhodo);
		//return true;
	}
	inline bool noVeto(double* veto){
		return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<VETOcut;
	}
	inline bool passHodo(double* hodo){
		return hodo[0]>HODOcut && hodo[1]>HODOcut && hodo[2]>HODOcut && hodo[3]>HODOcut && hodo[4]>HODOcut && hodo[5]>HODOcut && hodo[6]>HODOcut && hodo[7]>HODOcut;
	}
	OfficalBeamData& operator=(OfficalBeamData other){
		pbglEnergy=other.pbglEnergy;
		beamVoltage=other.beamVoltage;
		pbglPlot=other.pbglPlot;
		return *this;
	}
	void makeGaus(){
		int maxbin = pbglPlot->GetMaximumBin();
		double gausLowBound = pbglPlot->GetBinLowEdge(maxbin);
		double temp = gausLowBound*.7;
		double gausUpbound = gausLowBound +temp;
		if(gausUpbound >16400){gausUpbound=16400;} //so that fit doesn't exceed range of histogram
		gausLowBound -= temp; 
		TF1* gaus = new TF1("gaus","gaus",gausLowBound,gausUpbound);

		pbglPlot->Fit(gaus,"R");       //“R” Use the range specified in the function range
		double mygaus[2];
		mygaus[0] = gaus->GetParameter(1); //mean
		mygaus[1] = gaus->GetParameter(2); //sigma
		int lazyMan = 0;
		recursiveGaus(pbglPlot, gaus, mygaus, 1.5,lazyMan);
		pbglPlot->GetXaxis()->SetRangeUser(mygaus[0]-(5*mygaus[1]),mygaus[0]+5*mygaus[1]);
		mean = mygaus[0];
		sigma = mygaus[1];
	}	
	double getResolution(){
		return sigma/mean;
	}
	double getMean(){
		return mean;
	}
private:
	int SIZE;

	//we will need to play with these values
	const double CERENKOVcut = 1000;
	const float VETOcut = .25;
	const float HODOcut = .45;

	queue<double> pbglEnergy;
	int beamVoltage;
	int beamEnergy;
	double gaus;
	TH1F *pbglPlot;
	double mean;
	double sigma;

	void recursiveGaus(TH1* h, TF1* gaus, double* data, float sigmadistance,int lazyMan=0){
	    h->Fit(gaus,"","",data[0]-sigmadistance*data[1],data[0]+sigmadistance*data[1]);
	    if(data[0]!=gaus->GetParameter(1)){
	    	if(lazyMan == 100){return;}
	        data[0] = gaus->GetParameter(1);
	        data[1] = gaus->GetParameter(2);
	        lazyMan++;
	        recursiveGaus(h,gaus,data,sigmadistance,lazyMan);
	    }
	    else{
	        data[0] = gaus->GetParameter(1);
	        data[1] = gaus->GetParameter(2);
	        return;
	    }
	}
};
#endif

using namespace fhist;
using namespace atlasStyle;

///////////Analysis of calibrated test beam data//////////////
void BeamAnalysis(TTree *maintree, float* mygaus2){

	Long64_t LENGTH = maintree->GetEntries();
	const int VETOSIZE =4;
	const int HODOSIZE = 8;
	const int CERENSIZE = 3;
	const int PbGlSIZE = 1;
	double cerenkovEnergies[CERENSIZE]; // only need the [1] value 
	maintree->SetBranchAddress("TOWER_CALIB_C2.energy", &cerenkovEnergies);
	double vetoEnergy[VETOSIZE]; //need all the values 
	maintree->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", &vetoEnergy);
	double hodoVerticalEnergy[HODOSIZE];
	maintree->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", &hodoVerticalEnergy);
	double hodoHorizontalEnergy[HODOSIZE];
	maintree->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", &hodoHorizontalEnergy);
	double pbglTemp[PbGlSIZE];
	maintree->SetBranchAddress("TOWER_CALIB_PbGL.energy", &pbglTemp);
			
	//TH1F* ht = new TH1F("temp","",800,200,16400);
	float tempglass;
	for (Long64_t i = 1; i < LENGTH; ++i)
	{
		maintree->GetEntry(i);
		//CalbAnalyzer myPs(cerenkovEnergies[1],vetoEnergy,hodoVerticalEnergy,hodoHorizontalEnergy,pbglTemp);
		cout<<cerenkovEnergies[1]<<'\n';
		//tempglass = myPs.getGoodPbGl();
		
		/*if(tempglass>0){
			goodPbGl.push(tempglass);
			ht->Fill(tempglass);
		}*/
	}
	/*ht->Sumw2();
	TCanvas *tc = new TCanvas();
	gStyle->SetOptStat(0);
	TLegend *tl = new TLegend(.15,.7,.4,.85);
	tl->AddEntry(ht,"PbGl","l");

	fhist::makeNiceHist(ht);
	fhist::axisTitles(ht,"#Delta ADC","Count");
	fhist::axisTitleSize(ht,.03);
	fhist::axisLabelSize(ht,.03);
	fhist::smallBorders();

	int maxbin = ht->GetMaximumBin();
	float gausLowBound = ht->GetBinLowEdge(maxbin);
	float temp = gausLowBound*.7;
	float gausUpbound = gausLowBound +temp;
	if(gausUpbound >16400){gausUpbound=16400;} //so that fit doesn't exceed range of histogram
	gausLowBound -= temp; 
	TF1* gaus = new TF1("gaus","gaus",gausLowBound,gausUpbound);

	gaus->SetLineColor(kRed);
	ht->Fit(gaus,"R");       //“R” Use the range specified in the function range
	float mygaus[2];
	mygaus[0] = gaus->GetParameter(1); //mean
	mygaus[1] = gaus->GetParameter(2); //sigma

	int lazyMan = 0;
	recursiveGaus(ht, gaus, mygaus, lazyMan);

	ht->GetXaxis()->SetRangeUser(mygaus[0]-(5*mygaus[1]),mygaus[0]+5*mygaus[1]);

	tl->AddEntry((TObject*)0,Form("Mean: %0.2f", mygaus[0]),"l");
	tl->AddEntry((TObject*)0,Form("Sigma: %0.2f", mygaus[1]),"l");

	ht->Draw("p");
	tl->Draw();


	mygaus[0] = gaus->GetParameter(1); //mean
	mygaus[1] = gaus->GetParameter(2); //sigma
	mygaus2[0] = mygaus[0];
	mygaus2[1] = mygaus[1];
	mygaus2[2] = runnumber[0];
	mygaus2[3] = gaus->GetParError(1);
	mygaus2[4] = gaus->GetParError(2);

	int rangeLowBin = ht->FindFixBin(gaus->GetParameter(1)-gaus->GetParameter(2));
	int rangeHighBin = ht->FindFixBin(gaus->GetParameter(1)+gaus->GetParameter(2));
	mygaus2[5] = ht->Integral(rangeLowBin,rangeHighBin);*/
}

/////////////////////Implementing BeamAnalysis calibrated files///////////////////////////
void Calib1()
{
	int counter = 0;
	ifstream inFile ("Calib_Runs.txt"); //calibrated runs

	string intemp;
	queue<std::string> someRuns;

	while(inFile>>intemp)
	{
		someRuns.push(intemp);
		cout<<"Run: "<<intemp<<endl;
		counter++;
	}
	inFile.close();
	const unsigned int SIZE = someRuns.size();

	float mygaus2[6]; //temp array to contain sigma and mean
	float whichRuns[SIZE]; //array of run rumbers
	float allmeans[SIZE]; //array of means
	float meanError[SIZE];
	float allsigma[SIZE]; //array of sigma 
	float sigmaError[SIZE];
	float numEntries[SIZE]; 
	int looptemp=0;
	while(!someRuns.empty())
	{
		TFile *input = new TFile(someRuns.front().c_str()); //read in tree so it can be passed to BeamAnalysis1
		someRuns.pop();
		TTree *maintree = (TTree*) input->Get("T");

		BeamAnalysis(maintree, mygaus2); //BeamAnalysis modifies array to contain run number, mean, and sigma
		allmeans[looptemp] = mygaus2[0];
		allsigma[looptemp] = mygaus2[1];
		whichRuns[looptemp] = runToEnergy(mygaus2[2]);
		meanError[looptemp] = mygaus2[3];
		sigmaError[looptemp] = mygaus2[4];
		numEntries[looptemp] = mygaus2[5];
 		looptemp++;
	}

	superArraySorter9000(whichRuns,allmeans,meanError,allsigma,sigmaError,numEntries,SIZE);


	ofstream outFile;
	outFile.open("PbGl_Calib_data1_1100.txt");
	if(outFile.is_open()) //read info out to txt file if it opens
	{
		cout<<"File Opened!"<<endl;

		outFile << "RunNumber";
		for(int i = 0; i < SIZE; i++) //enter all run numbers into txt file
		{
			outFile << ","<< whichRuns[i];
		}
		outFile << "\n";
		outFile << "Mean";
		for(int i = 0; i < SIZE; i++) //enter all mean values into txt file
		{
			outFile << ","<< allmeans[i];
		}
		outFile << "\n";
		outFile << "MeanError";
		for(int i = 0; i < SIZE; i++) //enter all mean values into txt file
		{
			outFile << ","<< meanError[i];
		}
		outFile << "\n";
		outFile << "Sigma";
		for(int i = 0; i < SIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< allsigma[i];
		}
		outFile << "\n";
		outFile << "SigmaError";
		for(int i = 0; i < SIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< sigmaError[i];
		}
		outFile << "\n";
		outFile << "NumEntires";
		for(int i = 0; i < SIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< numEntries[i];
		}
		outFile << "\n";

		outFile.close();
	}
	else {cout<<"RED ALERT! RED ALERT! FAILED TO WRITE TO A TEXT FILE! I REPEAT! RED ALERT!"<<endl;}

}

