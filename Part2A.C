// Made by Francesco Vassalli for analysis of PbGl detector for sPHENIX EMCal calibration effort
// 6/27/18 at the CU Boulder
// This script takes beam data files in DSTReader format and plots the PbGl energy after making 
// hodoscope and cherenkov radiation cuts. It then fits a gaussian to the energy to get 
// mean energy as well as the sigma on the energies gaussian distribution.
#include "NicePlots.C"
#include <Scalar.h>

#include "TClonesArray.h"
#include "TObject.h"
#include "TH1D.h"
#include "TChain.h"
#include "TF1.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <sstream>
#include <queue>
#include <iostream>
// Header file for the classes stored in the TTree if any.

//#include "/Users/Chase/Documents/HeavyIonsResearch/FranTools/Bin/NiceHists.C" //for chase
void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {
  	TLatex l; //l.SetTextAlign(12); 
  	l.SetTextSize(tsize); 
  	l.SetNDC();
 	l.SetTextColor(color);
  	l.DrawLatex(x,y,text);
}
	template<class T>
void oleSwitcheroo(T* xp, T* yp)
{
	T temp = *xp;
	*xp = *yp;
	*yp = temp;
}

/*a 5 parameter function that combines a gaussian and a power law 
	p0: alpha - the point where the gaussian becomes a power law 
	p1: n - the power on the power law 
	p2: mu - the pseudomean 
	p3: sigma - the width on the gaussian
	p4: N -normalization */
double franCrystalBall(double* x, double* p){
		//factors of the CB
		double deviation = -1*(x[0]-p[2])/p[3];
		double A=TMath::Power(p[1]/TMath::Abs(p[0]),p[1])*TMath::Exp(-1.0*p[0]*p[0]/2.0);
		double B=p[1]/TMath::Abs(p[0])-TMath::Abs(p[0]);
		
		if (deviation>-1*p[0])
		{
			return p[4]*TMath::Exp(-1.0*(x[0]-p[2])*(x[0]-p[2])/(2*p[3]*p[3]));
		}
		else{
			return p[4]*A*TMath::Power(B-deviation,-1.0*p[1]);
		}
}

using namespace std;

namespace {
	int plotcount=0;
}

int runToVoltage(int run);
int runToEnergy(int run);
//classes from NiceHist
 
class PlotWithLine
{
public:
	virtual void Draw(){
		main->Draw();
	}
	~PlotWithLine(){ // there might be some mem leakage here 
	}
	virtual TH1* get_main(){
		return main;
	}

protected:
	TH1 *main;
	
};

class CutPlot :public PlotWithLine{
public:
	CutPlot(TH1 *main, TLine* cut) : cut(cut){
		this->main  = main;
	}
	~CutPlot(){
		delete cut;
		cut=NULL;
	}
	void Draw(){
		main->Draw();
		cut->Draw("same");
	}
private:
	TLine *cut;
};

class GausPlot :public PlotWithLine
{
public:
	GausPlot(TH1* main, TF1* gaus,double lowBound,double upBound) : gaus(gaus),lowBound(lowBound), upBound(upBound){
		this->main  = main;
	}
	~GausPlot(){
		delete gaus;
		gaus=NULL;
	}
	void Draw(){
		main->Draw();
		gaus->Draw("same");
	}
	TF1* get_gaus(){
		return gaus;
	}
	double getUpBound(){
		return upBound;
	}

private:
	TF1 *gaus;
	double lowBound;
	double upBound;
};

class RunSelecTOR
{
public:
	RunSelecTOR(bool newData, bool checkVoltage, int voltage){
		this->voltage=voltage;
		this->newData=newData;
		this->checkVoltage=checkVoltage;
	}
	~RunSelecTOR(){}

 int* operator()(int *runNumber, int* SIZE){
 	int *r = new int[*SIZE];
 	int j=0;
 	if (newData)
 	{
 		for (int i = 0; i < *SIZE; ++i)
 		{
 			if ((!checkVoltage||runToVoltage(runNumber[i])==voltage)&&runNumber[i]>1000)
 			{
 				r[j] = runNumber[i];
 				j++;
 			}
 		}
 	}
 	else{
 		for (int i = 0; i < *SIZE; ++i)
 		{
 			if ((!checkVoltage||runToVoltage(runNumber[i])==voltage)&&runNumber[i]<1000)
 			{
 				r[j] = runNumber[i];
 				j++;
 			}
 		}
 	}
 	*SIZE = j;
 	return r;
 }
 std::string getName(){
 	if (newData)
 	{
 		return "PbGl"+std::to_string(voltage)+"new.txt";
 	}
 	else{
 		return "PbGl"+std::to_string(voltage)+".txt";
 	}
 }


private:
	int voltage;
	bool newData;
	bool checkVoltage;
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
		case 632:
			r=12;
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
		case 1876:
			r=8; break;
		case 1879:
			r=8; break;
		case 1882:
			r=8; break;
		case 1883:
			r=8; break;
		case 1888:
			r=8; break;
		case 1890:
			r=6; break;
		case 2128:
			r=16; break;
		case 2125:
			r=12; break;
		case 2126:
			r=12; break;
		case 2127:
			r=16; break;
		case 1889:
			r=6; break;
		case 1901:
			r=4; break;
		case 1902:
			r=4; break;
		case 1904:
			r=2; break;
		case 1906:
			r=2; break;
		case 1924:
			r=12; break;
		case 1925:
			r=12; break;
		case 1943:
			r=24; break;
		case 1945:
			r=28; break;
		case 2073:
			r=3; break;
		case 2074:
			r=4; break;
		case 2094:
			r=6; break;
		case 2095:
			r=6; break;
		case 2097:
			r=8; break;
		case 2098:
			r=8; break;
		case 2149:
			r=20; break;
		case 2150:
			r=24; break;
		case 2167:
			r=28; break;
		case 773:
			r=10;
			break;
		case 772:
			r=10;
			break;
		case 1926:
			r=16; 
			break;
		case 2045:
			r=2; 
			break;
        default:
            r=-1;
            cout<<"Error in runToEnergy line:"<<__LINE__<<std::endl;
            break;
    }
    return r;
}

int runToVoltage(int run){
    int r;
    int s = (int) run;

    switch (s){
    	case 2045:
			r=1200; 
			break;
    	case 1926:
			r=1100; 
			break;
    	case 2167:
			r=1000; 
			break;
    	case 1945:
			r=1000; 
			break;
    	case 2150:
			r=1000;
			break;
    	case 1943:
			r=1000;
			break;
    	case 2149:
			r=1000;
			break;
    	case 1924:
			r=1200;
			break;
    	case 1925:
			r=1100;
			break;
    	case 2125:
			r=1200;
			break;
    	case 2126:
			r=1100;
			break;
    	case 632:
			r=1100;
			break;
    	case 773:
			r=-1;
			break;
		case 772:
			r=-1;
			break;
    	case 1901:
            r=1200;
            break;
        case 2074:
            r=1200;
            break;
        case 2094:
            r=1200;
            break;
        case 2095:
            r=1100;
            break;
        case 1889:
            r=1100;
            break;
        case 2127:
            r=1100;
            break;
        case 2128: // probably saturated 
            r=1200;
            break;
        case 2098:
            r=1200;
            break;
        case 1883:
            r=1200;
            break;
        case 1882:
            r=1200;
            break;
        case 1879:
            r=1200;
            break;
        case 1906:
            r=1200;
            break;
        case 900: //may need to test this at 1100V 
            r=1200;
            break;
        case 1904:
            r=1100;
            break;
        case 2073:
            r=1200;
            break;
        case 2097:
            r=1100;
            break;
        case 1888:
            r=1100;
            break;
         case 1890:
            r=1200;
            break;
        case 1902:
            r=1100;
            break;
        case 558:
            r=1200;
            break;
        case 551:
            r= 1200;
            break;
        case 563:
            r= 1200;
            break;
        case 573: //saturated
            r= 00;
            break;
        case 567:
            r= 1200;
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
		case 816: // double check this 
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
		case 631: // not in list 
			r=1100; 
			break;
		case 544:
			r= 1100;
			break;
		case 652:
			r= 1100;
			break;
		case 653:
			r= 1000;
			break;
		case 654:
			r= 1000;
			break;
		case 687: // saturated 1100V run  28GeV
			r= 1000;
			break;
		case 572:
			r= 0;
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
            r=0;
            cout<<"warning voltage not found for run:"<<run<<std::endl;
            break;
    }
    return r;
}
};

