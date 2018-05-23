#include "Analyzer/BDTCPPF/interface/BDTCPPF.h"

BDTCPPF::BDTCPPF(const edm::ParameterSet& iConfig):
  genParToken_(consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticlesLabel"))),
  recHitToken_(consumes<RPCRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitLabel"))),
  cppfDigiToken_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel"))),  
  Gen_Muon(0.,0.,0.),
  Pt_Muon_Matched(0.),
  IPt_Muon(0.),
  NMuons(0),
  NRechits(0),
  NCPPF(0),
  deltaphi12(-2111),
  deltaphi23(-2111),
  deltaphi34(-2111),
  deltatheta12(-2111),
  deltatheta23(-2111),
  deltatheta34(-2111),
  deltatheta14(-2111),
  cluster1(-2111),
  cluster2(-2111),
  cluster3(-2111),
  cluster4(-2111){
}

BDTCPPF::~BDTCPPF(){
}

void BDTCPPF::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  edm::Handle<std::vector<reco::GenParticle>> genParHandle;
  iEvent.getByToken(genParToken_,genParHandle);
  
  edm::Handle<RPCRecHitCollection> recHits;
  iEvent.getByToken(recHitToken_, recHits);
  
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis;
  iEvent.getByToken(cppfDigiToken_, CppfDigis);
  
  
  if(genParHandle.isValid()){
    NMuons = 0;
    for (auto& gen_iter : *genParHandle){
      //For muons  == -13 
      if(gen_iter.pdgId() == -13){   
	NMuons++;
        Gen_Muon.SetXYZ(gen_iter.p4().x(), gen_iter.p4().y(), gen_iter.p4().z());	
        Pt_Muon_Matched = gen_iter.pt();
        if(Pt_Muon_Matched != 0) IPt_Muon = 1.0/Pt_Muon_Matched;
        //std::cout << Pt_Muon_Matched << std::endl;	
	
	double minDeltaR[4];
	for(int i = 0; i < 4; i++ ) minDeltaR[i] = 0.3;
	int match_rawId[4]; 
	for(int i = 0; i < 4; i++ ) match_rawId[i] = 0;
	
	double DeltaR_M_Rechit=0.;
	
	NRechits = 0;
	
	for (auto& rechit_it : *recHits){
	  RPCDetId rpcId = rechit_it.rpcId();	
          int bx = rechit_it.BunchX();
	  int station = rpcId.station();
	  int region = rpcId.region();
	  const RPCRoll* roll = rpcGeom->roll(rpcId);
	  const BoundPlane& rollSurface = roll->surface();
	  LocalPoint lPos = rechit_it.localPosition();
	  GlobalPoint gPos = rollSurface.toGlobal(lPos);
	  TVector3 pos(gPos.x(),gPos.y(),gPos.z());
	  
	  if(region == 0) continue;
          if(abs(bx) > 1) continue;
	  DeltaR_M_Rechit = Gen_Muon.DeltaR(pos);
	  
	  //std::cout << DeltaR_M_Rechit << " " << station << std::endl;
	  if(DeltaR_M_Rechit < minDeltaR[station-1]){
	    minDeltaR[station-1] = DeltaR_M_Rechit; 
	    match_rawId[station-1] = rpcId.rawId();
	    //global_phi[station-1] = emtf::rad_to_deg(gPos.phi().value());
	  }
	  
	  
	} //Loop over rechit to find the match digis 
	
	for(int i = 0; i < 4; i++){
	  if(match_rawId[i] != 0) NRechits++; 
	}	

        if(NRechits == 0) continue;

        
	
	int phi_int[4];
	for(int i = 0; i < 4; i++ ) phi_int[i] = -2111;
	int theta_int[4];
	for(int i = 0; i < 4; i++ ) theta_int[i] = -2111;
	int cluster[4];
	for(int i = 0; i < 4; i++ ) cluster[i] = -2111;
	
	NCPPF = 0;
	
        phi1 = -2111;
        phi2 = -2111;
        phi3 = -2111;
        phi4 = -2111;
	
        theta1 = -2111;
        theta2 = -2111;
        theta3 = -2111;
        theta4 = -2111;
	
        deltaphi12 = -2111;
        deltaphi23 = -2111;
        deltaphi34 = -2111;
		
        deltatheta12 = -2111;
        deltatheta23 = -2111;
        deltatheta34 = -2111;
        deltatheta14 = -2111;


	for(auto& cppf_digis : *CppfDigis){ 
	  const RPCDetId& rpcId = cppf_digis.rpcId();
	  int rawId = rpcId.rawId();
	  int Cppf_station = rpcId.station();
	
	  if(match_rawId[Cppf_station-1] == rawId){
	    phi_int[Cppf_station-1] = cppf_digis.phi_int();
	    theta_int[Cppf_station-1] = cppf_digis.theta_int();
	    cluster[Cppf_station-1] = cppf_digis.cluster_size();
	  }
	  
	} //loop over CPPFDigisi
	
	for(int i = 0; i < 4; i++){
	  if(phi_int[i] != -2111) NCPPF++; 
	}
	
	if(NRechits != NCPPF) continue;	
	//if(NCPPF < 4) continue; // Cut over the number of cppf

        // set of variable to fill	

	phi1 = phi_int[0];
	phi2 = phi_int[1];
	phi3 = phi_int[2];
	phi4 = phi_int[3];
        
	theta1 = theta_int[0];
	theta2 = theta_int[1];
	theta3 = theta_int[2];
	theta4 = theta_int[3];
	
	
        if((phi1 != -2111) && (phi2 != -2111)) deltaphi12 = phi1-phi2;
        if((phi2 != -2111) && (phi3 != -2111)) deltaphi23 = phi2-phi3;
        if((phi3 != -2111) && (phi4 != -2111)) deltaphi34 = phi3-phi4;
	
        
        if((theta1 != -2111) && (theta2 != -2111)) deltatheta12 = theta1-theta2;
        if((theta2 != -2111) && (theta3 != -2111)) deltatheta23 = theta2-theta3;
        if((theta3 != -2111) && (theta4 != -2111)) deltatheta34 = theta3-theta4;
        if((theta1 != -2111) && (theta4 != -2111)) deltatheta14 = theta1-theta4;
	
        cluster1 = cluster[0];
        cluster2 = cluster[1];
        cluster3 = cluster[2];
        cluster4 = cluster[3];
	
	
        tree_->Fill();
	
	
      } // chossing only muons
    } // loop over GenParticles
  } //isValid GenCollection
  
  
}

void BDTCPPF::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
}

