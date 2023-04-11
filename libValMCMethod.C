//includes
#define libValMCMethod_cxx
#include "libValMCMethod.h"
#include <string.h>
#include <cmath>
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
double ELMASS = 0.00051;
double NUMASS = 0;
double ELMASSSQR = ELMASS * ELMASS;
double MUMASSSQR = MUMASS * MUMASS;

//misc functions


//4 momentum transfer
double GetQ2(double enu, double emu, double pmuz, int lepID)
{
	double pmuL = pmuz;
	double mom4 = 2*enu*(emu-pmuL);
	switch(lepID)
	{
		case 11:
		case -11:
			mom4 = mom4 - ELMASSSQR; 
		break;
		case 13:
		case -13:
			mom4 = mom4 - MUMASSSQR;
		break;
		case 14:
		case -14:
			mom4 = mom4 - NUMASS;
		break;
	}
	return mom4;
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

double Getw(double enu, double emu)
{
	return enu-emu;
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

	//setup lepton general matching
	
	int leptonId = 0;

	if(strcmp("Ar40/cc/nu_mu",nameDir)==0)
	{
		leptonId = 13;
	}
	else
	if(strcmp("Ar40/cc/nu_e",nameDir)==0)
	{
		leptonId = 11;
	}
	else
	if(strcmp("Ar40/nc/nu",nameDir)==0)
	{
		leptonId = 14;
	}
	else
	if(strcmp("Ar40/nc/nu_bar",nameDir)==0)
	{
		leptonId = -14;
	}
	else
	if(strcmp("Ar40/cc/nu_mu_bar",nameDir)==0)
	{
		leptonId = -13;
	}
	else
	if(strcmp("Ar40/cc/nu_e_bar",nameDir)==0)
	{
		leptonId = -11;
	}
	if(leptonId == 0)
	{
		std::cout<<"Error finding Lepton! directory extension = "<<nameDir<<std::endl;
	}

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

	TH1D* QES_Enu = new TH1D("QES_Enu","Neutrino energy for Quasi-elastic Scattering",100,0,5);  
	TH1D* NSBrRes_Enu = new TH1D("NSBrRes_Enu","Neutrino energy for Non-Strange Baryon Resonance",100,0,5);
	TH1D* PiNBg_Enu = new TH1D("PiNBg_Enu","Neutrino Energy for Pi-Neutron Background",100,0,5);
	TH1D* PiPBg_Enu = new TH1D("PiPBg_Enu","Neutrino Energy for Pi-Proton Background",100,0,5);
	TH1D* DIS_Enu = new TH1D("DIS_Enu","Neutrino Energy for Dip Inelastic Scattering",100,0,5);
	TH1D* DpDhQES_Enu = new TH1D("2p2hQES_Enu","Neutrino energy for 2p2h Quasi-elastic Scattering",100,0,5);
	TH1D* DpDhDelta_Enu = new TH1D("2p2hDelta_Enu","Neutrino energy for 2p2h Delta resonance",100,0,5);
	TH1D* DpBg_Enu = new TH1D("2pBg","Neutrino Energy for 2 pion background",100,0,5);
	TH1D* otherOOB_Enu = new TH1D("OOB_Enu","Neutrino Energy for out of bounds channels",100,0,5);

	//Q^2 histograms

	TH1D* QES_Q = new TH1D("QES_Q^{2}","4-P transfer for Quasi-elastic Scattering",100,0,2);  
	TH1D* NSBrRes_Q = new TH1D("NSBrRes_Q^{2}","4-P transfer for Non-Strange Baryon Resonance",100,0,2);
	TH1D* PiNBg_Q = new TH1D("PiNBg_Q^{2}","4-P transfer for Pi-Neutron Background",100,0,2);
	TH1D* PiPBg_Q = new TH1D("PiPBg_Q^{2}","4-P transfer for Pi-Proton Background",100,0,2);
	TH1D* DIS_Q = new TH1D("DIS_Q^{2}","4-P transfer for Dip Inelastic Scattering",100,0,2);
	TH1D* DpDhQES_Q = new TH1D("2p2hQES_Q^{2}","4-P transfer for 2p2h Quasi-elastic Scattering",100,0,2);
	TH1D* DpDhDelta_Q = new TH1D("2p2hDelta_Q^{2}","4-P transfer for 2p2h Delta resonance",100,0,2);
	TH1D* DpBg_Q = new TH1D("2pBg_Q^{2}","4-P transfer for 2 pion background",100,0,2);
	TH1D* otherOOB_Q = new TH1D("OOB_Q^{2}","4-P transfer for out of bounds channels",100,0,2);

	//Energy Transfer 

	TH1D* QES_w = new TH1D("QES_#omega","Energy transfer for Quasi-elastic Scattering",100,0,2);  
	TH1D* NSBrRes_w = new TH1D("NSBrRes_#omega","Energy transfer for Non-Strange Baryon Resonance",100,0,2);
	TH1D* PiNBg_w = new TH1D("PiNBg_#omega","Energy transfer for Pi-Neutron Background",100,0,2);
	TH1D* PiPBg_w = new TH1D("PiPBg_#omega","Energy transfer for Pi-Proton Background",100,0,2);
	TH1D* DIS_w = new TH1D("DIS_#omega","Energy transfer for Dip Inelastic Scattering",100,0,2);
	TH1D* DpDhQES_w = new TH1D("2p2hQES_#omega","Energy transfer for 2p2h Quasi-elastic Scattering",100,0,2);
	TH1D* DpDhDelta_w = new TH1D("2p2hDelta_#omega","Energy transfer for 2p2h Delta resonance",100,0,2);
	TH1D* DpBg_w = new TH1D("2pBg_#omega","Energy transfer for 2 pion background",100,0,2);
	TH1D* otherOOB_w = new TH1D("OOB_#omega","Energy transfer for out of bounds channels",100,0,2);

	//invarient hadronic mass

	TH1D* QES_W2 = new TH1D("QES_W^{2}","invarient hadronic mass for Quasi-elastic Scattering",100,0,4);  
	TH1D* NSBrRes_W2 = new TH1D("NSBrRes_W^{2}","invarient hadronic mass for Non-Strange Baryon Resonance",100,0,4);
	TH1D* PiNBg_W2 = new TH1D("PiNBg_W^{2}","invarient hadronic mass for Pi-Neutron Background",100,0,4);
	TH1D* PiPBg_W2 = new TH1D("PiPBg_W^{2}","invarient hadronic mass for Pi-Proton Background",100,0,4);
	TH1D* DIS_W2 = new TH1D("DIS_W^{2}","invarient hadronic mass for Dip Inelastic Scattering",100,0,4);
	TH1D* DpDhQES_W2 = new TH1D("2p2hQES_W^{2}","invarient hadronic mass for 2p2h Quasi-elastic Scattering",100,0,4);
	TH1D* DpDhDelta_W2 = new TH1D("2p2hDelta_W^{2}","invarient hadronic mass for 2p2h Delta resonance",100,0,4);
	TH1D* DpBg_W2 = new TH1D("2pBg_W^{2}","invarient hadronic mass for 2 pion background",100,0,4);
	TH1D* otherOOB_W2 = new TH1D("OOB_W^{2}","invarient hadronic mass for out of bounds channels",100,0,4);

	//x-bjorken scaling
	TH1D* QES_x = new TH1D("QES_x","x-bjorken scaling for Quasi-elastic Scattering",100,0,4);  
	TH1D* NSBrRes_x = new TH1D("NSBrRes_x","x-bjorken scaling for Non-Strange Baryon Resonance",100,0,4);
	TH1D* PiNBg_x = new TH1D("PiNBg_x","x-bjorken scaling for Pi-Neutron Background",100,0,4);
	TH1D* PiPBg_x = new TH1D("PiPBg_x","x-bjorken scaling for Pi-Proton Background",100,0,4);
	TH1D* DIS_x = new TH1D("DIS_x","x-bjorken scaling for Dip Inelastic Scattering",100,0,4);
	TH1D* DpDhQES_x = new TH1D("2p2hQES_x","x-bjorken scalings for 2p2h Quasi-elastic Scattering",100,0,4);
	TH1D* DpDhDelta_x = new TH1D("2p2hDelta_x","x-bjorken scaling for 2p2h Delta resonance",100,0,4);
	TH1D* DpBg_x = new TH1D("2pBg_x","x-bjorken scaling for 2 pion background",100,0,4);
	TH1D* otherOOB_x = new TH1D("OOB_x","x-bjorken scaling for out of bounds channels",100,0,4);

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


	//total histograms for channel-normalized data

	
	TH1D* Tot_Enu = new TH1D("Tot_Enu","Neutrino energy",100,0,5);
	TH1D* Tot_Q = new TH1D("Tot_Q2","4-P transfer",100,0,2);
	TH1D* Tot_w = new TH1D("Tot_w","Energy transfer",100,0,2);
	TH1D* Tot_W2 = new TH1D("Tot_W2","invarient hadronic mass",100,0,4);
	TH1D* Tot_x = new TH1D("Tot_x","x-bjorken scaling",100,0,4);
	TH1D* Tot_y = new TH1D("Tot_y","relative energy transfer",100,0,1);

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

	Tot_y->SetFillColor(kYellow);
	Tot_x->SetFillColor(kBlue);
	Tot_W2->SetFillColor(kGreen);
	Tot_w->SetFillColor(kRed);
	Tot_Q->SetFillColor(kPink);
	Tot_Enu->SetFillColor(kMagenta);

	if (fChain == 0) return; 

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	//main logic loop
	int muonFound;
	std::cout<<"Starting Data Analysis and sort..."<<std::endl;
	

	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{

		if(jentry % 1000000 == 0)
		{
			std::cout<<"Processing Entry Number "<<jentry<<std::endl;
		}
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break; 
		nb = fChain->GetEntry(jentry);   
		nbytes += nb;
		muonFound = 0;

		Tot_Enu->Fill(Enu,weight);
		for(int i = 0; i < nparts; i++)
		{
			if(muonFound == 0) //no lepton check needed- this is channel-agnostic
			{
				Tot_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
				Tot_w->Fill(Getw(Enu,E[i]),weight);
				Tot_W2->Fill(GetHadInvarientMass(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
				Tot_x->Fill(GetXBjorken(GetQ2Method2(px[i], py[i], pz[i], E[i], Enu),Getw(Enu,E[i])),weight);
				Tot_y->Fill(GetY(Getw(Enu,E[i]),Enu),weight);
				muonFound = 1;
			}
		}
		
		muonFound = 0;
		switch(prod_id)
		{
			case 1:
				QES_Enu->Fill(Enu,weight);
				for(int i = 0; i < nparts; i++)
				{
					if(prod_id == 1 && muonFound == 0 && pdg[i] == leptonId)
					{
						QES_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id>=2 && prod_id<=31 && pdg[i] == leptonId && muonFound == 0)
					{
						NSBrRes_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==32 && muonFound == 0 && pdg[i] == leptonId)
					{
						PiNBg_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==33 && muonFound == 0 && pdg[i] == leptonId)
					{
						PiPBg_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==34 && muonFound == 0 && pdg[i] == leptonId)
					{
						DIS_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==35 && muonFound == 0 && pdg[i] == leptonId)
					{
						DpDhQES_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==36 && muonFound == 0 && pdg[i] == leptonId)
					{
						DpDhDelta_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
					if(prod_id==37 && muonFound == 0 && pdg[i] == leptonId)
					{
						DpBg_Q->Fill(GetQ2(Enu, E[i], pz[i],leptonId),weight);
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
	TCanvas *Canvas_Enu = new TCanvas("Canvas_Enu");
	THStack *Stack_Enu = new THStack("Stack_Enu","Selected production methods Nuetrino Energies");
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
	Stack_Enu->GetXaxis()->SetTitle("Neutrino Energy (E_{#nu})");
	Stack_Enu->GetYaxis()->SetTitle("#sigma at this Energy");

	TLegend *EnuLegend = new TLegend(0.6,0.5,0.9,0.9);
	EnuLegend->AddEntry(DpDhQES_Enu,"2p2hQES","f");
	EnuLegend->AddEntry(QES_Enu,"QES","f");
	EnuLegend->AddEntry(NSBrRes_Enu,"NSBrRes","f");
	EnuLegend->AddEntry(PiPBg_Enu,"PiPBg","f");
	EnuLegend->AddEntry(PiNBg_Enu,"PiNBg","f");
	EnuLegend->AddEntry(DpBg_Enu,"2pBg","f");
	EnuLegend->AddEntry(DIS_Enu,"DIS","f");
	EnuLegend->AddEntry(DpDhDelta_Enu,"2p2hDelta","f");
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


	
	std::cout<<"Cross-section: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	
	//todo! generalizing names!
	
	TFile blob(fileNameOut,"RECREATE");
	Stack_Enu->Write();
	//blobE.Close();
	Canvas_Enu->SaveAs("histResultsE.png");

	//Q stacking
	TCanvas *Canvas_Q = new TCanvas("Canvas_Q");
	THStack *Stack_Q = new THStack("Stack_Q","Selected production methods 4-momentum transfer");
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
	Stack_Q->GetXaxis()->SetTitle("4 momentum transfer (Q^{2})");
	Stack_Q->GetYaxis()->SetTitle("#sigma at this Q^{2}");

	TLegend *Q2Legend = new TLegend(0.3,0.5,0.9,0.9);
	Q2Legend->AddEntry(DpDhQES_Q,"2p2hQES","f");
	Q2Legend->AddEntry(QES_Q,"QES","f");
	Q2Legend->AddEntry(NSBrRes_Q,"NSBrRes","f");
	Q2Legend->AddEntry(PiPBg_Q,"PiPBg","f");
	Q2Legend->AddEntry(PiNBg_Q,"PiNBg","f");
	Q2Legend->AddEntry(DpBg_Q,"2pBg","f");
	Q2Legend->AddEntry(DIS_Q,"DIS","f");
	Q2Legend->AddEntry(DpDhDelta_Q,"2p2hDelta","f");
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

	std::cout<<"Cross-section: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	
	//TFile blobQ("histResultsQ.root","RECREATE");
	blob.cd();
	Stack_Q->Write();
	//blobQ.Close();
	Canvas_Q->SaveAs("histResultsQ.png");
	

	//w
	TCanvas *Canvas_w = new TCanvas("Canvas_w");
	THStack *Stack_w = new THStack("Stack_w","Selected production methods energy transfer");
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
	Stack_w->GetXaxis()->SetTitle("Energy transfer (#omega)");
	Stack_w->GetYaxis()->SetTitle("#sigma at this #omega");

	TLegend *wLegend = new TLegend(0.3,0.5,0.9,0.9);
	wLegend->AddEntry(DpDhQES_w,"2p2hQES","f");
	wLegend->AddEntry(QES_w,"QES","f");
	wLegend->AddEntry(NSBrRes_w,"NSBrRes","f");
	wLegend->AddEntry(PiPBg_w,"PiPBg","f");
	wLegend->AddEntry(PiNBg_w,"PiNBg","f");
	wLegend->AddEntry(DpBg_w,"2pBg","f");
	wLegend->AddEntry(DIS_w,"DIS","f");
	wLegend->AddEntry(DpDhDelta_w,"2p2hDelta","f");
	wLegend->AddEntry(otherOOB_w,"otherOOB","f");
	wLegend->Draw();
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

	std::cout<<"Cross-section: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	//TFile blobw("histResultsw.root","RECREATE");
	blob.cd();
	Stack_w->Write();
	//blobw.Close();
	Canvas_w->SaveAs("histResultsW.png");

	//W2
	TCanvas *Canvas_W2 = new TCanvas("Canvas_W2");
	THStack *Stack_W2 = new THStack("Stack_W^{2}","Selected production methods Hadronic Invariant mass");
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
	Stack_W2->GetXaxis()->SetTitle("Hadronic Invariant mass (W^{2})");
	Stack_W2->GetYaxis()->SetTitle("#sigma at this W^{2}");

	TLegend *WLegend = new TLegend(0.5,0.5,0.9,0.9);
	WLegend->AddEntry(DpDhQES_W2,"2p2hQES","f");
	WLegend->AddEntry(QES_W2,"QES","f");
	WLegend->AddEntry(NSBrRes_W2,"NSBrRes","f");
	WLegend->AddEntry(PiPBg_W2,"PiPBg","f");
	WLegend->AddEntry(PiNBg_W2,"PiNBg","f");
	WLegend->AddEntry(DpBg_W2,"2pBg","f");
	WLegend->AddEntry(DIS_W2,"DIS","f");
	WLegend->AddEntry(DpDhDelta_W2,"2p2hDelta","f");
	WLegend->AddEntry(otherOOB_W2,"otherOOB","f");
	WLegend->Draw();
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
	//TFile blobW2("histResultsW2.root","RECREATE");
	blob.cd();
	Stack_W2->Write();
	//blobW2.Close();
	Canvas_W2->SaveAs("histResultsW2.png");

	//x
	TCanvas *Canvas_x = new TCanvas("Canvas_x");
	THStack *Stack_x = new THStack("Stack_x","Selected production methods x bjorken scaling");
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
	Stack_x->GetXaxis()->SetTitle("x-Bjorken Scaling (x)");
	Stack_x->GetYaxis()->SetTitle("#sigma at this x-Bjorken Scaling");

	TLegend *xLegend = new TLegend(0.4,0.5,0.9,0.9);
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

	std::cout<<"Cross-section: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	//TFile blobx("histResultsx.root","RECREATE");
	blob.cd();
	Stack_x->Write();
	//blobx.Close();
	Canvas_x->SaveAs("histResultsx.png");

	//y
	TCanvas *Canvas_Y = new TCanvas("Canvas_y");
	THStack *Stack_Y = new THStack("Stack_y","Selected production methods relative energy transfer");
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
	Stack_Y->GetYaxis()->SetTitle("#sigma at this y");

	TLegend *yLegend = new TLegend(0.7,0.5,0.9,0.9);
	yLegend->AddEntry(DpDhQES_Y,"2p2hQES","f");
	yLegend->AddEntry(QES_Y,"QES","f");
	yLegend->AddEntry(NSBrRes_Y,"NSBrRes","f");
	yLegend->AddEntry(PiPBg_Y,"PiPBg","f");
	yLegend->AddEntry(PiNBg_Y,"PiNBg","f");
	yLegend->AddEntry(DpBg_Y,"2pBg","f");
	yLegend->AddEntry(DIS_Y,"DIS","f");
	yLegend->AddEntry(DpDhDelta_Y,"2p2hDelta","f");
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

	std::cout<<"Cross-section: "<<postProcEntries<<std::endl;
	//Saving and cleaning up
	//TFile bloby("histResultsy.root","RECREATE");
	blob.cd();
	Stack_Y->Write();
	blob.Close();
	Canvas_Y->SaveAs("histResultsy.png");

	//total histogram ratios- displays entries by channel type
	//enu
	TH1D* QES_Enu_R = (TH1D*) QES_Enu->Clone();
	QES_Enu_R->Divide(Tot_Enu);
	TH1D* NSBrRes_Enu_R = (TH1D*) NSBrRes_Enu->Clone();
	NSBrRes_Enu_R->Divide(Tot_Enu);
	TH1D* PiNBg_Enu_R = (TH1D*) PiNBg_Enu->Clone();
	PiNBg_Enu_R->Divide(Tot_Enu);
	TH1D* PiPBg_Enu_R = (TH1D*) PiPBg_Enu->Clone();
	PiPBg_Enu_R->Divide(Tot_Enu);
	TH1D* DIS_Enu_R = (TH1D*) DIS_Enu->Clone();
	DIS_Enu_R->Divide(Tot_Enu);
	TH1D* DpDhQES_Enu_R = (TH1D*) DpDhQES_Enu->Clone();
	DpDhQES_Enu_R->Divide(Tot_Enu);
	TH1D* DpDhDelta_Enu_R = (TH1D*) DpDhDelta_Enu->Clone();
	DpDhDelta_Enu_R->Divide(Tot_Enu);
	TH1D* DpBg_Enu_R = (TH1D*) DpBg_Enu->Clone();
	DpBg_Enu_R->Divide(Tot_Enu);
	//Enu styling
	QES_Enu_R->SetFillColor(kYellow);
	NSBrRes_Enu_R->SetFillColor(kPink);
	PiNBg_Enu_R->SetFillColor(kGreen);
	PiPBg_Enu_R->SetFillColor(kRed);
	DIS_Enu_R->SetFillColor(kBlue);
	DpDhQES_Enu_R->SetFillColor(kMagenta);
	DpDhDelta_Enu_R->SetFillColor(kOrange);
	DpBg_Enu_R->SetFillColor(kTeal);
	//Enu stack
	TCanvas *Canvas_Enu_R = new TCanvas("Canvas_Enu_R");
	THStack *Stack_Enu_R = new THStack("Stack_Enu_R","Ratio of channel-specific neutrino energies normalized to the total");
	Stack_Enu_R->Add(DpDhQES_Enu_R);
	Stack_Enu_R->Add(NSBrRes_Enu_R);
	Stack_Enu_R->Add(QES_Enu_R);
	Stack_Enu_R->Add(PiPBg_Enu_R);
	Stack_Enu_R->Add(PiNBg_Enu_R);
	Stack_Enu_R->Add(DpBg_Enu_R);
	Stack_Enu_R->Add(DIS_Enu_R);
	Stack_Enu_R->Add(DpDhDelta_Enu_R);
	Stack_Enu_R->Draw("hist");
	Stack_Enu_R->GetXaxis()->SetTitle("Neutrino Energy (E_{#nu})");
	Stack_Enu_R->GetYaxis()->SetTitle("Ratio of #sigma at this Energy");
	//legend
	TLegend *EnuRLegend = new TLegend(0.7,0.5,0.9,0.9);
	EnuRLegend->AddEntry(DpDhQES_Enu_R,"2p2hQES","f");
	EnuRLegend->AddEntry(NSBrRes_Enu_R,"NSBrRes","f");
	EnuRLegend->AddEntry(QES_Enu_R,"QES","f");
	EnuRLegend->AddEntry(PiPBg_Enu_R,"PiPBg","f");
	EnuRLegend->AddEntry(PiNBg_Enu_R,"PiNBg","f");
	EnuRLegend->AddEntry(DpBg_Enu_R,"2pBg","f");
	EnuRLegend->AddEntry(DIS_Enu_R,"DIS","f");
	EnuRLegend->AddEntry(DpDhDelta_Enu_R,"2p2hDelta","f");
	EnuRLegend->Draw();

	//Q^2
	TH1D* QES_Q_R = (TH1D*) QES_Q->Clone();
	QES_Q_R->Divide(Tot_Q);
	TH1D* NSBrRes_Q_R = (TH1D*) NSBrRes_Q->Clone();
	NSBrRes_Q_R->Divide(Tot_Q);
	TH1D* PiNBg_Q_R = (TH1D*) PiNBg_Q->Clone();
	PiNBg_Q_R->Divide(Tot_Q);
	TH1D* PiPBg_Q_R = (TH1D*) PiPBg_Q->Clone();
	PiPBg_Q_R->Divide(Tot_Q);
	TH1D* DIS_Q_R = (TH1D*) DIS_Q->Clone();
	DIS_Q_R->Divide(Tot_Q);
	TH1D* DpDhQES_Q_R = (TH1D*) DpDhQES_Q->Clone();
	DpDhQES_Q_R->Divide(Tot_Q);
	TH1D* DpDhDelta_Q_R = (TH1D*) DpDhDelta_Q->Clone();
	DpDhDelta_Q_R->Divide(Tot_Q);
	TH1D* DpBg_Q_R = (TH1D*) DpBg_Q->Clone();
	DpBg_Q_R->Divide(Tot_Q);
	//Q^2 styling
	QES_Q_R->SetFillColor(kYellow);
	NSBrRes_Q_R->SetFillColor(kPink);
	PiNBg_Q_R->SetFillColor(kGreen);
	PiPBg_Q_R->SetFillColor(kRed);
	DIS_Q_R->SetFillColor(kBlue);
	DpDhQES_Q_R->SetFillColor(kMagenta);
	DpDhDelta_Q_R->SetFillColor(kOrange);
	DpBg_Q_R->SetFillColor(kTeal);
	//Q^2 stack
	TCanvas *Canvas_Q_R = new TCanvas("Canvas_Q2_R");
	THStack *Stack_Q_R = new THStack("Stack_Q2_R","Ratio of channel-specific 4-mom normalized to the total");
	Stack_Q_R->Add(DpDhQES_Q_R);
	Stack_Q_R->Add(NSBrRes_Q_R);
	Stack_Q_R->Add(QES_Q_R);
	Stack_Q_R->Add(PiPBg_Q_R);
	Stack_Q_R->Add(PiNBg_Q_R);
	Stack_Q_R->Add(DpBg_Q_R);
	Stack_Q_R->Add(DIS_Q_R);
	Stack_Q_R->Add(DpDhDelta_Q_R);
	Stack_Q_R->Draw("hist");
	Stack_Q_R->GetXaxis()->SetTitle("4-momentum Transfer (Q^{2})");
	Stack_Q_R->GetYaxis()->SetTitle("Ratio of #sigma at this Q^{2}");
	//legend
	TLegend *QRLegend = new TLegend(0.7,0.5,0.9,0.9);
	QRLegend->AddEntry(DpDhQES_Q_R,"2p2hQES","f");
	QRLegend->AddEntry(NSBrRes_Q_R,"NSBrRes","f");
	QRLegend->AddEntry(QES_Q_R,"QES","f");
	QRLegend->AddEntry(PiPBg_Q_R,"PiPBg","f");
	QRLegend->AddEntry(PiNBg_Q_R,"PiNBg","f");
	QRLegend->AddEntry(DpBg_Q_R,"2pBg","f");
	QRLegend->AddEntry(DIS_Q_R,"DIS","f");
	QRLegend->AddEntry(DpDhDelta_Q_R,"2p2hDelta","f");
	QRLegend->Draw();

	//w
	TH1D* QES_W_R = (TH1D*) QES_w->Clone();
	QES_W_R->Divide(Tot_w);
	TH1D* NSBrRes_W_R = (TH1D*) NSBrRes_w->Clone();
	NSBrRes_W_R->Divide(Tot_w);
	TH1D* PiNBg_W_R = (TH1D*) PiNBg_w->Clone();
	PiNBg_W_R->Divide(Tot_w);
	TH1D* PiPBg_W_R = (TH1D*) PiPBg_w->Clone();
	PiPBg_W_R->Divide(Tot_w);
	TH1D* DIS_W_R = (TH1D*) DIS_w->Clone();
	DIS_W_R->Divide(Tot_w);
	TH1D* DpDhQES_W_R = (TH1D*) DpDhQES_w->Clone();
	DpDhQES_W_R->Divide(Tot_w);
	TH1D* DpDhDelta_W_R = (TH1D*) DpDhDelta_w->Clone();
	DpDhDelta_W_R->Divide(Tot_w);
	TH1D* DpBg_W_R = (TH1D*) DpBg_w->Clone();
	DpBg_W_R->Divide(Tot_w);
	//w styling
	QES_W_R->SetFillColor(kYellow);
	NSBrRes_W_R->SetFillColor(kPink);
	PiNBg_W_R->SetFillColor(kGreen);
	PiPBg_W_R->SetFillColor(kRed);
	DIS_W_R->SetFillColor(kBlue);
	DpDhQES_W_R->SetFillColor(kMagenta);
	DpDhDelta_W_R->SetFillColor(kOrange);
	DpBg_W_R->SetFillColor(kTeal);
	//w stack
	TCanvas *Canvas_W_R = new TCanvas("Canvas_w_R");
	THStack *Stack_W_R = new THStack("Stack_w_R","Ratio of channel-specific energy transfer normalized to the total");
	Stack_W_R->Add(DpDhQES_W_R);
	Stack_W_R->Add(NSBrRes_W_R);
	Stack_W_R->Add(QES_W_R);
	Stack_W_R->Add(PiPBg_W_R);
	Stack_W_R->Add(PiNBg_W_R);
	Stack_W_R->Add(DpBg_W_R);
	Stack_W_R->Add(DIS_W_R);
	Stack_W_R->Add(DpDhDelta_W_R);
	Stack_W_R->Draw("hist");
	Stack_W_R->GetXaxis()->SetTitle("Energy Transfer (#omega)");
	Stack_W_R->GetYaxis()->SetTitle("Ratio of #sigma at this #omega");
	//legend
	TLegend *WRLegend = new TLegend(0.7,0.5,0.9,0.9);
	WRLegend->AddEntry(DpDhQES_W_R,"2p2hQES","f");
	WRLegend->AddEntry(NSBrRes_W_R,"NSBrRes","f");
	WRLegend->AddEntry(QES_W_R,"QES","f");
	WRLegend->AddEntry(PiPBg_W_R,"PiPBg","f");
	WRLegend->AddEntry(PiNBg_W_R,"PiNBg","f");
	WRLegend->AddEntry(DpBg_W_R,"2pBg","f");
	WRLegend->AddEntry(DIS_W_R,"DIS","f");
	WRLegend->AddEntry(DpDhDelta_W_R,"2p2hDelta","f");
	WRLegend->Draw();

	//w2
	TH1D* QES_W2_R = (TH1D*) QES_W2->Clone();
	QES_W2_R->Divide(Tot_W2);
	TH1D* NSBrRes_W2_R = (TH1D*) NSBrRes_W2->Clone();
	NSBrRes_W2_R->Divide(Tot_W2);
	TH1D* PiNBg_W2_R = (TH1D*) PiNBg_W2->Clone();
	PiNBg_W2_R->Divide(Tot_W2);
	TH1D* PiPBg_W2_R = (TH1D*) PiPBg_W2->Clone();
	PiPBg_W2_R->Divide(Tot_W2);
	TH1D* DIS_W2_R = (TH1D*) DIS_W2->Clone();
	DIS_W2_R->Divide(Tot_W2);
	TH1D* DpDhQES_W2_R = (TH1D*) DpDhQES_W2->Clone();
	DpDhQES_W2_R->Divide(Tot_W2);
	TH1D* DpDhDelta_W2_R = (TH1D*) DpDhDelta_W2->Clone();
	DpDhDelta_W2_R->Divide(Tot_W2);
	TH1D* DpBg_W2_R = (TH1D*) DpBg_W2->Clone();
	DpBg_W2_R->Divide(Tot_W2);
	//w2 styling
	QES_W2_R->SetFillColor(kYellow);
	NSBrRes_W2_R->SetFillColor(kPink);
	PiNBg_W2_R->SetFillColor(kGreen);
	PiPBg_W2_R->SetFillColor(kRed);
	DIS_W2_R->SetFillColor(kBlue);
	DpDhQES_W2_R->SetFillColor(kMagenta);
	DpDhDelta_W2_R->SetFillColor(kOrange);
	DpBg_W2_R->SetFillColor(kTeal);
	//W2 stack
	TCanvas *Canvas_W2_R = new TCanvas("Canvas_W2_R");
	THStack *Stack_W2_R = new THStack("Stack_W2_R","Ratio of channel-specific hadronic invarient mass normalized to the total");
	Stack_W2_R->Add(DpDhQES_W2_R);
	Stack_W2_R->Add(NSBrRes_W2_R);
	Stack_W2_R->Add(QES_W2_R);
	Stack_W2_R->Add(PiPBg_W2_R);
	Stack_W2_R->Add(PiNBg_W2_R);
	Stack_W2_R->Add(DpBg_W2_R);
	Stack_W2_R->Add(DIS_W2_R);
	Stack_W2_R->Add(DpDhDelta_W2_R);
	Stack_W2_R->Draw("hist");
	Stack_W2_R->GetXaxis()->SetTitle("Hadronic invarient mass (#W^{2})");
	Stack_W2_R->GetYaxis()->SetTitle("Ratio of #sigma at this #W^{2}");
	//legend
	TLegend *W2RLegend = new TLegend(0.7,0.5,0.9,0.9);
	W2RLegend->AddEntry(DpDhQES_W2_R,"2p2hQES","f");
	W2RLegend->AddEntry(NSBrRes_W2_R,"NSBrRes","f");
	W2RLegend->AddEntry(QES_W2_R,"QES","f");
	W2RLegend->AddEntry(PiPBg_W2_R,"PiPBg","f");
	W2RLegend->AddEntry(PiNBg_W2_R,"PiNBg","f");
	W2RLegend->AddEntry(DpBg_W2_R,"2pBg","f");
	W2RLegend->AddEntry(DIS_W2_R,"DIS","f");
	W2RLegend->AddEntry(DpDhDelta_W2_R,"2p2hDelta","f");
	W2RLegend->Draw();

	//X
	TH1D* QES_X_R = (TH1D*) QES_x->Clone();
	QES_X_R->Divide(Tot_x);
	TH1D* NSBrRes_X_R = (TH1D*) NSBrRes_x->Clone();
	NSBrRes_X_R->Divide(Tot_x);
	TH1D* PiNBg_X_R = (TH1D*) PiNBg_x->Clone();
	PiNBg_X_R->Divide(Tot_x);
	TH1D* PiPBg_X_R = (TH1D*) PiPBg_x->Clone();
	PiPBg_X_R->Divide(Tot_x);
	TH1D* DIS_X_R = (TH1D*) DIS_x->Clone();
	DIS_X_R->Divide(Tot_x);
	TH1D* DpDhQES_X_R = (TH1D*) DpDhQES_x->Clone();
	DpDhQES_X_R->Divide(Tot_x);
	TH1D* DpDhDelta_X_R = (TH1D*) DpDhDelta_x->Clone();
	DpDhDelta_X_R->Divide(Tot_x);
	TH1D* DpBg_X_R = (TH1D*) DpBg_x->Clone();
	DpBg_X_R->Divide(Tot_x);
	//X styling
	QES_X_R->SetFillColor(kYellow);
	NSBrRes_X_R->SetFillColor(kPink);
	PiNBg_X_R->SetFillColor(kGreen);
	PiPBg_X_R->SetFillColor(kRed);
	DIS_X_R->SetFillColor(kBlue);
	DpDhQES_X_R->SetFillColor(kMagenta);
	DpDhDelta_X_R->SetFillColor(kOrange);
	DpBg_X_R->SetFillColor(kTeal);
	//X stack
	TCanvas *Canvas_X_R = new TCanvas("Canvas_XB_R");
	THStack *Stack_X_R = new THStack("Stack_XB_R","Ratio of channel-specific X Bjorken scaling normalized to the total");
	Stack_X_R->Add(DpDhQES_X_R);
	Stack_X_R->Add(NSBrRes_X_R);
	Stack_X_R->Add(QES_X_R);
	Stack_X_R->Add(PiPBg_X_R);
	Stack_X_R->Add(PiNBg_X_R);
	Stack_X_R->Add(DpBg_X_R);
	Stack_X_R->Add(DIS_X_R);
	Stack_X_R->Add(DpDhDelta_X_R);
	Stack_X_R->Draw("hist");
	Stack_X_R->GetXaxis()->SetTitle("X Bjorken scaling (X)");
	Stack_X_R->GetYaxis()->SetTitle("Ratio of #sigma at this X");
	//legend
	TLegend *XRLegend = new TLegend(0.7,0.5,0.9,0.9);
	XRLegend->AddEntry(DpDhQES_X_R,"2p2hQES","f");
	XRLegend->AddEntry(NSBrRes_X_R,"NSBrRes","f");
	XRLegend->AddEntry(QES_X_R,"QES","f");
	XRLegend->AddEntry(PiPBg_X_R,"PiPBg","f");
	XRLegend->AddEntry(PiNBg_X_R,"PiNBg","f");
	XRLegend->AddEntry(DpBg_X_R,"2pBg","f");
	XRLegend->AddEntry(DIS_X_R,"DIS","f");
	XRLegend->AddEntry(DpDhDelta_X_R,"2p2hDelta","f");
	XRLegend->Draw();

	//y
	TH1D* QES_y_R = (TH1D*) QES_Y->Clone();
	QES_y_R->Divide(Tot_y);
	TH1D* NSBrRes_y_R = (TH1D*) NSBrRes_Y->Clone();
	NSBrRes_y_R->Divide(Tot_y);
	TH1D* PiNBg_y_R = (TH1D*) PiNBg_Y->Clone();
	PiNBg_y_R->Divide(Tot_y);
	TH1D* PiPBg_y_R = (TH1D*) PiPBg_Y->Clone();
	PiPBg_y_R->Divide(Tot_y);
	TH1D* DIS_y_R = (TH1D*) DIS_Y->Clone();
	DIS_y_R->Divide(Tot_y);
	TH1D* DpDhQES_y_R = (TH1D*) DpDhQES_Y->Clone();
	DpDhQES_y_R->Divide(Tot_y);
	TH1D* DpDhDelta_y_R = (TH1D*) DpDhDelta_Y->Clone();
	DpDhDelta_y_R->Divide(Tot_y);
	TH1D* DpBg_y_R = (TH1D*) DpBg_Y->Clone();
	DpBg_y_R->Divide(Tot_y);
	//Y styling
	QES_y_R->SetFillColor(kYellow);
	NSBrRes_y_R->SetFillColor(kPink);
	PiNBg_y_R->SetFillColor(kGreen);
	PiPBg_y_R->SetFillColor(kRed);
	DIS_y_R->SetFillColor(kBlue);
	DpDhQES_y_R->SetFillColor(kMagenta);
	DpDhDelta_y_R->SetFillColor(kOrange);
	DpBg_y_R->SetFillColor(kTeal);
	//Y stack
	TCanvas *Canvas_y_R = new TCanvas("Canvas_y_R");
	THStack *Stack_y_R = new THStack("Stack_y_R","Ratio of channel-specific relative energy transfer normalized to the total");
	Stack_y_R->Add(DpDhQES_y_R);
	Stack_y_R->Add(NSBrRes_y_R);
	Stack_y_R->Add(QES_y_R);
	Stack_y_R->Add(PiPBg_y_R);
	Stack_y_R->Add(PiNBg_y_R);
	Stack_y_R->Add(DpBg_y_R);
	Stack_y_R->Add(DIS_y_R);
	Stack_y_R->Add(DpDhDelta_y_R);
	Stack_y_R->Draw("hist");
	Stack_y_R->GetXaxis()->SetTitle("Relative energy transfer (#gamma)");
	Stack_y_R->GetYaxis()->SetTitle("Ratio of #sigma at this #gamma");
	//legend
	TLegend *yRLegend = new TLegend(0.7,0.5,0.9,0.9);
	yRLegend->AddEntry(DpDhQES_y_R,"2p2hQES","f");
	yRLegend->AddEntry(NSBrRes_y_R,"NSBrRes","f");
	yRLegend->AddEntry(QES_y_R,"QES","f");
	yRLegend->AddEntry(PiPBg_y_R,"PiPBg","f");
	yRLegend->AddEntry(PiNBg_y_R,"PiNBg","f");
	yRLegend->AddEntry(DpBg_y_R,"2pBg","f");
	yRLegend->AddEntry(DIS_y_R,"DIS","f");
	yRLegend->AddEntry(DpDhDelta_y_R,"2p2hDelta","f");
	yRLegend->Draw();

	TFile blobR("Ratios.root","RECREATE");
	Stack_Enu_R->Write();
	blobR.Close();
	Canvas_Enu_R->SaveAs("histRatioResultsE.png");

}







