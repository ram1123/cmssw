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
#include <unordered_map>

class DQM_CPPF : public edm::EDAnalyzer {

public:
  explicit DQM_CPPF(const edm::ParameterSet&);
  ~DQM_CPPF() override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  int occupancy_value(int region_, int station_, int ring_);
  int bx_value(int region_, int emtfsector_);
  int GetSubsector(int emtfsector_, int lsubsector_);

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

  // std::vector<int> EMTFsector0bins;
  // std::vector<int> EMTFsector1bins;
  // std::vector<int> EMTFsector2bins;
  // std::vector<int> EMTFsector3bins;
  // std::vector<int> EMTFsector4bins;
  // std::vector<int> EMTFsector5bins;
  // std::vector<int> EMTFsector6bins;

  // std::map<int , std::vector<int>> fill_info;
  //
  // comment format
  // Title;x-axis;y-axis
  TH1D* h1_nEvents; ///< #events

  TH1D* h1_nEvents_Ce_NoHits; ///< Unpacker: nHits == 0 ;#nEvents;
  TH1D* h1_nEvents_Cu_NoHits; ///< Emulator: nHits == 0 ;#nEvents;

  TH1D* h1_nChmbersWithHit_Cu;  ///< Unpacker: #nChambers having Hit/event; #nChambers with Hit/event;
  TH1D* h1_nEvents_Cu_nHitNotMoreThan2;  ///< Unpacker: nHits >= 2;#nEvents;
  TH1D* h1_nEvents_Cu_bx; ///<  Unpacker: nHits >= 2 && (bx==0 for each individual hit);#nEvents;

  TH1D* h1_nChmbersWithHit_Ce;  ///< Emulator: #nChambers having Hit/event; #nChambers with Hit/event;
  TH1D* h1_nEvents_Ce_nHitNotMoreThan2;  ///< Emulator: nHits >= 2;#nEvents;
  TH1D* h1_nEvents_Ce_bx; ///< Emulator: nHits >= 2 && (bx==0 for each individual hit);#nEvents;

  TH1D* h1_nEvents_CuCe_SameKey;  ///< #nEvents if Unpacker and emulator have same key
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits;  ///< #nEvents if Unpacker and emulator have same key && same total number of hits on each chamber
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx;  ///< #nEvents if Unpacker and emulator have same key && same total number of hits on each chamber && if any hit has bx==0
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx; ///< #nEvents if Unpacker and emulator have same key&& same total number of hits on each chamber && if all hits has bx==0
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onPhi; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offPhi; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onTheta; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offTheta; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffTheta
  TH1D* h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onPhi; ///< #nEvents after matching if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnPhi
  TH1D* h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offPhi; ///< #nEvents after matching if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffPhi
  TH1D* h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onTheta; ///< #nEvents after matching if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnTheta
  TH1D* h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offTheta; ///< #nEvents after matching if Unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_onPhi; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OnPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_offPhi; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OffPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_onTheta; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OnTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_offTheta; ///< #nEvents if Unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OffTheta

  TH1D* h1_total_hits_unpacker; ///< Number of total hits on one chamber in an Unpacker.
  TH1D* h1_total_hits_unpacker_LessThan3Hit; ///< Number of hits on one chamber in Unpacker (bxE == bxU)
  TH1D* h1_total_hits_unpacker_LessThan3Hit_BxZero;  ///< Number of hits  on one chamber in Unpacker (#phiE == #phiU)

  TH1D* h1_cluster_size_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_phi_Cu; ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_phi_glob_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_theta_Cu; ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_theta_glob_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_ID_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_zone_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_roll_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_ring_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_emtfSector_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_emtfSubsector_Cu  ; ///<  ;Unpacker; Number of hits on a chamber > 2

  TH1D* h1_cluster_size_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_Cu_bx; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_glob_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_Cu_bx; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_glob_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ID_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_zone_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_roll_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ring_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSector_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Cu_bx  ; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_cluster_size_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_Cu_bx_OneHit; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_glob_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_Cu_bx_OneHit; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_glob_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ID_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_zone_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_roll_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ring_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSector_Cu_bx_OneHit;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Cu_bx_OneHit  ; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_total_hits_emulator; ///< Number of hits on one chamber in emulator
  TH1D* h1_total_hits_emulator_LessThan3Hit;  ///< Number of hits on one chamber in emulator (bxE == bxU)
  TH1D* h1_total_hits_emulator_LessThan3Hit_BxZero;  ///< Number of hits on one chamber in emulator (#phiE == #phiU)

