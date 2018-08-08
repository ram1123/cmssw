#include "EMTF_CPPF_DQM.h"
#include <sstream>
#include <map>
///////////////////////////////////////////////
//  u: unpacker
//  e: emulator
//  E: EMTF 
//  C: CPPF
///////////////////////////////////////////////  

EMTF_CPPF_DQM::EMTF_CPPF_DQM(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),
  cppfDigiToken3_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel3"))),
  cppfDigiToken4_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel4"))) {
}

EMTF_CPPF_DQM::~EMTF_CPPF_DQM(){
}

void EMTF_CPPF_DQM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){  
  
  //Get the CPPFDigi 
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis1;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis2;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis3;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis4;
  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);
  iEvent.getByToken(cppfDigiToken3_, CppfDigis3);
  iEvent.getByToken(cppfDigiToken4_, CppfDigis4);
  
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
    //int fillBxCe = bx_value(regionCe, emtfSectorCe); 
    
    //Chamber ID
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);

    std::ostringstream oss;
    oss << regionCe << stationCe << ringCe << sectorCe << subsectorCe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;
    if ( _nHit_Ce.find(unique_id) == _nHit_Ce.end() ) { // chamber had no hit so far
      _nHit_Ce.insert({unique_id, 1});
      _phi_Ce.insert({unique_id,phiIntCe});
      _theta_Ce.insert({unique_id,thetaIntCe});
      _ID_Ce.insert({unique_id, chamberIDCe});
      _zone_Ce.insert({unique_id,fillOccupancyCe});
      _emtfSubsector_Ce.insert({unique_id,emtfSubsectorCe});
      _emtfSector_Ce.insert({unique_id,emtfSectorCe});
      _bx_Ce.insert({unique_id,bxCe});
    }
    else {
      _nHit_Ce.at(unique_id) += 1;
    }
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
        if (it.second == 1 && _nHit_Cu.at(key) == 1) {
          h2CeVsCuChamberCeChamberCuOneHit->Fill(_ID_Ce.at(key), _ID_Cu.at(key)); 
	        h2CeVsCuPhiCePhiCuOneHit->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	        h2CeVsCuThetaCeThetaCuOneHit->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
          // Occupancy 
	        h2CeVsCuChamberCuZoneCuOneHit->Fill(_emtfSubsector_Cu.at(key), _zone_Cu.at(key));
	        h2CeVsCuChamberCeZoneCeOneHit->Fill(_emtfSubsector_Ce.at(key), _zone_Ce.at(key));
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

          //if ((_bx_Ce.at(key) - _bx_Cu.at(key)) == 0){
          //} 

          if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) h1CeVsCuPhiInDiagonalOneHit->Fill(0.);
          else h1CeVsCuPhiOffDiagonalOneHit->Fill(0.);
          if ((_theta_Ce.at(key) - _theta_Cu.at(key)) == 0) h1CeVsCuThetaInDiagonalOneHit->Fill(0.);
          else h1CeVsCuThetaOffDiagonalOneHit->Fill(0.);
          if (_bx_Ce.at(key) == 0 && _bx_Cu.at(key) == 0){
            h1CeVsCuPhiCePhiCuDiffOneHit_bx->Fill(_phi_Ce.at(key) - _phi_Cu.at(key));
            h1CeVsCuThetaCeThetaCuDiffOneHit_bx->Fill(_theta_Ce.at(key) - _theta_Cu.at(key));
	          h2CeVsCuPhiCePhiCuOneHit_bx->Fill(_phi_Cu.at(key), _phi_Ce.at(key));
	          h2CeVsCuThetaCeThetaCuOneHit_bx->Fill(_theta_Cu.at(key), _theta_Ce.at(key)); 
            if ((_phi_Ce.at(key) - _phi_Cu.at(key)) == 0) h1CeVsCuPhiInDiagonalOneHit_bx->Fill(0.);
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
      }
  }
  








  for(auto& cppf_digis : *CppfDigis4){

    RPCDetId rpcIdEe = (int)cppf_digis.rpcId();
    int regionEe = (int)rpcIdEe.region();
    int stationEe = (int)rpcIdEe.station();
    int ringEe = (int)rpcIdEe.ring();
    int sectorEe = (int)rpcIdEe.sector();
    int subsectorEe = (int)rpcIdEe.subsector();
    int phiIntEe = (int)cppf_digis.phi_int();
    int thetaIntEe =  (int)cppf_digis.theta_int();
    int bxEe = cppf_digis.bx();
    
    int emtfSectorEe = (int)cppf_digis.emtf_sector();
    int emtfSubsectorEe = GetSubsector(emtfSectorEe, subsectorEe);
    int fillOccupancyEe = occupancy_value(regionEe, stationEe, ringEe);
    //int fillBxEe = bx_value(regionEe, emtfSectorEe); 
    
    //Chamber ID
    int nsubEe = 6;
    (ringEe == 1 && stationEe > 1) ? nsubEe = 3 : nsubEe = 6;
    int chamberIDEe = subsectorEe + nsubEe * ( sectorEe - 1);

    std::ostringstream oss;
    oss << regionEe << stationEe << ringEe << sectorEe << subsectorEe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;
    if ( _nHit_Ee.find(unique_id) == _nHit_Ee.end() ) { // chamber had no hit so far
      _nHit_Ee.insert({unique_id, 1});
      _phi_Ee.insert({unique_id,phiIntEe});
      _theta_Ee.insert({unique_id,thetaIntEe});
      _ID_Ee.insert({unique_id, chamberIDEe});
      _zone_Ee.insert({unique_id,fillOccupancyEe});
      _emtfSubsector_Ee.insert({unique_id,emtfSubsectorEe});
      _emtfSector_Ee.insert({unique_id,emtfSectorEe});
      _bx_Ee.insert({unique_id,bxEe});
    }
    else {
      _nHit_Ee.at(unique_id) += 1;
    }
  }

  for(auto& cppf_digis2 : *CppfDigis3){

    RPCDetId rpcIdEu = cppf_digis2.rpcId();
    int regionEu = rpcIdEu.region();
    int stationEu = rpcIdEu.station();
    int ringEu = rpcIdEu.ring();
    int sectorEu = rpcIdEu.sector();
    int subsectorEu = rpcIdEu.subsector();
    int phiIntEu = cppf_digis2.phi_int();
    int thetaIntEu =  cppf_digis2.theta_int();
    int bxEu = cppf_digis2.bx();
    
    int emtfSectorEu = (int)cppf_digis2.emtf_sector();
    int emtfSubsectorEu = GetSubsector(emtfSectorEu, subsectorEu);
    int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
    //int fillBxEu = bx_value(regionEu, emtfSectorEu); 
    
    //Chamber ID
    int nsubEu = 6;
    (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
    int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);

    std::ostringstream oss2;
    oss2 << regionEu << stationEu << ringEu << sectorEu << subsectorEu;
    std::istringstream iss2(oss2.str());
    int unique_id;
    iss2 >> unique_id;
    if ( _nHit_Eu.find(unique_id) == _nHit_Eu.end() ) { // chamber had no hit so far
      _nHit_Eu.insert({unique_id, 1});
      _phi_Eu.insert({unique_id,phiIntEu});
      _theta_Eu.insert({unique_id,thetaIntEu});
      _ID_Eu.insert({unique_id, chamberIDEu});
      _zone_Eu.insert({unique_id,fillOccupancyEu});
      _emtfSubsector_Eu.insert({unique_id,emtfSubsectorEu});
      _emtfSector_Eu.insert({unique_id,emtfSectorEu});
      _bx_Eu.insert({unique_id,bxEu});
    }
    else {
      _nHit_Eu.at(unique_id) += 1;
    }
  }
  for (auto const& it : _nHit_Ee){
      int key = it.first;
      if (_nHit_Eu.find(key) != _nHit_Eu.end()){
        if (it.second == 1 && _nHit_Eu.at(key) == 1) {
          h2EeVsEuChamberEeChamberEuOneHit->Fill(_ID_Ee.at(key), _ID_Eu.at(key)); 
	        h2EeVsEuPhiEePhiEuOneHit->Fill(_phi_Eu.at(key), _phi_Ee.at(key));
	        h2EeVsEuThetaEeThetaEuOneHit->Fill(_theta_Eu.at(key), _theta_Ee.at(key)); 
          // Occupancy 
	        h2EeVsEuChamberEuZoneEuOneHit->Fill(_emtfSubsector_Eu.at(key), _zone_Eu.at(key));
	        h2EeVsEuChamberEeZoneEeOneHit->Fill(_emtfSubsector_Ee.at(key), _zone_Ee.at(key));
          // Bx by zone
          h2EeVsEuBxEuZoneEuOccupancyOneHit->Fill(_bx_Eu.at(key), _zone_Eu.at(key));
          h2EeVsEuBxEeZoneEeOccupancyOneHit->Fill(_bx_Ee.at(key), _zone_Ee.at(key));
          // Bx by EMTF sector
          h2EeVsEuBxEuZoneEuOneHit->Fill(_emtfSector_Eu.at(key), _bx_Eu.at(key));
          h2EeVsEuBxEeZoneEeOneHit->Fill(_emtfSector_Ee.at(key), _bx_Ee.at(key));
          //Difference between variables
          h1EeVsEuPhiEePhiEuDiffOneHit->Fill(_phi_Ee.at(key) - _phi_Eu.at(key));
          h1EeVsEuThetaEeThetaEuDiffOneHit->Fill(_theta_Ee.at(key) - _theta_Eu.at(key));
          h1EeVsEuBxEeBxEuDiffOneHit->Fill(_bx_Ee.at(key) - _bx_Eu.at(key));

          //if ((_bx_Ee.at(key) - _bx_Eu.at(key)) == 0){
          //} 

          if ((_phi_Ee.at(key) - _phi_Eu.at(key)) == 0) h1EeVsEuPhiInDiagonalOneHit->Fill(0.);
          else h1EeVsEuPhiOffDiagonalOneHit->Fill(0.);
          if ((_theta_Ee.at(key) - _theta_Eu.at(key)) == 0) h1EeVsEuThetaInDiagonalOneHit->Fill(0.);
          else h1EeVsEuThetaOffDiagonalOneHit->Fill(0.);
          if (_bx_Ee.at(key) == 0 && _bx_Eu.at(key) == 0){
            h1EeVsEuPhiEePhiEuDiffOneHit_bx->Fill(_phi_Ee.at(key) - _phi_Eu.at(key));
            h1EeVsEuThetaEeThetaEuDiffOneHit_bx->Fill(_theta_Ee.at(key) - _theta_Eu.at(key));
	          h2EeVsEuPhiEePhiEuOneHit_bx->Fill(_phi_Eu.at(key), _phi_Ee.at(key));
	          h2EeVsEuThetaEeThetaEuOneHit_bx->Fill(_theta_Eu.at(key), _theta_Ee.at(key)); 
            if ((_phi_Ee.at(key) - _phi_Eu.at(key)) == 0) h1EeVsEuPhiInDiagonalOneHit_bx->Fill(0.);
            else {
              h1EeVsEuPhiOffDiagonalOneHit_bx->Fill(0.);
              h2EeVsEuBxEeZoneEeOccupancyOffPhiDiagonalOneHit->Fill(_emtfSubsector_Ee.at(key), _zone_Ee.at(key));
              h2EeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit->Fill(_emtfSubsector_Eu.at(key), _zone_Eu.at(key));
            }
            if ((_theta_Ee.at(key) - _theta_Eu.at(key)) == 0) h1EeVsEuThetaInDiagonalOneHit_bx->Fill(0.);
            else {
              h1EeVsEuThetaOffDiagonalOneHit_bx->Fill(0.);
              h2EeVsEuBxEeZoneEeOccupancyOffThetaDiagonalOneHit->Fill(_emtfSubsector_Ee.at(key), _zone_Ee.at(key));
              h2EeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit->Fill(_emtfSubsector_Eu.at(key), _zone_Eu.at(key));
            }
            if ((_theta_Ee.at(key) == _theta_Eu.at(key)) && (_phi_Ee.at(key) != _phi_Eu.at(key))) h1EeVsEuThetaPhiEeThetaPhiEuDiffOneHit_bx->Fill(_phi_Ee.at(key) - _phi_Eu.at(key));
          }
        }
      }
  }






  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcIdCe = (int)cppf_digis.rpcId();
    int regionCe = (int)rpcIdCe.region();
    //int rollCe = (int)rpcIdCe.roll();
    //int layerCe = (int)rpcIdCe.layer();
    int stationCe = (int)rpcIdCe.station();
    int ringCe = (int)rpcIdCe.ring();
    int sectorCe = (int)rpcIdCe.sector();
    int subsectorCe = (int)rpcIdCe.subsector();
    int phiIntCe = (int)cppf_digis.phi_int();
    int thetaIntCe =  (int)cppf_digis.theta_int();
    
    int emtfSectorCe = (int)cppf_digis.emtf_sector();
    int emtfSubsectorCe = GetSubsector(emtfSectorCe, subsectorCe);
    
    int bxCe = cppf_digis.bx();
    
    int fillOccupancyCe = occupancy_value(regionCe, stationCe, ringCe);
    int fillBxCe = bx_value(regionCe, emtfSectorCe); 
    
    //Chamber ID
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);
    
    
    int matchesCu = 0;  
    int matchesCu_bx = 0;
    int matchesCu_int = 0;
    int phiIntCu = 0;
    int thetaIntCu = 0;
    for(auto& cppf_digis_unpacker : *CppfDigis2){
      RPCDetId rpcIdCu = (int)cppf_digis_unpacker.rpcId();
      int regionCu = (int)rpcIdCu.region();
      //int rollCu = (int)rpcIdCu.roll();
      //int layerCu = (int)rpcIdCu.layer();
      int stationCu = (int)rpcIdCu.station();
      int ringCu = (int)rpcIdCu.ring();
      int sectorCu = (int)rpcIdCu.sector();
      int subsectorCu = (int)rpcIdCu.subsector();
      int bxCu = (int)cppf_digis_unpacker.bx();
      
      //int emtfSectorCu = (int)cppf_digis_unpacker.emtf_sector();
      int emtfSubsectorCu = GetSubsector(sectorCu, subsectorCu);
      //std::cout << sectorCu << "   " << subsectorCu << "   "<< "   " << GetSubsector(sectorCu, subsectorCu) << "    " << emtfSubsectorCu << std::endl;

      //Chamber ID
      int nsubCu = 6;
      (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
      int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);
      
      int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
      int fillBxCu = bx_value(regionCu, sectorCu);
      
      if((regionCe == regionCu) && 
	        (stationCe == stationCu) &&
	        (ringCe == ringCu) &&
	        (sectorCe == sectorCu) &&
	        (subsectorCe == subsectorCu)){
	
        phiIntCu = (int)cppf_digis_unpacker.phi_int();
        thetaIntCu =  (int)cppf_digis_unpacker.theta_int();
	      matchesCu++;
        h1CeVsCuMatches_ch->Fill(matchesCu);
	
        h2CeVsCuChamberCeChamberCu->Fill(chamberIDCe, chamberIDCu); 
        h2CeVsCuPhiCePhiCu->Fill(phiIntCe, phiIntCu);
        h1CeVsCuPhiCePhiCuDiff->Fill(phiIntCe - phiIntCu);
        h2CeVsCuThetaCeThetaCu->Fill(thetaIntCe, thetaIntCu);
	      if((phiIntCe == phiIntCu) && (thetaIntCe == thetaIntCu)){
          h2CeVsCuBxCeBxCu->Fill(bxCe, bxCu);
          h1CeVsCuBxCeBxCuDiff->Fill(bxCe - bxCu);
        }
        
        
        if(bxCe ==  bxCu){
	        matchesCu_bx++;
	        h1CeVsCuMatches_bx->Fill(matchesCu_bx);	
	        h2CeVsCuPhiCePhiCu_bx->Fill(phiIntCe, phiIntCu);
	        h2CeVsCuThetaCeThetaCu_bx->Fill(thetaIntCe, thetaIntCu); 
	        h2CeVsCuBxCeBxCu_bx->Fill(bxCe, bxCu);
	  
	        if((phiIntCe == phiIntCu) && (thetaIntCe == thetaIntCu)){
	          matchesCu_int++;
	          h1CeVsCuMatches_int->Fill(matchesCu_int);	
	          h2CeVsCuPhiCePhiCu_int->Fill(phiIntCe, phiIntCu);
	          h2CeVsCuThetaCeThetaCu_int->Fill(thetaIntCe, thetaIntCu); 
            //Occupancy 
	          h2CeVsCuChamberCuZoneCu->Fill(emtfSubsectorCu, fillOccupancyCu);
	          h2CeVsCuChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe);
            //bx occupancy 
            h2CeVsCuBxCuZoneCuOccupancy->Fill(bxCu, fillOccupancyCu);
            h2CeVsCuBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsCuBxCuZoneCu->Fill(fillBxCu, bxCu);
            h2CeVsCuBxCeZoneCe->Fill(fillBxCe, bxCe);
	        } 
	      }
      }
    }
    //Matching information
    h1CeVsCuMatches->Fill(matchesCu);
    int matchesEu = 0;
    int matchesEu_bx = 0; 
    int matchesEu_int = 0; 
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = cppf_digis_emtf.rpcId();
      int regionEu = (int)rpcIdEu.region();
      int stationEu = (int)rpcIdEu.station();
      int ringEu = (int)rpcIdEu.ring();
      int sectorEu = (int)rpcIdEu.sector();
      int subsectorEu = (int)rpcIdEu.subsector();
      int phiIntEu = (int)cppf_digis_emtf.phi_int();
      int thetaIntEu =  (int)cppf_digis_emtf.theta_int();
      int bxEu = (int)cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      int emtfSectorEu = (int)cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(emtfSectorEu,subsectorEu);
      
      if((phiIntCe == phiIntEu) && (thetaIntCe == thetaIntEu) && (regionCe == regionEu)){
	      h2CeVsEuChamberCeChamberEu_test->Fill(chamberIDCe, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      if((regionCe == regionEu) && 
	        (stationCe == stationEu) &&
	        (ringCe == ringEu) &&
	        (sectorCe == sectorEu) &&
	        (subsectorCe == subsectorEu)){
     /* 
      if((chamberIDCe == chamberIDEu) && (regionCe == regionEu) 
         && (ringCe == ringEu) && (stationCe == stationEu) && (regionCe == regionEu) 
          ){
     */
	      matchesEu++;
        h1CeVsEuMatches_ch->Fill(matchesEu);
	
        h2CeVsEuChamberCeChamberEu->Fill(chamberIDCe, chamberIDEu); 
	
        h2CeVsEuPhiCePhiEu->Fill(phiIntCe, phiIntEu);
        h1CeVsEuPhiCePhiEuDiff->Fill(phiIntCe - phiIntEu);
        h2CeVsEuThetaCeThetaEu->Fill(thetaIntCe, thetaIntEu);
	      if((phiIntCe == phiIntEu) && (thetaIntCe == thetaIntEu)){
          h2CeVsEuBxCeBxEu->Fill(bxCe, bxEu);
          h1CeVsEuBxCeBxEuDiff->Fill(bxCe - bxEu);
        }
	
        if(bxCe ==  bxEu){
	        matchesEu_bx++;
	        h1CeVsEuMatches_bx->Fill(matchesEu_bx);	    
	        h2CeVsEuPhiCePhiEu_bx->Fill(phiIntCe, phiIntEu);
	        h2CeVsEuThetaCeThetaEu_bx->Fill(thetaIntCe, thetaIntEu); 
	        h2CeVsEuBxCeBxEu_bx->Fill(bxCe, bxEu);
	        if((phiIntCe == phiIntEu) && (thetaIntCe == thetaIntEu)){
	          matchesEu_int++;
	          h1CeVsEuMatches_int->Fill(matchesEu_int);
	          h2CeVsEuPhiCePhiEu_int->Fill(phiIntCe, phiIntCe);
	          h2CeVsEuThetaCeThetaEu_int->Fill(thetaIntCe, thetaIntEu);
 
            //Occupancy
	          h2CeVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu);
	          h2CeVsEuChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe); 
            //bx occupancy 
            h2CeVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            h2CeVsEuBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
            h2CeVsEuBxCeZoneCe->Fill(fillBxCe,bxCe);
	        }	    
        }
      }
    }
    //Matching information
    h1CeVsEuMatches->Fill(matchesEu);
    int matchesEe = 0;
    int matchesEe_bx = 0; 
    int matchesEe_int = 0; 
    
    for(auto& cppf_digis_emtf : *CppfDigis4){
      
      RPCDetId rpcIdEe = (int)cppf_digis_emtf.rpcId();
      int regionEe = (int)rpcIdEe.region();
      int stationEe = (int)rpcIdEe.station();
      int ringEe = (int)rpcIdEe.ring();
      int sectorEe = (int)rpcIdEe.sector();
      int subsectorEe = (int)rpcIdEe.subsector();
      int phiIntEe = (int)cppf_digis_emtf.phi_int();
      int thetaIntEe =  (int)cppf_digis_emtf.theta_int();
      int bxEe = (int)cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEe = 6;
      (ringEe == 1 && stationEe > 1) ? nsubEe = 3 : nsubEe = 6;
      int chamberIDEe = subsectorEe + nsubEe * ( sectorEe - 1);
      
      int emtfSectorEe = (int)cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEe = GetSubsector(emtfSectorEe,subsectorEe);
      
      if((phiIntCe == phiIntEe) && (thetaIntCe == thetaIntEe) && (regionCe == regionEe)){
	      h2CeVsEeChamberCeChamberEe_test->Fill(chamberIDCe, chamberIDEe);
      }
      int fillOccupancyEe = occupancy_value(regionEe, stationEe, ringEe);
      int fillBxEe = bx_value(regionEe, emtfSectorEe);

      if((regionCe == regionEe) && 
	        (stationCe == stationEe) &&
	        (ringCe == ringEe) &&
	        (sectorCe == sectorEe) &&
	        (subsectorCe == subsectorEe)){
     /* 
      if((chamberIDCe == chamberIDEe) && (regionCe == regionEe) 
         && (ringCe == ringEe) && (stationCe == stationEe) 
          ){
      */
	      matchesEe++;
        h1CeVsEeMatches_ch->Fill(matchesEe);
	
        h2CeVsEeChamberCeChamberEe->Fill(chamberIDCe, chamberIDEe); 
	
        h2CeVsEePhiCePhiEe->Fill(phiIntCe, phiIntEe);
        h1CeVsEePhiCePhiEeDiff->Fill(phiIntCe - phiIntEe);
        h2CeVsEeThetaCeThetaEe->Fill(thetaIntCe, thetaIntEe);
	        
        if((phiIntCe == phiIntEe) && (thetaIntCe == thetaIntEe)){
          h2CeVsEeBxCeBxEe->Fill(bxCe, bxEe);
          h1CeVsEeBxCeBxEeDiff->Fill(bxCe - bxEe);
        }

	
        if(bxCe ==  bxEe){
	        matchesEe_bx++;
	        h1CeVsEeMatches_bx->Fill(matchesEe_bx);	    
	        h2CeVsEePhiCePhiEe_bx->Fill(phiIntCe, phiIntEe);
	        h2CeVsEeThetaCeThetaEe_bx->Fill(thetaIntCe, thetaIntEe); 
	        h2CeVsEeBxCeBxEe_bx->Fill(bxCe, bxEe);
	        if((phiIntCe == phiIntEe) && (thetaIntCe == thetaIntEe)){
	          matchesEe_int++;
	          h1CeVsEeMatches_int->Fill(matchesEe_int);
	          h2CeVsEePhiCePhiEe_int->Fill(phiIntCe, phiIntEe);
	          h2CeVsEeThetaCeThetaEe_int->Fill(thetaIntCe, thetaIntEe);
 
            //Occupancy
	          h2CeVsEeChamberEeZoneEe->Fill(emtfSubsectorEe, fillOccupancyEe);
	          h2CeVsEeChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe); 
            //bx occupancy 
            h2CeVsEeBxEeZoneEeOccupancy->Fill(bxEe, fillOccupancyEe);
            h2CeVsEeBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsEeBxEeZoneEe->Fill(fillBxEe,bxEe);
            h2CeVsEeBxCeZoneCe->Fill(fillBxCe,bxCe);
	        }	    
        }
      }
    }
    //Matching information
    h1CeVsEeMatches->Fill(matchesEe);
  } // loop over CPPFDigis
  
  //Comparison between CPPF and EMTF
    
  for(auto& cppf_digis_unpacker : *CppfDigis2){
    RPCDetId rpcIdCu = (int)cppf_digis_unpacker.rpcId();
    int regionCu = (int)rpcIdCu.region();
    int stationCu = (int)rpcIdCu.station();
    int ringCu = (int)rpcIdCu.ring();
    int sectorCu = (int)rpcIdCu.sector();
    int subsectorCu = (int)rpcIdCu.subsector();
    int phiIntCu = (int)cppf_digis_unpacker.phi_int();
    int thetaIntCu =  (int)cppf_digis_unpacker.theta_int();
    int bxCu = (int)cppf_digis_unpacker.bx();
    
    //int emtfSectorCu = (int)cppf_digis_unpacker.emtf_sector();
    int emtfSubsectorCu = GetSubsector(sectorCu, subsectorCu);

    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);
    
    int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
    int fillBxCu = bx_value(regionCu, sectorCu);
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = (int)cppf_digis_emtf.rpcId();
      int regionEu = (int)rpcIdEu.region();
      int stationEu = (int)rpcIdEu.station();
      int ringEu = (int)rpcIdEu.ring();
      int sectorEu = (int)rpcIdEu.sector();
      int subsectorEu = (int)rpcIdEu.subsector();
      int phiIntEu = (int)cppf_digis_emtf.phi_int();
      int thetaIntEu =  (int)cppf_digis_emtf.theta_int();
      int bxEu = (int)cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      if((phiIntCu == phiIntEu) && (thetaIntCu == thetaIntEu) && (regionCu == regionEu)){
	      h2CuVsEuChamberCuChamberEu_test->Fill(chamberIDCu, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int emtfSectorEu = (int)cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(emtfSectorEu, subsectorEu);

      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      if((regionCu == regionEu) && 
	        (stationCu == stationEu) &&
	        (ringCu == ringEu) &&
	        (sectorCu == sectorEu) &&
	        (subsectorCu == subsectorEu)){
     /* 
      if((chamberIDCu == chamberIDEu)  
         && (ringCu == ringEu) && (stationCu == stationEu) && (regionCu == regionEu) 
          ){
     */
	
        h2CuVsEuChamberCuChamberEu->Fill(chamberIDCu, chamberIDEu); 
        
        if (abs(phiIntCu - phiIntEu) > 500.)  {
          h2CuVsEuChamberCuChamberEuBox->Fill(chamberIDCu, chamberIDEu); 
	        h2CuVsEuChamberCuZoneCuBox->Fill(emtfSubsectorCu, fillOccupancyCu); 
	        h2CuVsEuChamberEuZoneEuBox->Fill(emtfSubsectorEu, fillOccupancyEu); 
        }
	
        h2CuVsEuPhiCuPhiEu->Fill(phiIntCu, phiIntEu);
        h1CuVsEuPhiCuPhiEuDiff->Fill(phiIntCu - phiIntEu);
        h2CuVsEuThetaCuThetaEu->Fill(thetaIntCu, thetaIntEu);
	      if((phiIntCu == phiIntEu) && (thetaIntCu == thetaIntEu)){
          h2CuVsEuBxCuBxEu->Fill(bxCu,bxEu);
          h1CuVsEuBxCuBxEuDiff->Fill(bxCu-bxEu);
        }
	
        if(bxCu ==  bxEu){
	        h2CuVsEuPhiCuPhiEu_bx->Fill(phiIntCu, phiIntEu);
	        h2CuVsEuThetaCuThetaEu_bx->Fill(thetaIntCu, thetaIntEu); 
	        h2CuVsEuBxCuBxEu_bx->Fill(bxCu,bxEu);
	        if((phiIntCu == phiIntEu) && (thetaIntCu == thetaIntEu)){
	          h2CuVsEuPhiCuPhiEu_int->Fill(phiIntCu, phiIntEu);
	          h2CuVsEuThetaCuThetaEu_int->Fill(thetaIntCu, thetaIntEu);
	          //occupancy 
            h2CuVsEuChamberCuZoneCu->Fill(emtfSubsectorCu, fillOccupancyCu); 
	          h2CuVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu); 
            //bx occupancy 
            h2CuVsEuBxCuZoneCuOccupancy->Fill(bxCu, fillOccupancyEu);
            h2CuVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            //bx
            h2CuVsEuBxCuZoneCu->Fill(fillBxCu,bxCu);
            h2CuVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
	        }	    
        }
      }
    }
  } // loop over CPPFDigis





  for(auto& cppf_digis_unpacker : *CppfDigis4){
    RPCDetId rpcIdEe = (int)cppf_digis_unpacker.rpcId();
    int regionEe = (int)rpcIdEe.region();
    int stationEe = (int)rpcIdEe.station();
    int ringEe = (int)rpcIdEe.ring();
    int sectorEe = (int)rpcIdEe.sector();
    int subsectorEe = (int)rpcIdEe.subsector();
    int phiIntEe = (int)cppf_digis_unpacker.phi_int();
    int thetaIntEe =  (int)cppf_digis_unpacker.theta_int();
    int bxEe = (int)cppf_digis_unpacker.bx();
    
    int emtfSectorEe = (int)cppf_digis_unpacker.emtf_sector();
    int emtfSubsectorEe = GetSubsector(emtfSectorEe, subsectorEe);

    //Chamber ID
    int nsubEe = 6;
    (ringEe == 1 && stationEe > 1) ? nsubEe = 3 : nsubEe = 6;
    int chamberIDEe = subsectorEe + nsubEe * ( sectorEe - 1);
    
    int fillOccupancyEe = occupancy_value(regionEe, stationEe, ringEe);
    int fillBxEe = bx_value(regionEe, emtfSectorEe);
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = (int)cppf_digis_emtf.rpcId();
      int regionEu = (int)rpcIdEu.region();
      int stationEu = (int)rpcIdEu.station();
      int ringEu = (int)rpcIdEu.ring();
      int sectorEu = (int)rpcIdEu.sector();
      int subsectorEu = (int)rpcIdEu.subsector();
      int phiIntEu = (int)cppf_digis_emtf.phi_int();
      int thetaIntEu =  (int)cppf_digis_emtf.theta_int();
      int bxEu = (int)cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      if((phiIntEe == phiIntEu) && (thetaIntEe == thetaIntEu) && (regionEe == regionEu)){
	      h2EeVsEuChamberEeChamberEu_test->Fill(chamberIDEe, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int emtfSectorEu = (int)cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(emtfSectorEu, subsectorEu);

      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      if((regionEe == regionEu) && 
	        (stationEe == stationEu) &&
	        (ringEe == ringEu) &&
	        (sectorEe == sectorEu) &&
	        (subsectorEe == subsectorEu)){
      /*
      if((chamberIDEe == chamberIDEu)  
         && (ringEe == ringEu) && (stationEe == stationEu) && (regionEe == regionEu) 
          ){
	*/
        h2EeVsEuChamberEeChamberEu->Fill(chamberIDEe, chamberIDEu); 
	
        h2EeVsEuPhiEePhiEu->Fill(phiIntEe, phiIntEu);
        h1EeVsEuPhiEePhiEuDiff->Fill(phiIntEe - phiIntEu);
        h2EeVsEuThetaEeThetaEu->Fill(thetaIntEe, thetaIntEu);
	      if((phiIntEe == phiIntEu) && (thetaIntEe == thetaIntEu)){
          h2EeVsEuBxEeBxEu->Fill(bxEe,bxEu);
          h1EeVsEuBxEeBxEuDiff->Fill(bxEe-bxEu);
        }
	      if(thetaIntEe == thetaIntEu){
          h1CeVsCuInDiagonal->Fill(0.);
        }
        else{
          h1CeVsCuOffDiagonal->Fill(0.);
        }
	
        if(bxEe ==  bxEu){
	        h2EeVsEuPhiEePhiEu_bx->Fill(phiIntEe, phiIntEu);
	        h2EeVsEuThetaEeThetaEu_bx->Fill(thetaIntEe, thetaIntEu); 
	        h2EeVsEuBxEeBxEu_bx->Fill(bxEe,bxEu);
	        if((phiIntEe == phiIntEu) && (thetaIntEe == thetaIntEu)){
	          h2EeVsEuPhiEePhiEu_int->Fill(phiIntEe, phiIntEu);
	          h2EeVsEuThetaEeThetaEu_int->Fill(thetaIntEe, thetaIntEu);
 
	          h2EeVsEuChamberEeZoneEe->Fill(emtfSubsectorEe, fillOccupancyEe); 
	          h2EeVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu); 
            //bx occupancy 
            h2EeVsEuBxEeZoneEeOccupancy->Fill(bxEe, fillOccupancyEu);
            h2EeVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            //bx
            h2EeVsEuBxEeZoneEe->Fill(fillBxEe,bxEe);
            h2EeVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
	        }	    
        }
      }
    }
  } // loop over CPPFDigis
} //End class

