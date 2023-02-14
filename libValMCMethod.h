//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 18 09:56:29 2022 by ROOT version 6.26/02
// from TTree records/records
// found on file: GiBUU2021_SBND_Library_pos.root
//////////////////////////////////////////////////////////

#ifndef libValMCMethod_h
#define libValMCMethod_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#define MAXNAMELEN 327000

// Header file for the classes stored in the TTree if any.

class libValMCMethod {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   
   //todo! pass string into .C file side 
   char 			fileNameOut[MAXNAMELEN];
   char				nameDir[MAXNAMELEN];
   //String 			fOut;
   //String			nameDir;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         Enu;
   Float_t         weight;
   Int_t           prod_id;
   Int_t           nparts;
   Int_t           pdg[35];   //[nparts]
   Float_t         E[35];   //[nparts]
   Float_t         px[35];   //[nparts]
   Float_t         py[35];   //[nparts]
   Float_t         pz[35];   //[nparts]

   // List of branches
   TBranch        *b_Enu;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_prod_id;   //!
   TBranch        *b_nparts;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_E;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!

   libValMCMethod(const char* fileIn, const char* dirExtension, const char* fileOut);
   virtual ~libValMCMethod();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef libValMCMethod_cxx
libValMCMethod::libValMCMethod(const char* fileIn, const char* dirExtension, const char* fileOut) 
{

	//input checking
	if(fileOut == NULL || dirExtension == NULL || fileIn == NULL)
	{
		std::cout<<"INPUT INVALID! EXITING"<<std::endl;
		exit(-1);
	}
	std::cout<<"File in : "<<fileIn<<std::endl;
	std::cout<<"File directory extension : "<<dirExtension<<std::endl;
	std::cout<<"File out : "<<fileOut<<std::endl;
	strcpy(fileNameOut,fileOut);
   	strcpy(nameDir,dirExtension);


	TFile *f=new TFile(fileIn,"READ");
	TTree *tree=(TTree*)f->Get(Form("%s/records",dirExtension));

	//depricated

	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
   	/*if (tree == 0) {
     	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("GiBUU2021_SBND_Library_pos.root");
    	if (!f || !f->IsOpen()) {
        	f = new TFile("GiBUU2021_SBND_Library_pos.root");
      	}
      	TDirectory * dir = (TDirectory*)f->Get("GiBUU2021_SBND_Library_pos.root:/Ar40/cc/nu_mu");
      	dir->GetObject("records",tree);

   	}*/
   
   
   	Init(tree);
}

libValMCMethod::~libValMCMethod()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t libValMCMethod::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t libValMCMethod::LoadTree(Long64_t entry)
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

void libValMCMethod::Init(TTree *tree)
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
   fChain->SetBranchAddress("nparts", &nparts, &b_nparts);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("E", E, &b_E);
   fChain->SetBranchAddress("px", px, &b_px);
   fChain->SetBranchAddress("py", py, &b_py);
   fChain->SetBranchAddress("pz", pz, &b_pz);
   Notify();
}

Bool_t libValMCMethod::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void libValMCMethod::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t libValMCMethod::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef libValMCMethod_cxx
