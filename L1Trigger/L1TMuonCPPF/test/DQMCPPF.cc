#include "DQMCPPF.h"

DQM_CPPF::DQM_CPPF(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),

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

  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);

  ///////////////////////////////////////////////////////////
  // Comparison CPPF emulator (Ce) vs CPPF unpacked (Cu), 
  // EMTF unpacked (Eu) and EMTF emulator (Ee) 
  ///////////////////////////////////////////////////////////
  std::map<int,int> _nHit_Ce;
  std::map<int,int> _nHit_Cu;
  std::map<int,int> _phi_Ce;
  std::map<int,int> _phi_Cu;
  std::map<int,int> _phi_glob_Ce;
  std::map<int,int> _phi_glob_Cu;
  std::map<int,int> _theta_Ce;
  std::map<int,int> _theta_Cu;
  std::map<int,int> _theta_glob_Ce;
  std::map<int,int> _theta_glob_Cu;
  std::map<int,int> _roll_Ce;
  std::map<int,int> _roll_Cu;
  std::map<int,int> _zone_Ce;
  std::map<int,int> _zone_Cu;
  std::map<int,int> _ID_Ce;
  std::map<int,int> _ID_Cu;
  std::map<int,int> _emtfSubsector_Ce;
  std::map<int,int> _emtfSubsector_Cu;
  std::map<int,int> _emtfSector_Ce;
  std::map<int,int> _emtfSector_Cu;
  std::map<int,int> _bx_Ce;
  std::map<int,int> _bx_Cu;
  std::map<int,int> _cluster_size_Ce;
  std::map<int,int> _cluster_size_Cu;
  
  //std::cout << "DEBUG: Start..... " << std::endl;
  for(auto& cppf_digis : *CppfDigis1){

    RPCDetId rpcIdCe = (int)cppf_digis.rpcId();
    int regionCe = (int)rpcIdCe.region();
    int stationCe = (int)rpcIdCe.station();
    int sectorCe = (int)rpcIdCe.sector();
    int subsectorCe = (int)rpcIdCe.subsector();
    int ringCe = (int)rpcIdCe.ring();
    int rollCe = (int)(rpcIdCe.roll());
    int phiIntCe = (int)cppf_digis.phi_int();
    int thetaIntCe =  (int)cppf_digis.theta_int();
    int phiGlobalCe = (int)cppf_digis.phi_glob();
    int thetaGlobalCe =  (int)cppf_digis.theta_glob();
    int cluster_sizeCe = (int)cppf_digis.cluster_size();
    int bxCe = cppf_digis.bx();
    
    int emtfSectorCe = (int)cppf_digis.emtf_sector();
    int emtfSubsectorCe = GetSubsector(emtfSectorCe, subsectorCe);
    int fillOccupancyCe = occupancy_value(regionCe, stationCe, ringCe);

    //std::cout << "==================================" << std::endl;
    //std::cout << "=         DEBUG: 1               =" << std::endl;
    //std::cout << "roll = " << rollCe << std::endl;
    //std::cout << "regionCe = " << regionCe << std::endl;
    //std::cout << "stationCe = " << stationCe << std::endl;
    //std::cout << "ringCe = " << ringCe << std::endl;
    //std::cout << "sectorCe = " << sectorCe << std::endl;
    //std::cout << "subsectorCe = " << subsectorCe << std::endl;
    //std::cout << "phiIntCe = " << phiIntCe << std::endl;
    //std::cout << "thetaIntCe = " << thetaIntCe << std::endl;
    //std::cout << "bxCe = " << bxCe << std::endl;
    //std::cout << "emtfSectorCe = " << emtfSectorCe << std::endl;
    ////std::cout << "emtfSubsectorCe = " << emtfSubsectorCe << std::endl;
    //std::cout << "fillOccupancyCe = " << fillOccupancyCe << std::endl;
    //std::cout << "==================================" << std::endl;
    ////int fillBxCe = bx_value(regionCe, emtfSectorCe); 
    
    //Chamber ID
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);

    std::ostringstream oss;
    oss << regionCe << stationCe << ringCe << sectorCe << subsectorCe << rollCe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;
    //std::cout << "\n\n==================================================================\n" << std::endl;
    //std::cout << "\n\n printing _nHit_Ce ... \n" << std::endl;
    //std::cout << "iss = " << iss << "\n";
    //std::cout << "unique_id = " << unique_id << "\n";
    //std::cout << "_nHit_Ce.find(unique_id) = " << _nHit_Ce.find(unique_id) << "\n";
    //for (auto& t : _nHit_Ce)
    //   std::cout << "before: " << t.first << "\t" << t.second << std::endl;
    //std::cout << "_nHit_Ce.end() = " << _nHit_Ce.end() << std::endl;
    if ( _nHit_Ce.find(unique_id) == _nHit_Ce.end() ) { // chamber had no hit so far
      _nHit_Ce.insert({unique_id, 1});
      _phi_Ce.insert({unique_id,phiIntCe});
      _phi_glob_Ce.insert({unique_id,phiGlobalCe});
      _theta_Ce.insert({unique_id,thetaIntCe});
      _theta_glob_Ce.insert({unique_id,thetaGlobalCe});
      _roll_Ce.insert({unique_id,rollCe});
      _ID_Ce.insert({unique_id, chamberIDCe});
      _zone_Ce.insert({unique_id,fillOccupancyCe});
      _emtfSubsector_Ce.insert({unique_id,emtfSubsectorCe});
      _emtfSector_Ce.insert({unique_id,emtfSectorCe});
      _bx_Ce.insert({unique_id,bxCe});
      _cluster_size_Ce.insert({unique_id,cluster_sizeCe});
      //std::cout << "***" << std::endl;
    }
    else {
      _nHit_Ce.at(unique_id) += 1;
    }
  }
    std::cout << "\n\nprinting nHit_Ce after if condition" << std::endl;
    for (auto& t : _nHit_Ce)
       std::cout << t.first << "\t" << t.second << std::endl;

  for(auto& cppf_digis2 : *CppfDigis2){

    RPCDetId rpcIdCu = cppf_digis2.rpcId();
    int regionCu = rpcIdCu.region();
    int stationCu = rpcIdCu.station();
    int sectorCu = rpcIdCu.sector();
    int subsectorCu = rpcIdCu.subsector();
    int ringCu = rpcIdCu.ring();
    int rollCu = (int)(rpcIdCu.roll());
    int phiIntCu = cppf_digis2.phi_int();
    int thetaIntCu =  cppf_digis2.theta_int();
    int phiGlobalCu = (int)cppf_digis2.phi_glob();
    int thetaGlobalCu =  (int)cppf_digis2.theta_glob();
    int cluster_sizeCu = (int)cppf_digis2.cluster_size();
    int bxCu = cppf_digis2.bx();
    
    int emtfSectorCu = (int)cppf_digis2.emtf_sector();
    int emtfSubsectorCu = GetSubsector(emtfSectorCu, subsectorCu);
    int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
    //int fillBxCu = bx_value(regionCu, emtfSectorCu); 
    //std::cout << "==================================" << std::endl;
    //std::cout << "=         DEBUG: 1               =" << std::endl;
    //std::cout << "roll Unpack = " << rollCu << std::endl;
    //std::cout << "==================================" << std::endl;
    
    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);

    std::ostringstream oss2;
    oss2 << regionCu << stationCu << ringCu << sectorCu << subsectorCu << rollCu ;
    std::istringstream iss2(oss2.str());
    int unique_id;
    iss2 >> unique_id;
    //for (auto& t : _nHit_Cu)
    //   std::cout << "Unpacker - before: " << t.first << "\t" << t.second << std::endl;
    if ( _nHit_Cu.find(unique_id) == _nHit_Cu.end() ) { // chamber had no hit so far
      _nHit_Cu.insert({unique_id, 1});
      _phi_Cu.insert({unique_id,phiIntCu});
      _theta_Cu.insert({unique_id,thetaIntCu});
      _phi_glob_Cu.insert({unique_id,phiGlobalCu});
      _theta_glob_Cu.insert({unique_id,thetaGlobalCu});
      _ID_Cu.insert({unique_id, chamberIDCu});
      _zone_Cu.insert({unique_id,fillOccupancyCu});
      _roll_Cu.insert({unique_id,rollCu});
      _emtfSubsector_Cu.insert({unique_id,emtfSubsectorCu});
      _emtfSector_Cu.insert({unique_id,emtfSectorCu});
      _bx_Cu.insert({unique_id,bxCu});
      _cluster_size_Cu.insert({unique_id,cluster_sizeCu});
      //std::cout << "***" << std::endl;
    }
    else {
      _nHit_Cu.at(unique_id) += 1;
    }
  }
    std::cout << "\n\nprinting nHit_Cu after if condition" << std::endl;
    for (auto& t : _nHit_Cu)
       std::cout << t.first << "\t" << t.second << std::endl;

  int total_hits_unpacker = 0;
  int total_hits_unpacker_bx = 0;
  int total_hits_unpacker_bx_phi = 0;

  int total_hits_emulator = 0;
  int total_hits_emulator_bx = 0;
  int total_hits_emulator_bx_phi = 0;

  for (auto const& it : _nHit_Ce){
      int key = it.first;
      std::cout << "Main for loop starts..." << key << std::endl;
      if (_nHit_Cu.find(key) != _nHit_Cu.end()){
        //std::cout << "DEBUG:1: key = "<<key<<" Number of hits = it.second = " << it.second << "\t _nHit_Cu.at(key) = " << _nHit_Cu.at(key) << std::endl; 
        std::cout << "key = "<<key<<" Number of hits = it.second = " << it.second << "\t _nHit_Cu.at(key) = " << _nHit_Cu.at(key) << std::endl; 
        int region = 0;
        int station = 0;
        int ring = 0;
        int sector = 0;
        int subsector = 0;
        //int roll = 0;
        if (key < 0) {
           int splitter_temp = -1*key;
           region = -1*(splitter_temp/100000)%10;
           station = (splitter_temp/10000)%10;
           ring = (splitter_temp/1000)%10;
           sector = (splitter_temp/100)%10;
           subsector = (splitter_temp/10)%10;
           //roll = splitter_temp%10;
           std::cout << region << "\t" << station << "\t" << ring << "\t" << sector << "\t" << subsector << "\t" << _nHit_Cu.at(key) << "\t" << _nHit_Ce.at(key) << std::endl;
           h1_nHits_each_chamber_unpacker[0][station][ring][sector][subsector]->Fill(_nHit_Cu.at(key));
           h1_nHits_each_chamber_emulator[0][station][ring][sector][subsector]->Fill(_nHit_Ce.at(key));
        } else {
           region = (key/100000)%10;
           station = (key/10000)%10;
           ring = (key/1000)%10;
           sector = (key/100)%10;
           subsector = (key/10)%10;
           //roll = key%10;
           std::cout << region << "\t" << station << "\t" << ring << "\t" << sector << "\t" << subsector << "\t" << _nHit_Cu.at(key) << "\t" << _nHit_Ce.at(key) << std::endl;
           h1_nHits_each_chamber_unpacker[1][station][ring][sector][subsector]->Fill(_nHit_Cu.at(key));
           h1_nHits_each_chamber_emulator[1][station][ring][sector][subsector]->Fill(_nHit_Ce.at(key));
	}
        //std::cout << "====> " << region << "\t" << station << "\t" << ring << std::endl;
	total_hits_unpacker += _nHit_Cu.at(key);
	total_hits_emulator += _nHit_Ce.at(key);
	
        h2_chamber_emu_unpacker->Fill(_ID_Ce.at(key), _ID_Cu.at(key)); 
        if (region==-1 && station==4 &&  ring==3 )  h2_chamber_emu_unpacker_REm43->Fill(_ID_Ce.at(key), _ID_Cu.at(key)); 
        else if (region==-1 && station==4 &&  ring==2 ) h2_chamber_emu_unpacker_REm42->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==-1 && station==3 &&  ring==3 ) h2_chamber_emu_unpacker_REm33->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==-1 && station==3 &&  ring==2 ) h2_chamber_emu_unpacker_REm32->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==-1 && station==2 &&  ring==2 ) h2_chamber_emu_unpacker_REm22->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==-1 && station==1 &&  ring==2 ) h2_chamber_emu_unpacker_REm12->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==1 &&  ring==2 ) h2_chamber_emu_unpacker_REp12->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==2 &&  ring==2 ) h2_chamber_emu_unpacker_REp22->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==3 &&  ring==2 ) h2_chamber_emu_unpacker_REp32->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==3 &&  ring==3 ) h2_chamber_emu_unpacker_REp33->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==4 &&  ring==2 ) h2_chamber_emu_unpacker_REp42->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else if (region==+1 && station==4 &&  ring==3 ) h2_chamber_emu_unpacker_REp43->Fill(_ID_Ce.at(key), _ID_Cu.at(key));
        else {
           std::cout << "Region or Station or ring number does not have physical meaning!!!" << std::endl;
           std::cout << "Check below output and debug the code..." << std::endl;
           std::cout << "region = " << region << "\nstation = " << station << "\nring = " << ring << std::endl;
           std::cout << "Terminating the program." << std::endl;
           std::exit(0);
        }

        h2_phi_emu_unpacker->Fill(_phi_Ce.at(key), _phi_Cu.at(key));
        h2_theta_emu_unpacker->Fill(_theta_Ce.at(key), _theta_Cu.at(key));
        h2_bx_emu_unpacker->Fill(_bx_Ce.at(key),_bx_Cu.at(key));
        h1_bx_emulated->Fill(_bx_Ce.at(key));
        h1_bx_unpacker->Fill(_bx_Cu.at(key));
        h1_bx_diff_emu_unpacker->Fill(std::abs(_bx_Cu.at(key)-_bx_Ce.at(key)));
        h1_phi_diff_emu_unpacker->Fill(std::abs(_phi_Ce.at(key)-_phi_Cu.at(key)));
	
        if(_bx_Ce.at(key) ==  _bx_Cu.at(key)){
	  total_hits_unpacker_bx += _nHit_Cu.at(key);
	  total_hits_emulator_bx += _nHit_Ce.at(key);
	  h2_phi_emu_unpacker_bx->Fill(_phi_Ce.at(key), _phi_Cu.at(key));
	  h2_theta_emu_unpacker_bx->Fill(_theta_Ce.at(key), _theta_Cu.at(key)); 
	  h2_bx_emu_unpacker_bx->Fill(_bx_Ce.at(key),_bx_Cu.at(key));
	  
	  if((_phi_Ce.at(key) == _phi_Cu.at(key)) && (_theta_Ce.at(key) == _theta_Cu.at(key))){
	    total_hits_unpacker_bx_phi += _nHit_Cu.at(key);
	    total_hits_emulator_bx_phi += _nHit_Ce.at(key);
	    h2_phi_emu_unpacker_bx_phi->Fill(_phi_Ce.at(key), _phi_Cu.at(key));
	    h2_theta_emu_unpacker_bx_phi->Fill(_theta_Ce.at(key), _theta_Cu.at(key)); 
            //Occupancy 
	    h2_occupancy_unpacker_bx_phi->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
	    h2_occupancy_emu_unpacker_bx_phi->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
            //bx occupancy 
            //h2_bx_occupancy_unpacker_bx_phi->Fill(EMTF_bx, _zone_Cu.at(key));
            //h2_bx_occupancy_emu_unpacker_bx_phi->Fill(EMTF_bx, _zone_Ce.at(key));
            //bx
            //h2_bx_sector_unpacker_bx_phi->Fill(fill_bx_unpacker,EMTF_bx);
            //h2_bx_sector_emu_unpacker_bx_phi->Fill(fill_bx,EMTF_bx);
	  } 
	}

        if (it.second == 1 && _nHit_Cu.at(key) == 1) {
          //std::cout << "Entered in one hit condition" << std::endl;
          h2CeVsCuChamberCuChamberCe_OneHit->Fill(_ID_Cu.at(key), _ID_Ce.at(key)); 
	  h2CeVsCuPhiCePhiCu_OneHit->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	  h2CeVsCuThetaCeThetaCu_OneHit->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
          // Occupancy 
          if (_bx_Ce.at(key) == 0 && _bx_Cu.at(key) == 0){
	        h2CeVsCuChamberCuZoneCu_OneHit_bx->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
	        h2CeVsCuChamberCeZoneCe_OneHit_bx->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
          }
          // Bx by zone
          h2CeVsCuBxCuZoneCuOccupancy_OneHit->Fill(_bx_Cu.at(key), _zone_Cu.at(key));
          h2CeVsCuBxCeZoneCeOccupancy_OneHit->Fill(_bx_Ce.at(key), _zone_Ce.at(key));
          // Bx by EMTF sector
          h2CeVsCuBxCuZoneCu_OneHit->Fill(_emtfSector_Cu.at(key), _bx_Cu.at(key));
          h2CeVsCuBxCeZoneCe_OneHit->Fill(_emtfSector_Ce.at(key), _bx_Ce.at(key));
          //Difference between variables
          h1CeVsCuPhiCePhiCuDiff_OneHit->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
          h1CeVsCuThetaCeThetaCuDiff_OneHit->Fill(_theta_Ce.at(key) - _theta_Cu.at(key));
          h1CeVsCuBxCeBxCuDiff_OneHit->Fill(_bx_Ce.at(key) - _bx_Cu.at(key));

          if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) h1CeVsCuPhi_InDiagonal_OneHit->Fill(0.);
          else h1CeVsCuPhi_OffDiagonal_OneHit->Fill(0.);
          if ((_theta_Ce.at(key) - _theta_Cu.at(key)) == 0) h1CeVsCuTheta_InDiagonal_OneHit->Fill(0.);
          else h1CeVsCuTheta_OffDiagonal_OneHit->Fill(0.);
          if (_bx_Ce.at(key) == 0 && _bx_Cu.at(key) == 0){
            h1CeVsCuPhiCePhiCuDiff_OneHit_bx->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
            h1CeVsCuThetaCeThetaCuDiff_OneHit_bx->Fill(_theta_Ce.at(key) - _theta_Cu.at(key));
	    h2CeVsCuPhiCePhiCu_OneHit_bx->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	    h2CeVsCuThetaCeThetaCu_OneHit_bx->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
            if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) {
              h1CeVsCuPhi_InDiagonal_OneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            else {
              h1CeVsCuPhi_OffDiagonal_OneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            if ((_theta_Ce.at(key) - _theta_Cu.at(key)) == 0) h1CeVsCuTheta_InDiagonal_OneHit_bx->Fill(0.);
            else {
              h1CeVsCuTheta_OffDiagonal_OneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            if ((_theta_Ce.at(key) == _theta_Cu.at(key)) && (_phi_Ce.at(key) != _phi_Cu.at(key))) h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
          }
        }
        else {
          // Occupancy 
          h2CeVsCuChamberCuZoneCu_NotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          h2CeVsCuChamberCeZoneCe_NotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
          //std::cout << "Entered in more than one hit condition" << std::endl;

          if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) {
              h1CeVsCuPhi_InDiagonal_NotOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          }
          else {
              h1CeVsCuPhi_OffDiagonal_NotOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          }
        }
      }
  }
 
  h1_total_hits_unpacker->Fill(total_hits_unpacker);
  h1_total_hits_unpacker_bx->Fill(total_hits_unpacker_bx);
  h1_total_hits_unpacker_bx_phi->Fill(total_hits_unpacker_bx_phi);	

  h1_total_hits_emulator->Fill(total_hits_emulator);
  h1_total_hits_emulator_bx->Fill(total_hits_emulator_bx);
  h1_total_hits_emulator_bx_phi->Fill(total_hits_emulator_bx_phi);	
  
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