int EMTF_CPPF_DQM::occupancy_value(int region_, int station_, int ring_){
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

int EMTF_CPPF_DQM::GetSubsector(int emtfsector_, int lsubsector_){
  const int nsectors = 6;
  int gsubsector = 0;
  if ((emtfsector_ != -99) and (lsubsector_ != 0)){
    gsubsector = (emtfsector_ - 1)*nsectors + lsubsector_;
  }
  return gsubsector;
}

int EMTF_CPPF_DQM::bx_value(int region_, int emtfsector_){
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


void EMTF_CPPF_DQM::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
}

void EMTF_CPPF_DQM::beginJob(){
  edm::Service<TFileService> fs;
  
  h1CeVsCuMatches = fs->make<TH1D>("h1CeVsCuMatches", "CPPFDigis_h1CeVsCuMatches" , 5, 0. , 5.);
  h1CeVsCuOffDiagonal = fs->make<TH1D>("h1CeVsCuOffDiagonal", "CPPFDigis_h1CeVsCuOffDiagonal" , 2, 0. , 2.);
  h1CeVsCuInDiagonal = fs->make<TH1D>("h1CeVsCuInDiagonal", "CPPFDigis_h1CeVsCuInDiagonal" , 2, 0. , 2.);
  
  h1CeVsCuMatches_ch = fs->make<TH1D>("h1CeVsCuMatches_ch", "CPPFDigis_h1CeVsCuMatches_ch" , 5, 0. , 5.);
  h2CeVsCuChamberCeChamberCu = fs->make<TH2D>("h2CeVsCuChamberCeChamberCu","h2CeVsCuChamberCeChamberCu", 36, 1.,37.,36,1.,37.);
  h2CeVsCuPhiCePhiCu=fs->make<TH2D>("h2CeVsCuPhiCePhiCu", "h2CeVsCuPhiCePhiCu",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsCuPhiCePhiCuDiff=fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiff", "h1CeVsCuPhiCePhiCuDiff",  200, -100., 100.);
  h2CeVsCuThetaCeThetaCu=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu", "h2CeVsCuThetaCeThetaCu",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCu = fs->make<TH2D>("h2CeVsCuBxCeBxCu","h2CeVsCuBxCeBxCu", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsCuBxCeBxCuDiff = fs->make<TH1D>("h1CeVsCuBxCeBxCuDiff","h1CeVsCuBxCeBxCuDiff", 8, -3.5, 4.5);
  
  h1CeVsCuMatches_bx = fs->make<TH1D>("h1CeVsCuMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsCuPhiCePhiCu_bx=fs->make<TH2D>("h2CeVsCuPhiCePhiCu_bx", "h2CeVsCuPhiCePhiCu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCu_bx=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_bx", "h2CeVsCuThetaCeThetaCu_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCu_bx = fs->make<TH2D>("h2CeVsCuBxCeBxCu_bx","h2CeVsCuBxCeBxCu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsCuMatches_int = fs->make<TH1D>("h1CeVsCuMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsCuPhiCePhiCu_int=fs->make<TH2D>("h2CeVsCuPhiCePhiCu_int", "h2CeVsCuPhiCePhiCu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCu_int=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_int", "h2CeVsCuThetaCeThetaCu_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuChamberCuZoneCu = fs->make<TH2D>("h2CeVsCuChamberCuZoneCu", "CPPFDigis h2CeVsCuChamberCuZoneCu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCe = fs->make<TH2D>("h2CeVsCuChamberCeZoneCe", "CPPFDigis h2CeVsCuChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCuZoneCuOccupancy = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancy","CPPFDigis Bx_Occupancy_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsCuBxCeZoneCe","CPPFDigis Bx_Occupancy_emu_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCuZoneCu = fs->make<TH2D>("h2CeVsCuBxCuZoneCu","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCe = fs->make<TH2D>("h2CeVsCuBxCeZoneCe","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);


  h1CeVsEuMatches = fs->make<TH1D>("h1CeVsEuMatches", "CPPFDigis_h1CeVsEuMatches" , 5, 0. , 5.);
  
  h1CeVsEuMatches_ch = fs->make<TH1D>("h1CeVsEuMatches_ch", "CPPFDigis_Matches_ch" , 5, 0. , 5.);
  h2CeVsEuChamberCeChamberEu_test = fs->make<TH2D>("h2CeVsEuChamberCeChamberEu_test","h2CeVsEuChamberCeChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2CeVsEuChamberCeChamberEu = fs->make<TH2D>("h2CeVsEuChamberCeChamberEu","h2CeVsEuChamberCeChamberEu", 36, 1.,37.,36,1.,37.);
  h2CeVsEuPhiCePhiEu=fs->make<TH2D>("h2CeVsEuPhiCePhiEu", "h2CeVsEuPhiCePhiEu",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsEuPhiCePhiEuDiff=fs->make<TH1D>("h1CeVsEuPhiCePhiEuDiff", "h1CeVsEuPhiCePhiEuDiff",  100, -100., 100.);
  h2CeVsEuThetaCeThetaEu=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu", "h2CeVsEuThetaCeThetaEu",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEu = fs->make<TH2D>("h2CeVsEuBxCeBxEu","h2CeVsEuBxCeBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsEuBxCeBxEuDiff = fs->make<TH1D>("h1CeVsEuBxCeBxEuDiff","h1CeVsEuBxCeBxEuDiff", 8, -3.5, 4.5);
  
  h1CeVsEuMatches_bx = fs->make<TH1D>("h1CeVsEuMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsEuPhiCePhiEu_bx=fs->make<TH2D>("h2CeVsEuPhiCePhiEu_bx", "h2CeVsEuPhiCePhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEu_bx=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu_bx", "h2CeVsEuThetaCeThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEu_bx = fs->make<TH2D>("h2CeVsEuBxCeBxEu_bx","h2CeVsEuBxCeBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEuMatches_int = fs->make<TH1D>("h1CeVsEuMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsEuPhiCePhiEu_int=fs->make<TH2D>("h2CeVsEuPhiCePhiEu_int", "h2CeVsEuPhiCePhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEu_int=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu_int", "h2CeVsEuThetaCeThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuChamberEuZoneEu = fs->make<TH2D>("h2CeVsEuChamberEuZoneEu", "CPPFDigis h2CeVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEuChamberCeZoneCe = fs->make<TH2D>("h2CeVsEuChamberCeZoneCe", "CPPFDigis h2CeVsEuChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  
  h2CeVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOccupancy","CPPFDigis Bx_Occupancy_emu_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxEuZoneEu = fs->make<TH2D>("h2CeVsEuBxEuZoneEu","CPPFDigis Bx_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEuBxCeZoneCe = fs->make<TH2D>("h2CeVsEuBxCeZoneCe","CPPFDigis Bx_emu_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
 
                                    
  h1CeVsEeMatches = fs->make<TH1D>("h1CeVsEeMatches", "CPPFDigis_h1CeVsEeMatches" , 5, 0. , 5.);
  
  h1CeVsEeMatches_ch = fs->make<TH1D>("h1CeVsEeMatches_ch", "CPPFDigis_Matches_ch" , 5, 0. , 5.);
  h2CeVsEeChamberCeChamberEe_test = fs->make<TH2D>("h2CeVsEeChamberCeChamberEe_test","h2CeVsEeChamberCeChamberEe_test", 36, 1.,37.,36,1.,37.);
  h2CeVsEeChamberCeChamberEe = fs->make<TH2D>("h2CeVsEeChamberCeChamberEe","h2CeVsEeChamberCeChamberEe", 36, 1.,37.,36,1.,37.);
  h2CeVsEePhiCePhiEe=fs->make<TH2D>("h2CeVsEePhiCePhiEe", "h2CeVsEePhiCePhiEe",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsEePhiCePhiEeDiff=fs->make<TH1D>("h1CeVsEePhiCePhiEeDiff", "h1CeVsEePhiCePhiEeDiff",  100, -100., 100.);
  h2CeVsEeThetaCeThetaEe=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe", "h2CeVsEeThetaCeThetaEe",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEe = fs->make<TH2D>("h2CeVsEeBxCeBxEe","h2CeVsEeBxCeBxEe", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsEeBxCeBxEeDiff = fs->make<TH1D>("h1CeVsEeBxCeBxEeDiff","h1CeVsEeBxCeBxEeDiff", 8, -3.5, 4.5);
  
  h1CeVsEeMatches_bx = fs->make<TH1D>("h1CeVsEeMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsEePhiCePhiEe_bx=fs->make<TH2D>("h2CeVsEePhiCePhiEe_bx", "h2CeVsEePhiCePhiEe_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEe_bx=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe_bx", "h2CeVsEeThetaCeThetaEe_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEe_bx = fs->make<TH2D>("h2CeVsEeBxCeBxEe_bx","h2CeVsEeBxCeBxEe_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEeMatches_int = fs->make<TH1D>("h1CeVsEeMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsEePhiCePhiEe_int=fs->make<TH2D>("h2CeVsEePhiCePhiEe_int", "h2CeVsEePhiCePhiEe_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEe_int=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe_int", "h2CeVsEeThetaCeThetaEe_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeChamberEeZoneEe = fs->make<TH2D>("h2CeVsEeChamberEeZoneEe", "CPPFDigis h2CeVsEeChamberEeZoneEe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEeChamberCeZoneCe = fs->make<TH2D>("h2CeVsEeChamberCeZoneCe", "CPPFDigis h2CeVsEeChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  
  h2CeVsEeBxEeZoneEeOccupancy = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOccupancy","CPPFDigis Bx_Occupancy_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOccupancy","CPPFDigis Bx_Occupancy_emu_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxEeZoneEe = fs->make<TH2D>("h2CeVsEeBxEeZoneEe","CPPFDigis Bx_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEeBxCeZoneCe = fs->make<TH2D>("h2CeVsEeBxCeZoneCe","CPPFDigis Bx_emu_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
 
  h2CuVsEuChamberCuChamberEu_test = fs->make<TH2D>("h2CuVsEuChamberCuChamberEu_test","h2CuVsEuChamberCuChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2CuVsEuChamberCuChamberEu = fs->make<TH2D>("h2CuVsEuChamberCuChamberEu","h2CuVsEuChamberCuChamberEu", 36, 1.,37.,36,1.,37.);
  h2CuVsEuChamberCuChamberEuBox = fs->make<TH2D>("h2CuVsEuChamberCuChamberEuBox","h2CuVsEuChamberCuChamberEuBox", 36, 1.,37.,36,1.,37.);
  h2CuVsEuPhiCuPhiEu=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu", "h2CuVsEuPhiCuPhiEu",  124, 0., 1240., 124, 0., 1240.);
  h1CuVsEuPhiCuPhiEuDiff=fs->make<TH1D>("h1CuVsEuPhiCuPhiEuDiff", "h1CuVsEuPhiCuPhiEuDiff",  100, -100., 100.);
  h2CuVsEuThetaCuThetaEu=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu", "h2CuVsEuThetaCuThetaEu",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEu = fs->make<TH2D>("h2CuVsEuBxCuBxEu","h2CuVsEuBxCuBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CuVsEuBxCuBxEuDiff = fs->make<TH1D>("h1CuVsEuBxCuBxEuDiff","h1CuVsEuBxCuBxEuDiff", 8, -3.5, 4.5);
  
  h2CuVsEuPhiCuPhiEu_bx=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu_bx", "h2CuVsEuPhiCuPhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEu_bx=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu_bx", "h2CuVsEuThetaCuThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEu_bx = fs->make<TH2D>("h2CuVsEuBxCuBxEu_bx","h2CuVsEuBxCuBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2CuVsEuPhiCuPhiEu_int=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu_int", "h2CuVsEuPhiCuPhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEu_int=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu_int", "h2CuVsEuThetaCuThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuChamberCuZoneCu = fs->make<TH2D>("h2CuVsEuChamberCuZoneCu", "CPPFDigis h2CuVsEuChamberCuZoneCu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberCuZoneCuBox = fs->make<TH2D>("h2CuVsEuChamberCuZoneCuBox", "CPPFDigis h2CuVsEuChamberCuZoneCuBox", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEu = fs->make<TH2D>("h2CuVsEuChamberEuZoneEu", "CPPFDigis h2CuVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEuBox = fs->make<TH2D>("h2CuVsEuChamberEuZoneEuBox", "CPPFDigis h2CuVsEuChamberEuZoneEuBox", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  h2CuVsEuBxCuZoneCuOccupancy = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_u", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_e", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxCuZoneCu = fs->make<TH2D>("h2CuVsEuBxCuZoneCu","CPPFDigis Bx_unpacker_emtf_u", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CuVsEuBxEuZoneEu = fs->make<TH2D>("h2CuVsEuBxEuZoneEu","CPPFDigis Bx_unpacker_emtf_e", 12, 0.5, 12.5, 8,-3.5,4.5);



  h2EeVsEuChamberEeChamberEu_test = fs->make<TH2D>("h2EeVsEuChamberEeChamberEu_test","h2EeVsEuChamberEeChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2EeVsEuChamberEeChamberEu = fs->make<TH2D>("h2EeVsEuChamberEeChamberEu","h2EeVsEuChamberEeChamberEu", 36, 1.,37.,36,1.,37.);
  h2EeVsEuPhiEePhiEu=fs->make<TH2D>("h2EeVsEuPhiEePhiEu", "h2EeVsEuPhiEePhiEu",  124, 0., 1240., 124, 0., 1240.);
  h1EeVsEuPhiEePhiEuDiff=fs->make<TH1D>("h1EeVsEuPhiEePhiEuDiff", "h1EeVsEuPhiEePhiEuDiff",  100, -100., 100.);
  h2EeVsEuThetaEeThetaEu=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu", "h2EeVsEuThetaEeThetaEu",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEu = fs->make<TH2D>("h2EeVsEuBxEeBxEu","h2EeVsEuBxEeBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1EeVsEuBxEeBxEuDiff = fs->make<TH1D>("h1EeVsEuBxEeBxEuDiff","h1EeVsEuBxEeBxEuDiff", 8, -3.5, 4.5);
  
  h2EeVsEuPhiEePhiEu_bx=fs->make<TH2D>("h2EeVsEuPhiEePhiEu_bx", "h2EeVsEuPhiEePhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEu_bx=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu_bx", "h2EeVsEuThetaEeThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEu_bx = fs->make<TH2D>("h2EeVsEuBxEeBxEu_bx","h2EeVsEuBxEeBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2EeVsEuPhiEePhiEu_int=fs->make<TH2D>("h2EeVsEuPhiEePhiEu_int", "h2EeVsEuPhiEePhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEu_int=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu_int", "h2EeVsEuThetaEeThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuChamberEeZoneEe = fs->make<TH2D>("h2EeVsEuChamberEeZoneEe", "CPPFDigis h2EeVsEuChamberEeZoneEe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2EeVsEuChamberEuZoneEu = fs->make<TH2D>("h2EeVsEuChamberEuZoneEu", "CPPFDigis h2EeVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  h2EeVsEuBxEeZoneEeOccupancy = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_u", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_e", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEeZoneEe = fs->make<TH2D>("h2EeVsEuBxEeZoneEe","CPPFDigis Bx_unpacker_emtf_u", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2EeVsEuBxEuZoneEu = fs->make<TH2D>("h2EeVsEuBxEuZoneEu","CPPFDigis Bx_unpacker_emtf_e", 12, 0.5, 12.5, 8,-3.5,4.5);


// Only one match histograms

  h1CeVsCuPhiOffDiagonalOneHit = fs->make<TH1D>("h1CeVsCuPhiOffDiagonalOneHit", "CPPFDigis_h1CeVsCuPhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuPhiInDiagonalOneHit = fs->make<TH1D>("h1CeVsCuPhiInDiagonalOneHit", "CPPFDigis_h1CeVsCuPhiInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuThetaOffDiagonalOneHit = fs->make<TH1D>("h1CeVsCuThetaOffDiagonalOneHit", "CPPFDigis_h1CeVsCuThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuThetaInDiagonalOneHit = fs->make<TH1D>("h1CeVsCuThetaInDiagonalOneHit", "CPPFDigis_h1CeVsCuThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsCuPhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuPhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuPhiInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuPhiInDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuPhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsCuThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsCuThetaInDiagonalOneHit_bx", "CPPFDigis_h1CeVsCuThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h2CeVsCuChamberCeChamberCuOneHit = fs->make<TH2D>("h2CeVsCuChamberCeChamberCuOneHit","h2CeVsCuChamberCeChamberCuOneHit", 36, 1.,37.,36,1.,37.);
  h2CeVsCuPhiCePhiCuOneHit=fs->make<TH2D>("h2CeVsCuPhiCePhiCuOneHit", "h2CeVsCuPhiCePhiCuOneHit",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsCuPhiCePhiCuDiffOneHit=fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiffOneHit", "h1CeVsCuPhiCePhiCuDiffOneHit",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiffOneHit=fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiffOneHit", "h1CeVsCuThetaCeThetaCuDiffOneHit",  200, -100., 100.);
  h1CeVsCuPhiCePhiCuDiffOneHit_bx=fs->make<TH1D>("h1CeVsCuPhiCePhiCuDiffOneHit_bx", "h1CeVsCuPhiCePhiCuDiffOneHit_bx",  200, -100., 100.);
  h1CeVsCuThetaCeThetaCuDiffOneHit_bx=fs->make<TH1D>("h1CeVsCuThetaCeThetaCuDiffOneHit_bx", "h1CeVsCuThetaCeThetaCuDiffOneHit_bx",  200, -100., 100.);
  h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx=fs->make<TH1D>("h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx", "h1CeVsCuThetaPhiCeThetaPhiCuDiffOneHit_bx",  200, -100., 100.);
  h2CeVsCuThetaCeThetaCuOneHit=fs->make<TH2D>("h2CeVsCuThetaCeThetaCuOneHit", "h2CeVsCuThetaCeThetaCuOneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCuOneHit = fs->make<TH2D>("h2CeVsCuBxCeBxCu","h2CeVsCuBxCeBxCuOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsCuBxCeBxCuDiffOneHit = fs->make<TH1D>("h1CeVsCuBxCeBxCuDiffOneHit","h1CeVsCuBxCeBxCuDiffOneHit", 8, -3.5, 4.5);
  h2CeVsCuPhiCePhiCuOneHit_bx=fs->make<TH2D>("h2CeVsCuPhiCePhiCuOneHit_bx", "h2CeVsCuPhiCePhiCuOneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCuOneHit_bx=fs->make<TH2D>("h2CeVsCuThetaCeThetaCuOneHit_bx", "h2CeVsCuThetaCeThetaCuOneHit_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCuOneHit_bx = fs->make<TH2D>("h2CeVsCuBxCeBxCuOneHit_bx","h2CeVsCuBxCeBxCuOneHit_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  h2CeVsCuPhiCePhiCuOneHit_int=fs->make<TH2D>("h2CeVsCuPhiCePhiCuOneHit_int", "h2CeVsCuPhiCePhiCu_intOneHit",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCuOneHit_int=fs->make<TH2D>("h2CeVsCuThetaCeThetaCuOneHit_int", "h2CeVsCuThetaCeThetaCuOneHit_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuChamberCuZoneCuOneHit = fs->make<TH2D>("h2CeVsCuChamberCuZoneCuOneHit", "CPPFDigis h2CeVsCuChamberCuZoneCuOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCeOneHit = fs->make<TH2D>("h2CeVsCuChamberCeZoneCeOneHit", "CPPFDigis h2CeVsCuChamberCeZoneCeOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCuZoneCuOccupancyOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCeZoneCeOccupancyOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsCuBxCuZoneCuOneHit = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOneHit","CPPFDigis Bx_unpackerOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCeOneHit = fs->make<TH2D>("h2CeVsCuBxCeZoneCeOneHit","CPPFDigis Bx_emu_unpackerOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);


  h1CeVsEuPhiOffDiagonalOneHit = fs->make<TH1D>("h1CeVsEuPhiOffDiagonalOneHit", "CPPFDigis_h1CeVsEuPhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEuPhiInDiagonalOneHit = fs->make<TH1D>("h1CeVsEuPhiInDiagonalOneHit", "CPPFDigis_h1CeVsEuPhiInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEuThetaOffDiagonalOneHit = fs->make<TH1D>("h1CeVsEuThetaOffDiagonalOneHit", "CPPFDigis_h1CeVsEuThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEuThetaInDiagonalOneHit = fs->make<TH1D>("h1CeVsEuThetaInDiagonalOneHit", "CPPFDigis_h1CeVsEuThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEuPhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEuPhiOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsEuPhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEuPhiInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEuPhiInDiagonalOneHit_bx", "CPPFDigis_h1CeVsEuPhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEuThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEuThetaOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsEuThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEuThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEuThetaInDiagonalOneHit_bx", "CPPFDigis_h1CeVsEuThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h2CeVsEuChamberCeChamberEuOneHit = fs->make<TH2D>("h2CeVsEuChamberCeChamberEuOneHit","h2CeVsEuChamberCeChamberEuOneHit", 36, 1.,37.,36,1.,37.);
  h2CeVsEuPhiCePhiEuOneHit=fs->make<TH2D>("h2CeVsEuPhiCePhiEuOneHit", "h2CeVsEuPhiCePhiEuOneHit",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsEuPhiCePhiEuDiffOneHit=fs->make<TH1D>("h1CeVsEuPhiCePhiEuDiffOneHit", "h1CeVsEuPhiCePhiEuDiffOneHit",  100, -100., 100.);
  h1CeVsEuThetaCeThetaEuDiffOneHit=fs->make<TH1D>("h1CeVsEuThetaCeThetaEuDiffOneHit", "h1CeVsEuThetaCeThetaEuDiffOneHit",  100, -100., 100.);
  h1CeVsEuPhiCePhiEuDiffOneHit_bx=fs->make<TH1D>("h1CeVsEuPhiCePhiEuDiffOneHit_bx", "h1CeVsEuPhiCePhiEuDiffOneHit_bx",  100, -100., 100.);
  h1CeVsEuThetaCeThetaEuDiffOneHit_bx=fs->make<TH1D>("h1CeVsEuThetaCeThetaEuDiffOneHit_bx", "h1CeVsEuThetaCeThetaEuDiffOneHit_bx",  100, -100., 100.);
  h1CeVsEuThetaPhiCeThetaPhiEuDiffOneHit_bx=fs->make<TH1D>("h1CeVsEuThetaPhiCeThetaPhiEuDiffOneHit_bx", "h1CeVsEuThetaPhiCeThetaPhiEuDiffOneHit_bx",  100, -100., 100.);
  h2CeVsEuThetaCeThetaEuOneHit=fs->make<TH2D>("h2CeVsEuThetaCeThetaEuOneHit", "h2CeVsEuThetaCeThetaEuOneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEuOneHit = fs->make<TH2D>("h2CeVsEuBxCeBxEuOneHit","h2CeVsEuBxCeBxEuOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsEuBxCeBxEuDiffOneHit = fs->make<TH1D>("h1CeVsEuBxCeBxEuDiffOneHit","h1CeVsEuBxCeBxEuDiffOneHit", 8, -3.5, 4.5);
  h2CeVsEuPhiCePhiEuOneHit_bx=fs->make<TH2D>("h2CeVsEuPhiCePhiEuOneHit_bx", "h2CeVsEuPhiCePhiEuOneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuPhiCePhiEuOneHit_bx=fs->make<TH2D>("h2CeVsEuPhiCePhiEuOneHit_bx", "h2CeVsEuPhiCePhiEuOneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEuOneHit_bx=fs->make<TH2D>("h2CeVsEuThetaCeThetaEuOneHit_bx", "h2CeVsEuThetaCeThetaEuOneHit_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEuOneHit_bx = fs->make<TH2D>("h2CeVsEuBxCeBxEuOneHit_bx","h2CeVsEuBxCeBxEuOneHit_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  h2CeVsEuPhiCePhiEuOneHit_int=fs->make<TH2D>("h2CeVsEuPhiCePhiEuOneHit_int", "h2CeVsEuPhiCePhiEuOneHit_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEuOneHit_int=fs->make<TH2D>("h2CeVsEuThetaCeThetaEuOneHit_int", "h2CeVsEuThetaCeThetaEuOneHit_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuChamberEuZoneEuOneHit = fs->make<TH2D>("h2CeVsEuChamberEuZoneEuOneHit", "CPPFDigis h2CeVsEuChamberEuZoneEuOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEuChamberCeZoneCeOneHit = fs->make<TH2D>("h2CeVsEuChamberCeZoneCeOneHit", "CPPFDigis h2CeVsEuChamberCeZoneCeOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEuBxEuZoneEuOccupancyOneHit = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOccupancyOneHit","CPPFDigis Bx_Occupancy_emtfOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxCeZoneCeOccupancyOneHit = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOccupancyOneHit","CPPFDigis Bx_Occupancy_emu_emtfOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEuBxCeZoneCeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEuBxCeZoneCeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEuBxEuZoneEuOneHit = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOneHit","CPPFDigis Bx_emtfOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEuBxCeZoneCeOneHit = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOneHit","CPPFDigis Bx_emu_emtfOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
 
  h1CeVsEePhiOffDiagonalOneHit = fs->make<TH1D>("h1CeVsEePhiOffDiagonalOneHit", "CPPFDigis_h1CeVsEePhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEePhiInDiagonalOneHit = fs->make<TH1D>("h1CeVsEePhiInDiagonalOneHit", "CPPFDigis_h1CeVsEePhiInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEeThetaOffDiagonalOneHit = fs->make<TH1D>("h1CeVsEeThetaOffDiagonalOneHit", "CPPFDigis_h1CeVsEeThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEeThetaInDiagonalOneHit = fs->make<TH1D>("h1CeVsEeThetaInDiagonalOneHit", "CPPFDigis_h1CeVsEeThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1CeVsEePhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEePhiOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsEePhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEePhiInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEePhiInDiagonalOneHit_bx", "CPPFDigis_h1CeVsEePhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEeThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEeThetaOffDiagonalOneHit_bx", "CPPFDigis_h1CeVsEeThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CeVsEeThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1CeVsEeThetaInDiagonalOneHit_bx", "CPPFDigis_h1CeVsEeThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h2CeVsEeChamberCeChamberEeOneHit = fs->make<TH2D>("h2CeVsEeChamberCeChamberEeOneHit","h2CeVsEeChamberCeChamberEeOneHit", 36, 1.,37.,36,1.,37.);
  h2CeVsEePhiCePhiEeOneHit=fs->make<TH2D>("h2CeVsEePhiCePhiEeOneHit", "h2CeVsEePhiCePhiEeOneHit",  124, 0., 1240., 124, 0., 1240.);
  h1CeVsEePhiCePhiEeDiffOneHit=fs->make<TH1D>("h1CeVsEePhiCePhiEeDiffOneHit", "h1CeVsEePhiCePhiEeDiffOneHit",  100, -100., 100.);
  h1CeVsEeThetaCeThetaEeDiffOneHit=fs->make<TH1D>("h1CeVsEeThetaCeThetaEeDiffOneHit", "h1CeVsEeThetaCeThetaEeDiffOneHit",  100, -100., 100.);
  h1CeVsEePhiCePhiEeDiffOneHit_bx=fs->make<TH1D>("h1CeVsEePhiCePhiEeDiffOneHit_bx", "h1CeVsEePhiCePhiEeDiffOneHit_bx",  100, -100., 100.);
  h1CeVsEeThetaCeThetaEeDiffOneHit_bx=fs->make<TH1D>("h1CeVsEeThetaCeThetaEeDiffOneHit_bx", "h1CeVsEeThetaCeThetaEeDiffOneHit_bx",  100, -100., 100.);
  h1CeVsEeThetaPhiCeThetaPhiEeDiffOneHit_bx=fs->make<TH1D>("h1CeVsEeThetaPhiCeThetaPhiEeDiffOneHit_bx", "h1CeVsEeThetaPhiCeThetaPhiEeDiffOneHit_bx",  100, -100., 100.);
  h2CeVsEeThetaCeThetaEeOneHit=fs->make<TH2D>("h2CeVsEeThetaCeThetaEeOneHit", "h2CeVsEeThetaCeThetaEeOneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEeOneHit = fs->make<TH2D>("h2CeVsEeBxCeBxEeOneHit","h2CeVsEeBxCeBxEeOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CeVsEeBxCeBxEeDiffOneHit = fs->make<TH1D>("h1CeVsEeBxCeBxEeDiffOneHit","h1CeVsEeBxCeBxEeDiffOneHit", 8, -3.5, 4.5);
  h2CeVsEePhiCePhiEeOneHit_bx=fs->make<TH2D>("h2CeVsEePhiCePhiEe_bxOneHit", "h2CeVsEePhiCePhiEe_bxOneHit",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEeOneHit_bx=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe_bxOneHit", "h2CeVsEeThetaCeThetaEe_bxOneHit",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEeOneHit_bx = fs->make<TH2D>("h2CeVsEeBxCeBxEe_bxOneHit","h2CeVsEeBxCeBxEe_bxOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h2CeVsEePhiCePhiEeOneHit_int=fs->make<TH2D>("h2CeVsEePhiCePhiEeOneHit_int", "h2CeVsEePhiCePhiEeOneHit_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEeOneHit_int=fs->make<TH2D>("h2CeVsEeThetaCeThetaEeOneHit_int", "h2CeVsEeThetaCeThetaEeOneHit_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeChamberEeZoneEeOneHit = fs->make<TH2D>("h2CeVsEeChamberEeZoneEeOneHit", "CPPFDigis h2CeVsEeChamberEeZoneEeOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEeChamberCeZoneCeOneHit = fs->make<TH2D>("h2CeVsEeChamberCeZoneCeOneHit", "CPPFDigis h2CeVsEeChamberCeZoneCeOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEeBxEeZoneEeOccupancyOneHit = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOccupancyOneHit","CPPFDigis Bx_Occupancy_emtfOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxCeZoneCeOccupancyOneHit = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOccupancyOneHit","CPPFDigis Bx_Occupancy_emu_emtfOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxEeZoneEeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEeBxCeZoneCeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEeBxEeZoneEeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEeBxCeZoneCeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CeVsEeBxEeZoneEeOneHit = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOneHit","CPPFDigis Bx_emtfOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEeBxCeZoneCeOneHit = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOneHit","CPPFDigis Bx_emu_emtfOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
 
  h1CuVsEuPhiOffDiagonalOneHit = fs->make<TH1D>("h1CuVsEuPhiOffDiagonalOneHit", "CPPFDigis_h1CuVsEuPhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1CuVsEuPhiInDiagonalOneHit = fs->make<TH1D>("h1CuVsEuPhiInDiagonalOneHit", "CPPFDigis_h1CuVsEuPhiInDiagonalOneHit" , 2, 0. , 2.);
  h1CuVsEuThetaOffDiagonalOneHit = fs->make<TH1D>("h1CuVsEuThetaOffDiagonalOneHit", "CPPFDigis_h1CuVsEuThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1CuVsEuThetaInDiagonalOneHit = fs->make<TH1D>("h1CuVsEuThetaInDiagonalOneHit", "CPPFDigis_h1CuVsEuThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1CuVsEuPhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1CuVsEuPhiOffDiagonalOneHit_bx", "CPPFDigis_h1CuVsEuPhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CuVsEuPhiInDiagonalOneHit_bx = fs->make<TH1D>("h1CuVsEuPhiInDiagonalOneHit_bx", "CPPFDigis_h1CuVsEuPhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CuVsEuThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1CuVsEuThetaOffDiagonalOneHit_bx", "CPPFDigis_h1CuVsEuThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1CuVsEuThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1CuVsEuThetaInDiagonalOneHit_bx", "CPPFDigis_h1CuVsEuThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h2CuVsEuChamberCuChamberEuOneHit = fs->make<TH2D>("h2CuVsEuChamberCuChamberEuOneHit","h2CuVsEuChamberCuChamberEuOneHit", 36, 1.,37.,36,1.,37.);
  h2CuVsEuChamberCuChamberEuBoxOneHit = fs->make<TH2D>("h2CuVsEuChamberCuChamberEuBoxOneHit","h2CuVsEuChamberCuChamberEuBoxOneHit", 36, 1.,37.,36,1.,37.);
  h2CuVsEuPhiCuPhiEuOneHit=fs->make<TH2D>("h2CuVsEuPhiCuPhiEuOneHit", "h2CuVsEuPhiCuPhiEuOneHit",  124, 0., 1240., 124, 0., 1240.);
  h1CuVsEuPhiCuPhiEuDiffOneHit=fs->make<TH1D>("h1CuVsEuPhiCuPhiEuDiffOneHit", "h1CuVsEuPhiCuPhiEuDiffOneHit",  100, -100., 100.);
  h1CuVsEuThetaCuThetaEuDiffOneHit=fs->make<TH1D>("h1CuVsEuThetaCuThetaEuDiffOneHit", "h1CuVsEuThetaCuThetaEuDiffOneHit",  100, -100., 100.);
  h1CuVsEuPhiCuPhiEuDiffOneHit_bx=fs->make<TH1D>("h1CuVsEuPhiCuPhiEuDiffOneHit_bx", "h1CuVsEuPhiCuPhiEuDiffOneHit_bx",  100, -100., 100.);
  h1CuVsEuThetaCuThetaEuDiffOneHit_bx=fs->make<TH1D>("h1CuVsEuThetaCuThetaEuDiffOneHit_bx", "h1CuVsEuThetaCuThetaEuDiffOneHit_bx",  100, -100., 100.);
  h1CuVsEuThetaPhiCuThetaPhiEuDiffOneHit_bx=fs->make<TH1D>("h1CuVsEuThetaPhiCuThetaPhiEuDiffOneHit_bx", "h1CuVsEuThetaPhiCuThetaPhiEuDiffOneHit_bx",  100, -100., 100.);
  h2CuVsEuThetaCuThetaEuOneHit=fs->make<TH2D>("h2CuVsEuThetaCuThetaEuOneHit", "h2CuVsEuThetaCuThetaEuOneHit",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEuOneHit = fs->make<TH2D>("h2CuVsEuBxCuBxEuOneHit","h2CuVsEuBxCuBxEuOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1CuVsEuBxCuBxEuDiffOneHit = fs->make<TH1D>("h1CuVsEuBxCuBxEuDiffOneHit","h1CuVsEuBxCuBxEuDiffOneHit", 8, -3.5, 4.5);
  h2CuVsEuPhiCuPhiEuOneHit_bx=fs->make<TH2D>("h2CuVsEuPhiCuPhiEuOneHit_bx", "h2CuVsEuPhiCuPhiEuOneHit_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEuOneHit_bx=fs->make<TH2D>("h2CuVsEuThetaCuThetaEuOneHit_bx", "h2CuVsEuThetaCuThetaEuOneHit_bx",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEuOneHit_bx = fs->make<TH2D>("h2CuVsEuBxCuBxEuOneHit_bx","h2CuVsEuBxCuBxEuOneHit_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  h2CuVsEuPhiCuPhiEuOneHit_int=fs->make<TH2D>("h2CuVsEuPhiCuPhiEuOneHitOneHit_int", "h2CuVsEuPhiCuPhiEuOneHitOneHit_int",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEuOneHit_int=fs->make<TH2D>("h2CuVsEuThetaCuThetaEuOneHit_int", "h2CuVsEuThetaCuThetaEuOneHit_int",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuChamberCuZoneCuOneHit = fs->make<TH2D>("h2CuVsEuChamberCuZoneCuOneHit", "CPPFDigis h2CuVsEuChamberCuZoneCuOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberCuZoneCuBoxOneHit = fs->make<TH2D>("h2CuVsEuChamberCuZoneCuBoxOneHit", "CPPFDigis h2CuVsEuChamberCuZoneCuBoxOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEuOneHit = fs->make<TH2D>("h2CuVsEuChamberEuZoneEuOneHit", "CPPFDigis h2CuVsEuChamberEuZoneEuOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEuBoxOneHit = fs->make<TH2D>("h2CuVsEuChamberEuZoneEuBoxOneHit", "CPPFDigis h2CuVsEuChamberEuZoneEuBoxOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuBxCuZoneCuOccupancyOneHit = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOccupancyOneHit","CPPFDigis Bx_Occupancy_unpacker_emtf_uOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxEuZoneEuOccupancyOneHit = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOccupancyOneHit","CPPFDigis Bx_Occupancy_unpacker_emtf_eOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CuVsEuBxCuZoneCuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CuVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CuVsEuBxCuZoneCuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2CuVsEuBxCuZoneCuOneHit = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOneHit","CPPFDigis Bx_unpacker_emtf_uOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CuVsEuBxEuZoneEuOneHit = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOneHit","CPPFDigis Bx_unpacker_emtf_eOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);

  h1EeVsEuPhiOffDiagonalOneHit = fs->make<TH1D>("h1EeVsEuPhiOffDiagonalOneHit", "CPPFDigis_h1EeVsEuPhiOffDiagonalOneHit" , 2, 0. , 2.);
  h1EeVsEuPhiInDiagonalOneHit = fs->make<TH1D>("h1EeVsEuPhiInDiagonalOneHit", "CPPFDigis_h1EeVsEuPhiInDiagonalOneHit" , 2, 0. , 2.);
  h1EeVsEuThetaOffDiagonalOneHit = fs->make<TH1D>("h1EeVsEuThetaOffDiagonalOneHit", "CPPFDigis_h1EeVsEuThetaOffDiagonalOneHit" , 2, 0. , 2.);
  h1EeVsEuThetaInDiagonalOneHit = fs->make<TH1D>("h1EeVsEuThetaInDiagonalOneHit", "CPPFDigis_h1EeVsEuThetaInDiagonalOneHit" , 2, 0. , 2.);
  h1EeVsEuPhiOffDiagonalOneHit_bx = fs->make<TH1D>("h1EeVsEuPhiOffDiagonalOneHit_bx", "CPPFDigis_h1EeVsEuPhiOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1EeVsEuPhiInDiagonalOneHit_bx = fs->make<TH1D>("h1EeVsEuPhiInDiagonalOneHit_bx", "CPPFDigis_h1EeVsEuPhiInDiagonalOneHit_bx" , 2, 0. , 2.);
  h1EeVsEuThetaOffDiagonalOneHit_bx = fs->make<TH1D>("h1EeVsEuThetaOffDiagonalOneHit_bx", "CPPFDigis_h1EeVsEuThetaOffDiagonalOneHit_bx" , 2, 0. , 2.);
  h1EeVsEuThetaInDiagonalOneHit_bx = fs->make<TH1D>("h1EeVsEuThetaInDiagonalOneHit_bx", "CPPFDigis_h1EeVsEuThetaInDiagonalOneHit_bx" , 2, 0. , 2.);
  h2EeVsEuChamberEeChamberEuOneHit = fs->make<TH2D>("h2EeVsEuChamberEeChamberEuOneHit","h2EeVsEuChamberEeChamberEuOneHit", 36, 1.,37.,36,1.,37.);
  h2EeVsEuPhiEePhiEuOneHit=fs->make<TH2D>("h2EeVsEuPhiEePhiEuOneHit", "h2EeVsEuPhiEePhiEuOneHit",  124, 0., 1240., 124, 0., 1240.);
  h1EeVsEuPhiEePhiEuDiffOneHit=fs->make<TH1D>("h1EeVsEuPhiEePhiEuDiffOneHit", "h1EeVsEuPhiEePhiEuDiffOneHit",  100, -100., 100.);
  h1EeVsEuThetaEeThetaEuDiffOneHit=fs->make<TH1D>("h1EeVsEuThetaEeThetaEuDiffOneHit", "h1EeVsEuThetaEeThetaEuDiffOneHit",  100, -100., 100.);
  h1EeVsEuPhiEePhiEuDiffOneHit_bx=fs->make<TH1D>("h1EeVsEuPhiEePhiEuDiffOneHit_bx", "h1EeVsEuPhiEePhiEuDiffOneHit_bx",  100, -100., 100.);
  h1EeVsEuThetaEeThetaEuDiffOneHit_bx=fs->make<TH1D>("h1EeVsEuThetaEeThetaEuDiffOneHit_bx", "h1EeVsEuThetaEeThetaEuDiffOneHit_bx",  100, -100., 100.);
  h1EeVsEuThetaPhiEeThetaPhiEuDiffOneHit_bx=fs->make<TH1D>("h1EeVsEuThetaPhiEeThetaPhiEuDiffOneHit_bx", "h1EeVsEuThetaPhiEeThetaPhiEuDiffOneHit_bx",  100, -100., 100.);
  h2EeVsEuThetaEeThetaEuOneHit=fs->make<TH2D>("h2EeVsEuThetaEeThetaEuOneHit", "h2EeVsEuThetaEeThetaEuOneHit",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEuOneHit = fs->make<TH2D>("h2EeVsEuBxEeBxEuOneHit","h2EeVsEuBxEeBxEuOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h1EeVsEuBxEeBxEuDiffOneHit = fs->make<TH1D>("h1EeVsEuBxEeBxEuDiffOneHit","h1EeVsEuBxEeBxEuDiffOneHit", 8, -3.5, 4.5);
  h2EeVsEuPhiEePhiEuOneHit_bx=fs->make<TH2D>("h2EeVsEuPhiEePhiEuOneHit_bx", "h2EeVsEuPhiEePhiEu_bxOneHit",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEuOneHit_bx=fs->make<TH2D>("h2EeVsEuThetaEeThetaEuOneHit_bx", "h2EeVsEuThetaEeThetaEu_bxOneHit",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEuOneHit_bx = fs->make<TH2D>("h2EeVsEuBxEeBxEuOneHit_bx","h2EeVsEuBxEeBxEu_bxOneHit", 8, -3.5,4.5, 8, -3.5, 4.5);
  h2EeVsEuPhiEePhiEuOneHit_int=fs->make<TH2D>("h2EeVsEuPhiEePhiEuOneHit_int", "h2EeVsEuPhiEePhiEuOneHit_int",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEuOneHit_int=fs->make<TH2D>("h2EeVsEuThetaEeThetaEuOneHit_int", "h2EeVsEuThetaEeThetaEuOneHit_int",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuChamberEeZoneEeOneHit = fs->make<TH2D>("h2EeVsEuChamberEeZoneEeOneHit", "CPPFDigis h2EeVsEuChamberEeZoneEeOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2EeVsEuChamberEuZoneEuOneHit = fs->make<TH2D>("h2EeVsEuChamberEuZoneEuOneHit", "CPPFDigis h2EeVsEuChamberEuZoneEuOneHit", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2EeVsEuBxEeZoneEeOccupancyOneHit = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOccupancyOneHit","CPPFDigis Bx_Occupancy_unpacker_emtf_uOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEuZoneEuOccupancyOneHit = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOccupancyOneHit","CPPFDigis Bx_Occupancy_unpacker_emtf_eOneHit", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2EeVsEuBxEeZoneEeOccupancyOffPhiDiagonalOneHit = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOccupancyOffPhiDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2EeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2EeVsEuBxEeZoneEeOccupancyOffThetaDiagonalOneHit = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOccupancyOffThetaDiagonalOneHit","CPPFDigis Bx_Occupancy_emu_unpackerOffThetaDiagonalOneHit", 36, 0.5, 36.5, 12, 0.5,12.5);
  h2EeVsEuBxEeZoneEeOneHit = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOneHit","CPPFDigis Bx_unpacker_emtf_uOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2EeVsEuBxEuZoneEuOneHit = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOneHit","CPPFDigis Bx_unpacker_emtf_eOneHit", 12, 0.5, 12.5, 8,-3.5,4.5);
  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(EMTF_CPPF_DQM);

