import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
l1tStage2Cppf = DQMEDAnalyzer(
    "L1TStage2CPPF",
    cppfSource = cms.InputTag("rpcCPPFRawToDigi"),
#    emulSource = cms.InputTag("valCppfStage2Digis","recHit"),#can define different tokens
    monitorDir = cms.untracked.string("L1T/L1TStage2CPPF"), 
    verbose = cms.untracked.bool(False),
)

