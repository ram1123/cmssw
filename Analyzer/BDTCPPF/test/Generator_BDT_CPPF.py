import FWCore.ParameterSet.Config as cms
import subprocess
#import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("DQMPlots")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('BDT_Training')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
	limit = cms.untracked.int32(-1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(800000) )

readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
        fileNames = readFiles,
)
#in_dir_name = '/eos/cms/store/user/masegura/CRAB_PrivateMC/CRAB3_CPPF_April2018_MC_analysis/180510_162500/0000/'
#in_dir_name = '/eos/cms/store/user/masegura/CRAB_PrivateMC/CRAB3_CPPF_April2018_MC_analysis/180510_162500/0000/test/'
#in_dir_name = '/eos/cms/store/user/masegura/CRAB_PrivateMC/CRAB3_CPPF_April2018_MC_100_analysis/180511_103006/0000/'
in_dir_name = '/eos/cms/store/user/masegura/CRAB_PrivateMC/CRAB3_CPPF_April2018_MC_100_analysis/180511_103006/0000/test/'
#in_dir_name = '/afs/cern.ch/user/m/masegura/public/MC/'
#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'MC_0.root') )
#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'MC_1.root') )

iFile = 0
for in_file_name in subprocess.check_output(['ls', in_dir_name]).splitlines():
    if not ('.root' in in_file_name): continue
    iFile += 1
    readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+in_file_name) )



process.load('Analyzer.BDTCPPF.bdtcppf_cfi')
process.TFileService = cms.Service("TFileService",
	fileName = cms.string("BDT_CPPF.root")
)
process.p = cms.Path(process.BDTCPPF)


