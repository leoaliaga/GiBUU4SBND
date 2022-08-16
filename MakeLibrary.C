#include "TFile.h"
#include "TGraph.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1F.h"
#include <iostream>
#include <string>
#include <fstream>

const int maxparts   = 1000;
const int Ntyp       = 6;
const double Emax    = 5.5;
const double dE      = .05;

const char* typ[Ntyp]   = {"numunc","numubarnc","nuelcc","numucc","nuelbarcc","numubarcc"};
const char* fdirs[Ntyp] = {"nu","nu_bar","nu_e","nu_mu","nu_e_bar","nu_mu_bar"};
const char* dirIO       = "/pnfs/sbnd/persistent/users/laliaga/GiBUU_Production";
const char* dirFx       = "/pnfs/sbnd/persistent/users/laliaga/GRID_INPUTS/GiBUU";

const double NnucleonsAr40 = 40.;

void FillEnu(TTree* mytree, TH1D* myhist);

//It requires CERN ROOT enviromental variables 
//Compile using: g++ -o MakeLibrary MakeLibrary.C `root-config --cflags --glibs`

void MakeLibrary(const char* cSig){
   std::cout<<"MakeLibrary() start "<<cSig<<std::endl; // Sign of the weights (pos: positive or neg: negative)
   
   if( std::string(cSig)!="pos" && std::string(cSig)!="neg" )exit (1);
   
  //Declaring neutrino energy bins for the quick test
  std::vector<double> edges;
  for(double x = 0; x < 2.999; x += dE)       edges.push_back(x);
  for(double x = 3; x < Emax-.0001; x += 0.5) edges.push_back(x);  
  edges.push_back(Emax);  
  int Npoints = int(edges.size());
  
  double enup[Npoints];
  enup[0] = 0;
  for(unsigned int i=0;i<(Npoints-1);i++) {
    double pval = 0.5*(edges[i]+edges[i+1]); 
    enup[i+1] = pval;
  }   
  
  //Getting flux
  //numu
  TTree* trFlux_numu = new TTree;
  trFlux_numu->ReadFile(Form("%s/SBND-FHC-numu.dat",dirFx), "Enu/D:flux_numu/D"); 
  TH1D* flux_numu = new TH1D("flux_numu","", edges.size()-1, &edges[0]); 
  trFlux_numu->Draw("Enu>>flux_numu", "flux_numu", "goff norm");
  flux_numu->Scale(1./flux_numu->Integral(0, -1));
  //nuel
  TTree* trFlux_nuel = new TTree;
  trFlux_nuel->ReadFile(Form("%s/SBND-FHC-nue.dat",dirFx), "Enu/D:flux_nuel/D"); 
  TH1D* flux_nuel = new TH1D("flux_nuel","", edges.size()-1, &edges[0]); 
  trFlux_nuel->Draw("Enu>>flux_nuel", "flux_nuel", "goff norm");
  flux_nuel->Scale(1./flux_nuel->Integral(0, -1));
    
  ///////////////////////////////////////////////////////////    
  TTree* tInn[Ntyp];    
  TH1D* hxs[Ntyp]; 
  TFile* fIn[Ntyp];
  
  for(int i=0;i<Ntyp;i++){
    std::cout<<"Getting: "<<typ[i]<<std::endl;  
    hxs[i] = new TH1D(Form("hxs%d",i),"", edges.size()-1, &edges[0]); 
    std::cout<<"fIn "<<Form("%s/Library/ntuples/SBND_Ar40_%s_%s.root",dirIO,typ[i],cSig)<<std::endl;
    fIn[i] = new TFile(Form("%s/Library/ntuples/SBND_Ar40_%s_%s.root",dirIO,typ[i],cSig),"read");
    //fIn[i]->ls();
    fIn[i]->GetObject("records",tInn[i]);
    //
    FillEnu(tInn[i],hxs[i]);
    hxs[i]->SetDirectory(0);
    //Converting from nucleon to Ar40: 
    hxs[i]->Scale(NnucleonsAr40);
    if(i==0 || i==1 || i==3 || i==5){hxs[i]->Divide(flux_numu);std::cout<<"Fillingnumu "<<i<<std::endl;}  
    if(i==2 || i==4 )               {hxs[i]->Divide(flux_nuel);std::cout<<"Fillingnuel "<<i<<std::endl;}
  }
  
  TFile* fOut = new TFile(Form("%s/Library/GiBUU2021_SBND_Library_%s.root",dirIO,cSig),"recreate"); 
  std::cout<<"fOut "<<    Form("%s/Library/GiBUU2021_SBND_Library_%s.root",dirIO,cSig)  <<std::endl;
  
  TTree* tOut[Ntyp]; 
  for(int i=0;i<Ntyp;i++){
    std::cout<<"Clonning: "<<typ[i]<<std::endl;  
    tOut[i] = tInn[i]->CloneTree(-1,"fast"); 
    tOut[i]->SetDirectory(0); 
    fIn[i]->Close(); 
  }
  
  TDirectory* dAr40 = fOut->mkdir("Ar40");
  TDirectory* dnc = dAr40->mkdir("nc");
  TDirectory* dcc = dAr40->mkdir("cc");
  TDirectory* dneu[Ntyp]; 
  //
  for(int i=0;i<Ntyp;i++){
    std::cout<<"Storing: "<<typ[i]<<std::endl;  
    if(i<2)dneu[i] = dnc->mkdir(fdirs[i]);
    else   dneu[i] = dcc->mkdir(fdirs[i]);
    dneu[i]->cd(); 
    tOut[i]->Write();
    //
    double xscont[Npoints];
    xscont[0] = 0;
    for(int j=1;j<=(hxs[i]->GetXaxis()->GetNbins());j++){
      xscont[j] = hxs[i]->GetBinContent(j);
    }  
    TGraph* gr = new TGraph(Npoints,enup,xscont);
    gr->Write("xsec");
  } 
 
 fOut->Close();   
 std::cout<<"MakeLibrary() end"<<std::endl; 
}
////////////////////////////////
void FillEnu(TTree* mytree, TH1D* myhist){
  float enuIn, wgtIn;
  mytree->SetBranchAddress("Enu"   , &enuIn);
  mytree->SetBranchAddress("weight", &wgtIn);
  int nentries = mytree->GetEntries();
  for(int k=0;k<nentries;k++){
    mytree->GetEntry(k);
    myhist->Fill(enuIn,wgtIn);
  } 
}
////////////////////////////////
#ifndef __CINT__
int main(int argc, const char* argv[]){  
  MakeLibrary(argv[1]);
  return 0;
}
#endif






