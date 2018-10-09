#ifndef DQM_L1TMonitor_L1TdeStage2CPPF_h
#define DQM_L1TMonitor_L1TdeStage2CPPF_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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

  edm::EDGetTokenT<l1t::CPPFDigiCollection> dataToken;
  edm::EDGetTokenT<l1t::CPPFDigiCollection> emulToken;
  std::string monitorDir;
  bool verbose;

  MonitorElement* cppfDataBX;
  MonitorElement* cppfEmulBX;

};

#endif
