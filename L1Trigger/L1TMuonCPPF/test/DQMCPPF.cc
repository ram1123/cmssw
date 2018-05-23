#include "DQMCPPF.h"

DQM_CPPF::DQM_CPPF(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),
  cppfDigiToken3_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel3"))),
  EMTF_sector(0),    
  EMTF_subsector(0),
  EMTF_subsector_unpacker(0),
  EMTF_subsector_EMTF(0),
  EMTF_bx(0) {
}

DQM_CPPF::~DQM_CPPF(){
}

void DQM_CPPF::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){  
  
  //Get the CPPFDigi 
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis1;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis2;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis3;
  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);
  iEvent.getByToken(cppfDigiToken3_, CppfDigis3);
  
  //Fill the specific bin for each EMTF sector 
  for(int i = 1; i < 7; i++ ){
    EMTFsector0bins.push_back(0);
    EMTFsector1bins.push_back(i);
    EMTFsector2bins.push_back(i+6);
    EMTFsector3bins.push_back(i+12);
    EMTFsector4bins.push_back(i+18);
    EMTFsector5bins.push_back(i+24);
    EMTFsector6bins.push_back(i+30);
  }
  //FIll the map for each EMTF sector 
  fill_info[1] = EMTFsector1bins;
  fill_info[2] = EMTFsector2bins;
  fill_info[3] = EMTFsector3bins;
  fill_info[4] = EMTFsector4bins;
  fill_info[5] = EMTFsector5bins;
  fill_info[6] = EMTFsector6bins;
  fill_info[-99] = EMTFsector0bins;
  
  
  int matches_unpacker = 0;
  int matches_emtf = 0;
  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcId = cppf_digis.rpcId();
    int ring = rpcId.ring();
    int station = rpcId.station();
    int region = rpcId.region();
    int subsector = rpcId.subsector();
    int phi_int = cppf_digis.phi_int();
    int theta_int =  cppf_digis.theta_int();

    EMTF_sector = cppf_digis.emtf_sector();
    EMTF_subsector = fill_info[EMTF_sector][subsector-1];
    
      
    EMTF_bx = cppf_digis.bx();
    if(EMTF_bx >= 0) EMTF_bx = EMTF_bx+0.5;  
    else if(EMTF_bx < 0) EMTF_bx = EMTF_bx-0.5;  

    int fill_occupancy = occupancy_value(region, station, ring);
    int fill_bx = bx_value(region, EMTF_sector); 

  
    matches_unpacker = 0;  
    for(auto& cppf_digis_unpacker : *CppfDigis2){
      
      //RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
      //int ring_unpacker = rpcId_unpacker.ring();
      //int station_unpacker = rpcId_unpacker.station();
      //int region_unpacker = rpcId_unpacker.region();
      //int subsector_unpacker = rpcId_unpacker.subsector();
      int phi_int_unpacker = cppf_digis_unpacker.phi_int();
      int theta_int_unpacker =  cppf_digis_unpacker.theta_int();
      int bx_unpacker = cppf_digis_unpacker.bx();

      if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
	matches_unpacker++;
	Occupancy_unpacker->Fill(EMTF_subsector, fill_occupancy);
        std::cout << " emulated bx " << cppf_digis.bx() << " unpacker " << bx_unpacker << std::endl; 
      }
            
    }

    matches_emtf = 0;  
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcId_emtf = cppf_digis_emtf.rpcId();
      int ring_emtf = rpcId_emtf.ring();
      int station_emtf = rpcId_emtf.station();
      int region_emtf = rpcId_emtf.region();
      int subsector_emtf = rpcId_emtf.subsector();
      int phi_int_emtf = cppf_digis_emtf.phi_int();
      int theta_int_emtf =  cppf_digis_emtf.theta_int();
      int bx_emtf = cppf_digis_emtf.bx(); 

      int fill_occupancy_emtf = occupancy_value(region_emtf, station_emtf, ring_emtf);
      int EMTF_subsector_emtf = fill_info[cppf_digis_emtf.emtf_sector()][subsector_emtf-1];

      if((phi_int == phi_int_emtf) && (theta_int == theta_int_emtf)){
	matches_emtf++;
	Occupancy_EMTF->Fill(EMTF_subsector_emtf, fill_occupancy_emtf);
        std::cout << " emulated bx " << cppf_digis.bx() << " EMTF " << bx_emtf << std::endl;
      }
            
    }

    
    std::cout << matches_unpacker << std::endl;
    std::cout << matches_emtf << std::endl;
    std::cout << "--------------" << std::endl;
    

    Occupancy->Fill(EMTF_subsector, fill_occupancy); 
    Bx_Occupancy->Fill(EMTF_bx, fill_occupancy);
    Bx->Fill(fill_bx,EMTF_bx);
      
      
      Phi_Integer->Fill(cppf_digis.phi_int());
      Theta_Integer->Fill(cppf_digis.theta_int());
      
      Phi_Global->Fill(cppf_digis.phi_glob()*TMath::Pi()/180.);
      Theta_Global->Fill(cppf_digis.theta_glob()*TMath::Pi()/180.);
      
      Board->Fill(cppf_digis.board());
      Channel->Fill(cppf_digis.channel());
      
      if(cppf_digis.bx() == 0){
	Matches->Fill(matches_unpacker);
	Matches_e->Fill(matches_emtf);
      }    
      
      else if (cppf_digis.bx() == -1) Matches_e_minus1->Fill(matches_emtf);
      else if (cppf_digis.bx() == -2) Matches_e_minus2->Fill(matches_emtf);
      else if (cppf_digis.bx() == +1) Matches_e_plus1->Fill(matches_emtf);
      else if (cppf_digis.bx() == +2) Matches_e_plus2->Fill(matches_emtf);
      
      Phi_Global_Integer->Fill(cppf_digis.phi_glob(), cppf_digis.phi_int());
      Theta_Global_Integer->Fill(cppf_digis.theta_glob(), cppf_digis.theta_int());
      
      
      
      if(matches_unpacker == 0 && cppf_digis.bx() == 0){
	int near_value1 = 0.;
	int distance1 = 10000.;
	for(std::vector<int>::iterator it = phi_int_unpacker.begin(); it != phi_int_unpacker.end(); it++){
	  Phi_emu_unpacker_nomatched->Fill(cppf_digis.phi_int(),*it);
	  if(abs(cppf_digis.phi_int()-*it) < distance1){
	    distance1 = abs(cppf_digis.phi_int()-*it);
	    near_value1 = *it;
	  }  	
	}
	Phi_emu_unpacker_near->Fill(cppf_digis.phi_int(),near_value1);
	int near_value2 = 0.;
	int distance2 = 10000.;
	for(std::vector<int>::iterator it = theta_int_unpacker.begin(); it != theta_int_unpacker.end(); it++){
	  Theta_emu_unpacker_nomatched->Fill(cppf_digis.theta_int(),*it);	
	  if(abs(cppf_digis.theta_int()-*it) < distance2){ 
	    distance2 = abs(cppf_digis.theta_int()-*it);
	    near_value2 = *it;
	  }
	}
	Theta_emu_unpacker_near->Fill(cppf_digis.theta_int(),near_value2);	
	if(cppf_digis.theta_int() == 11) std::cout << cppf_digis.theta_int() << "  " << near_value2 << std::endl;
      } 
      
      
  } // loop over CPPFDigis
  
  
  
  
} //End class

