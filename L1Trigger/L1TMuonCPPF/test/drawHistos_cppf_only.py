#!/usr/bin/python
import ROOT
from ROOT import gSystem, TFile, gROOT, gStyle, TGaxis
from ROOT import TCanvas, TH1, TColor
from array import array
import os

gROOT.SetBatch()
TGaxis.SetMaxDigits(3)
gStyle.SetOptStat(0)
  
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

f = TFile.Open('DQM_CPPF.root','read')
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

                 ]

axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

Table_Histo_Details = []
Table_Histo_Details_Th2 = []

#Drawing 1D histograms
for i in range(0,len(hnames)):
  print "===> Search hist : ",hdir+"/"+hnames[i][0]
  h.append(f.Get(hdir+"/"+hnames[i][0]))
  h[i].SetLineWidth(6)
  h[i].SetTitle(str(hnames[i][3]))
  h[i].GetXaxis().SetTitle(str(hnames[i][1]))
  h[i].GetYaxis().SetTitle(str(hnames[i][2]))
  h[i].GetXaxis().SetTitleSize(0.05)
  h[i].GetYaxis().SetTitleSize(0.05)
  h[i].GetXaxis().SetTitleOffset(0.9)
  h[i].GetYaxis().SetTitleOffset(0.9)
  h[i].Draw()
  c1.SaveAs("plots/1D_hist/"+hnames[i][0]+".png")
  c1.SetLogy(1)
  c1.SaveAs("plots/1D_hist/"+hnames[i][0]+"Log.png")
  c1.SetLogy(0)
  TH1_histo_details = []
  TH1_histo_details.append(hnames[i][0])
  TH1_histo_details.append("TH1F")
  TH1_histo_details.append(hnames[i][1])
  TH1_histo_details.append(hnames[i][2])
  TH1_histo_details.append(hnames[i][3])
  #TH1_histo_details.append(h[i].Integral())
  TH1_histo_details.append(h[i].GetEntries())
  Table_Histo_Details.append(TH1_histo_details)
  #
  # One hit histograms
  #
  hOneHit.append(f.Get(hdir+"/"+hnames[i][0]+"_oneHit"))
  hOneHit[i].SetLineWidth(6)
  hOneHit[i].SetTitle(str(hnames[i][3]+" && One Hit"))
  hOneHit[i].GetXaxis().SetTitle(str(hnames[i][1]))
  hOneHit[i].GetYaxis().SetTitle(str(hnames[i][2]))
  hOneHit[i].GetXaxis().SetTitleSize(0.05)
  hOneHit[i].GetYaxis().SetTitleSize(0.05)
  hOneHit[i].GetXaxis().SetTitleOffset(0.9)
  hOneHit[i].GetYaxis().SetTitleOffset(0.9)
  hOneHit[i].Draw()
  c1.SaveAs("plots/1D_hist/"+hnames[i][0]+"_oneHit.png")
  c1.SetLogy(1)
  c1.SaveAs("plots/1D_hist/"+hnames[i][0]+"_oneHit_Log.png")
  c1.SetLogy(0)
  TH1_histo_details = []
  TH1_histo_details.append(hnames[i][0]+"_oneHit")
  TH1_histo_details.append("TH1F")
  TH1_histo_details.append(hnames[i][1])
  TH1_histo_details.append(hnames[i][2])
  TH1_histo_details.append(hnames[i][3]+" && One Hit")
  #TH1_histo_details.append(hOneHit[i].Integral())
  TH1_histo_details.append(hOneHit[i].GetEntries())
  Table_Histo_Details.append(TH1_histo_details)
  #
  # thetaCe == thetaCu
  #
  if ("bx_phi" in hnames[i][0]):
     print (hnames[i][0]).replace("bx_phi","bx_theta")
     print hdir+"/"+(hnames[i][0]).replace("bx_phi","bx_theta")
     hbx_theta.append(f.Get(hdir+"/"+(hnames[i][0]).replace("bx_phi","bx_theta")))
     hbx_theta[i].SetLineWidth(6)
     hbx_theta[i].SetTitle(str((hnames[i][3]).replace("phi","theta")))
     hbx_theta[i].GetXaxis().SetTitle(str(hnames[i][1]))
     hbx_theta[i].GetYaxis().SetTitle(str(hnames[i][2]))
     hbx_theta[i].GetXaxis().SetTitleSize(0.05)
     hbx_theta[i].GetYaxis().SetTitleSize(0.05)
     hbx_theta[i].GetXaxis().SetTitleOffset(0.9)
     hbx_theta[i].GetYaxis().SetTitleOffset(0.9)
     hbx_theta[i].Draw()
     c1.SaveAs("plots/1D_hist/"+(hnames[i][0]).replace("bx_phi","bx_theta")+".png")
     c1.SetLogy(1)
     c1.SaveAs("plots/1D_hist/"+(hnames[i][0]).replace("bx_phi","bx_theta")+"_Log.png")
     c1.SetLogy(0)
     TH1_histo_details = []
     TH1_histo_details.append((hnames[i][0]).replace("bx_phi","bx_theta"))
     TH1_histo_details.append("TH1F")
     TH1_histo_details.append(hnames[i][1])
     TH1_histo_details.append(hnames[i][2])
     TH1_histo_details.append((hnames[i][3]).replace("phi","theta"))
     #TH1_histo_details.append(hbx_theta[i].Integral())
     TH1_histo_details.append(hbx_theta[i].GetEntries())
     Table_Histo_Details.append(TH1_histo_details)
  else: hbx_theta.append("")
  #
  # thetaCe != thetaCu
  #
  if ("bx_Offphi" in hnames[i][0]):
     print (hnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     print hdir+"/"+(hnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     hbx_Offtheta.append(f.Get(hdir+"/"+(hnames[i][0]).replace("bx_Offphi","bx_Offtheta")))
     hbx_Offtheta[i].SetLineWidth(6)
     hbx_Offtheta[i].SetTitle(str((hnames[i][3]).replace("phi","theta")))
     hbx_Offtheta[i].GetXaxis().SetTitle(str(hnames[i][1]))
     hbx_Offtheta[i].GetYaxis().SetTitle(str(hnames[i][2]))
     hbx_Offtheta[i].GetXaxis().SetTitleSize(0.05)
     hbx_Offtheta[i].GetYaxis().SetTitleSize(0.05)
     hbx_Offtheta[i].GetXaxis().SetTitleOffset(0.9)
     hbx_Offtheta[i].GetYaxis().SetTitleOffset(0.9)
     hbx_Offtheta[i].Draw()
     c1.SaveAs("plots/1D_hist/"+(hnames[i][0]).replace("bx_Offphi","bx_Offtheta")+".png")
     c1.SetLogy(1)
     c1.SaveAs("plots/1D_hist/"+(hnames[i][0]).replace("bx_Offphi","bx_Offtheta")+"_Log.png")
     c1.SetLogy(0)
     TH1_histo_details = []
     TH1_histo_details.append((hnames[i][0]).replace("bx_Offphi","bx_Offtheta"))
     TH1_histo_details.append("TH1F")
     TH1_histo_details.append(hnames[i][1])
     TH1_histo_details.append(hnames[i][2])
     TH1_histo_details.append((hnames[i][3]).replace("phi","theta"))
     #TH1_histo_details.append(hbx_Offtheta[i].Integral())
     TH1_histo_details.append(hbx_Offtheta[i].GetEntries())
     Table_Histo_Details.append(TH1_histo_details)
  else: hbx_Offtheta.append("")

  
#Drawing 2D histograms
c1.SetRightMargin(0.15)
for i in range(0,len(h2dnames)):
  h2d.append(f.Get(hdir+"/"+h2dnames[i][0]))
  h2d[i].SetTitle(h2dnames[i][1])
  h2d[i].GetXaxis().SetTitle(str(h2dnames[i][2]))
  if ("roll" in h2dnames[i][0]): h2d[i].GetXaxis().CenterLabels(1)
  h2d[i].GetYaxis().SetTitle(str(h2dnames[i][3]))
  h2d[i].GetXaxis().SetTitleSize(0.05)
  h2d[i].GetYaxis().SetTitleSize(0.05)
  h2d[i].GetXaxis().SetTitleOffset(0.9)
  h2d[i].GetYaxis().SetTitleOffset(0.9)
  h2d[i].GetZaxis().SetRangeUser(0,999);
  #if i < 2 : 
  #  for j in range(0,12):
  #    h2d[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
  h2d[i].Draw("COLZ")
  c1.SaveAs("plots/2D_hist/"+h2dnames[i][0]+".png")
  TH2_histo_details = []
  TH2_histo_details.append(h2dnames[i][0])
  TH2_histo_details.append("TH2F")
  TH2_histo_details.append(h2dnames[i][2])
  TH2_histo_details.append(h2dnames[i][3])
  TH2_histo_details.append(h2dnames[i][1])
  TH2_histo_details.append(h2d[i].Integral())
  Table_Histo_Details_Th2.append(TH2_histo_details)
  #
  # One hit histograms
  #
  h2dOneHit.append(f.Get(hdir+"/"+h2dnames[i][0]+"_oneHit"))
  h2dOneHit[i].SetTitle(h2dnames[i][1]+" && One Hit")
  h2dOneHit[i].GetXaxis().SetTitle(str(h2dnames[i][2]))
  h2dOneHit[i].GetYaxis().SetTitle(str(h2dnames[i][3]))
  h2dOneHit[i].GetXaxis().SetTitleSize(0.05)
  h2dOneHit[i].GetYaxis().SetTitleSize(0.05)
  h2dOneHit[i].GetXaxis().SetTitleOffset(0.9)
  h2dOneHit[i].GetYaxis().SetTitleOffset(0.9)
  h2dOneHit[i].GetZaxis().SetRangeUser(0,999);
  #if i < 2 : 
  #  for j in range(0,12):
  #    h2dOneHit[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
  h2dOneHit[i].Draw("COLZ")
  c1.SaveAs("plots/2D_hist/"+h2dnames[i][0]+"_oneHit.png")
  TH2_histo_details = []
  TH2_histo_details.append(h2dnames[i][0]+"_oneHit")
  TH2_histo_details.append("TH2F")
  TH2_histo_details.append(h2dnames[i][2])
  TH2_histo_details.append(h2dnames[i][3])
  TH2_histo_details.append(h2dnames[i][1]+" && One Hit")
  TH2_histo_details.append(h2dOneHit[i].Integral())
  Table_Histo_Details_Th2.append(TH2_histo_details)

  if ("bx_phi" in hnames[i][0]):
     print (h2dnames[i][0]).replace("bx_phi","bx_theta")
     print hdir+"/"+(h2dnames[i][0]).replace("bx_phi","bx_theta")
     h2d_bx_theta.append(f.Get(hdir+"/"+(h2dnames[i][0]).replace("bx_phi","bx_theta")))
     h2d_bx_theta[i].SetTitle((h2dnames[i][1]).replace("phi","theta"))
     h2d_bx_theta[i].GetXaxis().SetTitle(str(h2dnames[i][2]))
     h2d_bx_theta[i].GetYaxis().SetTitle(str(h2dnames[i][3]))
     h2d_bx_theta[i].GetXaxis().SetTitleSize(0.05)
     h2d_bx_theta[i].GetYaxis().SetTitleSize(0.05)
     h2d_bx_theta[i].GetXaxis().SetTitleOffset(0.9)
     h2d_bx_theta[i].GetYaxis().SetTitleOffset(0.9)
     h2d_bx_theta[i].GetZaxis().SetRangeUser(0,999);
     #if i < 2 : 
     #  for j in range(0,12):
     #    h2d_bx_theta[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
     h2d_bx_theta[i].Draw("COLZ")
     c1.SaveAs("plots/2D_hist/"+(h2dnames[i][0]).replace("bx_phi","bx_theta")+".png")
     TH2_histo_details = []
     TH2_histo_details.append((h2dnames[i][0]).replace("bx_phi","bx_theta"))
     TH2_histo_details.append("TH2F")
     TH2_histo_details.append(h2dnames[i][2])
     TH2_histo_details.append(h2dnames[i][3])
     TH2_histo_details.append((h2dnames[i][1]).replace("phi","theta"))
     TH2_histo_details.append(h2d_bx_theta[i].Integral())
     Table_Histo_Details_Th2.append(TH2_histo_details)
  else: h2d_bx_theta.append("")

  if ("bx_Offphi" in hnames[i][0]):
     print (h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     print hdir+"/"+(h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")
     h2d_bx_Offtheta.append(f.Get(hdir+"/"+(h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")))
     h2d_bx_Offtheta[i].SetTitle((h2dnames[i][1]).replace("phi","theta"))
     h2d_bx_Offtheta[i].GetXaxis().SetTitle(str(h2dnames[i][2]))
     h2d_bx_Offtheta[i].GetYaxis().SetTitle(str(h2dnames[i][3]))
     h2d_bx_Offtheta[i].GetXaxis().SetTitleSize(0.05)
     h2d_bx_Offtheta[i].GetYaxis().SetTitleSize(0.05)
     h2d_bx_Offtheta[i].GetXaxis().SetTitleOffset(0.9)
     h2d_bx_Offtheta[i].GetYaxis().SetTitleOffset(0.9)
     h2d_bx_Offtheta[i].GetZaxis().SetRangeUser(0,999);
     #if i < 2 : 
     #  for j in range(0,12):
     #    h2d_bx_Offtheta[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
     h2d_bx_Offtheta[i].Draw("COLZ")
     c1.SaveAs("plots/2D_hist/"+(h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta")+".png")
     TH2_histo_details = []
     TH2_histo_details.append((h2dnames[i][0]).replace("bx_Offphi","bx_Offtheta"))
     TH2_histo_details.append("TH2F")
     TH2_histo_details.append(h2dnames[i][2])
     TH2_histo_details.append(h2dnames[i][3])
     TH2_histo_details.append((h2dnames[i][1]).replace("phi","theta"))
     TH2_histo_details.append(h2d_bx_Offtheta[i].Integral())
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
   Numerator.GetZaxis().SetRangeUser(0,5);
   Numerator.Draw("COLZ")
   print "integral = ",Numerator.Integral()
   c1.SaveAs("plots/"+"Ratio_"+RatioHistNames[i][1]+"-"+RatioHistNames[i][0]+".png")

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

