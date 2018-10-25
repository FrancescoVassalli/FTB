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
TF1* makeGaus(TH1* pbglPlot){
	int maxbin = pbglPlot->GetMaximumBin();
	double gausLowBound = pbglPlot->GetBinLowEdge(maxbin);
	double temp = gausLowBound*.3; //sometimes this number needs to be ajusted to make the guas fit well
	double gausUpbound = gausLowBound +temp;
	if(gausUpbound >10000){gausUpbound=10000;} //so that fit doesn't exceed range of histogram
	gausLowBound -= temp; 
	Scalar mygaus[2];
	TF1* gaus; 
	gaus= new TF1("gaus","gaus",gausLowBound,gausUpbound);
	const float gausRange = 1.5; //how many rigma the gaussian is fit over
	pbglPlot->Fit(gaus,"QN","",gausLowBound,gausUpbound);       //“R” Use the range specified in the function range
	mygaus[0] = Scalar(gaus->GetParameter(1),gaus->GetParError(1)); //mean
	mygaus[1] = Scalar(gaus->GetParameter(2),gaus->GetParError(2)); //sigma
	recursiveGaus(pbglPlot, gaus, mygaus, gausRange,97);
	gausLowBound=mygaus[0]-(mygaus[1]*gausRange);
	gausUpbound=mygaus[0]+mygaus[1]*gausRange;
	//I wrap the hist and fit into these PlotWithLine classes 
	pbglPlot->GetXaxis()->SetRangeUser(mygaus[0]-(mygaus[1]*5.0),mygaus[0]+mygaus[1]*5.0);
	gaus->SetLineStyle(1);
	gaus->SetLineWidth(2);
	gaus->SetLineColor(kRed);
	return gaus;
}	

/*removes the contents of file2 from file1 then fits a guass*/
void SignalSubtraction(){
	//these strings can be edited to handle the files you make 
	int run=2127;
	string plotname1=std::to_string(run);
	string plotname2=plotname1;
	string base=std::to_string(run)+"-";
	string extension="pbglfinal.root";
	string name1 = base+std::to_string(1)+extension;
	string name2 = base+std::to_string(2)+extension;
	string outname= plotname1+"subtracted.pdf";
	

	//this is boilerplate 
	TFile* file1 = new TFile(name1.c_str(),"READ");
	TFile* file2 = new TFile(name2.c_str(),"READ");
	TH1 *hist1 = (TH1*) file1->Get(plotname1.c_str());
	TH1 *hist2 = (TH1*) file2->Get(plotname2.c_str());
	//subtract the hists then delete 2 for clairity all the data is in 1

	TCanvas *t3 = new TCanvas();
	hist1->Scale(1/hist1->Integral());
	hist2->Scale(1/hist2->Integral());
	hist1->Add(hist2,-1);
	delete hist2;
	//TF1* subfit = makeGaus(hist1);
	hist1->Draw();
	t3->SaveAs(outname.c_str());
	//subfit->Draw("same");
}