int DQM_CPPF::occupancy_value(int region_, int station_, int ring_){
  
  int fill_val = 0;
  if(region_ == -1){
    
    if((station_ == 4) && (ring_ == 3)) fill_val = 1;
    else if ((station_ == 4) && (ring_ == 2)) fill_val = 2;
    else if ((station_ == 3) && (ring_ == 3)) fill_val = 3;
    else if ((station_ == 3) && (ring_ == 2)) fill_val = 4;
    else if ((station_ == 2) && (ring_ == 2)) fill_val = 5;
    else if ((station_ == 1) && (ring_ == 2)) fill_val = 6;
    
  }
  else if(region_ == +1){
    
    if((station_ == 1) && (ring_ == 2)) fill_val = 7; 
    else if((station_ == 2) && (ring_ == 2)) fill_val = 8;
    else if((station_ == 3) && (ring_ == 2)) fill_val = 9;
    else if((station_ == 3) && (ring_ == 3)) fill_val = 10;
    else if((station_ == 4) && (ring_ == 2)) fill_val = 11;
    else if((station_ == 4) && (ring_ == 3)) fill_val = 12;
    
  }
  return fill_val;
}

int DQM_CPPF::bx_value(int region_, int emtfsector_){
  
  int fill_val = 0;
  
  if(region_ == -1){
    
    if(emtfsector_ == 1) fill_val = 6;
    else if(emtfsector_ == 2) fill_val = 5;
    else if(emtfsector_ == 3) fill_val = 4;
    else if(emtfsector_ == 4) fill_val = 3;
    else if(emtfsector_ == 5) fill_val = 2;
    else if(emtfsector_ == 6) fill_val = 1;
    
  }
  
  else if(region_ == +1){
    
    if(emtfsector_ == 1) fill_val = 7;
    else if(emtfsector_ == 2) fill_val = 8;
    else if(emtfsector_ == 3) fill_val = 9;
    else if(emtfsector_ == 4) fill_val = 10;
    else if(emtfsector_ == 5) fill_val = 11;
    else if(emtfsector_ == 6) fill_val = 12;
  }
  return fill_val;
}


void DQM_CPPF::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
}

