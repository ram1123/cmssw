#ifndef Analyzer_BDTCPPF_BDTCPPF_h
#define Analyzer_BDTCPPF_BDTCPPF_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/L1TMuon/interface/CPPFDigi.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CondFormats/RPCObjects/interface/RPCMaskedStrips.h"
#include "CondFormats/RPCObjects/interface/RPCDeadStrips.h"
#include "CondFormats/Serialization/interface/Serializable.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"

#include<boost/cstdint.hpp>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1.h"
#include "TH2.h"
#include <memory>
#include <iostream>
#include <vector>
#include <ios>
#include <map>

class BDTCPPF : public edm::EDAnalyzer {

 public:

  explicit BDTCPPF(const edm::ParameterSet&);
  ~BDTCPPF() override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::ESHandle <RPCGeometry> rpcGeom;
 private:

  void beginJob() override;

  const edm::EDGetTokenT<std::vector<reco::GenParticle>> genParToken_;   
  const edm::EDGetTokenT<RPCRecHitCollection> recHitToken_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken_;

  //For Muon
  TVector3 Gen_Muon;

  //Float_t Pt_Muon;
  Float_t Pt_Muon_Matched;
  Float_t IPt_Muon;
  Int_t NMuons;
  Int_t NRechits;
  Int_t NCPPF;

  Int_t phi1;
  Int_t phi2;
  Int_t phi3;
  Int_t phi4;

  Int_t theta1;
  Int_t theta2;
  Int_t theta3;
  Int_t theta4;

  Int_t deltaphi12;
  Int_t deltaphi23;
  Int_t deltaphi34;

  Int_t deltatheta12;
  Int_t deltatheta23;
  Int_t deltatheta34;
  Int_t deltatheta14;

  Int_t cluster1;
  Int_t cluster2;
  Int_t cluster3;
  Int_t cluster4;

  TTree* tree_;

};


#endif