#ifndef OfficalBeamData_h
#define OfficalBeamData_h 
class OfficalBeamData
{
public:
	OfficalBeamData(){}
	//this constructor makes the TH1Ds and tracks the voltage and energy
	//descriptions of the plots are in the declarations 
	OfficalBeamData(int number, int voltage, float beamEnergy) : beamVoltage(voltage), beamEnergy(beamEnergy){
		runNumber = number;
		cout<<"Processing run:"<<runNumber<<endl;
		name = to_string(number);
		setHighMultiplicity();
		const float kMAX = 10000;
		const float kMainBins = 300;
		mainBinWidth = kMAX/kMainBins;
		pbglPlot = new TH1D(name.c_str(),"",kMainBins,0,kMAX); 
		pbglPlot->Sumw2();
		pbglUnFit = new TH1D(string(name+" no fit").c_str(),"",200,0,10000); 
		pbglNoCut= new TH1D(string(name+"NoCUT").c_str(),"",200,0,10000);
		pbglCCut = new TH1D(string(name+"CCUT").c_str(),"",200,0,10000);
		pbglCVCut = new TH1D(string(name+"CVCUT").c_str(),"",200,0,10000);
		hodo2=new TH1D (string( name+"hodo2").c_str(),"",70,0,8000);
		hodo4=new TH1D (string( name+"hodo4").c_str(),"",70,0,8000);
		hodo8=new TH1D (string( name+"hodo8").c_str(),"",70,0,8000);
		pbglPlot->Sumw2();
		pbglUnFit->Sumw2();
		pbglCCut->Sumw2();
		pbglCVCut->Sumw2();
		pbglNoCut->Sumw2();
		//declare plots for energy, all veto counters, and hodoscopes counters
		cerenkov = new TH1D(string(name+"ceren").c_str(),"",200,0,10000); 
		p_veto1 = new TH1D(string(name+"veto1").c_str(),"",20,0,2);
		p_veto2 = new TH1D(string(name+"veto2").c_str(),"",20,0,2);
		p_veto3 = new TH1D(string(name+"veto3").c_str(),"",20,0,2);
		p_veto4 = new TH1D(string(name+"veto4").c_str(),"",20,0,2);
		p_hodov1 = new TH1D(string(name+"hodov1").c_str(),"",20,0,2);
		p_hodov2 = new TH1D(string(name+"hodov2").c_str(),"",20,0,2);
		p_hodov3 = new TH1D(string(name+"hodov3").c_str(),"",20,0,2);
		p_hodov4 = new TH1D(string(name+"hodov4").c_str(),"",20,0,2);
		p_hodov5 = new TH1D(string(name+"hodov5").c_str(),"",20,0,2);
		p_hodov6 = new TH1D(string(name+"hodov6").c_str(),"",20,0,2);
		p_hodov7 = new TH1D(string(name+"hodov7").c_str(),"",20,0,2);
		p_hodov8 = new TH1D(string(name+"hodov8").c_str(),"",20,0,2);
		p_hodoh1 = new TH1D(string(name+"hodoh1").c_str(),"",20,0,2);
		p_hodoh2 = new TH1D(string(name+"hodoh2").c_str(),"",20,0,2);
		p_hodoh3 = new TH1D(string(name+"hodoh3").c_str(),"",20,0,2);
		p_hodoh4 = new TH1D(string(name+"hodoh4").c_str(),"",20,0,2);
		p_hodoh5 = new TH1D(string(name+"hodoh5").c_str(),"",20,0,2);
		p_hodoh6 = new TH1D(string(name+"hodoh6").c_str(),"",20,0,2);
		p_hodoh7 = new TH1D(string(name+"hodoh7").c_str(),"",20,0,2);
		p_hodoh8 = new TH1D(string(name+"hodoh8").c_str(),"",20,0,2);

		for (int i = 0; i < NUMPLOTS; ++i)
		{
			plotsexits[i]=false;
		}
		gStyle->SetOptStat(0);
	}
	~OfficalBeamData(){ // should delete all the plots 
		if(mainGaus!=NULL){
		 	delete mainGaus;
		}
		if(cut_cerenkov!=NULL) delete cut_cerenkov;
		if(cut_hodov1!=NULL) delete cut_hodov1;
		if(cut_hodov2!=NULL) delete cut_hodov2;
		if(cut_hodov3!=NULL) delete cut_hodov3;
		if(cut_hodov4!=NULL) delete cut_hodov4;
		if(cut_hodov5!=NULL) delete cut_hodov5;
		if(cut_hodov6!=NULL) delete cut_hodov6;
		if(cut_hodov7!=NULL) delete cut_hodov7;
		if(cut_hodov8!=NULL) delete cut_hodov8;
		if(cut_hodoh1!=NULL) delete cut_hodoh1;
		if(cut_hodoh2!=NULL) delete cut_hodoh2;
		if(cut_hodoh3!=NULL) delete cut_hodoh3;
		if(cut_hodoh4!=NULL) delete cut_hodoh4;
		if(cut_hodoh5!=NULL) delete cut_hodoh5;
		if(cut_hodoh6!=NULL) delete cut_hodoh6;
		if(cut_hodoh7!=NULL) delete cut_hodoh7;
		if(cut_hodoh8!=NULL) delete cut_hodoh8;
		if(cut_veto1!=NULL) delete cut_veto1;
		if(cut_veto2!=NULL) delete cut_veto2;
		if(cut_veto3!=NULL) delete cut_veto3;
		if(cut_veto4!=NULL) delete cut_veto4;
		/*if(pbglPlot!=NULL) delete pbglPlot; // these seg fault but also probably mem leak
		if(cerenkov!=NULL) delete cerenkov;
		if(p_hodov1!=NULL) delete p_hodov1;
		if(p_hodov2!=NULL) delete p_hodov2;
		if(p_hodov3!=NULL) delete p_hodov3;
		if(p_hodov4!=NULL) delete p_hodov4;
		if(p_hodov5!=NULL) delete p_hodov5;
		if(p_hodov6!=NULL) delete p_hodov6;
		if(p_hodov7!=NULL) delete p_hodov7;
		if(p_hodov8!=NULL) delete p_hodov8;
		if(p_hodoh1!=NULL) delete p_hodoh1;
		if(p_hodoh2!=NULL) delete p_hodoh2;
		if(p_hodoh3!=NULL) delete p_hodoh3;
		if(p_hodoh4!=NULL) delete p_hodoh4;
		if(p_hodoh5!=NULL) delete p_hodoh5;
		if(p_hodoh6!=NULL) delete p_hodoh6;
		if(p_hodoh7!=NULL) delete p_hodoh7;
		if(p_hodoh8!=NULL) delete p_hodoh8;
		if(p_veto1!=NULL) delete p_veto1;
		if(p_veto2!=NULL) delete p_veto2;
		if(p_veto3!=NULL) delete p_veto3;
		if(p_veto4!=NULL) delete p_veto4;*/
		/*if(hodo2!=NULL) delete hodo2; //maybe root objects delete their members   
		if(hodo4!=NULL) delete hodo4;
		if(hodo8!=NULL) delete hodo8;*/
	}
	//use this function to add data to the class is will return wether the data passes the cuts and only adds it if it does
	bool add(double cerenkov, double* veto, double* hhodo, double* vhodo, double pbgl){
		bool r = false;
		this->cerenkov->Fill(cerenkov);
		p_hodoh1->Fill(hhodo[0]);
		p_hodoh2->Fill(hhodo[1]);
		p_hodoh3->Fill(hhodo[2]);
		p_hodoh4->Fill(hhodo[3]);
		p_hodoh5->Fill(hhodo[4]);
		p_hodoh6->Fill(hhodo[5]);
		p_hodoh7->Fill(hhodo[6]);
		p_hodoh8->Fill(hhodo[7]);
		p_hodov1->Fill(vhodo[0]);
		p_hodov2->Fill(vhodo[1]);
		p_hodov3->Fill(vhodo[2]);
		p_hodov4->Fill(vhodo[3]);
		p_hodov5->Fill(vhodo[4]);
		p_hodov6->Fill(vhodo[5]);
		p_hodov7->Fill(vhodo[6]);
		p_hodov8->Fill(vhodo[7]);
		p_veto1->Fill(veto[0]);
		p_veto2->Fill(veto[1]);
		p_veto3->Fill(veto[2]);
		p_veto4->Fill(veto[3]);
		int pbglCUT;
		if (beamEnergy>8)
		{
			pbglCUT=500;
		}
		else{
			pbglCUT=100;
		}
		if (pbgl>pbglCUT)
		{
			pbglNoCut->Fill(pbgl);
			if(passCerenkov((float)cerenkov)){
				pbglCCut->Fill(pbgl);
				if(noVeto(veto)){
					pbglCVCut->Fill(pbgl);
					if (passHodoH(hhodo)&&passHodoV(vhodo)){
						hodo8->Fill(pbgl);
						switch(multiplicityType){
							case 0:
								pbglPlot->Fill(pbgl);
								pbglUnFit->Fill(pbgl);
								r=true;
								if (passHodoH4x4(hhodo)&&passHodoV4x4(vhodo))
								{	
									hodo4->Fill(pbgl);
									if (passHodoH2x2(hhodo)&&passHodoV2x2(vhodo))
									{
										hodo2->Fill(pbgl);
									}
								}
								break;
							case 1:
								if (passHodoH4x4(hhodo)&&passHodoV4x4(vhodo))
								{	
									pbglPlot->Fill(pbgl);
									pbglUnFit->Fill(pbgl);
									hodo4->Fill(pbgl);
									r=true;
									if (passHodoH2x2(hhodo)&&passHodoV2x2(vhodo))
									{
										hodo2->Fill(pbgl);
									}
								}
								break;
							case 2:
								if (passHodoH4x4(hhodo)&&passHodoV4x4(vhodo))
								{	
									hodo4->Fill(pbgl);
									if (passHodoH2x2(hhodo)&&passHodoV2x2(vhodo))
									{
										hodo2->Fill(pbgl);
										pbglUnFit->Fill(pbgl);
										pbglPlot->Fill(pbgl);
										r=true;
									}
								}
								break;
							default:
								cout<<"Error multiplicityType"<<endl;
								exit(2);
								break;
						}
					}
				}
			}
		}
		return r;
}
	//deprecated beacause I cut one at a time 
	inline bool passCuts(double cerenkov, double* veto, double* vhodo, double* hhodo){
		return passCerenkov((float)cerenkov) && noVeto(veto),passHodoV(vhodo),passHodoH(hhodo);
	}
	inline bool noVeto(double* veto){
		if(runNumber<1000){
			return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<VETOcut;
		}
		else{ // the secod data set has runnumber >1000 and the 4th veto was moved slightly
			return veto[0]<VETOcut && veto[1]<VETOcut && veto[2]<VETOcut && veto[3]<.25;

		}
	}
	inline bool passCerenkov(float cerenkov){
		if(beamEnergy>=20){
			//cout<<"High Energy return true"<<endl;
			return true;
		} 
		return cerenkov>CERENKOVcut;
	}
	inline bool passHodoV(double* hodo){
		if (beamEnergy>2)
		{
			return hodo[0]>HODOVcut[0] ^ hodo[1]>HODOVcut[1] ^ hodo[2]>HODOVcut[2] ^ hodo[3]>HODOVcut[3] ^ hodo[4]>HODOVcut[4] ^ hodo[5]>HODOVcut[5] ^ hodo[6]>HODOVcut[6] ^ hodo[7]>HODOVcut[7];
		}
		else{
			return hodo[0]>HODOVcut[0]+.05 ^ hodo[1]>HODOVcut[1]+.05 ^ hodo[2]>HODOVcut[2]+.05 ^ hodo[3]>HODOVcut[3]+.05 ^ hodo[4]>HODOVcut[4]+.05 ^ hodo[5]>HODOVcut[5]+.05 ^ hodo[6]>HODOVcut[6]+.05 ^ hodo[7]>HODOVcut[7]+.05;
		}
	}
	inline bool passHodoH(double* hodo){ 
		if (beamEnergy>2)
		{
			return hodo[0]>HODOHcut[0] ^ hodo[1]>HODOHcut[1] ^ hodo[2]>HODOHcut[2] ^ hodo[3]>HODOHcut[3] ^ hodo[4]>HODOHcut[4] ^ hodo[5]>HODOHcut[5] ^ hodo[6]>HODOHcut[6] ^ hodo[7]>HODOHcut[7];
		}
		else{
			return hodo[0]>HODOHcut[0]+.05 ^ hodo[1]>HODOHcut[1]+.05 ^ hodo[2]>HODOHcut[2]+.05 ^ hodo[3]>HODOHcut[3]+.05 ^ hodo[4]>HODOHcut[4]+.05 ^ hodo[5]>HODOHcut[5]+.05 ^ hodo[6]>HODOHcut[6]+.05 ^ hodo[7]>HODOHcut[7]+.05;
		}	}
	inline bool passHodoV4x4(double* hodo){ //exclusive or 
		return hodo[2]>HODOVcut[2] ^ hodo[3]>HODOVcut[3] ^ hodo[4]>HODOVcut[4] ^ hodo[5]>HODOVcut[5];
	}
	inline bool passHodoH4x4(double* hodo){ //exclusive or 
		return hodo[2]>HODOHcut[2] ^ hodo[3]>HODOHcut[3] ^ hodo[4]>HODOHcut[4] ^ hodo[5]>HODOHcut[5];
	}
	inline bool passHodoV2x2(double* hodo){ //exclusive or 
		return hodo[3]>HODOVcut[3] ^ hodo[4]>HODOVcut[4];
	}
	inline bool passHodoH2x2(double* hodo){ //exclusive or 
		return hodo[3]>HODOHcut[3] ^ hodo[4]>HODOHcut[4];
	}
	//returns the fit to the signal after the cuts 
	TF1* makeGaus(){
		made=true;
		int maxbin = pbglPlot->GetMaximumBin();
		double gausLowBound = pbglPlot->GetBinLowEdge(maxbin);
		double temp = gausLowBound*.3; //sometimes this number needs to be ajusted to make the guas fit well
		double gausUpbound = gausLowBound +temp;
		if(gausUpbound >10000){gausUpbound=10000;} //so that fit doesn't exceed range of histogram
		gausLowBound -= temp; 
		Scalar mygaus[2];
		TF1* gaus; 
		if (beamEnergy>=20)
		{
			gaus= noCerenkovFit();
			mygaus[0] = Scalar(gaus->GetParameter(4),gaus->GetParError(4)); //sigma
			mygaus[1] = Scalar(gaus->GetParameter(5),gaus->GetParError(5)); //sigma
		}
		else{
			gaus= new TF1("gaus","gaus",gausLowBound,gausUpbound);
			pbglPlot->Fit(gaus,"QN","",gausLowBound,gausUpbound);       //“R” Use the range specified in the function range
			mygaus[0] = Scalar(gaus->GetParameter(1),gaus->GetParError(1)); //mean
			mygaus[1] = Scalar(gaus->GetParameter(2),gaus->GetParError(2)); //sigma
			recursiveGaus(pbglPlot, gaus, mygaus, 1.5,97);
			pbglPlot->GetXaxis()->SetRangeUser(mygaus[0]-(mygaus[1]*5.0),mygaus[0]+mygaus[1]*5.0);
			gausLowBound=mygaus[0]-(mygaus[1]*5.0);
			gausUpbound=mygaus[0]+mygaus[1]*5.0;
		}
		mainGaus = new GausPlot(pbglPlot,gaus,mygaus[0]-(mygaus[1]*5.0),mygaus[0]+mygaus[1]*5.0);
		gaus->SetLineStyle(1);
		gaus->SetLineWidth(2);
		gaus->SetLineColor(kRed);
		mean = mygaus[0];
		sigma = mygaus[1];
		return gaus;
	}	

	
	//print a fit to the pbgl and display the parameters indexed in the list
	void printFit(TF1* fit,string filename,std::pair<float,float>* range=NULL,int nTitles=0,int* parameterNums=NULL,string* names=NULL){
		TCanvas* tc = new TCanvas();
		TH1* plot = (TH1*)pbglPlot->Clone("plot");
		TLegend* tl = new TLegend(.75,.75,.95,.95);
		plot->SetTitle(";signal;counts");
		plot->Draw();
		if(range){
			fit->Clone()->Draw("same");
			cout<<"drew mains"<<endl;
			fit->SetRange(range->first,range->second);
			fit->SetLineColor(kBlue);
		}
		fit->Draw("same");
		if (nTitles>0)
		{
			string title="";
			for (int i = nTitles-1; i >=0 ; i--)
			{
				title = names[i]+":"+Form("%0.3f#pm%0.3f",fit->GetParameter(i),fit->GetParError(i))+'\n';
				tl->AddEntry(fit,title.c_str(),"l");
			}
			cout<<"drawing legend"<<endl;
			tl->Draw();
		}
		cout<<"draw range"<<endl;
		tc->SaveAs(filename.c_str());
		if (plot)
		{
			delete plot;
		}
		delete tc;
	}
	string* titleSigmaMean(){
		string *titles = new string[2];
		titles[0]="#sigma";
		titles[1]="#mu";
		return titles;
	}
	TF1* noCerenkovFit(){
		//fit double gaus 
		TH1F *h_work = (TH1F*)pbglPlot->Clone("pbglPlotworking");
		//make single gausses for the guesses
		TF1* backgroundGaus = new TF1("background","gaus",500,10000);	
		TF1* backgroundExpo = new TF1("background2","expo",500,10000);
		TF1* signalGaus = new TF1("rgaus","gaus",4000,10000);
		signalGaus->SetLineColor(kBlue);
		//kinda weird run 
		if(runNumber==1945){
			signalGaus->SetParLimits(1,6000,7000);
		}
		else{
			signalGaus->SetParLimits(1,500,9000);
		}

		//for printing
		int paramNums[]={1,2};

		//make the initial gaussian background fit 
		backgroundGaus->SetParLimits(1,500,10000);
		h_work->Fit(backgroundGaus,"NL","",500,4000);
		string outname="background1G-"+to_string(runNumber)+".pdf";
		printFit(backgroundGaus,outname);
		
		//use it to define the range 
		std::pair<float,float> expoRange;
		expoRange.second=4500;
		expoRange.first=backgroundGaus->GetParameter(1);

		//try and exponential fit
		h_work->Fit(backgroundExpo,"NL","",expoRange.first,expoRange.second);
		outname="background1E-"+to_string(runNumber)+".pdf";
		printFit(backgroundExpo,outname,&expoRange);

		//try fitting the signal with each of the background models 
		h_work->Add(backgroundGaus,-1);
		signalGaus->SetParameter(1,h_work->GetBinLowEdge(h_work->GetMaximumBin()));
		h_work->Fit(signalGaus,"RN");	
		outname="background2G-"+to_string(runNumber)+".pdf";
		printFit(signalGaus,outname,NULL,2,paramNums,titleSigmaMean());

		h_work->Add(backgroundGaus,1);
		h_work->Add(backgroundExpo,-1);
		h_work->Fit(signalGaus,"RN");	
		outname="background2E-"+to_string(runNumber)+".pdf";
		printFit(signalGaus,outname,NULL,2,paramNums,titleSigmaMean());

		//use the guesses to fit a double gaus 
		TF1* doubleGaus = new TF1("doubleGaus","[0]*TMath::Exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]*TMath::Exp(-(x-[4])*(x-[4])	/(2*[5]*[5]))",500,10000);
		doubleGaus->SetParLimits(1,500,10000);
		doubleGaus->SetParLimits(4,4000,10000);
		doubleGaus->SetParameter(1,backgroundGaus->GetParameter(1)); //guess the background peak 
		doubleGaus->SetParameter(2,backgroundGaus->GetParameter(2)); //guess the background width
		doubleGaus->SetParameter(0,backgroundGaus->GetParameter(0)); //guess the background area 
		doubleGaus->SetParameter(3,signalGaus->GetParameter(0)); //guess the signal area 
		doubleGaus->SetParameter(4,signalGaus->GetParameter(1)); //guess the signal peak
		doubleGaus->SetParameter(5,signalGaus->GetParameter(2)); //guess the signal width
		pbglPlot->Fit(doubleGaus,"RMNL");
		outname="background3-"+to_string(runNumber)+".pdf";
		printFit(doubleGaus,outname);
		//return the signal
		delete backgroundGaus;
		delete signalGaus;
		delete backgroundExpo;
		delete h_work;
		return doubleGaus;
	}
	
