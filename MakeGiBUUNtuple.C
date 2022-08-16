
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int gibuu_to_pdg(int id, int charge);

//It requires CERN ROOT enviromental variables 
//Compile using: g++ -o MakeGiBUUNtuple MakeGiBUUNtuple.C `root-config --cflags --glibs`

void MakeGiBUUNtuple(const char* cfIn, const char* cnfOut, const char* cSig){
 
  std::cout<< cfIn   <<std::endl;          //input file with a list of ".dat" GiBUU output files
  std::cout<< cnfOut <<std::endl;          //output file name
  std::cout<< cSig   <<std::endl;          // Sign of the weights (pos: positive or neg: negative)
  
  bool ispos = (std::string(cSig)=="pos");
  bool isneg = (std::string(cSig)=="neg");
  if(!ispos && !isneg) exit (1);
  ///////////////////////
  //Declaring output root tree 
  TFile* rootOut = new TFile(cnfOut, "RECREATE");  
  TTree* tr   = new TTree("records", "records");
  float Enu, weight;
  tr->Branch("Enu"   , &Enu);
  tr->Branch("weight", &weight);
  int prod_id;
  tr->Branch("prod_id", &prod_id);
  
  int nparts = 0;
  int pdg[1000];
  float E[1000];
  float px[1000];
  float py[1000];
  float pz[1000];
  tr->Branch("nparts", &nparts);
  tr->Branch("pdg", &pdg, "pdg[nparts]/I");
  tr->Branch("E", &E, "E[nparts]/F");
  tr->Branch("px", &px, "px[nparts]/F");
  tr->Branch("py", &py, "py[nparts]/F");
  tr->Branch("pz", &pz, "pz[nparts]/F");

  std::vector<std::string> fnames;
  std::string line;
  std::ifstream input_file(cfIn);
  while (getline(input_file, line)){
    if(line.find(".dat")>1000000)continue;    
    fnames.push_back(line);
  }
  std::cout<<"=> "<<fnames.size()<<" data files"<<std::endl;
  for(unsigned int i = 0; i < fnames.size(); ++i){
    const std::string fname = fnames[i];
    std::cout << "Adding " << fname << std::endl; 

    FILE* f = fopen(fname.c_str(), "r");
    int prevEvt = -1;
    //
    char header[4096];
    fgets(header, 4096, f);  
    int run, event, id, charge;
    float perweight, pos1, pos2, pos3, p0, p1, p2, p3;
    int history, prod_id_in;
    float Enu_in;
  
   // Loop over events in FinalEvents.dat
    while(true){
      int ret = fscanf(f, "%d %d %d %d %f %f %f %f %f %f %f %f %d %d %f",
                       &run, &event, &id, &charge,
                       &perweight, &pos1, &pos2, &pos3, &p0, &p1, &p2, &p3,
                       &history, &prod_id_in,
                       &Enu_in);

      if(ret != 15){
        fclose(f);
        if(nparts > 0) tr->Fill(); 
        nparts = 0; 
        break;
      }

      if(prevEvt == -1) prevEvt = event;

      if(event != prevEvt){
        prevEvt = event;
        if(nparts > 0)tr->Fill();
        nparts = 0;
      }
      
      Enu = Enu_in;
      prod_id = prod_id_in;
      
      float pwgt = abs(perweight);
      
      if( (perweight>0 && ispos) || (perweight<0 && isneg)   ){
        pdg[nparts] = gibuu_to_pdg(id, charge);
        E[nparts]  = p0;
        px[nparts] = p1;
        py[nparts] = p2;
        pz[nparts] = p3;
        ++nparts;
        weight = pwgt/fnames.size();
      }  
    } // end while
  } //end file loop
  
  //Storing the tree:
  rootOut->cd();
  tr->Write();
  rootOut->Close();
}

int gibuu_to_pdg(int id, int charge){
  // See https://gibuu.hepforge.org/trac/wiki/ParticleIDs

  if(id ==   1 && charge == -1) return -2212; // pbar
  if(id ==   1 && charge ==  0) return  2112; // n
  if(id ==   1 && charge == +1) return +2212; // p

  if(id ==  32 && charge ==  0) return  3122; // Lambda0

  if(id ==  33 && charge == -1) return  3112; // Sigma-
  if(id ==  33 && charge ==  0) return  3212; // Sigma0
  if(id ==  33 && charge == +1) return  3222; // Sigma+

  if(id ==  53 && charge == -1) return +3312; // Xi-
  if(id ==  53 && charge ==  0) return  3322; // Xi0
  if(id ==  53 && charge == +1) return -3312; // Xi+

  if(id ==  56 && charge == -1) return -4122; // Lambda_c-
  if(id ==  56 && charge == +1) return +4122; // Lambda_c+

  if(id ==  57 && charge == -2) return -4222; // Sigma_c--
  if(id ==  57 && charge == -1) return -4212; // Sigma_c-
  if(id ==  57 && charge ==  0) return  4112; // Sigma_c0
  if(id ==  57 && charge == +1) return  4212; // Sigma_c+
  if(id ==  57 && charge == +2) return  4222; // Sigma_c++

  if(id ==  59 && charge == -1) return -4232;// Xi_c-
  if(id ==  59 && charge ==  0) return  4132;// Xi_c0
  if(id ==  59 && charge == +1) return +4232;// Xi_c+

  if(id == 101 && charge == -1) return  -211; // pi-
  if(id == 101 && charge ==  0) return   111; // pi0
  if(id == 101 && charge == +1) return  +211; // pi+

  if(id == 110 && charge ==  0) return  +311; // K0
  if(id == 110 && charge == +1) return  +321; // K+

  if(id == 111 && charge == -1) return  -321; // K-
  if(id == 111 && charge ==  0) return  -311; // K0bar

  if(id == 114 && charge ==  0) return  +421; // D0
  if(id == 114 && charge == +1) return  +411; // D+

  if(id == 115 && charge == -1) return  -411; // D-
  if(id == 115 && charge ==  0) return  -421; // D0bar

  if(id == 118 && charge == +1) return  +431; // D_s+
  if(id == 119 && charge == -1) return  -431; // D_s-

  if(id == 901 && charge == -1) return   +11; // e-
  if(id == 901 && charge == +1) return   -11; // e+
  if(id == 902 && charge == -1) return   +13; // mu-
  if(id == 902 && charge == +1) return   -13; // mu+
  if(id == 903 && charge == -1) return   +15; // tau-
  if(id == 903 && charge == +1) return   -15; // tau+

  if(id ==  911 && charge == 0) return   +12; // nu_e
  if(id ==  912 && charge == 0) return   +14; // nu_mu
  if(id ==  913 && charge == 0) return   +16; // nu_tau
  if(id == -911 && charge == 0) return   -12; // nu_ebar
  if(id == -912 && charge == 0) return   -14; // nu_mubar
  if(id == -913 && charge == 0) return   -16; // nu_taubar


  std::cout << "Unknown particle: ID=" << id << ", charge=" << charge
            << ". Please implement." << std::endl;
  //  abort();
  return 0;
}

////////////////////////////////
#ifndef __CINT__
int main(int argc, const char* argv[]){  
  MakeGiBUUNtuple(argv[1],argv[2],argv[3]);
  return 0;
}
#endif






