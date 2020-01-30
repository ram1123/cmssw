#include "DQMCPPF.h"

DQM_CPPF::DQM_CPPF(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))){
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
  
  //std::cout << "DEBUG: Start..... " << std::endl;

  int matches_unpacker = 0;
  int matches_unpacker_bx = 0;
  int matches_unpacker_bx_phi = 0;
  int matches_unpacker_bx_Offphi = 0;
  int matches_unpacker_bx_theta = 0;
  int matches_unpacker_bx_Offtheta = 0;
  bool matches = false;
  bool matches_bx = false;
  bool matches_bx_phi = false;
  bool matches_bx_Offphi = false;
  bool matches_bx_theta = false;
  bool matches_bx_Offtheta = false;


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

    ar_cluster_sizeCe.clear();
    ar_cluster_sizeCu.clear();
    ar_bxCe.clear();
    ar_bxCu.clear();
    ar_phiIntCe.clear();
    ar_phiIntCu.clear();
    ar_thetaIntCu.clear();
    ar_thetaIntCe.clear();
    ar_thetaGlobalCe.clear();
    ar_thetaGlobalCu.clear();
    ar_phiGlobalCe.clear();
    ar_phiGlobalCu.clear();
    ar_chamberIDCe.clear();
    ar_chamberIDCu.clear();
    ar_rollCe.clear();
    ar_rollCu.clear();
    ar_emtfSubsectorCu.clear();
    ar_emtfSubsectorCe.clear();
    ar_fillOccupancyCu.clear();
    ar_fillOccupancyCe.clear();


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

      if((regionCe == regionCu) && 
         (stationCe == stationCu) &&
         (ringCe == ringCu) &&
         (sectorCe == sectorCu) &&
         (subsectorCe == subsectorCu) &&
         (emtfSubsectorCe == emtfSubsectorCu)) {
         
         matches = true;
         matches_unpacker++;

         //if (emtfSubsectorCu == emtfSubsectorCe) {
            ar_cluster_sizeCe.push_back(cluster_sizeCe);
            ar_cluster_sizeCu.push_back(cluster_sizeCu);
            ar_bxCe.push_back(bxCe);
            ar_bxCu.push_back(bxCu);
            ar_phiIntCe.push_back(phiIntCe);
            ar_phiIntCu.push_back(phiIntCu);
            ar_thetaIntCu.push_back(thetaIntCu);
            ar_thetaIntCe.push_back(thetaIntCe);
            ar_thetaGlobalCe.push_back(thetaGlobalCe);
            ar_thetaGlobalCu.push_back(thetaGlobalCu);
            ar_phiGlobalCe.push_back(phiGlobalCe);
            ar_phiGlobalCu.push_back(phiGlobalCu);
            ar_chamberIDCe.push_back(chamberIDCe);
            ar_chamberIDCu.push_back(chamberIDCu);
            ar_rollCe.push_back(rollCe);
            ar_rollCu.push_back(rollCu);
            ar_emtfSubsectorCu.push_back(emtfSubsectorCu);
            ar_emtfSubsectorCe.push_back(emtfSubsectorCe);
            ar_fillOccupancyCu.push_back(fillOccupancyCu);
            ar_fillOccupancyCe.push_back(fillOccupancyCe);
         //}

         h1_cluster_sizeCe->Fill(cluster_sizeCe);
         h1_cluster_sizeCu->Fill(cluster_sizeCu);
         h1_bxCe->Fill(bxCe);
         h1_bxCu->Fill(bxCu);
         h1_phiIntCe->Fill(phiIntCe);
         h1_phiIntCu->Fill(phiIntCu);
         h1_thetaIntCu->Fill(thetaIntCu);
         h1_thetaIntCe->Fill(thetaIntCe);
         h1_thetaGlobalCe->Fill(thetaGlobalCe);
         h1_thetaGlobalCu->Fill(thetaGlobalCu);
         h1_phiGlobalCe->Fill(phiGlobalCe);
         h1_phiGlobalCu->Fill(phiGlobalCu);
         h2_bx_emu_unpack->Fill(bxCe, bxCu);
         h2_phiInt_emu_unpack->Fill(phiIntCe, phiIntCu);
         h2_phiGlobal_emu_unpack->Fill(phiGlobalCe, phiGlobalCu);
         h2_thetaInt_emu_unpack->Fill(thetaIntCe, thetaIntCu);
         h2_thetaGlobal_emu_unpack->Fill(thetaGlobalCe, thetaGlobalCu);
         h2_chamberID_emu_unpack->Fill(chamberIDCe, chamberIDCu);
         h2_chamberID_vs_roll_emu->Fill(chamberIDCe, rollCe);
         h2_chamberID_vs_roll_unpack->Fill(chamberIDCu, rollCu);
         h2_occupancy_unpacker->Fill(emtfSubsectorCu, fillOccupancyCu);
         h2_occupancy_emulator->Fill(emtfSubsectorCe, fillOccupancyCe);

         if (bxCe == bxCu) {
            matches_bx = true;
            matches_unpacker_bx++;

            if (matches_unpacker_bx<2) {
               ar_cluster_sizeCe_bx.push_back(cluster_sizeCe);
               ar_cluster_sizeCu_bx.push_back(cluster_sizeCu);
               ar_bxCe_bx.push_back(bxCe);
               ar_bxCu_bx.push_back(bxCu);
               ar_phiIntCe_bx.push_back(phiIntCe);
               ar_phiIntCu_bx.push_back(phiIntCu);
               ar_thetaIntCu_bx.push_back(thetaIntCu);
               ar_thetaIntCe_bx.push_back(thetaIntCe);
               ar_thetaGlobalCe_bx.push_back(thetaGlobalCe);
               ar_thetaGlobalCu_bx.push_back(thetaGlobalCu);
               ar_phiGlobalCe_bx.push_back(phiGlobalCe);
               ar_phiGlobalCu_bx.push_back(phiGlobalCu);
               ar_chamberIDCe_bx.push_back(chamberIDCe);
               ar_chamberIDCu_bx.push_back(chamberIDCu);
               ar_rollCe_bx.push_back(rollCe);
               ar_rollCu_bx.push_back(rollCu);
               ar_emtfSubsectorCu_bx.push_back(emtfSubsectorCu);
               ar_emtfSubsectorCe_bx.push_back(emtfSubsectorCe);
               ar_fillOccupancyCu_bx.push_back(fillOccupancyCu);
               ar_fillOccupancyCe_bx.push_back(fillOccupancyCe);
            }

            h1_cluster_sizeCe_bx->Fill(cluster_sizeCe);
            h1_cluster_sizeCu_bx->Fill(cluster_sizeCu);
            h1_bxCe_bx->Fill(bxCe);
            h1_bxCu_bx->Fill(bxCu);
            h1_phiIntCe_bx->Fill(phiIntCe);
            h1_phiIntCu_bx->Fill(phiIntCu);
            h1_thetaIntCu_bx->Fill(thetaIntCu);
            h1_thetaIntCe_bx->Fill(thetaIntCe);
            h1_thetaGlobalCe_bx->Fill(thetaGlobalCe);
            h1_thetaGlobalCu_bx->Fill(thetaGlobalCu);
            h1_phiGlobalCe_bx->Fill(phiGlobalCe);
            h1_phiGlobalCu_bx->Fill(phiGlobalCu);

            h2_bx_emu_unpack_bx->Fill(bxCe, bxCu);
            h2_phiInt_emu_unpack_bx->Fill(phiIntCe, phiIntCu);
            h2_phiGlobal_emu_unpack_bx->Fill(phiGlobalCe, phiGlobalCu);
            h2_thetaInt_emu_unpack_bx->Fill(thetaIntCe, thetaIntCu);
            h2_thetaGlobal_emu_unpack_bx->Fill(thetaGlobalCe, thetaGlobalCu);
            h2_chamberID_emu_unpack_bx->Fill(chamberIDCe, chamberIDCu);

            h2_chamberID_vs_roll_emu_bx->Fill(chamberIDCe, rollCe);
            h2_chamberID_vs_roll_unpack_bx->Fill(chamberIDCu, rollCu);

            h2_occupancy_unpacker_bx->Fill(emtfSubsectorCu, fillOccupancyCu);
            h2_occupancy_emulator_bx->Fill(emtfSubsectorCe, fillOccupancyCe);

            if (phiIntCe == phiIntCu) {
               matches_bx_phi = true;
               matches_unpacker_bx_phi++;

               if (matches_unpacker_bx_phi<2) {
                  ar_cluster_sizeCe_bx_phi.push_back(cluster_sizeCe);
                  ar_cluster_sizeCu_bx_phi.push_back(cluster_sizeCu);
                  ar_bxCe_bx_phi.push_back(bxCe);
                  ar_bxCu_bx_phi.push_back(bxCu);
                  ar_phiIntCe_bx_phi.push_back(phiIntCe);
                  ar_phiIntCu_bx_phi.push_back(phiIntCu);
                  ar_thetaIntCu_bx_phi.push_back(thetaIntCu);
                  ar_thetaIntCe_bx_phi.push_back(thetaIntCe);
                  ar_thetaGlobalCe_bx_phi.push_back(thetaGlobalCe);
                  ar_thetaGlobalCu_bx_phi.push_back(thetaGlobalCu);
                  ar_phiGlobalCe_bx_phi.push_back(phiGlobalCe);
                  ar_phiGlobalCu_bx_phi.push_back(phiGlobalCu);
                  ar_chamberIDCe_bx_phi.push_back(chamberIDCe);
                  ar_chamberIDCu_bx_phi.push_back(chamberIDCu);
                  ar_rollCe_bx_phi.push_back(rollCe);
                  ar_rollCu_bx_phi.push_back(rollCu);
                  ar_emtfSubsectorCu_bx_phi.push_back(emtfSubsectorCu);
                  ar_emtfSubsectorCe_bx_phi.push_back(emtfSubsectorCe);
                  ar_fillOccupancyCu_bx_phi.push_back(fillOccupancyCu);
                  ar_fillOccupancyCe_bx_phi.push_back(fillOccupancyCe);
               }

               h1_cluster_sizeCe_bx_phi->Fill(cluster_sizeCe);
               h1_cluster_sizeCu_bx_phi->Fill(cluster_sizeCu);

               h1_bxCe_bx_phi->Fill(bxCe);
               h1_bxCu_bx_phi->Fill(bxCu);
               h1_phiIntCe_bx_phi->Fill(phiIntCe);
               h1_phiIntCu_bx_phi->Fill(phiIntCu);
               h1_thetaIntCu_bx_phi->Fill(thetaIntCu);
               h1_thetaIntCe_bx_phi->Fill(thetaIntCe);
               h1_thetaGlobalCe_bx_phi->Fill(thetaGlobalCe);
               h1_thetaGlobalCu_bx_phi->Fill(thetaGlobalCu);
               h1_phiGlobalCe_bx_phi->Fill(phiGlobalCe);
               h1_phiGlobalCu_bx_phi->Fill(phiGlobalCu);

               h2_bx_emu_unpack_bx_phi->Fill(bxCe, bxCu);
               h2_phiInt_emu_unpack_bx_phi->Fill(phiIntCe, phiIntCu);
               h2_phiGlobal_emu_unpack_bx_phi->Fill(phiGlobalCe, phiGlobalCu);
               h2_thetaInt_emu_unpack_bx_phi->Fill(thetaIntCe, thetaIntCu);
               h2_thetaGlobal_emu_unpack_bx_phi->Fill(thetaGlobalCe, thetaGlobalCu);
               h2_chamberID_emu_unpack_bx_phi->Fill(chamberIDCe, chamberIDCu);

               h2_chamberID_vs_roll_emu_bx_phi->Fill(chamberIDCe, rollCe);
               h2_chamberID_vs_roll_unpack_bx_phi->Fill(chamberIDCu, rollCu);

               h2_occupancy_unpacker_bx_phi->Fill(emtfSubsectorCu, fillOccupancyCu);
               h2_occupancy_emulator_bx_phi->Fill(emtfSubsectorCe, fillOccupancyCe);

            } else {  // if (phiIntCe == phiIntCu) 
               matches_bx_Offphi = true;
               matches_unpacker_bx_Offphi++;

               if (matches_unpacker_bx_Offphi<2) {
                  ar_cluster_sizeCe_bx_Offphi.push_back(cluster_sizeCe);
                  ar_cluster_sizeCu_bx_Offphi.push_back(cluster_sizeCu);
                  ar_bxCe_bx_Offphi.push_back(bxCe);
                  ar_bxCu_bx_Offphi.push_back(bxCu);
                  ar_phiIntCe_bx_Offphi.push_back(phiIntCe);
                  ar_phiIntCu_bx_Offphi.push_back(phiIntCu);
                  ar_thetaIntCu_bx_Offphi.push_back(thetaIntCu);
                  ar_thetaIntCe_bx_Offphi.push_back(thetaIntCe);
                  ar_thetaGlobalCe_bx_Offphi.push_back(thetaGlobalCe);
                  ar_thetaGlobalCu_bx_Offphi.push_back(thetaGlobalCu);
                  ar_phiGlobalCe_bx_Offphi.push_back(phiGlobalCe);
                  ar_phiGlobalCu_bx_Offphi.push_back(phiGlobalCu);
                  ar_chamberIDCe_bx_Offphi.push_back(chamberIDCe);
                  ar_chamberIDCu_bx_Offphi.push_back(chamberIDCu);
                  ar_rollCe_bx_Offphi.push_back(rollCe);
                  ar_rollCu_bx_Offphi.push_back(rollCu);
                  ar_emtfSubsectorCu_bx_Offphi.push_back(emtfSubsectorCu);
                  ar_emtfSubsectorCe_bx_Offphi.push_back(emtfSubsectorCe);
                  ar_fillOccupancyCu_bx_Offphi.push_back(fillOccupancyCu);
                  ar_fillOccupancyCe_bx_Offphi.push_back(fillOccupancyCe);
               }

               h1_cluster_sizeCe_bx_Offphi->Fill(cluster_sizeCe);
               h1_cluster_sizeCu_bx_Offphi->Fill(cluster_sizeCu);
               h1_bxCe_bx_Offphi->Fill(bxCe);
               h1_bxCu_bx_Offphi->Fill(bxCu);
               h1_phiIntCe_bx_Offphi->Fill(phiIntCe);
               h1_phiIntCu_bx_Offphi->Fill(phiIntCu);
               h1_thetaIntCu_bx_Offphi->Fill(thetaIntCu);
               h1_thetaIntCe_bx_Offphi->Fill(thetaIntCe);
               h1_thetaGlobalCe_bx_Offphi->Fill(thetaGlobalCe);
               h1_thetaGlobalCu_bx_Offphi->Fill(thetaGlobalCu);
               h1_phiGlobalCe_bx_Offphi->Fill(phiGlobalCe);
               h1_phiGlobalCu_bx_Offphi->Fill(phiGlobalCu);

               h2_bx_emu_unpack_bx_Offphi->Fill(bxCe, bxCu);
               h2_phiInt_emu_unpack_bx_Offphi->Fill(phiIntCe, phiIntCu);
               h2_phiGlobal_emu_unpack_bx_Offphi->Fill(phiGlobalCe, phiGlobalCu);
               h2_thetaInt_emu_unpack_bx_Offphi->Fill(thetaIntCe, thetaIntCu);
               h2_thetaGlobal_emu_unpack_bx_Offphi->Fill(thetaGlobalCe, thetaGlobalCu);
               h2_chamberID_emu_unpack_bx_Offphi->Fill(chamberIDCe, chamberIDCu);

               h2_chamberID_vs_roll_emu_bx_Offphi->Fill(chamberIDCe, rollCe);
               h2_chamberID_vs_roll_unpack_bx_Offphi->Fill(chamberIDCu, rollCu);

               h2_occupancy_unpacker_bx_Offphi->Fill(emtfSubsectorCu, fillOccupancyCu);
               h2_occupancy_emulator_bx_Offphi->Fill(emtfSubsectorCe, fillOccupancyCe);
            } // END else of (phiIntCe == phiIntCu)

            if (thetaIntCe == thetaIntCu) {
               matches_bx_theta = true;
               matches_unpacker_bx_theta++;

               if (matches_unpacker_bx_theta<2) {
                  ar_cluster_sizeCe_bx_theta.push_back(cluster_sizeCe);
                  ar_cluster_sizeCu_bx_theta.push_back(cluster_sizeCu);
                  ar_bxCe_bx_theta.push_back(bxCe);
                  ar_bxCu_bx_theta.push_back(bxCu);
                  ar_phiIntCe_bx_theta.push_back(phiIntCe);
                  ar_phiIntCu_bx_theta.push_back(phiIntCu);
                  ar_thetaIntCu_bx_theta.push_back(thetaIntCu);
                  ar_thetaIntCe_bx_theta.push_back(thetaIntCe);
                  ar_thetaGlobalCe_bx_theta.push_back(thetaGlobalCe);
                  ar_thetaGlobalCu_bx_theta.push_back(thetaGlobalCu);
                  ar_phiGlobalCe_bx_theta.push_back(phiGlobalCe);
                  ar_phiGlobalCu_bx_theta.push_back(phiGlobalCu);
                  ar_chamberIDCe_bx_theta.push_back(chamberIDCe);
                  ar_chamberIDCu_bx_theta.push_back(chamberIDCu);
                  ar_rollCe_bx_theta.push_back(rollCe);
                  ar_rollCu_bx_theta.push_back(rollCu);
                  ar_emtfSubsectorCu_bx_theta.push_back(emtfSubsectorCu);
                  ar_emtfSubsectorCe_bx_theta.push_back(emtfSubsectorCe);
                  ar_fillOccupancyCu_bx_theta.push_back(fillOccupancyCu);
                  ar_fillOccupancyCe_bx_theta.push_back(fillOccupancyCe);
               }

               h1_cluster_sizeCe_bx_theta->Fill(cluster_sizeCe);
               h1_cluster_sizeCu_bx_theta->Fill(cluster_sizeCu);

               h1_bxCe_bx_theta->Fill(bxCe);
               h1_bxCu_bx_theta->Fill(bxCu);
               h1_phiIntCe_bx_theta->Fill(phiIntCe);
               h1_phiIntCu_bx_theta->Fill(phiIntCu);
               h1_thetaIntCu_bx_theta->Fill(thetaIntCu);
               h1_thetaIntCe_bx_theta->Fill(thetaIntCe);
               h1_thetaGlobalCe_bx_theta->Fill(thetaGlobalCe);
               h1_thetaGlobalCu_bx_theta->Fill(thetaGlobalCu);
               h1_phiGlobalCe_bx_theta->Fill(phiGlobalCe);
               h1_phiGlobalCu_bx_theta->Fill(phiGlobalCu);

               h2_bx_emu_unpack_bx_theta->Fill(bxCe, bxCu);
               h2_phiInt_emu_unpack_bx_theta->Fill(phiIntCe, phiIntCu);
               h2_phiGlobal_emu_unpack_bx_theta->Fill(phiGlobalCe, phiGlobalCu);
               h2_thetaInt_emu_unpack_bx_theta->Fill(thetaIntCe, thetaIntCu);
               h2_thetaGlobal_emu_unpack_bx_theta->Fill(thetaGlobalCe, thetaGlobalCu);
               h2_chamberID_emu_unpack_bx_theta->Fill(chamberIDCe, chamberIDCu);

               h2_chamberID_vs_roll_emu_bx_theta->Fill(chamberIDCe, rollCe);
               h2_chamberID_vs_roll_unpack_bx_theta->Fill(chamberIDCu, rollCu);

               h2_occupancy_unpacker_bx_theta->Fill(emtfSubsectorCu, fillOccupancyCu);
               h2_occupancy_emulator_bx_theta->Fill(emtfSubsectorCe, fillOccupancyCe);

            } else {  // if (thetaIntCe == thetaIntCu) 
               matches_bx_Offtheta = true;
               matches_unpacker_bx_Offtheta++;

               if (matches_unpacker_bx_Offtheta<2) {
                  ar_cluster_sizeCe_bx_Offtheta.push_back(cluster_sizeCe);
                  ar_cluster_sizeCu_bx_Offtheta.push_back(cluster_sizeCu);
                  ar_bxCe_bx_Offtheta.push_back(bxCe);
                  ar_bxCu_bx_Offtheta.push_back(bxCu);
                  ar_phiIntCe_bx_Offtheta.push_back(phiIntCe);
                  ar_phiIntCu_bx_Offtheta.push_back(phiIntCu);
                  ar_thetaIntCu_bx_Offtheta.push_back(thetaIntCu);
                  ar_thetaIntCe_bx_Offtheta.push_back(thetaIntCe);
                  ar_thetaGlobalCe_bx_Offtheta.push_back(thetaGlobalCe);
                  ar_thetaGlobalCu_bx_Offtheta.push_back(thetaGlobalCu);
                  ar_phiGlobalCe_bx_Offtheta.push_back(phiGlobalCe);
                  ar_phiGlobalCu_bx_Offtheta.push_back(phiGlobalCu);
                  ar_chamberIDCe_bx_Offtheta.push_back(chamberIDCe);
                  ar_chamberIDCu_bx_Offtheta.push_back(chamberIDCu);
                  ar_rollCe_bx_Offtheta.push_back(rollCe);
                  ar_rollCu_bx_Offtheta.push_back(rollCu);
                  ar_emtfSubsectorCu_bx_Offtheta.push_back(emtfSubsectorCu);
                  ar_emtfSubsectorCe_bx_Offtheta.push_back(emtfSubsectorCe);
                  ar_fillOccupancyCu_bx_Offtheta.push_back(fillOccupancyCu);
                  ar_fillOccupancyCe_bx_Offtheta.push_back(fillOccupancyCe);
               }

               h1_cluster_sizeCe_bx_Offtheta->Fill(cluster_sizeCe);
               h1_cluster_sizeCu_bx_Offtheta->Fill(cluster_sizeCu);
               h1_bxCe_bx_Offtheta->Fill(bxCe);
               h1_bxCu_bx_Offtheta->Fill(bxCu);
               h1_phiIntCe_bx_Offtheta->Fill(phiIntCe);
               h1_phiIntCu_bx_Offtheta->Fill(phiIntCu);
               h1_thetaIntCu_bx_Offtheta->Fill(thetaIntCu);
               h1_thetaIntCe_bx_Offtheta->Fill(thetaIntCe);
               h1_thetaGlobalCe_bx_Offtheta->Fill(thetaGlobalCe);
               h1_thetaGlobalCu_bx_Offtheta->Fill(thetaGlobalCu);
               h1_phiGlobalCe_bx_Offtheta->Fill(phiGlobalCe);
               h1_phiGlobalCu_bx_Offtheta->Fill(phiGlobalCu);

               h2_bx_emu_unpack_bx_Offtheta->Fill(bxCe, bxCu);
               h2_phiInt_emu_unpack_bx_Offtheta->Fill(phiIntCe, phiIntCu);
               h2_phiGlobal_emu_unpack_bx_Offtheta->Fill(phiGlobalCe, phiGlobalCu);
               h2_thetaInt_emu_unpack_bx_Offtheta->Fill(thetaIntCe, thetaIntCu);
               h2_thetaGlobal_emu_unpack_bx_Offtheta->Fill(thetaGlobalCe, thetaGlobalCu);
               h2_chamberID_emu_unpack_bx_Offtheta->Fill(chamberIDCe, chamberIDCu);

               h2_chamberID_vs_roll_emu_bx_Offtheta->Fill(chamberIDCe, rollCe);
               h2_chamberID_vs_roll_unpack_bx_Offtheta->Fill(chamberIDCu, rollCu);

               h2_occupancy_unpacker_bx_Offtheta->Fill(emtfSubsectorCu, fillOccupancyCu);
               h2_occupancy_emulator_bx_Offtheta->Fill(emtfSubsectorCe, fillOccupancyCe);
            } // END else of (thetaIntCe == thetaIntCu)
         } // END: if (bxCe == bxCu)
      } // END of matching region, station, ring, sector and subsector if condition

      if (matches_unpacker == 1) {
         h1_cluster_sizeCe_oneHit->Fill(ar_cluster_sizeCe[0]);
         h1_cluster_sizeCu_oneHit->Fill(ar_cluster_sizeCu[0]);
         h1_bxCe_oneHit->Fill(ar_bxCe[0]);
         h1_bxCu_oneHit->Fill(ar_bxCu[0]);
         h1_phiIntCe_oneHit->Fill(ar_phiIntCe[0]);
         h1_phiIntCu_oneHit->Fill(ar_phiIntCu[0]);
         h1_thetaIntCu_oneHit->Fill(ar_thetaIntCu[0]);
         h1_thetaIntCe_oneHit->Fill(ar_thetaIntCe[0]);
         h1_thetaGlobalCe_oneHit->Fill(ar_thetaGlobalCe[0]);
         h1_thetaGlobalCu_oneHit->Fill(ar_thetaGlobalCu[0]);
         h1_phiGlobalCe_oneHit->Fill(ar_phiGlobalCe[0]);
         h1_phiGlobalCu_oneHit->Fill(ar_phiGlobalCu[0]);
         h2_bx_emu_unpack_oneHit->Fill(ar_bxCe[0], ar_bxCu[0]);
         h2_phiInt_emu_unpack_oneHit->Fill(ar_phiIntCe[0], ar_phiIntCu[0]);
         h2_phiGlobal_emu_unpack_oneHit->Fill(ar_phiGlobalCe[0], ar_phiGlobalCu[0]);
         h2_thetaInt_emu_unpack_oneHit->Fill(ar_thetaIntCe[0], ar_thetaIntCu[0]);
         h2_thetaGlobal_emu_unpack_oneHit->Fill(ar_thetaGlobalCe[0], ar_thetaGlobalCu[0]);
         h2_chamberID_emu_unpack_oneHit->Fill(ar_chamberIDCe[0], ar_chamberIDCu[0]);
         h2_chamberID_vs_roll_emu_oneHit->Fill(ar_chamberIDCe[0], ar_rollCe[0]);
         h2_chamberID_vs_roll_unpack_oneHit->Fill(ar_chamberIDCu[0], ar_rollCu[0]);
         h2_occupancy_unpacker_oneHit->Fill(ar_emtfSubsectorCu[0], ar_fillOccupancyCu[0]);
         h2_occupancy_emulator_oneHit->Fill(ar_emtfSubsectorCe[0], ar_fillOccupancyCe[0]);
      } //(matches_unpacker == 1)

      if (matches_unpacker_bx == 1) {
         h1_cluster_sizeCe_bx_oneHit->Fill(ar_cluster_sizeCe_bx[0]);
         h1_cluster_sizeCu_bx_oneHit->Fill(ar_cluster_sizeCu_bx[0]);
         h1_bxCe_bx_oneHit->Fill(ar_bxCe_bx[0]);
         h1_bxCu_bx_oneHit->Fill(ar_bxCu_bx[0]);
         h1_phiIntCe_bx_oneHit->Fill(ar_phiIntCe_bx[0]);
         h1_phiIntCu_bx_oneHit->Fill(ar_phiIntCu_bx[0]);
         h1_thetaIntCu_bx_oneHit->Fill(ar_thetaIntCu_bx[0]);
         h1_thetaIntCe_bx_oneHit->Fill(ar_thetaIntCe_bx[0]);
         h1_thetaGlobalCe_bx_oneHit->Fill(ar_thetaGlobalCe_bx[0]);
         h1_thetaGlobalCu_bx_oneHit->Fill(ar_thetaGlobalCu_bx[0]);
         h1_phiGlobalCe_bx_oneHit->Fill(ar_phiGlobalCe_bx[0]);
         h1_phiGlobalCu_bx_oneHit->Fill(ar_phiGlobalCu_bx[0]);
         h2_bx_emu_unpack_bx_oneHit->Fill(ar_bxCe_bx[0], ar_bxCu_bx[0]);
         h2_phiInt_emu_unpack_bx_oneHit->Fill(ar_phiIntCe_bx[0], ar_phiIntCu_bx[0]);
         h2_phiGlobal_emu_unpack_bx_oneHit->Fill(ar_phiGlobalCe_bx[0], ar_phiGlobalCu_bx[0]);
         h2_thetaInt_emu_unpack_bx_oneHit->Fill(ar_thetaIntCe_bx[0], ar_thetaIntCu_bx[0]);
         h2_thetaGlobal_emu_unpack_bx_oneHit->Fill(ar_thetaGlobalCe_bx[0], ar_thetaGlobalCu_bx[0]);
         h2_chamberID_emu_unpack_bx_oneHit->Fill(ar_chamberIDCe_bx[0], ar_chamberIDCu_bx[0]);
         h2_chamberID_vs_roll_emu_bx_oneHit->Fill(ar_chamberIDCe_bx[0], ar_rollCe_bx[0]);
         h2_chamberID_vs_roll_unpack_bx_oneHit->Fill(ar_chamberIDCu_bx[0], ar_rollCu_bx[0]);
         h2_occupancy_unpacker_bx_oneHit->Fill(ar_emtfSubsectorCu_bx[0], ar_fillOccupancyCu_bx[0]);
         h2_occupancy_emulator_bx_oneHit->Fill(ar_emtfSubsectorCe_bx[0], ar_fillOccupancyCe_bx[0]);
      }  // matches_unpacker_bx == 1
      if (matches_unpacker_bx_phi==1) {
         h1_cluster_sizeCe_bx_phi_oneHit->Fill(ar_cluster_sizeCe_bx_phi[0]);
         h1_cluster_sizeCu_bx_phi_oneHit->Fill(ar_cluster_sizeCu_bx_phi[0]);
         h1_bxCe_bx_phi_oneHit->Fill(ar_bxCe_bx_phi[0]);
         h1_bxCu_bx_phi_oneHit->Fill(ar_bxCu_bx_phi[0]);
         h1_phiIntCe_bx_phi_oneHit->Fill(ar_phiIntCe_bx_phi[0]);
         h1_phiIntCu_bx_phi_oneHit->Fill(ar_phiIntCu_bx_phi[0]);
         h1_thetaIntCu_bx_phi_oneHit->Fill(ar_thetaIntCu_bx_phi[0]);
         h1_thetaIntCe_bx_phi_oneHit->Fill(ar_thetaIntCe_bx_phi[0]);
         h1_thetaGlobalCe_bx_phi_oneHit->Fill(ar_thetaGlobalCe_bx_phi[0]);
         h1_thetaGlobalCu_bx_phi_oneHit->Fill(ar_thetaGlobalCu_bx_phi[0]);
         h1_phiGlobalCe_bx_phi_oneHit->Fill(ar_phiGlobalCe_bx_phi[0]);
         h1_phiGlobalCu_bx_phi_oneHit->Fill(ar_phiGlobalCu_bx_phi[0]);
         h2_bx_emu_unpack_bx_phi_oneHit->Fill(ar_bxCe_bx_phi[0], ar_bxCu_bx_phi[0]);
         h2_phiInt_emu_unpack_bx_phi_oneHit->Fill(ar_phiIntCe_bx_phi[0], ar_phiIntCu_bx_phi[0]);
         h2_phiGlobal_emu_unpack_bx_phi_oneHit->Fill(ar_phiGlobalCe_bx_phi[0], ar_phiGlobalCu_bx_phi[0]);
         h2_thetaInt_emu_unpack_bx_phi_oneHit->Fill(ar_thetaIntCe_bx_phi[0], ar_thetaIntCu_bx_phi[0]);
         h2_thetaGlobal_emu_unpack_bx_phi_oneHit->Fill(ar_thetaGlobalCe_bx_phi[0], ar_thetaGlobalCu_bx_phi[0]);
         h2_chamberID_emu_unpack_bx_phi_oneHit->Fill(ar_chamberIDCe_bx_phi[0], ar_chamberIDCu_bx_phi[0]);
         h2_chamberID_vs_roll_emu_bx_phi_oneHit->Fill(ar_chamberIDCe_bx_phi[0], ar_rollCe_bx_phi[0]);
         h2_chamberID_vs_roll_unpack_bx_phi_oneHit->Fill(ar_chamberIDCu_bx_phi[0], ar_rollCu_bx_phi[0]);
         h2_occupancy_unpacker_bx_phi_oneHit->Fill(ar_emtfSubsectorCu_bx_phi[0], ar_fillOccupancyCu_bx_phi[0]);
         h2_occupancy_emulator_bx_phi_oneHit->Fill(ar_emtfSubsectorCe_bx_phi[0], ar_fillOccupancyCe_bx_phi[0]);
      }

      if (matches_unpacker_bx_Offphi==1) {
         h1_cluster_sizeCe_bx_Offphi_oneHit->Fill(ar_cluster_sizeCe_bx_Offphi[0]);
         h1_cluster_sizeCu_bx_Offphi_oneHit->Fill(ar_cluster_sizeCu_bx_Offphi[0]);
         h1_bxCe_bx_Offphi_oneHit->Fill(ar_bxCe_bx_Offphi[0]);
         h1_bxCu_bx_Offphi_oneHit->Fill(ar_bxCu_bx_Offphi[0]);
         h1_phiIntCe_bx_Offphi_oneHit->Fill(ar_phiIntCe_bx_Offphi[0]);
         h1_phiIntCu_bx_Offphi_oneHit->Fill(ar_phiIntCu_bx_Offphi[0]);
         h1_thetaIntCu_bx_Offphi_oneHit->Fill(ar_thetaIntCu_bx_Offphi[0]);
         h1_thetaIntCe_bx_Offphi_oneHit->Fill(ar_thetaIntCe_bx_Offphi[0]);
         h1_thetaGlobalCe_bx_Offphi_oneHit->Fill(ar_thetaGlobalCe_bx_Offphi[0]);
         h1_thetaGlobalCu_bx_Offphi_oneHit->Fill(ar_thetaGlobalCu_bx_Offphi[0]);
         h1_phiGlobalCe_bx_Offphi_oneHit->Fill(ar_phiGlobalCe_bx_Offphi[0]);
         h1_phiGlobalCu_bx_Offphi_oneHit->Fill(ar_phiGlobalCu_bx_Offphi[0]);
         h2_bx_emu_unpack_bx_Offphi_oneHit->Fill(ar_bxCe_bx_Offphi[0], ar_bxCu_bx_Offphi[0]);
         h2_phiInt_emu_unpack_bx_Offphi_oneHit->Fill(ar_phiIntCe_bx_Offphi[0], ar_phiIntCu_bx_Offphi[0]);
         h2_phiGlobal_emu_unpack_bx_Offphi_oneHit->Fill(ar_phiGlobalCe_bx_Offphi[0], ar_phiGlobalCu_bx_Offphi[0]);
         h2_thetaInt_emu_unpack_bx_Offphi_oneHit->Fill(ar_thetaIntCe_bx_Offphi[0], ar_thetaIntCu_bx_Offphi[0]);
         h2_thetaGlobal_emu_unpack_bx_Offphi_oneHit->Fill(ar_thetaGlobalCe_bx_Offphi[0], ar_thetaGlobalCu_bx_Offphi[0]);
         h2_chamberID_emu_unpack_bx_Offphi_oneHit->Fill(ar_chamberIDCe_bx_Offphi[0], ar_chamberIDCu_bx_Offphi[0]);
         h2_chamberID_vs_roll_emu_bx_Offphi_oneHit->Fill(ar_chamberIDCe_bx_Offphi[0], ar_rollCe_bx_Offphi[0]);
         h2_chamberID_vs_roll_unpack_bx_Offphi_oneHit->Fill(ar_chamberIDCu_bx_Offphi[0], ar_rollCu_bx_Offphi[0]);
         h2_occupancy_unpacker_bx_Offphi_oneHit->Fill(ar_emtfSubsectorCu_bx_Offphi[0], ar_fillOccupancyCu_bx_Offphi[0]);
         h2_occupancy_emulator_bx_Offphi_oneHit->Fill(ar_emtfSubsectorCe_bx_Offphi[0], ar_fillOccupancyCe_bx_Offphi[0]);
      }// matches_unpacker_bx_Offphi==1
    } // END: of for(auto& cppf_digis2 : *CppfDigis2)
  } // END: for(auto& cppf_digis : *CppfDigis1)
  if (matches == true) h1_matches_unpacker->Fill(matches_unpacker);
  if (matches_bx == true) h1_matches_unpacker_bx->Fill(matches_unpacker_bx);
  if (matches_bx_phi == true) h1_matches_unpacker_bx_phi->Fill(matches_unpacker_bx_phi);
  if (matches_bx_Offphi == true) h1_matches_unpacker_bx_Offphi->Fill(matches_unpacker_bx_Offphi);
  if (matches_bx_theta == true) h1_matches_unpacker_bx_theta->Fill(matches_unpacker_bx_theta);
  if (matches_bx_Offtheta == true) h1_matches_unpacker_bx_Offtheta->Fill(matches_unpacker_bx_Offtheta);
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
  
  h1_matches_unpacker = fs->make<TH1D>("h1_matches_unpacker", "CPPFDigis total matched hits" , 50, 0. , 50.);
  h1_matches_unpacker_bx = fs->make<TH1D>("h1_matches_unpacker_bx", "CPPFDigis total matched hits with same BX" , 50, 0. , 50.);
  h1_matches_unpacker_bx_phi = fs->make<TH1D>("h1_matches_unpacker_bx_phi", "CPPFDigis total matched hits with same BX and Phi" , 50, 0. , 50.);
  h1_matches_unpacker_bx_Offphi = fs->make<TH1D>("h1_matches_unpacker_bx_Offphi", "CPPFDigis total matched hits with same BX and Not matched Phi" , 50, 0. , 50.);
  h1_matches_unpacker_bx_theta = fs->make<TH1D>("h1_matches_unpacker_bx_theta", "CPPFDigis total matched hits with same BX and theta" , 50, 0. , 50.);
  h1_matches_unpacker_bx_Offtheta = fs->make<TH1D>("h1_matches_unpacker_bx_Offtheta", "CPPFDigis total matched hits with same BX and Not matched theta" , 50, 0. , 50.);
  h1_cluster_sizeCe = fs->make<TH1D>("h1_cluster_sizeCe", "Cluster size of emulator", 10, 0., 10.);
  h1_cluster_sizeCe_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_oneHit", "Cluster size of emulator", 10, 0., 10.);
  h1_cluster_sizeCe_bx = fs->make<TH1D>("h1_cluster_sizeCe_bx", "Cluster size of emulator with same BX", 10, 0., 10.);
  h1_cluster_sizeCe_bx_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_bx_oneHit", "Cluster size of emulator with same BX", 10, 0., 10.);
  h1_cluster_sizeCe_bx_phi = fs->make<TH1D>("h1_cluster_sizeCe_bx_phi", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_phi_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_bx_phi_oneHit", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_Offphi = fs->make<TH1D>("h1_cluster_sizeCe_bx_Offphi", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_bx_Offphi_oneHit", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_theta = fs->make<TH1D>("h1_cluster_sizeCe_bx_theta", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_theta_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_bx_theta_oneHit", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_Offtheta = fs->make<TH1D>("h1_cluster_sizeCe_bx_Offtheta", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_cluster_sizeCe_bx_Offtheta_oneHit", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCu = fs->make<TH1D>("h1_cluster_sizeCu", "Cluster size of emulator", 10, 0., 10.);
  h1_cluster_sizeCu_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_oneHit", "Cluster size of emulator", 10, 0., 10.);
  h1_cluster_sizeCu_bx = fs->make<TH1D>("h1_cluster_sizeCu_bx", "Cluster size of emulator with same BX", 10, 0., 10.);
  h1_cluster_sizeCu_bx_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_bx_oneHit", "Cluster size of emulator with same BX", 10, 0., 10.);
  h1_cluster_sizeCu_bx_phi = fs->make<TH1D>("h1_cluster_sizeCu_bx_phi", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_phi_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_bx_phi_oneHit", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_Offphi = fs->make<TH1D>("h1_cluster_sizeCu_bx_Offphi", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_bx_Offphi_oneHit", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_theta = fs->make<TH1D>("h1_cluster_sizeCu_bx_theta", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_theta_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_bx_theta_oneHit", "Cluster size of emulator with same BX and Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_Offtheta = fs->make<TH1D>("h1_cluster_sizeCu_bx_Offtheta", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_cluster_sizeCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_cluster_sizeCu_bx_Offtheta_oneHit", "Cluster size of emulator with same BX and Not matched Phi", 10, 0., 10.);
  h1_bxCe = fs->make<TH1D>("h1_bxCe", "h1_bxCe", 8, -4., 4.);
  h1_bxCe_oneHit = fs->make<TH1D>("h1_bxCe_oneHit", "h1_bxCe", 8, -4., 4.);
  h1_bxCe_bx = fs->make<TH1D>("h1_bxCe_bx", "h1_bxCe_bx", 8, -4., 4.);
  h1_bxCe_bx_oneHit = fs->make<TH1D>("h1_bxCe_bx_oneHit", "h1_bxCe_bx", 8, -4., 4.);
  h1_bxCe_bx_phi = fs->make<TH1D>("h1_bxCe_bx_phi", "h1_bxCe_bx_phi", 8, -4., 4.);
  h1_bxCe_bx_phi_oneHit = fs->make<TH1D>("h1_bxCe_bx_phi_oneHit", "h1_bxCe_bx_phi", 8, -4., 4.);
  h1_bxCe_bx_Offphi = fs->make<TH1D>("h1_bxCe_bx_Offphi", "h1_bxCe_bx_Offphi", 8, -4., 4.);
  h1_bxCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_bxCe_bx_Offphi_oneHit", "h1_bxCe_bx_Offphi", 8, -4., 4.);
  h1_bxCe_bx_theta = fs->make<TH1D>("h1_bxCe_bx_theta", "h1_bxCe_bx_theta", 8, -4., 4.);
  h1_bxCe_bx_theta_oneHit = fs->make<TH1D>("h1_bxCe_bx_theta_oneHit", "h1_bxCe_bx_theta", 8, -4., 4.);
  h1_bxCe_bx_Offtheta = fs->make<TH1D>("h1_bxCe_bx_Offtheta", "h1_bxCe_bx_Offtheta", 8, -4., 4.);
  h1_bxCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_bxCe_bx_Offtheta_oneHit", "h1_bxCe_bx_Offtheta", 8, -4., 4.);
  h1_bxCu = fs->make<TH1D>("h1_bxCu", "h1_bxCu", 8, -4., 4.);
  h1_bxCu_oneHit = fs->make<TH1D>("h1_bxCu_oneHit", "h1_bxCu", 8, -4., 4.);
  h1_bxCu_bx = fs->make<TH1D>("h1_bxCu_bx", "h1_bxCu_bx", 8, -4., 4.);
  h1_bxCu_bx_oneHit = fs->make<TH1D>("h1_bxCu_bx_oneHit", "h1_bxCu_bx", 8, -4., 4.);
  h1_bxCu_bx_phi = fs->make<TH1D>("h1_bxCu_bx_phi", "h1_bxCu_bx_phi", 8, -4., 4.);
  h1_bxCu_bx_phi_oneHit = fs->make<TH1D>("h1_bxCu_bx_phi_oneHit", "h1_bxCu_bx_phi", 8, -4., 4.);
  h1_bxCu_bx_Offphi = fs->make<TH1D>("h1_bxCu_bx_Offphi", "h1_bxCu_bx_Offphi", 8, -4., 4.);
  h1_bxCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_bxCu_bx_Offphi_oneHit", "h1_bxCu_bx_Offphi", 8, -4., 4.);
  h1_bxCu_bx_theta = fs->make<TH1D>("h1_bxCu_bx_theta", "h1_bxCu_bx_theta", 8, -4., 4.);
  h1_bxCu_bx_theta_oneHit = fs->make<TH1D>("h1_bxCu_bx_theta_oneHit", "h1_bxCu_bx_theta", 8, -4., 4.);
  h1_bxCu_bx_Offtheta = fs->make<TH1D>("h1_bxCu_bx_Offtheta", "h1_bxCu_bx_Offtheta", 8, -4., 4.);
  h1_bxCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_bxCu_bx_Offtheta_oneHit", "h1_bxCu_bx_Offtheta", 8, -4., 4.);
  h1_phiIntCe = fs->make<TH1D>("h1_phiIntCe", "h1_phiIntCe", 124, 0., 1240.);
  h1_phiIntCe_oneHit = fs->make<TH1D>("h1_phiIntCe_oneHit", "h1_phiIntCe", 124, 0., 1240.);
  h1_phiIntCe_bx = fs->make<TH1D>("h1_phiIntCe_bx", "h1_phiIntCe_bx", 124, 0., 1240.);
  h1_phiIntCe_bx_oneHit = fs->make<TH1D>("h1_phiIntCe_bx_oneHit", "h1_phiIntCe_bx", 124, 0., 1240.);
  h1_phiIntCe_bx_phi = fs->make<TH1D>("h1_phiIntCe_bx_phi", "h1_phiIntCe_bx_phi", 124, 0., 1240.);
  h1_phiIntCe_bx_phi_oneHit = fs->make<TH1D>("h1_phiIntCe_bx_phi_oneHit", "h1_phiIntCe_bx_phi", 124, 0., 1240.);
  h1_phiIntCe_bx_Offphi = fs->make<TH1D>("h1_phiIntCe_bx_Offphi", "h1_phiIntCe_bx_Offphi", 124, 0., 1240.);
  h1_phiIntCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_phiIntCe_bx_Offphi_oneHit", "h1_phiIntCe_bx_Offphi", 124, 0., 1240.);
  h1_phiIntCe_bx_theta = fs->make<TH1D>("h1_phiIntCe_bx_theta", "h1_phiIntCe_bx_theta", 124, 0., 1240.);
  h1_phiIntCe_bx_theta_oneHit = fs->make<TH1D>("h1_phiIntCe_bx_theta_oneHit", "h1_phiIntCe_bx_theta", 124, 0., 1240.);
  h1_phiIntCe_bx_Offtheta = fs->make<TH1D>("h1_phiIntCe_bx_Offtheta", "h1_phiIntCe_bx_Offtheta", 124, 0., 1240.);
  h1_phiIntCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_phiIntCe_bx_Offtheta_oneHit", "h1_phiIntCe_bx_Offtheta", 124, 0., 1240.);
  h1_phiIntCu = fs->make<TH1D>("h1_phiIntCu", "h1_phiIntCu", 124, 0., 1240.);
  h1_phiIntCu_oneHit = fs->make<TH1D>("h1_phiIntCu_oneHit", "h1_phiIntCu", 124, 0., 1240.);
  h1_phiIntCu_bx = fs->make<TH1D>("h1_phiIntCu_bx", "h1_phiIntCu_bx", 124, 0., 1240.);
  h1_phiIntCu_bx_oneHit = fs->make<TH1D>("h1_phiIntCu_bx_oneHit", "h1_phiIntCu_bx", 124, 0., 1240.);
  h1_phiIntCu_bx_phi = fs->make<TH1D>("h1_phiIntCu_bx_phi", "h1_phiIntCu_bx_phi", 124, 0., 1240.);
  h1_phiIntCu_bx_phi_oneHit = fs->make<TH1D>("h1_phiIntCu_bx_phi_oneHit", "h1_phiIntCu_bx_phi", 124, 0., 1240.);
  h1_phiIntCu_bx_Offphi = fs->make<TH1D>("h1_phiIntCu_bx_Offphi", "h1_phiIntCu_bx_Offphi", 124, 0., 1240.);
  h1_phiIntCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_phiIntCu_bx_Offphi_oneHit", "h1_phiIntCu_bx_Offphi", 124, 0., 1240.);
  h1_phiIntCu_bx_theta = fs->make<TH1D>("h1_phiIntCu_bx_theta", "h1_phiIntCu_bx_theta", 124, 0., 1240.);
  h1_phiIntCu_bx_theta_oneHit = fs->make<TH1D>("h1_phiIntCu_bx_theta_oneHit", "h1_phiIntCu_bx_theta", 124, 0., 1240.);
  h1_phiIntCu_bx_Offtheta = fs->make<TH1D>("h1_phiIntCu_bx_Offtheta", "h1_phiIntCu_bx_Offtheta", 124, 0., 1240.);
  h1_phiIntCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_phiIntCu_bx_Offtheta_oneHit", "h1_phiIntCu_bx_Offtheta", 124, 0., 1240.);
  h1_thetaIntCu = fs->make<TH1D>("h1_thetaIntCu", "h1_thetaIntCu", 32, 0., 32.);
  h1_thetaIntCu_oneHit = fs->make<TH1D>("h1_thetaIntCu_oneHit", "h1_thetaIntCu", 32, 0., 32.);
  h1_thetaIntCu_bx = fs->make<TH1D>("h1_thetaIntCu_bx", "h1_thetaIntCu_bx", 32, 0., 32.);
  h1_thetaIntCu_bx_oneHit = fs->make<TH1D>("h1_thetaIntCu_bx_oneHit", "h1_thetaIntCu_bx", 32, 0., 32.);
  h1_thetaIntCu_bx_phi = fs->make<TH1D>("h1_thetaIntCu_bx_phi", "h1_thetaIntCu_bx_phi", 32, 0., 32.);
  h1_thetaIntCu_bx_phi_oneHit = fs->make<TH1D>("h1_thetaIntCu_bx_phi_oneHit", "h1_thetaIntCu_bx_phi", 32, 0., 32.);
  h1_thetaIntCu_bx_Offphi = fs->make<TH1D>("h1_thetaIntCu_bx_Offphi", "h1_thetaIntCu_bx_Offphi", 32, 0., 32.);
  h1_thetaIntCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_thetaIntCu_bx_Offphi_oneHit", "h1_thetaIntCu_bx_Offphi", 32, 0., 32.);
  h1_thetaIntCu_bx_theta = fs->make<TH1D>("h1_thetaIntCu_bx_theta", "h1_thetaIntCu_bx_theta", 32, 0., 32.);
  h1_thetaIntCu_bx_theta_oneHit = fs->make<TH1D>("h1_thetaIntCu_bx_theta_oneHit", "h1_thetaIntCu_bx_theta", 32, 0., 32.);
  h1_thetaIntCu_bx_Offtheta = fs->make<TH1D>("h1_thetaIntCu_bx_Offtheta", "h1_thetaIntCu_bx_Offtheta", 32, 0., 32.);
  h1_thetaIntCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_thetaIntCu_bx_Offtheta_oneHit", "h1_thetaIntCu_bx_Offtheta", 32, 0., 32.);
  h1_thetaIntCe = fs->make<TH1D>("h1_thetaIntCe", "h1_thetaIntCe", 32, 0., 32.);
  h1_thetaIntCe_oneHit = fs->make<TH1D>("h1_thetaIntCe_oneHit", "h1_thetaIntCe", 32, 0., 32.);
  h1_thetaIntCe_bx = fs->make<TH1D>("h1_thetaIntCe_bx", "h1_thetaIntCe_bx", 32, 0., 32.);
  h1_thetaIntCe_bx_oneHit = fs->make<TH1D>("h1_thetaIntCe_bx_oneHit", "h1_thetaIntCe_bx", 32, 0., 32.);
  h1_thetaIntCe_bx_phi = fs->make<TH1D>("h1_thetaIntCe_bx_phi", "h1_thetaIntCe_bx_phi", 32, 0., 32.);
  h1_thetaIntCe_bx_phi_oneHit = fs->make<TH1D>("h1_thetaIntCe_bx_phi_oneHit", "h1_thetaIntCe_bx_phi", 32, 0., 32.);
  h1_thetaIntCe_bx_Offphi = fs->make<TH1D>("h1_thetaIntCe_bx_Offphi", "h1_thetaIntCe_bx_Offphi", 32, 0., 32.);
  h1_thetaIntCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_thetaIntCe_bx_Offphi_oneHit", "h1_thetaIntCe_bx_Offphi", 32, 0., 32.);
  h1_thetaIntCe_bx_theta = fs->make<TH1D>("h1_thetaIntCe_bx_theta", "h1_thetaIntCe_bx_theta", 32, 0., 32.);
  h1_thetaIntCe_bx_theta_oneHit = fs->make<TH1D>("h1_thetaIntCe_bx_theta_oneHit", "h1_thetaIntCe_bx_theta", 32, 0., 32.);
  h1_thetaIntCe_bx_Offtheta = fs->make<TH1D>("h1_thetaIntCe_bx_Offtheta", "h1_thetaIntCe_bx_Offtheta", 32, 0., 32.);
  h1_thetaIntCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_thetaIntCe_bx_Offtheta_oneHit", "h1_thetaIntCe_bx_Offtheta", 32, 0., 32.);
  h1_thetaGlobalCe = fs->make<TH1D>("h1_thetaGlobalCe", "h1_thetaGlobalCe", 360,-180.,180);
  h1_thetaGlobalCe_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_oneHit", "h1_thetaGlobalCe", 360,-180.,180);
  h1_thetaGlobalCe_bx = fs->make<TH1D>("h1_thetaGlobalCe_bx", "h1_thetaGlobalCe_bx", 360,-180.,180);
  h1_thetaGlobalCe_bx_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_bx_oneHit", "h1_thetaGlobalCe_bx", 360,-180.,180);
  h1_thetaGlobalCe_bx_phi = fs->make<TH1D>("h1_thetaGlobalCe_bx_phi", "h1_thetaGlobalCe_bx_phi", 360,-180.,180);
  h1_thetaGlobalCe_bx_phi_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_bx_phi_oneHit", "h1_thetaGlobalCe_bx_phi", 360,-180.,180);
  h1_thetaGlobalCe_bx_Offphi = fs->make<TH1D>("h1_thetaGlobalCe_bx_Offphi", "h1_thetaGlobalCe_bx_Offphi", 360,-180.,180);
  h1_thetaGlobalCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_bx_Offphi_oneHit", "h1_thetaGlobalCe_bx_Offphi", 360,-180.,180);
  h1_thetaGlobalCe_bx_theta = fs->make<TH1D>("h1_thetaGlobalCe_bx_theta", "h1_thetaGlobalCe_bx_theta", 360,-180.,180);
  h1_thetaGlobalCe_bx_theta_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_bx_theta_oneHit", "h1_thetaGlobalCe_bx_theta", 360,-180.,180);
  h1_thetaGlobalCe_bx_Offtheta = fs->make<TH1D>("h1_thetaGlobalCe_bx_Offtheta", "h1_thetaGlobalCe_bx_Offtheta", 360,-180.,180);
  h1_thetaGlobalCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_thetaGlobalCe_bx_Offtheta_oneHit", "h1_thetaGlobalCe_bx_Offtheta", 360,-180.,180);
  h1_thetaGlobalCu = fs->make<TH1D>("h1_thetaGlobalCu", "h1_thetaGlobalCu", 360,-180.,180);
  h1_thetaGlobalCu_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_oneHit", "h1_thetaGlobalCu", 360,-180.,180);
  h1_thetaGlobalCu_bx = fs->make<TH1D>("h1_thetaGlobalCu_bx", "h1_thetaGlobalCu_bx", 360,-180.,180);
  h1_thetaGlobalCu_bx_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_bx_oneHit", "h1_thetaGlobalCu_bx", 360,-180.,180);
  h1_thetaGlobalCu_bx_phi = fs->make<TH1D>("h1_thetaGlobalCu_bx_phi", "h1_thetaGlobalCu_bx_phi", 360,-180.,180);
  h1_thetaGlobalCu_bx_phi_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_bx_phi_oneHit", "h1_thetaGlobalCu_bx_phi", 360,-180.,180);
  h1_thetaGlobalCu_bx_Offphi = fs->make<TH1D>("h1_thetaGlobalCu_bx_Offphi", "h1_thetaGlobalCu_bx_Offphi", 360,-180.,180);
  h1_thetaGlobalCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_bx_Offphi_oneHit", "h1_thetaGlobalCu_bx_Offphi", 360,-180.,180);
  h1_thetaGlobalCu_bx_theta = fs->make<TH1D>("h1_thetaGlobalCu_bx_theta", "h1_thetaGlobalCu_bx_theta", 360,-180.,180);
  h1_thetaGlobalCu_bx_theta_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_bx_theta_oneHit", "h1_thetaGlobalCu_bx_theta", 360,-180.,180);
  h1_thetaGlobalCu_bx_Offtheta = fs->make<TH1D>("h1_thetaGlobalCu_bx_Offtheta", "h1_thetaGlobalCu_bx_Offtheta", 360,-180.,180);
  h1_thetaGlobalCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_thetaGlobalCu_bx_Offtheta_oneHit", "h1_thetaGlobalCu_bx_Offtheta", 360,-180.,180);
  h1_phiGlobalCe = fs->make<TH1D>("h1_phiGlobalCe", "h1_phiGlobalCe", 360,-180.,180);
  h1_phiGlobalCe_oneHit = fs->make<TH1D>("h1_phiGlobalCe_oneHit", "h1_phiGlobalCe", 360,-180.,180);
  h1_phiGlobalCe_bx = fs->make<TH1D>("h1_phiGlobalCe_bx", "h1_phiGlobalCe_bx", 360,-180.,180);
  h1_phiGlobalCe_bx_oneHit = fs->make<TH1D>("h1_phiGlobalCe_bx_oneHit", "h1_phiGlobalCe_bx", 360,-180.,180);
  h1_phiGlobalCe_bx_phi = fs->make<TH1D>("h1_phiGlobalCe_bx_phi", "h1_phiGlobalCe_bx_phi", 360,-180.,180);
  h1_phiGlobalCe_bx_phi_oneHit = fs->make<TH1D>("h1_phiGlobalCe_bx_phi_oneHit", "h1_phiGlobalCe_bx_phi", 360,-180.,180);
  h1_phiGlobalCe_bx_Offphi = fs->make<TH1D>("h1_phiGlobalCe_bx_Offphi", "h1_phiGlobalCe_bx_Offphi", 360,-180.,180);
  h1_phiGlobalCe_bx_Offphi_oneHit = fs->make<TH1D>("h1_phiGlobalCe_bx_Offphi_oneHit", "h1_phiGlobalCe_bx_Offphi", 360,-180.,180);
  h1_phiGlobalCe_bx_theta = fs->make<TH1D>("h1_phiGlobalCe_bx_theta", "h1_phiGlobalCe_bx_theta", 360,-180.,180);
  h1_phiGlobalCe_bx_theta_oneHit = fs->make<TH1D>("h1_phiGlobalCe_bx_theta_oneHit", "h1_phiGlobalCe_bx_theta", 360,-180.,180);
  h1_phiGlobalCe_bx_Offtheta = fs->make<TH1D>("h1_phiGlobalCe_bx_Offtheta", "h1_phiGlobalCe_bx_Offtheta", 360,-180.,180);
  h1_phiGlobalCe_bx_Offtheta_oneHit = fs->make<TH1D>("h1_phiGlobalCe_bx_Offtheta_oneHit", "h1_phiGlobalCe_bx_Offtheta", 360,-180.,180);
  h1_phiGlobalCu = fs->make<TH1D>("h1_phiGlobalCu", "h1_phiGlobalCu", 360,-180.,180);
  h1_phiGlobalCu_oneHit = fs->make<TH1D>("h1_phiGlobalCu_oneHit", "h1_phiGlobalCu", 360,-180.,180);
  h1_phiGlobalCu_bx = fs->make<TH1D>("h1_phiGlobalCu_bx", "h1_phiGlobalCu_bx", 360,-180.,180);
  h1_phiGlobalCu_bx_oneHit = fs->make<TH1D>("h1_phiGlobalCu_bx_oneHit", "h1_phiGlobalCu_bx", 360,-180.,180);
  h1_phiGlobalCu_bx_phi = fs->make<TH1D>("h1_phiGlobalCu_bx_phi", "h1_phiGlobalCu_bx_phi", 360,-180.,180);
  h1_phiGlobalCu_bx_phi_oneHit = fs->make<TH1D>("h1_phiGlobalCu_bx_phi_oneHit", "h1_phiGlobalCu_bx_phi", 360,-180.,180);
  h1_phiGlobalCu_bx_Offphi = fs->make<TH1D>("h1_phiGlobalCu_bx_Offphi", "h1_phiGlobalCu_bx_Offphi", 360,-180.,180);
  h1_phiGlobalCu_bx_Offphi_oneHit = fs->make<TH1D>("h1_phiGlobalCu_bx_Offphi_oneHit", "h1_phiGlobalCu_bx_Offphi", 360,-180.,180);
  h1_phiGlobalCu_bx_theta = fs->make<TH1D>("h1_phiGlobalCu_bx_theta", "h1_phiGlobalCu_bx_theta", 360,-180.,180);
  h1_phiGlobalCu_bx_theta_oneHit = fs->make<TH1D>("h1_phiGlobalCu_bx_theta_oneHit", "h1_phiGlobalCu_bx_theta", 360,-180.,180);
  h1_phiGlobalCu_bx_Offtheta = fs->make<TH1D>("h1_phiGlobalCu_bx_Offtheta", "h1_phiGlobalCu_bx_Offtheta", 360,-180.,180);
  h1_phiGlobalCu_bx_Offtheta_oneHit = fs->make<TH1D>("h1_phiGlobalCu_bx_Offtheta_oneHit", "h1_phiGlobalCu_bx_Offtheta", 360,-180.,180);

  // Defining soem TH2D type histograms
  h2_bx_emu_unpack = fs->make<TH2D>("h2_bx_emu_unpack", "h2_bx_emu_unpack", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_oneHit", "h2_bx_emu_unpack", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx = fs->make<TH2D>("h2_bx_emu_unpack_bx", "h2_bx_emu_unpack_bx", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_bx_oneHit", "h2_bx_emu_unpack_bx", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_phi = fs->make<TH2D>("h2_bx_emu_unpack_bx_phi", "h2_bx_emu_unpack_bx_phi", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_bx_phi_oneHit", "h2_bx_emu_unpack_bx_phi", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_bx_emu_unpack_bx_Offphi", "h2_bx_emu_unpack_bx_Offphi", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_bx_Offphi_oneHit", "h2_bx_emu_unpack_bx_Offphi", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_theta = fs->make<TH2D>("h2_bx_emu_unpack_bx_theta", "h2_bx_emu_unpack_bx_theta", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_bx_theta_oneHit", "h2_bx_emu_unpack_bx_theta", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_bx_emu_unpack_bx_Offtheta", "h2_bx_emu_unpack_bx_Offtheta", 8, -4., 4.,8, -4., 4.);
  h2_bx_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_bx_emu_unpack_bx_Offtheta_oneHit", "h2_bx_emu_unpack_bx_Offtheta", 8, -4., 4.,8, -4., 4.);
  h2_phiInt_emu_unpack = fs->make<TH2D>("h2_phiInt_emu_unpack", "h2_phiInt_emu_unpack", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_oneHit", "h2_phiInt_emu_unpack", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx = fs->make<TH2D>("h2_phiInt_emu_unpack_bx", "h2_phiInt_emu_unpack_bx", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_oneHit", "h2_phiInt_emu_unpack_bx", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_phi = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_phi", "h2_phiInt_emu_unpack_bx_phi", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_phi_oneHit", "h2_phiInt_emu_unpack_bx_phi", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_Offphi", "h2_phiInt_emu_unpack_bx_Offphi", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_Offphi_oneHit", "h2_phiInt_emu_unpack_bx_Offphi", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_theta = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_theta", "h2_phiInt_emu_unpack_bx_theta", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_theta_oneHit", "h2_phiInt_emu_unpack_bx_theta", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_Offtheta", "h2_phiInt_emu_unpack_bx_Offtheta", 124, 0., 1240.,124, 0., 1240.);
  h2_phiInt_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_phiInt_emu_unpack_bx_Offtheta_oneHit", "h2_phiInt_emu_unpack_bx_Offtheta", 124, 0., 1240.,124, 0., 1240.);
  h2_phiGlobal_emu_unpack = fs->make<TH2D>("h2_phiGlobal_emu_unpack", "h2_phiGlobal_emu_unpack", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_oneHit", "h2_phiGlobal_emu_unpack", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx", "h2_phiGlobal_emu_unpack_bx", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_oneHit", "h2_phiGlobal_emu_unpack_bx", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_phi = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_phi", "h2_phiGlobal_emu_unpack_bx_phi", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_phi_oneHit", "h2_phiGlobal_emu_unpack_bx_phi", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_Offphi", "h2_phiGlobal_emu_unpack_bx_Offphi", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_Offphi_oneHit", "h2_phiGlobal_emu_unpack_bx_Offphi", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_theta = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_theta", "h2_phiGlobal_emu_unpack_bx_theta", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_theta_oneHit", "h2_phiGlobal_emu_unpack_bx_theta", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_Offtheta", "h2_phiGlobal_emu_unpack_bx_Offtheta", 360,-180.,180, 360,-180.,180);
  h2_phiGlobal_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_phiGlobal_emu_unpack_bx_Offtheta_oneHit", "h2_phiGlobal_emu_unpack_bx_Offtheta", 360,-180.,180, 360,-180.,180);
  h2_thetaInt_emu_unpack = fs->make<TH2D>("h2_thetaInt_emu_unpack", "h2_thetaInt_emu_unpack", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_oneHit", "h2_thetaInt_emu_unpack", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx", "h2_thetaInt_emu_unpack_bx", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_oneHit", "h2_thetaInt_emu_unpack_bx", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_phi = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_phi", "h2_thetaInt_emu_unpack_bx_phi", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_phi_oneHit", "h2_thetaInt_emu_unpack_bx_phi", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_Offphi", "h2_thetaInt_emu_unpack_bx_Offphi", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_Offphi_oneHit", "h2_thetaInt_emu_unpack_bx_Offphi", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_theta = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_theta", "h2_thetaInt_emu_unpack_bx_theta", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_theta_oneHit", "h2_thetaInt_emu_unpack_bx_theta", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_Offtheta", "h2_thetaInt_emu_unpack_bx_Offtheta", 32, 0., 32., 32, 0., 32.);
  h2_thetaInt_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_thetaInt_emu_unpack_bx_Offtheta_oneHit", "h2_thetaInt_emu_unpack_bx_Offtheta", 32, 0., 32., 32, 0., 32.);
  h2_thetaGlobal_emu_unpack = fs->make<TH2D>("h2_thetaGlobal_emu_unpack", "h2_thetaGlobal_emu_unpack", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_oneHit", "h2_thetaGlobal_emu_unpack", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx", "h2_thetaGlobal_emu_unpack_bx", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_oneHit", "h2_thetaGlobal_emu_unpack_bx", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_phi = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_phi", "h2_thetaGlobal_emu_unpack_bx_phi", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_phi_oneHit", "h2_thetaGlobal_emu_unpack_bx_phi", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_Offphi", "h2_thetaGlobal_emu_unpack_bx_Offphi", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_Offphi_oneHit", "h2_thetaGlobal_emu_unpack_bx_Offphi", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_theta = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_theta", "h2_thetaGlobal_emu_unpack_bx_theta", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_theta_oneHit", "h2_thetaGlobal_emu_unpack_bx_theta", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_Offtheta", "h2_thetaGlobal_emu_unpack_bx_Offtheta", 360,-180.,180, 360,-180.,180);
  h2_thetaGlobal_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_thetaGlobal_emu_unpack_bx_Offtheta_oneHit", "h2_thetaGlobal_emu_unpack_bx_Offtheta", 360,-180.,180, 360,-180.,180);
  h2_chamberID_emu_unpack = fs->make<TH2D>("h2_chamberID_emu_unpack", "h2_chamberID_emu_unpack", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_oneHit", "h2_chamberID_emu_unpack", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx = fs->make<TH2D>("h2_chamberID_emu_unpack_bx", "h2_chamberID_emu_unpack_bx", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_oneHit", "h2_chamberID_emu_unpack_bx", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_phi = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_phi", "h2_chamberID_emu_unpack_bx_phi", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_phi_oneHit", "h2_chamberID_emu_unpack_bx_phi", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_Offphi = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_Offphi", "h2_chamberID_emu_unpack_bx_Offphi", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_Offphi_oneHit", "h2_chamberID_emu_unpack_bx_Offphi", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_theta = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_theta", "h2_chamberID_emu_unpack_bx_theta", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_theta_oneHit", "h2_chamberID_emu_unpack_bx_theta", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_Offtheta = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_Offtheta", "h2_chamberID_emu_unpack_bx_Offtheta", 36, 1, 37, 36, 1, 37);
  h2_chamberID_emu_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_chamberID_emu_unpack_bx_Offtheta_oneHit", "h2_chamberID_emu_unpack_bx_Offtheta", 36, 1, 37, 36, 1, 37);
  h2_chamberID_vs_roll_emu = fs->make<TH2D>("h2_chamberID_vs_roll_emu", "h2_chamberID_vs_roll_emu", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_oneHit", "h2_chamberID_vs_roll_emu", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx", "h2_chamberID_vs_roll_emu_bx", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_oneHit", "h2_chamberID_vs_roll_emu_bx", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_phi = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_phi", "h2_chamberID_vs_roll_emu_bx_phi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_phi_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_phi_oneHit", "h2_chamberID_vs_roll_emu_bx_phi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_Offphi = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_Offphi", "h2_chamberID_vs_roll_emu_bx_Offphi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_Offphi_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_Offphi_oneHit", "h2_chamberID_vs_roll_emu_bx_Offphi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_theta = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_theta", "h2_chamberID_vs_roll_emu_bx_theta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_theta_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_theta_oneHit", "h2_chamberID_vs_roll_emu_bx_theta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_Offtheta = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_Offtheta", "h2_chamberID_vs_roll_emu_bx_Offtheta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_emu_bx_Offtheta_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_emu_bx_Offtheta_oneHit", "h2_chamberID_vs_roll_emu_bx_Offtheta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack = fs->make<TH2D>("h2_chamberID_vs_roll_unpack", "h2_chamberID_vs_roll_unpack", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_oneHit", "h2_chamberID_vs_roll_unpack", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx", "h2_chamberID_vs_roll_unpack_bx", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_oneHit", "h2_chamberID_vs_roll_unpack_bx", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_phi = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_phi", "h2_chamberID_vs_roll_unpack_bx_phi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_phi_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_phi_oneHit", "h2_chamberID_vs_roll_unpack_bx_phi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_Offphi = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_Offphi", "h2_chamberID_vs_roll_unpack_bx_Offphi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_Offphi_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_Offphi_oneHit", "h2_chamberID_vs_roll_unpack_bx_Offphi", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_theta = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_theta", "h2_chamberID_vs_roll_unpack_bx_theta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_theta_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_theta_oneHit", "h2_chamberID_vs_roll_unpack_bx_theta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_Offtheta = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_Offtheta", "h2_chamberID_vs_roll_unpack_bx_Offtheta", 36, 1, 37, 4, 0, 4);
  h2_chamberID_vs_roll_unpack_bx_Offtheta_oneHit = fs->make<TH2D>("h2_chamberID_vs_roll_unpack_bx_Offtheta_oneHit", "h2_chamberID_vs_roll_unpack_bx_Offtheta", 36, 1, 37, 4, 0, 4);
  h2_occupancy_unpacker = fs->make<TH2D>("h2_occupancy_unpacker", "h2_occupancy_unpacker", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_oneHit", "h2_occupancy_unpacker", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx = fs->make<TH2D>("h2_occupancy_unpacker_bx", "h2_occupancy_unpacker_bx", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_bx_oneHit", "h2_occupancy_unpacker_bx", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_phi = fs->make<TH2D>("h2_occupancy_unpacker_bx_phi", "h2_occupancy_unpacker_bx_phi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_phi_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_bx_phi_oneHit", "h2_occupancy_unpacker_bx_phi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_Offphi = fs->make<TH2D>("h2_occupancy_unpacker_bx_Offphi", "h2_occupancy_unpacker_bx_Offphi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_Offphi_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_bx_Offphi_oneHit", "h2_occupancy_unpacker_bx_Offphi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_theta = fs->make<TH2D>("h2_occupancy_unpacker_bx_theta", "h2_occupancy_unpacker_bx_theta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_theta_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_bx_theta_oneHit", "h2_occupancy_unpacker_bx_theta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_Offtheta = fs->make<TH2D>("h2_occupancy_unpacker_bx_Offtheta", "h2_occupancy_unpacker_bx_Offtheta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_unpacker_bx_Offtheta_oneHit = fs->make<TH2D>("h2_occupancy_unpacker_bx_Offtheta_oneHit", "h2_occupancy_unpacker_bx_Offtheta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator = fs->make<TH2D>("h2_occupancy_emulator", "h2_occupancy_emulator", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_oneHit = fs->make<TH2D>("h2_occupancy_emulator_oneHit", "h2_occupancy_emulator", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx = fs->make<TH2D>("h2_occupancy_emulator_bx", "h2_occupancy_emulator_bx", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_oneHit = fs->make<TH2D>("h2_occupancy_emulator_bx_oneHit", "h2_occupancy_emulator_bx", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_phi = fs->make<TH2D>("h2_occupancy_emulator_bx_phi", "h2_occupancy_emulator_bx_phi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_phi_oneHit = fs->make<TH2D>("h2_occupancy_emulator_bx_phi_oneHit", "h2_occupancy_emulator_bx_phi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_Offphi = fs->make<TH2D>("h2_occupancy_emulator_bx_Offphi", "h2_occupancy_emulator_bx_Offphi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_Offphi_oneHit = fs->make<TH2D>("h2_occupancy_emulator_bx_Offphi_oneHit", "h2_occupancy_emulator_bx_Offphi", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_theta = fs->make<TH2D>("h2_occupancy_emulator_bx_theta", "h2_occupancy_emulator_bx_theta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_theta_oneHit = fs->make<TH2D>("h2_occupancy_emulator_bx_theta_oneHit", "h2_occupancy_emulator_bx_theta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_Offtheta = fs->make<TH2D>("h2_occupancy_emulator_bx_Offtheta", "h2_occupancy_emulator_bx_Offtheta", 36, 1, 37, 12, 1, 13);
  h2_occupancy_emulator_bx_Offtheta_oneHit = fs->make<TH2D>("h2_occupancy_emulator_bx_Offtheta_oneHit", "h2_occupancy_emulator_bx_Offtheta", 36, 1, 37, 12, 1, 13);

  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);

