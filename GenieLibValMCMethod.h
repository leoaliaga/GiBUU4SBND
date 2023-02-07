//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Dec  1 20:14:42 2022 by ROOT version 6.26/02
// from TTree events/GiBUU events in SBND
// found on file: events_gibuu_sbnd.root
//////////////////////////////////////////////////////////

#ifndef GenieLibValMCMethod_h
#define GenieLibValMCMethod_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class GenieLibValMCMethod {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         Enu;
   Float_t         weight;
   Int_t           prod_id;
   Float_t         Vx;
   Float_t         Vy;
   Float_t         Vz;
   Int_t           nparts;
   Int_t           pdg[34];   //[nparts]
   Float_t         E[34];   //[nparts]
   Float_t         px[34];   //[nparts]
   Float_t         py[34];   //[nparts]
   Float_t         pz[34];   //[nparts]
   Float_t         W;
   Float_t         x;
   Float_t         y;
   Float_t         Qsq;

   // List of branches
   TBranch        *b_Enu;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_prod_id;   //!
   TBranch        *b_Vx;   //!
   TBranch        *b_Vy;   //!
   TBranch        *b_Vz;   //!
   TBranch        *b_nparts;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_E;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_W;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_Qsq;   //!

   GenieLibValMCMethod(TTree *tree=0);
   virtual ~GenieLibValMCMethod();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef GenieLibValMCMethod_cxx
GenieLibValMCMethod::GenieLibValMCMethod(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("events_gibuu_sbnd.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("events_gibuu_sbnd.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("events_gibuu_sbnd.root:/generator");
      dir->GetObject("events",tree);

   }
   Init(tree);
}

GenieLibValMCMethod::~GenieLibValMCMethod()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t GenieLibValMCMethod::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t GenieLibValMCMethod::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void GenieLibValMCMethod::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Enu", &Enu, &b_Enu);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("prod_id", &prod_id, &b_prod_id);
   fChain->SetBranchAddress("Vx", &Vx, &b_Vx);
   fChain->SetBranchAddress("Vy", &Vy, &b_Vy);
   fChain->SetBranchAddress("Vz", &Vz, &b_Vz);
   fChain->SetBranchAddress("nparts", &nparts, &b_nparts);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("E", E, &b_E);
   fChain->SetBranchAddress("px", px, &b_px);
   fChain->SetBranchAddress("py", py, &b_py);
   fChain->SetBranchAddress("pz", pz, &b_pz);
   fChain->SetBranchAddress("W", &W, &b_W);
   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("Qsq", &Qsq, &b_Qsq);
   Notify();
}

Bool_t GenieLibValMCMethod::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void GenieLibValMCMethod::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t GenieLibValMCMethod::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef GenieLibValMCMethod_cxx
