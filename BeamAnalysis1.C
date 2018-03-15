#include "TFile.h"
#include "TCanvas.h"
/*#include "AtlasStyle.h"
#include "AtlasUtils.h"*/
#include "TLegend.h"

using namespace std;

namespace nicehists{
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
		h->SetMarkerStyle(kFullTriangleUp);
	}
}

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
		return pmp(172)<100&&pmp(173)<100&&pmp(174)<150&&pmp(175)<200&&pmp(160)>1000;
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

void BeamAnalysis1(){
	TFile *input = new TFile("beam_00000558-0000.root");
	TTree *maintree = (TTree*) input->Get("W;24");
	const int SIZE = 192;
	Long64_t LENGTH = maintree->GetEntries();
	const int VETOSIZE =4;
	const int HODOSIZE = 8;
	float mypeak[SIZE];
	float mypedestal[SIZE];
	maintree->SetBranchAddress("peak",&mypeak);
	maintree->SetBranchAddress("pedestal",&mypedestal);
	TH1F* ht = new TH1F("temp","",50,50,5000);
	queue<float> veto[4];
	queue<float> hHodo[8];
	queue<float> vHodo[8];
	queue<float> cerenkov;
	queue<float> goodPbGl;
	float tempglass;
	for (Long64_t i = 0; i < LENGTH; ++i)
	{
		//cout<<"here"<<'\n';
		maintree->GetEntry(i);
		DiffADC myPs(&mypeak[0],&mypedestal[0]);
		/* get the veto */
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
		//cout<<"there"<<'\n';
		if(tempglass>0){
			goodPbGl.push(tempglass);
			ht->Fill(tempglass);
		}
		//cout<<tempglass<<'\n';
	}

	TCanvas *tc = new TCanvas();
	gStyle->SetOptStat(0);
	TLegend *tl = new TLegend(.3,.2,.4,.3);
	tl->AddEntry(ht,"PbGl","l");
	nicehists::makeNiceHist(ht);
	ht->Draw("l");
	tl->Draw();
	
}