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


class EMTF_CPPF_DQM : public edm::EDAnalyzer {
  
 public:
  explicit EMTF_CPPF_DQM(const edm::ParameterSet&);
  ~EMTF_CPPF_DQM() override;
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
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken4_;


  
  TH1D* h1CeVsCuMatches;
  TH1D* h1CeVsCuOffDiagonal;
  TH1D* h1CeVsCuInDiagonal;

  TH1D* h1CeVsCuMatches_ch;
  TH2D* h2CeVsCuChamberCeChamberCu;
  TH2D* h2CeVsCuPhiCePhiCu;
  TH1D* h1CeVsCuPhiCePhiCuDiff;
  TH2D* h2CeVsCuThetaCeThetaCu;
  TH2D* h2CeVsCuBxCeBxCu;
  TH1D* h1CeVsCuBxCeBxCuDiff;

  TH1D* h1CeVsCuMatches_bx;
  TH2D* h2CeVsCuPhiCePhiCu_bx;
  TH2D* h2CeVsCuThetaCeThetaCu_bx;
  TH2D* h2CeVsCuBxCeBxCu_bx;

  TH1D* h1CeVsCuMatches_int;
  TH2D* h2CeVsCuPhiCePhiCu_int;
  TH2D* h2CeVsCuThetaCeThetaCu_int;
  TH2D* h2CeVsCuChamberCuZoneCu;
  TH2D* h2CeVsCuChamberCeZoneCe;
  TH2D* h2CeVsCuBxCuZoneCuOccupancy;
  TH2D* h2CeVsCuBxCeZoneCeOccupancy;
  TH2D* h2CeVsCuBxCuZoneCu;
  TH2D* h2CeVsCuBxCeZoneCe;


  TH1D* h1CeVsEuMatches;

  TH1D* h1CeVsEuMatches_ch;
  TH2D* h2CeVsEuChamberCeChamberEu_test;
  TH2D* h2CeVsEuChamberCeChamberEu;
  TH2D* h2CeVsEuPhiCePhiEu;
  TH1D* h1CeVsEuPhiCePhiEuDiff;
  TH2D* h2CeVsEuThetaCeThetaEu;
  TH2D* h2CeVsEuBxCeBxEu;
  TH1D* h1CeVsEuBxCeBxEuDiff;

  TH1D* h1CeVsEuMatches_bx;
  TH2D* h2CeVsEuPhiCePhiEu_bx;
  TH2D* h2CeVsEuThetaCeThetaEu_bx;
  TH2D* h2CeVsEuBxCeBxEu_bx;

  TH1D* h1CeVsEuMatches_int;
  TH2D* h2CeVsEuPhiCePhiEu_int;
  TH2D* h2CeVsEuThetaCeThetaEu_int;
  TH2D* h2CeVsEuChamberEuZoneEu;
  TH2D* h2CeVsEuChamberCeZoneCe;


  TH2D* h2CeVsEuBxEuZoneEuOccupancy;
  TH2D* h2CeVsEuBxCeZoneCeOccupancy;
  TH2D* h2CeVsEuBxEuZoneEu;
  TH2D* h2CeVsEuBxCeZoneCe;

  TH1D* h1CeVsEeMatches;

  TH1D* h1CeVsEeMatches_ch;
  TH2D* h2CeVsEeChamberCeChamberEe_test;
  TH2D* h2CeVsEeChamberCeChamberEe;
  TH2D* h2CeVsEePhiCePhiEe;
  TH1D* h1CeVsEePhiCePhiEeDiff;
  TH2D* h2CeVsEeThetaCeThetaEe;
  TH2D* h2CeVsEeBxCeBxEe;
  TH1D* h1CeVsEeBxCeBxEeDiff;

  TH1D* h1CeVsEeMatches_bx;
  TH2D* h2CeVsEePhiCePhiEe_bx;
  TH2D* h2CeVsEeThetaCeThetaEe_bx;
  TH2D* h2CeVsEeBxCeBxEe_bx;

