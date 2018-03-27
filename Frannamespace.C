#include "TLegend.h"
#include "TH1F.h"

	short colors[7]={kRed,kBlue,kGreen+2,kMagenta+3,kOrange+4,kCyan+1,kMagenta-7};
	short styles[7]={kFullCircle,kOpenSquare,kFullTriangleUp,kFullDiamond,kFullCross,kFullStar,kOpenFourTrianglesX};
	void makeBins(float* bins, int min, int nBins, float width){
		for(int i=0; i<=nBins;++i){
			bins[i] = min + width*i;
		}
	}
	double quadrature(double d1, double d2){
		return TMath::Sqrt(d1*d1+d2*d2);
	}
	void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize, double angle) {

  	TLatex l; //l.SetTextAlign(12); 
  	l.SetTextSize(tsize); 
  	l.SetNDC();
 	 l.SetTextColor(color);
  	if (angle > 0) l.SetTextAngle(angle);
  	l.DrawLatex(x,y,text);
	}	
	double errorDivide(double d1, double e1, double d2, double e2){
		double r = d1/d2;
		return r * quadrature(e1/d1,e2/d2);
	}
	void makeMarkerNice(TH1F** h, int n){
		for (int i = 1; i < n; ++i)
		{
			(*h)->SetMarkerStyle(styles[i-1]);
			(*h)->SetMarkerColor(colors[i-1]);
			h++;
		}
	}
	void gNice(){
		gStyle->SetOptStat(0);
		gStyle->SetErrorX(0);
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
	void axisTitles(TH1* h,std::string x, std::string y){
		h->GetYaxis()->SetTitle(y.c_str());
		h->GetXaxis()->SetTitle(x.c_str());
	}
	void axisTitles(TGraph* h,std::string x, std::string y){
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
	void doubleZero(TGraph *h, float y, float x){
		h->GetYaxis()->SetRangeUser(0,y);
		h->GetXaxis()->SetRangeUser(0,x);
	}
	template<class T>
	T* queueToArray(queue<T> q){
		T* r = new T[q.size()];
		int i=0;
		while(!q.empty())
		{
			r[i++] = q.front();
			q.pop();
		}
		return r;
	}
