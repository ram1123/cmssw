from CRABClient.UserUtilities import config, getUsernameFromSiteDB 
config = config()

config.General.instance = 'preprod'
config.General.requestName = 'April2018_MC_analysis'
config.General.workArea = 'CPPF'
config.General.transferOutputs = True
config.General.transferLogs = True


config.section_("JobType")
config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = '/afs/cern.ch/user/m/masegura/CMSSW_10_1_1_patch1/src/L1Trigger/L1TMuonCPPF/test/cppf_emulator_MC.py'
#config.JobType.maxMemoryMB = 2000

config.section_("Data")
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 10000
NJOBS = 200
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/%s/' %(getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_CPPF_April2018_MC_100_analysis'

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
