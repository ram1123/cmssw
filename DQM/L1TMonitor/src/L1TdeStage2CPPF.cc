#include <string>

#include "DQM/L1TMonitor/interface/L1TdeStage2CPPF.h"


L1TdeStage2CPPF::L1TdeStage2CPPF(const edm::ParameterSet& ps)
    : dataToken(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emulToken(consumes<l1t::CPPFDigiCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TdeStage2CPPF::~L1TdeStage2CPPF() {}

void L1TdeStage2CPPF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {
//	c.get<MuonGeometryRecord>().get(rpcGeom);
}

//void L1TdeStage2CPPF::dqmBeginRun(const edm::Run& run, const edm::EventSetup& iSetup){
//  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
//}

void L1TdeStage2CPPF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(monitorDir);

//  cppfDataBX = ibooker.book1D("cppfDataBX", "CPPF data BX", 7, -3, 4);
//  cppfDataBX->setAxisTitle("BX", 1);
//
//  cppfEmulBX = ibooker.book1D("cppfEmulBX", "CPPF emulator BX", 7, -3, 4);
//  cppfEmulBX->setAxisTitle("BX", 1);
//
//  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
//    cppfDataBX->setBinLabel(bin, std::to_string(bin_label), 1);
//    cppfEmulBX->setBinLabel(bin, std::to_string(bin_label), 1);
//  }

	//from Gabriel'code
  Matches_unpacker     =    ibooker.book1D("Matches_unpacker", "CPPFDigis_Matches_unpacker" , 5, 0. , 5.);
  
  Matches_unpacker_ch  =    ibooker.book1D("Matches_unpacker_ch", "CPPFDigis_Matches_unpacker_ch" , 5, 0. , 5.);
  chamber_emu_unpacker =    ibooker.book2D("chamber_emu_unpacker","chamber_emu_unpacker", 36, 1.,37.,36,1.,37.);
  phi_emu_unpacker     =    ibooker.book2D("phi_emu_unpacker", "phi_emu_unpacker",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker   =    ibooker.book2D("theta_emu_unpacker", "theta_emu_unpacker",  32, 0., 32., 32, 0., 32.);
  bx_emu_unpacker      =    ibooker.book2D("bx_emu_unpacker","bx_emu_unpacker", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_unpacker_bx  =    ibooker.book1D("Matches_unpacker_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  phi_emu_unpacker_bx  =    ibooker.book2D("phi_emu_unpacker_bx", "phi_emu_unpacker_bx",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker_bx=    ibooker.book2D("theta_emu_unpacker_bx", "theta_emu_unpacker_bx",  32, 0., 32., 32, 0., 32.);
  bx_emu_unpacker_bx   =    ibooker.book2D("bx_emu_unpacker_bx","bx_emu_unpacker_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_unpacker_int =    ibooker.book1D("Matches_unpacker_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  phi_emu_unpacker_int =    ibooker.book2D("phi_emu_unpacker_int", "phi_emu_unpacker_int",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker_int   =ibooker.book2D("theta_emu_unpacker_int", "theta_emu_unpacker_int",  32, 0., 32., 32, 0., 32.);
  occupancy_unpacker   =    ibooker.book2D("occupancy_unpacker", "CPPFDigis occupancy_unpacker", 36, 0.5, 36.5, 12, 0.5,12.5); 
  occupancy_emu_unpacker   =ibooker.book2D("occupancy_emu_unpacker", "CPPFDigis occupancy_emu_unpacker", 36, 0.5, 36.5, 12, 0.5,12.5); 
  bx_occupancy_unpacker    =ibooker.book2D("bx_occupancy_unpacker","CPPFDigis Bx_Occupancy_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_occupancy_emu_unpacker=ibooker.book2D("bx_occupancy_emu_unpacker","CPPFDigis Bx_Occupancy_emu_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_sector_unpacker      = ibooker.book2D("bx_sector_unpacker","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  bx_sector_emu_unpacker  = ibooker.book2D("bx_sector_emu_unpacker","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);

	//from Cheng libo
	const std::array<std::string, 10> suffix_name{{"12", "22", "32", "33", "42", "43"}};
	const std::array<std::string, 10> suffix_label{{"1/2", "2/2", "3/2", "3/3", "4/2", "4/3"}};

	for(int i = 0;i<6;i++){
		theta_shift[i]    =ibooker.book2D("theta_shift"    +suffix_name[i], "Theta shift SR_"             +suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);
		phi_shift[i]      =ibooker.book2D("phi_shift"      +suffix_name[i], "Phi shift SR_"               +suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);
  	theta_phi_shift[i]=ibooker.book2D("theta_phi_shift"+suffix_name[i], "Both Theta and Phi shift SR_"+suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);
		not_agree[i]      =ibooker.book2D("not_agree"      +suffix_name[i], "Totally not agree SR_"       +suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);
		well_agree[i]     =ibooker.book2D("well_agree"     +suffix_name[i], "Well agree SR_"              +suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);
		almost_agree[i]   =ibooker.book2D("almost_agree"   +suffix_name[i], "Almost agree SR_"            +suffix_label[i], 36, 0.5, 36.5, 3, 0.5, 3.5);

		theta_shift[i]->setAxisTitle("Section",1);
	  phi_shift[i]->setAxisTitle("Section",1);
		theta_phi_shift[i]->setAxisTitle("Section",1);
		not_agree[i]->setAxisTitle("Section",1);
		well_agree[i]->setAxisTitle("Section",1);
		almost_agree[i]->setAxisTitle("Section",1);
		
		theta_shift[i]->setAxisTitle("Roll",2);
	  phi_shift[i]->setAxisTitle("Roll",2);
		theta_phi_shift[i]->setAxisTitle("Roll",2);
		not_agree[i]->setAxisTitle("Roll",2);
		well_agree[i]->setAxisTitle("Roll",2);
		almost_agree[i]->setAxisTitle("Roll",2);
	}

}

void L1TdeStage2CPPF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TdeStage2CPPF") << "L1TdeStage2CPPF: analyze..." << std::endl;

  edm::Handle<l1t::CPPFDigiCollection> dataCPPFs;
  e.getByToken(dataToken, dataCPPFs);

  edm::Handle<l1t::CPPFDigiCollection> emulCPPFs;
  e.getByToken(emulToken, emulCPPFs);


//  for(auto& cppf_digi : *dataCPPFs){
//	  cppfDataBX->Fill(cppf_digi.bx());
//  }
//  for(auto& cppf_digi : *emulCPPFs){
//	  cppfEmulBX->Fill(cppf_digi.bx());
//  }

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
  int matches_unpacker_int = 0;
//  int matches_emtf = 0;
//  int matches_emtf_bx = 0;
//  int matches_emtf_int = 0;

	//from Gabriel's code
  for(auto& cppf_digis : *emulCPPFs){

	  RPCDetId rpcId = cppf_digis.rpcId();
	  int region = rpcId.region();
	  int station = rpcId.station();
	  int ring = rpcId.ring();
	  int sector = rpcId.sector();
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

	  //Chamber ID
	  int nsub_e = 6;
	  (ring == 1 && station > 1) ? nsub_e = 3 : nsub_e = 6;
	  int chamberID = subsector + nsub_e * ( sector - 1);


	  matches_unpacker = 0;  
	  matches_unpacker_bx = 0;
	  matches_unpacker_int = 0;
	  for(auto& cppf_digis_unpacker : *dataCPPFs){

		  RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
		  int region_unpacker = rpcId_unpacker.region();
		  int station_unpacker = rpcId_unpacker.station();
		  int ring_unpacker = rpcId_unpacker.ring();
		  int sector_unpacker = rpcId_unpacker.sector();
		  int subsector_unpacker = rpcId_unpacker.subsector();
		  int phi_int_unpacker = cppf_digis_unpacker.phi_int();
		  int theta_int_unpacker =  cppf_digis_unpacker.theta_int();
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
			  Matches_unpacker_ch->Fill(matches_unpacker);

			  chamber_emu_unpacker->Fill(chamberID, chamberID_unpacker); 

			  phi_emu_unpacker->Fill(phi_int, phi_int_unpacker);
			  theta_emu_unpacker->Fill(theta_int, theta_int_unpacker);
			  bx_emu_unpacker->Fill(cppf_digis.bx(),bx_unpacker);

			  if(cppf_digis.bx() ==  bx_unpacker){
				  matches_unpacker_bx++;
				  Matches_unpacker_bx->Fill(matches_unpacker_bx);	
				  phi_emu_unpacker_bx->Fill(phi_int, phi_int_unpacker);
				  theta_emu_unpacker_bx->Fill(theta_int, theta_int_unpacker); 
				  bx_emu_unpacker_bx->Fill(cppf_digis.bx(),bx_unpacker);

				  if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
					  matches_unpacker_int++;
					  Matches_unpacker_int->Fill(matches_unpacker_int);	
					  phi_emu_unpacker_int->Fill(phi_int, phi_int_unpacker);
					  theta_emu_unpacker_int->Fill(theta_int, theta_int_unpacker); 
					  //Occupancy 
					  occupancy_unpacker->Fill(EMTF_subsector_u, fill_occupancy_unpacker);
					  //occupancy_unpacker->Fill(EMTF_subsector, fill_occupancy);
					  occupancy_emu_unpacker->Fill(EMTF_subsector, fill_occupancy);
					  //bx occupancy 
					  bx_occupancy_unpacker->Fill(EMTF_bx, fill_occupancy_unpacker);
					  bx_occupancy_emu_unpacker->Fill(EMTF_bx, fill_occupancy);
					  //bx
					  bx_sector_unpacker->Fill(fill_bx_unpacker,EMTF_bx);
					  bx_sector_emu_unpacker->Fill(fill_bx,EMTF_bx);
				  } 
			  }

		  }

	  }
	  //Matching information
	  Matches_unpacker->Fill(matches_unpacker); 
  }//loop 1st is over


  //from Cheng libo
  if(dataCPPFs->size()==0){
		for(auto& cppf_digis : *emulCPPFs){
			RPCDetId rpcId = cppf_digis.rpcId();
			int station = rpcId.station();
			int ring = rpcId.ring();
			int roll = rpcId.roll();
			int sector = rpcId.sector();
			int subsector = rpcId.subsector();
			// "station ring"
			//{{"12", "22", "32", "33", "42", "43"}}
			//    0     1     2     3     4     5
			//f = station-1 + ring-2 +i/4
			int order=station-1+ring-2+station/4;
      int secnum=(sector-1)*6+subsector;
			if(order<0 ||order>5) std::cout<<"deStage2CPPF: wrong order  "<<order<<" "<<station<<" "<<ring<<std::endl;
			if(secnum<0 ||secnum>36) std::cout<<"deStage2CPPF: wrong secnum  "<<secnum<<std::endl;
			not_agree[order]->Fill((rpcId.sector()-1)*6+rpcId.subsector(),roll);
		}
	}
  for(auto& cppf_digis : *emulCPPFs){
		RPCDetId rpcId = cppf_digis.rpcId();
		int region = rpcId.region();
		int station = rpcId.station();
		int ring = rpcId.ring();
		int sector = rpcId.sector();
		int subsector = rpcId.subsector();
		int phi_int = cppf_digis.phi_int();
		int theta_int =  cppf_digis.theta_int();
		
		for(auto& cppf_digis_unpacker : *dataCPPFs){
			RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
			int region_unpacker = rpcId_unpacker.region();
			int station_unpacker = rpcId_unpacker.station();
			int ring_unpacker = rpcId_unpacker.ring();
			int sector_unpacker = rpcId_unpacker.sector();
			int subsector_unpacker = rpcId_unpacker.subsector();
			int phi_int_unpacker = cppf_digis_unpacker.phi_int();
			int theta_int_unpacker =  cppf_digis_unpacker.theta_int();

			int order=station-1+ring-2+station/4;
			int secnum=(sector-1)*6+subsector;
			if(order<0 ||order>5) std::cout<<"deStage2CPPF: wrong order  "<<order<<" "<<station<<" "<<ring<<std::endl;
			if(secnum<0 ||secnum>36) std::cout<<"deStage2CPPF: wrong secnum  "<<secnum<<std::endl;
			if((region == region_unpacker) &&
						(station == station_unpacker) &&
						(ring == ring_unpacker) &&
						(sector == sector_unpacker) &&
						(subsector == subsector_unpacker)){
        if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
					well_agree[order]->Fill(secnum,rpcId.roll());
					almost_agree[order]->Fill(secnum,rpcId.roll());
				}else if((theta_int == theta_int_unpacker) && (abs(phi_int - phi_int_unpacker)<3)){
					phi_shift[order]->Fill(secnum,rpcId.roll());
					almost_agree[order]->Fill(secnum,rpcId.roll());
				}else{
					//if(ihit==(nhits-1)) in python
					not_agree[order]->Fill(secnum,rpcId.roll());
				}
			}else if((region == region_unpacker) &&
						(station == station_unpacker) &&
						(ring == ring_unpacker) &&
						((sector+1) == sector_unpacker) &&
						(subsector == subsector_unpacker)){
				if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
					well_agree[order]->Fill(secnum,rpcId.roll());
					almost_agree[order]->Fill(secnum,rpcId.roll());
				}else if((theta_int == theta_int_unpacker) && (abs(phi_int - phi_int_unpacker)<3)){
					phi_shift[order]->Fill(secnum,rpcId.roll());
					almost_agree[order]->Fill(secnum,rpcId.roll());
				}else{
					//if(ihit==(nhits-1))
					not_agree[order]->Fill(secnum,rpcId.roll());
				}
			}else{
				//if(ihit==(nhits-1))
				not_agree[order]->Fill(secnum,rpcId.roll());
		  }//if is over
		}//loop emulator
	}//loop data

}

int L1TdeStage2CPPF::occupancy_value(int region_, int station_, int ring_){
  
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

int L1TdeStage2CPPF::bx_value(int region_, int emtfsector_){
  
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

