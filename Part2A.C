#include <sstream>
#include <queue>
#include "TH1F.h"
#include "TChain.h"
#include <iostream>
#include "TF1.h"
//#include "NiceHists.C"
using namespace std;

namespace {
	int plotcount=0;
}

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


#ifndef OfficalBeamData_h
#define OfficalBeamData_h 
class OfficalBeamData
{
public:
	OfficalBeamData(){}
	OfficalBeamData(TChain *orange, int beamVoltage, int beamEnergy,string name) : SIZE(orange->GetEntries()), beamVoltage(beamVoltage), beamEnergy(beamEnergy){ 
		if (SIZE==0)
		{
			cout<<"Error Data Size is 0"<<endl;
		}
		else{
			TH1F *pbglPlot = new TH1F(name.c_str(),"",100,0,20);

			float cerenkovEnergies[3]; // only need the [1] value 
	 		orange->SetBranchAddress("TOWER_CALIB_C2.energy", &cerenkovEnergies);
	 		float vetoEnergy[4]; //need all the values 
	 		orange->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", &vetoEnergy);
	 		float hodoVerticalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", &hodoVerticalEnergy);
			float hodoHorizontalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", &hodoHorizontalEnergy);
			float pbglTemp;
			orange->SetBranchAddress("TOWER_CALIB_PbGL.energy", &pbglTemp);
			for (int i = 0; i < SIZE; ++i)
			{
				if(i%1000==0) cout<<i<<" events processed"<<'\n';
				orange->GetEntry(i);
				if (passCuts(cerenkovEnergies[1],vetoEnergy,hodoVerticalEnergy,hodoHorizontalEnergy))
				{
					pbglEnergy.push(pbglTemp);
					pbglPlot->Fill(pbglTemp);
					cout<<pbglTemp<<'\n';
				}	
			}
			pbglPlot->Sumw2();
		}
	}
	~OfficalBeamData(){}
	inline bool passCuts(float cerenkov, float* veto, float* vhodo, float* hhodo){
	//	return cerenkov>CERENKOVcut && noVeto(veto),passHodo(vhodo),passHodo(hhodo);
		return true;
	}
	inline bool noVeto(float* veto){
		return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<VETOcut;
	}
	inline bool passHodo(float* hodo){
		return hodo[0]>HODOcut && hodo[1]>HODOcut && hodo[2]>HODOcut && hodo[3]>HODOcut && hodo[4]>HODOcut && hodo[5]>HODOcut && hodo[6]>HODOcut && hodo[7]>HODOcut;
	}
	OfficalBeamData& operator=(OfficalBeamData other){
		pbglEnergy=other.pbglEnergy;
		beamVoltage=other.beamVoltage;
		pbglPlot=other.pbglPlot;
		return *this;
	}
	double makeGaus(){
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

void Part2A(){
	cout<<"Start"<<endl;
	string fileLocation = "";
	string filename = "beam_00000";
	string extension = "-0000_DSTReader.root";
	filename = fileLocation+filename;
	//const int NUMSIZE=18;
	//int numbers[] = {551,558,563,567,573,652,653,654,776,777,809,810,816,829,830,849,859,900};
	const int NUMSIZE=1;
	int numbers[] = {551};
	int* voltages = runToVoltage(numbers,NUMSIZE); //double check that these are right
	int* energies = runToEnergy(numbers,NUMSIZE);
	OfficalBeamData *data= new OfficalBeamData[NUMSIZE];
	TChain *all; 
	stringstream ss;
	for (int i = 0; i < NUMSIZE; ++i)
	{
		all= new TChain("T");
		ss<<numbers[i];
		fileLocation = filename+ss.str()+extension;
		all->Add(fileLocation.c_str());
		ss.clear();
		ss<<"beamdata"<<numbers[i];
		data[i] = OfficalBeamData(all,voltages[i],energies[i],ss.str());
		cout<<data[i].getMean()<<endl;
		ss.clear();
		delete all;
	}
	
	cout<<"End"<<std::endl;
}
