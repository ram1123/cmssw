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
  
  std::vector<int> EMTFsector0bins;
  std::vector<int> EMTFsector1bins;
  std::vector<int> EMTFsector2bins;
  std::vector<int> EMTFsector3bins;
  std::vector<int> EMTFsector4bins;
  std::vector<int> EMTFsector5bins;
  std::vector<int> EMTFsector6bins;
  
  std::map<int , std::vector<int>> fill_info;
  
  // TH1D* h1_total_hits_unpacker;
  // TH1D* h1_total_hits_unpacker_bx;
  // TH1D* h1_total_hits_unpacker_bx_phi;
  
  // TH1D* h1_total_hits_emulator;
  // TH1D* h1_total_hits_emulator_bx;
  // TH1D* h1_total_hits_emulator_bx_phi;
  
  // TH1D* h1_bx_emulated;
  // TH1D* h1_bx_unpacker;
  // TH1D* h1_bx_diff_emu_unpacker;
  // TH1D* h1_phi_diff_emu_unpacker;
  
  // TH2D* h2_chamber_emu_unpacker;
  // TH2D* h2_chamber_emu_unpacker_REm43;
  // TH2D* h2_chamber_emu_unpacker_REm42;
  // TH2D* h2_chamber_emu_unpacker_REm33;
  // TH2D* h2_chamber_emu_unpacker_REm32;
  // TH2D* h2_chamber_emu_unpacker_REm22;
  // TH2D* h2_chamber_emu_unpacker_REm12;
  // TH2D* h2_chamber_emu_unpacker_REp12;
  // TH2D* h2_chamber_emu_unpacker_REp22;
  // TH2D* h2_chamber_emu_unpacker_REp32;
  // TH2D* h2_chamber_emu_unpacker_REp33;
  // TH2D* h2_chamber_emu_unpacker_REp42;
  // TH2D* h2_chamber_emu_unpacker_REp43;
  
  // TH2D* h2_phi_emu_unpacker;
  // TH2D* h2_theta_emu_unpacker;
  // TH2D* h2_bx_emu_unpacker;
  
  // TH2D* h2_phi_emu_unpacker_bx;
  // TH2D* h2_theta_emu_unpacker_bx;
  // TH2D* h2_bx_emu_unpacker_bx;
  
  // TH2D* h2_phi_emu_unpacker_bx_phi;
  // TH2D* h2_theta_emu_unpacker_bx_phi;
  // TH2D* h2_occupancy_unpacker_bx_phi;
  // TH2D* h2_occupancy_emu_unpacker_bx_phi;
  
  // //TH2D* h2_bx_occupancy_unpacker_bx_phi;
  // //TH2D* h2_bx_occupancy_emu_unpacker_bx_phi;
  // //TH2D* h2_bx_sector_unpacker_bx_phi; 
  // //TH2D* h2_bx_sector_emu_unpacker_bx_phi;
  
  // TH2D* h2_bx;
  // TH2D* h2_bx_occupancy;
  
  
  // // NEW HISTOS
  // TH1D*  h1CeVsCuPhiCePhiCuDiff_OneHit;
  // TH1D*  h1CeVsCuPhiCePhiCuDiff_OneHit_bx;
  // TH1D*  h1CeVsCuThetaCeThetaCuDiff_OneHit;
  // TH1D*  h1CeVsCuThetaCeThetaCuDiff_OneHit_bx;
  // TH1D*  h1CeVsCuBxCeBxCuDiff_OneHit;
  // TH1D*  h1CeVsCuPhi_InDiagonal_OneHit;
  // TH1D*  h1CeVsCuPhi_InDiagonal_OneHit_bx;
  // TH1D*  h1CeVsCuPhi_OffDiagonal_OneHit;
  // TH1D*  h1CeVsCuPhi_OffDiagonal_OneHit_bx;
  // TH1D*  h1CeVsCuTheta_InDiagonal_OneHit;
  // TH1D*  h1CeVsCuTheta_InDiagonal_OneHit_bx;
  // TH1D*  h1CeVsCuTheta_OffDiagonal_OneHit;
  // TH1D*  h1CeVsCuTheta_OffDiagonal_OneHit_bx;
  // TH1D*  h1CeVsCuPhi_InDiagonal_NotOneHit_bx;
  // TH1D*  h1CeVsCuPhi_OffDiagonal_NotOneHit_bx;
  // TH1D*  h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx;
  
  // TH2D*  h2CeVsCuChamberCuChamberCe_OneHit;
  // TH2D*  h2CeVsCuPhiCePhiCu_OneHit;
  // TH2D*  h2CeVsCuPhiCePhiCu_OneHit_bx;
  // TH2D*  h2CeVsCuChamberCuZoneCu_OneHit_bx;
  // TH2D*  h2CeVsCuChamberCeZoneCe_OneHit_bx;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancy_OneHit;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancy_OneHit;
  // TH2D*  h2CeVsCuBxCuZoneCu_OneHit;
  // TH2D*  h2CeVsCuBxCeZoneCe_OneHit;
  // TH2D*  h2CeVsCuThetaCeThetaCu_OneHit;
  // TH2D*  h2CeVsCuThetaCeThetaCu_OneHit_bx;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx;
  // TH2D*  h2CeVsCuChamberCuZoneCu_NotOneHit;
  // TH2D*  h2CeVsCuChamberCeZoneCe_NotOneHit;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit;
  // TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit;
  // TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit;
  
  // //std::map<int, TH1F*> h1_nHits_each_chamber_unpacker;
  // //
  
  // TH1F* h1_nHits_each_chamber_unpacker[2][5][4][7][7];
  // TH1F* h1_nHits_each_chamber_emulator[2][5][4][7][7];
  
};


#endif