int DQM_CPPF::GetSubsector(int emtfsector_, int lsubsector_){
  const int nsectors = 6;
  int gsubsector = 0;
  if ((emtfsector_ != -99) and (lsubsector_ != 0)){
    gsubsector = (emtfsector_ - 1)*nsectors + lsubsector_;
  }
  return gsubsector;
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
  
  h1_total_hits_unpacker = fs->make<TH1D>("h1_total_hits_unpacker", "CPPFDigis_total_hits_unpacker" , 25, 0. , 25.);
  h1_total_hits_unpacker_bx = fs->make<TH1D>("h1_total_hits_unpacker_bx", "CPPFDigis_Matches_bx" , 25, 0. , 25.);
  h1_total_hits_unpacker_bx_phi = fs->make<TH1D>("h1_total_hits_unpacker_bx_phi", "CPPFDigis_Matches_int" , 25, 0. , 25.);

  h1_total_hits_emulator = fs->make<TH1D>("h1_total_hits_emulator", "CPPFDigis_total_hits_emulator" , 25, 0. , 25.);
  h1_total_hits_emulator_bx = fs->make<TH1D>("h1_total_hits_emulator_bx", "CPPFDigis_Matches_bx" , 25, 0. , 25.);
  h1_total_hits_emulator_bx_phi = fs->make<TH1D>("h1_total_hits_emulator_bx_phi", "CPPFDigis_Matches_int" , 25, 0. , 25.);

  h1_bx_emulated = fs->make<TH1D>("h1_bx_emulated","Emulated bunch crossing",8, -4., 4.);
  h1_bx_unpacker = fs->make<TH1D>("h1_bx_unpacker","Unpacked bunch crossing",8, -4., 4.);
  h1_bx_diff_emu_unpacker = fs->make<TH1D>("h1_bx_diff_emu_unpacker","Difference of Bunch crossing emulator and unpacker",500,0,100.);
  h1_phi_diff_emu_unpacker = fs->make<TH1D>("h1_phi_diff_emu_unpacker","Difference of phi emulator and unpacker",32,0.,8.);


  
  h2_chamber_emu_unpacker = fs->make<TH2D>("h2_chamber_emu_unpacker","h2_chamber_emu_unpacker", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm43 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm43","h2_chamber_emu_unpacker_REm43", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm42 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm42","h2_chamber_emu_unpacker_REm42", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm33 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm33","h2_chamber_emu_unpacker_REm33", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm32 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm32","h2_chamber_emu_unpacker_REm32", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm22 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm22","h2_chamber_emu_unpacker_REm22", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REm12 = fs->make<TH2D>("h2_chamber_emu_unpacker_REm12","h2_chamber_emu_unpacker_REm12", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp12 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp12","h2_chamber_emu_unpacker_REp12", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp22 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp22","h2_chamber_emu_unpacker_REp22", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp32 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp32","h2_chamber_emu_unpacker_REp32", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp33 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp33","h2_chamber_emu_unpacker_REp33", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp42 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp42","h2_chamber_emu_unpacker_REp42", 36, 1.,37.,36,1.,37.);
  h2_chamber_emu_unpacker_REp43 = fs->make<TH2D>("h2_chamber_emu_unpacker_REp43","h2_chamber_emu_unpacker_REp43", 36, 1.,37.,36,1.,37.);

  h2_phi_emu_unpacker=fs->make<TH2D>("h2_phi_emu_unpacker", "h2_phi_emu_unpacker",  124, 0., 1240., 124, 0., 1240.);
  h2_theta_emu_unpacker=fs->make<TH2D>("h2_theta_emu_unpacker", "h2_theta_emu_unpacker",  32, 0., 32., 32, 0., 32.);
  h2_bx_emu_unpacker = fs->make<TH2D>("h2_bx_emu_unpacker","h2_bx_emu_unpacker", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2_phi_emu_unpacker_bx=fs->make<TH2D>("h2_phi_emu_unpacker_bx", "h2_phi_emu_unpacker_bx",  124, 0., 1240., 124, 0., 1240.);
  h2_theta_emu_unpacker_bx=fs->make<TH2D>("h2_theta_emu_unpacker_bx", "h2_theta_emu_unpacker_bx",  32, 0., 32., 32, 0., 32.);
  h2_bx_emu_unpacker_bx = fs->make<TH2D>("h2_bx_emu_unpacker_bx","h2_bx_emu_unpacker_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2_phi_emu_unpacker_bx_phi=fs->make<TH2D>("h2_phi_emu_unpacker_bx_phi", "h2_phi_emu_unpacker_bx_phi",  124, 0., 1240., 124, 0., 1240.);
  h2_theta_emu_unpacker_bx_phi=fs->make<TH2D>("h2_theta_emu_unpacker_bx_phi", "h2_theta_emu_unpacker_bx_phi",  32, 0., 32., 32, 0., 32.);
  h2_occupancy_unpacker_bx_phi = fs->make<TH2D>("h2_occupancy_unpacker_bx_phi", "CPPFDigis h2_occupancy_unpacker_bx_phi", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2_occupancy_emu_unpacker_bx_phi = fs->make<TH2D>("h2_occupancy_emu_unpacker_bx_phi", "CPPFDigis h2_occupancy_emu_unpacker_bx_phi", 36, 0.5, 36.5, 12, 0.5,12.5); 
  //h2_bx_occupancy_unpacker_bx_phi = fs->make<TH2D>("h2_bx_occupancy_unpacker_bx_phi","CPPFDigis h2_bx_occupancy_unpacker_bx_phi", 8, -3.5, 4.5, 12, 0.5, 12.5);
  //h2_bx_occupancy_emu_unpacker_bx_phi = fs->make<TH2D>("h2_bx_occupancy_emu_unpacker_bx_phi","CPPFDigis h2_bx_occupancy_emu_unpacker_bx_phi", 8, -3.5, 4.5, 12, 0.5, 12.5);
  //h2_bx_sector_unpacker_bx_phi = fs->make<TH2D>("h2_bx_sector_unpacker_bx_phi","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  //h2_bx_sector_emu_unpacker_bx_phi = fs->make<TH2D>("h2_bx_sector_emu_unpacker_bx_phi","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);


  /// NEW HISTO FROM ANOTHER CODDEp

  h1CeVsCuPhiCePhiCuDiff_OneHit = fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiff_OneHit", "h1CeVsCuPhiCePhiCuDiff_OneHit",  200, -100., 100.);
  h1CeVsCuPhiCePhiCuDiff_OneHit_bx = fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiff_OneHit_bx", "h1CeVsCuPhiCePhiCuDiff_OneHit_bx",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiff_OneHit = fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiff_OneHit", "h1CeVsCuThetaCeThetaCuDiff_OneHit",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiff_OneHit_bx = fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiff_OneHit_bx", "h1CeVsCuThetaCeThetaCuDiff_OneHit_bx",  200, -100., 100.);
  h1CeVsCuBxCeBxCuDiff_OneHit = fs->make<TH1D>("h1CeVsCuBxCeBxCuDiff_OneHit","h1CeVsCuBxCeBxCuDiff_OneHit", 8, -3.5, 4.5);
  h1CeVsCuPhi_InDiagonal_OneHit = fs->make<TH1D>("h1CeVsCuPhi_InDiagonal_OneHit", "CPPFDigis_h1CeVsCuPhi_InDiagonal_OneHit" , 2, 0. , 2.);
  h1CeVsCuPhi_InDiagonal_OneHit_bx = fs->make<TH1D>("h1CeVsCuPhi_InDiagonal_OneHit_bx", "CPPFDigis_h1CeVsCuPhi_InDiagonal_OneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhi_OffDiagonal_OneHit = fs->make<TH1D>("h1CeVsCuPhi_OffDiagonal_OneHit", "CPPFDigis_h1CeVsCuPhi_OffDiagonal_OneHit" , 2, 0. , 2.);
  h1CeVsCuPhi_OffDiagonal_OneHit_bx = fs->make<TH1D>("h1CeVsCuPhi_OffDiagonal_OneHit_bx", "CPPFDigis_h1CeVsCuPhi_OffDiagonal_OneHit_bx" , 2, 0. , 2.);
  h1CeVsCuTheta_InDiagonal_OneHit = fs->make<TH1D>("h1CeVsCuTheta_InDiagonal_OneHit", "CPPFDigis_h1CeVsCuTheta_InDiagonal_OneHit" , 2, 0. , 2.);
  h1CeVsCuTheta_InDiagonal_OneHit_bx = fs->make<TH1D>("h1CeVsCuTheta_InDiagonal_OneHit_bx", "CPPFDigis_h1CeVsCuTheta_InDiagonal_OneHit_bx" , 2, 0. , 2.);
  h1CeVsCuTheta_OffDiagonal_OneHit = fs->make<TH1D>("h1CeVsCuTheta_OffDiagonal_OneHit", "CPPFDigis_h1CeVsCuTheta_OffDiagonal_OneHit" , 2, 0. , 2.);
  h1CeVsCuTheta_OffDiagonal_OneHit_bx = fs->make<TH1D>("h1CeVsCuTheta_OffDiagonal_OneHit_bx", "CPPFDigis_h1CeVsCuTheta_OffDiagonal_OneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhi_InDiagonal_NotOneHit_bx = fs->make<TH1D>("h1CeVsCuPhi_InDiagonal_NotOneHit_bx", "CPPFDigis_h1CeVsCuPhi_InDiagonal_NotOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhi_OffDiagonal_NotOneHit_bx = fs->make<TH1D>("h1CeVsCuPhi_OffDiagonal_NotOneHit_bx", "CPPFDigis_h1CeVsCuPhi_OffDiagonal_NotOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx = fs->make<TH1D>("h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx", "h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx",  200, -100., 100.);


  h2CeVsCuChamberCuChamberCe_OneHit = fs->make<TH2D>("h2CeVsCuChamberCuChamberCe_OneHit","h2CeVsCuChamberCuChamberCe_OneHit", 36, 1.,37.,36,1.,37.);
  h2CeVsCuPhiCePhiCu_OneHit = fs->make<TH2D>("h2CeVsCuPhiCePhiCu_OneHit", "h2CeVsCuPhiCePhiCu_OneHit",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuPhiCePhiCu_OneHit_bx = fs->make<TH2D>("h2CeVsCuPhiCePhiCu_OneHit_bx", "h2CeVsCuPhiCePhiCu_OneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCu_OneHit = fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_OneHit", "h2CeVsCuThetaCeThetaCu_OneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuThetaCeThetaCu_OneHit_bx = fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_OneHit_bx", "h2CeVsCuThetaCeThetaCu_OneHit_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuChamberCuZoneCu_OneHit_bx = fs->make<TH2D>("h2CeVsCuChamberCuZoneCu_OneHit_bx", "CPPFDigis h2CeVsCuChamberCuZoneCu_OneHit_bx", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCe_OneHit_bx = fs->make<TH2D>("h2CeVsCuChamberCeZoneCe_OneHit_bx", "CPPFDigis h2CeVsCuChamberCeZoneCe_OneHit_bx", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCuZoneCuOccupancy_OneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancy_OneHit","CPPFDigis Bx_Occupancy_unpacker_OneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCeZoneCeOccupancy_OneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancy_OneHit","CPPFDigis Bx_Occupancy_emu_unpacker_OneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCuZoneCu_OneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCu_OneHit","CPPFDigis Bx_unpacker_OneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCe_OneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCe_OneHit","CPPFDigis Bx_emu_unpacker_OneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_emu_unpacker_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_unpacker_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_emu_unpacker_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_unpacker_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonal_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonal_OneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuChamberCuZoneCu_NotOneHit = fs->make<TH2D>("h2CeVsCuChamberCuZoneCu_NotOneHit", "CPPFDigis h2CeVsCuChamberCuZoneCu_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCe_NotOneHit = fs->make<TH2D>("h2CeVsCuChamberCeZoneCe_NotOneHit", "CPPFDigis h2CeVsCuChamberCeZoneCe_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit","CPPFDigis Bx_Occupancy_emu_unpacker_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit","CPPFDigis Bx_Occupancy_unpacker_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit","CPPFDigis Bx_Occupancy_emu_unpacker_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit","CPPFDigis Bx_Occupancy_unpacker_NotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);


  // for (int ii=0; ii<432; ii++)
  //   h1_nHits_each_chamber_unpacker[ii] = fs->make<TH1F>(Form("h1%d",ii), Form("h1%d",ii), 25, 0, 25);
  //   
  for (int region=0; region<2; region++)
    for (int station=0; station<5; station++)
      for (int ring=0; ring<4; ring++)
        for (int sector=0; sector<7; sector++)
          for (int subsector=0; subsector<7; subsector++) {
            h1_nHits_each_chamber_unpacker[region][station][ring][sector][subsector] = fs->make<TH1F>("h1_nHits_each_chamber_unpacker", Form("h1_nHits_region%d_station%d_ring%d_sector%d_subsector%d_unpacker",region, station, ring, sector, subsector), 10, 0, 10);
            h1_nHits_each_chamber_emulator[region][station][ring][sector][subsector] = fs->make<TH1F>("h1_nHits_each_chamber_emulator", Form("h1_nHits_region%d_station%d_ring%d_sector%d_subsector%d_emulator",region, station, ring, sector, subsector), 10, 0, 10);
          }


  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);
