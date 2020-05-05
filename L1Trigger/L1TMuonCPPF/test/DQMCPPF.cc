#include "DQMCPPF.h"
#include <iostream>
#include <vector>

DQM_CPPF::DQM_CPPF(const edm::ParameterSet& iConfig) :
cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),
EMTF_sector(0),
EMTF_subsector(0),
EMTF_subsector_unpacker(0),
EMTF_subsector_EMTF(0),
EMTF_bx(0) {
}

DQM_CPPF::~DQM_CPPF() {
}

void DQM_CPPF::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get the CPPFDigi
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis1;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis2;

  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);

  bool DEBUG = false;

  /// Count total number of events.
  h1_nEvents->Fill(1.0);

  ///////////////////////////////////////////////////////////
  /// Syntax: Comparison CPPF emulator (Ce) vs CPPF unpacked (Cu),
  ///  
  /// Syntax: EMTF unpacked (Eu) and EMTF emulator (Ee)
  ///////////////////////////////////////////////////////////
  std::map<int,int> _nHit_Ce; 
  std::map<int,int> _nHit_Cu;
  std::map<int,std::vector<int>> _phi_Ce;
  std::map<int,std::vector<int>> _phi_Cu;
  std::map<int,std::vector<int>> _phi_glob_Ce;
  std::map<int,std::vector<int>> _phi_glob_Cu;
  std::map<int,std::vector<int>> _theta_Ce;
  std::map<int,std::vector<int>> _theta_Cu;
  std::map<int,std::vector<int>> _theta_glob_Ce;
  std::map<int,std::vector<int>> _theta_glob_Cu;
  std::map<int,std::vector<int>> _roll_Ce;
  std::map<int,std::vector<int>> _roll_Cu;
  std::map<int,std::vector<int>> _zone_Ce;
  std::map<int,std::vector<int>> _zone_Cu;
  std::map<int,std::vector<int>> _ID_Ce;
  std::map<int,std::vector<int>> _ID_Cu;
  std::map<int,std::vector<int>> _emtfSubsector_Ce;
  std::map<int,std::vector<int>> _emtfSubsector_Cu;
  std::map<int,std::vector<int>> _emtfSector_Ce;
  std::map<int,std::vector<int>> _emtfSector_Cu;
  std::map<int,std::vector<int>> _bx_Ce;
  std::map<int,std::vector<int>> _bx_Cu;
  std::map<int,std::vector<int>> _cluster_size_Ce;
  std::map<int,std::vector<int>> _cluster_size_Cu;

  /**
   * ***Step-1***: Loop over cppf_digis for emulator and put information into map.
   */
  for(auto& cppf_digis : *CppfDigis1)
  {
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
    //Chamber ID
    ///
    /// .
    ///   - **TODO**: Check the definition of this variable named **chamberIDCe**.
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);

    if (DEBUG) std::cout << "Info: " << regionCe << "\t" << stationCe << "\t" << ringCe << "\t" << sectorCe << "\t" << subsectorCe << "\t" << emtfSectorCe << "\t" << emtfSubsectorCe << std::endl;

    /// 
    /// .
    ///   - Generate Unique ID baseed on:
    ///   - [regionCe, stationCe, ringCe, sectorCe, subsectorCe, emtfSectorCe, emtfSubsectorCe]
    std::ostringstream oss;
    oss << regionCe << stationCe << ringCe << sectorCe << subsectorCe<< emtfSectorCe << emtfSubsectorCe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;

    /// 1 (a) - Emulator: Check if key exists in the map if not add the key and 
    /// corresponding variables in the map of vectors.    
    if ( _nHit_Ce.find(unique_id) == _nHit_Ce.end() ) { // chamber had no hit so far
      _nHit_Ce.insert({unique_id, 1});
      _phi_Ce[unique_id].push_back(phiIntCe);
      _phi_glob_Ce[unique_id].push_back(phiGlobalCe);
      _theta_Ce[unique_id].push_back(thetaIntCe);
      _theta_glob_Ce[unique_id].push_back(thetaGlobalCe);
      _roll_Ce[unique_id].push_back(rollCe);
      _ID_Ce[unique_id].push_back( chamberIDCe);
      _zone_Ce[unique_id].push_back(fillOccupancyCe);
      _emtfSubsector_Ce[unique_id].push_back(emtfSubsectorCe);
      _emtfSector_Ce[unique_id].push_back(emtfSectorCe);
      _bx_Ce[unique_id].push_back(bxCe);
      _cluster_size_Ce[unique_id].push_back(cluster_sizeCe);
    }
    else {
      _nHit_Ce.at(unique_id) += 1;
      _phi_Ce[unique_id].push_back(phiIntCe);
      _phi_glob_Ce[unique_id].push_back(phiGlobalCe);
      _theta_Ce[unique_id].push_back(thetaIntCe);
      _theta_glob_Ce[unique_id].push_back(thetaGlobalCe);
      _roll_Ce[unique_id].push_back(rollCe);
      _ID_Ce[unique_id].push_back( chamberIDCe);
      _zone_Ce[unique_id].push_back(fillOccupancyCe);
      _emtfSubsector_Ce[unique_id].push_back(emtfSubsectorCe);
      _emtfSector_Ce[unique_id].push_back(emtfSectorCe);
      _bx_Ce[unique_id].push_back(bxCe);
      _cluster_size_Ce[unique_id].push_back(cluster_sizeCe);
    }
  } // END :: for(auto& cppf_digis : *CppfDigis1)

  /**
   * ***Step-2***: Loop over cppf_digis for Unpacker and put information into map.
   */
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

    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);

    if (DEBUG) std::cout << "Info: " << regionCu << "\t" << stationCu << "\t" << ringCu << "\t" << sectorCu << "\t" << subsectorCu << "\t" << emtfSectorCu << "\t" << emtfSubsectorCu << std::endl;
    
    /// Generate Unique ID for unpacker baseed on:
    /// [regionCu, stationCu, ringCu, sectorCu, subsectorCu, emtfSectorCu, emtfSubsectorCu]
    std::ostringstream oss2;
    oss2 << regionCu << stationCu << ringCu << sectorCu << subsectorCu << emtfSectorCu << emtfSubsectorCu;
    std::istringstream iss2(oss2.str());
    int unique_id;
    iss2 >> unique_id;

    /// 2 (a) - Unpacker: Check if key exists in the map if not add the key and 
    /// corresponding variables in the map of vectors.
    if ( _nHit_Cu.find(unique_id) == _nHit_Cu.end() ) { // chamber had no hit so far
      _nHit_Cu.insert({unique_id, 1});
      _phi_Cu[unique_id].push_back(phiIntCu);
      _theta_Cu[unique_id].push_back(thetaIntCu);
      _phi_glob_Cu[unique_id].push_back(phiGlobalCu);
      _theta_glob_Cu[unique_id].push_back(thetaGlobalCu);
      _ID_Cu[unique_id].push_back( chamberIDCu);
      _zone_Cu[unique_id].push_back(fillOccupancyCu);
      _roll_Cu[unique_id].push_back(rollCu);
      _emtfSubsector_Cu[unique_id].push_back(emtfSubsectorCu);
      _emtfSector_Cu[unique_id].push_back(emtfSectorCu);
      _bx_Cu[unique_id].push_back(bxCu);
      _cluster_size_Cu[unique_id].push_back(cluster_sizeCu);
    } else {
      _nHit_Cu.at(unique_id) += 1;
      _phi_Cu[unique_id].push_back(phiIntCu);
      _theta_Cu[unique_id].push_back(thetaIntCu);
      _phi_glob_Cu[unique_id].push_back(phiGlobalCu);
      _theta_glob_Cu[unique_id].push_back(thetaGlobalCu);
      _ID_Cu[unique_id].push_back( chamberIDCu);
      _zone_Cu[unique_id].push_back(fillOccupancyCu);
      _roll_Cu[unique_id].push_back(rollCu);
      _emtfSubsector_Cu[unique_id].push_back(emtfSubsectorCu);
      _emtfSector_Cu[unique_id].push_back(emtfSectorCu);
      _bx_Cu[unique_id].push_back(bxCu);
      _cluster_size_Cu[unique_id].push_back(cluster_sizeCu);
    }
  }  // END: : for(auto& cppf_digis2 : *CppfDigis2)

  /**
   * 
   * Fill two histograms that contains number of chambers having hits
   * distribution.
   * 
   */
  h1_nChmbersWithHit_Ce->Fill(_nHit_Ce.size());
  h1_nChmbersWithHit_Cu->Fill(_nHit_Cu.size());

  if (DEBUG)
    std::cout << "\n\n\n===========================================================\n" << std::endl;
  if (DEBUG)
    std::cout << "\n=== Emulator \t===\n" << std::endl;

  /**
   * ***Step:3*** : Now loop over number of hits in emulator:
   *   - Saves all the variable for the emulator.
   */
  for (auto const& it : _nHit_Ce) 
  {
    int key = it.first;
    int nHit = it.second;

    h1_total_hits_emulator->Fill(nHit);
    /**
     * .
     *   - If the number of Hits for one key (i.e. for one chamber)
     *     is more than 2 then skip that event.
     */
    if (nHit > 2) continue;
    bool ifBxZero = true;  // This boolean helps us to identify
                              // if a chamber having hits have bx==0 for each hits.
    if (DEBUG) std::cout << "Main for loop starts for emulator..." << key << "\t" << nHit << std::endl;
    
    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      if (_bx_Ce.at(key)[vecSize]!=0) ifBxZero = ifBxZero && false;
     
      if (DEBUG) std::cout << _phi_Ce.at(key)[vecSize] <<  " ( " << _theta_Ce.at(key)[vecSize] << ", "
      << _ID_Ce.at(key)[vecSize] << ", " << _zone_Ce.at(key)[vecSize] << ", " << _roll_Ce.at(key)[vecSize] << ", "
      << _emtfSector_Ce.at(key)[vecSize] << ", " << ", " << _emtfSubsector_Ce.at(key)[vecSize] << ", "
      << _bx_Ce.at(key)[vecSize] << ", " << _cluster_size_Ce.at(key)[vecSize] <<")\t";
    }
    if (DEBUG) std::cout << "\t" << std::endl;

    if (!ifBxZero)  continue;

    h1_total_hits_emulator_bx->Fill(nHit);
    
    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      h1_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key)[vecSize]);
      h1_phi_Ce_bx->Fill(_phi_Ce.at(key)[vecSize]);
      h1_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key)[vecSize]);
      h1_theta_Ce_bx->Fill(_theta_Ce.at(key)[vecSize]);
      h1_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key)[vecSize]);
      h1_ID_Ce_bx->Fill(_ID_Ce.at(key)[vecSize]);
      h1_zone_Ce_bx->Fill(_zone_Ce.at(key)[vecSize]);
      h1_roll_Ce_bx->Fill(_roll_Ce.at(key)[vecSize]);
      h1_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key)[vecSize]);
      h1_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key)[vecSize]);
    }
  }
  
  if (DEBUG) std::cout << "\n\n=== Unpacker \t===\n" << std::endl;
  /**
   * ***Step:4***: Now loop over number of hits in unpacker:
   *   - Saves all the variables for the unpacker.
   */
  for (auto const& it : _nHit_Cu) {
    int key = it.first;
    int nHit = it.second;

    h1_total_hits_unpacker->Fill(nHit);
    /**
     * .
     *   - If the number of Hits for one key (i.e. for one chamber)
     *     is more than 2 then skip that event.
     */    
    if (nHit > 2) continue;
    bool ifBxZero = true;

    if (DEBUG) std::cout << "Main for loop starts for unpacker..." << key << "\t" << nHit << std::endl;
    
    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      if (_bx_Cu.at(key)[vecSize]!=0) ifBxZero = ifBxZero && false;

      if (DEBUG) std::cout << _phi_Cu.at(key)[vecSize] <<  " ( " << _theta_Cu.at(key)[vecSize] << ", "
        << _ID_Cu.at(key)[vecSize] << ", " << _zone_Cu.at(key)[vecSize] << ", " << _roll_Cu.at(key)[vecSize] << ", "
        << _emtfSector_Cu.at(key)[vecSize] << ", " << ", " << _emtfSubsector_Cu.at(key)[vecSize] << ", "
        << _bx_Cu.at(key)[vecSize] << ", " << _cluster_size_Cu.at(key)[vecSize] <<")\t";
    }
    if (DEBUG) std::cout << "\t" << std::endl;

    if (!ifBxZero)  continue;
    
    h1_total_hits_unpacker_bx->Fill(nHit);

    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      h1_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key)[vecSize]);
      h1_phi_Cu_bx->Fill(_phi_Cu.at(key)[vecSize]);
      h1_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key)[vecSize]);
      h1_theta_Cu_bx->Fill(_theta_Cu.at(key)[vecSize]);
      h1_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key)[vecSize]);
      h1_ID_Cu_bx->Fill(_ID_Cu.at(key)[vecSize]);
      h1_zone_Cu_bx->Fill(_zone_Cu.at(key)[vecSize]);
      h1_roll_Cu_bx->Fill(_roll_Cu.at(key)[vecSize]);
      h1_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key)[vecSize]);
      h1_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key)[vecSize]);
    }    
  }
  
  if (DEBUG) std::cout << "\n\n=== Emulator And Unpacker Comparison \t===\n" << std::endl;

  /**
   * ***Step:5*** : Now make the comparison between unpacker and emulator
   * then saves the variables.
   *   - for this loop over 
   */
  for (auto const& Ce : _nHit_Ce)
  {
    int key_Ce = Ce.first;
    int nHit_Ce = Ce.second;

    bool ifBxZero_Ce = true;
    for (int vecSize_Ce = 0; vecSize_Ce < nHit_Ce; ++vecSize_Ce)
    {
      if (_bx_Ce.at(key_Ce)[vecSize_Ce]!=0) ifBxZero_Ce = ifBxZero_Ce && false;
    }

    for (auto const& Cu : _nHit_Cu)
    {
      int key_Cu = Cu.first;
      int nHit_Cu = Cu.second;

      bool ifBxZero_Cu = true;
      for (int vecSize_Cu = 0; vecSize_Cu < nHit_Cu; ++vecSize_Cu)
      {
        if (_bx_Cu.at(key_Cu)[vecSize_Cu]!=0) ifBxZero_Cu = ifBxZero_Cu && false;
      }

      if (key_Ce != key_Cu) continue;
      if (!ifBxZero_Ce)  continue;
      if (!ifBxZero_Cu)  continue;
      for (int vecSize = 0; vecSize < nHit_Cu; ++vecSize)
      {
        h1_SameKey_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
      }
      for (int vecSize = 0; vecSize < nHit_Ce; ++vecSize)
      {
        h1_SameKey_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
      } 

      // Fill the histogram if phi-matches   
      for (int vecSize_phiMatchCu = 0; vecSize_phiMatchCu < nHit_Cu; ++vecSize_phiMatchCu)
      for (int vecSize_phiMatchCe = 0; vecSize_phiMatchCe < nHit_Ce; ++vecSize_phiMatchCe)
      {
        //Fill histogram for on/Off phi
        if ( _phi_Ce.at(key_Ce)[vecSize_phiMatchCe] == _phi_Cu.at(key_Cu)[vecSize_phiMatchCu])
        {
          h1_SameKey_OnPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          
          h1_SameKey_OnPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize_phiMatchCe]);        
        }
        else
        {
          h1_SameKey_OffPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
        }

        // Fill histogram for on/off Theta
        if ( _theta_Ce.at(key_Ce)[vecSize_phiMatchCe] == _theta_Cu.at(key_Cu)[vecSize_phiMatchCu])
        {
          h1_SameKey_OnTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OnTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OnTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize_phiMatchCe]);        
        }
        else
        {
          h1_SameKey_OffTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize_phiMatchCu]);
          h1_SameKey_OffTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
          h1_SameKey_OffTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize_phiMatchCe]);
        }
        

      }
    }
  }
} //End class


