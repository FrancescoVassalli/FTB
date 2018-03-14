#include "TFile.h"
#include "TCanvas.h"
/*#include "AtlasStyle.h"
#include "AtlasUtils.h"*/
#include "TLegend.h"

using namespace std;

class DiffADC
{
public:
	DiffADC(TObjArray* peak, TObjArray* pedestal){
		this->peak = peak;
		this->pedestal = pedestal;
	}
	~DiffADC(){
		delete peak;
		delete pedestal;
	}
	int pmp(int i){
		return peak[i] - pedestal[i];
	}
private:
	TObjArray* peak;
	TObjArray *pedestal;
	
};

void BeamAnalysis1(){
	TFile *input = new TFile("beam_00000558-0000.root");
	TTree *maintree = (TTree*) input->Get("W;24");
	TBranch *tbranch = maintree->FindBranch("pedestal");
	TObjArray* peak;
	TObjArray* pedestal = tbranch->GetListOfLeaves();
	tbranch = maintree->FindBranch("peak");
	peak = tbranch->GetListOfLeaves();


}