import FWCore.ParameterSet.Config as cms

from HLTrigger.Muon.CommonModules_cff import *
import copy

hltmmkFilter = cms.EDFilter("HLTmmkFilter",
    MinCosinePointingAngle = cms.double(0.9),
    MinLxySignificance = cms.double(3.0),
    MinPt = cms.double(3.0),
    MaxEta = cms.double(2.5),
    ThirdTrackMass = cms.double(0.106),
    FastAccept = cms.bool(False),
    MaxInvMass = cms.double(2.2),
    TrackCand = cms.InputTag("hltMumukAllConeTracks"),
    MaxNormalisedChi2 = cms.double(10.0),
    MinInvMass = cms.double(1.2),
    MuCand = cms.InputTag("hltMuTracks")
)