  TH1D* h1_total_hits_SameKey_Cu; ///< Same SubSector;#nHits;
  TH1D* h1_total_hits_SameKey_Ce; ///< Same SubSector;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits; ///< Same SubSector && Same #nHits;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx; ///< Same SubSector && Same #nHits && bx==0;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_OnPhi; ///< Same SubSector && Same #nHits && bx==0 && OnPhi;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_OffPhi; ///< Same SubSector && Same #nHits && bx==0 && OffPhi;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_OnTheta; ///< Same SubSector && Same #nHits && bx==0 && OnTheta;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_OffTheta; ///< Same SubSector && Same #nHits && bx==0 && OffTheta;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_Matching_OnPhi; ///< Same SubSector && Same #nHits && bx==0 && OnPhi && Matching;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_Matching_OffPhi; ///< Same SubSector && Same #nHits && bx==0 && OffPhi && Matching;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_Matching_OnTheta; ///< Same SubSector && Same #nHits && bx==0 && OnTheta && Matching;#nHits;
  TH1D* h1_total_hits_SameKey_SameNHits_bx_Matching_OffTheta; ///< Same SubSector && Same #nHits && bx==0 && OffTheta && Matching;#nHits;

  TH1D* h1_cluster_size_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_phi_Ce; ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_phi_glob_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_theta_Ce; ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_theta_glob_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_ID_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_zone_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_roll_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_ring_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_emtfSector_Ce;  ///< ;Emulator; #nHits on chamber > 2
  TH1D* h1_emtfSubsector_Ce; ///< ;Emulator; #nHits on chamber > 2

  TH1D* h1_cluster_size_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_Ce_bx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_glob_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_Ce_bx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_glob_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ID_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_zone_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_roll_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ring_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSector_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Ce_bx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_cluster_size_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_Ce_bx_OneHit; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_phi_glob_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_Ce_bx_OneHit; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_theta_glob_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ID_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_zone_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_roll_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_ring_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSector_Ce_bx_OneHit;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Ce_bx_OneHit; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_SameKey_cluster_size_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_phi_Cu; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_theta_Cu; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_ID_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_zone_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_roll_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_ring_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Cu  ; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_phi_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_theta_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_ID_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_zone_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_roll_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_ring_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_phi_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_theta_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_ID_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_zone_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_roll_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_ring_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Cu_bx  ; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_phi_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_theta_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_ID_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_zone_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_roll_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_ring_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OnPhi_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ring_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSubsector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_cluster_size_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ID_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_zone_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_roll_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ring_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis


  TH1D* h1_SameKey_OneHit_cluster_size_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_Cu; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_Cu; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ID_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_zone_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_roll_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ring_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSector_Cu;  ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSubsector_Cu  ; ///<  ;Unpacker; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_cluster_size_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_glob_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_glob_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ID_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_zone_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_roll_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ring_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSector_Ce;  ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSubsector_Ce; ///< ;Emulator; && Hit at same chamber in Unpacker and emulator && #nHits is same in both Unpacker and emulator
  TH1D* h1_SameKey_OneHit_cluster_size_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ID_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_zone_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_roll_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ring_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSector_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSubsector_Cu_bx  ; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_cluster_size_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_phi_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_theta_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ID_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_zone_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_roll_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_ring_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSector_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_emtfSubsector_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in Unpacker and emulator
  TH1D* h1_SameKey_OneHit_OnPhi_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_ring_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_emtfSector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_emtfSubsector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_cluster_size_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_phi_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_phi_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_theta_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_theta_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_ID_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_zone_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_roll_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_ring_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OneHit_OnPhi_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis

  TH1D* h1_Matching_SameKey_OnPhi_cluster_size_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_phi_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_phi_glob_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_theta_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_theta_glob_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_ID_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_zone_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_roll_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_ring_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_emtfSector_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_emtfSubsector_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_cluster_size_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_phi_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_phi_glob_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_theta_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_theta_glob_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_ID_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_zone_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_roll_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_ring_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_emtfSector_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_Matching_SameKey_OnPhi_emtfSubsector_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis


  TH1D* h1_SameKey_OneHit_OffPhi_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_ring_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_emtfSector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_emtfSubsector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_cluster_size_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_phi_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_phi_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_theta_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_theta_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_ID_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_zone_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_roll_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_ring_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OneHit_OffPhi_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis

  TH1D* h1_SameKey_OffPhi_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ring_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSubsector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_cluster_size_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ID_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_zone_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_roll_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ring_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis

  TH1D* h1_Matching_SameKey_OffPhi_cluster_size_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_phi_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_phi_glob_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_theta_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_theta_glob_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_ID_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_zone_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_roll_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_ring_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_emtfSector_Cu_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_emtfSubsector_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_cluster_size_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_phi_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_phi_glob_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_theta_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_theta_glob_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_ID_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_zone_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_roll_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_ring_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_emtfSector_Ce_bx; ///<  ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_Matching_SameKey_OffPhi_emtfSubsector_Ce_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis

  TH1D* h1_SameKey_OnTheta_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ring_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSubsector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_cluster_size_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ID_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_zone_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_roll_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ring_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis

  TH1D* h1_SameKey_OneHit_OnTheta_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_ring_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_emtfSector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_emtfSubsector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_cluster_size_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_phi_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_phi_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_theta_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_theta_glob_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_ID_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_zone_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_roll_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_ring_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OneHit_OnTheta_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis

