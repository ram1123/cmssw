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

  TH1D* h1_nEvents; ///< Total number of event over which we run this framework
  TH1D* h1_nChmbersWithHit_Cu;  ///< Number of chamber distribution having hits
  TH1D* h1_nEvents_Cu_nHitMoreThan2;  ///< #nEvents for unpacker if (nHits > 2 )
  TH1D* h1_nEvents_Cu_bx; ///< #nEvents for unpacker if (nHits > 2 && bx==0 for each individual hit)
  TH1D* h1_nEvents_Cu_Allbx;  ///< #nEvents for unpacker if (nHits > 2 && bx==0 for all hits on an individual chamber.)

  TH1D* h1_nChmbersWithHit_Ce;  ///< Number of chamber distribution having hits
  TH1D* h1_nEvents_Ce_nHitMoreThan2;  ///< #nEvents for emulator if (nHits > 2 )
  TH1D* h1_nEvents_Ce_bx; ///< #nEvents for emulator if (nHits > 2 && bx==0 for each individual hit)
  TH1D* h1_nEvents_Ce_Allbx;  ///< #nEvents for emulator if (nHits > 2 && bx==0 for all hits on an individual chamber.)

  TH1D* h1_nEvents_CuCe_SameKey;  ///< #nEvents if unpacker and emulator have same key 
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits;  ///< #nEvents if unpacker and emulator have same key && same total number of hits on each chamber 
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx;  ///< #nEvents if unpacker and emulator have same key && same total number of hits on each chamber && if any hit has bx==0
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx; ///< #nEvents if unpacker and emulator have same key&& same total number of hits on each chamber && if all hits has bx==0
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onPhi; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offPhi; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onTheta; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OnTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offTheta; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if any hit has bx==0 && OffTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_onPhi; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OnPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_offPhi; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OffPhi
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_onTheta; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OnTheta
  TH1D* h1_nEvents_CuCe_SameKey_SameNHits_AllBx_offTheta; ///< #nEvents if unpacker and emulator have same keys && Same number of total hits on each chamber && if all hit has bx==0 && OffTheta

  TH1D* h1_total_hits_unpacker; ///< Number of total hits on one chamber in an unpacker. 
  TH1D* h1_total_hits_unpacker_bx; ///< Number of hits on one chamber in unpacker (bxE == bxU)
  TH1D* h1_total_hits_unpacker_bx_phi;  ///< Number of hits  on one chamber in unpacker (#phiE == #phiU)

  TH1D* h1_cluster_size_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_phi_Cu; ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_phi_glob_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_theta_Cu; ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_theta_glob_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_ID_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_zone_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
  TH1D* h1_roll_Cu;  ///<  ;Unpacker; Number of hits on a chamber > 2
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
  TH1D* h1_emtfSector_Cu_bx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Cu_bx  ; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_cluster_size_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_phi_Cu_Allbx; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_phi_glob_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_theta_Cu_Allbx; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_theta_glob_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_ID_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_zone_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_roll_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_emtfSector_Cu_Allbx;  ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber
  TH1D* h1_emtfSubsector_Cu_Allbx  ; ///<  ;Unpacker; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber

  TH1D* h1_total_hits_emulator; ///< Number of hits on one chamber in emulator
  TH1D* h1_total_hits_emulator_bx;  ///< Number of hits on one chamber in emulator (bxE == bxU)
  TH1D* h1_total_hits_emulator_bx_phi;  ///< Number of hits on one chamber in emulator (#phiE == #phiU)

  TH1D* h1_cluster_size_Ce;  ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_phi_Ce; ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_phi_glob_Ce;  ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_theta_Ce; ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_theta_glob_Ce;  ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_ID_Ce;  ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_zone_Ce;  ///< ;Emulator; #nHits on chamber > 2 
  TH1D* h1_roll_Ce;  ///< ;Emulator; #nHits on chamber > 2 
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
  TH1D* h1_emtfSector_Ce_bx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0
  TH1D* h1_emtfSubsector_Ce_bx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0

  TH1D* h1_cluster_size_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_phi_Ce_Allbx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_phi_glob_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_theta_Ce_Allbx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_theta_glob_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_ID_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_zone_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_roll_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_emtfSector_Ce_Allbx;  ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.
  TH1D* h1_emtfSubsector_Ce_Allbx; ///< ;Emulator; #nHits on chamber > 2 && bunch crossing == 0 for all hits on a chamber.

  TH1D* h1_SameKey_cluster_size_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_phi_Cu; ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_theta_Cu; ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_ID_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_zone_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_roll_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Cu;  ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Cu  ; ///<  ;Unpacker; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_phi_Ce; ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_theta_Ce; ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_ID_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_zone_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_roll_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Ce;  ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Ce; ///< ;Emulator; && Hit at same chamber in unpacker and emulator && #nHits is same in both unpacker and emulator
  
  TH1D* h1_SameKey_cluster_size_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_Cu_bx; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_ID_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_zone_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_roll_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Cu_bx;  ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Cu_bx  ; ///<  ;Unpacker; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_ID_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_zone_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_roll_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Ce_bx;  ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Ce_bx; ///< ;Emulator; bunch crossing == 0; && Hit at same chamber in unpacker and emulator

  TH1D* h1_SameKey_cluster_size_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_Cu_Allbx; ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_Cu_Allbx; ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_ID_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_zone_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_roll_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Cu_Allbx;  ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Cu_Allbx  ; ///<  ;Unpacker; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_cluster_size_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_Ce_Allbx; ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_phi_glob_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_Ce_Allbx; ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_theta_glob_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_ID_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_zone_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_roll_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSector_Ce_Allbx;  ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator
  TH1D* h1_SameKey_emtfSubsector_Ce_Allbx; ///< ;Emulator; bunch crossing == 0 for all hits on a chamber; && Hit at same chamber in unpacker and emulator

  TH1D* h1_SameKey_OnPhi_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
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
  TH1D* h1_SameKey_OnPhi_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis

  TH1D* h1_SameKey_OnPhi_cluster_size_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_glob_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_glob_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ID_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_zone_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_roll_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSector_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSubsector_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_cluster_size_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_phi_glob_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_theta_glob_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_ID_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_zone_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_roll_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSector_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH1D* h1_SameKey_OnPhi_emtfSubsector_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  
  TH1D* h1_SameKey_OffPhi_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
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
  TH1D* h1_SameKey_OffPhi_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
 
  TH1D* h1_SameKey_OffPhi_cluster_size_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_glob_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_glob_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ID_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_zone_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_roll_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSector_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSubsector_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_cluster_size_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_phi_glob_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_theta_glob_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_ID_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_zone_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_roll_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSector_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH1D* h1_SameKey_OffPhi_emtfSubsector_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis

  TH1D* h1_SameKey_OnTheta_cluster_size_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_glob_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ID_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_zone_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_roll_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
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
  TH1D* h1_SameKey_OnTheta_emtfSector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSubsector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis

  TH1D* h1_SameKey_OnTheta_cluster_size_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_glob_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_glob_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ID_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_zone_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_roll_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSector_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSubsector_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_cluster_size_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_phi_glob_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_theta_glob_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_ID_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_zone_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_roll_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSector_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH1D* h1_SameKey_OnTheta_emtfSubsector_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  
  TH1D* h1_SameKey_OffTheta_cluster_size_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_glob_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ID_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_zone_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_roll_Cu_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
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
  TH1D* h1_SameKey_OffTheta_emtfSector_Ce_bx; ///<  ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSubsector_Ce_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis

  TH1D* h1_SameKey_OffTheta_cluster_size_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_glob_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_glob_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ID_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_zone_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_roll_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSector_Cu_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSubsector_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_cluster_size_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_phi_glob_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_theta_glob_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_ID_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_zone_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_roll_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSector_Ce_Allbx; ///<  ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH1D* h1_SameKey_OffTheta_emtfSubsector_Ce_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis

  TH2D* h2_SameKey_OnPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_theta_Ce_theta_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_ID_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_roll_Ce; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Cu_roll_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_zone_Ce_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnPhi Axis

  TH2D* h2_SameKey_OnPhi_phi_Ce_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_theta_Ce_theta_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_ID_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Ce_roll_Ce_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_ID_Cu_roll_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis
  TH2D* h2_SameKey_OnPhi_zone_Ce_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnPhi Axis

  TH2D* h2_SameKey_OffPhi_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_theta_Ce_theta_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_ID_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_roll_Ce; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Cu_roll_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_zone_Ce_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffPhi Axis

  TH2D* h2_SameKey_OffPhi_phi_Ce_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_theta_Ce_theta_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_ID_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Ce_roll_Ce_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_ID_Cu_roll_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis
  TH2D* h2_SameKey_OffPhi_zone_Ce_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffPhi Axis

  TH2D* h2_SameKey_OnTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_theta_Ce_theta_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_ID_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_roll_Ce; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Cu_roll_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_zone_Ce_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OnTheta Axis

  TH2D* h2_SameKey_OnTheta_phi_Ce_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber&& Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_theta_Ce_theta_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_ID_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Ce_roll_Ce_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_ID_Cu_roll_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_Allbx;  ///< ;; bunch crossing == 0 && for each hit on a chamber Hit at same chamber && OnTheta Axis
  TH2D* h2_SameKey_OnTheta_zone_Ce_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 && for each hit on a chamber Hit at same chamber && OnTheta Axis

  TH2D* h2_SameKey_OffTheta_phi_Ce_phi_Cu_bx; ///< ; ; bunch crossing ==0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_theta_Ce_theta_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_ID_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_roll_Ce; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Cu_roll_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu; ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_zone_Ce_zone_Cu;  ///< ;; bunch crossing == 0 && Hit at same chamber && OffTheta Axis

  TH2D* h2_SameKey_OffTheta_phi_Ce_phi_Cu_Allbx; ///< ; ; bunch crossing ==0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_theta_Ce_theta_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_ID_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Ce_roll_Ce_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_ID_Cu_roll_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_Allbx; ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis
  TH2D* h2_SameKey_OffTheta_zone_Ce_zone_Cu_Allbx;  ///< ;; bunch crossing == 0 for each hit on a chamber && Hit at same chamber && OffTheta Axis

};


#endif

