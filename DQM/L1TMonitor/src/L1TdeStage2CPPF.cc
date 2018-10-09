#include <string>

#include "DQM/L1TMonitor/interface/L1TdeStage2CPPF.h"


L1TdeStage2CPPF::L1TdeStage2CPPF(const edm::ParameterSet& ps)
    : dataToken(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emulToken(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TdeStage2CPPF::~L1TdeStage2CPPF() {}

void L1TdeStage2CPPF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}


void L1TdeStage2CPPF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(monitorDir);

  cppfDataBX = ibooker.book1D("cppfDataBX", "CPPF data BX", 7, -3, 4);
  cppfDataBX->setAxisTitle("BX", 1);

  cppfEmulBX = ibooker.book1D("cppfEmulBX", "CPPF emulator BX", 7, -3, 4);
  cppfEmulBX->setAxisTitle("BX", 1);

  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    cppfDataBX->setBinLabel(bin, std::to_string(bin_label), 1);
    cppfEmulBX->setBinLabel(bin, std::to_string(bin_label), 1);
  }

}

void L1TdeStage2CPPF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TdeStage2CPPF") << "L1TdeStage2CPPF: analyze..." << std::endl;

  edm::Handle<l1t::CPPFDigiCollection> dataCPPFs;
  e.getByToken(dataToken, dataCPPFs);

  edm::Handle<l1t::CPPFDigiCollection> emulCPPFs;
  e.getByToken(emulToken, emulCPPFs);


  for(auto& cppf_digi : *dataCPPFs){
	  cppfDataBX->Fill(cppf_digi.bx());
  }
  for(auto& cppf_digi : *emulCPPFs){
	  cppfEmulBX->Fill(cppf_digi.bx());
  }
}

