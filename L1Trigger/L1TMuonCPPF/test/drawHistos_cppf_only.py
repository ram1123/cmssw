#!/usr/bin/python
import ROOT
from ROOT import gSystem, TFile, gROOT, gStyle, TGaxis
from ROOT import TCanvas, TH1, TColor
from array import array
import os

gROOT.SetBatch()
TGaxis.SetMaxDigits(3)
gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat("3.1f");
  
NRGBs = 5;
NCont = 255;
stops = [ 0.00, 0.34, 0.61, 0.84, 1.00 ]
red   = [ 0.00, 0.00, 0.87, 1.00, 0.51 ]
green = [ 0.00, 0.81, 1.00, 0.20, 0.00 ]
blue  = [ 0.51, 1.00, 0.12, 0.00, 0.00 ]

s = array('d', stops)
r = array('d', red)
g = array('d', green)
b = array('d', blue)

TColor.CreateGradientColorTable(NRGBs, s, r, g, b, NCont);
gStyle.SetNumberContours(NCont);

#f = TFile.Open('DQM_CPPF.root','read')
f = TFile.Open('DQM_CPPF-Final15Jan.root','read')
c1 = TCanvas('c1', '',1100,1000)

h = []
hOneHit = []
hbx_theta = []
hbx_Offtheta = []
h2d = []
h2dOneHit = []
h2d_bx_theta = []
h2d_bx_Offtheta = []
hdir = "DQM_CPPF"

switch_off_linear_plots = 0
switch_off_log_plots = 1

os.system("rm -rf plots/2D_hist")
os.system("rm -rf plots/1D_hist")
os.system("mkdir -p plots/1D_hist")
os.system("mkdir -p plots/2D_hist")
os.system("cp index.php plots/1D_hist/")
os.system("cp index.php plots/2D_hist/")
os.system("cp index.php plots/")
#Declaration of the list of the different setting of the histograms

