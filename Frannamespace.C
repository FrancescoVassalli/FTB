// Taken from Francescos Git on 4/17/18 
#include "TLegend.h"
#include "TH1F.h"
#include <limits.h>
#include <Scalar.h>
namespace Frannamespace{

	short colors[7]={kRed,kBlue,kGreen+2,kMagenta+3,kOrange+4,kCyan+1,kMagenta-7};
	short styles[7]={kFullCircle,kOpenSquare,kFullTriangleUp,kFullDiamond,kFullCross,kFullStar};
	
	void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize, double angle) {
	  	TLatex l; //l.SetTextAlign(12); 
	  	l.SetTextSize(tsize); 
	  	l.SetNDC();
	 	l.SetTextColor(color);
	  	if (angle > 0) l.SetTextAngle(angle);
	  	l.DrawLatex(x,y,text);
	}	

	template<class T>
	T* zeroArray(int n,T* type){
		T* r = new T[n];
		for (int i = 0; i < n; ++i)
		{
			r[i] = 0;
		}
		return r;
	}
	void makeBins(float* bins, int min, int nBins, float width){
		for(int i=0; i<=nBins;++i){
			bins[i] = min + width*i;
		}
	}
	template<class T>
	T quadrature(T d1, T d2){
		return TMath::Sqrt((double)d1*d1+d2*d2);
	}
	template<class T>
	void divideArray(int n, T* a, T d){
		for (int i = 0; i < n; ++i)
		{
			a[i] /=d;
			//cout<<a[i]<<'\n';
		}
	}
	template<class T>
	void divideArray(int n, T* a, T* d){
		for (int i = 0; i < n; ++i)
		{
			a[i] /=d[i];

		}
	}
	template<class T>
	T errorDivide(T d1, T e1, T d2, T e2){
		T r = d1/d2;
		return r * quadrature(e1/d1,e2/d2);
	}
	template<class T>
	void errorDivideArray(int n,T* values, T* errors, T divisor, T divisorError){
		for (int i = 0; i < n; ++i)
		{
			errors[i] = errorDivide(values[i],errors[i],divisor,divisorError);
		}
		divideArray(n,values,divisor);
	}
	template<class T>
	void errorDivideArray(int n,T* values, T* errors, T* divisor, T* divisorError){
		for (int i = 0; i < n; ++i)
		{
			errors[i] = errorDivide(values[i],errors[i],divisor[i],divisorError[i]);
		}
		divideArray(n,values,divisor);
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
	/*returns every index i in an array where the i value is unequal to the value at i-1 always starts with 0 and ends with SIZE*/
	template<class T>
	vector<int>* sameValueIndices(const int SIZE, T* a){
		vector<int> *maxes = new vector<int>(0);
		maxes->push_back(0);
		if(SIZE<=1){
			maxes->push_back(0);
			return maxes;
		}
		T temp;
		temp = a[0];
		for (int i = 1; i < SIZE; ++i)
		{
			if(a[i]!=temp){
				maxes->push_back(i);
				temp=a[i];
				//cout<<i<<endl;
			}
		}
		maxes->push_back(SIZE);
		return maxes;
	}
	/* pass a vector of the indices from an array you want*/
	template<class T>
	T* valuesAt(T* a,std::vector<int> v){
		T* r = new T[v.size()];
		for (int i = 0; i < v.size(); ++i)
		{
			r[i] = a[v[i]];
		}
		return r;
	}
	/*inclusive*/
	template<class T>
	T* partialArray(T* a, int start, int end){
		T *r = new T[end-start+1];
		int count=0;
		for (int i = start; i <= end; ++i)
		{
			r[count++]=a[i];
		}
		return r;
	}
	template<class T>
	queue<int> arrayNonZero(T* a, int SIZE){
		queue<int> r;
		for (int i = 0; i < SIZE; ++i)
		{
			if (a[i]!=0)
			{
				r.push(i);
			}
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
	/*inclusive*/
	template<class T>
	queue<T> arrayToQueue(T* a, int start, int end){
		queue<T> r;
		while(start<=end){
			r.push(a[start]);
		}
		return r;
	}
	template<class T>
	queue<T> arrayToQueue(const int SIZE, T* a){
		queue<T> r;
		for(int i=0; i<SIZE;++i){
			r.push(a[i]);
		}
		return r;
	}
	/** returns a 2-D array where each subarray is the parts of a broken down by the output of maxes and partial Array*/
	/*template<class T>
	queue<queue<T>> maxBrokenArray(const int SIZE, T* a){
		vector<int> *maxis = maxes(SIZE,a);
		for (unsigned int i = 0; i < maxis->size(); ++i)
		{
			cout<<maxis->at(i)<<'\n';
		}
		cout<<endl;
		queue<queue<T>> out;
		for (int i = 0; i < maxis->size()-1; ++i)
		{
			out.push(arrayToQueue(a,(*maxis)[i],(*maxis)[i+1]));
		}
		return out;
	}*/
	template<class T>
	T* sqrtArray(int n, T* in){
		T* t= new T[n];
		for (int i = 0; i < n; ++i)
		{
			t[i] = TMath::Sqrt(in[i]);
		}
		return t;
	}
	template<class T>
	T* sigmaNtoUncertainty(int SIZE, T* sigma, T* n, T* sigmaerror){
		const float factor = 1.465; // the inverse of 0.6827 which is the percentage of the area under a gaussian when integrated over +- 1 sigma
		T *r = new T[SIZE];
		T *r2 = new T[SIZE];
		for (int i = 0; i < SIZE; ++i)
		{
			r[i]= factor*sigma[i];
			r2[i] = sigmaerror[i];
		}
		errorDivideArray(SIZE,r,r2,sqrtArray(SIZE,n),zeroArray(SIZE,n));
		return r;
	}
	template<class T>
	T max(int SIZE, T* x){
		T temp=0;
		for (int i = 0; i < SIZE; ++i)
		{
			if (x[i]>temp)
			{
				temp=x[i];
			}
		}
		return temp;
	}
	template<class T>
	T max(queue<T> in){
		T temp=0;
		while(!in.empty()){
			if(in.front()>=temp){
				temp=in.front();
			}
			in.pop();
		}
		return temp;
	}
	template<class T>
	T min(queue<T> in){
		T temp=-1;
		while(!in.empty()){
			if(in.front()<=temp||temp==-1){
				temp=in.front();
			}
			in.pop();
		}
		return temp;
	}
	template<class T>
	T average(int SIZE, T* x){
		T sum=0;
		for (int i = 0; i < SIZE; ++i)
		{
			sum+= x[i];
		}
		return sum/SIZE;
	}
	template<class T>
	T average(queue<T> in){
		T sum=0;
		const int SIZE = in.size();
		while(!in.empty()){
			sum+=in.front();
			in.pop();
		}
		return sum/SIZE;
	}
	/*float systematicError(const int SIZE, float* means){ // by extreme - mean over sqrt(3)
		return (max<float>(SIZE,means)-average<float>(SIZE,means))/TMath::Sqrt(3);
	}*/
	template<class T>
	T systematicError(queue<T> means){ // by extreme 
		if (means.size()<2)
		{
			return 0;
		}
		T top = max<T>(means);
		T bottom = min<T>(means);
		T a = average<T>(means);
		if (top-a>bottom-a)
		{
			return((top-a)/TMath::Sqrt(3));
		}
		else{
			return ((bottom-a)/TMath::Sqrt(3));
		}
	}
	void oleSwitcheroo(float* xp, float* yp)
	{
    	float temp = *xp;
    	*xp = *yp;
    	*yp = temp;
	}
	template<class T>
	queue<queue<T>> breakArray(T* a, std::vector<int> breaks){
		queue<queue<T>> r;
		int breakcounter=0;
		while(breaks.size()-breakcounter>1){
			//cout<<breaks.at(breakcounter)<<" and "<<breaks.at(breakcounter+1)<<endl;
			queue<T> temp;
			for (int i = breaks.at(breakcounter); i < breaks.at(breakcounter+1); ++i)
			{
				//cout<<a[i]<<endl;
				temp.push(a[i]);
			}
			r.push(temp);
			breakcounter++;
			//cout<<r.back().front()<<'\n';
		}
		return r;
	}
	template<class T>
	T queueAverage(queue<T> q){
		T r=0;
		const unsigned int SIZE = q.size();
		while(!q.empty()){
			r+=q.front();
			q.pop();
		}
		return r/SIZE;
	}
	template<class T>
	queue<T> averageList(queue<queue<T>> q){
		queue<T> r;
		while(!q.empty()){
			r.push(queueAverage(q.front()));
			q.pop();
		}
		return r;

	}

/*template<class T>
class Scalar
{
public:
	Scalar(T value){
		this->value=value;
	}
	Scalar(T value, T uncertainty){
		this->value=value;
		this->uncertainty=uncertainty;
	}
	~Scalar(); 
	//operators might need to return class types but I'm not sure
	Scalar<T> operator+(Scalar<T> s){
		Scalar<T> next;
		next.value = this->value + s.value;
		next.uncertainty = quadrature(this->uncertainty, s.uncertainty);
		return next;
	}
	Scalar<T> operator+(T s){
		Scalar<T> next;
		next.value = this->value + s;
		return next;
	}
	Scalar<T> operator-(Scalar<T> s){
		Scalar<T> next;
		next.value = this->value - s.value;
		next.uncertainty = quadrature(this->uncertainty, s.uncertainty);
		return next;
	}
	Scalar<T> operator-(T s){
		Scalar<T> next;
		next.value = this->value - s;
		return next;
	}
	void operator=(Scalar<T> s){
		value=s.value;
		uncertainty=s.uncertainty;
	}
	bool operator==(Scalar<T> s){
		return value==s.value;
	}
	Scalar<T> operator*(Scalar<T> s){
		Scalar<T> next;
		next.uncertainty = (value*s.value)*quadrature(this->uncertainty/value, s.uncertainty/s.value);
		next.value *= s.value;
		return next;
	}
	Scalar<T> operator*(T s){
		Scalar<T> next;
		next.value *= s;
		return next;
	}
	Scalar<T> operator/(Scalar<T> s){
		Scalar<T> next;
		next.uncertainty = (value/s.value)*quadrature(this->uncertainty/value, s.uncertainty/s.value);
		next.value /= s.value;
		return next;
	}
	Scalar<T> operator/(T s){
		Scalar<T> next;
		next.value /= s;
		return next;
	}
	Scalar<T> pow(double n){
		Scalar<T> r;
		if (n==0)
		{
			r.value=1;
			r.uncertainty=0;
			return r;
		}
		r.value = TMath::Power((double)value,n);
		r.uncertainty=(r.value*n*uncertainty/value);
		return r;
	}
private:
	T value;
	T uncertainty;
	
};*/
	/*template<class T>
	Scalar<T> systematicError(const int SIZE, Scalar<T>* means){ // by extreme - mean over sqrt(3)
		Scalar<T> max = max(SIZE,means);
		Scalar<T> average = average(SIZE, means);
		return (max-average)/TMath::Sqrt(3);
	}*/
}
