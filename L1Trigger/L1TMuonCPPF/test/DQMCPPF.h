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
  
  TH1D* h1_matches_unpacker;
  TH1D* h1_matches_unpacker_bx;
  TH1D* h1_matches_unpacker_bx_phi;

  TH1D* h1_bx_emulated;
  TH1D* h1_bx_unpacker;
  TH1D* h1_bx_diff_emu_unpacker;
  TH1D* h1_phi_diff_emu_unpacker;

  TH2D* h2_chamber_emu_unpacker;
  TH2D* h2_chamber_emu_unpacker_REm43;
  TH2D* h2_chamber_emu_unpacker_REm42;
  TH2D* h2_chamber_emu_unpacker_REm33;
  TH2D* h2_chamber_emu_unpacker_REm32;
  TH2D* h2_chamber_emu_unpacker_REm22;
  TH2D* h2_chamber_emu_unpacker_REm12;
  TH2D* h2_chamber_emu_unpacker_REp12;
  TH2D* h2_chamber_emu_unpacker_REp22;
  TH2D* h2_chamber_emu_unpacker_REp32;
  TH2D* h2_chamber_emu_unpacker_REp33;
  TH2D* h2_chamber_emu_unpacker_REp42;
  TH2D* h2_chamber_emu_unpacker_REp43;

  TH2D* h2_phi_emu_unpacker;
  TH2D* h2_theta_emu_unpacker;
  TH2D* h2_bx_emu_unpacker;

  TH2D* h2_phi_emu_unpacker_bx;
  TH2D* h2_theta_emu_unpacker_bx;
  TH2D* h2_bx_emu_unpacker_bx;

  TH2D* h2_phi_emu_unpacker_bx_phi;
  TH2D* h2_theta_emu_unpacker_bx_phi;
  TH2D* h2_occupancy_unpacker_bx_phi;
  TH2D* h2_occupancy_emu_unpacker_bx_phi;

  TH2D* h2_bx_occupancy_unpacker_bx_phi;
  TH2D* h2_bx_occupancy_emu_unpacker_bx_phi;
  TH2D* h2_bx_sector_unpacker_bx_phi; 
  TH2D* h2_bx_sector_emu_unpacker_bx_phi;

  TH2D* h2_bx;
  TH2D* h2_bx_occupancy;


  // NEW HISTOS

  TH2D*  h2CeVsCuChamberCuChamberCeOneHit;
  TH2D*  h2CeVsCuPhiCePhiCuOneHit;
  TH2D*  h2CeVsCuPhiCePhiCuOneHit_bx;
  TH2D*  h2CeVsCuThetaCeThetaCuOneHit;
  TH2D*  h2CeVsCuChamberCuZoneCuOneHit_bx;
  TH2D*  h2CeVsCuChamberCeZoneCeOneHit_bx;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyOneHit;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOneHit;
  TH2D*  h2CeVsCuBxCeZoneCeOneHit;
  TH1D*  h1CeVsCuPhiCePhiCuDiffOneHit;
  TH1D*  h1CeVsCuThetaCeThetaCuDiffOneHit;
  TH1D*  h1CeVsCuBxCeBxCuDiffOneHit;
  TH1D*  h1CeVsCuPhiInDiagonalOneHit;
  TH1D*  h1CeVsCuPhiOffDiagonalOneHit;
  TH1D*  h1CeVsCuThetaInDiagonalOneHit;
  TH1D*  h1CeVsCuThetaOffDiagonalOneHit;
  TH1D*  h1CeVsCuPhiCePhiCuDiffOneHit_bx;
  TH1D*  h1CeVsCuThetaCeThetaCuDiffOneHit_bx;
  TH2D*  h2CeVsCuThetaCeThetaCuOneHit_bx;
  TH1D*  h1CeVsCuPhiInDiagonalNotOneHit_bx;
  TH1D*  h1CeVsCuPhiInDiagonalOneHit_bx;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalOneHit;
  TH1D*  h1CeVsCuPhiOffDiagonalOneHit_bx;
  TH1D*  h1CeVsCuPhiOffDiagonalNotOneHit_bx;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit;
  TH1D*  h1CeVsCuThetaInDiagonalOneHit_bx;
  TH1D*  h1CeVsCuThetaOffDiagonalOneHit_bx;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit;
  TH1D*  h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx;
  TH2D*  h2CeVsCuChamberCuZoneCuNotOneHit;
  TH2D*  h2CeVsCuChamberCeZoneCeNotOneHit;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalNotOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalNotOneHit;
  TH2D*  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalNotOneHit;
  TH2D*  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalNotOneHit;

};


#endif


