#include "TLegend.h"
#include "THStack.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TMarker.h"
#include <limits.h>
#include <queue>
#include <iostream>

namespace {
	short colors[7]={kRed,kBlue,kGreen+2,kMagenta+3,kOrange+4,kCyan+1,kMagenta-7};
	short styles[7]={kFullCircle,kOpenSquare,kFullTriangleUp,kFullDiamond,kFullCross,kFullStar,kOpenCircle};
/*	const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";*/
	}
using namespace std;
	template<class T>
	T* zeroArray(int n,T* type){
		T* r = new T[n];
		for (int i = 0; i < n; ++i)
		{
			r[i] = 0;
		}
		return r;
	}
	float* zeroArray(int n){
		float* r = new float[n];
		for (int i = 0; i < n; ++i)
		{
			r[i] = 0;
		}
		return r;
	}
	template<class T>
	T* clone(T* a,int SIZE){
		T* r = new T[SIZE];
		for (int i = 0; i < SIZE; ++i)
		{
			r[i] = a[i];
		}
		return r;
	}
	/*template<class T>
	queue<T> clone(queue<T> *q){
		queue<T> r;
		while(!q->empty()){
			r.push(q->front());
		}

	}*/
	void makeBins(float* bins, int min, int nBins, float width){
		for(int i=0; i<=nBins;++i){
			bins[i] = min + width*i;
		}
	}
	float* makeBins(float min, float max, float width, int* nBins){ //nBins can't be NULL
		*nBins =(int)((max-min+.5)/width);
		float *bins= new float[*nBins];
		for(int i=0; i<= *nBins;++i){
			bins[i] = min + width*i;
		}
		return bins;
	}
	void makeMarkerNice(TH1F** h, int n){
		for (int i = 1; i < n; ++i)
		{
			(*h)->SetMarkerStyle(styles[i-1]);
			(*h)->SetMarkerColor(colors[i-1]);
			h++;
		}
	}
	void makeMarkerNice(TGraph** h, int n){
		for (int i = 1; i < n; ++i)
		{
			(*h)->SetMarkerStyle(styles[i-1]);
			(*h)->SetMarkerColor(colors[i-1]);
			h++;
		}
	}
	void makeMarkerNice(TGraphErrors** h, int n){
		for (int i = 1; i < n; ++i)
		{
			(*h)->SetMarkerStyle(styles[i-1]);
			(*h)->SetMarkerColor(colors[i-1]);
			h++;
		}
	}
	/*void gNice(){
		gStyle->SetOptStat(0);
		gStyle->SetErrorX(0);
	}*/
	void fixOffset(TH1* plot){
		plot->GetYaxis()->SetTitleOffset(1);
		plot->GetXaxis()->SetTitleOffset(1);
	}
	void makeLineColors(TH1F** h, int n){
		for (int i = 1; i < n; ++i)
		{
			(*h)->SetLineColor(colors[i-1]);
			h++;
		}
	}
	void makeDifferent(TH1* h, int i){
		h->SetLineColor(colors[i]);
		h->SetMarkerStyle(styles[i]);
		h->SetMarkerColor(colors[i]);
	}
	void makeDifferent(TGraph* h, int i){
		h->SetLineColor(colors[i]);
		h->SetMarkerStyle(styles[i]);
		h->SetMarkerColor(colors[i]);
	}
	void makeDifferent(queue<TH1F*> hQ){
		int i=0;
		while(!hQ.empty()){
			hQ.front()->SetLineColor(colors[i]);
			hQ.front()->SetMarkerStyle(styles[i]);
			hQ.front()->SetMarkerColor(colors[i]);
			hQ.pop();
			i++;
		}
	}
	void makeDifferent(TH1F** h, int SIZE){
		for (int i = 0; i < SIZE; ++i)
		{
			h[i]->SetLineColor(colors[i]);
			h[i]->SetMarkerStyle(styles[i]);
			h[i]->SetMarkerColor(colors[i]);
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
	void axisTitles(TH1* h,std::string x, std::string y){
		h->GetYaxis()->SetTitle(y.c_str());
		h->GetXaxis()->SetTitle(x.c_str());
	}
	void axisTitles(TGraph* h,std::string x, std::string y){
		h->GetYaxis()->SetTitle(y.c_str());
		h->GetXaxis()->SetTitle(x.c_str());
	}
	void axisTitles(THStack* h,std::string x, std::string y){
		h->GetYaxis()->SetTitle(y.c_str());
		h->GetXaxis()->SetTitle(x.c_str());
	}
	/*void smallBorders(){
		gPad->SetBottomMargin(.1);
		gPad->SetTopMargin(.1);
	}*/
	void axisTitleSize(TH1F* h,float s){
		h->GetYaxis()->SetTitleSize(s);
		h->GetXaxis()->SetTitleSize(s);
	}
	void axisTitleSize(TH2F* h,float s){
		h->GetYaxis()->SetTitleSize(s);
		h->GetXaxis()->SetTitleSize(s);
	}
	void axisLabelSize(TH1F* h,float s){
		h->GetYaxis()->SetLabelSize(s);
		h->GetXaxis()->SetLabelSize(s);
	}
	template<class T>
	void axisTitleOffset(T* h, float s){
		h->GetYaxis()->SetTitleOffset(s);
		h->GetXaxis()->SetTitleOffset(s);
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
	void doubleZero(TGraph *h, float y, float x){
		h->GetYaxis()->SetRangeUser(0,y);
		h->GetXaxis()->SetRangeUser(0,x);
	}
	void doubleZero(TH1F *h, float y, float x){
		h->GetYaxis()->SetRangeUser(0,y);
		h->GetXaxis()->SetRangeUser(0,x);
	}
	void printHist(TH1F *h){
		cout<<h->GetName()<<":\n";
		for (int i = 0; i < h->GetNbinsX(); ++i)
		{
			cout<<"Bin"<<i<<":"<<h->GetBinContent(i)<<'\n';
		}
	}
	inline string getNextPlotName(int* plotcount){
		string r= "plot"+to_string(*plotcount);
		*plotcount = *plotcount+1;
		return r;
	}
	void normalizeTotal(TH1F** hlist,int SIZE){
		float sum=0;
		for (int i = 0; i < SIZE; ++i)
		{
			sum+=hlist[i]->Integral();
		}
		for (int i = 0; i < SIZE; ++i)
		{
			hlist[i]->Scale(1/sum);
		}
	}
	void normalizeBins(TH1F** hlist, int SIZE){ // note that this does not propagate the error
		for (int i = 1; i <= hlist[0]->GetNbinsX(); ++i)
		{
			double sum=0;
			for (int j = 0; j < SIZE; ++j)
			{
				sum+=hlist[j]->GetBinContent(i);
			}
			if(sum==0)continue;
			//cout<<"Sum"<<i<<":"<<sum<<'\n';
			for (int j = 0; j < SIZE; ++j)
			{
				hlist[j]->SetBinContent(i,hlist[j]->GetBinContent(i)/sum);
			}
		}
	}
	THStack* getStack(TH1F** hlist, int SIZE){
		string thisName = string(hlist[0]->GetName())+"Stack";
		THStack *r = new THStack(thisName.c_str(),hlist[0]->GetTitle());
		for (int i = 0; i < SIZE; ++i)
		{
			r->Add(hlist[i],"");
		}
		return r;
	}
	THStack* getStack(TH1F** hlist, int SIZE,string options){
		string thisName = string(hlist[0]->GetName())+"Stack";
		THStack *r = new THStack(thisName.c_str(),hlist[0]->GetTitle());
		for (int i = 0; i < SIZE; ++i)
		{
			r->Add(hlist[i],options.c_str());
		}
		return r;
	}
	THStack* getStack(TH1F** hlist, int SIZE,string options,string xTitle, string yTitle){ //must be drawn before it can be editted
		string thisName = string(hlist[0]->GetName())+"Stack";
		THStack *r = new THStack(thisName.c_str(),hlist[0]->GetTitle());
		for (int i = 0; i < SIZE; ++i)
		{
			r->Add(hlist[i],options.c_str());
		}
		return r;
	}
