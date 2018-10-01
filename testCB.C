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
		double A=TMath::Power(p[1]/TMath::Abs(p[0]),p[1])*TMath::Exp(-1.0*p[0]*p[0]/2.0);
		double B=p[1]/TMath::Abs(p[0])-TMath::Abs(p[0]);
		
		cout<<"A:"<<A<<"\nB:"<<B<<'\n';
		if ((x[0]-p[2])/p[3]>=p[0])
		{
			return p[4]*A*TMath::Power(B-((x[0]-p[2])/p[3]),-1.0*p[1]);
		}
		else{
			return p[4]*TMath::Exp(-1.0*(x[0]-p[2])*(x[0]-p[2])/(2*p[3]*p[3]));
		}
}

int testCB(){
	double x=.1;
	double sig=.2;
	double mu=0;
	double alpha=.3;
	double n=1.1;
	double N=500;
	double in[]={x};
	double p[]={alpha,n,mu,sig,N};
	std::cout<<franCrystalBall(in,p)<<'\n';
	return 0;
}