  TH1D* h1_Matching_SameKey_OnTheta_cluster_size_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_phi_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_phi_glob_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_theta_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_theta_glob_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_ID_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_zone_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_roll_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_ring_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_emtfSector_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_emtfSubsector_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_cluster_size_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_phi_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_phi_glob_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_theta_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_theta_glob_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_ID_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_zone_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_roll_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_ring_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_emtfSector_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_Matching_SameKey_OnTheta_emtfSubsector_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis

  TH1D* h1_SameKey_OffTheta_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ring_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSubsector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_cluster_size_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ID_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_zone_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_roll_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ring_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis

  TH1D* h1_SameKey_OneHit_OffTheta_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_ring_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_emtfSector_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_emtfSubsector_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_cluster_size_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_phi_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_phi_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_theta_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_theta_glob_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_ID_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_zone_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_roll_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_ring_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OneHit_OffTheta_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis

  TH1D* h1_Matching_SameKey_OffTheta_cluster_size_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_phi_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_phi_glob_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_theta_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_theta_glob_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_ID_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_zone_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_roll_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_ring_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_emtfSector_Cu_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_emtfSubsector_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_cluster_size_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_phi_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_phi_glob_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_theta_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_theta_glob_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_ID_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_zone_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_roll_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_ring_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_emtfSector_Ce_bx; ///<  ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_Matching_SameKey_OffTheta_emtfSubsector_Ce_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis

  TH2D* h2_SameKey_phi_Ce_phi_Cu_bx; ///<
  TH2D* h2_SameKey_theta_Ce_theta_Cu_bx; ///<
  TH2D* h2_SameKey_ID_Ce_ID_Cu_bx; ///<
  TH2D* h2_SameKey_ID_Ce_roll_Ce_bx; ///<
  TH2D* h2_SameKey_ID_Cu_roll_Cu_bx; ///<
  TH2D* h2_SameKey_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///<
  TH2D* h2_SameKey_emtfSubsector_Cu_zone_Cu_bx; ///<
  TH2D* h2_SameKey_emtfSubsector_Ce_zone_Ce_bx; ///<
  TH2D* h2_SameKey_zone_Ce_zone_Cu_bx; ///<
  TH2D* h2_SameKey_OnPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis

  TH2D* h2_SameKey_OneHit_phi_Ce_phi_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_theta_Ce_theta_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_ID_Ce_ID_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_ID_Ce_roll_Ce_bx; ///<
  TH2D* h2_SameKey_OneHit_ID_Cu_roll_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_emtfSubsector_Cu_zone_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_emtfSubsector_Ce_zone_Ce_bx; ///<
  TH2D* h2_SameKey_OneHit_zone_Ce_zone_Cu_bx; ///<
  TH2D* h2_SameKey_OneHit_OnPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OneHit_OnPhi_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis

  TH2D* h2_Matching_SameKey_OnPhi_phi_Ce_phi_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_theta_Ce_theta_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_ID_Ce_ID_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_ID_Ce_roll_Ce_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_ID_Cu_roll_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_Matching_SameKey_OnPhi_zone_Ce_zone_Cu_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OnPhi Axis

  TH2D* h2_SameKey_OffPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis

  TH2D* h2_SameKey_OneHit_OffPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OneHit_OffPhi_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis

  TH2D* h2_Matching_SameKey_OffPhi_phi_Ce_phi_Cu_bx; ///< ; Matching Phi ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_theta_Ce_theta_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_ID_Ce_ID_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_ID_Ce_roll_Ce_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_ID_Cu_roll_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_Matching_SameKey_OffPhi_zone_Ce_zone_Cu_bx;  ///< ;; Matching Phi bunch crossing == 0 && Hit at same chamber && OffPhi Axis

  TH2D* h2_SameKey_OnTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis

  TH2D* h2_SameKey_OneHit_OnTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OneHit_OnTheta_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis

  TH2D* h2_Matching_SameKey_OnTheta_phi_Ce_phi_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_theta_Ce_theta_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_ID_Ce_ID_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_ID_Ce_roll_Ce_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_ID_Cu_roll_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_Matching_SameKey_OnTheta_zone_Ce_zone_Cu_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OnTheta Axis

  TH2D* h2_SameKey_OffTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis

  TH2D* h2_SameKey_OneHit_OffTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_theta_Ce_theta_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_ID_Ce_ID_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_ID_Ce_roll_Ce_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_ID_Cu_roll_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OneHit_OffTheta_zone_Ce_zone_Cu_bx;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis

  TH2D* h2_Matching_SameKey_OffTheta_phi_Ce_phi_Cu_bx; ///< ; Matching Theta ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_theta_Ce_theta_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_ID_Ce_ID_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_ID_Ce_roll_Ce_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_ID_Cu_roll_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx; ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_Matching_SameKey_OffTheta_zone_Ce_zone_Cu_bx;  ///< ;; Matching Theta bunch crossing == 0 && Hit at same chamber && OffTheta Axis
};


#endif

