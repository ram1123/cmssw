#ifndef L1Trigger_L1TMuonCPPF_RecHitProcessor_h
#define L1Trigger_L1TMuonCPPF_RecHitProcessor_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/L1TMuon/interface/CPPFDigi.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "CondFormats/RPCObjects/interface/RPCMaskedStrips.h"
#include "CondFormats/RPCObjects/interface/RPCDeadStrips.h"

#include "CondFormats/Serialization/interface/Serializable.h"
#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"

#include<boost/cstdint.hpp>
#include<vector>
#include<iostream>
#include<sstream>
#include<string> 
#include<memory>
#include<utility>
#include <algorithm>
#include <map>

class RecHitProcessor {
 public:
  explicit RecHitProcessor();
  ~RecHitProcessor();
  
  struct CppfItem {
    
    int lb;
    int rawId;
    int strip;
    int lbchannel;
    int halfchannel;
    int int_phi;
    int int_theta;
    COND_SERIALIZABLE;
  };
  
  std::vector<CppfItem> const & getCppfVec() const {return CppfVec;}
  std::vector<CppfItem> CppfVec; 
  
  
  void processLook(
		   // Input
		   const edm::Event& iEvent,
		   const edm::EventSetup& iSetup,
		   const edm::EDGetToken& recHitToken,
		   std::vector<RecHitProcessor::CppfItem>& CppfVec1, 
		   // Output
		   l1t::CPPFDigiCollection& cppfDigis,
		   const int MaxClusterSize,
		   std::vector<int> BxCut
		   ) const;
  
  void process(
	       // Input
	       const edm::Event& iEvent,
	       const edm::EventSetup& iSetup,
	       const edm::EDGetToken& recHitToken,
	       // Output
	       l1t::CPPFDigiCollection& cppfDigis,
	       const int MaxClusterSize,
	       std::vector<int> BxCut
	       ) const;
  
 
  COND_SERIALIZABLE;
  
 private:

  std::shared_ptr<l1t::CPPFDigi> MainVariables1; 
  std::shared_ptr<l1t::CPPFDigi> MainVariables2; 
  
};

#endif /* #define L1Trigger_L1TMuonCPPF_RecHitProcessor_h */