  TH1D* h1CeVsEeMatches_int;
  TH2D* h2CeVsEePhiCePhiEe_int;
  TH2D* h2CeVsEeThetaCeThetaEe_int;
  TH2D* h2CeVsEeChamberEeZoneEe;
  TH2D* h2CeVsEeChamberCeZoneCe;


  TH2D* h2CeVsEeBxEeZoneEeOccupancy;
  TH2D* h2CeVsEeBxCeZoneCeOccupancy;
  TH2D* h2CeVsEeBxEeZoneEe;
  TH2D* h2CeVsEeBxCeZoneCe;


  TH2D* h2CuVsEuChamberCuChamberEu_test;
  TH2D* h2CuVsEuChamberCuChamberEu;
  TH2D* h2CuVsEuChamberCuChamberEuBox;
  TH2D* h2CuVsEuPhiCuPhiEu;
  TH1D* h1CuVsEuPhiCuPhiEuDiff;
  TH2D* h2CuVsEuThetaCuThetaEu;
  TH2D* h2CuVsEuBxCuBxEu;
  TH1D* h1CuVsEuBxCuBxEuDiff;

  TH2D* h2CuVsEuPhiCuPhiEu_bx;
  TH2D* h2CuVsEuThetaCuThetaEu_bx;
  TH2D* h2CuVsEuBxCuBxEu_bx;

  TH2D* h2CuVsEuPhiCuPhiEu_int;
  TH2D* h2CuVsEuThetaCuThetaEu_int;
  TH2D* h2CuVsEuChamberCuZoneCu;
  TH2D* h2CuVsEuChamberCuZoneCuBox;
  TH2D* h2CuVsEuChamberEuZoneEu;
  TH2D* h2CuVsEuChamberEuZoneEuBox;
  TH2D* h2CuVsEuBxCuZoneCuOccupancy;
  TH2D* h2CuVsEuBxEuZoneEuOccupancy;
  TH2D* h2CuVsEuBxCuZoneCu;
  TH2D* h2CuVsEuBxEuZoneEu;


  TH2D* h2EeVsEuChamberEeChamberEu_test;
  TH2D* h2EeVsEuChamberEeChamberEu;
  TH2D* h2EeVsEuPhiEePhiEu;
  TH1D* h1EeVsEuPhiEePhiEuDiff;
  TH2D* h2EeVsEuThetaEeThetaEu;
  TH2D* h2EeVsEuBxEeBxEu;
  TH1D* h1EeVsEuBxEeBxEuDiff;

  TH2D* h2EeVsEuPhiEePhiEu_bx;
  TH2D* h2EeVsEuThetaEeThetaEu_bx;
  TH2D* h2EeVsEuBxEeBxEu_bx;

  TH2D* h2EeVsEuPhiEePhiEu_int;
  TH2D* h2EeVsEuThetaEeThetaEu_int;
  TH2D* h2EeVsEuChamberEeZoneEe;
  TH2D* h2EeVsEuChamberEuZoneEu;
  TH2D* h2EeVsEuBxEeZoneEeOccupancy;
  TH2D* h2EeVsEuBxEuZoneEuOccupancy;
  TH2D* h2EeVsEuBxEeZoneEe;
  TH2D* h2EeVsEuBxEuZoneEu;
  
 //Only one hit match per chamber histograms. 
  
