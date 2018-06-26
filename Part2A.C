#include <sstream>
#include <queue>
#include "TH1F.h"
#include "TChain.h"
#include <iostream>
#include "TF1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"

using namespace std;

namespace {
	int plotcount=0;
}

int runToVoltage(int run);
int runToEnergy(int run);

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

			Double_t cerenkovEnergies[3]; // only need the [1] value 
	 		orange->SetBranchAddress("TOWER_CALIB_C2.energy", &cerenkovEnergies);
	 		Double_t vetoEnergy[4]; //need all the values 
	 		orange->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", &vetoEnergy);
	 		Double_t hodoVerticalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", &hodoVerticalEnergy);
			Double_t hodoHorizontalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", &hodoHorizontalEnergy);
			Double_t pbglTemp[1];
			orange->SetBranchAddress("TOWER_CALIB_PbGL.energy", &pbglTemp);
			cout<<orange->GetEntries()<<endl;
			for (int i = 1; i < 5; ++i)
			{
				orange->GetEntry(i);
				cout<<"Event"<<i<<'\n';
                cout<<"C:"<<cerenkovEnergies[1]<<'\n';
                cout<<"V:"<<vetoEnergy[0]<<','<<vetoEnergy[1]<<','<<vetoEnergy[2]<<','<<vetoEnergy[3]<<'\n';

				if(i%1000==0) cout<<i<<" events processed"<<'\n';
				
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
	OfficalBeamData(string name, int voltage, int energy) : beamVoltage(voltage), beamEnergy(energy){
		pbglPlot = new TH1F(name.c_str(),"",200,0,10000); // note the bounds are weird
		pbglPlot->Sumw2();
	}
	~OfficalBeamData(){}
	bool add(double cerenkov, double* veto, double* hhodo, double* vhodo, double pbgl){
		bool r = passCuts(cerenkov,veto,vhodo,hhodo);
		if (r)
		{
			//cout<<"This:"<<pbgl<<'\n';
			pbglEnergy.push(pbgl);
			pbglPlot->Fill(pbgl);			
		}
		return r;
	}
	inline bool passCuts(double cerenkov, double* veto, double* vhodo, double* hhodo){
		return cerenkov>CERENKOVcut && noVeto(veto),passHodo(vhodo),passHodo(hhodo);
		//return true;
	}
	inline bool noVeto(double* veto){
		return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<VETOcut;
	}
	inline bool passHodo(double* hodo){
		return hodo[0]>HODOcut || hodo[1]>HODOcut || hodo[2]>HODOcut || hodo[3]>HODOcut || hodo[4]>HODOcut || hodo[5]>HODOcut || hodo[6]>HODOcut || hodo[7]>HODOcut;
	}
	TF1* makeGaus(){
		int maxbin = pbglPlot->GetMaximumBin();
		double gausLowBound = pbglPlot->GetBinLowEdge(maxbin);
		double temp = gausLowBound*.7;
		double gausUpbound = gausLowBound +temp;
		if(gausUpbound >10000){gausUpbound=10000;} //so that fit doesn't exceed range of histogram
		gausLowBound -= temp; 
		TF1* gaus = new TF1("gaus","gaus",gausLowBound,gausUpbound);

		pbglPlot->Fit(gaus,"R");       //“R” Use the range specified in the function range
		Scalar mygaus[2];
		mygaus[0] = Scalar(gaus->GetParameter(1),gaus->GetParError(1)); //mean
		mygaus[1] = Scalar(gaus->GetParameter(2),gaus->GetParError(2)); //sigma
		int lazyMan = 0;
		recursiveGaus(pbglPlot, gaus, mygaus, 1.5,lazyMan);
		pbglPlot->GetXaxis()->SetRangeUser(mygaus[0]-(mygaus[1]*5.0),mygaus[0]+mygaus[1]*5.0);
		mean = mygaus[0];
		sigma = mygaus[1];
		return gaus;
	}	
	void plot(){
		TCanvas *tc = new TCanvas();
		pbglPlot->Draw();
		makeGaus()->Draw("same");
	}
	double getResolution(){
		return (sigma/mean).value;
	}
	double getMean(){
		return mean.value;
	}
	double getResolutionUncertainty(){
		return (sigma/mean).uncertainty;
	}
	double getMeanUncertainty(){
		return mean.uncertainty;
	}
	OfficalBeamData& operator=(OfficalBeamData other){
		pbglEnergy=other.pbglEnergy;
		beamVoltage=other.beamVoltage;
		pbglPlot=other.pbglPlot;
		return *this;
	}
private:
	int SIZE;

	//we will need to play with these values
	const double CERENKOVcut = 1600; //previously 1000
	const float VETOcut = .3; // from dennis 
	const float HODOcut = .45;
	const int VETOSIZE = 4;
	const int HODOSIZE = 8;

	queue<double> pbglEnergy;
	int beamVoltage;
	int beamEnergy;
	TH1F *pbglPlot;
	Scalar mean;
	Scalar sigma;

	void recursiveGaus(TH1* h, TF1* gaus, Scalar* data, float sigmadistance,int lazyMan=0){
	    h->Fit(gaus,"","",data[0]-data[1]*sigmadistance,data[0]+data[1]*sigmadistance);
	    if(data[0]!=gaus->GetParameter(1)){
	    	if(lazyMan == 100){return;}
	        data[0] = Scalar(gaus->GetParameter(1),gaus->GetParError(1));
	        data[1] = Scalar(gaus->GetParameter(2),gaus->GetParError(2));
	        lazyMan++;
	        recursiveGaus(h,gaus,data,sigmadistance,lazyMan);
	    }
	    else{
	        data[0] = Scalar(gaus->GetParameter(1),gaus->GetParError(1));
	        data[1] = Scalar(gaus->GetParameter(2),gaus->GetParError(2));
	        return;
	    }
	}
};
#endif

class DSTReader551 {

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun 26 12:53:58 2018 by ROOT version 6.10/02
// from TTree T/Prototype4DSTReader
// found on file: beam_00000551-0000_DSTReader.root
//////////////////////////////////////////////////////////


public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxTOWER_RAW_LG_HCALIN = 16;
   static constexpr Int_t kMaxTOWER_RAW_LG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_RAW_HG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_CALIB_LG_HCALIN = 16;
   static constexpr Int_t kMaxTOWER_CALIB_LG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_CALIB_HG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_RAW_CEMC = 64;
   static constexpr Int_t kMaxTOWER_CALIB_CEMC = 64;
   static constexpr Int_t kMaxTOWER_RAW_HODO_VERTICAL = 8;
   static constexpr Int_t kMaxTOWER_RAW_HODO_HORIZONTAL = 8;
   static constexpr Int_t kMaxTOWER_CALIB_HODO_VERTICAL = 8;
   static constexpr Int_t kMaxTOWER_CALIB_HODO_HORIZONTAL = 8;
   static constexpr Int_t kMaxTOWER_RAW_C1 = 1;
   static constexpr Int_t kMaxTOWER_CALIB_C1 = 1;
   static constexpr Int_t kMaxTOWER_RAW_C2 = 3;
   static constexpr Int_t kMaxTOWER_CALIB_C2 = 3;
   static constexpr Int_t kMaxTOWER_RAW_PbGL = 1;
   static constexpr Int_t kMaxTOWER_CALIB_PbGL = 1;
   static constexpr Int_t kMaxTOWER_RAW_TRIGGER_VETO = 4;
   static constexpr Int_t kMaxTOWER_CALIB_TRIGGER_VETO = 4;
   static constexpr Int_t kMaxTOWER_RAW_TILE_MAPPER = 16;
   static constexpr Int_t kMaxTOWER_CALIB_TILE_MAPPER = 16;
   static constexpr Int_t kMaxTOWER_RAW_SC3 = 1;
   static constexpr Int_t kMaxTOWER_CALIB_SC3 = 1;
   static constexpr Int_t kMaxTOWER_RAW_FINGER_COUNTER = 1;
   static constexpr Int_t kMaxTOWER_CALIB_FINGER_COUNTER = 1;

