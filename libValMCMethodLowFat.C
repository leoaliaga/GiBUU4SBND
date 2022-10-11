//includes
#define libValMCMethod_cxx
#include "libValMCMethod.h"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <THStack.h>
#include <TCanvas.h>

//global masses

double MUMASS = 0.10566;
double CPIMASS = 0.13957;
double PI0MASS = 0.13497;
double PRTMASS = 0.93827;
double MUMASSSQR = MUMASS * MUMASS;

//misc functions


//4 momentum transfer
double GetQ2(double enu, double emu, double pmuz)
{
	double pmuL = pmuz;
	double mom4 = 2*enu*(emu-pmuL);
	mom4 = mom4 - MUMASSSQR; 
	return mom4;
}

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
	TH1D* otherOOB_Enu = new TH1D("OOB_Enu","Neutrino Energy for out of bounds channels",100,0,7);

	//Q^2 histograms-- NYI

	TH1D* QES_Q = new TH1D("QES_Q","4-P transfer for Quasi-elastic Scattering",100,0,7);  
	TH1D* NSBrRes_Q = new TH1D("NSBrRes_Q","4-P transfer for Non-Strange Baryon Resonance",100,0,7);
	TH1D* PiNBg_Q = new TH1D("PiNBg_Q","4-P transfer for Pi-Neutron Background",100,0,7);
	TH1D* PiPBg_Q = new TH1D("PiPBg_Q","4-P transfer for Pi-Proton Background",100,0,7);
	TH1D* DIS_Q = new TH1D("DIS_Q","4-P transfer for Dip Inelastic Scattering",100,0,7);
	TH1D* DpDhQES_Q = new TH1D("2p2hQES_Q","4-P transfer for 2p2h Quasi-elastic Scattering",100,0,7);
	TH1D* DpDhDelta_Q = new TH1D("2p2hDelta_Q","4-P transfer for 2p2h Delta resonance",100,0,7);
	TH1D* DpBg_Q = new TH1D("2pBg_Q","4-P transfer for 2 pion background",100,0,7);
	TH1D* otherOOB_Q = new TH1D("OOB_Q","4-P transfer for out of bounds channels",100,0,7);

	//set indidivual colors/styles
	
	QES_Enu->SetFillColor(kYellow);
	NSBrRes_Enu->SetFillColor(kBlue);
	PiNBg_Enu->SetFillColor(kGreen);
	PiPBg_Enu->SetFillColor(kRed);
	DIS_Enu->SetFillColor(kPink);
	DpDhQES_Enu->SetFillColor(kMagenta);
	DpDhDelta_Enu->SetFillColor(kOrange);
	DpBg_Enu->SetFillColor(kTeal);
	otherOOB_Enu->SetFillColor(kBlack);

	QES_Q->SetFillColor(kYellow);
	NSBrRes_Q->SetFillColor(kBlue);
	PiNBg_Q->SetFillColor(kGreen);
	PiPBg_Q->SetFillColor(kRed);
	DIS_Q->SetFillColor(kPink);
	DpDhQES_Q->SetFillColor(kMagenta);
	DpDhDelta_Q->SetFillColor(kOrange);
	DpBg_Q->SetFillColor(kTeal);
	otherOOB_Q->SetFillColor(kBlack);

	if (fChain == 0) return; //ugly- fix this

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	//main logic loop

	std::cout<<"Starting Data Analysis and sort..."<<std::endl;

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
			for(int iter = 0; iter <= nparts; iter++)
			{
				QES_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id>=2 && prod_id<=31)
			NSBrRes_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				NSBrRes_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==32)
			PiNBg_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				PiNBg_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==33)
			PiPBg_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				PiPBg_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==34)
			DIS_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				DIS_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==35)
			DpDhQES_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				DpDhQES_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==36)
			DpDhDelta_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				DpDhDelta_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id==37)
			DpBg_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				DpBg_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		if(prod_id<1 || prod_id>37)
		{
			std::cout<<"Weird Prod ID found: "<<prod_id<<std::endl;
			otherOOB_Enu->Fill(Enu);
			for(int iter = 0; iter <= nparts; iter++)
			{
				otherOOB_Q->Fill(GetQ2(Enu,E[iter],pz[iter]));
			}
		}

	}

	std::cout<<"Still Alive! Linking histograms..."<<std::endl;

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
	Stack_Enu->Add(otherOOB_Enu);
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

	std::cout<<"Total entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blobE("histResultsE.root","RECREATE");
	Stack_Enu->Write();
	blobE.Close();

	//Q stacking
	THStack *Stack_Q = new THStack("Stack_Q","All (read: numu-cc atm) production methods 4-momentum transfer");
	Stack_Q->Add(DpDhQES_Q);
	Stack_Q->Add(QES_Q);
	Stack_Q->Add(NSBrRes_Q);
	Stack_Q->Add(PiPBg_Q);
	Stack_Q->Add(PiNBg_Q);
	Stack_Q->Add(DpBg_Q);
	Stack_Q->Add(DIS_Q);
	Stack_Q->Add(DpDhDelta_Q);
	Stack_Q->Add(otherOOB_Q);
	Stack_Q->Draw("hist");
	Stack_Q->GetXaxis()->SetTitle("4 momentum transfer (Q2)");
	Stack_Q->GetYaxis()->SetTitle("Entries per Q2");

	//finding sum of entries for error check
	postProcEntries=0;
	postProcEntries = DpDhQES_Q->Integral();
	postProcEntries = postProcEntries + QES_Q->Integral();
	postProcEntries = postProcEntries + NSBrRes_Q->Integral();
	postProcEntries = postProcEntries + PiPBg_Q->Integral();
	postProcEntries = postProcEntries + PiNBg_Q->Integral();
	postProcEntries = postProcEntries + DpBg_Q->Integral();
	postProcEntries = postProcEntries + DIS_Q->Integral();
	postProcEntries = postProcEntries + DpDhDelta_Q->Integral(); 

	std::cout<<"Total entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blobQ("histResultsQ.root","RECREATE");
	Stack_Q->Write();
	blobQ.Close();
}