  TH1D* h1CeVsCuPhiOffDiagonalOneHit;
  TH1D* h1CeVsCuPhiInDiagonalOneHit;
  TH1D* h1CeVsCuThetaOffDiagonalOneHit;
  TH1D* h1CeVsCuThetaInDiagonalOneHit;
  TH1D* h1CeVsCuPhiOffDiagonalOneHit_bx;
  TH1D* h1CeVsCuPhiInDiagonalOneHit_bx;
  TH1D* h1CeVsCuThetaOffDiagonalOneHit_bx;
  TH1D* h1CeVsCuThetaInDiagonalOneHit_bx;
  TH2D* h2CeVsCuChamberCeChamberCuOneHit;
  TH2D* h2CeVsCuPhiCePhiCuOneHit;
  TH1D* h1CeVsCuPhiCePhiCuDiffOneHit;
  TH1D* h1CeVsCuThetaCeThetaCuDiffOneHit;
  TH1D* h1CeVsCuPhiCePhiCuDiffOneHit_bx;
  TH1D* h1CeVsCuThetaCeThetaCuDiffOneHit_bx;
  TH1D* h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx;
  TH2D* h2CeVsCuThetaCeThetaCuOneHit;
  TH2D* h2CeVsCuBxCeBxCuOneHit;
  TH1D* h1CeVsCuBxCeBxCuDiffOneHit;
  TH2D* h2CeVsCuPhiCePhiCuOneHit_bx;
  TH2D* h2CeVsCuThetaCeThetaCuOneHit_bx;
  TH2D* h2CeVsCuBxCeBxCuOneHit_bx;
  TH2D* h2CeVsCuPhiCePhiCuOneHit_int;
  TH2D* h2CeVsCuThetaCeThetaCuOneHit_int;
  TH2D* h2CeVsCuChamberCuZoneCuOneHit;
  TH2D* h2CeVsCuChamberCeZoneCeOneHit;
  TH2D* h2CeVsCuBxCuZoneCuOccupancyOneHit;
  TH2D* h2CeVsCuBxCeZoneCeOccupancyOneHit;
  TH2D* h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsCuBxCuZoneCuOneHit;
  TH2D* h2CeVsCuBxCeZoneCeOneHit;

  TH1D* h1CeVsEuPhiOffDiagonalOneHit;
  TH1D* h1CeVsEuPhiInDiagonalOneHit;
  TH1D* h1CeVsEuThetaOffDiagonalOneHit;
  TH1D* h1CeVsEuThetaInDiagonalOneHit;
  TH1D* h1CeVsEuPhiOffDiagonalOneHit_bx;
  TH1D* h1CeVsEuPhiInDiagonalOneHit_bx;
  TH1D* h1CeVsEuThetaOffDiagonalOneHit_bx;
  TH1D* h1CeVsEuThetaInDiagonalOneHit_bx;
  TH2D* h2CeVsEuChamberCeChamberEuOneHit;
  TH2D* h2CeVsEuPhiCePhiEuOneHit;
  TH1D* h1CeVsEuPhiCePhiEuDiffOneHit;
  TH1D* h1CeVsEuThetaCeThetaEuDiffOneHit;
  TH1D* h1CeVsEuPhiCePhiEuDiffOneHit_bx;
  TH1D* h1CeVsEuThetaCeThetaEuDiffOneHit_bx;
  TH1D* h1CeVsEuThetaPhiCeThetaPhiEuDiffOneHit_bx;
  TH2D* h2CeVsEuThetaCeThetaEuOneHit;
  TH2D* h2CeVsEuBxCeBxEuOneHit;
  TH1D* h1CeVsEuBxCeBxEuDiffOneHit;
  TH2D* h2CeVsEuPhiCePhiEuOneHit_bx;
  TH2D* h2CeVsEuThetaCeThetaEuOneHit_bx;
  TH2D* h2CeVsEuBxCeBxEuOneHit_bx;
  TH2D* h2CeVsEuPhiCePhiEuOneHit_int;
  TH2D* h2CeVsEuThetaCeThetaEuOneHit_int;
  TH2D* h2CeVsEuChamberEuZoneEuOneHit;
  TH2D* h2CeVsEuChamberCeZoneCeOneHit;
  TH2D* h2CeVsEuBxEuZoneEuOccupancyOneHit;
  TH2D* h2CeVsEuBxCeZoneCeOccupancyOneHit;
  TH2D* h2CeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsEuBxCeZoneCeOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsEuBxCeZoneCeOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsEuBxEuZoneEuOneHit;
  TH2D* h2CeVsEuBxCeZoneCeOneHit;

