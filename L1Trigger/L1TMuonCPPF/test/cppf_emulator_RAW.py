
import FWCore.ParameterSet.Config as cms
import datetime
import random
import subprocess


process = cms.Process('DIGI')

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
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = "92X_dataRun2_Express_v7"
process.GlobalTag.globaltag = "101X_dataRun2_Express_v7"

process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
muonRPCDigis.InputLabel = 'rawDataCollector'

process.load('RecoLocalMuon.RPCRecHit.rpcRecHits_cfi')
from RecoLocalMuon.RPCRecHit.rpcRecHits_cfi import *

process.load('L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi')
from L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

eos_cmd = '/afs/cern.ch/project/eos/installation/ams/bin/eos.select'

# Input source
readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
        fileNames = readFiles,
)

#in_dir_name = '/eos/cms/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/574/00000/'
#in_dir_name = '/eos/cms/store/data/Run2017F/SingleMuon/RAW/v1/000/306/121/00000/'
in_dir_name = '/eos/cms/store/data/Commissioning2018/ZeroBias/RAW/v1/000/314/663/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/ZeroBias/RAW/v1/000/315/420/00000/'

readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'4CD446F8-8643-E811-ADE1-FA163E24B9CF.root') )
#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'AADB01BE-474C-E811-A57C-02163E013D6F.root') )

#iFile = 0
#for in_file_name in subprocess.check_output([eos_cmd, 'ls', in_dir_name]).splitlines():
#    if not ('.root' in in_file_name): continue
#    iFile += 1
#    readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+in_file_name) )



process.options = cms.untracked.PSet(
    )

process.treeOut = cms.OutputModule("PoolOutputModule",
                                   fileName = cms.untracked.string('test_cppf_emulator.root'),
                                   outputCommands = cms.untracked.vstring('drop *',
                                                                          "keep *_rpcunpacker_*_*", 
                                                                          "keep *_emulatorCppfDigis_*_*",
                                                                          "keep *_rpcRecHits_*_*",
                                                                          #"keep *"
                                                                          )
                                   )


# Additional output definition

# Other statements

# process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.rpcunpacker.InputLabel = 'rawDataCollector'
process.rpcRecHits.rpcDigiLabel = 'rpcunpacker'
process.emulatorCppfDigis.recHitLabel = 'rpcRecHits'

# Path and EndPath definitions
process.path_step =  cms.Path(process.rpcunpacker)
process.rpcrechits_step = cms.Path(process.rpcRecHits)
process.emulatorCppfDigis_step = cms.Path(process.emulatorCppfDigis)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.treeOut_step = cms.EndPath(process.treeOut)



# Schedule definition
process.schedule = cms.Schedule(
    process.path_step,
    process.rpcrechits_step,
    process.emulatorCppfDigis_step,
    process.endjob_step,
    process.treeOut_step )

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Customisation from command line
# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

