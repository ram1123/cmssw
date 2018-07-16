import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import subprocess
import sys, os

from FWCore.PythonUtilities.LumiList import LumiList
from FWCore.ParameterSet.VarParsing import VarParsing
mylist = FileUtils.loadListFromFile('/afs/cern.ch/user/g/garamire/work/private/RPCserviceWork/trigger/CMSSW_10_1_1_patch1_test/src/L1Trigger/L1TMuonCPPF/test/inputFiles2.txt');
options = VarParsing("analysis")
options.register("runList"
                 , []
                 , VarParsing.multiplicity.list
                 , VarParsing.varType.int
                 , "Run selection")
options.register("lumiList"
                 #, "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/DCSOnly/json_DCSONLY.txt"
                 , "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/DCSOnly/json_DCSONLY.txt"
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
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.Geometry.GeometryDB_cff')
#process.load('Configuration.StandardSequences.GeometryExtended_cff')
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
#process.GlobalTag.globaltag = "92X_dataRun2_Express_v7"
process.GlobalTag.globaltag = "101X_dataRun2_Express_v7"

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1)
#)

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
#process.simEmtfDigis.RPCInput  = cms.InputTag('muonRPCDigis')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# Input source
#eos_cmd = '/afs/cern.ch/project/eos/installation/ams/bin/eos.select'
#readFiles = cms.untracked.vstring()

#process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( 
  
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/505/00000/FEA6AC9B-495C-E811-8D7A-FA163E674EDE.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/505/00000/FE9F76DC-4F5C-E811-BB67-FA163E78F43C.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/505/00000/FE6599DE-695C-E811-AAFA-FA163EA0835F.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/505/00000/FE16EC2A-475C-E811-8A13-FA163E4BB802.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/469/00000/9E1BFA61-085C-E811-BA52-FA163EA146B3.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/E00D409D-E95B-E811-8B2C-FA163E6B4587.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/DEE3CC05-DA5B-E811-A9C3-FA163EBDFE1C.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/DE6E98B0-E25B-E811-9EB9-FA163E58F6F5.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/DE0A24DB-DB5B-E811-AEE6-02163E01A135.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/DE06BD46-E55B-E811-84D5-FA163E16E0AB.root',
#                   'root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/457/00000/DE027BA9-DC5B-E811-B612-FA163E194E69.root',

#                   ))
process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring(*mylist)
            )
#in_dir_name = '/eos/cms/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/574/00000/'
#in_dir_name = '/eos/cms/store/data/Run2017F/SingleMuon/RAW/v1/000/306/121/00000/'
#in_dir_name = '/eos/cms/store/data/Commissioning2018/ZeroBias/RAW/v1/000/314/663/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/ZeroBias/RAW/v1/000/315/420/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/SingleMuon/RAW/v1/000/315/764/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/316/928/00000/'

#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'4CD446F8-8643-E811-ADE1-FA163E24B9CF.root') )
#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'AADB01BE-474C-E811-A57C-02163E013D6F.root') )
'''
iFile = 0
for in_file_name in subprocess.check_output([eos_cmd, 'ls', in_dir_name]).splitlines():
    if not ('.root' in in_file_name): continue
    iFile += 1
    readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+in_file_name) )
'''
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
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
                               , fileName = cms.untracked.string("test_cppf_unpacker_emulator.root")
                               , SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
)

process.e = cms.EndPath(process.out)

