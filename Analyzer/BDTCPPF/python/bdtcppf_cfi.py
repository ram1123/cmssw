import FWCore.ParameterSet.Config as cms

BDTCPPF = cms.EDAnalyzer('BDTCPPF',
		      genParticlesLabel = cms.InputTag("genParticles"),
		      recHitLabel = cms.InputTag("rpcRecHits"),
                      cppfdigiLabel = cms.InputTag("emulatorCppfDigis","recHit")                  
)