# hnames pattern ["hist_name as defined in input root file", "x-axis title", "y-axis title"]
hnames     = [ 
               #["h1_matches_unpacker", "Total matched #hits", "", "Basic Cuts"],
               #["h1_matches_unpacker_bx", "Total matched #hits", "", "Basic Cuts && Same BX"], 
               #["h1_matches_unpacker_bx_phi", "Total matched #hits", "", "Basic Cuts && Same BX && Same #phi"],
               #["h1_matches_unpacker_bx_Offphi", "Total matched #hits", "", "Basic Cuts && Same BX && Off axis #phi"],
               ["h1_cluster_sizeCe", "Emulator cluster size", "", "Basic Cuts"], 
               ["h1_cluster_sizeCe_bx", "Emulator cluster size", "", "Basic Cuts && Same BX"], 
               ["h1_cluster_sizeCe_bx_phi", "Emulator Cluster size", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_cluster_sizeCe_bx_Offphi", "Emulator cluster size", "", "Basic Cuts && Same BX && Off axis #phi"],
               ["h1_cluster_sizeCu", "Unpacker cluster size", "", "Basic Cuts"], 
               ["h1_cluster_sizeCu_bx", "Unpacker cluster size", "", "Basic Cuts && Same BX"], 
               ["h1_cluster_sizeCu_bx_phi", "Unpacker Cluster size", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_cluster_sizeCu_bx_Offphi", "Unpacker cluster size", "", "Basic Cuts && Same BX && Off axis #phi"],
               ["h1_bxCe", "Emulator Bunch Crossing",  "", "Basic Cuts"], 
               ["h1_bxCe_bx", "Emulator Bunch Crossing", "", "Basic Cuts && Same BX"],
               ["h1_bxCe_bx_phi", "Emulator Bunch Crossing", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_bxCe_bx_Offphi", "Emulator Bunch Crossing", "", "Basic Cuts && Same BX && Off axis #phi"],
               ["h1_bxCu", "Unpacker Bunch Crossing",  "", "Basic Cuts"], 
               ["h1_bxCu_bx", "Unpacker Bunch Crossing", "", "Basic Cuts && Same BX"],
               ["h1_bxCu_bx_phi", "Unpacker Bunch Crossing", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_bxCu_bx_Offphi", "Unpacker Bunch Crossing", "", "Basic Cuts && Same BX && Off axis #phi"],
               ["h1_phiIntCe", "Emulator Phi Int", "", "Basic Cuts"], 
               ["h1_phiIntCe_bx","Emulator Phi Int", "", "Basic Cuts && Same BX"], 
               ["h1_phiIntCe_bx_phi", "Emulator Phi Int", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_phiIntCe_bx_Offphi", "Emulator Phi Int", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_phiIntCu", "Unpacker Phi Int", "", "Basic Cuts"], 
               ["h1_phiIntCu_bx","Unpacker Phi Int", "", "Basic Cuts && Same BX"], 
               ["h1_phiIntCu_bx_phi", "Unpacker Phi Int", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_phiIntCu_bx_Offphi", "Unpacker Phi Int", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_thetaIntCe", "Emulator Theta Int", "", "Basic Cuts"], 
               ["h1_thetaIntCe_bx","Emulator Theta Int", "", "Basic Cuts && Same BX"], 
               ["h1_thetaIntCe_bx_phi", "Emulator Theta Int", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_thetaIntCe_bx_Offphi", "Emulator Theta Int", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_thetaIntCu", "Unpacker Theta Int", "", "Basic Cuts"], 
               ["h1_thetaIntCu_bx","Unpacker Theta Int", "", "Basic Cuts && Same BX"], 
               ["h1_thetaIntCu_bx_phi", "Unpacker Theta Int", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_thetaIntCu_bx_Offphi", "Unpacker Theta Int", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_phiGlobalCe", "Emulator Phi Global", "", "Basic Cuts"], 
               ["h1_phiGlobalCe_bx","Emulator Phi Global", "", "Basic Cuts && Same BX"], 
               ["h1_phiGlobalCe_bx_phi", "Emulator Phi Global", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_phiGlobalCe_bx_Offphi", "Emulator Phi Global", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_phiGlobalCu", "Unpacker Phi Global", "", "Basic Cuts"], 
               ["h1_phiGlobalCu_bx","Unpacker Phi Global", "", "Basic Cuts && Same BX"], 
               ["h1_phiGlobalCu_bx_phi", "Unpacker Phi Global", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_phiGlobalCu_bx_Offphi", "Unpacker Phi Global", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_thetaGlobalCe", "Emulator Theta Global", "", "Basic Cuts"], 
               ["h1_thetaGlobalCe_bx","Emulator Theta Global", "", "Basic Cuts && Same BX"], 
               ["h1_thetaGlobalCe_bx_phi", "Emulator Theta Global", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_thetaGlobalCe_bx_Offphi", "Emulator Theta Global", "", "Basic Cuts && Same BX && Off axis #phi"], 
               ["h1_thetaGlobalCu", "Unpacker Theta Global", "", "Basic Cuts"], 
               ["h1_thetaGlobalCu_bx","Unpacker Theta Global", "", "Basic Cuts && Same BX"], 
               ["h1_thetaGlobalCu_bx_phi", "Unpacker Theta Global", "", "Basic Cuts && Same BX && Same #phi"], 
               ["h1_thetaGlobalCu_bx_Offphi", "Unpacker Theta Global", "", "Basic Cuts && Same BX && Off axis #phi"], 
             ]

h2dnames = [
            ["h2_bx_emu_unpack", "Basic Cuts",
               "Bunch crossing (emulator)", "Bunch crossing (unpacker)"],
            ["h2_bx_emu_unpack_bx", "Basic Cuts && Same Bx",
               "Bunch crossing (emulator)", "Bunch crossing (unpacker)"],
            ["h2_bx_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "Bunch crossing (emulator)", "Bunch crossing (unpacker)"], 
            ["h2_bx_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Bunch crossing (emulator)", "Bunch crossing (unpacker)"], 
            ["h2_phiInt_emu_unpack", "Basic Cuts",
               "Phi (Int) emulator", "Phi (Int) unpacker"],
            ["h2_phiInt_emu_unpack_bx", "Basic Cuts && Same Bx",
               "Phi (Int) emulator", "Phi (Int) unpacker"],
            ["h2_phiInt_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "Phi (Int) emulator", "Phi (Int) unpacker"],
            ["h2_phiInt_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Phi (Int) emulator", "Phi (Int) unpacker"],
            ["h2_phiGlobal_emu_unpack", "Basic Cuts",
               "Phi (Global) emulator", "Phi (Global) unpacker"],
            ["h2_phiGlobal_emu_unpack_bx", "Basic Cuts && Same Bx",
               "Phi (Global) emulator", "Phi (Global) unpacker"],
            ["h2_phiGlobal_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "Phi (Global) emulator", "Phi (Global) unpacker"],
            ["h2_phiGlobal_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Phi (Global) emulator", "Phi (Global) unpacker"],
            ["h2_thetaInt_emu_unpack", "Basic Cuts",
               "theta (Int) emulator", "theta (Int) unpacker"],
            ["h2_thetaInt_emu_unpack_bx", "Basic Cuts && Same Bx",
               "theta (Int) emulator", "theta (Int) unpacker"],
            ["h2_thetaInt_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "theta (Int) emulator", "theta (Int) unpacker"],
            ["h2_thetaInt_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "theta (Int) emulator", "theta (Int) unpacker"],
            ["h2_thetaGlobal_emu_unpack", "Basic Cuts",
               "theta (Global) emulator", "theta (Global) unpacker"],
            ["h2_thetaGlobal_emu_unpack_bx", "Basic Cuts && Same Bx",
               "theta (Global) emulator", "theta (Global) unpacker"],
            ["h2_thetaGlobal_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "theta (Global) emulator", "theta (Global) unpacker"],
            ["h2_thetaGlobal_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "theta (Global) emulator", "theta (Global) unpacker"],
            ["h2_chamberID_emu_unpack", "Basic Cuts",
               "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["h2_chamberID_emu_unpack_bx", "Basic Cuts && Same Bx",
               "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["h2_chamberID_emu_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi", 
               "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["h2_chamberID_emu_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["h2_chamberID_vs_roll_emu", "Basic Cuts",
               "Number of Sub-Stations (emulator)", "Roll (emulator)"],
            ["h2_chamberID_vs_roll_emu_bx", "Basic Cuts && Same Bx",
               "Number of Sub-Stations (emulator)", "Roll (emulator)"],
            ["h2_chamberID_vs_roll_emu_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "Number of Sub-Stations (emulator)", "Roll (emulator)"],
            ["h2_chamberID_vs_roll_emu_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Number of Sub-Stations (emulator)", "Roll (emulator)"],
            ["h2_chamberID_vs_roll_unpack", "Basic Cuts",
               "Number of Sub-Stations (unpacker)", "Roll (unpacker)"],
            ["h2_chamberID_vs_roll_unpack_bx", "Basic Cuts && Same Bx",
               "Number of Sub-Stations (unpacker)", "Roll (unpacker)"],
            ["h2_chamberID_vs_roll_unpack_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "Number of Sub-Stations (unpacker)", "Roll (unpacker)"],
            ["h2_chamberID_vs_roll_unpack_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "Number of Sub-Stations (unpacker)", "Roll (unpacker)"],
            ["h2_occupancy_unpacker", "Basic Cuts", 
               "EMTF sub-sector (unpacker)", "Occupancy (unpacker)"],
            ["h2_occupancy_unpacker_bx", "Basic Cuts && Same Bx",
               "EMTF sub-sector (unpacker)", "Occupancy (unpacker)"],
            ["h2_occupancy_unpacker_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "EMTF sub-sector (unpacker)", "Occupancy (unpacker)"],
            ["h2_occupancy_unpacker_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "EMTF sub-sector (unpacker)", "Occupancy (unpacker)"],
            ["h2_occupancy_emulator", "Basic Cuts", 
               "EMTF sub-sector (emulator)", "Occupancy (emulator)"],
            ["h2_occupancy_emulator_bx", "Basic Cuts && Same Bx",
               "EMTF sub-sector (emulator)", "Occupancy (emulator)"],
            ["h2_occupancy_emulator_bx_phi", "Basic Cuts && Same Bx && Same #phi",
               "EMTF sub-sector (emulator)", "Occupancy (emulator)"],
            ["h2_occupancy_emulator_bx_Offphi", "Basic Cuts && Same BX && Off axis #phi",
               "EMTF sub-sector (emulator)", "Occupancy (emulator)"],
            #
           ]

RatioHistNames = [
                    ["h2_occupancy_unpacker_bx_phi", "h2_occupancy_emulator_bx_phi"],
                    ["h2_occupancy_unpacker_bx_Offphi", "h2_occupancy_emulator_bx_Offphi"],
                    ["h2_occupancy_unpacker_bx", "h2_occupancy_emulator_bx"],
                    ["h2_chamberID_vs_roll_emu_bx_phi", "h2_chamberID_vs_roll_emu_bx_Offphi"],
                    ["h2_occupancy_unpacker_bx_phi", "h2_occupancy_unpacker_bx_Offphi"],
                    ["h2_occupancy_emulator_bx_phi", "h2_occupancy_emulator_bx_Offphi"],
                    ["h2_chamberID_vs_roll_emu_bx_oneHit_OnPhi", "h2_chamberID_vs_roll_emu_bx_oneHit_OffPhi"],
                    ["h2_occupancy_unpacker_bx_oneHit_OnPhi", "h2_occupancy_unpacker_bx_oneHit_OffPhi"],
                    ["h2_occupancy_emulator_bx_oneHit_OnPhi", "h2_occupancy_emulator_bx_oneHit_OffPhi"]
                 ]

axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

Table_Histo_Details = []
Table_Histo_Details_Th2 = []

def SaveHistos(histName, hist_xaxis, hist_yaxis, hist_title, AdditionalTitleCut = " && TEST CUT"):
  print hdir+"/"+histName
  hOneHit = f.Get(hdir+"/"+histName)
  print type(hOneHit)
  hOneHit.SetLineWidth(6)
  hOneHit.SetTitle(str(hist_title+AdditionalTitleCut))
  hOneHit.GetXaxis().SetTitle(str(hist_xaxis))
  hOneHit.GetYaxis().SetTitle(str(hist_yaxis))
  hOneHit.GetXaxis().SetTitleSize(0.05)
  hOneHit.GetYaxis().SetTitleSize(0.05)
  hOneHit.GetXaxis().SetTitleOffset(0.9)
  hOneHit.GetYaxis().SetTitleOffset(0.9)
  hOneHit.Draw()
  if switch_off_linear_plots: c1.SaveAs("plots/1D_hist/"+histName+".png")
  c1.SetLogy(1)
  c1.SaveAs("plots/1D_hist/"+histName+"_Log.png")
  c1.SetLogy(0)
  TH1_histo_details = []
  TH1_histo_details.append(hnames[i][0])
  TH1_histo_details.append("TH1F")
  TH1_histo_details.append(hnames[i][1])
  TH1_histo_details.append(hnames[i][2])
  TH1_histo_details.append(hnames[i][3]+AdditionalTitleCut)
  #TH1_histo_details.append(hOneHit.Integral())
  TH1_histo_details.append(hOneHit.GetEntries())
  return TH1_histo_details

def Save2DHistos(histName, hist_xaxis, hist_yaxis, hist_title, AdditionalTitleCut = " && TEST CUT"):
  print "****=> histName = ",hdir+"/"+histName
  h2d = f.Get(hdir+"/"+histName)
  h2d.SetTitle(hist_title+AdditionalTitleCut)
  h2d.GetXaxis().SetTitle(str(hist_xaxis))
  if ("roll" in histName): h2d.GetXaxis().CenterLabels(1)
  h2d.GetYaxis().SetTitle(str(hist_yaxis))
  h2d.GetXaxis().SetTitleSize(0.05)
  h2d.GetYaxis().SetTitleSize(0.05)
  h2d.GetXaxis().SetTitleOffset(0.9)
  h2d.GetYaxis().SetTitleOffset(0.9)
  #h2d.GetZaxis().SetRangeUser(0,5000);
  #if str(h2dnames[i][2]).contains("Occupancy") or str(h2dnames[i][3]).contains("Occupancy"): 
  if "Occupancy" in str(hist_yaxis):
    for j in range(0,12):
      h2d.GetYaxis().SetBinLabel(j+1,axislabels[j])
      h2d.GetYaxis().SetTitleOffset(1.1)
      h2d.GetZaxis().SetRangeUser(0,999);
  h2d.Draw("COLZ")
  c1.SaveAs("plots/2D_hist/"+histName+".png")
  TH2_histo_details = []
  TH2_histo_details.append(histName)
  TH2_histo_details.append("TH2F")
  TH2_histo_details.append(hist_xaxis)
  TH2_histo_details.append(hist_yaxis)
  TH2_histo_details.append(hist_title+AdditionalTitleCut)
  TH2_histo_details.append(h2d.Integral())
  return TH2_histo_details

#Drawing 1D histograms
for i in range(0,len(hnames)):
  print "===> Search hist : ",hdir+"/"+hnames[i][0]
  TH1_histo_details = SaveHistos(hnames[i][0],  hnames[i][1], hnames[i][2], hnames[i][3], "")
  Table_Histo_Details.append(TH1_histo_details)
  TH1_histo_details = SaveHistos(hnames[i][0]+"_oneHit", hnames[i][1], hnames[i][2], hnames[i][3], " && One Hit")
  Table_Histo_Details.append(TH1_histo_details)
  if (hnames[i][0]).endswith("_bx"):
     print "\n\n\tEndswith = ",hnames[i][0],"\n\n"
     TH1_histo_details = SaveHistos(hnames[i][0]+"_twoHit", hnames[i][1], hnames[i][2], hnames[i][3], " && Two Hit")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_oneHit_OnPhi", hnames[i][1], hnames[i][2], hnames[i][3], " && One Hit && OnPhi")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_oneHit_OffPhi", hnames[i][1], hnames[i][2], hnames[i][3], " && One Hit && OffPhi")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_oneHit_OnTheta", hnames[i][1], hnames[i][2], hnames[i][3], " && One Hit && OnTheta")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_oneHit_OffTheta", hnames[i][1], hnames[i][2], hnames[i][3], " && One Hit && OffTheta")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_twoHit_OnPhi", hnames[i][1], hnames[i][2], hnames[i][3], " && Two Hit && OnPhi")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_twoHit_OffPhi", hnames[i][1], hnames[i][2], hnames[i][3], " && Two Hit && OffPhi")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_twoHit_OnTheta", hnames[i][1], hnames[i][2], hnames[i][3], " && Two Hit && OnTheta")
     Table_Histo_Details.append(TH1_histo_details)
     TH1_histo_details = SaveHistos(hnames[i][0]+"_twoHit_OffTheta", hnames[i][1], hnames[i][2], hnames[i][3], " && Two Hit && OffTheta")
     Table_Histo_Details.append(TH1_histo_details)
  #
  # thetaCe == thetaCu
  #
  if ("bx_phi" in hnames[i][0]):
     print (hnames[i][0]).replace("bx_phi","bx_theta")
     print hdir+"/"+(hnames[i][0]).replace("bx_phi","bx_theta")
     TH1_histo_details = SaveHistos((hnames[i][0]).replace("bx_phi","bx_theta"), hnames[i][1], hnames[i][2], (hnames[i][3]).replace("phi","theta"), "")
     Table_Histo_Details.append(TH1_histo_details)
  else: hbx_theta.append("")
  #
  # thetaCe != thetaCu
  #
  if ("bx_Offphi" in hnames[i][0]):
     print (hnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     print hdir+"/"+(hnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     TH1_histo_details = SaveHistos((hnames[i][0]).replace("bx_Offphi","bx_Offtheta"), hnames[i][1], hnames[i][2], (hnames[i][3]).replace("phi","theta"), "")
     Table_Histo_Details.append(TH1_histo_details)
     Table_Histo_Details.append(TH1_histo_details)
  else: hbx_Offtheta.append("")

  
#Drawing 2D histograms
c1.SetRightMargin(0.15)
for i in range(0,len(h2dnames)):
  #Save2DHistos(histName, hist_xaxis, hist_yaxis, hist_title, AdditionalTitleCut = " && TEST CUT"):
  TH2_histo_details = Save2DHistos(h2dnames[i][0], h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], "")
  Table_Histo_Details_Th2.append(TH2_histo_details)
  #
  # One hit histograms
  #
  TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_oneHit", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && One Hit")
  Table_Histo_Details_Th2.append(TH2_histo_details)
  if (hnames[i][0]).endswith("_bx"):
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_twoHit", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && Two Hit")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_oneHit_OnPhi", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && One Hit && OnPhi")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_oneHit_OffPhi", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && One Hit && OffPhi")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_oneHit_OnTheta", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && One Hit && OnTheta")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_oneHit_OffTheta", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && One Hit && OffTheta")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_twoHit_OnPhi", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && Two Hit && OnPhi")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_twoHit_OffPhi", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && Two Hit && OffPhi")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_twoHit_OnTheta", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && Two Hit && OnTheta")
     Table_Histo_Details_Th2.append(TH2_histo_details)
     TH2_histo_details = Save2DHistos(h2dnames[i][0]+"_twoHit_OffTheta", h2dnames[i][2], h2dnames[i][3], h2dnames[i][1], " && Two Hit && OffTheta")
     Table_Histo_Details_Th2.append(TH2_histo_details)
  if ("bx_phi" in h2dnames[i][0]):
     print (h2dnames[i][0]).replace("bx_phi","bx_theta")
     print hdir+"/"+(h2dnames[i][0]).replace("bx_phi","bx_theta")
     TH2_histo_details = Save2DHistos((h2dnames[i][0]).replace("bx_phi","bx_theta"), h2dnames[i][2], h2dnames[i][3], (h2dnames[i][1]).replace("phi","theta"), "")
     Table_Histo_Details_Th2.append(TH2_histo_details)
  else: h2d_bx_theta.append("")

  if ("bx_Offphi" in h2dnames[i][0]):
     print (h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     print hdir+"/"+(h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     TH2_histo_details = Save2DHistos((h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta"), h2dnames[i][2], h2dnames[i][3], (h2dnames[i][1]).replace("phi","theta"), "")
     Table_Histo_Details_Th2.append(TH2_histo_details)
  else: h2d_bx_Offtheta.append("")

for i in range(0,len(RatioHistNames)):
   Numerator = f.Get(hdir+"/"+RatioHistNames[i][1])
   Denomenator = f.Get(hdir+"/"+RatioHistNames[i][0])
   print "Entries in ",RatioHistNames[i][1]," = ",Numerator.GetEntries(), "\t", Numerator.Integral()
   print "Entries in ",RatioHistNames[i][0]," = ",Denomenator.GetEntries(), "\t", Denomenator.Integral()
   
   Numerator.Divide(Denomenator)
   Numerator.SetTitle('')
   Numerator.GetXaxis().SetTitle("")
   Numerator.GetYaxis().SetTitle("")
   Numerator.GetXaxis().SetTitleSize(0.05)
   Numerator.GetYaxis().SetTitleSize(0.05)
   Numerator.GetXaxis().SetTitleOffset(0.9)
   Numerator.GetYaxis().SetTitleOffset(0.9)
   Numerator.GetZaxis().SetRangeUser(0,1);
   Numerator.Draw("COLZ TEXT45")
   print "integral = ",Numerator.Integral()
   c1.SaveAs("plots/"+"Ratio_"+RatioHistNames[i][1]+"-"+RatioHistNames[i][0]+".png")
   c1.SaveAs("plots/"+"Ratio_"+RatioHistNames[i][1]+"-"+RatioHistNames[i][0]+".root")

# Markdown table generation
file = open("plots/1D_hist/yield-info.md", 'w')
file.write("|Hist-Name | Hist-type | X-axis | Y-Axis | Title | GetEntries() |\n")
file.write("|---       |---        |---     |---     |---    |---           |\n")
for info in Table_Histo_Details:
   file.write("| "+str(info[0])+str(" | ")+str(info[1])+str(" | ")+str(info[2])+str(" | ")+str(info[3])+str(" | ")+str(info[4])+str(" | ")+str(info[5])+str(" | \n"))
file.close()
os.system("pandoc -t html -o plots/1D_hist/yield-info.html plots/1D_hist/yield-info.md")
os.system("sed -i 's/<table>/<table border=\"1\">/' plots/1D_hist/yield-info.html")

file = open("plots/2D_hist/yield-info_2d.md", 'w')
file.write("|Hist-Name | Hist-type | X-axis | Y-Axis | Title | GetEntries() |\n")
file.write("|---       |---        |---     |---     |---    |---           |\n")
for info in Table_Histo_Details_Th2:
   file.write("| "+str(info[0])+str(" | ")+str(info[1])+str(" | ")+str(info[2])+str(" | ")+str(info[3])+str(" | ")+str(info[4])+str(" | ")+str(info[5])+str(" | \n"))
file.close()
os.system("pandoc -t html -o plots/2D_hist/yield-info_2d.html plots/2D_hist/yield-info_2d.md")
os.system("sed -i 's/<table>/<table border=\"1\">/' plots/2D_hist/yield-info_2d.html")

os.system('git diff > plots/diff.patch')
os.system('echo -e "\n\n============\n== Latest commit summary \n\n" > plots/commit-summary.txt')
os.system("git log -1 --pretty=tformat:' Commit: %h %n Date: %ad %n Relative time: %ar %n Commit Message: %s' >> plots/commit-summary.txt")
os.system('echo -e "\n\n============\n" >> plots/commit-summary.txt')
os.system('git log -1 --format="%H" >> plots/commit-summary.txt')
os.system('echo "https://github.com/ram1123/cmssw/tree/bugfix-CPPFcomparisons8August" >> plots/commit-summary.txt') 

