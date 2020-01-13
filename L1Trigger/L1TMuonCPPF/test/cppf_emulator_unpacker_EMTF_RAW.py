
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import subprocess
import sys, os

from FWCore.PythonUtilities.LumiList import LumiList
from FWCore.ParameterSet.VarParsing import VarParsing
mylist = FileUtils.loadListFromFile('/afs/cern.ch/user/r/rasharma/work/CPPF/garrasmelos/CMSSW_10_1_11/src/L1Trigger/L1TMuonCPPF/test/inputFiles_2018.txt');
options = VarParsing("analysis")
options.register("runList"
                 , []
                 , VarParsing.multiplicity.list
                 , VarParsing.varType.int
                 , "Run selection")
options.register("lumiList"
                 , "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/DCSOnly/json_DCSONLY.txt"
                 #, "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/DCSOnly/json_DCSONLY.txt"
                 , VarParsing.multiplicity.singleton
                 , VarParsing.varType.string
                 , "JSON file")
options.parseArguments()

lumilist = LumiList(filename = options.lumiList)
if len(options.runList) :
    runlist = LumiList(runs = options.runList)
    lumilist = lumilist & runlist
    if not len(lumilist) :
        raise RuntimeError, "The resulting LumiList is empty"

process = cms.Process("testRPCCPPF")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2016_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "101X_dataRun2_Express_v7"

process.load("EventFilter.RPCRawToDigi.RPCCPPFRawToDigi_sqlite_cff")
process.load("EventFilter.RPCRawToDigi.rpcPacker_cfi")
process.rpcpacker.InputLabel = cms.InputTag("rpcCPPFRawToDigi")
process.load("EventFilter.RPCRawToDigi.rpcUnpackingModule_cfi")
process.rpcUnpackingModulePacked = process.rpcUnpackingModule.clone()
process.rpcUnpackingModulePacked.InputLabel = cms.InputTag("rpcpacker")

process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
muonRPCDigis.InputLabel = 'rawDataCollector'

process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
process.muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
process.muonRPCDigis.InputLabel = 'rawDataCollector'

process.load('RecoLocalMuon.RPCRecHit.rpcRecHits_cfi')
from RecoLocalMuon.RPCRecHit.rpcRecHits_cfi import *
process.rpcRecHits.rpcDigiLabel = 'rpcunpacker'

process.load('L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi')
from L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi import *
process.emulatorCppfDigis.recHitLabel = 'rpcRecHits'

process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
process.load('L1Trigger.L1TMuonEndCap.simEmtfDigis_cfi')
process.simEmtfDigisData.RPCInput  = cms.InputTag('rpcunpacker')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring(*mylist)
            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(10))

process.p = cms.Path( process.rpcUnpackingModule 
			* process.rpcCPPFRawToDigi * process.rpcpacker * process.rpcUnpackingModulePacked + 
			 
			process.rpcunpacker * process.rpcRecHits * process.emulatorCppfDigis +
			process.emtfStage2Digis * process.simEmtfDigisData  
)

# Output
process.out = cms.OutputModule("PoolOutputModule"
                               , outputCommands = cms.untracked.vstring("drop *",
									                                                      "keep *_rpcunpacker_*_*",
                                                                        "keep *_emulatorCppfDigis_*_*",
                                                                        "keep *_rpcRecHits_*_*",
                                                                        "keep *_rpcCPPFRawToDigi_*_*",
									                                                      "keep *_emtfStage2Digis_*_*",
									                                                      "keep *_simEmtfDigisData_*_*")
                               #, fileName = cms.untracked.string(options.outputFile)
                               , fileName = cms.untracked.string("test_cppf_unpacker_emulator-2018-200k-BugFix-2018JSON.root")
                               , SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
)

process.e = cms.EndPath(process.out)