void DQM_CPPF::beginJob()
{
  edm::Service<TFileService> fs;

  h1_nEvents = fs->make<TH1D>("h1_nEvents", "Total number of events", 3, 0., 3.);

  h1_total_hits_unpacker = fs->make<TH1D>("h1_total_hits_unpacker", "Total number of hits in unpacker" , 5, 0. , 5.); 
  h1_total_hits_unpacker_bx = fs->make<TH1D>("h1_total_hits_unpacker_bx", "Total number of hits in unpacker (bxE == bxU)" , 5, 0. , 5.);
  h1_total_hits_unpacker_bx_phi = fs->make<TH1D>("h1_total_hits_unpacker_bx_phi", "CPPFDigis_Matches_int" , 25, 0. , 25.);
  h1_nChmbersWithHit_Cu = fs->make<TH1D>("h1_nChmbersWithHit_Cu","h1_nChmbersWithHit_Cu", 40, 0., 40. );
  h1_cluster_size_Cu_bx = fs->make<TH1D>("h1_cluster_size_Cu_bx","h1_cluster_size_Cu_bx", 10, 0., 10.);
  h1_phi_Cu_bx = fs->make<TH1D>("h1_phi_Cu_bx","h1_phi_Cu_bx", 124, 0.0, 1240.);
  h1_phi_glob_Cu_bx = fs->make<TH1D>("h1_phi_glob_Cu_bx","h1_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_theta_Cu_bx = fs->make<TH1D>("h1_theta_Cu_bx","h1_theta_Cu_bx", 32, 0, 32.);
  h1_theta_glob_Cu_bx = fs->make<TH1D>("h1_theta_glob_Cu_bx","h1_theta_glob_Cu_bx", 180., 0, 180.);
  h1_ID_Cu_bx = fs->make<TH1D>("h1_ID_Cu_bx","h1_ID_Cu_bx", 38, 0, 38);
  h1_zone_Cu_bx = fs->make<TH1D>("h1_zone_Cu_bx","h1_zone_Cu_bx", 15, 0, 15);
  h1_roll_Cu_bx = fs->make<TH1D>("h1_roll_Cu_bx","h1_roll_Cu_bx", 4, 0, 4);
  h1_emtfSector_Cu_bx = fs->make<TH1D>("h1_emtfSector_Cu_bx","h1_emtfSector_Cu_bx", 8, 0, 8);
  h1_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_emtfSubsector_Cu_bx","h1_emtfSubsector_Cu_bx", 38, 0, 38);

  h1_total_hits_emulator = fs->make<TH1D>("h1_total_hits_emulator", "CPPFDigis_total_hits_emulator" , 5, 0. , 5.);
  h1_total_hits_emulator_bx = fs->make<TH1D>("h1_total_hits_emulator_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h1_total_hits_emulator_bx_phi = fs->make<TH1D>("h1_total_hits_emulator_bx_phi", "CPPFDigis_Matches_int" , 25, 0. , 25.);
  h1_nChmbersWithHit_Ce = fs->make<TH1D>("h1_nChmbersWithHit_Ce","h1_nChmbersWithHit_Ce", 40, 0., 40. );
  h1_cluster_size_Ce_bx = fs->make<TH1D>("h1_cluster_size_Ce_bx","h1_cluster_size_Ce_bx", 10, 0., 10.);
  h1_phi_Ce_bx = fs->make<TH1D>("h1_phi_Ce_bx","h1_phi_Ce_bx", 124, 0.0, 1240.);
  h1_phi_glob_Ce_bx = fs->make<TH1D>("h1_phi_glob_Ce_bx","h1_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_theta_Ce_bx = fs->make<TH1D>("h1_theta_Ce_bx","h1_theta_Ce_bx", 32, 0, 32.);
  h1_theta_glob_Ce_bx = fs->make<TH1D>("h1_theta_glob_Ce_bx","h1_theta_glob_Ce_bx", 180., 0, 180.);
  h1_ID_Ce_bx = fs->make<TH1D>("h1_ID_Ce_bx","h1_ID_Ce_bx", 38, 0, 38);
  h1_zone_Ce_bx = fs->make<TH1D>("h1_zone_Ce_bx","h1_zone_Ce_bx", 15, 0, 15);
  h1_roll_Ce_bx = fs->make<TH1D>("h1_roll_Ce_bx","h1_roll_Ce_bx", 4, 0, 4);
  h1_emtfSector_Ce_bx = fs->make<TH1D>("h1_emtfSector_Ce_bx","h1_emtfSector_Ce_bx", 8, 0, 8);
  h1_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_emtfSubsector_Ce_bx","h1_emtfSubsector_Ce_bx", 38, 0, 38);

  h1_SameKey_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_cluster_size_Cu_bx","h1_SameKey_cluster_size_Cu_bx", 10, 0., 10.);
  h1_SameKey_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_phi_Cu_bx","h1_SameKey_phi_Cu_bx", 124, 0.0, 1240.);
  h1_SameKey_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_phi_glob_Cu_bx","h1_SameKey_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_SameKey_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_theta_Cu_bx","h1_SameKey_theta_Cu_bx", 32, 0, 32.);
  h1_SameKey_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_theta_glob_Cu_bx","h1_SameKey_theta_glob_Cu_bx", 180., 0, 180.);
  h1_SameKey_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_ID_Cu_bx","h1_SameKey_ID_Cu_bx", 38, 0, 38);
  h1_SameKey_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_zone_Cu_bx","h1_SameKey_zone_Cu_bx", 15, 0, 15);
  h1_SameKey_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_roll_Cu_bx","h1_SameKey_roll_Cu_bx", 4, 0, 4);
  h1_SameKey_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_emtfSector_Cu_bx","h1_SameKey_emtfSector_Cu_bx", 8, 0, 8);
  h1_SameKey_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_emtfSubsector_Cu_bx","h1_SameKey_emtfSubsector_Cu_bx", 38, 0, 38);
  h1_SameKey_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_cluster_size_Ce_bx","h1_SameKey_cluster_size_Ce_bx", 10, 0., 10.);
  h1_SameKey_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_phi_Ce_bx","h1_SameKey_phi_Ce_bx", 124, 0.0, 1240.);
  h1_SameKey_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_phi_glob_Ce_bx","h1_SameKey_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_SameKey_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_theta_Ce_bx","h1_SameKey_theta_Ce_bx", 32, 0, 32.);
  h1_SameKey_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_theta_glob_Ce_bx","h1_SameKey_theta_glob_Ce_bx", 180., 0, 180.);
  h1_SameKey_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_ID_Ce_bx","h1_SameKey_ID_Ce_bx", 38, 0, 38);
  h1_SameKey_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_zone_Ce_bx","h1_SameKey_zone_Ce_bx", 15, 0, 15);
  h1_SameKey_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_roll_Ce_bx","h1_SameKey_roll_Ce_bx", 4, 0, 4);
  h1_SameKey_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_emtfSector_Ce_bx","h1_SameKey_emtfSector_Ce_bx", 8, 0, 8);
  h1_SameKey_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_emtfSubsector_Ce_bx","h1_SameKey_emtfSubsector_Ce_bx", 38, 0, 38);

  h1_SameKey_OnPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_cluster_size_Cu_bx","h1_SameKey_OnPhi_cluster_size_Cu_bx", 10, 0., 10.);
  h1_SameKey_OnPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_Cu_bx","h1_SameKey_OnPhi_phi_Cu_bx", 124, 0.0, 1240.);
  h1_SameKey_OnPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_glob_Cu_bx","h1_SameKey_OnPhi_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_SameKey_OnPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_Cu_bx","h1_SameKey_OnPhi_theta_Cu_bx", 32, 0, 32.);
  h1_SameKey_OnPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_glob_Cu_bx","h1_SameKey_OnPhi_theta_glob_Cu_bx", 180., 0, 180.);
  h1_SameKey_OnPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_ID_Cu_bx","h1_SameKey_OnPhi_ID_Cu_bx", 38, 0, 38);
  h1_SameKey_OnPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_zone_Cu_bx","h1_SameKey_OnPhi_zone_Cu_bx", 15, 0, 15);
  h1_SameKey_OnPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_roll_Cu_bx","h1_SameKey_OnPhi_roll_Cu_bx", 4, 0, 4);
  h1_SameKey_OnPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSector_Cu_bx","h1_SameKey_OnPhi_emtfSector_Cu_bx", 8, 0, 8);
  h1_SameKey_OnPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSubsector_Cu_bx","h1_SameKey_OnPhi_emtfSubsector_Cu_bx", 38, 0, 38);
  h1_SameKey_OnPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_cluster_size_Ce_bx","h1_SameKey_OnPhi_cluster_size_Ce_bx", 10, 0., 10.);
  h1_SameKey_OnPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_Ce_bx","h1_SameKey_OnPhi_phi_Ce_bx", 124, 0.0, 1240.);
  h1_SameKey_OnPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_glob_Ce_bx","h1_SameKey_OnPhi_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_SameKey_OnPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_Ce_bx","h1_SameKey_OnPhi_theta_Ce_bx", 32, 0, 32.);
  h1_SameKey_OnPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_glob_Ce_bx","h1_SameKey_OnPhi_theta_glob_Ce_bx", 180., 0, 180.);
  h1_SameKey_OnPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_ID_Ce_bx","h1_SameKey_OnPhi_ID_Ce_bx", 38, 0, 38);
  h1_SameKey_OnPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_zone_Ce_bx","h1_SameKey_OnPhi_zone_Ce_bx", 15, 0, 15);
  h1_SameKey_OnPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_roll_Ce_bx","h1_SameKey_OnPhi_roll_Ce_bx", 4, 0, 4);
  h1_SameKey_OnPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSector_Ce_bx","h1_SameKey_OnPhi_emtfSector_Ce_bx", 8, 0, 8);
  h1_SameKey_OnPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSubsector_Ce_bx","h1_SameKey_OnPhi_emtfSubsector_Ce_bx", 38, 0, 38);

  h1_SameKey_OffPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_cluster_size_Cu_bx","h1_SameKey_OffPhi_cluster_size_Cu_bx", 10, 0., 10.);
  h1_SameKey_OffPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_Cu_bx","h1_SameKey_OffPhi_phi_Cu_bx", 124, 0.0, 1240.);
  h1_SameKey_OffPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_glob_Cu_bx","h1_SameKey_OffPhi_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_SameKey_OffPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_Cu_bx","h1_SameKey_OffPhi_theta_Cu_bx", 32, 0, 32.);
  h1_SameKey_OffPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_glob_Cu_bx","h1_SameKey_OffPhi_theta_glob_Cu_bx", 180., 0, 180.);
  h1_SameKey_OffPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_ID_Cu_bx","h1_SameKey_OffPhi_ID_Cu_bx", 38, 0, 38);
  h1_SameKey_OffPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_zone_Cu_bx","h1_SameKey_OffPhi_zone_Cu_bx", 15, 0, 15);
  h1_SameKey_OffPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_roll_Cu_bx","h1_SameKey_OffPhi_roll_Cu_bx", 4, 0, 4);
  h1_SameKey_OffPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSector_Cu_bx","h1_SameKey_OffPhi_emtfSector_Cu_bx", 8, 0, 8);
  h1_SameKey_OffPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSubsector_Cu_bx","h1_SameKey_OffPhi_emtfSubsector_Cu_bx", 38, 0, 38);
  h1_SameKey_OffPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_cluster_size_Ce_bx","h1_SameKey_OffPhi_cluster_size_Ce_bx", 10, 0., 10.);
  h1_SameKey_OffPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_Ce_bx","h1_SameKey_OffPhi_phi_Ce_bx", 124, 0.0, 1240.);
  h1_SameKey_OffPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_glob_Ce_bx","h1_SameKey_OffPhi_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_SameKey_OffPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_Ce_bx","h1_SameKey_OffPhi_theta_Ce_bx", 32, 0, 32.);
  h1_SameKey_OffPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_glob_Ce_bx","h1_SameKey_OffPhi_theta_glob_Ce_bx", 180., 0, 180.);
  h1_SameKey_OffPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_ID_Ce_bx","h1_SameKey_OffPhi_ID_Ce_bx", 38, 0, 38);
  h1_SameKey_OffPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_zone_Ce_bx","h1_SameKey_OffPhi_zone_Ce_bx", 15, 0, 15);
  h1_SameKey_OffPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_roll_Ce_bx","h1_SameKey_OffPhi_roll_Ce_bx", 4, 0, 4);
  h1_SameKey_OffPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSector_Ce_bx","h1_SameKey_OffPhi_emtfSector_Ce_bx", 8, 0, 8);
  h1_SameKey_OffPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSubsector_Ce_bx","h1_SameKey_OffPhi_emtfSubsector_Ce_bx", 38, 0, 38);

  h1_SameKey_OnTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_cluster_size_Cu_bx","h1_SameKey_OnTheta_cluster_size_Cu_bx", 10, 0., 10.);
  h1_SameKey_OnTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_Cu_bx","h1_SameKey_OnTheta_phi_Cu_bx", 124, 0.0, 1240.);
  h1_SameKey_OnTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_glob_Cu_bx","h1_SameKey_OnTheta_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_SameKey_OnTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_Cu_bx","h1_SameKey_OnTheta_theta_Cu_bx", 32, 0, 32.);
  h1_SameKey_OnTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_glob_Cu_bx","h1_SameKey_OnTheta_theta_glob_Cu_bx", 180., 0, 180.);
  h1_SameKey_OnTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_ID_Cu_bx","h1_SameKey_OnTheta_ID_Cu_bx", 38, 0, 38);
  h1_SameKey_OnTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_zone_Cu_bx","h1_SameKey_OnTheta_zone_Cu_bx", 15, 0, 15);
  h1_SameKey_OnTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_roll_Cu_bx","h1_SameKey_OnTheta_roll_Cu_bx", 4, 0, 4);
  h1_SameKey_OnTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSector_Cu_bx","h1_SameKey_OnTheta_emtfSector_Cu_bx", 8, 0, 8);
  h1_SameKey_OnTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSubsector_Cu_bx","h1_SameKey_OnTheta_emtfSubsector_Cu_bx", 38, 0, 38);
  h1_SameKey_OnTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_cluster_size_Ce_bx","h1_SameKey_OnTheta_cluster_size_Ce_bx", 10, 0., 10.);
  h1_SameKey_OnTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_Ce_bx","h1_SameKey_OnTheta_phi_Ce_bx", 124, 0.0, 1240.);
  h1_SameKey_OnTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_glob_Ce_bx","h1_SameKey_OnTheta_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_SameKey_OnTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_Ce_bx","h1_SameKey_OnTheta_theta_Ce_bx", 32, 0, 32.);
  h1_SameKey_OnTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_glob_Ce_bx","h1_SameKey_OnTheta_theta_glob_Ce_bx", 180., 0, 180.);
  h1_SameKey_OnTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_ID_Ce_bx","h1_SameKey_OnTheta_ID_Ce_bx", 38, 0, 38);
  h1_SameKey_OnTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_zone_Ce_bx","h1_SameKey_OnTheta_zone_Ce_bx", 15, 0, 15);
  h1_SameKey_OnTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_roll_Ce_bx","h1_SameKey_OnTheta_roll_Ce_bx", 4, 0, 4);
  h1_SameKey_OnTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSector_Ce_bx","h1_SameKey_OnTheta_emtfSector_Ce_bx", 8, 0, 8);
  h1_SameKey_OnTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSubsector_Ce_bx","h1_SameKey_OnTheta_emtfSubsector_Ce_bx", 38, 0, 38);

  h1_SameKey_OffTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_cluster_size_Cu_bx","h1_SameKey_OffTheta_cluster_size_Cu_bx", 10, 0., 10.);
  h1_SameKey_OffTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_Cu_bx","h1_SameKey_OffTheta_phi_Cu_bx", 124, 0.0, 1240.);
  h1_SameKey_OffTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_glob_Cu_bx","h1_SameKey_OffTheta_phi_glob_Cu_bx", 180, 0.0, 180.0);
  h1_SameKey_OffTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_Cu_bx","h1_SameKey_OffTheta_theta_Cu_bx", 32, 0, 32.);
  h1_SameKey_OffTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_glob_Cu_bx","h1_SameKey_OffTheta_theta_glob_Cu_bx", 180., 0, 180.);
  h1_SameKey_OffTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_ID_Cu_bx","h1_SameKey_OffTheta_ID_Cu_bx", 38, 0, 38);
  h1_SameKey_OffTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_zone_Cu_bx","h1_SameKey_OffTheta_zone_Cu_bx", 15, 0, 15);
  h1_SameKey_OffTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_roll_Cu_bx","h1_SameKey_OffTheta_roll_Cu_bx", 4, 0, 4);
  h1_SameKey_OffTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSector_Cu_bx","h1_SameKey_OffTheta_emtfSector_Cu_bx", 8, 0, 8);
  h1_SameKey_OffTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSubsector_Cu_bx","h1_SameKey_OffTheta_emtfSubsector_Cu_bx", 38, 0, 38);
  h1_SameKey_OffTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_cluster_size_Ce_bx","h1_SameKey_OffTheta_cluster_size_Ce_bx", 10, 0., 10.);
  h1_SameKey_OffTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_Ce_bx","h1_SameKey_OffTheta_phi_Ce_bx", 124, 0.0, 1240.);
  h1_SameKey_OffTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_glob_Ce_bx","h1_SameKey_OffTheta_phi_glob_Ce_bx", 180, 0.0, 180.0);
  h1_SameKey_OffTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_Ce_bx","h1_SameKey_OffTheta_theta_Ce_bx", 32, 0, 32.);
  h1_SameKey_OffTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_glob_Ce_bx","h1_SameKey_OffTheta_theta_glob_Ce_bx", 180., 0, 180.);
  h1_SameKey_OffTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_ID_Ce_bx","h1_SameKey_OffTheta_ID_Ce_bx", 38, 0, 38);
  h1_SameKey_OffTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_zone_Ce_bx","h1_SameKey_OffTheta_zone_Ce_bx", 15, 0, 15);
  h1_SameKey_OffTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_roll_Ce_bx","h1_SameKey_OffTheta_roll_Ce_bx", 4, 0, 4);
  h1_SameKey_OffTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSector_Ce_bx","h1_SameKey_OffTheta_emtfSector_Ce_bx", 8, 0, 8);
  h1_SameKey_OffTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSubsector_Ce_bx","h1_SameKey_OffTheta_emtfSubsector_Ce_bx", 38, 0, 38);


  return;
}

/**
 * Based on the value of region, station and ring information
 * it assigns a number from 1-12  to each ring. So, that one
 * could identify each ring.
 * @param  region_  +ve or -ve endcap.
 * @param  station_ In each region there are four stations.
 * @param  ring_    In each station there are three rings.
 *                  Currently, ring 1 is not instrumented.
 *                  Also, for station 1 and 2 only only ring two
 *                  is available and for 3 and 4 ring two and
 *                  three both are available.
 * @return          It returns an unique number between 1-12 for
 *                  each ring.
 */
int DQM_CPPF::occupancy_value(int region_, int station_, int ring_) {
  
  int fill_val = 0;
  if(region_ == -1) {
    
    if((station_ == 4) && (ring_ == 3)) fill_val = 1;
    else if ((station_ == 4) && (ring_ == 2)) fill_val = 2;
    else if ((station_ == 3) && (ring_ == 3)) fill_val = 3;
    else if ((station_ == 3) && (ring_ == 2)) fill_val = 4;
    else if ((station_ == 2) && (ring_ == 2)) fill_val = 5;
    else if ((station_ == 1) && (ring_ == 2)) fill_val = 6;
    
  }
  else if(region_ == +1) {
    
    if((station_ == 1) && (ring_ == 2)) fill_val = 7;
    else if((station_ == 2) && (ring_ == 2)) fill_val = 8;
    else if((station_ == 3) && (ring_ == 2)) fill_val = 9;
    else if((station_ == 3) && (ring_ == 3)) fill_val = 10;
    else if((station_ == 4) && (ring_ == 2)) fill_val = 11;
    else if((station_ == 4) && (ring_ == 3)) fill_val = 12;
    
  }
  return fill_val;
}

/**
 * Get EMTF sub-sector unique number based on the local EMTF sector
 * and CPPF local sub-sector numbers.
 * @param  emtfsector_ EMTF Sector number from 1 to 6.
 * @param  lsubsector_ CPPF sub-sector number from 1 to 6.
 * @return             returns a unique sub-sector number between [1,36] that
 *                     corresponds to the EMTF sub-sector.
 */
int DQM_CPPF::GetSubsector(int emtfsector_, int lsubsector_) {
  const int nsectors = 6;
  int gsubsector = 0;
  if ((emtfsector_ != -99) and (lsubsector_ != 0)) {
    gsubsector = (emtfsector_ - 1)*nsectors + lsubsector_;
  }
  return gsubsector;
}

int DQM_CPPF::bx_value(int region_, int emtfsector_) {
  
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


//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);
