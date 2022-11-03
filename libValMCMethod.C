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

//skeleton of an alt calculation for 4 momentum transfer
double GetQ2Method2(double pX, double pY, double pZ, double muE, double incNuEnergy)
{
	TLorentzVector incomingNeutrino(0,0,incNuEnergy,incNuEnergy);
	TLorentzVector resultantMuon(pX,pY,pZ,muE); 
	TLorentzVector q = incomingNeutrino-resultantMuon;	
	double mom4;
	mom4 = -q.Mag2();
	return mom4;
}

double Getw(double enu, double emu)
{
	return enu-emu;
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

	//Q^2 histograms

	TH1D* QES_Q = new TH1D("QES_Q","4-P transfer for Quasi-elastic Scattering",100,0,2);  
	TH1D* NSBrRes_Q = new TH1D("NSBrRes_Q","4-P transfer for Non-Strange Baryon Resonance",100,0,2);
	TH1D* PiNBg_Q = new TH1D("PiNBg_Q","4-P transfer for Pi-Neutron Background",100,0,2);
	TH1D* PiPBg_Q = new TH1D("PiPBg_Q","4-P transfer for Pi-Proton Background",100,0,2);
	TH1D* DIS_Q = new TH1D("DIS_Q","4-P transfer for Dip Inelastic Scattering",100,0,2);
	TH1D* DpDhQES_Q = new TH1D("2p2hQES_Q","4-P transfer for 2p2h Quasi-elastic Scattering",100,0,2);
	TH1D* DpDhDelta_Q = new TH1D("2p2hDelta_Q","4-P transfer for 2p2h Delta resonance",100,0,2);
	TH1D* DpBg_Q = new TH1D("2pBg_Q","4-P transfer for 2 pion background",100,0,2);
	TH1D* otherOOB_Q = new TH1D("OOB_Q","4-P transfer for out of bounds channels",100,0,2);

	//Energy Transfer 

	TH1D* QES_w = new TH1D("QES_w","Energy transfer for Quasi-elastic Scattering",100,0,7);  
	TH1D* NSBrRes_w = new TH1D("NSBrRes_w","Energy transfer for Non-Strange Baryon Resonance",100,0,7);
	TH1D* PiNBg_w = new TH1D("PiNBg_w","Energy transfer for Pi-Neutron Background",100,0,7);
	TH1D* PiPBg_w = new TH1D("PiPBg_w","Energy transfer for Pi-Proton Background",100,0,7);
	TH1D* DIS_w = new TH1D("DIS_w","Energy transfer for Dip Inelastic Scattering",100,0,7);
	TH1D* DpDhQES_w = new TH1D("2p2hQES_w","Energy transfer for 2p2h Quasi-elastic Scattering",100,0,7);
	TH1D* DpDhDelta_w = new TH1D("2p2hDelta_w","Energy transfer for 2p2h Delta resonance",100,0,7);
	TH1D* DpBg_w = new TH1D("2pBg_w","Energy transfer for 2 pion background",100,0,7);
	TH1D* otherOOB_w = new TH1D("OOB_w","Energy transfer for out of bounds channels",100,0,7);

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

	QES_w->SetFillColor(kYellow);
	NSBrRes_w->SetFillColor(kBlue);
	PiNBg_w->SetFillColor(kGreen);
	PiPBg_w->SetFillColor(kRed);
	DIS_w->SetFillColor(kPink);
	DpDhQES_w->SetFillColor(kMagenta);
	DpDhDelta_w->SetFillColor(kOrange);
	DpBg_w->SetFillColor(kTeal);
	otherOOB_w->SetFillColor(kBlack);

	if (fChain == 0) return; //ugly- fix this

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	//main logic loop
	int muonFound;
	std::cout<<"Starting Data Analysis and sort..."<<std::endl;
	nentries = 10000;

	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break; 
		nb = fChain->GetEntry(jentry);   
		nbytes += nb;
		muonFound = 0;
		switch(prod_id)
		{
			case 1:
				QES_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id == 1 && muonFound == 0 && pdg[i] == 13)
					{
						QES_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						QES_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 2 ... 31:
				NSBrRes_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id>=2 && prod_id<=31 && pdg[i] == 13 && muonFound == 0)
					{
						NSBrRes_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						NSBrRes_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 32:
				PiNBg_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==32 && muonFound == 0 && pdg[i] == 13)
					{
						PiNBg_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						PiNBg_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 33:
				PiPBg_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==33 && muonFound == 0 && pdg[i] == 13)
					{
						PiPBg_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
						PiPBg_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 34:
				DIS_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==34 && muonFound == 0 && pdg[i] == 13)
					{
						DIS_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						DIS_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 35:
				DpDhQES_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==35 && muonFound == 0 && pdg[i] == 13)
					{
						DpDhQES_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						DpDhQES_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 36:
				DpDhDelta_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==36 && muonFound == 0 && pdg[i] == 13)
					{
						DpDhDelta_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						DpDhDelta_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
			case 37:
				DpBg_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id==37 && muonFound == 0 && pdg[i] == 13)
					{
						DpBg_Q->Fill(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),weight);
						DpBg_w->Fill(Getw(Enu,E[i]),weight);
						muonFound = 1;
					}
				}
			break;
		}
	}

	//old format- holding JIC
	/*for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break; 
		nb = fChain->GetEntry(jentry);   
		nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		//switch to proper histogram for storage (prod_id check) 
		if(prod_id==1)
		{
			QES_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					QES_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}	
		}
		if(prod_id>=2 && prod_id<=31)
		{
			NSBrRes_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					NSBrRes_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}
		}	
		if(prod_id==32)
		{
			PiNBg_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					PiNBg_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}
		}	
		if(prod_id==33)
		{
			PiPBg_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					PiPBg_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}		
		}
		if(prod_id==34)
		{
			DIS_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					DIS_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}		
		}	
		if(prod_id==35)
		{
			DpDhQES_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					DpDhQES_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}
		}	
		if(prod_id==36)
		{
			DpDhDelta_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					DpDhDelta_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}
		}	
		if(prod_id==37)
		{
			DpBg_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					DpBg_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}		
		}	
		if(prod_id<1 || prod_id>37)
		{
			std::cout<<"Weird Prod ID found: "<<prod_id<<std::endl;
			otherOOB_Enu->Fill(Enu,weight);
			for(int i = 0; i < nparts; i++)
			{
				if(pdg[i] == 13)
				{
					otherOOB_Q->Fill(GetQ2(Enu, E[i], pz[i]),weight);
				}
			}
			
		}

	}*/

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
	
	THStack *Stack_w = new THStack("Stack_w","All (read: numu-cc atm) production methods energy transfer");
	Stack_w->Add(DpDhQES_w);
	Stack_w->Add(QES_w);
	Stack_w->Add(NSBrRes_w);
	Stack_w->Add(PiPBg_w);
	Stack_w->Add(PiNBg_w);
	Stack_w->Add(DpBg_w);
	Stack_w->Add(DIS_w);
	Stack_w->Add(DpDhDelta_w);
	Stack_w->Add(otherOOB_w);
	Stack_w->Draw("hist");
	Stack_w->GetXaxis()->SetTitle("Energy transfer (w)");
	Stack_w->GetYaxis()->SetTitle("Entries per w");

	//finding sum of entries for error check
	postProcEntries=0;
	postProcEntries = DpDhQES_w->Integral();
	postProcEntries = postProcEntries + QES_w->Integral();
	postProcEntries = postProcEntries + NSBrRes_w->Integral();
	postProcEntries = postProcEntries + PiPBg_w->Integral();
	postProcEntries = postProcEntries + PiNBg_w->Integral();
	postProcEntries = postProcEntries + DpBg_w->Integral();
	postProcEntries = postProcEntries + DIS_w->Integral();
	postProcEntries = postProcEntries + DpDhDelta_w->Integral();

	std::cout<<"Total entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blobw("histResultsw.root","RECREATE");
	Stack_w->Write();
	blobw.Close();
}







