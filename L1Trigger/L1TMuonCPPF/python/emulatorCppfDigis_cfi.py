import FWCore.ParameterSet.Config as cms

emulatorCppfDigis = cms.EDProducer("L1TMuonCPPFDigiProducer",
                                   
                                   ## Input collection
                                   recHitLabel = cms.InputTag("rpcRecHits"),
				   MaxClusterSize = cms.int32(3),
 				   BxCut = cms.vint32(-3,-2,-1,0,1,2,3),
 				   #BxCut = cms.vint32(0),
                                   #  cppfSource = cms.string('Geo'), #'File' for Look up table and 'Geo' for CMSSW Geometry 
                                   cppfSource = cms.string('File'), #'File' for Look up table and 'Geo' for CMSSW Geometry 
                                   
                                   cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFmerged.txt')                     
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFn1.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFn2.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFn3.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFn4.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFp1.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFp2.txt')                    
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFp3.txt')                   
                                   #    cppfvecfile = cms.FileInPath('L1Trigger/L1TMuon/data/cppf/angleScale_RPC_CPPFp4.txt')                    
                                   )

