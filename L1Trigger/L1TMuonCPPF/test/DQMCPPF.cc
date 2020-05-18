/********************************************************
 *
 *  Major change with this version:
 *    - Check the bunch crossing for each hit on chamber
 *      instead of each hit independently.
 *
 ********************************************************/
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
   * # Step-1 : Get map for Emulator
   * - Loop over cppf_digis for emulator and put information into map.
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
    /// \todo Check the definition of this variable named **chamberIDCe**.
    /// ```cpp
    /// int nsubCe = 6;
    /// (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    /// int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);
    /// ```
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);

    if (DEBUG) std::cout << "Info: " << regionCe << "\t" << stationCe << "\t" << ringCe << "\t" << sectorCe << "\t" << subsectorCe << "\t" << emtfSectorCe << "\t" << emtfSubsectorCe << std::endl;

    ///
    /// \li Generate Unique ID baseed on:
    ///   \li [regionCe, stationCe, ringCe, sectorCe, subsectorCe, emtfSectorCe, emtfSubsectorCe]
    std::ostringstream oss;
    oss << regionCe << stationCe << ringCe << sectorCe << subsectorCe<< emtfSectorCe << emtfSubsectorCe;
    std::istringstream iss(oss.str());
    int unique_id;
    iss >> unique_id;

    ///
    /// \li Check if key exists in the map if not add the key and
    ///     corresponding variables in the map of vectors.
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
   * # Step-2 : Get unpacker map
   * - Loop over cppf_digis for Unpacker and put information into map.
   */
  for(auto& cppf_digis2 : *CppfDigis2){
    RPCDetId rpcIdCu = cppf_digis2.rpcId();
    int regionCu = (int)rpcIdCu.region();
    int stationCu = (int)rpcIdCu.station();
    int sectorCu = (int)rpcIdCu.sector();
    int subsectorCu = (int)rpcIdCu.subsector();
    int ringCu = (int)rpcIdCu.ring();
    int rollCu = (int)(rpcIdCu.roll());
    int phiIntCu = (int)cppf_digis2.phi_int();
    int thetaIntCu =  (int)cppf_digis2.theta_int();
    int phiGlobalCu = (int)cppf_digis2.phi_glob();
    int thetaGlobalCu =  (int)cppf_digis2.theta_glob();
    int cluster_sizeCu = (int)cppf_digis2.cluster_size();
    int bxCu = (int)cppf_digis2.bx();
    int emtfSectorCu = (int)cppf_digis2.emtf_sector();
    int emtfSubsectorCu = GetSubsector(emtfSectorCu, subsectorCu);
    int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
    //int fillBxCu = bx_value(regionCu, emtfSectorCu);

    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);

    if (DEBUG) std::cout << "Info: " << regionCu << "\t" << stationCu << "\t" << ringCu << "\t" << sectorCu << "\t" << subsectorCu << "\t" << emtfSectorCu << "\t" << emtfSubsectorCu << std::endl;

    ///
    /// \li Generate Unique ID for unpacker baseed on:
    ///     - [regionCu, stationCu, ringCu, sectorCu, subsectorCu, emtfSectorCu, emtfSubsectorCu]
    std::ostringstream oss2;
    oss2 << regionCu << stationCu << ringCu << sectorCu << subsectorCu << emtfSectorCu << emtfSubsectorCu;
    std::istringstream iss2(oss2.str());
    int unique_id;
    iss2 >> unique_id;

    ///
    /// \li Check if key exists in the map if not add the key and
    ///     corresponding variables in the map of vectors.
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
   * ## general
   *    - Fill two histograms, one for unpacker and another for
   *    emulator, that contains number of chambers in each event having hits.
   *    - The corresponding histograms are #h1_nChmbersWithHit_Ce and #h1_nChmbersWithHit_Cu.
   */
  h1_nChmbersWithHit_Ce->Fill(_nHit_Ce.size());
  h1_nChmbersWithHit_Cu->Fill(_nHit_Cu.size());

  if (DEBUG)
    std::cout << "\n\n\n===========================================================\n" << std::endl;
  if (DEBUG)
    std::cout << "\n=== Emulator \t===\n" << std::endl;

  /**
   * \li Check how many events have no hits in unpacker and in emulator.
   * \li The corresponding histogram names are #h1_nEvents_Ce_NoHits
   *     and #h1_nEvents_Cu_NoHits
   */
  if (_nHit_Ce.size() == 0) h1_nEvents_Ce_NoHits->Fill(1.0);
  if (_nHit_Cu.size() == 0) h1_nEvents_Cu_NoHits->Fill(1.0);

  /**
   * # Step-4 : Loop over emulator map
   * - Now loop over number of hits in emulator and saves some
   *   useful variable for the emulator.
   */
  bool nHit_Ce_Count_nHitNotMoreThan2 = false;
  bool nHit_Ce_Count_bx = false;
  for (auto const& it : _nHit_Ce)
  {
    int key = it.first;
    int nHit = it.second;
    h1_total_hits_emulator->Fill(nHit);
    /**
     * \li If the number of Hits for one key (i.e. for one chamber)
     *     is more than 2 then skip that event.
     */
    if (nHit > 2) continue;
    h1_total_hits_emulator_LessThan3Hit->Fill(nHit);
    nHit_Ce_Count_nHitNotMoreThan2 = true;
    if (DEBUG) std::cout << "Main for loop starts for emulator..." << key << "\t" << nHit << std::endl;
    /**
     * \li \bug nHit distribution is reduces nEvents by ~32%. Since h1_total_hits_emulator
     *      gives 2.
     */
    int countNHitsHavingBxZero = 0;
    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      h1_cluster_size_Ce->Fill(_cluster_size_Ce.at(key)[vecSize]);
      h1_phi_Ce->Fill(_phi_Ce.at(key)[vecSize]);
      h1_phi_glob_Ce->Fill(_phi_glob_Ce.at(key)[vecSize]);
      h1_theta_Ce->Fill(_theta_Ce.at(key)[vecSize]);
      h1_theta_glob_Ce->Fill(_theta_glob_Ce.at(key)[vecSize]);
      h1_ID_Ce->Fill(_ID_Ce.at(key)[vecSize]);
      h1_zone_Ce->Fill(_zone_Ce.at(key)[vecSize]);
      h1_roll_Ce->Fill(_roll_Ce.at(key)[vecSize]);
      h1_emtfSector_Ce->Fill(_emtfSector_Ce.at(key)[vecSize]);
      h1_emtfSubsector_Ce->Fill(_emtfSubsector_Ce.at(key)[vecSize]);

      /**
      * \li Fill histograms if the bx==0.
      */
      if (_bx_Ce.at(key)[vecSize] != 0) continue;
      nHit_Ce_Count_bx = true;
      countNHitsHavingBxZero++;

      if (DEBUG) std::cout << _phi_Ce.at(key)[vecSize] <<  " ( " << _theta_Ce.at(key)[vecSize] << ", "
        << _ID_Ce.at(key)[vecSize] << ", " << _zone_Ce.at(key)[vecSize] << ", " << _roll_Ce.at(key)[vecSize] << ", "
        << _emtfSector_Ce.at(key)[vecSize] << ", " << ", " << _emtfSubsector_Ce.at(key)[vecSize] << ", "
        << _bx_Ce.at(key)[vecSize] << ", " << _cluster_size_Ce.at(key)[vecSize] <<")\t";

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

      if (nHit==1)
      {
        h1_cluster_size_Ce_bx_OneHit->Fill(_cluster_size_Ce.at(key)[vecSize]);
        h1_phi_Ce_bx_OneHit->Fill(_phi_Ce.at(key)[vecSize]);
        h1_phi_glob_Ce_bx_OneHit->Fill(_phi_glob_Ce.at(key)[vecSize]);
        h1_theta_Ce_bx_OneHit->Fill(_theta_Ce.at(key)[vecSize]);
        h1_theta_glob_Ce_bx_OneHit->Fill(_theta_glob_Ce.at(key)[vecSize]);
        h1_ID_Ce_bx_OneHit->Fill(_ID_Ce.at(key)[vecSize]);
        h1_zone_Ce_bx_OneHit->Fill(_zone_Ce.at(key)[vecSize]);
        h1_roll_Ce_bx_OneHit->Fill(_roll_Ce.at(key)[vecSize]);
        h1_emtfSector_Ce_bx_OneHit->Fill(_emtfSector_Ce.at(key)[vecSize]);
        h1_emtfSubsector_Ce_bx_OneHit->Fill(_emtfSubsector_Ce.at(key)[vecSize]);
      }
    }
    if (countNHitsHavingBxZero) h1_total_hits_emulator_LessThan3Hit_BxZero->Fill(countNHitsHavingBxZero);
    if (DEBUG) std::cout << "\t" << std::endl;
  }
  if (nHit_Ce_Count_nHitNotMoreThan2) h1_nEvents_Ce_nHitNotMoreThan2->Fill(1.0);
  if (nHit_Ce_Count_bx) h1_nEvents_Ce_bx->Fill(1.0);


  if (DEBUG) std::cout << "\n\n=== Unpacker \t===\n" << std::endl;
  /**
   * # Step-5: Loop over unpacker map
   * - Now loop over number of hits in unpacker:
   *   - Saves all the variables for the unpacker.
   */
  bool nHit_Cu_Count_nHitNotMoreThan2 = false;
  bool nHit_Cu_Count_bx = false;
  for (auto const& it : _nHit_Cu)
  {
    int key = it.first;
    int nHit = it.second;
    h1_total_hits_unpacker->Fill(nHit);
    /**
     *  \li If the number of Hits for one key (i.e. for one chamber)
     *     is more than 2 then skip that event.
     */
    if (nHit > 2) continue;
    h1_total_hits_unpacker_LessThan3Hit->Fill(nHit);
    nHit_Cu_Count_nHitNotMoreThan2=true;
                          // if a chamber having hits have bx==0 for each hits.
    if (DEBUG) std::cout << "Main for loop starts for unpacker..." << key << "\t" << nHit << std::endl;

    int countNHitsHavingBxZero = 0;
    for (int vecSize = 0; vecSize < nHit; ++vecSize)
    {
      h1_cluster_size_Cu->Fill(_cluster_size_Cu.at(key)[vecSize]);
      h1_phi_Cu->Fill(_phi_Cu.at(key)[vecSize]);
      h1_phi_glob_Cu->Fill(_phi_glob_Cu.at(key)[vecSize]);
      h1_theta_Cu->Fill(_theta_Cu.at(key)[vecSize]);
      h1_theta_glob_Cu->Fill(_theta_glob_Cu.at(key)[vecSize]);
      h1_ID_Cu->Fill(_ID_Cu.at(key)[vecSize]);
      h1_zone_Cu->Fill(_zone_Cu.at(key)[vecSize]);
      h1_roll_Cu->Fill(_roll_Cu.at(key)[vecSize]);
      h1_emtfSector_Cu->Fill(_emtfSector_Cu.at(key)[vecSize]);
      h1_emtfSubsector_Cu->Fill(_emtfSubsector_Cu.at(key)[vecSize]);

      if (_bx_Cu.at(key)[vecSize]!=0) continue;
      countNHitsHavingBxZero++;
      nHit_Cu_Count_bx = true;

      if (DEBUG) std::cout << _phi_Cu.at(key)[vecSize] <<  " ( " << _theta_Cu.at(key)[vecSize] << ", "
        << _ID_Cu.at(key)[vecSize] << ", " << _zone_Cu.at(key)[vecSize] << ", " << _roll_Cu.at(key)[vecSize] << ", "
        << _emtfSector_Cu.at(key)[vecSize] << ", " << ", " << _emtfSubsector_Cu.at(key)[vecSize] << ", "
        << _bx_Cu.at(key)[vecSize] << ", " << _cluster_size_Cu.at(key)[vecSize] <<")\t";
      /**
       * \li Fill histograms if the bx==0.
       */
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

      if (nHit == 1) {
        h1_cluster_size_Cu_bx_OneHit->Fill(_cluster_size_Cu.at(key)[vecSize]);
        h1_phi_Cu_bx_OneHit->Fill(_phi_Cu.at(key)[vecSize]);
        h1_phi_glob_Cu_bx_OneHit->Fill(_phi_glob_Cu.at(key)[vecSize]);
        h1_theta_Cu_bx_OneHit->Fill(_theta_Cu.at(key)[vecSize]);
        h1_theta_glob_Cu_bx_OneHit->Fill(_theta_glob_Cu.at(key)[vecSize]);
        h1_ID_Cu_bx_OneHit->Fill(_ID_Cu.at(key)[vecSize]);
        h1_zone_Cu_bx_OneHit->Fill(_zone_Cu.at(key)[vecSize]);
        h1_roll_Cu_bx_OneHit->Fill(_roll_Cu.at(key)[vecSize]);
        h1_emtfSector_Cu_bx_OneHit->Fill(_emtfSector_Cu.at(key)[vecSize]);
        h1_emtfSubsector_Cu_bx_OneHit->Fill(_emtfSubsector_Cu.at(key)[vecSize]);
      }
    }
    if (countNHitsHavingBxZero) h1_total_hits_unpacker_LessThan3Hit_BxZero->Fill(countNHitsHavingBxZero);
    if (DEBUG) std::cout << "\t" << std::endl;
  }
  if (nHit_Cu_Count_nHitNotMoreThan2) h1_nEvents_Cu_nHitNotMoreThan2->Fill(1.0);
  if (nHit_Cu_Count_bx) h1_nEvents_Cu_bx->Fill(1.0);
  if (DEBUG) std::cout << "\n\n=== Emulator And Unpacker Comparison \t===\n" << std::endl;
  /**
   * # Step: 6 : Unpacker-Emulator Comparison
   * - Now make the comparison between unpacker and emulator
   * then saves the variables.
   *   - Loop over emulator chambers
   *     - Loop over unpacker chambers
   */
  bool nHit_CuCe_SameKey = false;
  bool nHit_CuCe_SameKey_SameNHit = false;
  bool nHit_CuCe_SameKey_SameNHit_AnyBx0 = false;
  bool nHit_CuCe_SameKey_SameNHit_AnyBx0_onPhi = false;
  bool nHit_CuCe_SameKey_SameNHit_AnyBx0_offPhi = false;
  bool nHit_CuCe_SameKey_SameNHit_AnyBx0_onTheta = false;
  bool nHit_CuCe_SameKey_SameNHit_AnyBx0_offTheta = false;
  bool nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onPhi = false;
  bool nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offTheta = false;
  bool nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onTheta = false;
  bool nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offPhi = false;

  for (auto const& Ce : _nHit_Ce)
  {
    int key_Ce = Ce.first;
    int nHit_Ce = Ce.second;
    for (auto const& Cu : _nHit_Cu)
    {
      int key_Cu = Cu.first;
      int nHit_Cu = Cu.second;
      /**
       * \li If RPC chamber is not same for unpacker and emulator then skip to next chamber.
       */
      if (key_Ce != key_Cu) continue;
      h1_total_hits_SameKey_Cu->Fill(nHit_Cu);
      h1_total_hits_SameKey_Ce->Fill(nHit_Ce);
      nHit_CuCe_SameKey = true;
      /**
       * \li If number of hits on the chamber under consideration for unpacker
       *   and emulator is not same then skip to next chamber.
       */
      if (nHit_Ce != nHit_Cu) continue;
      h1_total_hits_SameKey_SameNHits->Fill(nHit_Cu);
      nHit_CuCe_SameKey_SameNHit = true;
      int countNHitsHavingBxZero = 0;
      int countNHitsHavingBxZero_OnPhi = 0;
      int countNHitsHavingBxZero_OffPhi = 0;
      int countNHitsHavingBxZero_OnTheta = 0;
      int countNHitsHavingBxZero_OffTheta = 0;
      for (int vecSize = 0; vecSize < nHit_Cu; ++vecSize)
      {
        h1_SameKey_cluster_size_Cu->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_phi_Cu->Fill(_phi_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_phi_glob_Cu->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_theta_Cu->Fill(_theta_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_theta_glob_Cu->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_ID_Cu->Fill(_ID_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_zone_Cu->Fill(_zone_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_roll_Cu->Fill(_roll_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_emtfSector_Cu->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_emtfSubsector_Cu->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
        h1_SameKey_cluster_size_Ce->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_phi_Ce->Fill(_phi_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_phi_glob_Ce->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_theta_Ce->Fill(_theta_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_theta_glob_Ce->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_ID_Ce->Fill(_ID_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_zone_Ce->Fill(_zone_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_roll_Ce->Fill(_roll_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_emtfSector_Ce->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
        h1_SameKey_emtfSubsector_Ce->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
        /**
         * \li If bx == 0 for unpacker as well as emulator.
         */
        if (_bx_Cu.at(key_Cu)[vecSize] != _bx_Ce.at(key_Ce)[vecSize]) continue;
        nHit_CuCe_SameKey_SameNHit_AnyBx0 = true;
        countNHitsHavingBxZero++;
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

        h2_SameKey_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
        h2_SameKey_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        h2_SameKey_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
        h2_SameKey_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        /**
         * \li Now, set anothre set of histograms for following conditions.
         *     - OnPhi axis: phi_emulator == phi_unpacker
         *     - OffPhi axis: phi_emulator != phi_unpacker
         *     - OnTheta axis: theta_emulator == Theta_unpacker
         *     - OffTheta axis: theta_emulator != Theta_unpacker
         */
        if ( _phi_Ce.at(key_Ce)[vecSize] == _phi_Cu.at(key_Cu)[vecSize])
        {
          nHit_CuCe_SameKey_SameNHit_AnyBx0_onPhi = true;
          countNHitsHavingBxZero_OnPhi++;
          h1_SameKey_OnPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);

          h1_SameKey_OnPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

          h2_SameKey_OnPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
          h2_SameKey_OnPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
          h2_SameKey_OnPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        }
        else
        {
          nHit_CuCe_SameKey_SameNHit_AnyBx0_offPhi = true;
          countNHitsHavingBxZero_OffPhi++;
          h1_SameKey_OffPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

          h2_SameKey_OffPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
          h2_SameKey_OffPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
          h2_SameKey_OffPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        }

        // Fill histogram for on/off Theta
        if ( _theta_Ce.at(key_Ce)[vecSize] == _theta_Cu.at(key_Cu)[vecSize])
        {
          nHit_CuCe_SameKey_SameNHit_AnyBx0_onTheta=true;
          countNHitsHavingBxZero_OnTheta++;
          h1_SameKey_OnTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OnTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OnTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

          h2_SameKey_OnTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
          h2_SameKey_OnTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
          h2_SameKey_OnTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        }
        else
        {
          nHit_CuCe_SameKey_SameNHit_AnyBx0_offTheta = true;
          countNHitsHavingBxZero_OffTheta++;
          h1_SameKey_OffTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OffTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OffTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

          h2_SameKey_OffTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
          h2_SameKey_OffTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
          h2_SameKey_OffTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
        }
      }
      if (countNHitsHavingBxZero) h1_total_hits_SameKey_SameNHits_bx->Fill(countNHitsHavingBxZero);
      if (countNHitsHavingBxZero_OnPhi) h1_total_hits_SameKey_SameNHits_bx_OnPhi->Fill(countNHitsHavingBxZero_OnPhi);
      if (countNHitsHavingBxZero_OffPhi) h1_total_hits_SameKey_SameNHits_bx_OffPhi->Fill(countNHitsHavingBxZero_OffPhi);
      if (countNHitsHavingBxZero_OnTheta) h1_total_hits_SameKey_SameNHits_bx_OnTheta->Fill(countNHitsHavingBxZero_OnTheta);
      if (countNHitsHavingBxZero_OffTheta) h1_total_hits_SameKey_SameNHits_bx_OffTheta->Fill(countNHitsHavingBxZero_OffTheta);
      /**
       * ## matching scenario
       *
       * 1. Loop over unpacker ( or emulator) cppf_digis. Here, I am looping over unpacker.
       *   1. Loop over emulator cppf_digis to check the matching **on-phi (or off-phi)**.
       *     1. within this loop check if there is matching on/off phi hits or not.
       *     1. Saves a bool named `OnPhi_Matching` as true or false.
       *     1. If the boolean is true update the matching index.
       */
      int countNHitsHavingBxZero_Matching_OnPhi = 0;
      int countNHitsHavingBxZero_Matching_OffPhi = 0;
      int countNHitsHavingBxZero_Matching_OnTheta = 0;
      int countNHitsHavingBxZero_Matching_OffTheta = 0;
      for (int vecSize = 0; vecSize < nHit_Cu; ++vecSize)
      {
        /**
         *  \li Now, set anothre set of histograms for following conditions.
         *    \li OnPhi axis: phi_emulator == phi_unpacker
         *    \li OffPhi axis: phi_emulator != phi_unpacker
         *    \li OnTheta axis: theta_emulator == Theta_unpacker
         *    \li OffTheta axis: theta_emulator != Theta_unpacker
         */
        if (_bx_Cu.at(key_Cu)[vecSize] != _bx_Ce.at(key_Ce)[vecSize]) continue;
        bool OnPhi_Matching = false;
        int index_Ce = vecSize;
        int index_Cu = vecSize;
        for (int i = 0; i < nHit_Ce; ++i)
        {
          if ( _phi_Ce.at(key_Ce)[i] == _phi_Cu.at(key_Cu)[vecSize] )
          {
              OnPhi_Matching = true;
              index_Cu = vecSize;
              index_Ce = i;
          }
        }
        if (OnPhi_Matching)
        {
          nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onPhi = true;
          countNHitsHavingBxZero_Matching_OnPhi++;
          h1_Matching_SameKey_OnPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[index_Cu]);
          // h1_Matching_SameKey_OnPhi_ring_Cu_bx->Fill(_ring_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[index_Cu]);

          h1_Matching_SameKey_OnPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[index_Ce]);
          // h1_Matching_SameKey_OnPhi_ring_Ce_bx->Fill(_ring_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce]);

          h2_Matching_SameKey_OnPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce], _phi_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce], _theta_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _ID_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _roll_Ce.at(key_Ce)[index_Ce]);
          h2_Matching_SameKey_OnPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[index_Ce], _roll_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _zone_Ce.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
        }
        else
        {
          nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offPhi = true;
          countNHitsHavingBxZero_Matching_OffPhi++;
          h1_Matching_SameKey_OffPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[index_Cu]);
          // h1_Matching_SameKey_OffPhi_ring_Cu_bx->Fill(_ring_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[index_Ce]);
          // h1_Matching_SameKey_OffPhi_ring_Ce_bx->Fill(_ring_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce]);

          h2_Matching_SameKey_OffPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce], _phi_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce], _theta_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _ID_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _roll_Ce.at(key_Ce)[index_Ce]);
          h2_Matching_SameKey_OffPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[index_Ce], _roll_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _zone_Ce.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
        }

        bool OnTheta_Matching = false;
        for (int i = 0; i < nHit_Ce; ++i)
        {
          if (_theta_Ce.at(key_Ce)[i] == _theta_Cu.at(key_Cu)[index_Cu])
          {
              OnTheta_Matching = true;
              index_Cu = vecSize;
              index_Ce = i;
          }
        }
        // Fill histogram for on/off Theta
        if ( OnTheta_Matching )
        {
          nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onTheta=true;
          countNHitsHavingBxZero_Matching_OnTheta++;
          h1_Matching_SameKey_OnTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[index_Cu]);
          // h1_Matching_SameKey_OnTheta_ring_Cu_bx->Fill(_ring_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OnTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[index_Ce]);
          // h1_Matching_SameKey_OnTheta_ring_Ce_bx->Fill(_ring_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OnTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce]);

          h2_Matching_SameKey_OnTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce], _phi_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce], _theta_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _ID_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _roll_Ce.at(key_Ce)[index_Ce]);
          h2_Matching_SameKey_OnTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[index_Ce], _roll_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _zone_Ce.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OnTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
        }
        else
        {
          nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offTheta = true;
          countNHitsHavingBxZero_Matching_OffTheta++;
          h1_Matching_SameKey_OffTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[index_Cu]);
          // h1_Matching_SameKey_OffTheta_ring_Cu_bx->Fill(_ring_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h1_Matching_SameKey_OffTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[index_Ce]);
          // h1_Matching_SameKey_OffTheta_ring_Ce_bx->Fill(_ring_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[index_Ce]);
          h1_Matching_SameKey_OffTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce]);

          h2_Matching_SameKey_OffTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[index_Ce], _phi_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[index_Ce], _theta_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _ID_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[index_Ce], _roll_Ce.at(key_Ce)[index_Ce]);
          h2_Matching_SameKey_OffTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[index_Ce], _roll_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _emtfSubsector_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[index_Ce], _zone_Ce.at(key_Cu)[index_Cu]);
          h2_Matching_SameKey_OffTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[index_Ce], _zone_Cu.at(key_Cu)[index_Cu]);
        }
      }
      if (countNHitsHavingBxZero_Matching_OnPhi) h1_total_hits_SameKey_SameNHits_bx_Matching_OnPhi->Fill(countNHitsHavingBxZero_Matching_OnPhi);
      if (countNHitsHavingBxZero_Matching_OffPhi) h1_total_hits_SameKey_SameNHits_bx_Matching_OffPhi->Fill(countNHitsHavingBxZero_Matching_OffPhi);
      if (countNHitsHavingBxZero_Matching_OnTheta) h1_total_hits_SameKey_SameNHits_bx_Matching_OnTheta->Fill(countNHitsHavingBxZero_Matching_OnTheta);
      if (countNHitsHavingBxZero_Matching_OffTheta) h1_total_hits_SameKey_SameNHits_bx_Matching_OffTheta->Fill(countNHitsHavingBxZero_Matching_OffTheta);

      if (nHit_Ce == 1 && nHit_Cu == 1)
      {
        for (int vecSize = 0; vecSize < nHit_Cu; ++vecSize)
        {
          h1_SameKey_OneHit_cluster_size_Cu->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_phi_Cu->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_phi_glob_Cu->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_theta_Cu->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_theta_glob_Cu->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_ID_Cu->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_zone_Cu->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_roll_Cu->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_emtfSector_Cu->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_emtfSubsector_Cu->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_cluster_size_Ce->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_phi_Ce->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_phi_glob_Ce->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_theta_Ce->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_theta_glob_Ce->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_ID_Ce->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_zone_Ce->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_roll_Ce->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_emtfSector_Ce->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_emtfSubsector_Ce->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
          /**
           * \li If bx == 0 for unpacker as well as emulator.
           */
          if (_bx_Cu.at(key_Cu)[vecSize] != _bx_Ce.at(key_Ce)[vecSize]) continue;
          // nHit_CuCe_SameKey_SameNHit_AnyBx0 = true;
          // countNHitsHavingBxZero++;
          h1_SameKey_OneHit_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h1_SameKey_OneHit_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
          h1_SameKey_OneHit_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

          h2_SameKey_OneHit_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
          h2_SameKey_OneHit_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
          h2_SameKey_OneHit_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          /**
           * \li Now, set anothre set of histograms for following conditions.
           *     - OnPhi axis: phi_emulator == phi_unpacker
           *     - OffPhi axis: phi_emulator != phi_unpacker
           *     - OnTheta axis: theta_emulator == Theta_unpacker
           *     - OffTheta axis: theta_emulator != Theta_unpacker
           */
          if ( _phi_Ce.at(key_Ce)[vecSize] == _phi_Cu.at(key_Cu)[vecSize])
          {
            // nHit_CuCe_SameKey_SameNHit_AnyBx0_onPhi = true;
            // countNHitsHavingBxZero_OnPhi++;
            h1_SameKey_OneHit_OnPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);

            h2_SameKey_OneHit_OnPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OnPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          }
          else
          {
            // nHit_CuCe_SameKey_SameNHit_AnyBx0_offPhi = true;
            // countNHitsHavingBxZero_OffPhi++;
            h1_SameKey_OneHit_OffPhi_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffPhi_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffPhi_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OffPhi_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OffPhi_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffPhi_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          }

          // Fill histogram for on/off Theta
          if ( _theta_Ce.at(key_Ce)[vecSize] == _theta_Cu.at(key_Cu)[vecSize])
          {
            // nHit_CuCe_SameKey_SameNHit_AnyBx0_onTheta=true;
            // countNHitsHavingBxZero_OnTheta++;
            h1_SameKey_OneHit_OnTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OnTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OnTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OnTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OnTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OnTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          }
          else
          {
            // nHit_CuCe_SameKey_SameNHit_AnyBx0_offTheta = true;
            // countNHitsHavingBxZero_OffTheta++;
            h1_SameKey_OneHit_OffTheta_cluster_size_Cu_bx->Fill(_cluster_size_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_phi_Cu_bx->Fill(_phi_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_phi_glob_Cu_bx->Fill(_phi_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_theta_Cu_bx->Fill(_theta_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_theta_glob_Cu_bx->Fill(_theta_glob_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_ID_Cu_bx->Fill(_ID_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_zone_Cu_bx->Fill(_zone_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_roll_Cu_bx->Fill(_roll_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_emtfSector_Cu_bx->Fill(_emtfSector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h1_SameKey_OneHit_OffTheta_cluster_size_Ce_bx->Fill(_cluster_size_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_phi_Ce_bx->Fill(_phi_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_phi_glob_Ce_bx->Fill(_phi_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_theta_Ce_bx->Fill(_theta_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_theta_glob_Ce_bx->Fill(_theta_glob_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_ID_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_zone_Ce_bx->Fill(_zone_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_roll_Ce_bx->Fill(_roll_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_emtfSector_Ce_bx->Fill(_emtfSector_Ce.at(key_Ce)[vecSize]);
            h1_SameKey_OneHit_OffTheta_emtfSubsector_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OffTheta_phi_Ce_phi_Cu_bx->Fill(_phi_Ce.at(key_Ce)[vecSize], _phi_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_theta_Ce_theta_Cu_bx->Fill(_theta_Ce.at(key_Ce)[vecSize], _theta_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_ID_Ce_ID_Cu_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _ID_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_ID_Ce_roll_Ce_bx->Fill(_ID_Ce.at(key_Ce)[vecSize], _roll_Ce.at(key_Ce)[vecSize]);
            h2_SameKey_OneHit_OffTheta_ID_Cu_roll_Cu_bx->Fill(_ID_Cu.at(key_Ce)[vecSize], _roll_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _emtfSubsector_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_emtfSubsector_Cu_zone_Cu_bx->Fill(_emtfSubsector_Cu.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_zone_Ce_bx->Fill(_emtfSubsector_Ce.at(key_Ce)[vecSize], _zone_Ce.at(key_Cu)[vecSize]);
            h2_SameKey_OneHit_OffTheta_zone_Ce_zone_Cu_bx->Fill(_zone_Ce.at(key_Ce)[vecSize], _zone_Cu.at(key_Cu)[vecSize]);
          } // if ( _theta_Ce.at(key_Ce)[vecSize] == _theta_Cu.at(key_Cu)[vecSize])
        } // or (int vecSize = 0; vecSize < nHit_Cu; ++vecSize)
      } // if (nHit_Ce == 1 && nHit_Cu == 1)
    } // for (auto const& Cu : _nHit_Cu)
  } // for (auto const& Ce : _nHit_Ce)

  if (nHit_CuCe_SameKey) h1_nEvents_CuCe_SameKey->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit) h1_nEvents_CuCe_SameKey_SameNHits->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit_AnyBx0) h1_nEvents_CuCe_SameKey_SameNHits_AnyBx->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit_AnyBx0_onPhi) h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onPhi->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit_AnyBx0_offPhi) h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offPhi->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit_AnyBx0_onTheta) h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onTheta->Fill(1.0);
  if (nHit_CuCe_SameKey_SameNHit_AnyBx0_offTheta) h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offTheta->Fill(1.0);
  if (nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onPhi) h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onPhi->Fill(1.0);
  if (nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offTheta) h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offPhi->Fill(1.0);
  if (nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_onTheta) h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onTheta->Fill(1.0);
  if (nHit_Matching_CuCe_SameKey_SameNHit_AnyBx0_offPhi) h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offTheta->Fill(1.0);
} //End class


void DQM_CPPF::beginJob()
{
  edm::Service<TFileService> fs;

  h1_nEvents = fs->make<TH1D>("h1_nEvents", "Total number of events", 3, 0., 3.);

  h1_nEvents_Ce_NoHits = fs->make<TH1D>("h1_nEvents_Ce_NoHits","Unpacker: nHits == 0 ;#nEvents;",3, 0., 3.);
  h1_nEvents_Cu_NoHits = fs->make<TH1D>("h1_nEvents_Cu_NoHits","Emulator: nHits == 0 ;#nEvents;",3, 0., 3.);

  h1_nChmbersWithHit_Ce = fs->make<TH1D>("h1_nChmbersWithHit_Ce","Unpacker: #nChambers having Hit/event; #nChambers with Hit/event;", 40, 0., 40. );
  h1_nEvents_Ce_nHitNotMoreThan2 = fs->make<TH1D>("h1_nEvents_Ce_nHitNotMoreThan2", "Unpacker: nHits >= 2;#nEvents;", 3, 0., 3.0);
  h1_nEvents_Ce_bx = fs->make<TH1D>("h1_nEvents_Ce_bx", "Unpacker: nHits >= 2 && (bx==0 for each individual hit);#nEvents;", 3, 0., 3.0);

  h1_nChmbersWithHit_Cu = fs->make<TH1D>("h1_nChmbersWithHit_Cu","Emulator: #nChambers having Hit/event; #nChambers with Hit/event;", 40, 0., 40. );
  h1_nEvents_Cu_nHitNotMoreThan2 = fs->make<TH1D>("h1_nEvents_Cu_nHitNotMoreThan2", "Emulator: nHits >= 2;#nEvents;", 3, 0., 3.0);
  h1_nEvents_Cu_bx = fs->make<TH1D>("h1_nEvents_Cu_bx", "Emulator: nHits >= 2 && (bx==0 for each individual hit);#nEvents;", 3, 0., 3.0);

  h1_nEvents_CuCe_SameKey = fs->make<TH1D>("h1_nEvents_CuCe_SameKey","Same SubSector;#nEvents;", 3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits","Same (SubSector && nHits) ;#nEvents;", 3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits_AnyBx = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits_AnyBx","Same (SubSector && nHits) && bx == 0 ;#nEvents;", 3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onPhi = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onPhi","Same (SubSector && nHits) && bx==0 && OnPhi ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offPhi = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offPhi","Same (SubSector && nHits) && bx==0 && OffPhi ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onTheta = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_onTheta","Same (SubSector && nHits) && bx==0 && OnTheta ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offTheta = fs->make<TH1D>("h1_nEvents_CuCe_SameKey_SameNHits_AnyBx_offTheta","Same (SubSector && nHits) && bx==0 && OffTheta ;#nEvents;",3.0, 0., 3.0);

  h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onPhi = fs->make<TH1D>("h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onPhi","Same (SubSector && nHits) && bx==0 && OnPhi ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offPhi = fs->make<TH1D>("h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offPhi","Same (SubSector && nHits) && bx==0 && OffPhi ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onTheta = fs->make<TH1D>("h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_onTheta","Same (SubSector && nHits) && bx==0 && OnTheta ;#nEvents;",3.0, 0., 3.0);
  h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offTheta = fs->make<TH1D>("h1_nEvents_CuCe_Matching_SameKey_SameNHits_AnyBx_offTheta","Same (SubSector && nHits) && bx==0 && OffTheta ;#nEvents;",3.0, 0., 3.0);

  h1_total_hits_unpacker = fs->make<TH1D>("h1_total_hits_unpacker", "h1_total_hits_unpacker" , 5, 0. , 5.);
  h1_total_hits_unpacker_LessThan3Hit = fs->make<TH1D>("h1_total_hits_unpacker_LessThan3Hit", "h1_total_hits_unpacker_LessThan3Hit" , 5, 0. , 5.);
  h1_total_hits_unpacker_LessThan3Hit_BxZero = fs->make<TH1D>("h1_total_hits_unpacker_LessThan3Hit_BxZero", "h1_total_hits_unpacker_LessThan3Hit_BxZero" , 25, 0. , 25.);

  h1_total_hits_SameKey_Cu = fs->make<TH1D>("h1_total_hits_SameKey_Cu","h1_total_hits_SameKey_Cu", 5, 0., 5.);
  h1_total_hits_SameKey_Ce = fs->make<TH1D>("h1_total_hits_SameKey_Ce","h1_total_hits_SameKey_Ce", 5, 0., 5.);
  h1_total_hits_SameKey_SameNHits = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits","Same SubSector && Same #nHits;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx  = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx","Same SubSector && Same #nHits && bx==0;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_OnPhi  = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_OnPhi","Same SubSector && Same #nHits && bx==0 && OnPhi;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_OffPhi = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_OffPhi","Same SubSector && Same #nHits && bx==0 && OffPhi;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_OnTheta  = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_OnTheta","Same SubSector && Same #nHits && bx==0 && OnTheta;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_OffTheta = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_OffTheta","Same SubSector && Same #nHits && bx==0 && OffTheta;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_Matching_OnPhi = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_Matching_OnPhi","Same SubSector && Same #nHits && bx==0 && OnPhi && Matching;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_Matching_OffPhi  = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_Matching_OffPhi","Same SubSector && Same #nHits && bx==0 && OffPhi && Matching;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_Matching_OnTheta = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_Matching_OnTheta","Same SubSector && Same #nHits && bx==0 && OnTheta && Matching;#nHits;",5, 0., 5.);
  h1_total_hits_SameKey_SameNHits_bx_Matching_OffTheta  = fs->make<TH1D>("h1_total_hits_SameKey_SameNHits_bx_Matching_OffTheta","Same SubSector && Same #nHits && bx==0 && OffTheta && Matching;#nHits;",5, 0., 5.);

  h1_cluster_size_Cu = fs->make<TH1D>("h1_cluster_size_Cu","Unpacker: No Cuts; Cluster Size ;", 10, 0., 10.);
  h1_phi_Cu = fs->make<TH1D>("h1_phi_Cu","Unpacker: No Cuts; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Cu = fs->make<TH1D>("h1_phi_glob_Cu","Unpacker: No Cuts; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Cu = fs->make<TH1D>("h1_theta_Cu","Unpacker: No Cuts; #theta ;", 32, 0, 32.);
  h1_theta_glob_Cu = fs->make<TH1D>("h1_theta_glob_Cu","Unpacker: No Cuts; Global #theta;", 180., 0, 180.);
  h1_ID_Cu = fs->make<TH1D>("h1_ID_Cu","Unpacker: No Cuts; Chamber ID;", 38, 0, 38);
  h1_zone_Cu = fs->make<TH1D>("h1_zone_Cu","Unpacker: No Cuts; Zone ;", 15, 0, 15);
  h1_roll_Cu = fs->make<TH1D>("h1_roll_Cu","Unpacker: No Cuts; Roll ;", 4, 0, 4);
  h1_emtfSector_Cu = fs->make<TH1D>("h1_emtfSector_Cu","Unpacker: No Cuts; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Cu = fs->make<TH1D>("h1_emtfSubsector_Cu","Unpacker: No Cuts; EMTF Sub-Sector ;", 38, 0, 38);

  h1_cluster_size_Cu_bx = fs->make<TH1D>("h1_cluster_size_Cu_bx","Unpacker: Bx==0; Cluster Size ;", 10, 0., 10.);
  h1_phi_Cu_bx = fs->make<TH1D>("h1_phi_Cu_bx","Unpacker: Bx==0; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Cu_bx = fs->make<TH1D>("h1_phi_glob_Cu_bx","Unpacker: Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Cu_bx = fs->make<TH1D>("h1_theta_Cu_bx","Unpacker: Bx==0; #theta ;", 32, 0, 32.);
  h1_theta_glob_Cu_bx = fs->make<TH1D>("h1_theta_glob_Cu_bx","Unpacker: Bx==0; Global #theta;", 180., 0, 180.);
  h1_ID_Cu_bx = fs->make<TH1D>("h1_ID_Cu_bx","Unpacker: Bx==0; Chamber ID;", 38, 0, 38);
  h1_zone_Cu_bx = fs->make<TH1D>("h1_zone_Cu_bx","Unpacker: Bx==0; Zone ;", 15, 0, 15);
  h1_roll_Cu_bx = fs->make<TH1D>("h1_roll_Cu_bx","Unpacker: Bx==0; Roll ;", 4, 0, 4);
  h1_emtfSector_Cu_bx = fs->make<TH1D>("h1_emtfSector_Cu_bx","Unpacker: Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_emtfSubsector_Cu_bx","Unpacker: Bx==0; EMTF Sub-Sector ;", 38, 0, 38);

  h1_cluster_size_Cu_bx_OneHit = fs->make<TH1D>("h1_cluster_size_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Cluster Size ;", 10, 0., 10.);
  h1_phi_Cu_bx_OneHit = fs->make<TH1D>("h1_phi_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Cu_bx_OneHit = fs->make<TH1D>("h1_phi_glob_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Cu_bx_OneHit = fs->make<TH1D>("h1_theta_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; #theta ;", 32, 0, 32.);
  h1_theta_glob_Cu_bx_OneHit = fs->make<TH1D>("h1_theta_glob_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Global #theta;", 180., 0, 180.);
  h1_ID_Cu_bx_OneHit = fs->make<TH1D>("h1_ID_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Chamber ID;", 38, 0, 38);
  h1_zone_Cu_bx_OneHit = fs->make<TH1D>("h1_zone_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Zone ;", 15, 0, 15);
  h1_roll_Cu_bx_OneHit = fs->make<TH1D>("h1_roll_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; Roll ;", 4, 0, 4);
  h1_emtfSector_Cu_bx_OneHit = fs->make<TH1D>("h1_emtfSector_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Cu_bx_OneHit = fs->make<TH1D>("h1_emtfSubsector_Cu_bx_OneHit","Unpacker: Bx==0 && One Hit; EMTF Sub-Sector ;", 38, 0, 38);

  h1_total_hits_emulator = fs->make<TH1D>("h1_total_hits_emulator", "CPPFDigis_total_hits_emulator" , 5, 0. , 5.);
  h1_total_hits_emulator_LessThan3Hit = fs->make<TH1D>("h1_total_hits_emulator_LessThan3Hit", "h1_total_hits_emulator_LessThan3Hit" , 5, 0. , 5.);
  h1_total_hits_emulator_LessThan3Hit_BxZero = fs->make<TH1D>("h1_total_hits_emulator_LessThan3Hit_BxZero", "h1_total_hits_emulator_LessThan3Hit_BxZero" , 5, 0. , 5.);

  h1_cluster_size_Ce = fs->make<TH1D>("h1_cluster_size_Ce","Emulator: No Cuts; Cluster Size ;", 10, 0., 10.);
  h1_phi_Ce = fs->make<TH1D>("h1_phi_Ce","Emulator: No Cuts; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Ce = fs->make<TH1D>("h1_phi_glob_Ce","Emulator: No Cuts; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Ce = fs->make<TH1D>("h1_theta_Ce","Emulator: No Cuts; #theta ;", 32, 0, 32.);
  h1_theta_glob_Ce = fs->make<TH1D>("h1_theta_glob_Ce","Emulator: No Cuts; Global #theta;", 180., 0, 180.);
  h1_ID_Ce = fs->make<TH1D>("h1_ID_Ce","Emulator: No Cuts; Chamber ID;", 38, 0, 38);
  h1_zone_Ce = fs->make<TH1D>("h1_zone_Ce","Emulator: No Cuts; Zone ;", 15, 0, 15);
  h1_roll_Ce = fs->make<TH1D>("h1_roll_Ce","Emulator: No Cuts; Roll ;", 4, 0, 4);
  h1_emtfSector_Ce = fs->make<TH1D>("h1_emtfSector_Ce","Emulator: No Cuts; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Ce = fs->make<TH1D>("h1_emtfSubsector_Ce","Emulator: No Cuts; EMTF Sub-Sector ;", 38, 0, 38);

  h1_cluster_size_Ce_bx = fs->make<TH1D>("h1_cluster_size_Ce_bx","Emulator: Bx==0; Cluster Size ;", 10, 0., 10.);
  h1_phi_Ce_bx = fs->make<TH1D>("h1_phi_Ce_bx","Emulator: Bx==0; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Ce_bx = fs->make<TH1D>("h1_phi_glob_Ce_bx","Emulator: Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Ce_bx = fs->make<TH1D>("h1_theta_Ce_bx","Emulator: Bx==0; #theta ;", 32, 0, 32.);
  h1_theta_glob_Ce_bx = fs->make<TH1D>("h1_theta_glob_Ce_bx","Emulator: Bx==0; Global #theta;", 180., 0, 180.);
  h1_ID_Ce_bx = fs->make<TH1D>("h1_ID_Ce_bx","Emulator: Bx==0; Chamber ID;", 38, 0, 38);
  h1_zone_Ce_bx = fs->make<TH1D>("h1_zone_Ce_bx","Emulator: Bx==0; Zone ;", 15, 0, 15);
  h1_roll_Ce_bx = fs->make<TH1D>("h1_roll_Ce_bx","Emulator: Bx==0; Roll ;", 4, 0, 4);
  h1_emtfSector_Ce_bx = fs->make<TH1D>("h1_emtfSector_Ce_bx","Emulator: Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_emtfSubsector_Ce_bx","Emulator: Bx==0; EMTF Sub-Sector ;", 38, 0, 38);

  h1_cluster_size_Ce_bx_OneHit = fs->make<TH1D>("h1_cluster_size_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Cluster Size ;", 10, 0., 10.);
  h1_phi_Ce_bx_OneHit = fs->make<TH1D>("h1_phi_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; #phi;", 62, 0.0, 1240.);
  h1_phi_glob_Ce_bx_OneHit = fs->make<TH1D>("h1_phi_glob_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Global #phi;", 180, 0.0, 180.0);
  h1_theta_Ce_bx_OneHit = fs->make<TH1D>("h1_theta_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; #theta ;", 32, 0, 32.);
  h1_theta_glob_Ce_bx_OneHit = fs->make<TH1D>("h1_theta_glob_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Global #theta;", 180., 0, 180.);
  h1_ID_Ce_bx_OneHit = fs->make<TH1D>("h1_ID_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Chamber ID;", 38, 0, 38);
  h1_zone_Ce_bx_OneHit = fs->make<TH1D>("h1_zone_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Zone ;", 15, 0, 15);
  h1_roll_Ce_bx_OneHit = fs->make<TH1D>("h1_roll_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; Roll ;", 4, 0, 4);
  h1_emtfSector_Ce_bx_OneHit = fs->make<TH1D>("h1_emtfSector_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; EMTF Sector ;", 8, 0, 8);
  h1_emtfSubsector_Ce_bx_OneHit = fs->make<TH1D>("h1_emtfSubsector_Ce_bx_OneHit","Emulator: Bx==0 && OneHit; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_cluster_size_Cu = fs->make<TH1D>("h1_SameKey_cluster_size_Cu","Unpacker: Same SubSector; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_phi_Cu = fs->make<TH1D>("h1_SameKey_phi_Cu","Unpacker: Same SubSector; #phi;", 62, 0.0, 1240.);
  h1_SameKey_phi_glob_Cu = fs->make<TH1D>("h1_SameKey_phi_glob_Cu","Unpacker: Same SubSector; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_theta_Cu = fs->make<TH1D>("h1_SameKey_theta_Cu","Unpacker: Same SubSector; #theta ;", 32, 0, 32.);
  h1_SameKey_theta_glob_Cu = fs->make<TH1D>("h1_SameKey_theta_glob_Cu","Unpacker: Same SubSector; Global #theta;", 180., 0, 180.);
  h1_SameKey_ID_Cu = fs->make<TH1D>("h1_SameKey_ID_Cu","Unpacker: Same SubSector; Chamber ID;", 38, 0, 38);
  h1_SameKey_zone_Cu = fs->make<TH1D>("h1_SameKey_zone_Cu","Unpacker: Same SubSector; Zone ;", 15, 0, 15);
  h1_SameKey_roll_Cu = fs->make<TH1D>("h1_SameKey_roll_Cu","Unpacker: Same SubSector; Roll ;", 4, 0, 4);
  h1_SameKey_emtfSector_Cu = fs->make<TH1D>("h1_SameKey_emtfSector_Cu","Unpacker: Same SubSector; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_emtfSubsector_Cu = fs->make<TH1D>("h1_SameKey_emtfSubsector_Cu","Unpacker: Same SubSector; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_cluster_size_Ce = fs->make<TH1D>("h1_SameKey_cluster_size_Ce","Emulator: Same SubSector; Cluster Size ", 10, 0., 10.);
  h1_SameKey_phi_Ce = fs->make<TH1D>("h1_SameKey_phi_Ce","Emulator: Same SubSector; #phi;", 62, 0.0, 1240.);
  h1_SameKey_phi_glob_Ce = fs->make<TH1D>("h1_SameKey_phi_glob_Ce","Emulator: Same SubSector; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_theta_Ce = fs->make<TH1D>("h1_SameKey_theta_Ce","Emulator: Same SubSector; #theta ;", 32, 0, 32.);
  h1_SameKey_theta_glob_Ce = fs->make<TH1D>("h1_SameKey_theta_glob_Ce","Emulator: Same SubSector; Global #theta;", 180., 0, 180.);
  h1_SameKey_ID_Ce = fs->make<TH1D>("h1_SameKey_ID_Ce","Emulator: Same SubSector; Chamber ID;", 38, 0, 38);
  h1_SameKey_zone_Ce = fs->make<TH1D>("h1_SameKey_zone_Ce","Emulator: Same SubSector; Zone ;", 15, 0, 15);
  h1_SameKey_roll_Ce = fs->make<TH1D>("h1_SameKey_roll_Ce","Emulator: Same SubSector; Roll ;", 4, 0, 4);
  h1_SameKey_emtfSector_Ce = fs->make<TH1D>("h1_SameKey_emtfSector_Ce","Emulator: Same SubSector; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_emtfSubsector_Ce = fs->make<TH1D>("h1_SameKey_emtfSubsector_Ce","Emulator: Same SubSector; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0; Cluster Size ", 10, 0., 10.);
  h1_SameKey_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_phi_Cu_bx","Unpacker: Same SubSector && Bx==0; #phi;", 62, 0.0, 1240.);
  h1_SameKey_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_theta_Cu_bx","Unpacker: Same SubSector && Bx==0; #theta ;", 32, 0, 32.);
  h1_SameKey_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0; Global #theta;", 180., 0, 180.);
  h1_SameKey_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_ID_Cu_bx","Unpacker: Same SubSector && Bx==0; Chamber ID;", 38, 0, 38);
  h1_SameKey_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_zone_Cu_bx","Unpacker: Same SubSector && Bx==0; Zone ;", 15, 0, 15);
  h1_SameKey_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_roll_Cu_bx","Unpacker: Same SubSector && Bx==0; Roll ;", 4, 0, 4);
  h1_SameKey_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0; Cluster Size ", 10, 0., 10.);
  h1_SameKey_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_phi_Ce_bx","Emulator: Same SubSector && Bx==0; #phi;", 62, 0.0, 1240.);
  h1_SameKey_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_theta_Ce_bx","Emulator: Same SubSector && Bx==0; #theta ;", 32, 0, 32.);
  h1_SameKey_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0; Global #theta;", 180., 0, 180.);
  h1_SameKey_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_ID_Ce_bx","Emulator: Same SubSector && Bx==0; Chamber ID;", 38, 0, 38);
  h1_SameKey_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_zone_Ce_bx","Emulator: Same SubSector && Bx==0; Zone ;", 15, 0, 15);
  h1_SameKey_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_roll_Ce_bx","Emulator: Same SubSector && Bx==0; Roll ;", 4, 0, 4);
  h1_SameKey_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OnPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OnPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OnPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OnPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OnPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OnPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OnPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OnPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OnPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OnPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OnPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OnPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OnPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OnPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OnPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OnPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OnPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OnPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OnPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OnPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_cluster_size_Cu = fs->make<TH1D>("h1_SameKey_OneHit_cluster_size_Cu","Unpacker: Same SubSector && One Hit; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_phi_Cu = fs->make<TH1D>("h1_SameKey_OneHit_phi_Cu","Unpacker: Same SubSector && One Hit; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_phi_glob_Cu = fs->make<TH1D>("h1_SameKey_OneHit_phi_glob_Cu","Unpacker: Same SubSector && One Hit; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_theta_Cu = fs->make<TH1D>("h1_SameKey_OneHit_theta_Cu","Unpacker: Same SubSector && One Hit; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_theta_glob_Cu = fs->make<TH1D>("h1_SameKey_OneHit_theta_glob_Cu","Unpacker: Same SubSector && One Hit; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_ID_Cu = fs->make<TH1D>("h1_SameKey_OneHit_ID_Cu","Unpacker: Same SubSector && One Hit; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_zone_Cu = fs->make<TH1D>("h1_SameKey_OneHit_zone_Cu","Unpacker: Same SubSector && One Hit; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_roll_Cu = fs->make<TH1D>("h1_SameKey_OneHit_roll_Cu","Unpacker: Same SubSector && One Hit; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_emtfSector_Cu = fs->make<TH1D>("h1_SameKey_OneHit_emtfSector_Cu","Unpacker: Same SubSector && One Hit; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_emtfSubsector_Cu = fs->make<TH1D>("h1_SameKey_OneHit_emtfSubsector_Cu","Unpacker: Same SubSector && One Hit; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_cluster_size_Ce = fs->make<TH1D>("h1_SameKey_OneHit_cluster_size_Ce","Emulator: Same SubSector && One Hit; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OneHit_phi_Ce = fs->make<TH1D>("h1_SameKey_OneHit_phi_Ce","Emulator: Same SubSector && One Hit; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_phi_glob_Ce = fs->make<TH1D>("h1_SameKey_OneHit_phi_glob_Ce","Emulator: Same SubSector && One Hit; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_theta_Ce = fs->make<TH1D>("h1_SameKey_OneHit_theta_Ce","Emulator: Same SubSector && One Hit; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_theta_glob_Ce = fs->make<TH1D>("h1_SameKey_OneHit_theta_glob_Ce","Emulator: Same SubSector && One Hit; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_ID_Ce = fs->make<TH1D>("h1_SameKey_OneHit_ID_Ce","Emulator: Same SubSector && One Hit; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_zone_Ce = fs->make<TH1D>("h1_SameKey_OneHit_zone_Ce","Emulator: Same SubSector && One Hit; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_roll_Ce = fs->make<TH1D>("h1_SameKey_OneHit_roll_Ce","Emulator: Same SubSector && One Hit; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_emtfSector_Ce = fs->make<TH1D>("h1_SameKey_OneHit_emtfSector_Ce","Emulator: Same SubSector && One Hit; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_emtfSubsector_Ce = fs->make<TH1D>("h1_SameKey_OneHit_emtfSubsector_Ce","Emulator: Same SubSector && One Hit; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_cluster_size_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OneHit_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_phi_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_phi_glob_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_theta_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_theta_glob_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_ID_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_zone_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_roll_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_emtfSector_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_emtfSubsector_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_cluster_size_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OneHit_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_phi_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_phi_glob_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_theta_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_theta_glob_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_ID_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_zone_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_roll_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_emtfSector_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_emtfSubsector_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_OnPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OneHit_OnPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_phi_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OnPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OnPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_theta_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OnPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OnPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_ID_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OnPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_zone_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OnPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_roll_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OnPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OnPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && One Hit && Bx==0 && OnPhi; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_OnPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_cluster_size_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Cluster Size ", 10, 0., 10.);
  h1_SameKey_OneHit_OnPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_phi_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OnPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_phi_glob_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OnPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_theta_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OnPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_theta_glob_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OnPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_ID_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OnPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_zone_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OnPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_roll_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OnPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_emtfSector_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OnPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && One Hit && Bx==0 && OnPhi; EMTF Sub-Sector ;", 38, 0, 38);

  h1_Matching_SameKey_OnPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OnPhi_phi_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OnPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OnPhi_theta_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OnPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OnPhi_ID_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OnPhi_zone_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OnPhi_roll_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OnPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OnPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnPhi && Matching; EMTF Sub-Sector ;", 38, 0, 38);
  h1_Matching_SameKey_OnPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OnPhi_phi_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OnPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OnPhi_theta_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OnPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OnPhi_ID_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OnPhi_zone_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OnPhi_roll_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OnPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OnPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnPhi && Matching; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OffPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OffPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OffPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OffPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OffPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OffPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OffPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OffPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OffPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OffPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OffPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OffPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OffPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OffPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OffPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OffPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OffPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OffPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OffPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OffPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_OffPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OffPhi_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_phi_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OffPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OffPhi_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_theta_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OffPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OffPhi_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_ID_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OffPhi_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_zone_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OffPhi_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_roll_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OffPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OffPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffPhi; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_OffPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_cluster_size_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OffPhi_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_phi_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OffPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_phi_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OffPhi_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_theta_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OffPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_theta_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OffPhi_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_ID_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OffPhi_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_zone_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OffPhi_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_roll_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OffPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_emtfSector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OffPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffPhi; EMTF Sub-Sector ;", 38, 0, 38);

  h1_Matching_SameKey_OffPhi_cluster_size_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OffPhi_phi_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OffPhi_phi_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OffPhi_theta_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OffPhi_theta_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OffPhi_ID_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OffPhi_zone_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OffPhi_roll_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OffPhi_emtfSector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OffPhi_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffPhi && Matching; EMTF Sub-Sector ;", 38, 0, 38);
  h1_Matching_SameKey_OffPhi_cluster_size_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OffPhi_phi_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OffPhi_phi_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OffPhi_theta_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OffPhi_theta_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OffPhi_ID_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OffPhi_zone_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OffPhi_roll_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OffPhi_emtfSector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OffPhi_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffPhi_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffPhi && Matching; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OnTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OnTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OnTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OnTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OnTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OnTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OnTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OnTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OnTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OnTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OnTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OnTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OnTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OnTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OnTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OnTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OnTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OnTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OnTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OnTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OnTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_OnTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OnTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_phi_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OnTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OnTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_theta_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OnTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OnTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_ID_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OnTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_zone_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OnTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_roll_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OnTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OnTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OnTheta; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_OnTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_cluster_size_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OnTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_phi_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OnTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_phi_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OnTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_theta_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OnTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_theta_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OnTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_ID_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OnTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_zone_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OnTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_roll_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OnTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_emtfSector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OnTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OnTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OnTheta; EMTF Sub-Sector ;", 38, 0, 38);

  h1_Matching_SameKey_OnTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OnTheta_phi_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OnTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OnTheta_theta_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OnTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OnTheta_ID_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OnTheta_zone_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OnTheta_roll_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OnTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OnTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OnTheta && Matching; EMTF Sub-Sector ;", 38, 0, 38);
  h1_Matching_SameKey_OnTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OnTheta_phi_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OnTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OnTheta_theta_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OnTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OnTheta_ID_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OnTheta_zone_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OnTheta_roll_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OnTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OnTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OnTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OnTheta && Matching; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OffTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OffTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OffTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OffTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OffTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OffTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OffTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OffTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OffTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OffTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OffTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OffTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OffTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OffTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OffTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OffTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OffTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OffTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OffTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OffTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OffTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta; EMTF Sub-Sector ;", 38, 0, 38);

  h1_SameKey_OneHit_OffTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OffTheta_phi_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_phi_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OffTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OffTheta_theta_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_theta_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OffTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OffTheta_ID_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_ID_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OffTheta_zone_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_zone_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OffTheta_roll_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_roll_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OffTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OffTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && OneHit && Bx==0 && OffTheta; EMTF Sub-Sector ;", 38, 0, 38);
  h1_SameKey_OneHit_OffTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_cluster_size_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Cluster Size ;", 10, 0., 10.);
  h1_SameKey_OneHit_OffTheta_phi_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_phi_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; #phi;", 62, 0.0, 1240.);
  h1_SameKey_OneHit_OffTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_phi_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Global #phi;", 180, 0.0, 180.0);
  h1_SameKey_OneHit_OffTheta_theta_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_theta_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; #theta ;", 32, 0, 32.);
  h1_SameKey_OneHit_OffTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_theta_glob_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Global #theta;", 180., 0, 180.);
  h1_SameKey_OneHit_OffTheta_ID_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_ID_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Chamber ID;", 38, 0, 38);
  h1_SameKey_OneHit_OffTheta_zone_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_zone_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Zone ;", 15, 0, 15);
  h1_SameKey_OneHit_OffTheta_roll_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_roll_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; Roll ;", 4, 0, 4);
  h1_SameKey_OneHit_OffTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_emtfSector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; EMTF Sector ;", 8, 0, 8);
  h1_SameKey_OneHit_OffTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_SameKey_OneHit_OffTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && OneHit && Bx==0 && OffTheta; EMTF Sub-Sector ;", 38, 0, 38);

  h1_Matching_SameKey_OffTheta_cluster_size_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_cluster_size_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OffTheta_phi_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_phi_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OffTheta_phi_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_phi_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OffTheta_theta_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_theta_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OffTheta_theta_glob_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_theta_glob_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OffTheta_ID_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_ID_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OffTheta_zone_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_zone_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OffTheta_roll_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_roll_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OffTheta_emtfSector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_emtfSector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OffTheta_emtfSubsector_Cu_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_emtfSubsector_Cu_bx","Unpacker: Same SubSector && Bx==0 && OffTheta && Matching; EMTF Sub-Sector ;", 38, 0, 38);
  h1_Matching_SameKey_OffTheta_cluster_size_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_cluster_size_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Cluster Size ;", 10, 0., 10.);
  h1_Matching_SameKey_OffTheta_phi_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_phi_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; #phi;", 62, 0.0, 1240.);
  h1_Matching_SameKey_OffTheta_phi_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_phi_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Global #phi;", 180, 0.0, 180.0);
  h1_Matching_SameKey_OffTheta_theta_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_theta_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; #theta ;", 32, 0, 32.);
  h1_Matching_SameKey_OffTheta_theta_glob_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_theta_glob_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Global #theta;", 180., 0, 180.);
  h1_Matching_SameKey_OffTheta_ID_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_ID_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Chamber ID;", 38, 0, 38);
  h1_Matching_SameKey_OffTheta_zone_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_zone_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Zone ;", 15, 0, 15);
  h1_Matching_SameKey_OffTheta_roll_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_roll_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; Roll ;", 4, 0, 4);
  h1_Matching_SameKey_OffTheta_emtfSector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_emtfSector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; EMTF Sector ;", 8, 0, 8);
  h1_Matching_SameKey_OffTheta_emtfSubsector_Ce_bx = fs->make<TH1D>("h1_Matching_SameKey_OffTheta_emtfSubsector_Ce_bx","Emulator: Same SubSector && Bx==0 && OffTheta && Matching; EMTF Sub-Sector ;", 38, 0, 38);

  h2_SameKey_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_phi_Ce_phi_Cu_bx","Same SubSector; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_theta_Ce_theta_Cu_bx","Same SubSector && bx==0; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_ID_Ce_ID_Cu_bx","Same SubSector && bx==0; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_ID_Ce_roll_Ce_bx","Same SubSector && bx==0;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_ID_Cu_roll_Cu_bx","Same SubSector && bx==0;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && bx==0;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && bx==0;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && bx==0;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_zone_Ce_zone_Cu_bx","Same SubSector && bx==0;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OnPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_phi_Ce_phi_Cu_bx","Same SubSector && OnPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OnPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_theta_Ce_theta_Cu_bx","Same SubSector && bx==0 && OnPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OnPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_ID_Ce_ID_Cu_bx","Same SubSector && bx==0 && OnPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OnPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OnPhi_ID_Ce_roll_Ce_bx","Same SubSector && bx==0 && OnPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OnPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_ID_Cu_roll_Cu_bx","Same SubSector && bx==0 && OnPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && bx==0 && OnPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && bx==0 && OnPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OnPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OnPhi_zone_Ce_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OneHit_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_phi_Ce_phi_Cu_bx","Same SubSector && OneHit && bx==0; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OneHit_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_theta_Ce_theta_Cu_bx","Same SubSector && OneHit && bx==0; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OneHit_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_ID_Ce_ID_Cu_bx","Same SubSector && OneHit && bx==0; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OneHit_ID_Ce_roll_Ce_bx","Same SubSector && OneHit && bx==0;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_ID_Cu_roll_Cu_bx","Same SubSector && OneHit && bx==0;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && OneHit && bx==0;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && OneHit && bx==0;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OneHit_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && OneHit && bx==0;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_zone_Ce_zone_Cu_bx","Same SubSector && OneHit && bx==0;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OneHit_OnPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_phi_Ce_phi_Cu_bx","Same SubSector && OneHit && OnPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OneHit_OnPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_theta_Ce_theta_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OneHit_OnPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_ID_Ce_ID_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OnPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_ID_Ce_roll_Ce_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OnPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_ID_Cu_roll_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OnPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OnPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnPhi_zone_Ce_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_Matching_SameKey_OnPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_phi_Ce_phi_Cu_bx","Matching && Same SubSector && OnPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_Matching_SameKey_OnPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_theta_Ce_theta_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_Matching_SameKey_OnPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_ID_Ce_ID_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OnPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_ID_Ce_roll_Ce_bx","Matching && Same SubSector && bx==0 && OnPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OnPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_ID_Cu_roll_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_emtfSubsector_Cu_zone_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_emtfSubsector_Ce_zone_Ce_bx","Matching && Same SubSector && bx==0 && OnPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OnPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnPhi_zone_Ce_zone_Cu_bx","Matching && Same SubSector && bx==0 && OnPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OffPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_phi_Ce_phi_Cu_bx","Same SubSector && OffPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OffPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_theta_Ce_theta_Cu_bx","Same SubSector && bx==0 && OffPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OffPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_ID_Ce_ID_Cu_bx","Same SubSector && bx==0 && OffPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OffPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OffPhi_ID_Ce_roll_Ce_bx","Same SubSector && bx==0 && OffPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OffPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_ID_Cu_roll_Cu_bx","Same SubSector && bx==0 && OffPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && bx==0 && OffPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && bx==0 && OffPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && bx==0 && OffPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OffPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OffPhi_zone_Ce_zone_Cu_bx","Same SubSector && bx==0 && OffPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OneHit_OffPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_phi_Ce_phi_Cu_bx","Same SubSector && OneHit && OffPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OneHit_OffPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_theta_Ce_theta_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OneHit_OffPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_ID_Ce_ID_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OffPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_ID_Ce_roll_Ce_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OffPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_ID_Cu_roll_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OffPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OffPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffPhi_zone_Ce_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OffPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_Matching_SameKey_OffPhi_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_phi_Ce_phi_Cu_bx","Matching && Same SubSector && OffPhi ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_Matching_SameKey_OffPhi_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_theta_Ce_theta_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_Matching_SameKey_OffPhi_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_ID_Ce_ID_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OffPhi_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_ID_Ce_roll_Ce_bx","Matching && Same SubSector && bx==0 && OffPhi ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OffPhi_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_ID_Cu_roll_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_emtfSubsector_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_emtfSubsector_Cu_zone_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_emtfSubsector_Ce_zone_Ce_bx","Matching && Same SubSector && bx==0 && OffPhi ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OffPhi_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffPhi_zone_Ce_zone_Cu_bx","Matching && Same SubSector && bx==0 && OffPhi ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OnTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_phi_Ce_phi_Cu_bx","Same SubSector && OnTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OnTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_theta_Ce_theta_Cu_bx","Same SubSector && bx==0 && OnTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OnTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_ID_Ce_ID_Cu_bx","Same SubSector && bx==0 && OnTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OnTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OnTheta_ID_Ce_roll_Ce_bx","Same SubSector && bx==0 && OnPhi ;EmulaTheta Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OnTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_ID_Cu_roll_Cu_bx","Same SubSector && bx==0 && OnPhi ;UnpacTheta Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && bx==0 && OnPhi ;EmulaTheta EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;UnpacTheta EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && bx==0 && OnPhi ;EmulaTheta EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OnTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OnTheta_zone_Ce_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;EmulaTheta Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OneHit_OnTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_phi_Ce_phi_Cu_bx","Same SubSector && OneHit && OnTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OneHit_OnTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_theta_Ce_theta_Cu_bx","Same SubSector && OneHit && bx==0 && OnTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OneHit_OnTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_ID_Ce_ID_Cu_bx","Same SubSector && OneHit && bx==0 && OnTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OnTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_ID_Ce_roll_Ce_bx","Same SubSector && OneHit && bx==0 && OnPhi ;EmulaTheta Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OnTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_ID_Cu_roll_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;UnpacTheta Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;EmulaTheta EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OnTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;UnpacTheta EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && OneHit && bx==0 && OnPhi ;EmulaTheta EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OnTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OnTheta_zone_Ce_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OnPhi ;EmulaTheta Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_Matching_SameKey_OnTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_phi_Ce_phi_Cu_bx","Matching && Same SubSector && OnTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_Matching_SameKey_OnTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_theta_Ce_theta_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_Matching_SameKey_OnTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_ID_Ce_ID_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OnTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_ID_Ce_roll_Ce_bx","Matching && Same SubSector && bx==0 && OnTheta ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OnTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_ID_Cu_roll_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_emtfSubsector_Cu_zone_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_emtfSubsector_Ce_zone_Ce_bx","Matching && Same SubSector && bx==0 && OnTheta ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OnTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OnTheta_zone_Ce_zone_Cu_bx","Matching && Same SubSector && bx==0 && OnTheta ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OffTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_phi_Ce_phi_Cu_bx","Same SubSector && OffTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OffTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_theta_Ce_theta_Cu_bx","Same SubSector && bx==0 && OffTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OffTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_ID_Ce_ID_Cu_bx","Same SubSector && bx==0 && OffTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OffTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OffTheta_ID_Ce_roll_Ce_bx","Same SubSector && bx==0 && OnPhi ;EmulffTheta Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OffTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_ID_Cu_roll_Cu_bx","Same SubSector && bx==0 && OnPhi ;UnpaffTheta Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && bx==0 && OnPhi ;EmulffTheta EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;UnpaffTheta EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && bx==0 && OnPhi ;EmulffTheta EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OffTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OffTheta_zone_Ce_zone_Cu_bx","Same SubSector && bx==0 && OnPhi ;EmulffTheta Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_SameKey_OneHit_OffTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_phi_Ce_phi_Cu_bx","Same SubSector && OneHit && OffTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_SameKey_OneHit_OffTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_theta_Ce_theta_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_SameKey_OneHit_OffTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_ID_Ce_ID_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OffTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_ID_Ce_roll_Ce_bx","Same SubSector && OneHit && bx==0 && OffTheta ;EmulffTheta Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OffTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_ID_Cu_roll_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ;UnpaffTheta Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ;EmulffTheta EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_SameKey_OneHit_OffTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_emtfSubsector_Cu_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ;UnpaffTheta EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_emtfSubsector_Ce_zone_Ce_bx","Same SubSector && OneHit && bx==0 && OffTheta ;EmulffTheta EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_SameKey_OneHit_OffTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_SameKey_OneHit_OffTheta_zone_Ce_zone_Cu_bx","Same SubSector && OneHit && bx==0 && OffTheta ;EmulffTheta Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);

  h2_Matching_SameKey_OffTheta_phi_Ce_phi_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_phi_Ce_phi_Cu_bx","Matching && Same SubSector && OffTheta ; Emulator #phi ; Unpacker #phi", 62, 0.0, 1240., 62, 0.0, 1240.);
  h2_Matching_SameKey_OffTheta_theta_Ce_theta_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_theta_Ce_theta_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ; Emulator #theta ; Unpacker #theta ", 32, 0, 32., 32, 0, 32.);
  h2_Matching_SameKey_OffTheta_ID_Ce_ID_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_ID_Ce_ID_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ; Emulator Chamber ID;Unpacker Chamber ID ", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OffTheta_ID_Ce_roll_Ce_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_ID_Ce_roll_Ce_bx","Matching && Same SubSector && bx==0 && OffTheta ;Emulator Chamber ID ;Emulator Roll ", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OffTheta_ID_Cu_roll_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_ID_Cu_roll_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ;Unpacker Chamber ID ;Unpacker Roll", 38, 0, 38, 4, 0, 4);
  h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_emtfSubsector_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ;Emulator EMTF SubSector ; Unpacker EMTF SubSector", 38, 0, 38, 38, 0, 38);
  h2_Matching_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_emtfSubsector_Cu_zone_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ;Unpacker EMTF SubSector ;Unpacker Zone ", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx   = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_emtfSubsector_Ce_zone_Ce_bx","Matching && Same SubSector && bx==0 && OffTheta ;Emulator EMTF SubSector ;Emulator Zone", 38, 0, 38, 15, 0, 15);
  h2_Matching_SameKey_OffTheta_zone_Ce_zone_Cu_bx = fs->make<TH2D>("h2_Matching_SameKey_OffTheta_zone_Ce_zone_Cu_bx","Matching && Same SubSector && bx==0 && OffTheta ;Emulator Zone ;Unpacker Zone ", 15, 0, 15, 15, 0, 15);
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
