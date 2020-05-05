import FWCore.ParameterSet.Config as cms
import subprocess
#import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("DQMPlots")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('DQMPlots')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
                                                    limit = cms.untracked.int32(0)
                                                    )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(-1))

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.GlobalTag.globaltag = "101X_dataRun2_Express_v8"

READFILES = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                            fileNames = READFILES,
                            )
DIRECTORYNAME = 'rootFiles/'
#READFILES.extend( cms.untracked.vstring('file:'+DIRECTORYNAME+'test_cppf_unpacker_emulator.root') )
READFILES.extend( cms.untracked.vstring('file:'+DIRECTORYNAME+'test_cppf_unpacker_emulator-2018-200k-BugFix-2018JSON.root') )
#READFILES.extend( cms.untracked.vstring('file:'+DIRECTORYNAME+'test_cppf_unpacker_emulator-2017-200k.root') )

process.load('L1Trigger.L1TMuonCPPF.cppf_dqm_cfi')
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("DQM_CPPF.root")
                                   )
process.p = cms.Path(process.DQM_CPPF)