  TH1D* h1CeVsEePhiOffDiagonalOneHit;
  TH1D* h1CeVsEePhiInDiagonalOneHit;
  TH1D* h1CeVsEeThetaOffDiagonalOneHit;
  TH1D* h1CeVsEeThetaInDiagonalOneHit;
  TH1D* h1CeVsEePhiOffDiagonalOneHit_bx;
  TH1D* h1CeVsEePhiInDiagonalOneHit_bx;
  TH1D* h1CeVsEeThetaOffDiagonalOneHit_bx;
  TH1D* h1CeVsEeThetaInDiagonalOneHit_bx;
  TH2D* h2CeVsEeChamberCeChamberEeOneHit;
  TH2D* h2CeVsEePhiCePhiEeOneHit;
  TH1D* h1CeVsEePhiCePhiEeDiffOneHit;
  TH1D* h1CeVsEeThetaCeThetaEeDiffOneHit;
  TH1D* h1CeVsEePhiCePhiEeDiffOneHit_bx;
  TH1D* h1CeVsEeThetaCeThetaEeDiffOneHit_bx;
  TH1D* h1CeVsEeThetaPhiCeThetaPhiEeDiffOneHit_bx;
  TH2D* h2CeVsEeThetaCeThetaEeOneHit;
  TH2D* h2CeVsEeBxCeBxEeOneHit;
  TH1D* h1CeVsEeBxCeBxEeDiffOneHit;
  TH2D* h2CeVsEePhiCePhiEeOneHit_bx;
  TH2D* h2CeVsEeThetaCeThetaEeOneHit_bx;
  TH2D* h2CeVsEeBxCeBxEeOneHit_bx;
  TH2D* h2CeVsEePhiCePhiEeOneHit_int;
  TH2D* h2CeVsEeThetaCeThetaEeOneHit_int;
  TH2D* h2CeVsEeChamberEeZoneEeOneHit;
  TH2D* h2CeVsEeChamberCeZoneCeOneHit;
  TH2D* h2CeVsEeBxEeZoneEeOccupancyOneHit;
  TH2D* h2CeVsEeBxCeZoneCeOccupancyOneHit;
  TH2D* h2CeVsEeBxEeZoneEeOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsEeBxCeZoneCeOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CeVsEeBxEeZoneEeOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsEeBxCeZoneCeOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CeVsEeBxEeZoneEeOneHit;
  TH2D* h2CeVsEeBxCeZoneCeOneHit;

  TH1D* h1CuVsEuPhiOffDiagonalOneHit;
  TH1D* h1CuVsEuPhiInDiagonalOneHit;
  TH1D* h1CuVsEuThetaOffDiagonalOneHit;
  TH1D* h1CuVsEuThetaInDiagonalOneHit;
  TH1D* h1CuVsEuPhiOffDiagonalOneHit_bx;
  TH1D* h1CuVsEuPhiInDiagonalOneHit_bx;
  TH1D* h1CuVsEuThetaOffDiagonalOneHit_bx;
  TH1D* h1CuVsEuThetaInDiagonalOneHit_bx;
  TH2D* h2CuVsEuChamberCuChamberEuOneHit;
  TH2D* h2CuVsEuChamberCuChamberEuBoxOneHit;
  TH2D* h2CuVsEuPhiCuPhiEuOneHit;
  TH1D* h1CuVsEuPhiCuPhiEuDiffOneHit;
  TH1D* h1CuVsEuThetaCuThetaEuDiffOneHit;
  TH1D* h1CuVsEuPhiCuPhiEuDiffOneHit_bx;
  TH1D* h1CuVsEuThetaCuThetaEuDiffOneHit_bx;
  TH1D* h1CuVsEuThetaPhiCuThetaPhiEuDiffOneHit_bx;
  TH2D* h2CuVsEuThetaCuThetaEuOneHit;
  TH2D* h2CuVsEuBxCuBxEuOneHit;
  TH1D* h1CuVsEuBxCuBxEuDiffOneHit;
  TH2D* h2CuVsEuPhiCuPhiEuOneHit_bx;
  TH2D* h2CuVsEuThetaCuThetaEuOneHit_bx;
  TH2D* h2CuVsEuBxCuBxEuOneHit_bx;
  TH2D* h2CuVsEuPhiCuPhiEuOneHit_int;
  TH2D* h2CuVsEuThetaCuThetaEuOneHit_int;
  TH2D* h2CuVsEuChamberCuZoneCuOneHit;
  TH2D* h2CuVsEuChamberCuZoneCuBoxOneHit;
  TH2D* h2CuVsEuChamberEuZoneEuOneHit;
  TH2D* h2CuVsEuChamberEuZoneEuBoxOneHit;
  TH2D* h2CuVsEuBxCuZoneCuOccupancyOneHit;
  TH2D* h2CuVsEuBxEuZoneEuOccupancyOneHit;
  TH2D* h2CuVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CuVsEuBxCuZoneCuOccupancyOffPhiDiagonalOneHit;
  TH2D* h2CuVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CuVsEuBxCuZoneCuOccupancyOffThetaDiagonalOneHit;
  TH2D* h2CuVsEuBxCuZoneCuOneHit;
  TH2D* h2CuVsEuBxEuZoneEuOneHit;


