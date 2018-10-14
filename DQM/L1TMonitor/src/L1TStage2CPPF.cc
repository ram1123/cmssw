/*
 * \L1TStage2CPPF.cc
 * \author
 * \September 2018
*/

#include "DQM/L1TMonitor/interface/L1TStage2CPPF.h"

L1TStage2CPPF::L1TStage2CPPF(const edm::ParameterSet & ps):
  monitorDir(ps.getUntrackedParameter<std::string>("monitorDir","")),
//  cppfdigiLabel(ps.getParameter<edm::InputTag>("cppfdigiLabel")),
  verbose(ps.getUntrackedParameter<bool>("verbose", false)),
  global_phi(-1000),
  cppfDigiToken_(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("cppfSource")))
{
//  cppfDigiToken_(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("cppfdigiLabel")));
//  cppfToken=consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("cppfSource"));
  //  cppfTokenTwinMux1 = consumes<L1MuDTChambThContainer>(ps.getParameter<edm::InputTag>("cppfSourceTwinMux1"));
  //  cppfTokenTwinMux2 = consumes<L1MuDTChambPhContainer>(ps.getParameter<edm::InputTag>("cppfSourceTwinMux2"));

}

L1TStage2CPPF::~L1TStage2CPPF()
{
}

void L1TStage2CPPF::dqmBeginRun(const edm::Run& iRrun, const edm::EventSetup& eveSetup)
{
}

void L1TStage2CPPF::beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& eveSetup)
{
}

void L1TStage2CPPF::bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& iRun, const edm::EventSetup& eveSetup)
{
  ibooker.setCurrentFolder(monitorDir);
  
  Occupancy_EMTFSector = ibooker.book2D("Occupancy_EMTFSector", "Occupancy_EMTFSector", 36, 1., 37., 12, 1.,13.);
  Track_Bx  = ibooker.book2D("Track_Bx","Track_Bx", 12, 1., 13., 7,-3.,4.);
//  cppf_wedge_bx->setTitle(";Wedge; BX");
//  for (int bin = 1; bin < 13; ++bin) {
//    cppf_wedge_bx->setBinLabel(bin, std::to_string(bin), 1);
//  }

}

void L1TStage2CPPF::analyze(const edm::Event & eve, const edm::EventSetup & eveSetup)
{
  if (verbose) {
    edm::LogInfo("L1TStage2CPPF") << "L1TStage2CPPF: analyze...." << std::endl;
  }

  edm::Handle<l1t::CPPFDigiCollection> CppfDigis;
  eve.getByToken(cppfDigiToken_, CppfDigis);

//  for(int itBX=cppfMuon->getFirstBX(); itBX<=cppfMuon->getLastBX(); ++itBX)
//    {
//      for(l1t::RegionalMuonCandBxCollection::const_iterator itMuon = cppfMuon->begin(itBX); itMuon != cppfMuon->end(itBX); ++itMuon)
//        {  
//
//          cppf_hwEta->Fill(itMuon->hwEta());
//          if (fabs(cppfMuon->getLastBX()-cppfMuon->getFirstBX())>3){
//            cppf_wedge_bx->Fill(itMuon->processor(), itBX);  
//          }
//        }
//    }

//Fill the specific bin for each EMTF sector 
  for(int i = 1; i < 7; i++ ){
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

  for(auto& cppf_digis : *CppfDigis){

    RPCDetId rpcId = cppf_digis.rpcId();
    int ring = rpcId.ring();
    int station = rpcId.station();
    int region = rpcId.region();
    int subsector = rpcId.subsector();


//TOBECHECKED why rpcId.sector() sometimes return 7 ?
if(cppf_digis.emtf_sector()!=-99) {std::cout<<" L1TStage2CPPF::analyze cppf_digis.emtf_sector="<<cppf_digis.emtf_sector()<<std::endl;}
if(rpcId.sector()>=7 or rpcId.sector()<1) { std::cout<<" L1TStage2CPPF::analyze rpcId.sector="<<rpcId.sector()<<" emtf_sector="<<cppf_digis.emtf_sector()<<std::endl; continue;}  

    //Region -  
    if(region == -1){
  
      //for Occupancy
      //EMTF_sector = cppf_digis.emtf_sector();   // TOBECHECKED it returns -99, seems not filled
      EMTF_sector = rpcId.sector();

      EMTF_subsector = fill_info[EMTF_sector][subsector-1];
    
      if((station == 4) && (ring == 3))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 1);
      else if((station == 4) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 2);
      else if((station == 3) && (ring == 3))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 3);
      else if((station == 3) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 4);
      else if((station == 2) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 5);
      else if((station == 1) && (ring == 2))
    
      //for Track_Bx
      EMTF_bx = cppf_digis.bx();
      if(EMTF_sector==1)
    	  Track_Bx->Fill(6,EMTF_bx);
      else if(EMTF_sector==2)
    	  Track_Bx->Fill(5,EMTF_bx);
      else if(EMTF_sector==3)
    	  Track_Bx->Fill(4,EMTF_bx);
      else if(EMTF_sector==4)
    	  Track_Bx->Fill(3,EMTF_bx);
      else if(EMTF_sector==5)
    	  Track_Bx->Fill(2,EMTF_bx);
      else if(EMTF_sector==6)
          Track_Bx->Fill(1,EMTF_bx);
    
    }
    //Region +  
    if(region == 1){
    
      //for Occupancy
      //EMTF_sector = cppf_digis.emtf_sector();   // TOBECHECKED it returns -99, seems not filled
      EMTF_sector = rpcId.sector();
      EMTF_subsector = fill_info[EMTF_sector][subsector-1];
    
      if((station == 1) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 7);
      else if((station == 2) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 8);
      else if((station == 3) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 9);
      else if((station == 3) && (ring == 3))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 10);
      else if((station == 4) && (ring == 2))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 11);
      else if((station == 4) && (ring == 3))
    	  Occupancy_EMTFSector->Fill(EMTF_subsector, 12);
    
      //for Track_Bx
      EMTF_bx = cppf_digis.bx();
      if(EMTF_sector==1)
    	  Track_Bx->Fill(7,EMTF_bx);
      else if(EMTF_sector==2)
    	  Track_Bx->Fill(8,EMTF_bx);
      else if(EMTF_sector==3)
    	  Track_Bx->Fill(9,EMTF_bx);
      else if(EMTF_sector==4)
    	  Track_Bx->Fill(10,EMTF_bx);
      else if(EMTF_sector==5)
    	  Track_Bx->Fill(11,EMTF_bx);
      else if(EMTF_sector==6)
    	  Track_Bx->Fill(12,EMTF_bx);
    }
} //loop over CPPFDigis





  }








