#define GenieLibValMCMethod_cxx
#include "GenieLibValMCMethod.h"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <THStack.h>
#include <TCanvas.h>

//Gobals

double MUMASS = 0.10566;
double CPIMASS = 0.13957;
double PI0MASS = 0.13497;
double PRTMASS = 0.93827;
double MUMASSSQR = MUMASS * MUMASS;

//misc functions

double Getw(double enu, double emu)
{
	return enu-emu;
}

//alt calculation for 4 momentum transfer
double GetQ2Method2(double pX, double pY, double pZ, double muE, double incNuEnergy)
{
	TLorentzVector incomingNeutrino(0,0,incNuEnergy,incNuEnergy);
	TLorentzVector resultantMuon(pX,pY,pZ,muE); 
	TLorentzVector q = incomingNeutrino-resultantMuon;	
	double mom4;
	mom4 = -q.Mag2();
	return mom4;
}

//invarient mass calculation 
double GetHadInvarientMass(double Q2, double w)
{
	return ((PRTMASS*PRTMASS)+(2*PRTMASS*w)-(Q2));
}

//x-bjorken scaling
double GetXBjorken(double Q2, double w)
{
	return ((Q2)/(2*PRTMASS*w));
}

//relative energy transfer
double GetY(double w, double enu)
{
	return (w/enu);
}

