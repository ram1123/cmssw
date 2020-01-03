#ifndef DQM_L1TMonitor_L1TStage2CPPF_h
#define DQM_L1TMonitor_L1TStage2CPPF_h

/*
 * \file L1TStage2CPPF.h
 * \Author
 * \September 2018 
*/

// system requirements
#include <iosfwd>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// general requirements
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

// stage2 requirements
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

// dqm requirements
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/L1TMuon/interface/MuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/RegionalMuonRawDigiTranslator.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

//from  /afs/cern.ch/user/c/chengl/tanggy/CMSSW_10_1_8/src/L1Trigger/L1TMuonCPPF/test/DQMCPPF.h
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

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

// class decleration

class  L1TStage2CPPF: public DQMEDAnalyzer {

public:

// class constructor
  L1TStage2CPPF(const edm::ParameterSet & ps);
// class destructor
  virtual ~L1TStage2CPPF();

// member functions
  edm::ESHandle <RPCGeometry> rpcGeom;

protected:
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) override;
  virtual void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  virtual void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override ;

// data members
private:  

  std::string monitorDir;
//  edm::InputTag cppfdigiLabel;
  //  edm::InputTag cppfSourceTwinMux1;
  //  edm::InputTag cppfSourceTwinMux2;
  bool verbose ;
//  edm::EDGetToken cppfToken;
  // edm::EDGetToken cppfTokenTwinMux1;
  //  edm::EDGetToken cppfTokenTwinMux2; 
  float global_phi;

//  MonitorElement* cppf_hwEta; 
//  MonitorElement* cppf_hwLocalPhi;
//  MonitorElement* cppf_hwGlobalPhi;
//  MonitorElement* cppf_hwPt;  
//  MonitorElement* cppf_hwQual;
//  MonitorElement* cppf_proc; 
//
//  MonitorElement* cppf_wedge_bx;
//  MonitorElement* cppf_hwEta_hwLocalPhi;
//  MonitorElement* cppf_hwEta_hwGlobalPhi;
//
//  MonitorElement* cppf_hwPt_hwEta;
//  MonitorElement* cppf_hwPt_hwLocalPhi;
//
//  MonitorElement* cppf_hwEta_bx;  
//  MonitorElement* cppf_hwLocalPhi_bx;  
//  MonitorElement* cppf_hwPt_bx;   
//  MonitorElement* cppf_hwQual_bx; 

  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken_;
  int EMTF_sector;
  int EMTF_subsector;
  int EMTF_bx;

  std::vector<int> EMTFsector1bins;
  std::vector<int> EMTFsector2bins;
  std::vector<int> EMTFsector3bins;
  std::vector<int> EMTFsector4bins;
  std::vector<int> EMTFsector5bins;
  std::vector<int> EMTFsector6bins;

  std::map<int , std::vector<int>> fill_info; 

//  MonitorElement* Phi_Integer;
//  MonitorElement* Theta_Integer;
//  MonitorElement* Phi_Global;
//  MonitorElement* Theta_Global;
//  MonitorElement* Phi_Global_Integer;
//  MonitorElement* Theta_Global_Integer;
  MonitorElement* Occupancy_EMTFSector;
  MonitorElement* Track_Bx;
};

#endif
