#include <iostream>
#include "TMath.h"
using namespace std;
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
		
		//cout<<"A:"<<A<<"\nB:"<<B<<'\n';
		//cout<<"Deviation:"<<deviation<<'\n';
		if (deviation>-1*p[0])
		{
			//cout<<"gaus\n";
			return p[4]*TMath::Exp(-1.0*(x[0]-p[2])*(x[0]-p[2])/(2*p[3]*p[3]));
		}
		else{
			//cout<<"power\n";
			//cout<<"base:"<<B-((x[0]-p[2])/p[3])<<'\n';
			return p[4]*A*TMath::Power(B-deviation,-1.0*p[1]);
		}
}

int testCB(){
	double x=500;
	double sig=500;
	double mu=1500;
	double alpha=2;
	double n=.9;
	double N=1;
	double in[]={x};
	double p[]={alpha,n,mu,sig,N};
	std::cout<<franCrystalBall(in,p)<<'\n';
	TH1D* plot = new TH1D("plot","",100,0,10000);
	plot->Draw();
	TF1* cb = new TF1("func",franCrystalBall,100,10000,5);
	cb->SetParameter(0,alpha);
	cb->SetParameter(1,n);
	cb->SetParameter(2,mu);
	cb->SetParameter(3,sig);
	cb->SetParameter(4,N);
	cb->Draw("same");
	return 0;
}