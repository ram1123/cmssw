#ifndef DQM_L1TMonitor_L1TdeStage2CPPF_h
#define DQM_L1TMonitor_L1TdeStage2CPPF_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/L1TMuon/interface/CPPFDigi.h"


class L1TdeStage2CPPF : public DQMEDAnalyzer {

 public:

  L1TdeStage2CPPF(const edm::ParameterSet& ps);
  ~L1TdeStage2CPPF() override;

 protected:

  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

 private:
  int occupancy_value(int region_, int station_, int ring_);
  int bx_value(int region_, int emtfsector_);

  edm::ESHandle <RPCGeometry> rpcGeom;

  edm::EDGetTokenT<l1t::CPPFDigiCollection> dataToken;
  edm::EDGetTokenT<l1t::CPPFDigiCollection> emulToken;
  std::string monitorDir;
	bool verbose;
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

//  MonitorElement* cppfDataBX;
//  MonitorElement* cppfEmulBX;

// from Gabriel's code
  MonitorElement* Matches_unpacker;

  MonitorElement* Matches_unpacker_ch;
  MonitorElement* chamber_emu_unpacker;
  MonitorElement* phi_emu_unpacker;
  MonitorElement* theta_emu_unpacker;
  MonitorElement* bx_emu_unpacker;

  MonitorElement* Matches_unpacker_bx;
  MonitorElement* phi_emu_unpacker_bx;
  MonitorElement* theta_emu_unpacker_bx;
  MonitorElement* bx_emu_unpacker_bx;

  MonitorElement* Matches_unpacker_int;
  MonitorElement* phi_emu_unpacker_int;
  MonitorElement* theta_emu_unpacker_int;
  MonitorElement* occupancy_unpacker;
  MonitorElement* occupancy_emu_unpacker;

  MonitorElement* bx_occupancy_unpacker;
  MonitorElement* bx_occupancy_emu_unpacker;
  MonitorElement* bx_sector_unpacker; 
  MonitorElement* bx_sector_emu_unpacker;

	// from Cheng libo
	MonitorElement* theta_shift[6];
	MonitorElement* phi_shift[6];
	MonitorElement* theta_phi_shift[6];
  MonitorElement* not_agree[6];
	MonitorElement* well_agree[6];
	MonitorElement* almost_agree[6];

};

#endif
