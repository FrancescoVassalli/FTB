#include <sstream>
#include <iostream>

using namespace std;

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
	OfficalBeamData(TChain *orange, int* beamVoltage) : SIZE(orange->GetEntries()){ 
		if (SIZE==0)
		{
			cout<<"Error Data Size is 0"<<endl;
		}
		else{
			double cerenkovEnergies[3]; // only need the [1] value 
	 		orange->SetBranchAddress("TOWER_CALIB_C2.energy", &cerenkovEnergies);
	 		double vetoEnergy[4]; //need all the values 
	 		orange->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", &vetoEnergy);
	 		double hodoVerticalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", &hodoVerticalEnergy);
			double hodoHorizontalEnergy[8];
			orange->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", &hodoHorizontalEnergy);
			double pbglTemp;
			orange->SetBranchAddress("TOWER_CALIB_PbGL.energy", &pbglTemp);
			for (int i = 0; i < SIZE; ++i)
			{
				orange->GetEntry(i);
				this->beamVoltage.push(beamVoltage[i]);
				if (passCuts(cerenkovEnergies[1],vetoEnergy,hodoVerticalEnergy,hodoHorizontalEnergy))
				{
					pbglEnergy.push(pbglTemp);
				}	
			}
		}
	}
	~OfficalBeamData(){}
	inline bool passCuts(double cerenkov, double* veto, double* vhodo, double* hhodo){
		return cerenkov>CERENKOVcut && noVeto(veto),passHodo(vhodo),passHodo(hhodo);
	}
	inline bool noVeto(double* veto){
		return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<VETOcut;
	}
	inline bool passHodo(double* hodo){
		return hodo[0]<HODOcut && hodo[1]<HODOcut && hodo[2]<HODOcut && hodo[3]<HODOcut && hodo[4]<HODOcut && hodo[5]<HODOcut && hodo[6]<HODOcut && hodo[7]<HODOcut;
	}
		
private:
	const int SIZE;

	//we will need to play with these values
	const double CERENKOVcut = 1000;
	const float VETOcut = .3;
	const float HODOcut = .4;

	queue<double> pbglEnergy;
	queue<int> beamVoltage;
};
#endif

void Part2A(){
	string fileLocation = "";
	string filename = "beam_00000";
	string extension = "-0000.root";
	filename = fileLocation+filename;
	const int NUMSIZE=18;
	int numbers[] = {551,558,563,567,573,652,653,654,776,777,809,810,816,829,830,849,859,900};
	int* voltages = runToVoltage(numbers,NUMSIZE); //double check that these are right
	TChain *all = new TChain("T1");
	stringstream ss;
	for (int i = 0; i < NUMSIZE; ++i)
	{
		ss<<numbers[i];
		fileLocation = filename+ss.str()+extension;
		all->Add(fileLocation.c_str());
		ss.clear();
	}
//	OfficalBeamData data(all,voltages);
}
