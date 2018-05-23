# CPPFDigis Emulator

This is the first version of the CPPF emulator. we use the RPC Digitization and 
reconstruction as intermedate steps. 
Under test you can find two examples, one unpacking 2017F RAW Data (cppf_emulator_RAW.py)
and another one using generated MC events (cppf_emulator_MC.py). 
The output of the unpacker is an edm branch named emulatorCppfDigis, following
the CPPFDigi dataformat already committed in CMSSW_10_1_X.

# Out of the box instructions

```
ssh -XY username@lxplus.cern.ch
#setenv SCRAM_ARCH slc6_amd64_gcc630 
#(or export SCRAM_ARCH=slc6_amd64_gcc630)
cmsrel CMSSW_10_1_1_patch1
cd CMSSW_10_1_1_patch1/src
cmsenv
```

```
git cms-init
git cms-addpkg DataFormats/L1TMuon
git cms-addpkg DataFormats/RPCDigi
git cms-addpkg L1Trigger/L1TMuonEndCap
git cms-addpkg EventFilter/L1TRawToDigi
git cms-addpkg EventFilter/RPCRawToDigi
git cms-addpkg CondTools/RPC
git cms-addpkg CondFormats/RPCObjects
git remote add maseguracern git@github.com:maseguracern/cmssw.git
git fetch maseguracern
git cms-merge-topic -u maseguracern:EMTF_test
git checkout maseguracern/EMTF_test
scram b -j6
```

## Update the AMCLink Map locally

```
cd CondTools/RPC
cp /eos/cms/store/group/dpg_rpc/comm_rpc/Run-II/cppf_payloads/RPCLinkMap.db data
for analyser in test/RPC*LinkMapPopConAnalyzer_cfg.py; do 
  cmsRun $analyser
done; # <- this produces RPCLinkMap.db sqlite file yourself
cd -
```

## Run the code (check the input)
```
cd L1Trigger/L1TMuonCPPF/test/
cmsRun cppf_emulator_RAW.py (Generate CPPFDigis only rpc unpacker)
cmsRun cppf_emulator_unpacker_RAW.py (before step plus Emulator Digis)
cmsRun cppf_emulator_unpacker_EMTF_RAW.py  (before step plus EMTF Digis)
```

## Run DQM comparisons
```
cmsRun Generator_DQM_CPPF.py
```

## Setup your Github space (In case you haven't)
```
git remote add YourGitHubName git@github.com:YourGitHubName/cmssw.git
git fetch YourGitHubName
git checkout -b YourBranchName
```

## Modifying files
```
git add <Modified files>
git commit -m "Commit message"
git push my-cmssw YourBranchName
```