	void plot(){ //note there may be a bug where it does not draw properly but it will save properly
		//draws the cut pbgl signal 
		TCanvas *tc = new TCanvas("tc","tc",800,600);
		plotsexits[0]=true;
		pbglPlot->Draw();
		TF1 *gaus = makeGaus();
		gaus->Draw("same");
		string out = name+".pdf";
		tc->Print(out.c_str());
		tc->Close();
		delete tc;
	}
	void SaveMainHist(){
		string outname = name+"MainHist.root";
		pbglPlot->SaveAs(outname.c_str());
	}
	TH1D* getPlot(){
		//just gets the signal 
		return pbglPlot;
	}
	GausPlot* getMainPlot(){
		//gets the signal plus fit 
		if (!plotsexits[0])
		{
			plotsexits[0]=true;
			TF1 *gaus = makeGaus();
			pbglPlot->SetMarkerSize(.03);
		}
		return mainGaus;
	}
	void plotCerenkov(){
		TCanvas *tc = new TCanvas("tc","tc",800,600);
		plotsexits[1]=true;
		gPad->SetLogy();
		cerenkov->GetXaxis()->SetRangeUser(1,100000);
		cerenkov->Draw();
		TLine *cut = new TLine(CERENKOVcut,0,CERENKOVcut+1,DBL_MAX);
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		cut->Draw("same");
		cut_cerenkov = new CutPlot(cerenkov,cut);
		string out = name+"ceren.pdf";
		tc->Print(out.c_str());
		tc->Close();
		delete tc;
	}
	void compareHodo(int number){
		TCanvas *tc = new TCanvas();
		gPad->SetLogy();
		makeDifferent(hodo4,1);
		makeDifferent(hodo2,2);
		hodo8->Draw();
		hodo4->Draw("same");
		hodo2->Draw("same");
		/*TLegend *dairy = new TLegend(.6,.6,.9,.9);
		dairy->AddEntry(hodo8,"8x8","l");
		dairy->AddEntry(hodo4,"4x4","l");
		dairy->AddEntry(hodo2,"2x2","l");
		dairy->Draw();*/
		string title  = "hodocompare"+to_string(number)+".pdf";
		tc->Print(title.c_str());
		delete tc;
	}
	CutPlot* getCerenkovPlot(){
		if (!plotsexits[1])
		{
			plotsexits[1]=true;
			TLine *cut =  new TLine(CERENKOVcut,0,CERENKOVcut+1,DBL_MAX);
			cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
			cut->SetLineStyle(9);
			cut_cerenkov = new CutPlot(cerenkov,cut);
		}
		return cut_cerenkov;
	}
	void plotVeto(int i){
		switch (i){
			case 1:
				plotVeto1();
				plotsexits[2]=true;
				break;
			case 2: 
				plotVeto2();
				plotsexits[3]=true;
				break;
			case 3: 
				plotVeto3();
				plotsexits[4]=true;
				break;
			case 4: 
				plotVeto4();
				plotsexits[5]=true;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;	
		}
	}
	CutPlot* getVetoPlot(int i){
		switch (i){
			case 1:
				if (!plotsexits[2])
				{
					plotsexits[2]=true;
					TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_veto1 = new CutPlot(p_veto1,cut);
				}
				return cut_veto1;
				break;
			case 2: 
				if (!plotsexits[3])
				{
					plotsexits[3]=true;
					TLine *cut =  new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_veto2 = new CutPlot(p_veto2,cut);
				}
				return cut_veto2;
				break;
			case 3: 
				if (!plotsexits[4])
				{
					plotsexits[4]=true;
					TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_veto3 = new CutPlot(p_veto3,cut);
				}
				return cut_veto3;
				break;
			case 4: 
				if (!plotsexits[5])
				{
					plotsexits[5]=true;
					TLine *cut;
					if (runNumber<1000)
					{
						cut=new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
					}
					else{
						cut=new TLine(.25,0,VETOcut+1,DBL_MAX);
					}
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_veto4 = new CutPlot(p_veto4,cut);
				}
				return cut_veto4;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;	
				return NULL;
		}
	}
	void plotVeto(){ 
		plotVeto1();
		plotVeto2();
		plotVeto3();
		plotVeto4();
		plotsexits[2]=true;
		plotsexits[3]=true;
		plotsexits[4]=true;
		plotsexits[5]=true;
	}

