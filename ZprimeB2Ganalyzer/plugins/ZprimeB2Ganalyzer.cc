// -*- C++ -*-
//
// Package:    Analysis/ZprimeB2Ganalyzer
// Class:      ZprimeB2Ganalyzer
// 
/**\class ZprimeB2Ganalyzer ZprimeB2Ganalyzer.cc Analysis/ZprimeB2Ganalyzer/plugins/ZprimeB2Ganalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  christine mclean
//         Created:  Wed, 15 Apr 2015 15:12:05 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//#include "DataFormats/JetReco/interface/CATopJetTagInfo.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include <TTree.h>
#include <TLorentzVector.h>
using namespace std;

//
// class declaration
//

class ZprimeB2Ganalyzer : public edm::EDAnalyzer {
   public:
      explicit ZprimeB2Ganalyzer(const edm::ParameterSet&);
      ~ZprimeB2Ganalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  //edm::EDGetTokenT<edm::View<vector<float>> > jetAK8PtSrc_;
  //edm::EDGetTokenT<edm::View<vector<float>> > jetAK8GenPtSrc_;
  

  //TTree *tree_lept;
  TTree *tree_semilept;
  TTree *tree_had;

  std::map<std::string, float> treeVars;
  std::vector<std::string> listOfVars;

  //options
  bool negativeWeights_;
  bool isMC_;
  bool isFlat_;
  bool applyFilters_;
  bool applyTriggers_;
  int JERshift_;

  //std::string testSrc_;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ZprimeB2Ganalyzer::ZprimeB2Ganalyzer(const edm::ParameterSet& iConfig):
  //testSrc_ (iConfig.getParameter<std::string> ("testSrc") )
  //jetAK8PtSrc_(consumes<edm::View<vector<float>> >(iConfig.getParameter<edm::InputTag>("jetAK8PtSrc"))),
  //jetAK8GenPtSrc_(consumes<edm::View<vector<float>> >(iConfig.getParameter<edm::InputTag>("jetAK8GenPtSrc")))

  negativeWeights_   (iConfig.getParameter<bool>("negativeWeights")),
  isMC_              (iConfig.getParameter<bool>("isMC")),
  isFlat_            (iConfig.getParameter<bool>("isFlat")),
  applyFilters_      (iConfig.getParameter<bool>("applyFilters")),
  applyTriggers_     (iConfig.getParameter<bool>("applyTriggers")),
  JERshift_          (iConfig.getParameter<bool>("JERshift"))
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  //tree_lept = fs->make<TTree>("tree_lept","tree_lept");//leptonic tree
  tree_semilept = fs->make<TTree>("tree_semilept","tree_semilept");//semi-leptonic tree
  tree_had = fs->make<TTree>("tree_had","tree_had");//all hadronic tree

  listOfVars.push_back("cscFilt");
  listOfVars.push_back("vertexFilt");
  listOfVars.push_back("hbheFilt");
  listOfVars.push_back("htTrig");
  listOfVars.push_back("jetTrig");
  listOfVars.push_back("trimjetTrig");

  listOfVars.push_back("npv");
  listOfVars.push_back("evWeight_corr");
  listOfVars.push_back("nAK8pt400eta2p4jets");
  //listOfVars.push_back("nTopTagJets");
  listOfVars.push_back("cutflow");
  listOfVars.push_back("topTagJet1_topTagFlag");
  listOfVars.push_back("topTagJet2_topTagFlag");

  listOfVars.push_back("topTagJet1_pt");
  listOfVars.push_back("topTagJet1_eta");
  listOfVars.push_back("topTagJet1_phi");
  listOfVars.push_back("topTagJet1_E");
  listOfVars.push_back("topTagJet1_Y");
  listOfVars.push_back("topTagJet1_minmass");
  listOfVars.push_back("topTagJet1_nSubJets");
  listOfVars.push_back("topTagJet1_tau1");
  listOfVars.push_back("topTagJet1_tau2");
  listOfVars.push_back("topTagJet1_tau3");
  listOfVars.push_back("topTagJet1_nSubjettiness");
  listOfVars.push_back("topTagJet1_ungroomedMass");
  listOfVars.push_back("topTagJet1_topMass");
  listOfVars.push_back("topTagJet1_filteredMass");
  listOfVars.push_back("topTagJet1_prunedMass");
  listOfVars.push_back("topTagJet1_trimmedMass");
  listOfVars.push_back("topTagJet1_softDropMass");

  listOfVars.push_back("topTagJet2_pt");
  listOfVars.push_back("topTagJet2_eta");
  listOfVars.push_back("topTagJet2_phi");
  listOfVars.push_back("topTagJet2_E");
  listOfVars.push_back("topTagJet2_Y");
  listOfVars.push_back("topTagJet2_minmass");
  listOfVars.push_back("topTagJet2_nSubJets");
  listOfVars.push_back("topTagJet2_tau1");
  listOfVars.push_back("topTagJet2_tau2");
  listOfVars.push_back("topTagJet2_tau3");
  listOfVars.push_back("topTagJet2_nSubjettiness");
  listOfVars.push_back("topTagJet2_ungroomedMass");
  listOfVars.push_back("topTagJet2_topMass");
  listOfVars.push_back("topTagJet2_filteredMass");
  listOfVars.push_back("topTagJet2_prunedMass");
  listOfVars.push_back("topTagJet2_trimmedMass");
  listOfVars.push_back("topTagJet2_softDropMass");

  listOfVars.push_back("topTagJet1_maxSoftDropSubjetbtag");
  listOfVars.push_back("topTagJet1_maxCMSTTSubjetbtag");
  listOfVars.push_back("topTagJet2_maxSoftDropSubjetbtag");
  listOfVars.push_back("topTagJet2_maxCMSTTSubjetbtag");
 
  //listOfVars.push_back("jet1AK8GenPt");
  //listOfVars.push_back("jet2AK8GenPt");

  //listOfVars.push_back("cmstt_Z_mass");
  //listOfVars.push_back("filtered_Z_mass");
  //listOfVars.push_back("pruned_Z_mass");
  //listOfVars.push_back("trimmed_Z_mass");
  listOfVars.push_back("softDrop_Z_mass");
  listOfVars.push_back("ungroomed_Z_mass");

  listOfVars.push_back("leptSelectionPass");
  listOfVars.push_back("semileptSelectionPass");
  listOfVars.push_back("hadSelectionPass");
  
  for (unsigned i = 0; i < listOfVars.size(); i++){
    treeVars[ listOfVars[i] ] = -999.99;
    //tree_lept->Branch( (listOfVars[i]).c_str() , &(treeVars[ listOfVars[i] ]), (listOfVars[i]+"/F").c_str() );
    tree_semilept->Branch( (listOfVars[i]).c_str() , &(treeVars[ listOfVars[i] ]), (listOfVars[i]+"/F").c_str() );
    tree_had->Branch( (listOfVars[i]).c_str() , &(treeVars[ listOfVars[i] ]), (listOfVars[i]+"/F").c_str() );
  }
}


ZprimeB2Ganalyzer::~ZprimeB2Ganalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ZprimeB2Ganalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   cout <<"Hello!"<<endl;
   //cout <<"Event number: "<<iEvent<<endl;

   //cout<<testSrc_<<endl;

   //***HANDLES***

   //trigger and filter handles
   Handle<vector<string> > triggerNameStringsHandleF;
   Handle<vector<float> > triggerBitHandleF;
   Handle<vector<int> > triggerPrescaleHandleF;

   Handle<bool> HBHEfilterHandleF;

   Handle<vector<string> > triggerNameStringsHandle;
   Handle<vector<float> > triggerBitHandle;
   Handle<vector<int> > triggerPrescaleHandle;

   //event info
   Handle<int> h_npv;//labeled
   Handle<int> h_puNtrueInt;
      
   Handle<vector<float> > h_pv_chi;
   Handle<vector<float> > h_pv_rho;
   Handle<vector<float> > h_pv_z;
   Handle<vector<int> > h_pv_ndof;

   

   //generator info
   Handle<GenEventInfoProduct> h_genEventInfo;//labeled
   //Handle<LHERunInfoProduct> h_lheRunInfo; 

   //gen parton handles - labeled
   Handle<vector<float> > h_genPartPt;
   Handle<vector<float> > h_genPartEta;
   Handle<vector<float> > h_genPartPhi;
   Handle<vector<float> > h_genPartMass;
   Handle<vector<float> > h_genPartID;
   Handle<vector<float> > h_genPartStatus;
   Handle<vector<float> > h_genPartMomID;

   //muon handles
   Handle<vector<float> > h_muPt;
   Handle<vector<float> > h_muEta;
   Handle<vector<float> > h_muPhi;
   Handle<vector<float> > h_muTight;
   Handle<vector<float> > h_muLoose;
   Handle<vector<float> > h_muMass;
   Handle<vector<float> > h_muDz;
   Handle<vector<float> > h_muCharge;
   Handle<vector<float> > h_muIso04;

   Handle<vector<vector<int> > > h_muKey;

   //electron handles
   Handle<vector<float> > h_elPt;
   Handle<vector<float> > h_elEta;
   Handle<vector<float> > h_elPhi;
   Handle<vector<float> > h_elTight;
   Handle<vector<float> > h_elLoose;
   Handle<vector<float> > h_eldEtaIn;
   Handle<vector<float> > h_eldPhiIn;
   Handle<vector<float> > h_elHoE;
   Handle<vector<float> > h_elfull5x5siee;
   Handle<vector<float> > h_elE;
   Handle<vector<float> > h_elD0;
   Handle<vector<float> > h_elDz;
   Handle<vector<float> > h_elIso03;
   Handle<vector<float> > h_elisVeto;
   Handle<vector<float> > h_elhasMatchedConVeto;
   Handle<vector<float> > h_elooEmooP;
   Handle<vector<float> > h_elMass;
   Handle<vector<float> > h_elscEta;
   Handle<vector<float> > h_elCharge;

   Handle<vector<vector<int> > > h_elKey;

   //Handle<vector<float> > ;

   //AK4 jet handles   
   Handle<vector<float> > h_jetsAK4E;
   Handle<vector<float> > h_jetsAK4Pt;
   Handle<vector<float> > h_jetsAK4Eta;
   Handle<vector<float> > h_jetsAK4Phi;
   Handle<vector<float> > h_jetsAK4ungroomedMass;
   Handle<vector<float> > h_jetsAK4JEC;
   Handle<vector<float> > h_jetsAK4CSV;
   Handle<vector<float> > h_jetsAK4NumDaughters;
   Handle<vector<float> > h_jetsAK4Area;

   Handle<vector<int> > h_jetsAK4Keys;

   Handle<vector<float> > h_jetsAK4nHadEnergy;
   Handle<vector<float> > h_jetsAK4nEMEnergy;
   Handle<vector<float> > h_jetsAK4HFHadronEnergy;
   Handle<vector<float> > h_jetsAK4cHadEnergy;
   Handle<vector<float> > h_jetsAK4cEMEnergy;
   Handle<vector<float> > h_jetsAK4numDaughters;
   Handle<vector<float> > h_jetsAK4cMultip;
   Handle<vector<float> > h_jetsAK4Y;

   //Rho handles
   Handle <double> h_rho;

   //MET handles
   Handle<vector<float> > h_metPt;
   Handle<vector<float> > h_metPx;
   Handle<vector<float> > h_metPy;
   Handle<vector<float> > h_metPhi;

   //AK8 jet handles - labeled
   Handle<vector<float> > h_jetsAK8GenJetE;
   Handle<vector<float> > h_jetsAK8GenJetPt;
   Handle<vector<float> > h_jetsAK8GenJetEta;
   Handle<vector<float> > h_jetsAK8GenJetPhi;
   Handle<vector<float> > h_jetsAK8GenJetY;//unlabeled

   Handle<vector<float> > h_jetsAK8E;
   Handle<vector<float> > h_jetsAK8Pt;
   Handle<vector<float> > h_jetsAK8Eta;
   Handle<vector<float> > h_jetsAK8Phi;
   Handle<vector<float> > h_jetsAK8Y;
   Handle<vector<float> > h_jetsAK8minmass;
   Handle<vector<float> > h_jetsAK8tau1;
   Handle<vector<float> > h_jetsAK8tau2;
   Handle<vector<float> > h_jetsAK8tau3;
   Handle<vector<float> > h_jetsAK8nSubJets;
   Handle<vector<float> > h_jetsAK8filteredMass;
   Handle<vector<float> > h_jetsAK8prunedMass;
   Handle<vector<float> > h_jetsAK8trimmedMass;
   Handle<vector<float> > h_jetsAK8softDropMass;
   Handle<vector<float> > h_jetsAK8topMass;
   Handle<vector<float> > h_jetsAK8ungroomedMass;

   Handle<vector<float> > h_ak8jetTopSubjetIndex0;
   Handle<vector<float> > h_ak8jetTopSubjetIndex1;
   Handle<vector<float> > h_ak8jetTopSubjetIndex2;
   Handle<vector<float> > h_ak8jetTopSubjetIndex3;
   Handle<vector<float> > h_ak8jetSoftDropSubjetIndex0;
   Handle<vector<float> > h_ak8jetSoftDropSubjetIndex1;
   Handle<vector<float> > h_subjetCmsTopTagCSV;
   Handle<vector<float> > h_subjetSoftDropCSV;

   /*Handle<vector<float> > h_subjetAK8GenE;
   Handle<vector<float> > h_subjetAK8GenPt;
   Handle<vector<float> > h_subjetAK8GenEta;
   Handle<vector<float> > h_subjetAK8GenPhi;

   Handle<vector<float> > h_subjetAK8E;
   Handle<vector<float> > h_subjetAK8Pt;
   Handle<vector<float> > h_subjetAK8Eta;
   Handle<vector<float> > h_subjetAK8Phi;*/


   //***LABELS***

   //trigger and filter labels
   iEvent.getByLabel("METUserData", "triggerNameTree", triggerNameStringsHandleF);
   iEvent.getByLabel("METUserData", "triggerBitTree", triggerBitHandleF);
   iEvent.getByLabel("METUserData", "triggerPrescaleTree", triggerPrescaleHandleF);

   iEvent.getByLabel("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult", HBHEfilterHandleF);

   iEvent.getByLabel("TriggerUserData", "triggerNameTree", triggerNameStringsHandle);
   iEvent.getByLabel("TriggerUserData", "triggerBitTree", triggerBitHandle);
   iEvent.getByLabel("TriggerUserData", "triggerPrescaleTree", triggerPrescaleHandle);

   //event info
   iEvent.getByLabel("eventUserData", "npv", h_npv );
 
   //generator info
   if (isMC_){
     iEvent.getByLabel("generator", "", h_genEventInfo);
     //iEvent.getByLabel("externalLHEProducer", "", h_lheRunInfo);
   }

   //iEvent.getByLabel();

   //gen parton labels
   iEvent.getByLabel("genPart" , "genPartPt", h_genPartPt);
   iEvent.getByLabel("genPart" , "genPartEta", h_genPartEta);
   iEvent.getByLabel("genPart" , "genPartPhi", h_genPartPhi);
   iEvent.getByLabel("genPart" , "genPartPhi", h_genPartPhi);
   iEvent.getByLabel("genPart" , "genPartMass", h_genPartMass);
   iEvent.getByLabel("genPart" , "genPartID", h_genPartID);
   iEvent.getByLabel("genPart" , "genPartStatus", h_genPartStatus);
   iEvent.getByLabel("genPart" , "genPartMom0ID", h_genPartMomID);

   iEvent.getByLabel("jetsAK8", "jetAK8GenJetE", h_jetsAK8GenJetE );
   iEvent.getByLabel("jetsAK8", "jetAK8GenJetPt", h_jetsAK8GenJetPt );
   iEvent.getByLabel("jetsAK8", "jetAK8GenJetEta", h_jetsAK8GenJetEta );
   iEvent.getByLabel("jetsAK8", "jetAK8GenJetPhi", h_jetsAK8GenJetPhi );

   iEvent.getByLabel("jetsAK8", "jetAK8E", h_jetsAK8E );
   iEvent.getByLabel("jetsAK8", "jetAK8Pt", h_jetsAK8Pt );
   iEvent.getByLabel("jetsAK8", "jetAK8Eta", h_jetsAK8Eta );
   iEvent.getByLabel("jetsAK8", "jetAK8Phi", h_jetsAK8Phi );
   iEvent.getByLabel("jetsAK8", "jetAK8Y", h_jetsAK8Y );
   iEvent.getByLabel("jetsAK8", "jetAK8minmass", h_jetsAK8minmass );
   iEvent.getByLabel("jetsAK8", "jetAK8tau1", h_jetsAK8tau1 );
   iEvent.getByLabel("jetsAK8", "jetAK8tau2", h_jetsAK8tau2 );
   iEvent.getByLabel("jetsAK8", "jetAK8tau3", h_jetsAK8tau3 );
   iEvent.getByLabel("jetsAK8", "jetAK8nSubJets", h_jetsAK8nSubJets );
   iEvent.getByLabel("jetsAK8", "jetAK8filteredMass", h_jetsAK8filteredMass );
   iEvent.getByLabel("jetsAK8", "jetAK8prunedMass", h_jetsAK8prunedMass );
   iEvent.getByLabel("jetsAK8", "jetAK8trimmedMass", h_jetsAK8trimmedMass );
   iEvent.getByLabel("jetsAK8", "jetAK8softDropMass", h_jetsAK8softDropMass );
   iEvent.getByLabel("jetsAK8", "jetAK8topMass", h_jetsAK8topMass );
   iEvent.getByLabel("jetsAK8", "jetAK8Mass", h_jetsAK8ungroomedMass );

   iEvent.getByLabel("jetsAK8", "jetAK8topSubjetIndex0", h_ak8jetTopSubjetIndex0);
   iEvent.getByLabel("jetsAK8", "jetAK8topSubjetIndex1", h_ak8jetTopSubjetIndex1);
   iEvent.getByLabel("jetsAK8", "jetAK8topSubjetIndex2", h_ak8jetTopSubjetIndex2);
   iEvent.getByLabel("jetsAK8", "jetAK8topSubjetIndex3", h_ak8jetTopSubjetIndex3);
   iEvent.getByLabel("jetsAK8", "jetAK8vSubjetIndex0", h_ak8jetSoftDropSubjetIndex0);
   iEvent.getByLabel("jetsAK8", "jetAK8topSubjetIndex1", h_ak8jetSoftDropSubjetIndex1);
   iEvent.getByLabel("subjetsCmsTopTag", "subjetCmsTopTagCSV", h_subjetCmsTopTagCSV);
   iEvent.getByLabel("subjetsAK8", "subjetAK8CSV", h_subjetSoftDropCSV);

   //cout<<"Handles set!"<<endl;

   /*iEvent.getByLabel("subjetsAK8", "subjetAK8GenSubjetE", h_subjetAK8GenE );
   iEvent.getByLabel("subjetsAK8", "subjetAK8GenSubjetPt", h_subjetAK8GenPt );
   iEvent.getByLabel("subjetsAK8", "subjetAK8GenSubjetEta", h_subjetAK8GenEta );
   iEvent.getByLabel("subjetsAK8", "subjetAK8GenSubjetPhi", h_subjetAK8GenPhi );

   iEvent.getByLabel("subjetsAK8", "subjetAK8E", h_subjetAK8E );
   iEvent.getByLabel("subjetsAK8", "subjetAK8Pt", h_subjetAK8Pt );
   iEvent.getByLabel("subjetsAK8", "subjetAK8Eta", h_subjetAK8Eta );
   iEvent.getByLabel("subjetsAK8", "subjetAK8Phi", h_subjetAK8Phi );*/
   
   //want to make sure the AK8 jet vectors match up
   if (h_jetsAK8Pt->size() != h_jetsAK8minmass->size() || h_jetsAK8Pt->size() != h_jetsAK8topMass->size()){
     cout<<"Error! Mismatched vectors."<<endl;
   }

   //cout<<"Initializing branches!"<<endl;

   //initializing branches to -999.99
   for (unsigned i = 0; i < listOfVars.size(); i++){
     treeVars[ listOfVars[i] ] = -999.99;
   }

   //event filters
   bool cscFilt = 0;
   bool vertexFilt = 0;
   bool hbheFilt = 0;
   
   hbheFilt = (*HBHEfilterHandleF);
   
   //cout<<"Setting filter bits!"<<endl;

   if (triggerNameStringsHandleF.isValid()){
     //cout<<"Valid Strings Handle!"<<endl;
     for (size_t i = 0; i < triggerNameStringsHandleF->size(); i++){
       if ( (*triggerNameStringsHandleF)[i].find("CSC") != string::npos) {
	 if ( (*triggerBitHandleF)[i] == 1 ) cscFilt = 1;
       }
       //Broken HBHE Filter
       //if ( (*triggerNameStringsHandleF)[i].find("HBHE") != string::npos) {
       //        if ( (*triggerBitHandleF)[i] == 1 ) hbheFilt = 1;
       //}
       if ( (*triggerNameStringsHandleF)[i].find("goodVer") != string::npos) {
	 if ( (*triggerBitHandleF)[i] == 1 ) vertexFilt = 1;
       }
     }
   }

   //cout<<"Filter bits set!"<<endl;

   treeVars["cscFilt"] = cscFilt;
   treeVars["vertexFilt"] = vertexFilt;
   treeVars["hbheFilt"] = hbheFilt;

   //cout<<"CSC Filter bit: "<<cscFilt<<endl;
   //cout<<"VERTEX Filter bit: "<<vertexFilt<<endl;
   //cout<<"HBHE Filter bit: "<<hbheFilt<<endl; 

   //trigger booleans
   bool htTrig = 0;
   bool jetTrig = 0;
   bool trimjetTrig = 0;

   //cout<<"Setting trigger bits!"<<endl;

   if (triggerNameStringsHandle.isValid()){
     for (size_t i = 0; i < triggerNameStringsHandle->size(); i++){
       if ( (*triggerNameStringsHandle)[i].find("PFHT800") != string::npos) {
	 if ( (*triggerBitHandle)[i] == 1 ) htTrig = 1;
       }
       if ( (*triggerNameStringsHandle)[i].find("PFJet400") != string::npos) {
	 if ( (*triggerBitHandle)[i] == 1 ) jetTrig = 1;
       }
       if ( (*triggerNameStringsHandle)[i].find("AK8PFJet360_TrimMass30") != string::npos) {
	 if ( (*triggerBitHandle)[i] == 1 ) trimjetTrig = 1;
       }
     }
   }

   //cout<<"Storing trigger bits!"<<endl;

   treeVars["htTrig"] = htTrig;
   treeVars["jetTrig"] = jetTrig;
   treeVars["trimjetTrig"] = trimjetTrig;

   //cout<<"Initializing event weights!"<<endl;

   //initializing and storing event weights
   double evWeight = 1.0 ;
   double qScale = 1.0 ;
   double pthat = 1.0 ;
   double evWeight_corr = 1.0;


   if (isMC_ && h_genEventInfo.isValid())
     {
       evWeight = h_genEventInfo->weight();
       qScale   = h_genEventInfo->qScale();
       pthat    = (h_genEventInfo->hasBinningValues() ? (h_genEventInfo->binningValues())[0] : 0.0);
       evWeight_corr = 1/pow(pthat/15,4.5);

       treeVars["evWeight_corr"] = evWeight_corr;
       //cout<<"GenEventInfo: qScale = "<<qScale<<" pthat = "<<pthat<<" evWeight = "<<evWeight<<" 1/pow(pthat/15,4.5) "<<1/pow(pthat/15,4.5)<<endl;
     }

   //reco AK8 jets
   int nAK8pt400eta2p4jets = 0;

   //top tagging
   //int nTopTagJets = 0;
   int topTagJet1_topTagFlag = 0;
   int topTagJet2_topTagFlag = 0;
   int nNsubjettinessCutJets = 0;
   int nBtagJets = 0;

   //saving jet 4 vectors to reconstruct Z' mass
   TLorentzVector softDropJet1;
   TLorentzVector softDropJet2;

   TLorentzVector ungroomedJet1;
   TLorentzVector ungroomedJet2;
   
   //int toptagindexJet1_cmstt = -1;
   //int toptagindexJet1_filtered = -1;
   //int toptagindexJet1_pruned = -1;
   //int toptagindexJet1_trimmed = -1;

   //int toptagindexJet2_cmstt = -1;
   //int toptagindexJet2_filtered = -1;
   //int toptagindexJet2_pruned = -1;
   //int toptagindexJet2_trimmed = -1;

   //TLorentzVector cmsttJet1;
   //TLorentzVector filteredJet1;
   //TLorentzVector prunedJet1;
   //TLorentzVector trimmedJet1;

   //TLorentzVector cmsttJet2;
   //TLorentzVector filteredJet2;
   //TLorentzVector prunedJet2;
   //TLorentzVector trimmedJet2;


   //Number of Primary Vertices
   treeVars["npv"] = *h_npv;

   //AK8 Jet Loop
   if (h_jetsAK8Pt.isValid()){//make sure there are AK8 jets
     for (unsigned i=0; i<h_jetsAK8Pt->size(); i++){//looping over AK8 jets
       //pt and eta preselection cuts
       if ((*h_jetsAK8Pt)[i] > 400 && abs((*h_jetsAK8Eta)[i]) < 2.4){
	 //treeVars["jet"+s+"AK8Pt"] = (*h_jetsAK8Pt)[i];
	 //top tagging requirements
	 if (nAK8pt400eta2p4jets == 0){//first top tag jet candidate
	   treeVars["topTagJet1_pt"] = (*h_jetsAK8Pt)[i];
	   treeVars["topTagJet1_eta"] = (*h_jetsAK8Eta)[i];
	   treeVars["topTagJet1_phi"] = (*h_jetsAK8Phi)[i];
	   treeVars["topTagJet1_E"] = (*h_jetsAK8E)[i];
	   treeVars["topTagJet1_Y"] = (*h_jetsAK8Y)[i];
	   treeVars["topTagJet1_minmass"] = (*h_jetsAK8minmass)[i];
	   treeVars["topTagJet1_nSubJets"] = (*h_jetsAK8nSubJets)[i];
	   treeVars["topTagJet1_tau1"] = (*h_jetsAK8tau1)[i];
	   treeVars["topTagJet1_tau2"] = (*h_jetsAK8tau2)[i];
	   treeVars["topTagJet1_tau3"] = (*h_jetsAK8tau3)[i];
	   treeVars["topTagJet1_ungroomedMass"] = (*h_jetsAK8ungroomedMass)[i];
	   treeVars["topTagJet1_topMass"] = (*h_jetsAK8topMass)[i];
	   treeVars["topTagJet1_filteredMass"] = (*h_jetsAK8filteredMass)[i];
	   treeVars["topTagJet1_prunedMass"] = (*h_jetsAK8prunedMass)[i];
	   treeVars["topTagJet1_trimmedMass"] = (*h_jetsAK8trimmedMass)[i];
	   treeVars["topTagJet1_softDropMass"] = (*h_jetsAK8softDropMass)[i];

	   //Lorentz vectors
	   softDropJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8softDropMass)[i]);
	   ungroomedJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8ungroomedMass)[i]);
	   
	   //////b-tagging
	   float nSubjets_jet1 = (*h_jetsAK8nSubJets)[i];

	   //vector of subjet CSV values
	   vector<float> subjet1cmsttCSVs;
	   vector<float> subjet1softDropCSVs;
	   if (nSubjets_jet1 > 0){
	     subjet1cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex0)[i]]);
	     subjet1softDropCSVs.push_back((*h_subjetSoftDropCSV)[(*h_ak8jetSoftDropSubjetIndex0)[i]]);
	   }
	   if (nSubjets_jet1 > 1){
	     subjet1cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex1)[i]]);
	     subjet1softDropCSVs.push_back((*h_subjetSoftDropCSV)[(*h_ak8jetSoftDropSubjetIndex1)[i]]);
	   }
	   if (nSubjets_jet1 > 2){
	     subjet1cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex2)[i]]);
	   }
	   if (nSubjets_jet1 > 3){
	     subjet1cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex3)[i]]);
	   }

	   //saving max cmstt subjet b-tag score for jet 1
	   float maxSubjet1cmsttCSV = -999.;
	   for (size_t j = 0; j < subjet1cmsttCSVs.size(); j++){
	     maxSubjet1cmsttCSV = max(maxSubjet1cmsttCSV, subjet1cmsttCSVs[j]);
	   }

	   //saving max soft drop subjet b-tag score for jet 1
	   float maxSubjet1softDropCSV = -999.;
	   for (size_t j = 0; j < subjet1softDropCSVs.size(); j++){
	     maxSubjet1softDropCSV = max(maxSubjet1softDropCSV, subjet1softDropCSVs[j]);
	   }

	   treeVars["topTagJet1_maxCMSTTSubjetbtag"] = maxSubjet1cmsttCSV;
	   treeVars["topTagJet1_maxSoftDropSubjetbtag"] = maxSubjet1softDropCSV;

	   //nsubjettiness
	   float nSubjettiness = ((*h_jetsAK8tau3)[i])/((*h_jetsAK8tau2)[i]);
	   treeVars["topTagJet1_nSubjettiness"] = nSubjettiness;
	   
	   //top tagging with softdrop mass
	   if ((*h_jetsAK8softDropMass)[i] > 110.0 && (*h_jetsAK8softDropMass)[i] < 210.0){
	     topTagJet1_topTagFlag = 1;
	   }
	   
	   treeVars["topTagJet1_topTagFlag"] = topTagJet1_topTagFlag;
	   
	   //incrementing counter for number of jets passing the Nsubjettiness cut
	   if (nSubjettiness < 0.61){
	     nNsubjettinessCutJets += 1;
	   }

	   //incrementing counter for number of jets passing the b-tag cut
	   if (maxSubjet1softDropCSV > 0.76){
	     nBtagJets += 1;
	   }

	   //flagging the jet as top tagged by the cmstt mass
	   /*if ((*h_jetsAK8topMass)[i] > 140.0 && (*h_jetsAK8topMass)[i] < 250.0){
	     topTagJet1_topTagFlag |= 1;
	     //toptagindexJet1_cmstt = i;
	     cmsttJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   } 

	   //flagging the jet as top tagged by the filtered mass
	   if ((*h_jetsAK8filteredMass)[i] > 140.0 && (*h_jetsAK8filteredMass)[i] < 250.0){
	     topTagJet1_topTagFlag |= 2;
	     //toptagindexJet1_filtered = i;
	     filteredJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   }

	   //flagging the jet as top tagged by the pruned mass
	   if ((*h_jetsAK8prunedMass)[i] > 140.0 && (*h_jetsAK8prunedMass)[i] < 250.0){
	     topTagJet1_topTagFlag |= 4;
	     //toptagindexJet1_pruned = i;
	     prunedJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   }
	   
	   //flagging the jet as top tagged by the trimmed mass
	   if ((*h_jetsAK8trimmedMass)[i] > 140.0 && (*h_jetsAK8trimmedMass)[i] < 250.0){
	     topTagJet1_topTagFlag |= 8;
	     //toptagindexJet1_trimmed = i;
	     trimmedJet1.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	     }*/
	 }//end AK8 jet 1 loop
	 else if (nAK8pt400eta2p4jets == 1){//second top tag jet candidate
	   treeVars["topTagJet2_pt"] = (*h_jetsAK8Pt)[i];
	   treeVars["topTagJet2_eta"] = (*h_jetsAK8Eta)[i];
	   treeVars["topTagJet2_phi"] = (*h_jetsAK8Phi)[i];
	   treeVars["topTagJet2_E"] = (*h_jetsAK8E)[i];
	   treeVars["topTagJet2_Y"] = (*h_jetsAK8Y)[i];
	   treeVars["topTagJet2_minmass"] = (*h_jetsAK8minmass)[i];
	   treeVars["topTagJet2_nSubJets"] = (*h_jetsAK8nSubJets)[i];
	   treeVars["topTagJet2_tau1"] = (*h_jetsAK8tau1)[i];
	   treeVars["topTagJet2_tau2"] = (*h_jetsAK8tau2)[i];
	   treeVars["topTagJet2_tau3"] = (*h_jetsAK8tau3)[i];
	   treeVars["topTagJet2_ungroomedMass"] = (*h_jetsAK8ungroomedMass)[i];
	   treeVars["topTagJet2_topMass"] = (*h_jetsAK8topMass)[i];
	   treeVars["topTagJet2_filteredMass"] = (*h_jetsAK8filteredMass)[i];
	   treeVars["topTagJet2_prunedMass"] = (*h_jetsAK8prunedMass)[i];
	   treeVars["topTagJet2_trimmedMass"] = (*h_jetsAK8trimmedMass)[i];
	   treeVars["topTagJet2_softDropMass"] = (*h_jetsAK8softDropMass)[i];

	   //Lorentz vectors
	   softDropJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8softDropMass)[i]);
	   ungroomedJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8ungroomedMass)[i]);
	   
	   //////b-tagging
	   float nSubjets_jet2 = (*h_jetsAK8nSubJets)[i];

	   //vector of subjet CSV values
	   vector<float> subjet2cmsttCSVs;
	   vector<float> subjet2softDropCSVs;
	   if (nSubjets_jet2 > 0){
	     subjet2cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex0)[i]]);
	     subjet2softDropCSVs.push_back((*h_subjetSoftDropCSV)[(*h_ak8jetSoftDropSubjetIndex0)[i]]);
	   }
	   if (nSubjets_jet2 > 1){
	     subjet2cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex1)[i]]);
	     subjet2softDropCSVs.push_back((*h_subjetSoftDropCSV)[(*h_ak8jetSoftDropSubjetIndex1)[i]]);
	   }
	   if (nSubjets_jet2 > 2){
	     subjet2cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex2)[i]]);
	   }
	   if (nSubjets_jet2 > 3){
	     subjet2cmsttCSVs.push_back((*h_subjetCmsTopTagCSV)[(*h_ak8jetTopSubjetIndex3)[i]]);
	   }

	   //saving max cmstt subjet b-tag score for jet 2
	   float maxSubjet2cmsttCSV = -999.;
	   for (size_t j = 0; j < subjet2cmsttCSVs.size(); j++){
	     maxSubjet2cmsttCSV = max(maxSubjet2cmsttCSV, subjet2cmsttCSVs[j]);
	   }

	   //saving max soft drop subjet b-tag score for jet 2
	   float maxSubjet2softDropCSV = -999.;
	   for (size_t j = 0; j < subjet2softDropCSVs.size(); j++){
	     maxSubjet2softDropCSV = max(maxSubjet2softDropCSV, subjet2softDropCSVs[j]);
	   }

	   treeVars["topTagJet2_maxCMSTTSubjetbtag"] = maxSubjet2cmsttCSV;
	   treeVars["topTagJet2_maxSoftDropSubjetbtag"] = maxSubjet2softDropCSV;

	   //nsubjettiness
	   float nSubjettiness = ((*h_jetsAK8tau3)[i])/((*h_jetsAK8tau2)[i]);
	   treeVars["topTagJet2_nSubjettiness"] = nSubjettiness;

	   //top tagging with softdrop mass
	   if ((*h_jetsAK8softDropMass)[i] > 110.0 && (*h_jetsAK8softDropMass)[i] < 210.0){
	     topTagJet2_topTagFlag = 1;
	   }
	   
	   treeVars["topTagJet2_topTagFlag"] = topTagJet2_topTagFlag;

	   //incrementing counter for number of jets passing the Nsubjettiness cut
	   if (nSubjettiness < 0.61){
	     nNsubjettinessCutJets += 1;
	   }

	   //incrementing counter for number of jets passing the b-tag cut
	   if (maxSubjet2softDropCSV > 0.76){
	     nBtagJets += 1;
	   }

	   //flagging the jet as top tagged by the cmstt top mass
	   /*if ((*h_jetsAK8topMass)[i] > 140.0 && (*h_jetsAK8topMass)[i] < 250.0){
	     topTagJet2_topTagFlag |= 1;
	     //toptagindexJet2_cmstt = i;
	     cmsttJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   } 
	   
	   //flagging the jet as top tagged by the filtered mass
	   if ((*h_jetsAK8filteredMass)[i] > 140.0 && (*h_jetsAK8filteredMass)[i] < 250.0){
	     topTagJet2_topTagFlag |= 2;
	     //toptagindexJet2_filtered = i;
	     filteredJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   }

	   //flagging the jet as top tagged by the pruned mass
	   if ((*h_jetsAK8prunedMass)[i] > 140.0 && (*h_jetsAK8prunedMass)[i] < 250.0){
	     topTagJet2_topTagFlag |= 4;
	     //toptagindexJet2_pruned = i;
	     prunedJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	   }
	   
	   //flagging the jet as top tagged by the trimmed mass
	   if ((*h_jetsAK8trimmedMass)[i] > 140.0 && (*h_jetsAK8trimmedMass)[i] < 250.0){
	     topTagJet2_topTagFlag |= 8;
	     //toptagindexJet2_trimmed = i;
	     trimmedJet2.SetPtEtaPhiM((*h_jetsAK8Pt)[i],(*h_jetsAK8Eta)[i],(*h_jetsAK8Phi)[i],(*h_jetsAK8topMass)[i]);
	     }*/
	 }//end AK8 jet 2 loop
	 nAK8pt400eta2p4jets +=1;//top tagging candidate iterator
       }//pt and eta preselection cuts
     }//end AK8 jet loop
   }//checking AK8 jet validity

   treeVars["nAK8pt400eta2p4jets"] = nAK8pt400eta2p4jets;

   //only filling tree if the event passes the filters
   if ((cscFilt == 1) && (vertexFilt == 1) && (hbheFilt == 1)){

     //cout <<"Filters passed!"<<endl;

     //filling out the cutflow values
     treeVars["cutflow"] = 0.0;
     tree_had->Fill();
     
     if (nAK8pt400eta2p4jets > 0){
       treeVars["cutflow"] = 1.0;//1 AK8 jet that passes preselection
       tree_had->Fill();
       if (nAK8pt400eta2p4jets > 1){
	 treeVars["cutflow"] = 2.0;//2 AK8 jets that pass preselection
	 tree_had->Fill();
	 //Reconstructing the Z-peak with 2 good AK8 jets
	 TLorentzVector softDrop_Z = softDropJet1 + softDropJet2;
	 treeVars["softDrop_Z_mass"] = softDrop_Z.M();
	 TLorentzVector ungroomed_Z = ungroomedJet1 + ungroomedJet2;
	 treeVars["ungroomed_Z_mass"] = ungroomed_Z.M();
       }//2+ good AK8 jets
       if ((topTagJet1_topTagFlag == 1) || (topTagJet2_topTagFlag == 1)){
	 treeVars["cutflow"] = 3.0;//1 good AK8 jet in mass window
	 tree_had->Fill();
	 if((topTagJet1_topTagFlag == 1) && (topTagJet2_topTagFlag == 1)){
	   treeVars["cutflow"] = 4.0;//2 good AK8 jets in mass window
	   tree_had->Fill();
	   if(nNsubjettinessCutJets > 1){
	     treeVars["cutflow"] = 5.0;//2 good AK8 jets in mass window and 2 good jets passing Nsubjettiness cut
	     tree_had->Fill();
	     if(nBtagJets > 0){
	       treeVars["cutflow"] = 6.0;//2 good AK8 jets in mass window, 2 good jet passing Nsubjettiness cut, and 1 good b-tagged jets
	       tree_had->Fill();
	     }//1+ b-tag jet
	     if(nBtagJets > 1){
	       treeVars["cutflow"] = 7.0;//2 good AK8 jets in mass window, 2 good jet passing Nsubjettiness cut, and 2 good b-tagged jets
	       tree_had->Fill();
	     }//1+ b-tag jet
	   }//2 jets passing Nsubjettiness cut
	 }//2 jets in mass window
       }//1+ jet in mass window
     }//1+ good AK8 jets

     //cout<<"cutflow end"<<endl;

     //2 cmstt top tag jets
     /*if(((topTagJet1_topTagFlag & 1) == 1) && ((topTagJet2_topTagFlag & 1) == 1)){
       TLorentzVector cmstt_Z = cmsttJet1 + cmsttJet2;
       treeVars["cmstt_Z_mass"] = cmstt_Z.M();
       }
   //2 filtered top tag jets
   if(((topTagJet1_topTagFlag & 2) == 2) && ((topTagJet2_topTagFlag & 2) == 2)){
     TLorentzVector filtered_Z = filteredJet1 + filteredJet2;
     treeVars["filtered_Z_mass"] = filtered_Z.M();
											       }
   //2 pruned top tag jets
   if(((topTagJet1_topTagFlag & 4) == 4) && ((topTagJet2_topTagFlag & 4) == 4)){
     TLorentzVector pruned_Z = prunedJet1 + prunedJet2;
     treeVars["pruned_Z_mass"] = pruned_Z.M();
											       }
   //2 trimmed top tag jets
   if(((topTagJet1_topTagFlag & 8) == 8) && ((topTagJet2_topTagFlag & 8) == 8)){
     TLorentzVector trimmed_Z = trimmedJet1 + trimmedJet2;
     treeVars["trimmed_Z_mass"] = trimmed_Z.M();
     }*/

   /////Number of b-tags per event
   //if (topTagJet1_topTagFlag > 0 || topTagJet2_topTagFlag > 0){
     
   //}


   //cout<<"Number of top tagged jets: "<<nTopTagJets<<endl;
   //cout<<"Number of AK8 jets with pt > 400 GeV: "<<nAK8pt400eta2p4jets<<endl;
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
ZprimeB2Ganalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZprimeB2Ganalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
ZprimeB2Ganalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
ZprimeB2Ganalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
ZprimeB2Ganalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
ZprimeB2Ganalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZprimeB2Ganalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZprimeB2Ganalyzer);