   // Declaration of leaf types
   Double_t        beam_MTNRG_GeV;
   Double_t        beam_2CH_mm;
   Double_t        beam_2CV_mm;
   Double_t        EMCAL_A0_HighGain;
   Double_t        EMCAL_GR0_BiasOffset_Tower21;
   Double_t        EMCAL_T0_Tower21;
   Double_t        EMCAL_Is_HighEta;
   Int_t           n_TOWER_RAW_LG_HCALIN;
   Int_t           TOWER_RAW_LG_HCALIN_;
   UInt_t          TOWER_RAW_LG_HCALIN_fUniqueID[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   UInt_t          TOWER_RAW_LG_HCALIN_fBits[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   UInt_t          TOWER_RAW_LG_HCALIN_towerid[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Double_t        TOWER_RAW_LG_HCALIN_energy[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Float_t         TOWER_RAW_LG_HCALIN_time[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Float_t         TOWER_RAW_LG_HCALIN_signal_samples[kMaxTOWER_RAW_LG_HCALIN][31];   //[TOWER_RAW_LG_HCALIN_]
   Int_t           TOWER_RAW_LG_HCALIN_HBD_channel[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Int_t           n_TOWER_RAW_LG_HCALOUT;
   Int_t           TOWER_RAW_LG_HCALOUT_;
   UInt_t          TOWER_RAW_LG_HCALOUT_fUniqueID[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   UInt_t          TOWER_RAW_LG_HCALOUT_fBits[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   UInt_t          TOWER_RAW_LG_HCALOUT_towerid[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Double_t        TOWER_RAW_LG_HCALOUT_energy[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Float_t         TOWER_RAW_LG_HCALOUT_time[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Float_t         TOWER_RAW_LG_HCALOUT_signal_samples[kMaxTOWER_RAW_LG_HCALOUT][31];   //[TOWER_RAW_LG_HCALOUT_]
   Int_t           TOWER_RAW_LG_HCALOUT_HBD_channel[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Int_t           n_TOWER_RAW_HG_HCALOUT;
   Int_t           TOWER_RAW_HG_HCALOUT_;
   UInt_t          TOWER_RAW_HG_HCALOUT_fUniqueID[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   UInt_t          TOWER_RAW_HG_HCALOUT_fBits[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   UInt_t          TOWER_RAW_HG_HCALOUT_towerid[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Double_t        TOWER_RAW_HG_HCALOUT_energy[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Float_t         TOWER_RAW_HG_HCALOUT_time[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Float_t         TOWER_RAW_HG_HCALOUT_signal_samples[kMaxTOWER_RAW_HG_HCALOUT][31];   //[TOWER_RAW_HG_HCALOUT_]
   Int_t           TOWER_RAW_HG_HCALOUT_HBD_channel[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Int_t           n_TOWER_CALIB_LG_HCALIN;
   Int_t           TOWER_CALIB_LG_HCALIN_;
   UInt_t          TOWER_CALIB_LG_HCALIN_fUniqueID[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   UInt_t          TOWER_CALIB_LG_HCALIN_fBits[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   UInt_t          TOWER_CALIB_LG_HCALIN_towerid[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Double_t        TOWER_CALIB_LG_HCALIN_energy[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Float_t         TOWER_CALIB_LG_HCALIN_time[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Float_t         TOWER_CALIB_LG_HCALIN_signal_samples[kMaxTOWER_CALIB_LG_HCALIN][31];   //[TOWER_CALIB_LG_HCALIN_]
   Int_t           TOWER_CALIB_LG_HCALIN_HBD_channel[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Int_t           n_TOWER_CALIB_LG_HCALOUT;
   Int_t           TOWER_CALIB_LG_HCALOUT_;
   UInt_t          TOWER_CALIB_LG_HCALOUT_fUniqueID[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   UInt_t          TOWER_CALIB_LG_HCALOUT_fBits[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   UInt_t          TOWER_CALIB_LG_HCALOUT_towerid[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Double_t        TOWER_CALIB_LG_HCALOUT_energy[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Float_t         TOWER_CALIB_LG_HCALOUT_time[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Float_t         TOWER_CALIB_LG_HCALOUT_signal_samples[kMaxTOWER_CALIB_LG_HCALOUT][31];   //[TOWER_CALIB_LG_HCALOUT_]
   Int_t           TOWER_CALIB_LG_HCALOUT_HBD_channel[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Int_t           n_TOWER_CALIB_HG_HCALOUT;
   Int_t           TOWER_CALIB_HG_HCALOUT_;
   UInt_t          TOWER_CALIB_HG_HCALOUT_fUniqueID[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   UInt_t          TOWER_CALIB_HG_HCALOUT_fBits[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   UInt_t          TOWER_CALIB_HG_HCALOUT_towerid[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Double_t        TOWER_CALIB_HG_HCALOUT_energy[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Float_t         TOWER_CALIB_HG_HCALOUT_time[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Float_t         TOWER_CALIB_HG_HCALOUT_signal_samples[kMaxTOWER_CALIB_HG_HCALOUT][31];   //[TOWER_CALIB_HG_HCALOUT_]
   Int_t           TOWER_CALIB_HG_HCALOUT_HBD_channel[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Int_t           n_TOWER_RAW_CEMC;
   Int_t           TOWER_RAW_CEMC_;
   UInt_t          TOWER_RAW_CEMC_fUniqueID[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   UInt_t          TOWER_RAW_CEMC_fBits[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   UInt_t          TOWER_RAW_CEMC_towerid[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Double_t        TOWER_RAW_CEMC_energy[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Float_t         TOWER_RAW_CEMC_time[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Float_t         TOWER_RAW_CEMC_signal_samples[kMaxTOWER_RAW_CEMC][31];   //[TOWER_RAW_CEMC_]
   Int_t           TOWER_RAW_CEMC_HBD_channel[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Int_t           n_TOWER_CALIB_CEMC;
   Int_t           TOWER_CALIB_CEMC_;
   UInt_t          TOWER_CALIB_CEMC_fUniqueID[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   UInt_t          TOWER_CALIB_CEMC_fBits[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   UInt_t          TOWER_CALIB_CEMC_towerid[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Double_t        TOWER_CALIB_CEMC_energy[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Float_t         TOWER_CALIB_CEMC_time[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Float_t         TOWER_CALIB_CEMC_signal_samples[kMaxTOWER_CALIB_CEMC][31];   //[TOWER_CALIB_CEMC_]
   Int_t           TOWER_CALIB_CEMC_HBD_channel[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Int_t           n_TOWER_RAW_HODO_VERTICAL;
   Int_t           TOWER_RAW_HODO_VERTICAL_;
   UInt_t          TOWER_RAW_HODO_VERTICAL_fUniqueID[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   UInt_t          TOWER_RAW_HODO_VERTICAL_fBits[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   UInt_t          TOWER_RAW_HODO_VERTICAL_towerid[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Double_t        TOWER_RAW_HODO_VERTICAL_energy[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Float_t         TOWER_RAW_HODO_VERTICAL_time[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Float_t         TOWER_RAW_HODO_VERTICAL_signal_samples[kMaxTOWER_RAW_HODO_VERTICAL][31];   //[TOWER_RAW_HODO_VERTICAL_]
   Int_t           TOWER_RAW_HODO_VERTICAL_HBD_channel[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Int_t           n_TOWER_RAW_HODO_HORIZONTAL;
   Int_t           TOWER_RAW_HODO_HORIZONTAL_;
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_fUniqueID[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_fBits[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_towerid[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Double_t        TOWER_RAW_HODO_HORIZONTAL_energy[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Float_t         TOWER_RAW_HODO_HORIZONTAL_time[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Float_t         TOWER_RAW_HODO_HORIZONTAL_signal_samples[kMaxTOWER_RAW_HODO_HORIZONTAL][31];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Int_t           TOWER_RAW_HODO_HORIZONTAL_HBD_channel[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Int_t           n_TOWER_CALIB_HODO_VERTICAL;
   Int_t           TOWER_CALIB_HODO_VERTICAL_;
   UInt_t          TOWER_CALIB_HODO_VERTICAL_fUniqueID[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   UInt_t          TOWER_CALIB_HODO_VERTICAL_fBits[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   UInt_t          TOWER_CALIB_HODO_VERTICAL_towerid[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Double_t        TOWER_CALIB_HODO_VERTICAL_energy[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Float_t         TOWER_CALIB_HODO_VERTICAL_time[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Float_t         TOWER_CALIB_HODO_VERTICAL_signal_samples[kMaxTOWER_CALIB_HODO_VERTICAL][31];   //[TOWER_CALIB_HODO_VERTICAL_]
   Int_t           TOWER_CALIB_HODO_VERTICAL_HBD_channel[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Int_t           n_TOWER_CALIB_HODO_HORIZONTAL;
   Int_t           TOWER_CALIB_HODO_HORIZONTAL_;
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_fUniqueID[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_fBits[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_towerid[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Double_t        TOWER_CALIB_HODO_HORIZONTAL_energy[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Float_t         TOWER_CALIB_HODO_HORIZONTAL_time[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Float_t         TOWER_CALIB_HODO_HORIZONTAL_signal_samples[kMaxTOWER_CALIB_HODO_HORIZONTAL][31];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Int_t           TOWER_CALIB_HODO_HORIZONTAL_HBD_channel[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Int_t           n_TOWER_RAW_C1;
   Int_t           TOWER_RAW_C1_;
   UInt_t          TOWER_RAW_C1_fUniqueID[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   UInt_t          TOWER_RAW_C1_fBits[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   UInt_t          TOWER_RAW_C1_towerid[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Double_t        TOWER_RAW_C1_energy[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Float_t         TOWER_RAW_C1_time[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Float_t         TOWER_RAW_C1_signal_samples[kMaxTOWER_RAW_C1][31];   //[TOWER_RAW_C1_]
   Int_t           TOWER_RAW_C1_HBD_channel[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Int_t           n_TOWER_CALIB_C1;
   Int_t           TOWER_CALIB_C1_;
   UInt_t          TOWER_CALIB_C1_fUniqueID[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   UInt_t          TOWER_CALIB_C1_fBits[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   UInt_t          TOWER_CALIB_C1_towerid[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Double_t        TOWER_CALIB_C1_energy[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Float_t         TOWER_CALIB_C1_time[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Float_t         TOWER_CALIB_C1_signal_samples[kMaxTOWER_CALIB_C1][31];   //[TOWER_CALIB_C1_]
   Int_t           TOWER_CALIB_C1_HBD_channel[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Int_t           n_TOWER_RAW_C2;
   Int_t           TOWER_RAW_C2_;
   UInt_t          TOWER_RAW_C2_fUniqueID[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   UInt_t          TOWER_RAW_C2_fBits[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   UInt_t          TOWER_RAW_C2_towerid[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Double_t        TOWER_RAW_C2_energy[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Float_t         TOWER_RAW_C2_time[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Float_t         TOWER_RAW_C2_signal_samples[kMaxTOWER_RAW_C2][31];   //[TOWER_RAW_C2_]
   Int_t           TOWER_RAW_C2_HBD_channel[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Int_t           n_TOWER_CALIB_C2;
   Int_t           TOWER_CALIB_C2_;
   UInt_t          TOWER_CALIB_C2_fUniqueID[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   UInt_t          TOWER_CALIB_C2_fBits[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   UInt_t          TOWER_CALIB_C2_towerid[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Double_t        TOWER_CALIB_C2_energy[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Float_t         TOWER_CALIB_C2_time[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Float_t         TOWER_CALIB_C2_signal_samples[kMaxTOWER_CALIB_C2][31];   //[TOWER_CALIB_C2_]
   Int_t           TOWER_CALIB_C2_HBD_channel[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Int_t           n_TOWER_RAW_PbGL;
   Int_t           TOWER_RAW_PbGL_;
   UInt_t          TOWER_RAW_PbGL_fUniqueID[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   UInt_t          TOWER_RAW_PbGL_fBits[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   UInt_t          TOWER_RAW_PbGL_towerid[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Double_t        TOWER_RAW_PbGL_energy[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Float_t         TOWER_RAW_PbGL_time[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Float_t         TOWER_RAW_PbGL_signal_samples[kMaxTOWER_RAW_PbGL][31];   //[TOWER_RAW_PbGL_]
   Int_t           TOWER_RAW_PbGL_HBD_channel[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Int_t           n_TOWER_CALIB_PbGL;
   Int_t           TOWER_CALIB_PbGL_;
   UInt_t          TOWER_CALIB_PbGL_fUniqueID[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   UInt_t          TOWER_CALIB_PbGL_fBits[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   UInt_t          TOWER_CALIB_PbGL_towerid[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Double_t        TOWER_CALIB_PbGL_energy[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Float_t         TOWER_CALIB_PbGL_time[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Float_t         TOWER_CALIB_PbGL_signal_samples[kMaxTOWER_CALIB_PbGL][31];   //[TOWER_CALIB_PbGL_]
   Int_t           TOWER_CALIB_PbGL_HBD_channel[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Int_t           n_TOWER_RAW_TRIGGER_VETO;
   Int_t           TOWER_RAW_TRIGGER_VETO_;
   UInt_t          TOWER_RAW_TRIGGER_VETO_fUniqueID[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   UInt_t          TOWER_RAW_TRIGGER_VETO_fBits[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   UInt_t          TOWER_RAW_TRIGGER_VETO_towerid[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Double_t        TOWER_RAW_TRIGGER_VETO_energy[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Float_t         TOWER_RAW_TRIGGER_VETO_time[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Float_t         TOWER_RAW_TRIGGER_VETO_signal_samples[kMaxTOWER_RAW_TRIGGER_VETO][31];   //[TOWER_RAW_TRIGGER_VETO_]
   Int_t           TOWER_RAW_TRIGGER_VETO_HBD_channel[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Int_t           n_TOWER_CALIB_TRIGGER_VETO;
   Int_t           TOWER_CALIB_TRIGGER_VETO_;
   UInt_t          TOWER_CALIB_TRIGGER_VETO_fUniqueID[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   UInt_t          TOWER_CALIB_TRIGGER_VETO_fBits[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   UInt_t          TOWER_CALIB_TRIGGER_VETO_towerid[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Double_t        TOWER_CALIB_TRIGGER_VETO_energy[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Float_t         TOWER_CALIB_TRIGGER_VETO_time[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Float_t         TOWER_CALIB_TRIGGER_VETO_signal_samples[kMaxTOWER_CALIB_TRIGGER_VETO][31];   //[TOWER_CALIB_TRIGGER_VETO_]
   Int_t           TOWER_CALIB_TRIGGER_VETO_HBD_channel[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Int_t           n_TOWER_RAW_TILE_MAPPER;
   Int_t           TOWER_RAW_TILE_MAPPER_;
   UInt_t          TOWER_RAW_TILE_MAPPER_fUniqueID[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   UInt_t          TOWER_RAW_TILE_MAPPER_fBits[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   UInt_t          TOWER_RAW_TILE_MAPPER_towerid[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Double_t        TOWER_RAW_TILE_MAPPER_energy[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Float_t         TOWER_RAW_TILE_MAPPER_time[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Float_t         TOWER_RAW_TILE_MAPPER_signal_samples[kMaxTOWER_RAW_TILE_MAPPER][31];   //[TOWER_RAW_TILE_MAPPER_]
   Int_t           TOWER_RAW_TILE_MAPPER_HBD_channel[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Int_t           n_TOWER_CALIB_TILE_MAPPER;
   Int_t           TOWER_CALIB_TILE_MAPPER_;
   UInt_t          TOWER_CALIB_TILE_MAPPER_fUniqueID[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   UInt_t          TOWER_CALIB_TILE_MAPPER_fBits[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   UInt_t          TOWER_CALIB_TILE_MAPPER_towerid[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Double_t        TOWER_CALIB_TILE_MAPPER_energy[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Float_t         TOWER_CALIB_TILE_MAPPER_time[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Float_t         TOWER_CALIB_TILE_MAPPER_signal_samples[kMaxTOWER_CALIB_TILE_MAPPER][31];   //[TOWER_CALIB_TILE_MAPPER_]
   Int_t           TOWER_CALIB_TILE_MAPPER_HBD_channel[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Int_t           n_TOWER_RAW_SC3;
   Int_t           TOWER_RAW_SC3_;
   UInt_t          TOWER_RAW_SC3_fUniqueID[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   UInt_t          TOWER_RAW_SC3_fBits[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   UInt_t          TOWER_RAW_SC3_towerid[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Double_t        TOWER_RAW_SC3_energy[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Float_t         TOWER_RAW_SC3_time[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Float_t         TOWER_RAW_SC3_signal_samples[kMaxTOWER_RAW_SC3][31];   //[TOWER_RAW_SC3_]
   Int_t           TOWER_RAW_SC3_HBD_channel[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Int_t           n_TOWER_CALIB_SC3;
   Int_t           TOWER_CALIB_SC3_;
   UInt_t          TOWER_CALIB_SC3_fUniqueID[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   UInt_t          TOWER_CALIB_SC3_fBits[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   UInt_t          TOWER_CALIB_SC3_towerid[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Double_t        TOWER_CALIB_SC3_energy[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Float_t         TOWER_CALIB_SC3_time[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Float_t         TOWER_CALIB_SC3_signal_samples[kMaxTOWER_CALIB_SC3][31];   //[TOWER_CALIB_SC3_]
   Int_t           TOWER_CALIB_SC3_HBD_channel[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Int_t           n_TOWER_RAW_FINGER_COUNTER;
   Int_t           TOWER_RAW_FINGER_COUNTER_;
   UInt_t          TOWER_RAW_FINGER_COUNTER_fUniqueID[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   UInt_t          TOWER_RAW_FINGER_COUNTER_fBits[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   UInt_t          TOWER_RAW_FINGER_COUNTER_towerid[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Double_t        TOWER_RAW_FINGER_COUNTER_energy[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Float_t         TOWER_RAW_FINGER_COUNTER_time[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Float_t         TOWER_RAW_FINGER_COUNTER_signal_samples[kMaxTOWER_RAW_FINGER_COUNTER][31];   //[TOWER_RAW_FINGER_COUNTER_]
   Int_t           TOWER_RAW_FINGER_COUNTER_HBD_channel[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Int_t           n_TOWER_CALIB_FINGER_COUNTER;
   Int_t           TOWER_CALIB_FINGER_COUNTER_;
   UInt_t          TOWER_CALIB_FINGER_COUNTER_fUniqueID[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   UInt_t          TOWER_CALIB_FINGER_COUNTER_fBits[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   UInt_t          TOWER_CALIB_FINGER_COUNTER_towerid[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Double_t        TOWER_CALIB_FINGER_COUNTER_energy[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Float_t         TOWER_CALIB_FINGER_COUNTER_time[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Float_t         TOWER_CALIB_FINGER_COUNTER_signal_samples[kMaxTOWER_CALIB_FINGER_COUNTER][31];   //[TOWER_CALIB_FINGER_COUNTER_]
   Int_t           TOWER_CALIB_FINGER_COUNTER_HBD_channel[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]

   // List of branches
   TBranch        *b_beam_MTNRG_GeV;   //!
   TBranch        *b_beam_2CH_mm;   //!
   TBranch        *b_beam_2CV_mm;   //!
   TBranch        *b_EMCAL_A0_HighGain;   //!
   TBranch        *b_EMCAL_GR0_BiasOffset_Tower21;   //!
   TBranch        *b_EMCAL_T0_Tower21;   //!
   TBranch        *b_EMCAL_Is_HighEta;   //!
   TBranch        *b_n_TOWER_RAW_LG_HCALIN;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_fBits;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_towerid;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_energy;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_time;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_signal_samples;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_LG_HCALOUT;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_time;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HG_HCALOUT;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_time;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_LG_HCALIN;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_fBits;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_towerid;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_energy;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_time;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_LG_HCALOUT;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_time;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HG_HCALOUT;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_time;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_CEMC;   //!
   TBranch        *b_TOWER_RAW_CEMC_;   //!
   TBranch        *b_TOWER_RAW_CEMC_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_CEMC_fBits;   //!
   TBranch        *b_TOWER_RAW_CEMC_towerid;   //!
   TBranch        *b_TOWER_RAW_CEMC_energy;   //!
   TBranch        *b_TOWER_RAW_CEMC_time;   //!
   TBranch        *b_TOWER_RAW_CEMC_signal_samples;   //!
   TBranch        *b_TOWER_RAW_CEMC_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_CEMC;   //!
   TBranch        *b_TOWER_CALIB_CEMC_;   //!
   TBranch        *b_TOWER_CALIB_CEMC_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_CEMC_fBits;   //!
   TBranch        *b_TOWER_CALIB_CEMC_towerid;   //!
   TBranch        *b_TOWER_CALIB_CEMC_energy;   //!
   TBranch        *b_TOWER_CALIB_CEMC_time;   //!
   TBranch        *b_TOWER_CALIB_CEMC_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_CEMC_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HODO_VERTICAL;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_fBits;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_towerid;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_energy;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_time;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HODO_HORIZONTAL;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_fBits;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_towerid;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_energy;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_time;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HODO_VERTICAL;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_fBits;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_towerid;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_energy;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_time;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HODO_HORIZONTAL;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_fBits;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_towerid;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_energy;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_time;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_C1;   //!
   TBranch        *b_TOWER_RAW_C1_;   //!
   TBranch        *b_TOWER_RAW_C1_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_C1_fBits;   //!
   TBranch        *b_TOWER_RAW_C1_towerid;   //!
   TBranch        *b_TOWER_RAW_C1_energy;   //!
   TBranch        *b_TOWER_RAW_C1_time;   //!
   TBranch        *b_TOWER_RAW_C1_signal_samples;   //!
   TBranch        *b_TOWER_RAW_C1_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_C1;   //!
   TBranch        *b_TOWER_CALIB_C1_;   //!
   TBranch        *b_TOWER_CALIB_C1_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_C1_fBits;   //!
   TBranch        *b_TOWER_CALIB_C1_towerid;   //!
   TBranch        *b_TOWER_CALIB_C1_energy;   //!
   TBranch        *b_TOWER_CALIB_C1_time;   //!
   TBranch        *b_TOWER_CALIB_C1_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_C1_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_C2;   //!
   TBranch        *b_TOWER_RAW_C2_;   //!
   TBranch        *b_TOWER_RAW_C2_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_C2_fBits;   //!
   TBranch        *b_TOWER_RAW_C2_towerid;   //!
   TBranch        *b_TOWER_RAW_C2_energy;   //!
   TBranch        *b_TOWER_RAW_C2_time;   //!
   TBranch        *b_TOWER_RAW_C2_signal_samples;   //!
   TBranch        *b_TOWER_RAW_C2_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_C2;   //!
   TBranch        *b_TOWER_CALIB_C2_;   //!
   TBranch        *b_TOWER_CALIB_C2_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_C2_fBits;   //!
   TBranch        *b_TOWER_CALIB_C2_towerid;   //!
   TBranch        *b_TOWER_CALIB_C2_energy;   //!
   TBranch        *b_TOWER_CALIB_C2_time;   //!
   TBranch        *b_TOWER_CALIB_C2_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_C2_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_PbGL;   //!
   TBranch        *b_TOWER_RAW_PbGL_;   //!
   TBranch        *b_TOWER_RAW_PbGL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_PbGL_fBits;   //!
   TBranch        *b_TOWER_RAW_PbGL_towerid;   //!
   TBranch        *b_TOWER_RAW_PbGL_energy;   //!
   TBranch        *b_TOWER_RAW_PbGL_time;   //!
   TBranch        *b_TOWER_RAW_PbGL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_PbGL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_PbGL;   //!
   TBranch        *b_TOWER_CALIB_PbGL_;   //!
   TBranch        *b_TOWER_CALIB_PbGL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_PbGL_fBits;   //!
   TBranch        *b_TOWER_CALIB_PbGL_towerid;   //!
   TBranch        *b_TOWER_CALIB_PbGL_energy;   //!
   TBranch        *b_TOWER_CALIB_PbGL_time;   //!
   TBranch        *b_TOWER_CALIB_PbGL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_PbGL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_TRIGGER_VETO;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_fBits;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_towerid;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_energy;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_time;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_signal_samples;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_TRIGGER_VETO;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_fBits;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_towerid;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_energy;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_time;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_TILE_MAPPER;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_fBits;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_towerid;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_energy;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_time;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_signal_samples;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_TILE_MAPPER;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_fBits;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_towerid;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_energy;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_time;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_SC3;   //!
   TBranch        *b_TOWER_RAW_SC3_;   //!
   TBranch        *b_TOWER_RAW_SC3_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_SC3_fBits;   //!
   TBranch        *b_TOWER_RAW_SC3_towerid;   //!
   TBranch        *b_TOWER_RAW_SC3_energy;   //!
   TBranch        *b_TOWER_RAW_SC3_time;   //!
   TBranch        *b_TOWER_RAW_SC3_signal_samples;   //!
   TBranch        *b_TOWER_RAW_SC3_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_SC3;   //!
   TBranch        *b_TOWER_CALIB_SC3_;   //!
   TBranch        *b_TOWER_CALIB_SC3_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_SC3_fBits;   //!
   TBranch        *b_TOWER_CALIB_SC3_towerid;   //!
   TBranch        *b_TOWER_CALIB_SC3_energy;   //!
   TBranch        *b_TOWER_CALIB_SC3_time;   //!
   TBranch        *b_TOWER_CALIB_SC3_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_SC3_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_FINGER_COUNTER;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_fBits;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_towerid;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_energy;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_time;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_signal_samples;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_FINGER_COUNTER;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_fBits;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_towerid;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_energy;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_time;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_HBD_channel;   //!

   DSTReader551(TTree *tree=0);
   virtual ~DSTReader551();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

DSTReader551::DSTReader551(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("beam_00000551-0000_DSTReader.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("beam_00000551-0000_DSTReader.root");
      }
      f->GetObject("T",tree);

   }
   Init(tree);
}

DSTReader551::~DSTReader551()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DSTReader551::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DSTReader551::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DSTReader551::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("beam_MTNRG_GeV", &beam_MTNRG_GeV, &b_beam_MTNRG_GeV);
   fChain->SetBranchAddress("beam_2CH_mm", &beam_2CH_mm, &b_beam_2CH_mm);
   fChain->SetBranchAddress("beam_2CV_mm", &beam_2CV_mm, &b_beam_2CV_mm);
   fChain->SetBranchAddress("EMCAL_A0_HighGain", &EMCAL_A0_HighGain, &b_EMCAL_A0_HighGain);
   fChain->SetBranchAddress("EMCAL_GR0_BiasOffset_Tower21", &EMCAL_GR0_BiasOffset_Tower21, &b_EMCAL_GR0_BiasOffset_Tower21);
   fChain->SetBranchAddress("EMCAL_T0_Tower21", &EMCAL_T0_Tower21, &b_EMCAL_T0_Tower21);
   fChain->SetBranchAddress("EMCAL_Is_HighEta", &EMCAL_Is_HighEta, &b_EMCAL_Is_HighEta);
   fChain->SetBranchAddress("n_TOWER_RAW_LG_HCALIN", &n_TOWER_RAW_LG_HCALIN, &b_n_TOWER_RAW_LG_HCALIN);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN", &TOWER_RAW_LG_HCALIN_, &b_TOWER_RAW_LG_HCALIN_);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.fUniqueID", TOWER_RAW_LG_HCALIN_fUniqueID, &b_TOWER_RAW_LG_HCALIN_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.fBits", TOWER_RAW_LG_HCALIN_fBits, &b_TOWER_RAW_LG_HCALIN_fBits);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.towerid", TOWER_RAW_LG_HCALIN_towerid, &b_TOWER_RAW_LG_HCALIN_towerid);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.energy", TOWER_RAW_LG_HCALIN_energy, &b_TOWER_RAW_LG_HCALIN_energy);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.time", TOWER_RAW_LG_HCALIN_time, &b_TOWER_RAW_LG_HCALIN_time);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.signal_samples[31]", TOWER_RAW_LG_HCALIN_signal_samples, &b_TOWER_RAW_LG_HCALIN_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.HBD_channel", TOWER_RAW_LG_HCALIN_HBD_channel, &b_TOWER_RAW_LG_HCALIN_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_LG_HCALOUT", &n_TOWER_RAW_LG_HCALOUT, &b_n_TOWER_RAW_LG_HCALOUT);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT", &TOWER_RAW_LG_HCALOUT_, &b_TOWER_RAW_LG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.fUniqueID", TOWER_RAW_LG_HCALOUT_fUniqueID, &b_TOWER_RAW_LG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.fBits", TOWER_RAW_LG_HCALOUT_fBits, &b_TOWER_RAW_LG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.towerid", TOWER_RAW_LG_HCALOUT_towerid, &b_TOWER_RAW_LG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.energy", TOWER_RAW_LG_HCALOUT_energy, &b_TOWER_RAW_LG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.time", TOWER_RAW_LG_HCALOUT_time, &b_TOWER_RAW_LG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.signal_samples[31]", TOWER_RAW_LG_HCALOUT_signal_samples, &b_TOWER_RAW_LG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.HBD_channel", TOWER_RAW_LG_HCALOUT_HBD_channel, &b_TOWER_RAW_LG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HG_HCALOUT", &n_TOWER_RAW_HG_HCALOUT, &b_n_TOWER_RAW_HG_HCALOUT);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT", &TOWER_RAW_HG_HCALOUT_, &b_TOWER_RAW_HG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.fUniqueID", TOWER_RAW_HG_HCALOUT_fUniqueID, &b_TOWER_RAW_HG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.fBits", TOWER_RAW_HG_HCALOUT_fBits, &b_TOWER_RAW_HG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.towerid", TOWER_RAW_HG_HCALOUT_towerid, &b_TOWER_RAW_HG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.energy", TOWER_RAW_HG_HCALOUT_energy, &b_TOWER_RAW_HG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.time", TOWER_RAW_HG_HCALOUT_time, &b_TOWER_RAW_HG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.signal_samples[31]", TOWER_RAW_HG_HCALOUT_signal_samples, &b_TOWER_RAW_HG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.HBD_channel", TOWER_RAW_HG_HCALOUT_HBD_channel, &b_TOWER_RAW_HG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_LG_HCALIN", &n_TOWER_CALIB_LG_HCALIN, &b_n_TOWER_CALIB_LG_HCALIN);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN", &TOWER_CALIB_LG_HCALIN_, &b_TOWER_CALIB_LG_HCALIN_);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.fUniqueID", TOWER_CALIB_LG_HCALIN_fUniqueID, &b_TOWER_CALIB_LG_HCALIN_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.fBits", TOWER_CALIB_LG_HCALIN_fBits, &b_TOWER_CALIB_LG_HCALIN_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.towerid", TOWER_CALIB_LG_HCALIN_towerid, &b_TOWER_CALIB_LG_HCALIN_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.energy", TOWER_CALIB_LG_HCALIN_energy, &b_TOWER_CALIB_LG_HCALIN_energy);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.time", TOWER_CALIB_LG_HCALIN_time, &b_TOWER_CALIB_LG_HCALIN_time);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.signal_samples[31]", TOWER_CALIB_LG_HCALIN_signal_samples, &b_TOWER_CALIB_LG_HCALIN_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.HBD_channel", TOWER_CALIB_LG_HCALIN_HBD_channel, &b_TOWER_CALIB_LG_HCALIN_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_LG_HCALOUT", &n_TOWER_CALIB_LG_HCALOUT, &b_n_TOWER_CALIB_LG_HCALOUT);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT", &TOWER_CALIB_LG_HCALOUT_, &b_TOWER_CALIB_LG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.fUniqueID", TOWER_CALIB_LG_HCALOUT_fUniqueID, &b_TOWER_CALIB_LG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.fBits", TOWER_CALIB_LG_HCALOUT_fBits, &b_TOWER_CALIB_LG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.towerid", TOWER_CALIB_LG_HCALOUT_towerid, &b_TOWER_CALIB_LG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.energy", TOWER_CALIB_LG_HCALOUT_energy, &b_TOWER_CALIB_LG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.time", TOWER_CALIB_LG_HCALOUT_time, &b_TOWER_CALIB_LG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.signal_samples[31]", TOWER_CALIB_LG_HCALOUT_signal_samples, &b_TOWER_CALIB_LG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.HBD_channel", TOWER_CALIB_LG_HCALOUT_HBD_channel, &b_TOWER_CALIB_LG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HG_HCALOUT", &n_TOWER_CALIB_HG_HCALOUT, &b_n_TOWER_CALIB_HG_HCALOUT);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT", &TOWER_CALIB_HG_HCALOUT_, &b_TOWER_CALIB_HG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.fUniqueID", TOWER_CALIB_HG_HCALOUT_fUniqueID, &b_TOWER_CALIB_HG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.fBits", TOWER_CALIB_HG_HCALOUT_fBits, &b_TOWER_CALIB_HG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.towerid", TOWER_CALIB_HG_HCALOUT_towerid, &b_TOWER_CALIB_HG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.energy", TOWER_CALIB_HG_HCALOUT_energy, &b_TOWER_CALIB_HG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.time", TOWER_CALIB_HG_HCALOUT_time, &b_TOWER_CALIB_HG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.signal_samples[31]", TOWER_CALIB_HG_HCALOUT_signal_samples, &b_TOWER_CALIB_HG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.HBD_channel", TOWER_CALIB_HG_HCALOUT_HBD_channel, &b_TOWER_CALIB_HG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_CEMC", &n_TOWER_RAW_CEMC, &b_n_TOWER_RAW_CEMC);
   fChain->SetBranchAddress("TOWER_RAW_CEMC", &TOWER_RAW_CEMC_, &b_TOWER_RAW_CEMC_);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.fUniqueID", TOWER_RAW_CEMC_fUniqueID, &b_TOWER_RAW_CEMC_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.fBits", TOWER_RAW_CEMC_fBits, &b_TOWER_RAW_CEMC_fBits);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.towerid", TOWER_RAW_CEMC_towerid, &b_TOWER_RAW_CEMC_towerid);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.energy", TOWER_RAW_CEMC_energy, &b_TOWER_RAW_CEMC_energy);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.time", TOWER_RAW_CEMC_time, &b_TOWER_RAW_CEMC_time);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.signal_samples[31]", TOWER_RAW_CEMC_signal_samples, &b_TOWER_RAW_CEMC_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.HBD_channel", TOWER_RAW_CEMC_HBD_channel, &b_TOWER_RAW_CEMC_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_CEMC", &n_TOWER_CALIB_CEMC, &b_n_TOWER_CALIB_CEMC);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC", &TOWER_CALIB_CEMC_, &b_TOWER_CALIB_CEMC_);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.fUniqueID", TOWER_CALIB_CEMC_fUniqueID, &b_TOWER_CALIB_CEMC_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.fBits", TOWER_CALIB_CEMC_fBits, &b_TOWER_CALIB_CEMC_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.towerid", TOWER_CALIB_CEMC_towerid, &b_TOWER_CALIB_CEMC_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.energy", TOWER_CALIB_CEMC_energy, &b_TOWER_CALIB_CEMC_energy);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.time", TOWER_CALIB_CEMC_time, &b_TOWER_CALIB_CEMC_time);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.signal_samples[31]", TOWER_CALIB_CEMC_signal_samples, &b_TOWER_CALIB_CEMC_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.HBD_channel", TOWER_CALIB_CEMC_HBD_channel, &b_TOWER_CALIB_CEMC_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HODO_VERTICAL", &n_TOWER_RAW_HODO_VERTICAL, &b_n_TOWER_RAW_HODO_VERTICAL);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL", &TOWER_RAW_HODO_VERTICAL_, &b_TOWER_RAW_HODO_VERTICAL_);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.fUniqueID", TOWER_RAW_HODO_VERTICAL_fUniqueID, &b_TOWER_RAW_HODO_VERTICAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.fBits", TOWER_RAW_HODO_VERTICAL_fBits, &b_TOWER_RAW_HODO_VERTICAL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.towerid", TOWER_RAW_HODO_VERTICAL_towerid, &b_TOWER_RAW_HODO_VERTICAL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.energy", TOWER_RAW_HODO_VERTICAL_energy, &b_TOWER_RAW_HODO_VERTICAL_energy);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.time", TOWER_RAW_HODO_VERTICAL_time, &b_TOWER_RAW_HODO_VERTICAL_time);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.signal_samples[31]", TOWER_RAW_HODO_VERTICAL_signal_samples, &b_TOWER_RAW_HODO_VERTICAL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.HBD_channel", TOWER_RAW_HODO_VERTICAL_HBD_channel, &b_TOWER_RAW_HODO_VERTICAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HODO_HORIZONTAL", &n_TOWER_RAW_HODO_HORIZONTAL, &b_n_TOWER_RAW_HODO_HORIZONTAL);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL", &TOWER_RAW_HODO_HORIZONTAL_, &b_TOWER_RAW_HODO_HORIZONTAL_);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.fUniqueID", TOWER_RAW_HODO_HORIZONTAL_fUniqueID, &b_TOWER_RAW_HODO_HORIZONTAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.fBits", TOWER_RAW_HODO_HORIZONTAL_fBits, &b_TOWER_RAW_HODO_HORIZONTAL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.towerid", TOWER_RAW_HODO_HORIZONTAL_towerid, &b_TOWER_RAW_HODO_HORIZONTAL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.energy", TOWER_RAW_HODO_HORIZONTAL_energy, &b_TOWER_RAW_HODO_HORIZONTAL_energy);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.time", TOWER_RAW_HODO_HORIZONTAL_time, &b_TOWER_RAW_HODO_HORIZONTAL_time);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.signal_samples[31]", TOWER_RAW_HODO_HORIZONTAL_signal_samples, &b_TOWER_RAW_HODO_HORIZONTAL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.HBD_channel", TOWER_RAW_HODO_HORIZONTAL_HBD_channel, &b_TOWER_RAW_HODO_HORIZONTAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HODO_VERTICAL", &n_TOWER_CALIB_HODO_VERTICAL, &b_n_TOWER_CALIB_HODO_VERTICAL);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL", &TOWER_CALIB_HODO_VERTICAL_, &b_TOWER_CALIB_HODO_VERTICAL_);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.fUniqueID", TOWER_CALIB_HODO_VERTICAL_fUniqueID, &b_TOWER_CALIB_HODO_VERTICAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.fBits", TOWER_CALIB_HODO_VERTICAL_fBits, &b_TOWER_CALIB_HODO_VERTICAL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.towerid", TOWER_CALIB_HODO_VERTICAL_towerid, &b_TOWER_CALIB_HODO_VERTICAL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", TOWER_CALIB_HODO_VERTICAL_energy, &b_TOWER_CALIB_HODO_VERTICAL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.time", TOWER_CALIB_HODO_VERTICAL_time, &b_TOWER_CALIB_HODO_VERTICAL_time);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.signal_samples[31]", TOWER_CALIB_HODO_VERTICAL_signal_samples, &b_TOWER_CALIB_HODO_VERTICAL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.HBD_channel", TOWER_CALIB_HODO_VERTICAL_HBD_channel, &b_TOWER_CALIB_HODO_VERTICAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HODO_HORIZONTAL", &n_TOWER_CALIB_HODO_HORIZONTAL, &b_n_TOWER_CALIB_HODO_HORIZONTAL);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL", &TOWER_CALIB_HODO_HORIZONTAL_, &b_TOWER_CALIB_HODO_HORIZONTAL_);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.fUniqueID", TOWER_CALIB_HODO_HORIZONTAL_fUniqueID, &b_TOWER_CALIB_HODO_HORIZONTAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.fBits", TOWER_CALIB_HODO_HORIZONTAL_fBits, &b_TOWER_CALIB_HODO_HORIZONTAL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.towerid", TOWER_CALIB_HODO_HORIZONTAL_towerid, &b_TOWER_CALIB_HODO_HORIZONTAL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", TOWER_CALIB_HODO_HORIZONTAL_energy, &b_TOWER_CALIB_HODO_HORIZONTAL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.time", TOWER_CALIB_HODO_HORIZONTAL_time, &b_TOWER_CALIB_HODO_HORIZONTAL_time);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.signal_samples[31]", TOWER_CALIB_HODO_HORIZONTAL_signal_samples, &b_TOWER_CALIB_HODO_HORIZONTAL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.HBD_channel", TOWER_CALIB_HODO_HORIZONTAL_HBD_channel, &b_TOWER_CALIB_HODO_HORIZONTAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_C1", &n_TOWER_RAW_C1, &b_n_TOWER_RAW_C1);
   fChain->SetBranchAddress("TOWER_RAW_C1", &TOWER_RAW_C1_, &b_TOWER_RAW_C1_);
   fChain->SetBranchAddress("TOWER_RAW_C1.fUniqueID", TOWER_RAW_C1_fUniqueID, &b_TOWER_RAW_C1_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_C1.fBits", TOWER_RAW_C1_fBits, &b_TOWER_RAW_C1_fBits);
   fChain->SetBranchAddress("TOWER_RAW_C1.towerid", TOWER_RAW_C1_towerid, &b_TOWER_RAW_C1_towerid);
   fChain->SetBranchAddress("TOWER_RAW_C1.energy", TOWER_RAW_C1_energy, &b_TOWER_RAW_C1_energy);
   fChain->SetBranchAddress("TOWER_RAW_C1.time", TOWER_RAW_C1_time, &b_TOWER_RAW_C1_time);
   fChain->SetBranchAddress("TOWER_RAW_C1.signal_samples[31]", TOWER_RAW_C1_signal_samples, &b_TOWER_RAW_C1_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_C1.HBD_channel", TOWER_RAW_C1_HBD_channel, &b_TOWER_RAW_C1_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_C1", &n_TOWER_CALIB_C1, &b_n_TOWER_CALIB_C1);
   fChain->SetBranchAddress("TOWER_CALIB_C1", &TOWER_CALIB_C1_, &b_TOWER_CALIB_C1_);
   fChain->SetBranchAddress("TOWER_CALIB_C1.fUniqueID", TOWER_CALIB_C1_fUniqueID, &b_TOWER_CALIB_C1_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_C1.fBits", TOWER_CALIB_C1_fBits, &b_TOWER_CALIB_C1_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_C1.towerid", TOWER_CALIB_C1_towerid, &b_TOWER_CALIB_C1_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_C1.energy", TOWER_CALIB_C1_energy, &b_TOWER_CALIB_C1_energy);
   fChain->SetBranchAddress("TOWER_CALIB_C1.time", TOWER_CALIB_C1_time, &b_TOWER_CALIB_C1_time);
   fChain->SetBranchAddress("TOWER_CALIB_C1.signal_samples[31]", TOWER_CALIB_C1_signal_samples, &b_TOWER_CALIB_C1_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_C1.HBD_channel", TOWER_CALIB_C1_HBD_channel, &b_TOWER_CALIB_C1_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_C2", &n_TOWER_RAW_C2, &b_n_TOWER_RAW_C2);
   fChain->SetBranchAddress("TOWER_RAW_C2", &TOWER_RAW_C2_, &b_TOWER_RAW_C2_);
   fChain->SetBranchAddress("TOWER_RAW_C2.fUniqueID", TOWER_RAW_C2_fUniqueID, &b_TOWER_RAW_C2_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_C2.fBits", TOWER_RAW_C2_fBits, &b_TOWER_RAW_C2_fBits);
   fChain->SetBranchAddress("TOWER_RAW_C2.towerid", TOWER_RAW_C2_towerid, &b_TOWER_RAW_C2_towerid);
   fChain->SetBranchAddress("TOWER_RAW_C2.energy", TOWER_RAW_C2_energy, &b_TOWER_RAW_C2_energy);
   fChain->SetBranchAddress("TOWER_RAW_C2.time", TOWER_RAW_C2_time, &b_TOWER_RAW_C2_time);
   fChain->SetBranchAddress("TOWER_RAW_C2.signal_samples[31]", TOWER_RAW_C2_signal_samples, &b_TOWER_RAW_C2_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_C2.HBD_channel", TOWER_RAW_C2_HBD_channel, &b_TOWER_RAW_C2_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_C2", &n_TOWER_CALIB_C2, &b_n_TOWER_CALIB_C2);
   fChain->SetBranchAddress("TOWER_CALIB_C2", &TOWER_CALIB_C2_, &b_TOWER_CALIB_C2_);
   fChain->SetBranchAddress("TOWER_CALIB_C2.fUniqueID", TOWER_CALIB_C2_fUniqueID, &b_TOWER_CALIB_C2_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_C2.fBits", TOWER_CALIB_C2_fBits, &b_TOWER_CALIB_C2_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_C2.towerid", TOWER_CALIB_C2_towerid, &b_TOWER_CALIB_C2_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_C2.energy", TOWER_CALIB_C2_energy, &b_TOWER_CALIB_C2_energy);
   fChain->SetBranchAddress("TOWER_CALIB_C2.time", TOWER_CALIB_C2_time, &b_TOWER_CALIB_C2_time);
   fChain->SetBranchAddress("TOWER_CALIB_C2.signal_samples[31]", TOWER_CALIB_C2_signal_samples, &b_TOWER_CALIB_C2_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_C2.HBD_channel", TOWER_CALIB_C2_HBD_channel, &b_TOWER_CALIB_C2_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_PbGL", &n_TOWER_RAW_PbGL, &b_n_TOWER_RAW_PbGL);
   fChain->SetBranchAddress("TOWER_RAW_PbGL", &TOWER_RAW_PbGL_, &b_TOWER_RAW_PbGL_);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.fUniqueID", TOWER_RAW_PbGL_fUniqueID, &b_TOWER_RAW_PbGL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.fBits", TOWER_RAW_PbGL_fBits, &b_TOWER_RAW_PbGL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.towerid", TOWER_RAW_PbGL_towerid, &b_TOWER_RAW_PbGL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.energy", TOWER_RAW_PbGL_energy, &b_TOWER_RAW_PbGL_energy);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.time", TOWER_RAW_PbGL_time, &b_TOWER_RAW_PbGL_time);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.signal_samples[31]", TOWER_RAW_PbGL_signal_samples, &b_TOWER_RAW_PbGL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.HBD_channel", TOWER_RAW_PbGL_HBD_channel, &b_TOWER_RAW_PbGL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_PbGL", &n_TOWER_CALIB_PbGL, &b_n_TOWER_CALIB_PbGL);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL", &TOWER_CALIB_PbGL_, &b_TOWER_CALIB_PbGL_);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.fUniqueID", TOWER_CALIB_PbGL_fUniqueID, &b_TOWER_CALIB_PbGL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.fBits", TOWER_CALIB_PbGL_fBits, &b_TOWER_CALIB_PbGL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.towerid", TOWER_CALIB_PbGL_towerid, &b_TOWER_CALIB_PbGL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.energy", TOWER_CALIB_PbGL_energy, &b_TOWER_CALIB_PbGL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.time", TOWER_CALIB_PbGL_time, &b_TOWER_CALIB_PbGL_time);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.signal_samples[31]", TOWER_CALIB_PbGL_signal_samples, &b_TOWER_CALIB_PbGL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.HBD_channel", TOWER_CALIB_PbGL_HBD_channel, &b_TOWER_CALIB_PbGL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_TRIGGER_VETO", &n_TOWER_RAW_TRIGGER_VETO, &b_n_TOWER_RAW_TRIGGER_VETO);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO", &TOWER_RAW_TRIGGER_VETO_, &b_TOWER_RAW_TRIGGER_VETO_);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.fUniqueID", TOWER_RAW_TRIGGER_VETO_fUniqueID, &b_TOWER_RAW_TRIGGER_VETO_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.fBits", TOWER_RAW_TRIGGER_VETO_fBits, &b_TOWER_RAW_TRIGGER_VETO_fBits);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.towerid", TOWER_RAW_TRIGGER_VETO_towerid, &b_TOWER_RAW_TRIGGER_VETO_towerid);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.energy", TOWER_RAW_TRIGGER_VETO_energy, &b_TOWER_RAW_TRIGGER_VETO_energy);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.time", TOWER_RAW_TRIGGER_VETO_time, &b_TOWER_RAW_TRIGGER_VETO_time);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.signal_samples[31]", TOWER_RAW_TRIGGER_VETO_signal_samples, &b_TOWER_RAW_TRIGGER_VETO_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.HBD_channel", TOWER_RAW_TRIGGER_VETO_HBD_channel, &b_TOWER_RAW_TRIGGER_VETO_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_TRIGGER_VETO", &n_TOWER_CALIB_TRIGGER_VETO, &b_n_TOWER_CALIB_TRIGGER_VETO);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO", &TOWER_CALIB_TRIGGER_VETO_, &b_TOWER_CALIB_TRIGGER_VETO_);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.fUniqueID", TOWER_CALIB_TRIGGER_VETO_fUniqueID, &b_TOWER_CALIB_TRIGGER_VETO_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.fBits", TOWER_CALIB_TRIGGER_VETO_fBits, &b_TOWER_CALIB_TRIGGER_VETO_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.towerid", TOWER_CALIB_TRIGGER_VETO_towerid, &b_TOWER_CALIB_TRIGGER_VETO_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", TOWER_CALIB_TRIGGER_VETO_energy, &b_TOWER_CALIB_TRIGGER_VETO_energy);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.time", TOWER_CALIB_TRIGGER_VETO_time, &b_TOWER_CALIB_TRIGGER_VETO_time);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.signal_samples[31]", TOWER_CALIB_TRIGGER_VETO_signal_samples, &b_TOWER_CALIB_TRIGGER_VETO_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.HBD_channel", TOWER_CALIB_TRIGGER_VETO_HBD_channel, &b_TOWER_CALIB_TRIGGER_VETO_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_TILE_MAPPER", &n_TOWER_RAW_TILE_MAPPER, &b_n_TOWER_RAW_TILE_MAPPER);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER", &TOWER_RAW_TILE_MAPPER_, &b_TOWER_RAW_TILE_MAPPER_);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.fUniqueID", TOWER_RAW_TILE_MAPPER_fUniqueID, &b_TOWER_RAW_TILE_MAPPER_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.fBits", TOWER_RAW_TILE_MAPPER_fBits, &b_TOWER_RAW_TILE_MAPPER_fBits);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.towerid", TOWER_RAW_TILE_MAPPER_towerid, &b_TOWER_RAW_TILE_MAPPER_towerid);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.energy", TOWER_RAW_TILE_MAPPER_energy, &b_TOWER_RAW_TILE_MAPPER_energy);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.time", TOWER_RAW_TILE_MAPPER_time, &b_TOWER_RAW_TILE_MAPPER_time);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.signal_samples[31]", TOWER_RAW_TILE_MAPPER_signal_samples, &b_TOWER_RAW_TILE_MAPPER_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.HBD_channel", TOWER_RAW_TILE_MAPPER_HBD_channel, &b_TOWER_RAW_TILE_MAPPER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_TILE_MAPPER", &n_TOWER_CALIB_TILE_MAPPER, &b_n_TOWER_CALIB_TILE_MAPPER);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER", &TOWER_CALIB_TILE_MAPPER_, &b_TOWER_CALIB_TILE_MAPPER_);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.fUniqueID", TOWER_CALIB_TILE_MAPPER_fUniqueID, &b_TOWER_CALIB_TILE_MAPPER_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.fBits", TOWER_CALIB_TILE_MAPPER_fBits, &b_TOWER_CALIB_TILE_MAPPER_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.towerid", TOWER_CALIB_TILE_MAPPER_towerid, &b_TOWER_CALIB_TILE_MAPPER_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.energy", TOWER_CALIB_TILE_MAPPER_energy, &b_TOWER_CALIB_TILE_MAPPER_energy);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.time", TOWER_CALIB_TILE_MAPPER_time, &b_TOWER_CALIB_TILE_MAPPER_time);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.signal_samples[31]", TOWER_CALIB_TILE_MAPPER_signal_samples, &b_TOWER_CALIB_TILE_MAPPER_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.HBD_channel", TOWER_CALIB_TILE_MAPPER_HBD_channel, &b_TOWER_CALIB_TILE_MAPPER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_SC3", &n_TOWER_RAW_SC3, &b_n_TOWER_RAW_SC3);
   fChain->SetBranchAddress("TOWER_RAW_SC3", &TOWER_RAW_SC3_, &b_TOWER_RAW_SC3_);
   fChain->SetBranchAddress("TOWER_RAW_SC3.fUniqueID", TOWER_RAW_SC3_fUniqueID, &b_TOWER_RAW_SC3_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_SC3.fBits", TOWER_RAW_SC3_fBits, &b_TOWER_RAW_SC3_fBits);
   fChain->SetBranchAddress("TOWER_RAW_SC3.towerid", TOWER_RAW_SC3_towerid, &b_TOWER_RAW_SC3_towerid);
   fChain->SetBranchAddress("TOWER_RAW_SC3.energy", TOWER_RAW_SC3_energy, &b_TOWER_RAW_SC3_energy);
   fChain->SetBranchAddress("TOWER_RAW_SC3.time", TOWER_RAW_SC3_time, &b_TOWER_RAW_SC3_time);
   fChain->SetBranchAddress("TOWER_RAW_SC3.signal_samples[31]", TOWER_RAW_SC3_signal_samples, &b_TOWER_RAW_SC3_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_SC3.HBD_channel", TOWER_RAW_SC3_HBD_channel, &b_TOWER_RAW_SC3_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_SC3", &n_TOWER_CALIB_SC3, &b_n_TOWER_CALIB_SC3);
   fChain->SetBranchAddress("TOWER_CALIB_SC3", &TOWER_CALIB_SC3_, &b_TOWER_CALIB_SC3_);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.fUniqueID", TOWER_CALIB_SC3_fUniqueID, &b_TOWER_CALIB_SC3_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.fBits", TOWER_CALIB_SC3_fBits, &b_TOWER_CALIB_SC3_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.towerid", TOWER_CALIB_SC3_towerid, &b_TOWER_CALIB_SC3_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.energy", TOWER_CALIB_SC3_energy, &b_TOWER_CALIB_SC3_energy);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.time", TOWER_CALIB_SC3_time, &b_TOWER_CALIB_SC3_time);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.signal_samples[31]", TOWER_CALIB_SC3_signal_samples, &b_TOWER_CALIB_SC3_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.HBD_channel", TOWER_CALIB_SC3_HBD_channel, &b_TOWER_CALIB_SC3_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_FINGER_COUNTER", &n_TOWER_RAW_FINGER_COUNTER, &b_n_TOWER_RAW_FINGER_COUNTER);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER", &TOWER_RAW_FINGER_COUNTER_, &b_TOWER_RAW_FINGER_COUNTER_);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.fUniqueID", TOWER_RAW_FINGER_COUNTER_fUniqueID, &b_TOWER_RAW_FINGER_COUNTER_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.fBits", TOWER_RAW_FINGER_COUNTER_fBits, &b_TOWER_RAW_FINGER_COUNTER_fBits);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.towerid", TOWER_RAW_FINGER_COUNTER_towerid, &b_TOWER_RAW_FINGER_COUNTER_towerid);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.energy", TOWER_RAW_FINGER_COUNTER_energy, &b_TOWER_RAW_FINGER_COUNTER_energy);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.time", TOWER_RAW_FINGER_COUNTER_time, &b_TOWER_RAW_FINGER_COUNTER_time);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.signal_samples[31]", TOWER_RAW_FINGER_COUNTER_signal_samples, &b_TOWER_RAW_FINGER_COUNTER_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.HBD_channel", TOWER_RAW_FINGER_COUNTER_HBD_channel, &b_TOWER_RAW_FINGER_COUNTER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_FINGER_COUNTER", &n_TOWER_CALIB_FINGER_COUNTER, &b_n_TOWER_CALIB_FINGER_COUNTER);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER", &TOWER_CALIB_FINGER_COUNTER_, &b_TOWER_CALIB_FINGER_COUNTER_);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.fUniqueID", TOWER_CALIB_FINGER_COUNTER_fUniqueID, &b_TOWER_CALIB_FINGER_COUNTER_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.fBits", TOWER_CALIB_FINGER_COUNTER_fBits, &b_TOWER_CALIB_FINGER_COUNTER_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.towerid", TOWER_CALIB_FINGER_COUNTER_towerid, &b_TOWER_CALIB_FINGER_COUNTER_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.energy", TOWER_CALIB_FINGER_COUNTER_energy, &b_TOWER_CALIB_FINGER_COUNTER_energy);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.time", TOWER_CALIB_FINGER_COUNTER_time, &b_TOWER_CALIB_FINGER_COUNTER_time);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.signal_samples[31]", TOWER_CALIB_FINGER_COUNTER_signal_samples, &b_TOWER_CALIB_FINGER_COUNTER_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.HBD_channel", TOWER_CALIB_FINGER_COUNTER_HBD_channel, &b_TOWER_CALIB_FINGER_COUNTER_HBD_channel);
   Notify();
}

Bool_t DSTReader551::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DSTReader551::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DSTReader551::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
void DSTReader551::Loop()
{
//   In a ROOT session, you can do:
//      root> .L DSTReader551.C
//      root> DSTReader551 t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   OfficalBeamData *tally = new OfficalBeamData("data551",runToVoltage(511),runToEnergy(511));
   Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      tally->add(TOWER_CALIB_C2_energy[1],TOWER_CALIB_TRIGGER_VETO_energy,TOWER_CALIB_HODO_HORIZONTAL_energy,TOWER_CALIB_HODO_VERTICAL_energy,TOWER_CALIB_PbGL_energy[0]);
      if (TOWER_CALIB_C2_energy[1]>1000)
      {
      	/*cout<<"Event"<<jentry<<'\n';
      	cout<<"C:"<<TOWER_CALIB_C2_energy[1]<<'\n';
      	cout<<"V:"<<TOWER_CALIB_TRIGGER_VETO_energy[0]<<','<<TOWER_CALIB_TRIGGER_VETO_energy[1]<<','<<TOWER_CALIB_TRIGGER_VETO_energy[2]<<','<<TOWER_CALIB_TRIGGER_VETO_energy[3]<<'\n';
      	cout<<"Hh:"<<TOWER_CALIB_HODO_HORIZONTAL_energy[0]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[1]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[2]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[3]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[4]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[5]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[6]<<','<<TOWER_CALIB_HODO_HORIZONTAL_energy[7]<<'\n';
      	cout<<"Hv:"<<TOWER_CALIB_HODO_VERTICAL_energy[0]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[1]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[2]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[3]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[4]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[5]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[6]<<','<<TOWER_CALIB_HODO_VERTICAL_energy[7]<<'\n';
      	cout<<"PbGl"<<TOWER_CALIB_PbGL_energy[0]<<'\n';
      	cout<<'\n';*/
      }
      
      if(jentry%10000==0) cout<<jentry<<" events entered"<<'\n';
    }
    tally->plot();
}

int* runToEnergy(int* runs, int SIZE){
	int *energies = new int[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		energies[i] = runToEnergy(runs[i]);
	}
	return energies;
}
int* runToVoltage(int* runs, int SIZE){
	int *voltages = new int[SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		voltages[i] = runToVoltage(runs[i]);
	}
	return &voltages[0];
}
/*
void drawTrend(TGraphErrors* mean, int nMeanBins, float* energies){
	TCanvas *canvas1 = new TCanvas();
	TF1* lin = new TF1("lin","[0]*x",0,energies[nMeanBins-1]);
	TF1* poly = new TF1("poly","[1]*x*x+[0]*x",0,energies[nMeanBins-1]);
	axisTitles(mean,"Beam Energy GeV","Mean #Delta ADC");
	gNice();
	mean->Fit(poly);
	double nonLinearFactor = poly->GetParameter(1);
	double nonLinearError = poly->GetParError(1);
	float chi2 = poly->GetChisquare();
	mean->Fit(lin,"0");
	lin->SetLineColor(kRed);
	double linearFactor = lin->GetParameter(0);
	double linearError = lin->GetParError(0);
	float chi = lin->GetChisquare();
	int ndf = lin->GetNDF();
	mean->SetMarkerStyle(kOpenCircle);
	mean->Draw("AP");
	poly->SetLineColor(kBlue);
	poly->Draw("same");
	lin->Draw("same");
}

void drawresolution(const int nMeanBins,float*meanBins, TGraphErrors *graph){
	TCanvas *canvas1 = new TCanvas();
	TF1* eF = new TF1("eF","TMath::Sqrt([0]*[0]/x+[1]*[1])",0,meanBins[nMeanBins-1]);
	eF->SetLineColor(kRed);
	graph->Fit(eF);
	float eA = eF->GetParameter(0);
	float eB = eF->GetParameter(1);
	float errors[2];
	errors[0] = eF->GetParError(0);
	errors[1] = eF->GetParError(1);
	//makeMarkerNice(&ehist, 1);
	graph->SetMarkerSize(2);
	graph->SetMinimum(0);
	graph->SetMaximum(0.1);
	graph->Draw("AP*");
	axisTitles(graph,"Beam Energy GeV","#sigma/mean");
	float chi = eF->GetChisquare();
	int ndf = eF->GetNDF();
}
*/


/*void superArraySorter5000(float* energies, float* mean, float* meanError, float* sigma, float* sigmaError,int SIZE)
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
}*/

void Part2A(){ // only put one voltage in for now 
	cout<<"Start"<<endl;
	string fileLocation = "";
	string filename = "beam_00000";
	string extension = "-0000_DSTReader.root";
	filename = fileLocation+filename;
	//const int NUMSIZE=18;
	//int numbers[] = {551,558,563,567,573,652,653,654,776,777,809,810,816,829,830,849,859,900};
	const int NUMSIZE=1;
	int numbers[] = {551};
	//int* voltages = runToVoltage(numbers,NUMSIZE); //double check that these are right
	//int* energies = runToEnergy(numbers,NUMSIZE);
	TFile *file;
	DSTReader551 *reader;
	stringstream ss;
	float resolution[NUMSIZE];
	float mean[NUMSIZE];
	float resolutionU[NUMSIZE];
	float meanU[NUMSIZE];
	TGraphErrors *stochastic;
	TGraphErrors *trend;
	for (int i = 0; i < NUMSIZE; ++i)
	{
		ss<<numbers[i];
		fileLocation = filename+ss.str()+extension;
		file = new TFile(fileLocation.c_str()); //mem leak
		TTree *orange= (TTree*) file->Get("T");
		reader = new DSTReader551(orange); //mem leak 
		reader->Loop();
		ss.clear();
		//ss<<"beamdata"<<numbers[i];
		//OfficalBeamData data = OfficalBeamData(tree,voltages[i],energies[i],ss.str());
		/*resolution[i] = data.getResolution();
		mean[i] = data.getMean();
		resolutionU[i] = data.getResolutionUncertainty();
		meanU[i] = data.getMeanUncertainty();*/
		//cout<<data[i].getMean()<<endl;
		//ss.clear();
		file->Close();
	}
	/*superArraySorter5000(energies,mean,meanU,resolution,resolutionU,NUMSIZE);
	float temp;
	trend = new TGraphErrors(NUMSIZE,energies,mean,zeroArray(NUMSIZE,&temp),meanU);
	resolution = new TGraphErrors(NUMSIZE,energies,resolution,zeroArray(NUMSIZE,&temp),resolutionU);*/
	cout<<"End"<<std::endl;
}



/*
Double_t cerenkovEnergies[3]; // only need the [1] value 
	 	reader->SetBranchAddress("TOWER_CALIB_C2.energy", cerenkovEnergies);
	 	Double_t vetoEnergy[4]; //need all the values 
	 	reader->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", vetoEnergy);
	 	Double_t hodoVerticalEnergy[8];
		reader->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", hodoVerticalEnergy);
		Double_t hodoHorizontalEnergy[8];
		reader->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", hodoHorizontalEnergy);
		Double_t pbglTemp[1];
		reader->SetBranchAddress("TOWER_CALIB_PbGL.energy", pbglTemp);
		cout<<orange->GetEntries()<<endl;
*/


int runToEnergy(int run){
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

int runToVoltage(int run){
    int r;
    int s = (int) run;
    switch (s){
        case 558:
            r=1100;
            break;
        case 551:
            r= 1100;
            break;
        case 563:
            r= 1100;
            break;
        case 573:
            r= 1100;
            break;
        case 567:
            r= 1100;
            break;
        case 776:
        	r=1200;
        	break;
    	case 777:
    		r=1200;
    		break;
		case 809:
			r=1200;
			break;
		case 810:
			r=1200;
			break;
		case 816:
			r=1200;
			break;
		case 829:
			r=1200;
			break;
		case 830:
			r=1200;
			break;
		case 849:
			r=1200;
			break;
		case 859:
			r=1200;
			break;
		case 900:
			r=1200;
			break;
		case 631:
			r=1100;
			break;
		case 544:
			r= 1100;
			break;
		case 652:
			r= 1100;
			break;
		case 653:
			r= 1100;
			break;
		case 654:
			r= 1100;
			break;
		case 687:
			r= 1100;
			break;
		case 572:
			r= 1100;
			break;
		case 574:
			r= 1100;
			break;
		case 577:
			r= 1100;
			break;
		case 578:
			r= 1100;
			break;
		case 579:
			r= 1100;
			break;	
		case 580:
			r=1100;
			break;
        default:
            r=-1;
            break;
    }
    return r;
}