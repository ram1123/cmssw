import FWCore.ParameterSet.Config as cms

EMTF_CPPF_DQM = cms.EDAnalyzer('EMTF_CPPF_DQM',
                      cppfdigiLabel1 = cms.InputTag("emulatorCppfDigis","recHit"),   
		                  cppfdigiLabel2 = cms.InputTag("rpcCPPFRawToDigi"),
		                  cppfdigiLabel3 = cms.InputTag("emtfStage2Digis"),
		                  cppfdigiLabel4 = cms.InputTag("simEmtfDigisData")
)
