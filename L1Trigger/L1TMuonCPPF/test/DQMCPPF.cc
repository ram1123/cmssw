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
  std::map<int,int> _theta_Ce;
  std::map<int,int> _theta_Cu;
  std::map<int,int> _occupancy_Ce;
  std::map<int,int> _occupancy_Cu;
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
  
  std::map<int,int> _nHit_Ee;
  std::map<int,int> _nHit_Eu;
  std::map<int,int> _phi_Ee;
  std::map<int,int> _phi_Eu;
  std::map<int,int> _theta_Ee;
  std::map<int,int> _theta_Eu;
  std::map<int,int> _occupancy_Ee;
  std::map<int,int> _occupancy_Eu;
  std::map<int,int> _zone_Ee;
  std::map<int,int> _zone_Eu;
  std::map<int,int> _ID_Ee;
  std::map<int,int> _ID_Eu;
  std::map<int,int> _emtfSubsector_Ee;
  std::map<int,int> _emtfSubsector_Eu;
  std::map<int,int> _emtfSector_Ee;
  std::map<int,int> _emtfSector_Eu;
  std::map<int,int> _bx_Ee;
  std::map<int,int> _bx_Eu;

  for(auto& cppf_digis : *CppfDigis1){

    RPCDetId rpcIdCe = (int)cppf_digis.rpcId();
    int regionCe = (int)rpcIdCe.region();
    int stationCe = (int)rpcIdCe.station();
    int ringCe = (int)rpcIdCe.ring();
    int sectorCe = (int)rpcIdCe.sector();
    int subsectorCe = (int)rpcIdCe.subsector();
    int phiIntCe = (int)cppf_digis.phi_int();
    int thetaIntCe =  (int)cppf_digis.theta_int();
    int bxCe = cppf_digis.bx();
    
    int emtfSectorCe = (int)cppf_digis.emtf_sector();
    int emtfSubsectorCe = GetSubsector(emtfSectorCe, subsectorCe);
    int fillOccupancyCe = occupancy_value(regionCe, stationCe, ringCe);

    //std::cout << "==================================" << std::endl;
    //std::cout << "=         DEBUG: 1               =" << std::endl;
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
    ////std::cout << "==================================" << std::endl;
    ////int fillBxCe = bx_value(regionCe, emtfSectorCe); 
    
    //Chamber ID
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);

    std::ostringstream oss;
    oss << regionCe << stationCe << ringCe << sectorCe << subsectorCe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;
    //std::cout << "\n\n==================================================================\n" << std::endl;
    //std::cout << "\n\n printing _nHit_Ce ... \n" << std::endl;
    //std::cout << "iss = " << iss << "\n";
    //std::cout << "unique_id = " << unique_id << "\n";
    //std::cout << "_nHit_Ce.find(unique_id) = " << _nHit_Ce.find(unique_id) << "\n";
    //for (auto& t : _nHit_Ce)
    //   std::cout << t.first << "\t" << t.second << std::endl;
    //std::cout << "_nHit_Ce.end() = " << _nHit_Ce.end() << std::endl;
    if ( _nHit_Ce.find(unique_id) == _nHit_Ce.end() ) { // chamber had no hit so far
      _nHit_Ce.insert({unique_id, 1});
      _phi_Ce.insert({unique_id,phiIntCe});
      _theta_Ce.insert({unique_id,thetaIntCe});
      _ID_Ce.insert({unique_id, chamberIDCe});
      _zone_Ce.insert({unique_id,fillOccupancyCe});
      _emtfSubsector_Ce.insert({unique_id,emtfSubsectorCe});
      _emtfSector_Ce.insert({unique_id,emtfSectorCe});
      _bx_Ce.insert({unique_id,bxCe});
      //std::cout << "***" << std::endl;
    }
    else {
      _nHit_Ce.at(unique_id) += 1;
    }
    //std::cout << "\n\nprinting nHit_Ce after if condition" << std::endl;
    //for (auto& t : _nHit_Ce)
    //   std::cout << t.first << "\t" << t.second << std::endl;
  }

  for(auto& cppf_digis2 : *CppfDigis2){

    RPCDetId rpcIdCu = cppf_digis2.rpcId();
    int regionCu = rpcIdCu.region();
    int stationCu = rpcIdCu.station();
    int ringCu = rpcIdCu.ring();
    int sectorCu = rpcIdCu.sector();
    int subsectorCu = rpcIdCu.subsector();
    int phiIntCu = cppf_digis2.phi_int();
    int thetaIntCu =  cppf_digis2.theta_int();
    int bxCu = cppf_digis2.bx();
    
    int emtfSectorCu = (int)cppf_digis2.emtf_sector();
    int emtfSubsectorCu = GetSubsector(emtfSectorCu, subsectorCu);
    int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
    //int fillBxCu = bx_value(regionCu, emtfSectorCu); 
    
    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);

    std::ostringstream oss2;
    oss2 << regionCu << stationCu << ringCu << sectorCu << subsectorCu;
    std::istringstream iss2(oss2.str());
    int unique_id;
    iss2 >> unique_id;
    if ( _nHit_Cu.find(unique_id) == _nHit_Cu.end() ) { // chamber had no hit so far
      _nHit_Cu.insert({unique_id, 1});
      _phi_Cu.insert({unique_id,phiIntCu});
      _theta_Cu.insert({unique_id,thetaIntCu});
      _ID_Cu.insert({unique_id, chamberIDCu});
      _zone_Cu.insert({unique_id,fillOccupancyCu});
      _emtfSubsector_Cu.insert({unique_id,emtfSubsectorCu});
      _emtfSector_Cu.insert({unique_id,emtfSectorCu});
      _bx_Cu.insert({unique_id,bxCu});
    }
    else {
      _nHit_Cu.at(unique_id) += 1;
    }
  }
  for (auto const& it : _nHit_Ce){
      int key = it.first;
      if (_nHit_Cu.find(key) != _nHit_Cu.end()){
        //std::cout << "DEBUG:1: Number of hits = it.second = " << it.second << "\t _nHit_Cu.at(key) = " << _nHit_Cu.at(key) << std::endl; 
        if (it.second == 1 && _nHit_Cu.at(key) == 1) {
          //std::cout << "Entered in one hit condition" << std::endl;
          h2CeVsCuChamberCuChamberCeOneHit->Fill(_ID_Cu.at(key), _ID_Ce.at(key)); 
	  h2CeVsCuPhiCePhiCuOneHit->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	  h2CeVsCuThetaCeThetaCuOneHit->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
          // Occupancy 
          if (_bx_Ce.at(key) == 0 && _bx_Cu.at(key) == 0){
	        h2CeVsCuChamberCuZoneCuOneHit_bx->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
	        h2CeVsCuChamberCeZoneCeOneHit_bx->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
          }
          // Bx by zone
          h2CeVsCuBxCuZoneCuOccupancyOneHit->Fill(_bx_Cu.at(key), _zone_Cu.at(key));
          h2CeVsCuBxCeZoneCeOccupancyOneHit->Fill(_bx_Ce.at(key), _zone_Ce.at(key));
          // Bx by EMTF sector
          h2CeVsCuBxCuZoneCuOneHit->Fill(_emtfSector_Cu.at(key), _bx_Cu.at(key));
          h2CeVsCuBxCeZoneCeOneHit->Fill(_emtfSector_Ce.at(key), _bx_Ce.at(key));
          //Difference between variables
          h1CeVsCuPhiCePhiCuDiffOneHit->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
          h1CeVsCuThetaCeThetaCuDiffOneHit->Fill(_theta_Ce.at(key) - _theta_Cu.at(key));
          h1CeVsCuBxCeBxCuDiffOneHit->Fill(_bx_Ce.at(key) - _bx_Cu.at(key));

          if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) h1CeVsCuPhiInDiagonalOneHit->Fill(0.);
          else h1CeVsCuPhiOffDiagonalOneHit->Fill(0.);
          if ((_theta_Ce.at(key) - _theta_Cu.at(key)) == 0) h1CeVsCuThetaInDiagonalOneHit->Fill(0.);
          else h1CeVsCuThetaOffDiagonalOneHit->Fill(0.);
          if (_bx_Ce.at(key) == 0 && _bx_Cu.at(key) == 0){
            h1CeVsCuPhiCePhiCuDiffOneHit_bx->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
            h1CeVsCuThetaCeThetaCuDiffOneHit_bx->Fill(_theta_Ce.at(key) - _theta_Cu.at(key));
	    h2CeVsCuPhiCePhiCuOneHit_bx->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	    h2CeVsCuThetaCeThetaCuOneHit_bx->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
            if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) {
              h1CeVsCuPhiInDiagonalOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            else {
              h1CeVsCuPhiOffDiagonalOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            if ((_theta_Ce.at(key) - _theta_Cu.at(key)) == 0) h1CeVsCuThetaInDiagonalOneHit_bx->Fill(0.);
            else {
              h1CeVsCuThetaOffDiagonalOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
            }
            if ((_theta_Ce.at(key) == _theta_Cu.at(key)) && (_phi_Ce.at(key) != _phi_Cu.at(key))) h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
          }
        }
        else {
          // Occupancy 
          h2CeVsCuChamberCuZoneCuNotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          h2CeVsCuChamberCeZoneCeNotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
          //std::cout << "Entered in more than one hit condition" << std::endl;

          if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) {
              h1CeVsCuPhiInDiagonalNotOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalNotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalNotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          }
          else {
              h1CeVsCuPhiOffDiagonalNotOneHit_bx->Fill(0.);
              h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalNotOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
              h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalNotOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
          }
        }
      }
  }


  //   OLD CODE
  
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
  int matches_unpacker_bx = 0;
  int matches_unpacker_bx_phi = 0;
  
  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcId = cppf_digis.rpcId();
    int region = rpcId.region();
    int station = rpcId.station();
    int ring = rpcId.ring();
    int sector = rpcId.sector();
    int subsector = rpcId.subsector();
    int phi_int = cppf_digis.phi_int();
    float phi_global = cppf_digis.phi_glob()*TMath::Pi()/180.;
    int board = cppf_digis.board();
    int cluster_size = cppf_digis.cluster_size();
    int theta_int =  cppf_digis.theta_int();
    float theta_global = cppf_digis.theta_glob()*TMath::Pi()/180.; 
    
    EMTF_sector = cppf_digis.emtf_sector();
    EMTF_subsector = fill_info[EMTF_sector][subsector-1];
    
    EMTF_bx = cppf_digis.bx();
    if(EMTF_bx >= 0) EMTF_bx = EMTF_bx+0.5;  
    else if(EMTF_bx < 0) EMTF_bx = EMTF_bx-0.5;  
    
    int fill_occupancy = occupancy_value(region, station, ring);
    int fill_bx = bx_value(region, EMTF_sector); 
    
    //Chamber ID
    int nsub_e = 6;
    (ring == 1 && station > 1) ? nsub_e = 3 : nsub_e = 6;
    int chamberID = subsector + nsub_e * ( sector - 1);
    
    
    matches_unpacker = 0;  
    matches_unpacker_bx = 0;
    matches_unpacker_bx_phi = 0;
    for(auto& cppf_digis_unpacker : *CppfDigis2){
      
      RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
      int region_unpacker = rpcId_unpacker.region();
      int station_unpacker = rpcId_unpacker.station();
      int ring_unpacker = rpcId_unpacker.ring();
      int sector_unpacker = rpcId_unpacker.sector();
      int subsector_unpacker = rpcId_unpacker.subsector();
      int phi_int_unpacker = cppf_digis_unpacker.phi_int();
      double phi_global_unpacker = cppf_digis_unpacker.phi_glob()*TMath::Pi()/180.;
      int theta_int_unpacker =  cppf_digis_unpacker.theta_int();
      double theta_global_unpacker = cppf_digis_unpacker.theta_glob()*TMath::Pi()/180.;
      int bx_unpacker = cppf_digis_unpacker.bx();
      
      int EMTF_sector_u = cppf_digis_unpacker.emtf_sector();
      int EMTF_subsector_u = fill_info[EMTF_sector_u][subsector_unpacker-1];

      //Chamber ID
      int nsub_un = 6;
      (ring_unpacker == 1 && station_unpacker > 1) ? nsub_un = 3 : nsub_un = 6;
      int chamberID_unpacker = subsector_unpacker + nsub_un * ( sector_unpacker - 1);
      
      int fill_occupancy_unpacker = occupancy_value(region_unpacker, station_unpacker, ring_unpacker);
      int fill_bx_unpacker = bx_value(region_unpacker, EMTF_sector_u);
      
      if((region == region_unpacker) && 
	 (station == station_unpacker) &&
	 (ring == ring_unpacker) &&
	 (sector == sector_unpacker) &&
	 (subsector == subsector_unpacker)){
	
	matches_unpacker++;
	
        h2_chamber_emu_unpacker->Fill(chamberID, chamberID_unpacker); 
        if (region==-1 && station==4 &&  ring==3 )  h2_chamber_emu_unpacker_REm43->Fill(chamberID, chamberID_unpacker); 
        else if (region==-1 && station==4 &&  ring==2 ) h2_chamber_emu_unpacker_REm42->Fill(chamberID, chamberID_unpacker);
        else if (region==-1 && station==3 &&  ring==3 ) h2_chamber_emu_unpacker_REm33->Fill(chamberID, chamberID_unpacker);
        else if (region==-1 && station==3 &&  ring==2 ) h2_chamber_emu_unpacker_REm32->Fill(chamberID, chamberID_unpacker);
        else if (region==-1 && station==2 &&  ring==2 ) h2_chamber_emu_unpacker_REm22->Fill(chamberID, chamberID_unpacker);
        else if (region==-1 && station==1 &&  ring==2 ) h2_chamber_emu_unpacker_REm12->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==1 &&  ring==2 ) h2_chamber_emu_unpacker_REp12->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==2 &&  ring==2 ) h2_chamber_emu_unpacker_REp22->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==3 &&  ring==2 ) h2_chamber_emu_unpacker_REp32->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==3 &&  ring==3 ) h2_chamber_emu_unpacker_REp33->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==4 &&  ring==2 ) h2_chamber_emu_unpacker_REp42->Fill(chamberID, chamberID_unpacker);
        else if (region==+1 && station==4 &&  ring==3 ) h2_chamber_emu_unpacker_REp43->Fill(chamberID, chamberID_unpacker);
        else {
           std::cout << "Region or Station or ring number does not have physical meaning!!!" << std::endl;
           std::cout << "Check below output and debug the code..." << std::endl;
           std::cout << "region = " << region << "\nstation = " << station << "\nring = " << ring << std::endl;
           std::cout << "Terminating the program." << std::endl;
           std::exit(0);
        }
        #if 0
        std::cout << "\n\n===========================" << std::endl;
        std::cout << "region_unpacker = " << region_unpacker << "\n";
        std::cout << "station_unpacker = " << station_unpacker << "\n";
        std::cout << "ring_unpacker = " << ring_unpacker << "\n";
        std::cout << "sector_unpacker = " << sector_unpacker << "\n";
        std::cout << "subsector_unpacker = " << subsector_unpacker << "\n" << std::endl;
        #endif
	
        h2_phi_emu_unpacker->Fill(phi_int, phi_int_unpacker);
        h2_theta_emu_unpacker->Fill(theta_int, theta_int_unpacker);
        h2_bx_emu_unpacker->Fill(cppf_digis.bx(),bx_unpacker);
        h1_bx_emulated->Fill(cppf_digis.bx());
        h1_bx_unpacker->Fill(bx_unpacker);
        h1_bx_diff_emu_unpacker->Fill(std::abs(bx_unpacker-cppf_digis.bx()));
        h1_phi_diff_emu_unpacker->Fill(std::abs(phi_int-phi_int_unpacker));
	
        if(cppf_digis.bx() ==  bx_unpacker){
	  matches_unpacker_bx++;
	  h2_phi_emu_unpacker_bx->Fill(phi_int, phi_int_unpacker);
	  h2_theta_emu_unpacker_bx->Fill(theta_int, theta_int_unpacker); 
	  h2_bx_emu_unpacker_bx->Fill(cppf_digis.bx(),bx_unpacker);
	  
	  if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
	    matches_unpacker_bx_phi++;
	    h2_phi_emu_unpacker_bx_phi->Fill(phi_int, phi_int_unpacker);
	    h2_theta_emu_unpacker_bx_phi->Fill(theta_int, theta_int_unpacker); 
            //Occupancy 
	    h2_occupancy_unpacker_bx_phi->Fill(EMTF_subsector_u, fill_occupancy_unpacker);
	    //h2_occupancy_unpacker_bx_phi->Fill(EMTF_subsector, fill_occupancy);
	    h2_occupancy_emu_unpacker_bx_phi->Fill(EMTF_subsector, fill_occupancy);
            //bx occupancy 
            h2_bx_occupancy_unpacker_bx_phi->Fill(EMTF_bx, fill_occupancy_unpacker);
            h2_bx_occupancy_emu_unpacker_bx_phi->Fill(EMTF_bx, fill_occupancy);
            //bx
            h2_bx_sector_unpacker_bx_phi->Fill(fill_bx_unpacker,EMTF_bx);
            h2_bx_sector_emu_unpacker_bx_phi->Fill(fill_bx,EMTF_bx);
	  } 
	}
	
      }
      
    } // END: loop over CppfDigis2
    //Matching information
    
  } // END: loop over CPPFDigis1
  
  h1_matches_unpacker->Fill(matches_unpacker);
  h1_matches_unpacker_bx->Fill(matches_unpacker_bx);
  h1_matches_unpacker_bx_phi->Fill(matches_unpacker_bx_phi);	
  
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
  
  h1_matches_unpacker = fs->make<TH1D>("h1_matches_unpacker", "CPPFDigis_matches_unpacker" , 5, 0. , 5.);
  h1_matches_unpacker_bx = fs->make<TH1D>("h1_matches_unpacker_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h1_matches_unpacker_bx_phi = fs->make<TH1D>("h1_matches_unpacker_bx_phi", "CPPFDigis_Matches_int" , 5, 0. , 5.);

  h1_bx_emulated = fs->make<TH1D>("h1_bx_emulated","Emulated bunch crossing",8, -4., 4.);
  h1_bx_unpacker = fs->make<TH1D>("h1_bx_unpacker","Unpacked bunch crossing",8, -4., 4.);
  h1_bx_diff_emu_unpacker = fs->make<TH1D>("h1_bx_diff_emu_unpacker","Difference of Bunch crossing emulator and unpacker",8,0,8.);
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
  h2_bx_occupancy_unpacker_bx_phi = fs->make<TH2D>("h2_bx_occupancy_unpacker_bx_phi","CPPFDigis h2_bx_occupancy_unpacker_bx_phi", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2_bx_occupancy_emu_unpacker_bx_phi = fs->make<TH2D>("h2_bx_occupancy_emu_unpacker_bx_phi","CPPFDigis h2_bx_occupancy_emu_unpacker_bx_phi", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2_bx_sector_unpacker_bx_phi = fs->make<TH2D>("h2_bx_sector_unpacker_bx_phi","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2_bx_sector_emu_unpacker_bx_phi = fs->make<TH2D>("h2_bx_sector_emu_unpacker_bx_phi","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);


  /// NEW HISTO FROM ANOTHER CODDEp

  h1CeVsCuPhiCePhiCuDiffOneHit = fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiffOneHit", "h1CeVsCuPhiCePhiCuDiffOneHit",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiffOneHit = fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiffOneHit", "h1CeVsCuThetaCeThetaCuDiffOneHit",  200, -100., 100.);
  h1CeVsCuBxCeBxCuDiffOneHit = fs->make<TH1D>("h1CeVsCuBxCeBxCuDiffOneHit","h1CeVsCuBxCeBxCuDiffOneHit", 8, -3.5, 4.5);
  h1CeVsCuPhiInDiagonalOneHit = fs->make<TH1D>("h1CeVsCuPhiInDiagonalOneHit", "CPPFDigis_h1CeVsCuPhiInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuPhiOffDiagonalOneHit = fs->make<TH1D>("h1CeVsCuPhiOffDiagonalOneHit", "CPPFDigis_h1CeVsCuPhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuThetaInDiagonalOneHit = fs->make<TH1D>("h1CeVsCuThetaInDiagonalOneHit", "CPPFDigis_h1CeVsCuThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuThetaOffDiagonalOneHit = fs->make<TH1D>("h1CeVsCuThetaOffDiagonalOneHit", "CPPFDigis_h1CeVsCuThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuPhiCePhiCuDiffOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiffOneHit_bx", "h1CeVsCuPhiCePhiCuDiffOneHit_bx",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiffOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiffOneHit_bx", "h1CeVsCuThetaCeThetaCuDiffOneHit_bx",  200, -100., 100.);
  h2CeVsCuThetaCeThetaCuOneHit_bx = fs->make<TH2D>("h2CeVsCuThetaCeThetaCuOneHit_bx", "h2CeVsCuThetaCeThetaCuOneHit_bx",  32, 0., 32., 32, 0., 32.);
  h1CeVsCuPhiInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiInDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuPhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhiInDiagonalNotOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiInDiagonalNotOneHit_bx", "CPPFDigis_h1CeVsCuPhiInDiagonalNotOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaInDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx", "h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx",  200, -100., 100.);
  h1CeVsCuPhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuPhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhiOffDiagonalNotOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiOffDiagonalNotOneHit_bx", "CPPFDigis_h1CeVsCuPhiOffDiagonalNotOneHit_bx" , 2, 0. , 2.);
  h2CeVsCuChamberCuChamberCeOneHit = fs->make<TH2D>("h2CeVsCuChamberCuChamberCeOneHit","h2CeVsCuChamberCuChamberCeOneHit", 36, 1.,37.,36,1.,37.);
  h2CeVsCuPhiCePhiCuOneHit = fs->make<TH2D>("h2CeVsCuPhiCePhiCuOneHit", "h2CeVsCuPhiCePhiCuOneHit",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuPhiCePhiCuOneHit_bx = fs->make<TH2D>("h2CeVsCuPhiCePhiCuOneHit_bx", "h2CeVsCuPhiCePhiCuOneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCuOneHit = fs->make<TH2D>("h2CeVsCuThetaCeThetaCuOneHit", "h2CeVsCuThetaCeThetaCuOneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuChamberCuZoneCuOneHit_bx = fs->make<TH2D>("h2CeVsCuChamberCuZoneCuOneHit_bx", "CPPFDigis h2CeVsCuChamberCuZoneCuOneHit_bx", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCeOneHit_bx = fs->make<TH2D>("h2CeVsCuChamberCeZoneCeOneHit_bx", "CPPFDigis h2CeVsCuChamberCeZoneCeOneHit_bx", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCuZoneCuOccupancyOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCeZoneCeOccupancyOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCuZoneCuOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOneHit","CPPFDigis Bx_unpackerOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCeOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOneHit","CPPFDigis Bx_emu_unpackerOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuChamberCuZoneCuNotOneHit = fs->make<TH2D>("h2CeVsCuChamberCuZoneCuNotOneHit", "CPPFDigis h2CeVsCuChamberCuZoneCuNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCeNotOneHit = fs->make<TH2D>("h2CeVsCuChamberCeZoneCeNotOneHit", "CPPFDigis h2CeVsCuChamberCeZoneCeNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalNotOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalNotOneHit","CPPFDigis Bx_Occupancy_emu_unpackerNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalNotOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalNotOneHit","CPPFDigis Bx_Occupancy_unpackerNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalNotOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalNotOneHit","CPPFDigis Bx_Occupancy_emu_unpackerNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalNotOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalNotOneHit","CPPFDigis Bx_Occupancy_unpackerNotOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);