void DQM_CPPF::beginJob(){
  edm::Service<TFileService> fs;

  Phi_Integer = fs->make<TH1D>("Phi_Integer", "Phi_Integer", 1240, 0., 1240.);
  Theta_Integer = fs->make<TH1D>("Theta_Integer", "Theta_Integer", 32, 0., 32.);
  Phi_Integer_unpacker = fs->make<TH1D>("Phi_Integer_unpacker", "Phi_Integer_unpacker", 1240, 0., 1240.);
  Theta_Integer_unpacker = fs->make<TH1D>("Theta_Integer_unpacker", "Theta_Integer_unpacker", 32, 0., 32.);

  Phi_Global = fs->make<TH1D>("Phi_Global", "Phi_Global", 72, -3.15, 3.15);
  Theta_Global = fs->make<TH1D>("Theta_Global", "Theta_Global", 32, 0., 3.15);

  Matches = fs->make<TH1D>("Matches", "CPPFDigis_Matched" , 20, 0. , 20.);
  Matches_e = fs->make<TH1D>("Matches_e", "CPPFDigis_Matched_EMTF_BX0" , 20, 0. , 20.);
  Matches_e_minus1 = fs->make<TH1D>("Matches_e_minus1", "CPPFDigis_Matched_EMTF_BX-1" , 20, 0. , 20.);
  Matches_e_minus2 = fs->make<TH1D>("Matches_e_minus2", "CPPFDigis_Matched_EMTF_BX-2" , 20, 0. , 20.);
  Matches_e_plus1 = fs->make<TH1D>("Matches_e_plus1", "CPPFDigis_Matched_EMTF_BX+1" , 20, 0. , 20.);
  Matches_e_plus2 = fs->make<TH1D>("Matches_e_plus2", "CPPFDigis_Matched_EMTF_BX+2" , 20, 0. , 20.);

  Phi_Global_Integer = fs->make<TH2D>("Phi_Global_Integer", "Phi_Global_Integer", 360, -180, 180, 1240, 0.,1240.);
  Theta_Global_Integer = fs->make<TH2D>("Theta_Global_Integer", "Theta_Global_Integer", 45, 0, 45, 32, 0.,32.);

  Occupancy = fs->make<TH2D>("Occupancy", "CPPFDigis Occupancy", 36, 0.5, 36.5, 12, 0.5,12.5); 
  Occupancy_unpacker = fs->make<TH2D>("Occupancy_unpacker", "CPPFDigis Occupancy_unpacker", 36, 0.5, 36.5, 12, 0.5,12.5); 
  Occupancy_EMTF = fs->make<TH2D>("Occupancy_EMTF", "CPPFDigis Occupancy_EMTF", 36, 0.5, 36.5, 12, 0.5,12.5); 

  Bx = fs->make<TH2D>("Bx","CPPFDigis Bx", 12, 0.5, 12.5, 8,-3.5,4.5);
  Bx_Occupancy = fs->make<TH2D>("Bx_Occupancy","CPPFDigis Bx_Occupancy", 8, -3.5, 4.5, 12, 0.5, 12.5);

  
  Phi_emu_unpacker=fs->make<TH2D>("Phi_emu_unpacker", "Phi_emu_unpacker",  124, 0., 1240., 124, 0., 1240.);
  Theta_emu_unpacker=fs->make<TH2D>("Theta_emu_unpacker", "Theta_emu_unpacker",  32, 0., 32., 32, 0., 32.);

  Phi_emu_emtf=fs->make<TH2D>("Phi_emu_emtf", "Phi_emu_emtf",  124, 0., 1240., 124, 0., 1240.);
  Theta_emu_emtf=fs->make<TH2D>("Theta_emu_emtf", "Theta_emu_emtf",  32, 0., 32., 32, 0., 32.);

  Phi_emu_unpacker_nomatched=fs->make<TH2D>("Phi_emu_unpacker_nomatched", "Phi_emu_unpacker_nomatched",  124, 0., 1240., 124, 0., 1240.);
  Theta_emu_unpacker_nomatched=fs->make<TH2D>("Theta_emu_unpacker_nomatched", "Theta_emu_unpacker_nomatched",  32, 0., 32., 32, 0., 32.);

  Phi_emu_unpacker_near=fs->make<TH2D>("Phi_emu_unpacker_near", "Phi_emu_unpacker_near",  124, 0., 1240., 124, 0., 1240.);
  Theta_emu_unpacker_near=fs->make<TH2D>("Theta_emu_unpacker_near", "Theta_emu_unpacker_near",  32, 0., 32., 32, 0., 32.);

  emtf_sector_comparison=fs->make<TH2D>("emtf_sector_comparison", "emtf_sector_comparison", 6, 1.,7.,6,1.,7.);

  Board = fs->make<TH1D>("Board", "Board", 50, 0, 100);
  Channel = fs->make<TH1D>("Channel", "Channel", 100, 0, 200);
  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);

