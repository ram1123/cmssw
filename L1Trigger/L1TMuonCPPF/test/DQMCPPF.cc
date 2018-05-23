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
  
  //Unpacker Digis 
  phi_int_unpacker.clear();
  theta_int_unpacker.clear();
  emtf_sector_unpacker.clear();
  subsector_unpacker.clear();
  //EMTF Digis
  phi_int_EMTF.clear();
  theta_int_EMTF.clear();
  emtf_sector_EMTF.clear();
  subsector_EMTF.clear();
  

  
  for(auto& cppf_digis : *CppfDigis3){
   RPCDetId rpcId = cppf_digis.rpcId();
   int subsector = rpcId.subsector();
    phi_int_EMTF.push_back(cppf_digis.phi_int());
    theta_int_EMTF.push_back(cppf_digis.theta_int());
    emtf_sector_EMTF.push_back(cppf_digis.emtf_sector());
    subsector_EMTF.push_back(subsector);
  }

  for(auto& cppf_digis : *CppfDigis2){
    RPCDetId rpcId = cppf_digis.rpcId();
    int subsector = rpcId.subsector();
    phi_int_unpacker.push_back(cppf_digis.phi_int());
    theta_int_unpacker.push_back(cppf_digis.theta_int());
    emtf_sector_unpacker.push_back(cppf_digis.emtf_sector());
    subsector_unpacker.push_back(subsector);
    //Filling unpacker digis
    Phi_Integer_unpacker->Fill(cppf_digis.phi_int());
    Theta_Integer_unpacker->Fill(cppf_digis.theta_int());
  }
  
  int matches = 0;
  int matches_emtf = 0;
  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcId = cppf_digis.rpcId();
    int ring = rpcId.ring();
    int station = rpcId.station();
    int region = rpcId.region();
    int subsector = rpcId.subsector();
   
    //Unpacker 
    bool find_unpacker = false;
    std::vector<int>::iterator it_phi;
    it_phi = std::find(phi_int_unpacker.begin(),phi_int_unpacker.end(),cppf_digis.phi_int());
    std::vector<int>::iterator it_theta;
    it_theta = std::find(theta_int_unpacker.begin(), theta_int_unpacker.end(), cppf_digis.theta_int());
   
    int emtf_unpacker_pos = 0;
    if((it_phi != phi_int_unpacker.end()) && (it_theta != theta_int_unpacker.end())){
      find_unpacker=true;
      auto pos = std::distance(std::begin(phi_int_unpacker), it_phi);
      emtf_unpacker_pos = int(pos);
      matches++;
    }
 
    //EMTF
    bool find_EMTF = false;
    std::vector<int>::iterator it_phi_e; 
    it_phi_e = std::find(phi_int_EMTF.begin(),phi_int_EMTF.end(),cppf_digis.phi_int());
    std::vector<int>::iterator it_theta_e;
    it_theta_e = std::find(theta_int_EMTF.begin(), theta_int_EMTF.end(), cppf_digis.theta_int());

    int emtf_EMTF_pos = 0;
    if((it_phi_e != phi_int_EMTF.end()) && (it_theta_e != theta_int_EMTF.end())){
	find_EMTF=true;
        auto pos = std::distance(std::begin(phi_int_EMTF), it_phi_e);
         emtf_EMTF_pos = int(pos);
         matches_emtf++;
    }

    int EMTF_sector_EMTF = 0;
    int EMTF_subsector_EMTF = 0;
    int EMTF_sector_unpacker = 0;
    int EMTF_subsector_unpacker = 0;

    if(find_EMTF){
      EMTF_sector_EMTF = emtf_sector_EMTF[emtf_EMTF_pos];
      EMTF_subsector_EMTF = subsector_EMTF[emtf_EMTF_pos];
      EMTF_subsector_EMTF = fill_info[EMTF_sector_EMTF][EMTF_subsector_EMTF-1];
      Phi_emu_emtf->Fill(cppf_digis.phi_int(),*it_phi_e);
      Theta_emu_emtf->Fill(cppf_digis.theta_int(),*it_theta_e);	   
    }

    if(find_unpacker){
      EMTF_sector_unpacker = emtf_sector_unpacker[emtf_unpacker_pos];
      EMTF_subsector_unpacker = subsector_unpacker[emtf_unpacker_pos];
      EMTF_subsector_unpacker = fill_info[EMTF_sector_unpacker][EMTF_subsector_unpacker-1];
      Phi_emu_unpacker->Fill(cppf_digis.phi_int(),*it_phi);
      Theta_emu_unpacker->Fill(cppf_digis.theta_int(),*it_theta);	   
    }   
 
    emtf_sector_comparison->Fill(cppf_digis.emtf_sector(), EMTF_sector_unpacker);

      //for Occupancy
      EMTF_sector = cppf_digis.emtf_sector();
      EMTF_subsector = fill_info[EMTF_sector][subsector-1];


   //Region -	
    if(region == -1){
      


      EMTF_bx = cppf_digis.bx();
      if(EMTF_bx >= 0) EMTF_bx = EMTF_bx+0.5;  
      else if(EMTF_bx < 0) EMTF_bx = EMTF_bx-0.5;  
      
      if((station == 4) && (ring == 3)){
	Occupancy->Fill(EMTF_subsector, 1);
        Bx_Occupancy->Fill(EMTF_bx, 1);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 1);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 1);
      }
      else if((station == 4) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 2);
        Bx_Occupancy->Fill(EMTF_bx, 2);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 2);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 2);
      }
      else if((station == 3) && (ring == 3)){
	Occupancy->Fill(EMTF_subsector, 3);
        Bx_Occupancy->Fill(EMTF_bx, 3);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 3);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 3);
      }
      else if((station == 3) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 4);
        Bx_Occupancy->Fill(EMTF_bx, 4);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 4);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 4);
      }
      else if((station == 2) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 5);
        Bx_Occupancy->Fill(EMTF_bx, 5);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 5);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 5);
      }
      else if((station == 1) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 6);
        Bx_Occupancy->Fill(EMTF_bx, 6);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 6);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 6);
      }
      
      //for Bx
      if(EMTF_sector==1)
	Bx->Fill(6,EMTF_bx);
      else if(EMTF_sector==2)
	Bx->Fill(5,EMTF_bx);
      else if(EMTF_sector==3)
	Bx->Fill(4,EMTF_bx);
      else if(EMTF_sector==4)
	Bx->Fill(3,EMTF_bx);
      else if(EMTF_sector==5)
	Bx->Fill(2,EMTF_bx);
      else if(EMTF_sector==6)
	Bx->Fill(1,EMTF_bx);

    }

    //Region +	
    else if(region == 1){
      
   
      EMTF_bx = cppf_digis.bx();
      if(EMTF_bx >= 0) EMTF_bx = EMTF_bx+0.5;  
      else if(EMTF_bx < 0) EMTF_bx = EMTF_bx-0.5;  
      
      if((station == 1) && (ring == 2)){ 
	Occupancy->Fill(EMTF_subsector, 7);          
        Bx_Occupancy->Fill(EMTF_bx, 7);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 7);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 7);
      }
      else if((station == 2) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 8);          
        Bx_Occupancy->Fill(EMTF_bx, 8);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 8);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 8);
      }
      else if((station == 3) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 9);          
        Bx_Occupancy->Fill(EMTF_bx, 9);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 9);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 9);
      }
      else if((station == 3) && (ring == 3)){
	Occupancy->Fill(EMTF_subsector, 10);          
        Bx_Occupancy->Fill(EMTF_bx, 10);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 10);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 10);
      }
      else if((station == 4) && (ring == 2)){
	Occupancy->Fill(EMTF_subsector, 11);          
        Bx_Occupancy->Fill(EMTF_bx, 11);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 11);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 11);
      }
      else if((station == 4) && (ring == 3)){
	Occupancy->Fill(EMTF_subsector, 12);          
        Bx_Occupancy->Fill(EMTF_bx, 12);
	if(find_unpacker) Occupancy_unpacker->Fill(EMTF_subsector, 12);
	if(find_EMTF) Occupancy_EMTF->Fill(EMTF_subsector_EMTF, 12);
      }
      
      //for Bx
      if(EMTF_sector==1)
	Bx->Fill(7,EMTF_bx);
      else if(EMTF_sector==2)
	Bx->Fill(8,EMTF_bx);
      else if(EMTF_sector==3)
	Bx->Fill(9,EMTF_bx);
      else if(EMTF_sector==4)
	Bx->Fill(10,EMTF_bx);
      else if(EMTF_sector==5)
	Bx->Fill(11,EMTF_bx);
      else if(EMTF_sector==6)
	Bx->Fill(12,EMTF_bx);
    }
    
    
    Phi_Integer->Fill(cppf_digis.phi_int());
    Theta_Integer->Fill(cppf_digis.theta_int());

    Phi_Global->Fill(cppf_digis.phi_glob()*TMath::Pi()/180.);
    Theta_Global->Fill(cppf_digis.theta_glob()*TMath::Pi()/180.);
    
    Board->Fill(cppf_digis.board());
    Channel->Fill(cppf_digis.channel());

    if(cppf_digis.bx() == 0){
	 Matches->Fill(matches);
	 Matches_e->Fill(matches_emtf);
	}    

    else if (cppf_digis.bx() == -1) Matches_e_minus1->Fill(matches_emtf);
    else if (cppf_digis.bx() == -2) Matches_e_minus2->Fill(matches_emtf);
    else if (cppf_digis.bx() == +1) Matches_e_plus1->Fill(matches_emtf);
    else if (cppf_digis.bx() == +2) Matches_e_plus2->Fill(matches_emtf);

    Phi_Global_Integer->Fill(cppf_digis.phi_glob(), cppf_digis.phi_int());
    Theta_Global_Integer->Fill(cppf_digis.theta_glob(), cppf_digis.theta_int());


    
    if(matches == 0 && cppf_digis.bx() == 0){
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

