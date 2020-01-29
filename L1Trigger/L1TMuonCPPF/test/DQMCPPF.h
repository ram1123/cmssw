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

  std::vector<int> ar_cluster_sizeCe;
  std::vector<int> ar_cluster_sizeCu;
  std::vector<int> ar_bxCe;
  std::vector<int> ar_bxCu;
  std::vector<int> ar_phiIntCe;
  std::vector<int> ar_phiIntCu;
  std::vector<int> ar_thetaIntCu;
  std::vector<int> ar_thetaIntCe;
  std::vector<int> ar_thetaGlobalCe;
  std::vector<int> ar_thetaGlobalCu;
  std::vector<int> ar_phiGlobalCe;
  std::vector<int> ar_phiGlobalCu;
  std::vector<int> ar_chamberIDCe;
  std::vector<int> ar_chamberIDCu;
  std::vector<int> ar_rollCe;
  std::vector<int> ar_rollCu;
  std::vector<int> ar_emtfSubsectorCu;
  std::vector<int> ar_emtfSubsectorCe;
  std::vector<int> ar_fillOccupancyCu;
  std::vector<int> ar_fillOccupancyCe;

  std::vector<int> ar_cluster_sizeCe_bx;
  std::vector<int> ar_cluster_sizeCu_bx;
  std::vector<int> ar_bxCe_bx;
  std::vector<int> ar_bxCu_bx;
  std::vector<int> ar_phiIntCe_bx;
  std::vector<int> ar_phiIntCu_bx;
  std::vector<int> ar_thetaIntCu_bx;
  std::vector<int> ar_thetaIntCe_bx;
  std::vector<int> ar_thetaGlobalCe_bx;
  std::vector<int> ar_thetaGlobalCu_bx;
  std::vector<int> ar_phiGlobalCe_bx;
  std::vector<int> ar_phiGlobalCu_bx;
  std::vector<int> ar_chamberIDCe_bx;
  std::vector<int> ar_chamberIDCu_bx;
  std::vector<int> ar_rollCe_bx;
  std::vector<int> ar_rollCu_bx;
  std::vector<int> ar_emtfSubsectorCu_bx;
  std::vector<int> ar_emtfSubsectorCe_bx;
  std::vector<int> ar_fillOccupancyCu_bx;
  std::vector<int> ar_fillOccupancyCe_bx;

  std::vector<int> ar_cluster_sizeCe_bx_phi;
  std::vector<int> ar_cluster_sizeCu_bx_phi;
  std::vector<int> ar_bxCe_bx_phi;
  std::vector<int> ar_bxCu_bx_phi;
  std::vector<int> ar_phiIntCe_bx_phi;
  std::vector<int> ar_phiIntCu_bx_phi;
  std::vector<int> ar_thetaIntCu_bx_phi;
  std::vector<int> ar_thetaIntCe_bx_phi;
  std::vector<int> ar_thetaGlobalCe_bx_phi;
  std::vector<int> ar_thetaGlobalCu_bx_phi;
  std::vector<int> ar_phiGlobalCe_bx_phi;
  std::vector<int> ar_phiGlobalCu_bx_phi;
  std::vector<int> ar_chamberIDCe_bx_phi;
  std::vector<int> ar_chamberIDCu_bx_phi;
  std::vector<int> ar_rollCe_bx_phi;
  std::vector<int> ar_rollCu_bx_phi;
  std::vector<int> ar_emtfSubsectorCu_bx_phi;
  std::vector<int> ar_emtfSubsectorCe_bx_phi;
  std::vector<int> ar_fillOccupancyCu_bx_phi;
  std::vector<int> ar_fillOccupancyCe_bx_phi;

  std::vector<int> ar_cluster_sizeCe_bx_Offphi;
  std::vector<int> ar_cluster_sizeCu_bx_Offphi;
  std::vector<int> ar_bxCe_bx_Offphi;
  std::vector<int> ar_bxCu_bx_Offphi;
  std::vector<int> ar_phiIntCe_bx_Offphi;
  std::vector<int> ar_phiIntCu_bx_Offphi;
  std::vector<int> ar_thetaIntCu_bx_Offphi;
  std::vector<int> ar_thetaIntCe_bx_Offphi;
  std::vector<int> ar_thetaGlobalCe_bx_Offphi;
  std::vector<int> ar_thetaGlobalCu_bx_Offphi;
  std::vector<int> ar_phiGlobalCe_bx_Offphi;
  std::vector<int> ar_phiGlobalCu_bx_Offphi;
  std::vector<int> ar_chamberIDCe_bx_Offphi;
  std::vector<int> ar_chamberIDCu_bx_Offphi;
  std::vector<int> ar_rollCe_bx_Offphi;
  std::vector<int> ar_rollCu_bx_Offphi;
  std::vector<int> ar_emtfSubsectorCu_bx_Offphi;
  std::vector<int> ar_emtfSubsectorCe_bx_Offphi;
  std::vector<int> ar_fillOccupancyCu_bx_Offphi;
  std::vector<int> ar_fillOccupancyCe_bx_Offphi;
  
  TH1D* h1_matches_unpacker;
  TH1D* h1_matches_unpacker_bx;
  TH1D* h1_matches_unpacker_bx_phi;
  TH1D* h1_matches_unpacker_bx_Offphi;
  TH1D*  h1_cluster_sizeCe;
  TH1D*  h1_cluster_sizeCe_oneHit;
  TH1D*  h1_cluster_sizeCe_bx;
  TH1D*  h1_cluster_sizeCe_bx_oneHit;
  TH1D*  h1_cluster_sizeCe_bx_phi;
  TH1D*  h1_cluster_sizeCe_bx_phi_oneHit;
  TH1D*  h1_cluster_sizeCe_bx_Offphi;
  TH1D*  h1_cluster_sizeCe_bx_Offphi_oneHit;
  TH1D*  h1_cluster_sizeCu;
  TH1D*  h1_cluster_sizeCu_oneHit;
  TH1D*  h1_cluster_sizeCu_bx;
  TH1D*  h1_cluster_sizeCu_bx_oneHit;
  TH1D*  h1_cluster_sizeCu_bx_phi;
  TH1D*  h1_cluster_sizeCu_bx_phi_oneHit;
  TH1D*  h1_cluster_sizeCu_bx_Offphi;
  TH1D*  h1_cluster_sizeCu_bx_Offphi_oneHit;
  TH1D*  h1_bxCe;
  TH1D*  h1_bxCe_oneHit;
  TH1D*  h1_bxCe_bx;
  TH1D*  h1_bxCe_bx_oneHit;
  TH1D*  h1_bxCe_bx_phi;
  TH1D*  h1_bxCe_bx_phi_oneHit;
  TH1D*  h1_bxCe_bx_Offphi;
  TH1D*  h1_bxCe_bx_Offphi_oneHit;
  TH1D*  h1_bxCu;
  TH1D*  h1_bxCu_oneHit;
  TH1D*  h1_bxCu_bx;
  TH1D*  h1_bxCu_bx_oneHit;
  TH1D*  h1_bxCu_bx_phi;
  TH1D*  h1_bxCu_bx_phi_oneHit;
  TH1D*  h1_bxCu_bx_Offphi;
  TH1D*  h1_bxCu_bx_Offphi_oneHit;
  TH1D*  h1_phiIntCe;
  TH1D*  h1_phiIntCe_oneHit;
  TH1D*  h1_phiIntCe_bx;
  TH1D*  h1_phiIntCe_bx_oneHit;
  TH1D*  h1_phiIntCe_bx_phi;
  TH1D*  h1_phiIntCe_bx_phi_oneHit;
  TH1D*  h1_phiIntCe_bx_Offphi;
  TH1D*  h1_phiIntCe_bx_Offphi_oneHit;
  TH1D*  h1_phiIntCu;
  TH1D*  h1_phiIntCu_oneHit;
  TH1D*  h1_phiIntCu_bx;
  TH1D*  h1_phiIntCu_bx_oneHit;
  TH1D*  h1_phiIntCu_bx_phi;
  TH1D*  h1_phiIntCu_bx_phi_oneHit;
  TH1D*  h1_phiIntCu_bx_Offphi;
  TH1D*  h1_phiIntCu_bx_Offphi_oneHit;
  TH1D*  h1_thetaIntCu;
  TH1D*  h1_thetaIntCu_oneHit;
  TH1D*  h1_thetaIntCu_bx;
  TH1D*  h1_thetaIntCu_bx_oneHit;
  TH1D*  h1_thetaIntCu_bx_phi;
  TH1D*  h1_thetaIntCu_bx_phi_oneHit;
  TH1D*  h1_thetaIntCu_bx_Offphi;
  TH1D*  h1_thetaIntCu_bx_Offphi_oneHit;
  TH1D*  h1_thetaIntCe;
  TH1D*  h1_thetaIntCe_oneHit;
  TH1D*  h1_thetaIntCe_bx;
  TH1D*  h1_thetaIntCe_bx_oneHit;
  TH1D*  h1_thetaIntCe_bx_phi;
  TH1D*  h1_thetaIntCe_bx_phi_oneHit;
  TH1D*  h1_thetaIntCe_bx_Offphi;
  TH1D*  h1_thetaIntCe_bx_Offphi_oneHit;
  TH1D*  h1_thetaGlobalCe;
  TH1D*  h1_thetaGlobalCe_oneHit;
  TH1D*  h1_thetaGlobalCe_bx;
  TH1D*  h1_thetaGlobalCe_bx_oneHit;
  TH1D*  h1_thetaGlobalCe_bx_phi;
  TH1D*  h1_thetaGlobalCe_bx_phi_oneHit;
  TH1D*  h1_thetaGlobalCe_bx_Offphi;
  TH1D*  h1_thetaGlobalCe_bx_Offphi_oneHit;
  TH1D*  h1_thetaGlobalCu;
  TH1D*  h1_thetaGlobalCu_oneHit;
  TH1D*  h1_thetaGlobalCu_bx;
  TH1D*  h1_thetaGlobalCu_bx_oneHit;
  TH1D*  h1_thetaGlobalCu_bx_phi;
  TH1D*  h1_thetaGlobalCu_bx_phi_oneHit;
  TH1D*  h1_thetaGlobalCu_bx_Offphi;
  TH1D*  h1_thetaGlobalCu_bx_Offphi_oneHit;
  TH1D*  h1_phiGlobalCe;
  TH1D*  h1_phiGlobalCe_oneHit;
  TH1D*  h1_phiGlobalCe_bx;
  TH1D*  h1_phiGlobalCe_bx_oneHit;
  TH1D*  h1_phiGlobalCe_bx_phi;
  TH1D*  h1_phiGlobalCe_bx_phi_oneHit;
  TH1D*  h1_phiGlobalCe_bx_Offphi;
  TH1D*  h1_phiGlobalCe_bx_Offphi_oneHit;
  TH1D*  h1_phiGlobalCu;
  TH1D*  h1_phiGlobalCu_oneHit;
  TH1D*  h1_phiGlobalCu_bx;
  TH1D*  h1_phiGlobalCu_bx_oneHit;
  TH1D*  h1_phiGlobalCu_bx_phi;
  TH1D*  h1_phiGlobalCu_bx_phi_oneHit;
  TH1D*  h1_phiGlobalCu_bx_Offphi;
  TH1D*  h1_phiGlobalCu_bx_Offphi_oneHit;
  TH2D*  h2_bx_emu_unpack;
  TH2D*  h2_bx_emu_unpack_oneHit;
  TH2D*  h2_bx_emu_unpack_bx;
  TH2D*  h2_bx_emu_unpack_bx_oneHit;
  TH2D*  h2_bx_emu_unpack_bx_phi;
  TH2D*  h2_bx_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_bx_emu_unpack_bx_Offphi;
  TH2D*  h2_bx_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_phiInt_emu_unpack;
  TH2D*  h2_phiInt_emu_unpack_oneHit;
  TH2D*  h2_phiInt_emu_unpack_bx;
  TH2D*  h2_phiInt_emu_unpack_bx_oneHit;
  TH2D*  h2_phiInt_emu_unpack_bx_phi;
  TH2D*  h2_phiInt_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_phiInt_emu_unpack_bx_Offphi;
  TH2D*  h2_phiInt_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_phiGlobal_emu_unpack;
  TH2D*  h2_phiGlobal_emu_unpack_oneHit;
  TH2D*  h2_phiGlobal_emu_unpack_bx;
  TH2D*  h2_phiGlobal_emu_unpack_bx_oneHit;
  TH2D*  h2_phiGlobal_emu_unpack_bx_phi;
  TH2D*  h2_phiGlobal_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_phiGlobal_emu_unpack_bx_Offphi;
  TH2D*  h2_phiGlobal_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_thetaInt_emu_unpack;
  TH2D*  h2_thetaInt_emu_unpack_oneHit;
  TH2D*  h2_thetaInt_emu_unpack_bx;
  TH2D*  h2_thetaInt_emu_unpack_bx_oneHit;
  TH2D*  h2_thetaInt_emu_unpack_bx_phi;
  TH2D*  h2_thetaInt_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_thetaInt_emu_unpack_bx_Offphi;
  TH2D*  h2_thetaInt_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_thetaGlobal_emu_unpack;
  TH2D*  h2_thetaGlobal_emu_unpack_oneHit;
  TH2D*  h2_thetaGlobal_emu_unpack_bx;
  TH2D*  h2_thetaGlobal_emu_unpack_bx_oneHit;
  TH2D*  h2_thetaGlobal_emu_unpack_bx_phi;
  TH2D*  h2_thetaGlobal_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_thetaGlobal_emu_unpack_bx_Offphi;
  TH2D*  h2_thetaGlobal_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_chamberID_emu_unpack;
  TH2D*  h2_chamberID_emu_unpack_oneHit;
  TH2D*  h2_chamberID_emu_unpack_bx;
  TH2D*  h2_chamberID_emu_unpack_bx_oneHit;
  TH2D*  h2_chamberID_emu_unpack_bx_phi;
  TH2D*  h2_chamberID_emu_unpack_bx_phi_oneHit;
  TH2D*  h2_chamberID_emu_unpack_bx_Offphi;
  TH2D*  h2_chamberID_emu_unpack_bx_Offphi_oneHit;
  TH2D*  h2_chamberID_vs_roll_emu;
  TH2D*  h2_chamberID_vs_roll_emu_oneHit;
  TH2D*  h2_chamberID_vs_roll_emu_bx;
  TH2D*  h2_chamberID_vs_roll_emu_bx_oneHit;
  TH2D*  h2_chamberID_vs_roll_emu_bx_phi;
  TH2D*  h2_chamberID_vs_roll_emu_bx_phi_oneHit;
  TH2D*  h2_chamberID_vs_roll_emu_bx_Offphi;
  TH2D*  h2_chamberID_vs_roll_emu_bx_Offphi_oneHit;
  TH2D*  h2_chamberID_vs_roll_unpack;
  TH2D*  h2_chamberID_vs_roll_unpack_oneHit;
  TH2D*  h2_chamberID_vs_roll_unpack_bx;
  TH2D*  h2_chamberID_vs_roll_unpack_bx_oneHit;
  TH2D*  h2_chamberID_vs_roll_unpack_bx_phi;
  TH2D*  h2_chamberID_vs_roll_unpack_bx_phi_oneHit;
  TH2D*  h2_chamberID_vs_roll_unpack_bx_Offphi;
  TH2D*  h2_chamberID_vs_roll_unpack_bx_Offphi_oneHit;
  TH2D*  h2_occupancy_unpacker;
  TH2D*  h2_occupancy_unpacker_oneHit;
  TH2D*  h2_occupancy_unpacker_bx;
  TH2D*  h2_occupancy_unpacker_bx_oneHit;
  TH2D*  h2_occupancy_unpacker_bx_phi;
  TH2D*  h2_occupancy_unpacker_bx_phi_oneHit;
  TH2D*  h2_occupancy_unpacker_bx_Offphi;
  TH2D*  h2_occupancy_unpacker_bx_Offphi_oneHit;
  TH2D*  h2_occupancy_emulator;
  TH2D*  h2_occupancy_emulator_oneHit;
  TH2D*  h2_occupancy_emulator_bx;
  TH2D*  h2_occupancy_emulator_bx_oneHit;
  TH2D*  h2_occupancy_emulator_bx_phi;
  TH2D*  h2_occupancy_emulator_bx_phi_oneHit;
  TH2D*  h2_occupancy_emulator_bx_Offphi;
  TH2D*  h2_occupancy_emulator_bx_Offphi_oneHit;
};


#endif