void GenieLibValMCMethod::Loop()
{
//   In a ROOT session, you can do:
//      root> .L GenieLibValMCMethod.C
//      root> GenieLibValMCMethod t
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

	TH1D* QES_w = new TH1D("QES_w","Energy transfer for Quasi-elastic Scattering",100,0,4);  
	TH1D* NSBrRes_w = new TH1D("NSBrRes_w","Energy transfer for Non-Strange Baryon Resonance",100,0,4);
	TH1D* PiNBg_w = new TH1D("PiNBg_w","Energy transfer for Pi-Neutron Background",100,0,4);
	TH1D* PiPBg_w = new TH1D("PiPBg_w","Energy transfer for Pi-Proton Background",100,0,4);
	TH1D* DIS_w = new TH1D("DIS_w","Energy transfer for Dip Inelastic Scattering",100,0,4);
	TH1D* DpDhQES_w = new TH1D("2p2hQES_w","Energy transfer for 2p2h Quasi-elastic Scattering",100,0,4);
	TH1D* DpDhDelta_w = new TH1D("2p2hDelta_w","Energy transfer for 2p2h Delta resonance",100,0,4);
	TH1D* DpBg_w = new TH1D("2pBg_w","Energy transfer for 2 pion background",100,0,4);
	TH1D* otherOOB_w = new TH1D("OOB_w","Energy transfer for out of bounds channels",100,0,4);

	//invarient hadronic mass

	TH1D* QES_W2 = new TH1D("QES_W2","invarient hadronic mass for Quasi-elastic Scattering",100,0,5);  
	TH1D* NSBrRes_W2 = new TH1D("NSBrRes_W2","invarient hadronic mass for Non-Strange Baryon Resonance",100,0,5);
	TH1D* PiNBg_W2 = new TH1D("PiNBg_W2","invarient hadronic mass for Pi-Neutron Background",100,0,5);
	TH1D* PiPBg_W2 = new TH1D("PiPBg_W2","invarient hadronic mass for Pi-Proton Background",100,0,5);
	TH1D* DIS_W2 = new TH1D("DIS_W2","invarient hadronic mass for Dip Inelastic Scattering",100,0,5);
	TH1D* DpDhQES_W2 = new TH1D("2p2hQES_W2","invarient hadronic mass for 2p2h Quasi-elastic Scattering",100,0,5);
	TH1D* DpDhDelta_W2 = new TH1D("2p2hDelta_W2","invarient hadronic mass for 2p2h Delta resonance",100,0,5);
	TH1D* DpBg_W2 = new TH1D("2pBg_W2","invarient hadronic mass for 2 pion background",100,0,5);
	TH1D* otherOOB_W2 = new TH1D("OOB_W2","invarient hadronic mass for out of bounds channels",100,0,5);

	//x-bjorken scaling
	TH1D* QES_x = new TH1D("QES_x","x-bjorken scaling for Quasi-elastic Scattering",100,0,7);  
	TH1D* NSBrRes_x = new TH1D("NSBrRes_x","x-bjorken scaling for Non-Strange Baryon Resonance",100,0,7);
	TH1D* PiNBg_x = new TH1D("PiNBg_x","x-bjorken scaling for Pi-Neutron Background",100,0,7);
	TH1D* PiPBg_x = new TH1D("PiPBg_x","x-bjorken scaling for Pi-Proton Background",100,0,7);
	TH1D* DIS_x = new TH1D("DIS_x","x-bjorken scaling for Dip Inelastic Scattering",100,0,7);
	TH1D* DpDhQES_x = new TH1D("2p2hQES_x","x-bjorken scalings for 2p2h Quasi-elastic Scattering",100,0,7);
	TH1D* DpDhDelta_x = new TH1D("2p2hDelta_x","x-bjorken scaling for 2p2h Delta resonance",100,0,7);
	TH1D* DpBg_x = new TH1D("2pBg_x","x-bjorken scaling for 2 pion background",100,0,7);
	TH1D* otherOOB_x = new TH1D("OOB_x","x-bjorken scaling for out of bounds channels",100,0,7);

	//relative energy transfer
	TH1D* QES_Y = new TH1D("QES_Y","relative energy transfer for Quasi-elastic Scattering",100,0,1);  
	TH1D* NSBrRes_Y = new TH1D("NSBrRes_Y","relative energy transfer for Non-Strange Baryon Resonance",100,0,1);
	TH1D* PiNBg_Y = new TH1D("PiNBg_Y","relative energy transfer for Pi-Neutron Background",100,0,1);
	TH1D* PiPBg_Y = new TH1D("PiPBg_Y","relative energy transfer for Pi-Proton Background",100,0,1);
	TH1D* DIS_Y = new TH1D("DIS_Y","relative energy transfer for Dip Inelastic Scattering",100,0,1);
	TH1D* DpDhQES_Y = new TH1D("2p2hQES_Y","relative energy transfer for 2p2h Quasi-elastic Scattering",100,0,1);
	TH1D* DpDhDelta_Y = new TH1D("2p2hDelta_Y","relative energy transfer for 2p2h Delta resonance",100,0,1);
	TH1D* DpBg_Y = new TH1D("2pBg_Y","relative energy transfer for 2 pion background",100,0,1);
	TH1D* otherOOB_Y = new TH1D("OOB_Y","relative energy transfer for out of bounds channels",100,0,1);

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

	QES_W2->SetFillColor(kYellow);
	NSBrRes_W2->SetFillColor(kBlue);
	PiNBg_W2->SetFillColor(kGreen);
	PiPBg_W2->SetFillColor(kRed);
	DIS_W2->SetFillColor(kPink);
	DpDhQES_W2->SetFillColor(kMagenta);
	DpDhDelta_W2->SetFillColor(kOrange);
	DpBg_W2->SetFillColor(kTeal);
	otherOOB_W2->SetFillColor(kBlack);

	QES_x->SetFillColor(kYellow);
	NSBrRes_x->SetFillColor(kBlue);
	PiNBg_x->SetFillColor(kGreen);
	PiPBg_x->SetFillColor(kRed);
	DIS_x->SetFillColor(kPink);
	DpDhQES_x->SetFillColor(kMagenta);
	DpDhDelta_x->SetFillColor(kOrange);
	DpBg_x->SetFillColor(kTeal);
	otherOOB_x->SetFillColor(kBlack);

	QES_Y->SetFillColor(kYellow);
	NSBrRes_Y->SetFillColor(kBlue);
	PiNBg_Y->SetFillColor(kGreen);
	PiPBg_Y->SetFillColor(kRed);
	DIS_Y->SetFillColor(kPink);
	DpDhQES_Y->SetFillColor(kMagenta);
	DpDhDelta_Y->SetFillColor(kOrange);
	DpBg_Y->SetFillColor(kTeal);
	otherOOB_Y->SetFillColor(kBlack);

	//actual logic

        if (fChain == 0) return;

        Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	int muonFound;
	std::cout<<"Starting Data Analysis and sort..."<<std::endl;

	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{
		if(jentry % 10000 == 0)
		{
			std::cout<<"Processing Entry Number "<<jentry<<std::endl;
		}
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		muonFound = 0;
		switch(prod_id)
		{
			case 1:
				QES_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id == 1 && muonFound == 0 && pdg[i] == 13)
					{
						QES_Q->Fill(Qsq,weight);
						QES_w->Fill(Getw(Enu,E[i]),weight);
						QES_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						QES_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						QES_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						NSBrRes_Q->Fill(Qsq,weight);
						NSBrRes_w->Fill(Getw(Enu,E[i]),weight);
						NSBrRes_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						NSBrRes_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						NSBrRes_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						PiNBg_Q->Fill(Qsq,weight);
						PiNBg_w->Fill(Getw(Enu,E[i]),weight);
						PiNBg_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						PiNBg_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						PiNBg_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						PiPBg_Q->Fill(Qsq,weight);
						PiPBg_w->Fill(Getw(Enu,E[i]),weight);
						PiPBg_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						PiPBg_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						PiPBg_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						DIS_Q->Fill(Qsq,weight);
						DIS_w->Fill(Getw(Enu,E[i]),weight);
						DIS_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DIS_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DIS_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						DpDhQES_Q->Fill(Qsq,weight);
						DpDhQES_w->Fill(Getw(Enu,E[i]),weight);
						DpDhQES_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpDhQES_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpDhQES_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						DpDhDelta_Q->Fill(Qsq,weight);
						DpDhDelta_w->Fill(Getw(Enu,E[i]),weight);
						DpDhDelta_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpDhDelta_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpDhDelta_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
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
						DpBg_Q->Fill(Qsq,weight);
						DpBg_w->Fill(Getw(Enu,E[i]),weight);
						DpBg_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpBg_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
						DpBg_Y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
						muonFound = 1;
					}
				}
			break;
		}
	}
	std::cout<<"Still Alive! Linking histograms..."<<std::endl;

	//linking these filled histograms into a single stacked histogram

	//Enu stacking
	TCanvas *Canvas_Enu = new TCanvas("Canvas_Enu");
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
	Stack_Enu->GetYaxis()->SetTitle("Entries at this Energy");

	TLegend *EnuLegend = new TLegend(0.5,0.7,0.9,0.9);
	EnuLegend->AddEntry(DpDhQES_Enu,"DpDhQES","f");
	EnuLegend->AddEntry(QES_Enu,"QES","f");
	EnuLegend->AddEntry(NSBrRes_Enu,"NSBrRes","f");
	EnuLegend->AddEntry(PiPBg_Enu,"PiPBg","f");
	EnuLegend->AddEntry(PiNBg_Enu,"PiNBg","f");
	EnuLegend->AddEntry(DpBg_Enu,"DpBg","f");
	EnuLegend->AddEntry(DIS_Enu,"DIS","f");
	EnuLegend->AddEntry(DpDhDelta_Enu,"DpDhDelta","f");
	EnuLegend->AddEntry(otherOOB_Enu,"otherOOB","f");
	EnuLegend->Draw();
	Canvas_Enu->Update();

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
	Canvas_Enu->SaveAs("histResultsE.png");

	//Q stacking
	TCanvas *Canvas_Q = new TCanvas("Canvas_Q");
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
	Stack_Q->GetYaxis()->SetTitle("Entries at this Q2");

	TLegend *Q2Legend = new TLegend(0.5,0.7,0.9,0.9);
	Q2Legend->AddEntry(DpDhQES_Q,"DpDhQES","f");
	Q2Legend->AddEntry(QES_Q,"QES","f");
	Q2Legend->AddEntry(NSBrRes_Q,"NSBrRes","f");
	Q2Legend->AddEntry(PiPBg_Q,"PiPBg","f");
	Q2Legend->AddEntry(PiNBg_Q,"PiNBg","f");
	Q2Legend->AddEntry(DpBg_Q,"DpBg","f");
	Q2Legend->AddEntry(DIS_Q,"DIS","f");
	Q2Legend->AddEntry(DpDhDelta_Q,"DpDhDelta","f");
	Q2Legend->AddEntry(otherOOB_Q,"otherOOB","f");
	Q2Legend->Draw();
	Canvas_Q->Update();
	
	
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
	Canvas_Q->SaveAs("histResultsQ.png");
	

	//w
	TCanvas *Canvas_w = new TCanvas("Canvas_w");
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
	Stack_w->GetYaxis()->SetTitle("Entries at this w");

	TLegend *WLegend = new TLegend(0.5,0.7,0.9,0.9);
	WLegend->AddEntry(DpDhQES_w,"DpDhQES","f");
	WLegend->AddEntry(QES_w,"QES","f");
	WLegend->AddEntry(NSBrRes_w,"NSBrRes","f");
	WLegend->AddEntry(PiPBg_w,"PiPBg","f");
	WLegend->AddEntry(PiNBg_w,"PiNBg","f");
	WLegend->AddEntry(DpBg_w,"DpBg","f");
	WLegend->AddEntry(DIS_w,"DIS","f");
	WLegend->AddEntry(DpDhDelta_w,"DpDhDelta","f");
	WLegend->AddEntry(otherOOB_w,"otherOOB","f");
	WLegend->Draw();
	Canvas_w->Update();

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
	Canvas_w->SaveAs("histResultsW.png");

	//W2
	TCanvas *Canvas_W2 = new TCanvas("Canvas_W2");
	THStack *Stack_W2 = new THStack("Stack_W2","All (read: numu-cc atm) production methods Hadronic Invariant mass");
	Stack_W2->Add(DpDhQES_W2);
	Stack_W2->Add(QES_W2);
	Stack_W2->Add(NSBrRes_W2);
	Stack_W2->Add(PiPBg_W2);
	Stack_W2->Add(PiNBg_W2);
	Stack_W2->Add(DpBg_W2);
	Stack_W2->Add(DIS_W2);
	Stack_W2->Add(DpDhDelta_W2);
	Stack_W2->Add(otherOOB_W2);
	Stack_W2->Draw("hist");
	Stack_W2->GetXaxis()->SetTitle("Hadronic Invariant mass (W2)");
	Stack_W2->GetYaxis()->SetTitle("Entries at this W2");

	TLegend *W2Legend = new TLegend(0.5,0.7,0.9,0.9);
	W2Legend->AddEntry(DpDhQES_W2,"DpDhQES","f");
	W2Legend->AddEntry(QES_W2,"QES","f");
	W2Legend->AddEntry(NSBrRes_W2,"NSBrRes","f");
	W2Legend->AddEntry(PiPBg_W2,"PiPBg","f");
	W2Legend->AddEntry(PiNBg_W2,"PiNBg","f");
	W2Legend->AddEntry(DpBg_W2,"DpBg","f");
	W2Legend->AddEntry(DIS_W2,"DIS","f");
	W2Legend->AddEntry(DpDhDelta_W2,"DpDhDelta","f");
	W2Legend->AddEntry(otherOOB_W2,"otherOOB","f");
	W2Legend->Draw();
	Canvas_W2->Update();

	//finding sum of entries for error check
	postProcEntries=0;
	postProcEntries = DpDhQES_W2->Integral();
	postProcEntries = postProcEntries + QES_W2->Integral();
	postProcEntries = postProcEntries + NSBrRes_W2->Integral();
	postProcEntries = postProcEntries + PiPBg_W2->Integral();
	postProcEntries = postProcEntries + PiNBg_W2->Integral();
	postProcEntries = postProcEntries + DpBg_W2->Integral();
	postProcEntries = postProcEntries + DIS_W2->Integral();
	postProcEntries = postProcEntries + DpDhDelta_W2->Integral();

	std::cout<<"Total entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blobW2("histResultsW2.root","RECREATE");
	Stack_W2->Write();
	blobW2.Close();
	Canvas_W2->SaveAs("histResultsW2.png");

	//x
	TCanvas *Canvas_x = new TCanvas("Canvas_x");
	THStack *Stack_x = new THStack("Stack_x","All (read: numu-cc atm) production methods x bjorken scaling");
	Stack_x->Add(DpDhQES_x);
	Stack_x->Add(QES_x);
	Stack_x->Add(NSBrRes_x);
	Stack_x->Add(PiPBg_x);
	Stack_x->Add(PiNBg_x);
	Stack_x->Add(DpBg_x);
	Stack_x->Add(DIS_x);
	Stack_x->Add(DpDhDelta_x);
	Stack_x->Add(otherOOB_x);
	Stack_x->Draw("hist");
	Stack_x->GetXaxis()->SetTitle("X Bjorken Scaling (x)");
	Stack_x->GetYaxis()->SetTitle("Entries at this x");

	TLegend *xLegend = new TLegend(0.5,0.7,0.9,0.9);
	xLegend->AddEntry(DpDhQES_x,"DpDhQES","f");
	xLegend->AddEntry(QES_x,"QES","f");
	xLegend->AddEntry(NSBrRes_x,"NSBrRes","f");
	xLegend->AddEntry(PiPBg_x,"PiPBg","f");
	xLegend->AddEntry(PiNBg_x,"PiNBg","f");
	xLegend->AddEntry(DpBg_x,"DpBg","f");
	xLegend->AddEntry(DIS_x,"DIS","f");
	xLegend->AddEntry(DpDhDelta_x,"DpDhDelta","f");
	xLegend->AddEntry(otherOOB_x,"otherOOB","f");
	xLegend->Draw();
	Canvas_x->Update();

	//finding sum of entries for error check
	postProcEntries=0;
	postProcEntries = DpDhQES_x->Integral();
	postProcEntries = postProcEntries + QES_x->Integral();
	postProcEntries = postProcEntries + NSBrRes_x->Integral();
	postProcEntries = postProcEntries + PiPBg_x->Integral();
	postProcEntries = postProcEntries + PiNBg_x->Integral();
	postProcEntries = postProcEntries + DpBg_x->Integral();
	postProcEntries = postProcEntries + DIS_x->Integral();
	postProcEntries = postProcEntries + DpDhDelta_x->Integral();

	std::cout<<"Entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile blobx("histResultsx.root","RECREATE");
	Stack_x->Write();
	blobx.Close();
	Canvas_x->SaveAs("histResultsx.png");

	//y
	TCanvas *Canvas_Y = new TCanvas("Canvas_y");
	THStack *Stack_Y = new THStack("Stack_y","All (read: numu-cc atm) production methods relative energy transfer");
	Stack_Y->Add(DpDhQES_Y);
	Stack_Y->Add(QES_Y);
	Stack_Y->Add(NSBrRes_Y);
	Stack_Y->Add(PiPBg_Y);
	Stack_Y->Add(PiNBg_Y);
	Stack_Y->Add(DpBg_Y);
	Stack_Y->Add(DIS_Y);
	Stack_Y->Add(DpDhDelta_Y);
	Stack_Y->Add(otherOOB_Y);
	Stack_Y->Draw("hist");
	Stack_Y->GetXaxis()->SetTitle("Relative Energy Transfer (y)");
	Stack_Y->GetYaxis()->SetTitle("Entries at this y");

	TLegend *yLegend = new TLegend(0.5,0.7,0.9,0.9);
	yLegend->AddEntry(DpDhQES_Y,"DpDhQES","f");
	yLegend->AddEntry(QES_Y,"QES","f");
	yLegend->AddEntry(NSBrRes_Y,"NSBrRes","f");
	yLegend->AddEntry(PiPBg_Y,"PiPBg","f");
	yLegend->AddEntry(PiNBg_Y,"PiNBg","f");
	yLegend->AddEntry(DpBg_Y,"DpBg","f");
	yLegend->AddEntry(DIS_Y,"DIS","f");
	yLegend->AddEntry(DpDhDelta_Y,"DpDhDelta","f");
	yLegend->AddEntry(otherOOB_Y,"otherOOB","f");
	yLegend->Draw();
	Canvas_Y->Update();

	//finding sum of entries for error check
	postProcEntries=0;
	postProcEntries = DpDhQES_Y->Integral();
	postProcEntries = postProcEntries + QES_Y->Integral();
	postProcEntries = postProcEntries + NSBrRes_Y->Integral();
	postProcEntries = postProcEntries + PiPBg_Y->Integral();
	postProcEntries = postProcEntries + PiNBg_Y->Integral();
	postProcEntries = postProcEntries + DpBg_Y->Integral();
	postProcEntries = postProcEntries + DIS_Y->Integral();
	postProcEntries = postProcEntries + DpDhDelta_Y->Integral();

	std::cout<<"Entries: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	TFile bloby("histResultsy.root","RECREATE");
	Stack_Y->Write();
	bloby.Close();
	Canvas_Y->SaveAs("histResultsy.png");
}
