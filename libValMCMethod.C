//includes
#define libValMCMethod_cxx
#include "libValMCMethod.h"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <THStack.h>
#include <TCanvas.h>

//global masses

#define mumass  = 0.10566
#define cpimass = 0.13957
#define pi0mass = 0.13497
#define prtmass = 0.93827

//driver function

void libValMCMethod::Loop()
{
	//default root instruction space

	//   In a ROOT session, you can do:
	//      root> .L libValMCMethod.C
	//      root> libValMCMethod t
	//      root> t.GetEntry(12); // Fill t data members with entry number 12
	//      root> t.Show();       // Show values of entry 12
	//      root> t.Show(16);     // Read and show values of entry 16
	//      root> t.Loop();       // Loop on all entries
	//

	//     This is the loop skeleton where:
	//    jentry is the global entry number in the chain
	//    ientry is the entry number in the current Tree
	//  Note that the argument to GetEntry must be:
	//    jentry for TChain::GetEntry
	//    ientry for TTree::GetEntry and TBranch::GetEntry
	//
	//       To read only selected branches, Insert statements like:
	// METHOD1:
	//    fChain->SetBranchStatus("*",0);  // disable all branches
	//    fChain->SetBranchStatus("branchname",1);  // activate branchname
	// METHOD2: replace line
	//    fChain->GetEntry(jentry);       //read all branches
	//by  b_branchname->GetEntry(ientry); //read only this branch

	//Drawing estab. as per recommended style from Raquel

	gStyle->SetOptStat(0000);   
	gStyle->SetOptFit(1111);   
	gStyle->SetOptTitle(0);   
	gStyle->SetPadColor(kWhite);   
	gStyle->SetStatY(0.90);   
	gStyle->SetStatX(0.90);   
	gStyle->SetStatW(0.15);   
	gStyle->SetStatH(0.2);   
	gStyle->SetLabelSize(0.04,"X");   
	gStyle->SetLabelFont(62,"X");   
	gStyle->SetTitleSize(0.04,"X");   
	gStyle->SetTitleFont(62,"X");   
	gStyle->SetTitleOffset(0.85,"X");
  //  	gStyle->SetLabelOffset(0.015,"Y");   
	gStyle->SetLabelSize(0.03,"Y");   
	gStyle->SetLabelFont(62,"Y");   
	gStyle->SetTitleSize(0.04,"Y");   
	gStyle->SetTitleFont(62,"Y");   
	gStyle->SetTitleOffset(1.3,"Y");   
	gStyle->SetTitleX(0.22);   
	gStyle->SetTitleY(0.98);   
	gStyle->SetTitleSize(0.04,"t");   
	gStyle->SetTitleBorderSize(0);   
	gStyle->SetCanvasBorderSize(0);


	//creating various histograms for data of specific channels

	//Enu histograms

	TH1D* QES_Enu = new TH1D("QES_Enu","Neutrino energy for Quasi-elastic Scattering",100,0,7);  
	TH1D* NSBrRes_Enu = new TH1D("NSBrRes_Enu","Neutrino energy for Non-Strange Baryon Resonance",100,0,7);
	TH1D* PiNBg_Enu = new TH1D("PiNBg_Enu","Neutrino Energy for Pi-Neutron Background",100,0,7);
	TH1D* PiPBg_Enu = new TH1D("PiPBg_Enu","Neutrino Energy for Pi-Proton Background",100,0,7);
	TH1D* DIS_Enu = new TH1D("DIS_Enu","Neutrino Energy for Dip Inelastic Scattering",100,0,7);
	TH1D* DpDhQES_Enu = new TH1D("2p2hQES_Enu","Neutrino energy for 2p2h Quasi-elastic Scattering",100,0,7);
	TH1D* DpDhDelta_Enu = new TH1D("2p2hDelta_Enu","Neutrino energy for 2p2h Delta resonance",100,0,7);
	TH1D* DpBg_Enu = new TH1D("2pBg","Neutrino Energy for 2 pion background",100,0,7);

	//set indidivual colors/styles
	
	QES_Enu->SetFillColor(kYellow);
	NSBrRes_Enu->SetFillColor(kBlue);
	PiNBg_Enu->SetFillColor(kGreen);
	PiPBg_Enu->SetFillColor(kRed);
	DIS_Enu->SetFillColor(kPink);
	DpDhQES_Enu->SetFillColor(kMagenta);
	DpDhDelta_Enu->SetFillColor(kOrange);
	DpBg_Enu->SetFillColor(kTeal);

	//Declare variables and functions for data !!TO DO!!


	if (fChain == 0) return; //ugly- fix this

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	//main logic loop
	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break; //ugly- fix this
		nb = fChain->GetEntry(jentry);   
		nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		//switch to proper histogram for storage (prod_id check) //ugly as shit- refactor to better format later
		if(prod_id==1)
			QES_Enu->Fill(Enu);
		if(prod_id>=2 && prod_id<=31)
			NSBrRes_Enu->Fill(Enu);
		if(prod_id==32)
			PiNBg_Enu->Fill(Enu);
		if(prod_id==33)
			PiPBg_Enu->Fill(Enu);
		if(prod_id==34)
			DIS_Enu->Fill(Enu);
		if(prod_id==35)
			DpDhQES_Enu->Fill(Enu);
		if(prod_id==36)
			DpDhDelta_Enu->Fill(Enu);
		if(prod_id==37)
			DpBg_Enu->Fill(Enu);
		if(prod_id<1 || prod_id>37)
			std::cout<<"Weird Prod ID found: "<<prod_id<<std::endl;

	}

	//linking these filled histograms into a single stacked histogram

	//Enu stacking
	
	THStack *Stack_Enu = new THStack("Stack_Enu","All (read: numu-cc atm) production methods Nuetrino Energies");
	Stack_Enu->Add(DpDhQES_Enu);
	Stack_Enu->Add(QES_Enu);
	Stack_Enu->Add(NSBrRes_Enu);
	Stack_Enu->Add(PiPBg_Enu);
	Stack_Enu->Add(PiNBg_Enu);
	Stack_Enu->Add(DpBg_Enu);
	Stack_Enu->Add(DIS_Enu);
	Stack_Enu->Add(DpDhDelta_Enu);
	Stack_Enu->Draw("hist");
	Stack_Enu->GetXaxis()->SetTitle("Neutrino Energy (Enu)");
	Stack_Enu->GetYaxis()->SetTitle("Entries per Energy");

	//finding sum of entries for error check
	double postProcEntries=0;
	postProcEntries = DpDhQES_Enu->Integral();
	postProcEntries = postProcEntries + QES_Enu->Integral();
	postProcEntries = postProcEntries + NSBrRes_Enu->Integral();
	postProcEntries = postProcEntries + PiPBg_Enu->Integral();
	postProcEntries = postProcEntries + PiNBg_Enu->Integral();
	postProcEntries = postProcEntries + DpBg_Enu->Integral();
	postProcEntries = postProcEntries + DIS_Enu->Integral();
	postProcEntries = postProcEntries + DpDhDelta_Enu->Integral();

	//as of last check, graph was still displaying less than expected. How many histograms can a stack plot stack at a single time?
	//If it's limited, there is a chance some of the plots are not writing as expected/being pushed off. 

	std::cout<<"Total entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blob("histResults.root","RECREATE");
	Stack_Enu->Write();
	blob.Close();
}






