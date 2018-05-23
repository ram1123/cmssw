source /cvmfs/cms.cern.ch/crab3/crab.sh
#source /cvmfs/cms.cern.ch/crab3/crab_standalone.sh
voms-proxy-init -voms cms -valid 192:00
voms-proxy-info -all
#crab checkusername
#crab checkwrite --site=T2_CH_CERN
