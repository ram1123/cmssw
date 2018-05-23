#ifndef L1Trigger_DQMCPPF_h
#define L1Trigger_DQMCPPF_h

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
#include "DataFormats/L1TMuon/interface/CPPFDigi.h"
#include "DataFormats/Common/interface/Handle.h"

#include "CondFormats/RPCObjects/interface/RPCMaskedStrips.h"
#include "CondFormats/RPCObjects/interface/RPCDeadStrips.h"
#include "CondFormats/Serialization/interface/Serializable.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"

#include<boost/cstdint.hpp>
#include "TH1.h"
#include "TH2.h"
#include <memory>
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <ios>
#include <map>


class DQM_CPPF : public edm::EDAnalyzer {
  
 public:
  explicit DQM_CPPF(const edm::ParameterSet&);
  ~DQM_CPPF() override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::ESHandle <RPCGeometry> rpcGeom;
  
 private:
  
  void beginJob() override;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken1_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken2_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken3_;

  int EMTF_sector;
  int EMTF_subsector;
  int EMTF_subsector_unpacker;
  int EMTF_subsector_EMTF;
  int EMTF_bx;
 
  std::vector<int> phi_int_unpacker;
  std::vector<int> theta_int_unpacker;
  std::vector<int> emtf_sector_unpacker;
  std::vector<int> subsector_unpacker;
 
  std::vector<int> phi_int_EMTF;
  std::vector<int> theta_int_EMTF;
  std::vector<int> emtf_sector_EMTF;
  std::vector<int> subsector_EMTF;

  std::vector<int> EMTFsector0bins;
  std::vector<int> EMTFsector1bins;
  std::vector<int> EMTFsector2bins;
  std::vector<int> EMTFsector3bins;
  std::vector<int> EMTFsector4bins;
  std::vector<int> EMTFsector5bins;
  std::vector<int> EMTFsector6bins;
  
  std::map<int , std::vector<int>> fill_info;
  
  TH1D* Phi_Integer;
  TH1D* Theta_Integer;
  TH1D* Phi_Integer_unpacker;
  TH1D* Theta_Integer_unpacker;

  TH1D* Phi_Global;
  TH1D* Theta_Global;

  TH1D* Board;
  TH1D* Channel;

  TH1D* Matches;
  TH1D* Matches_e;
  TH1D* Matches_e_minus1;
  TH1D* Matches_e_minus2;
  TH1D* Matches_e_plus1;
  TH1D* Matches_e_plus2;

  TH2D* Phi_Global_Integer;
  TH2D* Theta_Global_Integer;
  TH2D* Occupancy;
  TH2D* Occupancy_unpacker;
  TH2D* Occupancy_EMTF;
  TH2D* Bx;
  TH2D* Bx_Occupancy;

  TH2D* Phi_emu_unpacker;
  TH2D* Theta_emu_unpacker;

  TH2D* Phi_emu_emtf;
  TH2D* Theta_emu_emtf;

  TH2D* Phi_emu_unpacker_nomatched;
  TH2D* Theta_emu_unpacker_nomatched;
  TH2D* Phi_emu_unpacker_near;
  TH2D* Theta_emu_unpacker_near;

  TH2D* emtf_sector_comparison;
};


#endif