  TH1D* h1EeVsEuPhiOffDiagonalOneHit;
  TH1D* h1EeVsEuPhiInDiagonalOneHit;
  TH1D* h1EeVsEuThetaOffDiagonalOneHit;
  TH1D* h1EeVsEuThetaInDiagonalOneHit;
  TH1D* h1EeVsEuPhiOffDiagonalOneHit_bx;
  TH1D* h1EeVsEuPhiInDiagonalOneHit_bx;
  TH1D* h1EeVsEuThetaOffDiagonalOneHit_bx;
  TH1D* h1EeVsEuThetaInDiagonalOneHit_bx;
  TH2D* h2EeVsEuChamberEeChamberEuOneHit;
  TH2D* h2EeVsEuPhiEePhiEuOneHit;
  TH1D* h1EeVsEuPhiEePhiEuDiffOneHit;
  TH1D* h1EeVsEuThetaEeThetaEuDiffOneHit;
  TH1D* h1EeVsEuPhiEePhiEuDiffOneHit_bx;
  TH1D* h1EeVsEuThetaEeThetaEuDiffOneHit_bx;
  TH1D* h1EeVsEuThetaPhiEeThetaPhiEuDiffOneHit_bx;
  TH2D* h2EeVsEuThetaEeThetaEuOneHit;
  TH2D* h2EeVsEuBxEeBxEuOneHit;
  TH1D* h1EeVsEuBxEeBxEuDiffOneHit;
  TH2D* h2EeVsEuPhiEePhiEuOneHit_bx;
  TH2D* h2EeVsEuThetaEeThetaEuOneHit_bx;
  TH2D* h2EeVsEuBxEeBxEuOneHit_bx;
  TH2D* h2EeVsEuPhiEePhiEuOneHit_int;
  TH2D* h2EeVsEuThetaEeThetaEuOneHit_int;
  TH2D* h2EeVsEuChamberEeZoneEeOneHit;
  TH2D* h2EeVsEuChamberEuZoneEuOneHit;
  TH2D* h2EeVsEuBxEeZoneEeOccupancyOneHit;
  TH2D* h2EeVsEuBxEuZoneEuOccupancyOneHit;
  TH2D* h2EeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit;
  TH2D* h2EeVsEuBxEeZoneEeOccupancyOffPhiDiagonalOneHit;
  TH2D* h2EeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit;
  TH2D* h2EeVsEuBxEeZoneEeOccupancyOffThetaDiagonalOneHit;
  TH2D* h2EeVsEuBxEeZoneEeOneHit;
  TH2D* h2EeVsEuBxEuZoneEuOneHit;
};


#endif