	CutPlot* getHodoH(int i){
		switch (i){
			case 1:
				if (!plotsexits[6])
				{
					plotsexits[6]=true;
					TLine *cut = new TLine(HODOHcut[0],0,HODOHcut[0]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh1 = new CutPlot(p_hodoh1,cut);
				}
				return cut_hodoh1;
				break;
			case 2: 
				if (!plotsexits[7])
				{
					plotsexits[7]=true;
					TLine *cut = new TLine(HODOHcut[1],0,HODOHcut[1]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh2 = new CutPlot(p_hodoh2,cut);
				}
				return cut_hodoh2;
				break;
			case 3: 
				if (!plotsexits[8])
				{
					plotsexits[8]=true;
					TLine *cut = new TLine(HODOHcut[2],0,HODOHcut[2]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh3 = new CutPlot(p_hodoh3,cut);
				}
				return cut_hodoh3;
				break;
			case 4: 
				if (!plotsexits[9])
				{
					plotsexits[9]=true;
					TLine *cut = new TLine(HODOHcut[3],0,HODOHcut[3]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh4 = new CutPlot(p_hodoh4,cut);
				}
				return cut_hodoh4;
				break;
			case 5:
				if (!plotsexits[10])
				{
					plotsexits[10]=true;
					TLine *cut = new TLine(HODOHcut[4],0,HODOHcut[4]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh5 = new CutPlot(p_hodoh5,cut);
				}
				return cut_hodoh5;
				break;
			case 6: 
				if (!plotsexits[11])
				{
					plotsexits[11]=true;
					TLine *cut = new TLine(HODOHcut[5],0,HODOHcut[5]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh6 = new CutPlot(p_hodoh6,cut);
				}
				return cut_hodoh6;
				break;
			case 7: 
				if (!plotsexits[12])
				{
					plotsexits[12]=true;
					TLine *cut = new TLine(HODOHcut[6],0,HODOHcut[6]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh7 = new CutPlot(p_hodoh7,cut);
				}
				return cut_hodoh7;
				break;
			case 8: 
				if (!plotsexits[13])
				{
					plotsexits[13]=true;
					TLine *cut = new TLine(HODOHcut[7],0,HODOHcut[7]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodoh8 = new CutPlot(p_hodoh8,cut);
				}
				return cut_hodoh8;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;
				return NULL;
		}
	}
	void plotHodoH(int i){
		switch (i){
			case 1:
				
				break;
			case 2: 
				plotHodoh2();
				plotsexits[7]=true;
				break;
			case 3: 
				plotHodoh3();
				plotsexits[8]=true;
				break;
			case 4: 
				plotHodoh4();
				plotsexits[9]=true;
				break;
			case 5:
				plotHodoh5();
				plotsexits[10]=true;
				break;
			case 6: 
				plotHodoh6();
				plotsexits[11]=true;
				break;
			case 7: 
				plotHodoh7();
				plotsexits[12]=true;
				break;
			case 8: 
				plotHodoh8();
				plotsexits[13]=true;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;
		}
	}
	void plotHodoH(){ 
		plotHodoh1();
		plotHodoh2();
		plotHodoh3();
		plotHodoh4();
		plotHodoh5();
		plotHodoh6();
		plotHodoh7();
		plotHodoh8();
		plotsexits[12]=true;
		plotsexits[13]=true;
		plotsexits[6]=true;
		plotsexits[7]=true;
		plotsexits[8]=true;
		plotsexits[9]=true;
		plotsexits[10]=true;
		plotsexits[11]=true;
	}
	CutPlot* getHodoV(int i){
		switch (i){
			case 1:
				if (!plotsexits[14])
				{
					plotsexits[14]=true;
					TLine *cut = new TLine(HODOVcut[0],0,HODOVcut[0]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov1 = new CutPlot(p_hodov1,cut);
				}
				return cut_hodov1;
				break;
			case 2: 
				if (!plotsexits[15])
				{
					plotsexits[15]=true;
					TLine *cut = new TLine(HODOVcut[1],0,HODOVcut[1]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ; cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov2 = new CutPlot(p_hodov2,cut);
				}
				return cut_hodov2;
				break;
			case 3: 
				if (!plotsexits[16])
				{
					plotsexits[16]=true;
					TLine *cut = new TLine(HODOVcut[2],0,HODOVcut[2]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov3 = new CutPlot(p_hodov3,cut);
				}
				return cut_hodov3;
				break;
			case 4: 
				if (!plotsexits[17])
				{
					plotsexits[17]=true;
					TLine *cut = new TLine(HODOVcut[3],0,HODOVcut[3]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov4 = new CutPlot(p_hodov4,cut);
				}
				return cut_hodov4;
				break;
			case 5:
				if (!plotsexits[18])
				{
					plotsexits[18]=true;
					TLine *cut = new TLine(HODOVcut[4],0,HODOVcut[4]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov5 = new CutPlot(p_hodov5,cut);
				}
				return cut_hodov5;
				break;
			case 6: 
				if (!plotsexits[19])
				{
					plotsexits[19]=true;
					TLine *cut = new TLine(HODOVcut[5],0,HODOVcut[5]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov6 = new CutPlot(p_hodov6,cut);
				}
				return cut_hodov6;
				break;
			case 7: 
				if (!plotsexits[20])
				{
					plotsexits[20]=true;
					TLine *cut = new TLine(HODOVcut[6],0,HODOVcut[6]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov7 = new CutPlot(p_hodov7,cut);
				}
				return cut_hodov7;
				break;
			case 8: 
				if (!plotsexits[21])
				{
					plotsexits[21]=true;
					TLine *cut = new TLine(HODOVcut[7],0,HODOVcut[7]+1,DBL_MAX);
					cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
					cut->SetLineStyle(9);
					cut_hodov8 = new CutPlot(p_hodov8,cut);
				}
				return cut_hodov8;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;
				return NULL;
		}
	}

	void plotHodoV(int i){
		switch (i){
			case 1:
				plotHodov1();
				plotsexits[14]=true;
				break;
			case 2: 
				plotHodov2();
				plotsexits[15]=true;
				break;
			case 3: 
				plotHodov3();
				plotsexits[16]=true;
				break;
			case 4: 
				plotHodov4();
				plotsexits[17]=true;
				break;
			case 5:
				plotHodov5();
				plotsexits[18]=true;
				break;
			case 6: 
				plotHodov6();
				plotsexits[19]=true;
				break;
			case 7: 
				plotHodov7();
				plotsexits[20]=true;
				break;
			case 8: 
				plotHodov8();
				plotsexits[21]=true;
				break;
			default:
				cout<<"invalid plot number: "<<i<<endl;
		}
	}
	void plotHodoV(){ //overloaded to plot all 8 horizontal hodo plots
		plotHodov1();
		plotHodov2();
		plotHodov3();
		plotHodov4();
		plotHodov5();
		plotHodov6();
		plotHodov7();
		plotHodov8();
		plotsexits[14]=true;
		plotsexits[15]=true;
		plotsexits[16]=true;
		plotsexits[17]=true;
		plotsexits[18]=true;
		plotsexits[19]=true;
		plotsexits[20]=true;
		plotsexits[21]=true;
	}

	double getResolution(){
		if(!made) makeGaus();
		return (sigma/mean).value;
	}
	double getMean(){
		if(!made) makeGaus();
		return mean.value;
	}
	double getResolutionUncertainty(){
		if(!made) makeGaus();
		return (sigma/mean).uncertainty;
	}
	double getMeanUncertainty(){
		if(!made) makeGaus();
		return TMath::Sqrt(mean.uncertainty*mean.uncertainty+mainBinWidth*mainBinWidth);
	}
	double getSigmaUncertainty(){
		if(!made) makeGaus();
		return sigma.uncertainty;
	}
	double getSigma(){
		if(!made) makeGaus();
		return sigma.value;
	}
	int getEnergy(){
		return beamEnergy;
	}
	void makeAllPlots(){
		getMainPlot();
		getCerenkovPlot();
		getVetoPlot(1);
		getVetoPlot(2);
		getVetoPlot(3);
		getVetoPlot(4);
		getHodoV(1);
		getHodoV(2);
		getHodoV(3);
		getHodoV(4);
		getHodoV(5);
		getHodoV(6);
		getHodoV(7);
		getHodoV(8);
		getHodoH(1);
		getHodoH(2);
		getHodoH(3);
		getHodoH(4);
		getHodoH(5);
		getHodoH(6);
		getHodoH(7);
		getHodoH(8);
	}
	void fitPlot(string name){
		TCanvas *tc = new TCanvas();
		TH1 *p_fit = mainGaus->get_main();
		p_fit->Divide(mainGaus->get_gaus());
		p_fit->Draw();
		name+=".pdf";
		tc->Print(name.c_str());
	}
	void makeBigPlot(string name){
		makeAllPlots();
		TCanvas *tc = new TCanvas(getNextPlotName(&plotcount).c_str(),"",800,600);
		tc->Divide(5,5,0.01,0.01);
		tc->cd(23); 
		mainGaus->Draw();
		myText(.6,.8,kBlack,"PbGl E",.16);
		tc->cd(24); 
		/*pbglCCut->GetXaxis()->SetRangeUser(0,mainGaus->getUpBound()*1.25);
		pbglCVCut->GetXaxis()->SetRangeUser(0,mainGaus->getUpBound()*1.25);
		pbglUnFit->GetXaxis()->SetRangeUser(0,mainGaus->getUpBound()*1.25);
		pbglNoCut->GetXaxis()->SetRangeUser(0,mainGaus->getUpBound()*1.25);*/
		pbglCCut->SetMarkerSize(.03);
		pbglCVCut->SetMarkerSize(.03);
		pbglUnFit->SetMarkerSize(.03);
		pbglNoCut->SetMarkerSize(.03);
		makeDifferent(pbglCCut,1);
		makeDifferent(pbglCVCut,2);
		makeDifferent(pbglNoCut,3);
		gPad->SetLogy();
		TLegend *cutLegend = new TLegend(.2,.2,.8,.8);
		cutLegend->SetBorderSize(0);
		cutLegend->SetFillColorAlpha(kWhite,0);
		pbglNoCut->Draw();
		pbglCCut->Draw("same");
		pbglCVCut->Draw("same");
		pbglUnFit->Draw("same");
		cutLegend->AddEntry(pbglNoCut,"No Cuts","l");
		cutLegend->AddEntry(pbglCCut,"Cherenkov Cut","l");
		cutLegend->AddEntry(pbglCVCut,"Cherenkov+Veto","l");
		cutLegend->AddEntry(pbglUnFit,"All Cuts","l");
		myText(.2,.8,kBlack,"PbGl Cuts",.16);
		tc->cd(25);
		cutLegend->Draw();
		tc->cd(2); gPad->SetLogy();
		cut_cerenkov->Draw();
		myText(.4,.8,kBlack,"Cherenkov Signal",.18);
		tc->cd(3); gPad->SetLogy();
		cut_veto1->Draw();
		myText(.4,.8,kBlack,"Veto1",.18);
		tc->cd(4); gPad->SetLogy();
		cut_veto2->Draw();
		myText(.4,.8,kBlack,"Veto2",.18);
		tc->cd(5); gPad->SetLogy();
		cut_veto3->Draw();
		myText(.4,.8,kBlack,"Veto3",.18);
		tc->cd(6); gPad->SetLogy();
		cut_veto4->Draw();
		myText(.4,.8,kBlack,"Veto4",.18);
		tc->cd(7); gPad->SetLogy();
		cut_hodov1->Draw();
		myText(.4,.8,kBlack,"H-Hodo1",.18);
		tc->cd(8); gPad->SetLogy();
		cut_hodov2->Draw();
		myText(.4,.8,kBlack,"H-Hodo2",.18);
		tc->cd(9); gPad->SetLogy();
		cut_hodov3->Draw();
		myText(.4,.8,kBlack,"H-Hodo3",.18);
		tc->cd(10); gPad->SetLogy();
		cut_hodov4->Draw();
		myText(.4,.8,kBlack,"H-Hodo4",.18);
		tc->cd(11); gPad->SetLogy();
		cut_hodov5->Draw();
		myText(.4,.8,kBlack,"H-Hodo5",.18);
		tc->cd(12); gPad->SetLogy();
		cut_hodov6->Draw();
		myText(.4,.8,kBlack,"H-Hodo6",.18);
		tc->cd(13); gPad->SetLogy();
		cut_hodov7->Draw();
		myText(.4,.8,kBlack,"H-Hodo7",.18);
		tc->cd(14); gPad->SetLogy();
		cut_hodov8->Draw();
		myText(.4,.8,kBlack,"H-Hodo8",.18);
		tc->cd(15); gPad->SetLogy();
		cut_hodoh1->Draw();
		myText(.4,.8,kBlack,"V-Hodo1",.18);
		tc->cd(16); gPad->SetLogy();
		cut_hodoh2->Draw();
		myText(.4,.8,kBlack,"V-Hodo2",.18);
		tc->cd(17); gPad->SetLogy();
		cut_hodoh3->Draw();
		myText(.4,.8,kBlack,"V-Hodo3",.18);
		tc->cd(18); gPad->SetLogy();
		cut_hodoh4->Draw();
		myText(.4,.8,kBlack,"V-Hodo4",.18);
		tc->cd(19); gPad->SetLogy();
		cut_hodoh5->Draw();
		myText(.4,.8,kBlack,"V-Hodo5",.18);
		tc->cd(20); gPad->SetLogy();
		cut_hodoh6->Draw();
		myText(.4,.8,kBlack,"V-Hodo6",.18);
		tc->cd(21); gPad->SetLogy();
		cut_hodoh7->Draw();
		myText(.4,.8,kBlack,"V-Hodo7",.18);
		tc->cd(22); gPad->SetLogy();
		cut_hodoh8->Draw();
		myText(.4,.8,kBlack,"V-Hodo8",.18);
		tc->cd(1);
		myText(.12,.8,kBlack,name.c_str(),.13);
		myText(.12,.6,kBlack,Form("Mean:%0.2f#pm %0.2f",mean.value,mean.uncertainty),.13);
		myText(.12,.4,kBlack,Form("#sigma:%0.2f#pm %0.2f",sigma.value,sigma.uncertainty),.13);
		myText(.12,.2,kBlack,Form("Resolution:%0.2f#pm%0.3f",(sigma/mean).value,(sigma/mean).uncertainty),.11);
		for (int i = 0; i < 25; ++i)
		{
			tc->cd(i+1);
			gPad->SetTopMargin(.01);
			gPad->SetBottomMargin(.06);
			gPad->SetRightMargin(.001);
			gPad->SetLeftMargin(.07);
		}
		name+=".pdf";
		tc->SaveAs(name.c_str());
		tc->Close();
		delete tc;
	}
private:
	const double CERENKOVcut = 1400; 
	const float VETOcut = .35; 
	const float HODOHcut[8] = {.45,.55,.45,.5,.5,.55,.45,.45}; 
	const float HODOVcut[8] = {.65,.66,.70,.65,.65,.55,.60,.60};
	const int VETOSIZE = 4;
	const int HODOSIZE = 8;

	int beamVoltage;
	int beamEnergy;
	int multiplicityType;

	TH1D *pbglPlot=NULL; // the main data plot that gets fit 
	TH1D *cerenkov=NULL; // the signal from the cherenkov counter 
	TH1D *p_hodov1=NULL; // signal from hodo 
	TH1D *p_hodov2=NULL;
	TH1D *p_hodov3=NULL;
	TH1D *p_hodov4=NULL;
	TH1D *p_hodov5=NULL;
	TH1D *p_hodov6=NULL;
	TH1D *p_hodov7=NULL;
	TH1D *p_hodov8=NULL;
	TH1D *p_hodoh1=NULL;
	TH1D *p_hodoh2=NULL;
	TH1D *p_hodoh3=NULL;
	TH1D *p_hodoh4=NULL;
	TH1D *p_hodoh5=NULL;
	TH1D *p_hodoh6=NULL;
	TH1D *p_hodoh7=NULL;
	TH1D *p_hodoh8=NULL;
	TH1D *p_veto1=NULL; // signal from veto
	TH1D *p_veto2=NULL;
	TH1D *p_veto3=NULL;
	TH1D *p_veto4=NULL;
	
	TH1D *pbglCCut=NULL; //pbgl energy after cherenkov cut 
	TH1D *pbglCVCut=NULL;//pbgl energy after cherenkov and veto cut 
	TH1D *pbglNoCut=NULL; //pbgl energy without cuts 
	TH1D *pbglUnFit=NULL; //the main plot but does not get fit to a guassian 
	TH1D *hodo2=NULL; // the final plot with nxn hodo cuts 
	TH1D *hodo4=NULL;
	TH1D *hodo8=NULL;
	
	GausPlot *mainGaus=NULL; //combined class of the main plot and its gaussian 
	CutPlot *cut_cerenkov=NULL; // combined class of the cherenkov signal and the line showing the cut 
	CutPlot *cut_hodov1=NULL; // combined class of the hodo signal  and the line showing the cut
	CutPlot *cut_hodov2=NULL;
	CutPlot *cut_hodov3=NULL;
	CutPlot *cut_hodov4=NULL;
	CutPlot *cut_hodov5=NULL;
	CutPlot *cut_hodov6=NULL;
	CutPlot *cut_hodov7=NULL;
	CutPlot *cut_hodov8=NULL;
	CutPlot *cut_hodoh1=NULL;
	CutPlot *cut_hodoh2=NULL;
	CutPlot *cut_hodoh3=NULL;
	CutPlot *cut_hodoh4=NULL;
	CutPlot *cut_hodoh5=NULL;
	CutPlot *cut_hodoh6=NULL;
	CutPlot *cut_hodoh7=NULL;
	CutPlot *cut_hodoh8=NULL;
	CutPlot *cut_veto1=NULL; // combined class of the veto signal  and the line showing the cut
	CutPlot *cut_veto2=NULL;
	CutPlot *cut_veto3=NULL;
	CutPlot *cut_veto4=NULL;

	string name; // a descriptive class name used for printing 
	int runNumber=0;
	float mainBinWidth;
	
	//internal booleans for tracking what values have been initialized 
	bool made=false; 
	bool hasEnergy=false;
	static const int NUMPLOTS=22;
	bool plotsexits[NUMPLOTS]; // to track if the plots have been made first the main then the cerenkov then the veto then the vhodo then the hhodo

	//the important results of the class 
	Scalar mean;
	Scalar sigma;

	inline void setHighMultiplicity(){
		if(beamEnergy>2){
			if (beamEnergy>20)
			{
				multiplicityType=1;
			}
			else{
				multiplicityType=1;
			}
		}
		else{
			multiplicityType=0;	
		}
	}

	void recursiveGaus(TH1* h, TF1* gaus, Scalar* data, float sigmadistance,int lazyMan=0){
	    h->Fit(gaus,"QN","",data[0]-data[1]*sigmadistance,data[0]+data[1]*sigmadistance);
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

	void plotVeto1(){
		TCanvas *v1 = new TCanvas("v1","tc",800,600);
		gPad->SetLogy();
		p_veto1->Draw();
		TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_veto1,"calibrated veto1 energy","count");
		cut_veto1 = new CutPlot(p_veto1,cut);
		string out = name+"veto1.pdf";
		v1->Print(out.c_str());
		v1->Close();
		delete v1;
	}
	void plotHodoh1(){
		TCanvas *hh1 = new TCanvas("hh1","tc",800,600);
		gPad->SetLogy();
		p_hodoh1->Draw();
		TLine *cut = new TLine(HODOHcut[0],0,HODOHcut[0]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh1,"calibrated hodoscope h1 energy","count");
		cut_hodoh1 = new CutPlot(p_hodoh1 ,cut);
		string out = name+"hodoh1.pdf";
		hh1->Print(out.c_str());
		hh1->Close();
		delete hh1;
	}
	void plotVeto2(){
		TCanvas *v2 = new TCanvas("v2","tc",800,600);
		gPad->SetLogy();
		p_veto1->Draw();
		TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_veto2,"calibrated veto2 energy","count");
		cut_veto2 = new CutPlot(p_veto2,cut);
		string out = name+"veto2.pdf";
		v2->Print(out.c_str());
		v2->Close();
		delete v2;
	}
	void plotHodoh2(){
		TCanvas *hh2 = new TCanvas("hh2","tc",800,600);
		gPad->SetLogy();
		p_hodoh2->Draw();
		TLine *cut = new TLine(HODOHcut[1],0,HODOHcut[1]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh2,"calibrated hodoscope h2 energy","count");
		cut_hodoh2 = new CutPlot(p_hodoh2,cut);
		string out = name+"hodoh2.pdf";
		hh2->Print(out.c_str());
		hh2->Close();delete hh2;
	}
	void plotVeto3(){
		TCanvas *v3 = new TCanvas("v3","tc",800,600);
		gPad->SetLogy();
		p_veto3->Draw();
		TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_veto3,"calibrated veto3 energy","count");
		cut_veto3 = new CutPlot(p_veto3,cut);
		string out = name+"veto3.pdf";
		v3->Print(out.c_str());
		v3->Close();delete v3;
	}
	void plotHodoh3(){
		TCanvas *hh3 = new TCanvas("hh3","tc",800,600);
		gPad->SetLogy();
		p_hodoh3->Draw();
		TLine *cut = new TLine(HODOHcut[2],0,HODOHcut[2]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh3,"calibrated hodoscope h3 energy","count");
		cut_hodoh3 = new CutPlot(p_hodoh3,cut);
		string out = name+"hodoh3.pdf";
		hh3->Print(out.c_str());
		hh3->Close();delete hh3;
	}

	void plotVeto4(){
		TCanvas *v4 = new TCanvas("v4","tc",800,600);
		gPad->SetLogy();
		p_veto4->Draw();
		TLine *cut = new TLine(VETOcut,0,VETOcut+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_veto4,"calibrated veto4 energy","count");
		cut_veto3 = new CutPlot(p_veto3,cut);
		string out = name+"veto4.pdf";
		v4->Print(out.c_str());
		v4->Close();delete v4;
	}
	void plotHodoh4(){
		TCanvas *hh4 = new TCanvas("hh4","tc",800,600);
		gPad->SetLogy();
		p_hodoh4->Draw();
		TLine *cut = new TLine(HODOHcut[3],0,HODOHcut[3]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh4,"calibrated hodoscope h4 energy","count");
		cut_hodoh4 = new CutPlot(p_hodoh4,cut);
		string out = name+"hodoh4.pdf";
		hh4->Print(out.c_str());
		hh4->Close();delete hh4;
	}
	void plotHodoh5(){
		TCanvas *hh5 = new TCanvas("hh5","tc",800,600);
		gPad->SetLogy();
		p_hodoh5->Draw();
		TLine *cut = new TLine(HODOHcut[4],0,HODOHcut[4]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh5,"calibrated hodoscope h5 energy","count");
		cut_hodoh5 = new CutPlot(p_hodoh5,cut);
		string out = name+"hodoh5.pdf";
		hh5->Print(out.c_str());
		hh5->Close();delete hh5;
	}
	void plotHodoh6(){
		TCanvas *hh6 = new TCanvas("hh6","tc",800,600);
		gPad->SetLogy();
		p_hodoh6->Draw();
		TLine *cut = new TLine(HODOHcut[5],0,HODOHcut[5]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh6,"calibrated hodoscope h6 energy","count");
		cut_hodoh6 = new CutPlot(p_hodoh6,cut);
		string out = name+"hodoh6.pdf";
		hh6->Print(out.c_str());
		hh6->Close();delete hh6;
	}
	void plotHodoh7(){
		TCanvas *hh7 = new TCanvas("hh7","tc",800,600);
		gPad->SetLogy();
		p_hodoh7->Draw();
		TLine *cut = new TLine(HODOHcut[6],0,HODOHcut[6]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh7,"calibrated hodoscope h7 energy","count");
		cut_hodoh7 = new CutPlot(p_hodoh7,cut);
		string out = name+"hodoh7.pdf";
		hh7->Print(out.c_str());
		hh7->Close();delete hh7;
	}
	void plotHodoh8(){
		TCanvas *hh8 = new TCanvas("hh8","tc",800,600);
		gPad->SetLogy();
		p_hodoh8->Draw();
		TLine *cut = new TLine(HODOHcut[7],0,HODOHcut[7]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodoh8,"calibrated hodoscope h8 energy","count");
		cut_hodoh8 = new CutPlot(p_hodoh8,cut);
		string out = name+"hodoh8.pdf";
		hh8->Print(out.c_str());
		hh8->Close();delete hh8;
	}
	void plotHodov1(){
		TCanvas *hv1 = new TCanvas("hv1","tc",800,600);
		gPad->SetLogy();
		p_hodov1->Draw();
		TLine *cut = new TLine(HODOVcut[0],0,HODOVcut[0]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov1,"calibrated hodoscope v1 energy","count");
		cut_hodov1 = new CutPlot(p_hodov1,cut);
		string out = name+"hodov1.pdf";
		hv1->Print(out.c_str());
		hv1->Close();delete hv1;
	}
	void plotHodov2(){
		TCanvas *hv2 = new TCanvas("hv2","tc",800,600);
		gPad->SetLogy();
		p_hodov2->Draw();
		TLine *cut = new TLine(HODOVcut[1],0,HODOVcut[1]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov2,"calibrated hodoscope v2 energy","count");
		cut_hodov2 = new CutPlot(p_hodov2,cut);
		string out = name+"hodov2.pdf";
		hv2->Print(out.c_str());
		hv2->Close();delete hv2;
	}
	void plotHodov3(){
		TCanvas *hv3 = new TCanvas("hv3","tc",800,600);
		gPad->SetLogy();
		p_hodov3->Draw();
		TLine *cut = new TLine(HODOVcut[2],0,HODOVcut[2]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov3,"calibrated hodoscope v3 energy","count");
		cut_hodov3 = new CutPlot(p_hodov3,cut);
		string out = name+"hodov3.pdf";
		hv3->Print(out.c_str());
		hv3->Close();delete hv3;
	}
	void plotHodov4(){
		TCanvas *hv4 = new TCanvas("hv4","tc",800,600);
		gPad->SetLogy();
		p_hodov4->Draw();
		TLine *cut = new TLine(HODOVcut[3],0,HODOVcut[3]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov4,"calibrated hodoscope v4 energy","count");
		cut_hodov4 = new CutPlot(p_hodov4,cut);
		string out = name+"hodov4.pdf";
		hv4->Print(out.c_str());
		hv4->Close();delete hv4;
	}
	void plotHodov5(){
		TCanvas *hv5 = new TCanvas("hv5","tc",800,600);
		gPad->SetLogy();
		p_hodov5->Draw();
		TLine *cut = new TLine(HODOVcut[4],0,HODOVcut[4]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov5,"calibrated hodoscope v5 energy","count");
		cut_hodov5 = new CutPlot(p_hodov5,cut);
		string out = name+"hodov5.pdf";
		hv5->Print(out.c_str());
		hv5->Close();delete hv5;
	}
	void plotHodov6(){
		TCanvas *hv6 = new TCanvas("hv6","tc",800,600);
		gPad->SetLogy();
		p_hodov6->Draw();
		TLine *cut = new TLine(HODOVcut[5],0,HODOVcut[5]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov6,"calibrated hodoscope v6 energy","count");
		cut_hodov6 = new CutPlot(p_hodov6,cut);
		string out = name+"hodov6.pdf";
		hv6->Print(out.c_str());
		hv6->Close();delete hv6;
	}
	void plotHodov7(){
		TCanvas *hv7 = new TCanvas("hv7","tc",800,600);
		gPad->SetLogy();
		p_hodov7->Draw();
		TLine *cut = new TLine(HODOVcut[6],0,HODOVcut[6]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov7,"calibrated hodoscope v7 energy","count");
		cut_hodov7 = new CutPlot(p_hodov7,cut);
		string out = name+"hodov7.pdf";
		hv7->Print(out.c_str());
		hv7->Close();delete hv7;
	}
		void plotHodov8(){
		TCanvas *hv8 = new TCanvas("hv8","tc",800,600);
		gPad->SetLogy();
		p_hodov8->Draw();
		TLine *cut = new TLine(HODOVcut[7],0,HODOVcut[7]+1,DBL_MAX);
		cut->Draw("same");
		cut->SetLineWidth(2); cut->SetLineColor(kBlue) ;
		cut->SetLineStyle(9);
		axisTitles(p_hodov8,"calibrated hodoscope v8 energy","count");
		cut_hodov8 = new CutPlot(p_hodov8,cut);
		string out = name+"hodov8.pdf";
		hv8->Print(out.c_str());
		hv8->Close();delete hv8;
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

   DSTReader551(TTree *tree=0,string name="beam_00000551-0000_DSTReader.root");
   virtual ~DSTReader551();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual OfficalBeamData*     Loop(int runnumber);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

DSTReader551::DSTReader551(TTree *tree,string file) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(file.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(file.c_str());
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
///////////////////////////////////////////////MAIN PROGRAM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

OfficalBeamData* DSTReader551::Loop(int number)
{
	if (fChain == 0) return NULL;
	fChain->GetEntry(1);
	if(number == 567){beam_MTNRG_GeV = 8;}
	OfficalBeamData *tally = new OfficalBeamData(number,runToVoltage(number),TMath::Abs(beam_MTNRG_GeV));
	Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=1; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      tally->add(TOWER_CALIB_C2_energy[1],TOWER_CALIB_TRIGGER_VETO_energy,TOWER_CALIB_HODO_HORIZONTAL_energy,TOWER_CALIB_HODO_VERTICAL_energy,TOWER_CALIB_PbGL_energy[0]);
            
      if(jentry%10000==0) cout<<jentry<<" events entered"<<'\n';
    }
    return tally;
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

void superArraySorter5000(float* energies, float* mean, float* meanError, float* sigma, float* sigmaError,int* runNumber,int SIZE)
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
	          oleSwitcheroo(&runNumber[j],&runNumber[j+1]);
	       }
	   }
	}
}

void Part2A(){
	cout<<"Start Here is your code Mr. Stark "<<endl;
	int voltageSelection=1000;
	bool newData=true;
	RunSelecTOR selecTOR(newData,true,voltageSelection); //newData, checkvoltage,voltage
	string fileLocation = "/Users/naglelab/Documents/FranData/FTB/"; //fran
	//string fileLocation = "springBeamFiles/"; //chase
	string filename = "beam_0000";
	string filenameleadingzero = "beam_00000";
	string extension = "-0000_DSTReader.root";
	filename = fileLocation+filename;
	filenameleadingzero=fileLocation+filenameleadingzero;
	const int totalNUMSIZE=45;
	//set of all the unsaturated files including haggerty's unsure of the voltages will do mostly by hand
	int totalnumber[] = {1879,1882,1883,1888,1889,1890,1901,1902,1906,1925,1943,1945,2073,2074,2094,2095,2097,2098,2126,2127,2149,2150,2167,631,632,558,551,563,544,652,653,654,687,772,773,776,777,809,810,829,830,849,859,574,567 }; //all,beam,files
	//saturated : 573 572 2125 2128 1924 
	//undersaturated: 578 579 580 1904 
	int NUMSIZE= totalNUMSIZE;
	int* number = selecTOR(&totalnumber[0],&NUMSIZE);
	DSTReader551 *reader; //get the root made class to process the tree from the beam you want
	TFile *file;
	stringstream ss;
	float mean[NUMSIZE];
	float meanU[NUMSIZE];
	float sigma[NUMSIZE];
	float sigmaU[NUMSIZE];
	float energy[NUMSIZE];
	for (int i = 0; i < NUMSIZE; ++i)//loop over beam files
	{
		if(number[i]>1000){
			fileLocation = filename+to_string(number[i])+extension;
		}
		else{
			fileLocation = filenameleadingzero+to_string(number[i])+extension;
		}
		file = new TFile(fileLocation.c_str());
		TTree *orange= (TTree*) file->Get("T");
		reader = new DSTReader551(orange,fileLocation);  
		OfficalBeamData* data = reader->Loop(number[i]); // call the loop method for the reader you have, this fills data structures
		data->SaveMainHist();
		cout<<"Res:"<<data->getResolution()<<'\n';
		mean[i] =data->getMean();
		meanU[i]=data->getMeanUncertainty();
		sigma[i]=data->getSigma();
		sigmaU[i]=data->getSigmaUncertainty();
		energy[i]=data->getEnergy();
		string bigname = to_string(number[i])+": "+to_string(runToEnergy(number[i]))+"GeV "+to_string(runToVoltage(number[i]))+"V";
		data->makeBigPlot(bigname);
		//data->fitPlot("fit3");
		//data->compareHodo(number[i]);
		cout<<"Energy:"<<energy[i]<<'\n';
		data->plot();
		cout<<fileLocation<<'\n';
		file->Close();
		delete file;
		delete data;
		delete reader;
	}
	superArraySorter5000(energy,mean,meanU,sigma,sigmaU,number,NUMSIZE); //sort all arrays so that it goes in ascending energy order
	ofstream outFile;
	outFile.open(selecTOR.getName().c_str());
	
	if(outFile.is_open()) //read info out to txt file if it opens
	{
		cout<<"File Opened!"<<endl;

		outFile << "RunNumber";
		for(int i = 0; i < NUMSIZE; i++) //enter all run numbers into txt file
		{
			outFile << ","<< number[i];
		}
		outFile << "\n";
		outFile << "Mean";
		for(int i = 0; i < NUMSIZE; i++) //enter all mean values into txt file
		{
			outFile << ","<< mean[i];
		}
		outFile << "\n";
		outFile << "MeanUncertainty";
		for(int i = 0; i < NUMSIZE; i++) //enter all mean values into txt file
		{
			outFile << ","<< meanU[i];
		}
		outFile << "\n";
		outFile << "Sigma";
		for(int i = 0; i < NUMSIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< sigma[i];
		}
		outFile << "\n";
		outFile << "SigmaUncertainty";
		for(int i = 0; i < NUMSIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< sigmaU[i];
		}
		outFile << "\n";
		outFile << "Energy";
		for(int i = 0; i < NUMSIZE; i++) //enter all signma values into txt file
		{
			outFile << ","<< energy[i];
		}
		outFile << "\n";

		outFile.close();
	}
	else {cout<<"RED ALERT! RED ALERT! FAILED TO WRITE TO A TEXT FILE! I REPEAT! RED ALERT!"<<endl;}

	cout<<"End"<<std::endl;
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
		case 632:
			r=12;
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
		case 1876:
			r=8; break;
		case 1879:
			r=8; break;
		case 1882:
			r=8; break;
		case 1883:
			r=8; break;
		case 1888:
			r=8; break;
		case 1890:
			r=6; break;
		case 2128:
			r=16; break;
		case 2125:
			r=12; break;
		case 2126:
			r=12; break;
		case 2127:
			r=16; break;
		case 1889:
			r=6; break;
		case 1901:
			r=4; break;
		case 1902:
			r=4; break;
		case 1904:
			r=2; break;
		case 1906:
			r=2; break;
		case 1924:
			r=12; break;
		case 1925:
			r=12; break;
		case 1943:
			r=24; break;
		case 1945:
			r=28; break;
		case 2073:
			r=3; break;
		case 2074:
			r=4; break;
		case 2094:
			r=6; break;
		case 2095:
			r=6; break;
		case 2097:
			r=8; break;
		case 2098:
			r=8; break;
		case 2149:
			r=20; break;
		case 2150:
			r=24; break;
		case 2167:
			r=28; break;
		case 773:
			r=10;
			break;
		case 772:
			r=10;
			break;
		case 1926:
			r=16; 
			break;
		case 2045:
			r=2; 
			break;
        default:
            r=-1;
            cout<<"Error in runToEnergy line:"<<__LINE__<<std::endl;
            break;
    }
    return r;
}

int runToVoltage(int run){
    int r;
    int s = (int) run;

    switch (s){
    	case 2045:
			r=1200; 
			break;
    	case 1926:
			r=1100; 
			break;
    	case 2167:
			r=1000; 
			break;
    	case 1945:
			r=1000; 
			break;
    	case 2150:
			r=1000;
			break;
    	case 1943:
			r=1000;
			break;
    	case 2149:
			r=1000;
			break;
    	case 1924:
			r=1200;
			break;
    	case 1925:
			r=1100;
			break;
    	case 2125:
			r=1200;
			break;
    	case 2126:
			r=1100;
			break;
    	case 632:
			r=1100;
			break;
    	case 773:
			r=-1;
			break;
		case 772:
			r=-1;
			break;
    	case 1901:
            r=1200;
            break;
        case 2074:
            r=1200;
            break;
        case 2094:
            r=1200;
            break;
        case 2095:
            r=1100;
            break;
        case 1889:
            r=1100;
            break;
        case 2127:
            r=1100;
            break;
        case 2128: // probably saturated 
            r=1200;
            break;
        case 2098:
            r=1200;
            break;
        case 1883:
            r=1200;
            break;
        case 1882:
            r=1200;
            break;
        case 1879:
            r=1200;
            break;
        case 1906:
            r=1200;
            break;
        case 900: //may need to test this at 1100V 
            r=1200;
            break;
        case 1904:
            r=1100;
            break;
        case 2073:
            r=1200;
            break;
        case 2097:
            r=1100;
            break;
        case 1888:
            r=1100;
            break;
         case 1890:
            r=1200;
            break;
        case 1902:
            r=1100;
            break;
        case 558:
            r=1200;
            break;
        case 551:
            r= 1200;
            break;
        case 563:
            r= 1200;
            break;
        case 573: //saturated
            r= 00;
            break;
        case 567:
            r= 1200;
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
		case 816: // double check this 
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
		case 631: // not in list 
			r=1100; 
			break;
		case 544:
			r= 1100;
			break;
		case 652:
			r= 1100;
			break;
		case 653:
			r= 1000;
			break;
		case 654:
			r= 1000;
			break;
		case 687: // saturated 1100V run  28GeV
			r= 1000;
			break;
		case 572:
			r= 0;
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
            r=0;
            cout<<"warning voltage not found for run:"<<run<<std::endl;
            break;
    }
    return r;
}




