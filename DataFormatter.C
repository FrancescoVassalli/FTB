
//this is used to organize the output text files in increasing beam energy 
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

void DataFormatter(){
//manuel data entry
/* 1200VA
const int NUMSIZE=4; 
float mean={2822.99,4119.50,1398.18,5428.33};
float meanU={5.87,5.62,7.63,5.89};
float sigma={104.24,115.72,56.64,149.61};
float sigmaU={8.07,6.91,7.87,7.06};
float energy={4,6,2,8};
*/
/* 1200VB
const int NUMSIZE=47; //not the right number is to enter manually
float mean={2822.99};
float meanU={5.87};
float sigma={104.24};
float sigmaU={8.07};
float energy={4};
*/
/* 1100VA
const int NUMSIZE=47; //not the right number is to enter manually
float mean={2878.16,1955.91,2902.93,3843.5};
float meanU={2.72,2.06,2.72,3.71};
float sigma={79.87,59.22,84.01,98.82};
float sigmaU={3.72,2.26,3.38,4.85};
float energy={12,8,12,16};
*/
/* 1100VB
const int NUMSIZE=47; //not the right number is to enter manually
float mean={2822.99};
float meanU={5.87};
float sigma={104.24};
float sigmaU={8.07};
float energy={4};
*/
/* 1000V
const int NUMSIZE=47; //not the right number is to enter manually
float mean={2822.99};
float meanU={5.87};
float sigma={104.24};
float sigmaU={8.07};
float energy={4};
*/





//organize the output file
	superArraySorter5000(energy,mean,meanU,sigma,sigmaU,number,NUMSIZE); //sort all arrays so that it goes in ascending energy order
	//output the file 
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