void BDTCPPF::beginJob(){
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("bdt_cppf","bdt_cppf");
  tree_->Branch("Pt_Muon_Matched", &Pt_Muon_Matched, "Pt_Muon_Matched/F");
  tree_->Branch("IPt_Muon", &IPt_Muon, "IPt_Muon/F");
  tree_->Branch("NMuons", &NMuons, "NMuons/S");
  tree_->Branch("NRechits", &NRechits, "NRechits/S");
  tree_->Branch("NCPPF", &NCPPF, "NCPPF/S");
  tree_->Branch("phi1", &phi1, "phi1/S");
  tree_->Branch("phi2", &phi2, "phi2/S");
  tree_->Branch("phi3", &phi3, "phi3/S");
  tree_->Branch("phi4", &phi4, "phi4/S");
  tree_->Branch("theta1", &theta1, "theta1/S");
  tree_->Branch("theta2", &theta2, "theta2/S");
  tree_->Branch("theta3", &theta3, "theta3/S");
  tree_->Branch("theta4", &theta4, "theta4/S");
  tree_->Branch("deltaphi12", &deltaphi12, "deltaphi12/S");
  tree_->Branch("deltaphi23", &deltaphi23, "deltaphi23/S");
  tree_->Branch("deltaphi34", &deltaphi34, "deltaphi34/S");
  tree_->Branch("deltatheta12", &deltatheta12, "deltatheta12/S");
  tree_->Branch("deltatheta23", &deltatheta23, "deltatheta23/S");
  tree_->Branch("deltatheta34", &deltatheta34, "deltatheta34/S");
  tree_->Branch("deltatheta14", &deltatheta14, "deltatheta14/S");
  tree_->Branch("cluster1", &cluster1, "cluster1/S");
  tree_->Branch("cluster2", &cluster2, "cluster2/S");
  tree_->Branch("cluster3", &cluster3, "cluster3/S");
  tree_->Branch("cluster4", &cluster4, "cluster4/S");

  return;
}
DEFINE_FWK_MODULE(BDTCPPF);
