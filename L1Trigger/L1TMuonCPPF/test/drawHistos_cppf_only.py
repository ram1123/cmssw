#!/usr/bin/python
import ROOT
from ROOT import gSystem, TFile, gROOT, gStyle, TGaxis
from ROOT import TCanvas, TH1, TColor
from array import array

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
h2d = []
hdir = "DQM_CPPF"
#Declaration of the list of the different setting of the histograms

# hnames pattern ["hist_name as defined in input root file", "x-axis title", "y-axis title"]
hnames     = [ 
               ["h1_matches_unpacker", "Inclusive total #hits", ""],
               ["h1_matches_unpacker_bx", "#hits (same bunch crossing)", ""], 
               ["h1_matches_unpacker_bx_phi", "#hits (same bunch && same phi)", ""],
               ["h1_bx_emulated", "Emulated Bunch crossing", ""],
               ["h1_bx_unpacker", "Unpacker Bunch crossing", ""],
               ["h1_bx_diff_emu_unpacker", "BX_{emulator} - BX_{unpacker}", ""],
               ["h1_phi_diff_emu_unpacker", "#phi_{emulator}-#phi_{unpacker}", ""],
             ]

h2dnames = [
            ["h2_bx_occupancy_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "EMTF Bunch crossing (Emulator)", "Number of Sub-Stations unpacker"],
            ["h2_bx_occupancy_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "EMTF Bunch crossing (Emulator)", "Number of Sub-Stations emulator"],
            ["h2_occupancy_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2_occupancy_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2_chamber_emu_unpacker", "Basic Cuts", 
              "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["h2_chamber_emu_unpacker_REm43", "Basic Cuts", 
              "Number of Sub-Sectors of RE-43 (emulator)", "Number of Sub-Sectors  of RE-43 (unpacker)"],
            ["h2_chamber_emu_unpacker_REm42", "Basic Cuts", 
              "Number of Sub-Sectors of RE-42 (emulator)", "Number of Sub-Sectors  of RE-42 (unpacker)"],
            ["h2_chamber_emu_unpacker_REm33", "Basic Cuts", 
              "Number of Sub-Sectors of RE-33 (emulator)", "Number of Sub-Sectors  of RE-33 (unpacker)"],
            ["h2_chamber_emu_unpacker_REm32", "Basic Cuts", 
              "Number of Sub-Sectors of RE-32 (emulator)", "Number of Sub-Sectors  of RE-32 (unpacker)"],
            ["h2_chamber_emu_unpacker_REm22", "Basic Cuts", 
              "Number of Sub-Sectors of RE-22 (emulator)", "Number of Sub-Sectors  of RE-22 (unpacker)"],
            ["h2_chamber_emu_unpacker_REm12", "Basic Cuts", 
              "Number of Sub-Sectors of RE-12 (emulator)", "Number of Sub-Sectors  of RE-12 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp12", "Basic Cuts", 
              "Number of Sub-Sectors of RE+12 (emulator)", "Number of Sub-Sectors  of RE+12 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp22", "Basic Cuts", 
              "Number of Sub-Sectors of RE+22 (emulator)", "Number of Sub-Sectors  of RE+22 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp32", "Basic Cuts", 
              "Number of Sub-Sectors of RE+32 (emulator)", "Number of Sub-Sectors  of RE+32 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp33", "Basic Cuts", 
              "Number of Sub-Sectors of RE+33 (emulator)", "Number of Sub-Sectors  of RE+33 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp42", "Basic Cuts", 
              "Number of Sub-Sectors of RE+42 (emulator)", "Number of Sub-Sectors  of RE+42 (unpacker)"],
            ["h2_chamber_emu_unpacker_REp43", "Basic Cuts", 
              "Number of Sub-Sectors of RE+43 (emulator)", "Number of Sub-Sectors  of RE+43 (unpacker)"],
            ["h2_bx_sector_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "EMTF sector (unpacker)", "EMTF Bunch crossing (Emulator)"],
            ["h2_bx_sector_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "EMTF sector (emulator)", "EMTF Bunch crossing (Emulator)"],
            ["h2_phi_emu_unpacker", "Basic Cuts", 
              "#phi emulator", "#phi unpacker"],
            ["h2_phi_emu_unpacker_bx", "Basic Cuts && Same Bx", 
              "#phi emulator", "#phi unpacker"],
            ["h2_phi_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "#phi emulator", "#phi unpacker"],
            ["h2_theta_emu_unpacker", "Basic Cuts", 
              "#theta emulator", "#theta unpacker"],
            ["h2_theta_emu_unpacker_bx", "Basic Cuts && Same Bx", 
              "#theta emulator", "#theta unpacker"],
            ["h2_theta_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
              "#theta emulator", "#theta unpacker"],
            ["h2_bx_emu_unpacker", "Basic Cuts", 
              "Bunch crossing emulator", "Bunch crossing unpacker"],
            ["h2_bx_emu_unpacker_bx", "Basic Cuts && Same Bx", 
              "Bunch crossing emulator", "Bunch crossing unpacker"],
           ]

RatioHistNames = [
                  ["h2_bx_occupancy_unpacker_bx_phi", "h2_bx_occupancy_emu_unpacker_bx_phi"],
                  ["h2_occupancy_unpacker_bx_phi", "h2_occupancy_emu_unpacker_bx_phi"],
                  ["h2_bx_sector_unpacker_bx_phi", "h2_bx_sector_emu_unpacker_bx_phi"],
                 ]

axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

#Drawing 1D histograms
for i in range(0,len(hnames)):
  h.append(f.Get(hdir+"/"+hnames[i][0]))
  h[i].SetLineWidth(6)
  h[i].SetTitle('')
  h[i].GetXaxis().SetTitle(str(hnames[i][1]))
  h[i].GetYaxis().SetTitle(str(hnames[i][2]))
  h[i].GetXaxis().SetTitleSize(0.05)
  h[i].GetYaxis().SetTitleSize(0.05)
  h[i].GetXaxis().SetTitleOffset(0.9)
  h[i].GetYaxis().SetTitleOffset(0.9)
  h[i].Draw()
  c1.SaveAs(hnames[i][0]+".png")
  c1.SetLogy(1)
  c1.SaveAs(hnames[i][0]+"Log.png")
  c1.SetLogy(0)
  
#Drawing 2D histograms
c1.SetRightMargin(0.15)
for i in range(0,len(h2dnames)):
  h2d.append(f.Get(hdir+"/"+h2dnames[i][0]))
  h2d[i].SetTitle(h2dnames[i][1])
  h2d[i].GetXaxis().SetTitle(str(h2dnames[i][2]))
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
  c1.SaveAs(h2dnames[i][0]+".png")

# for i in [0,2,4,6]:
#   h2d1 = f.Get(hdir+"/"+h2dnames[i])
#   h2d2 = f.Get(hdir+"/"+h2dnames[i+1])
#   h2d1.Sumw2()
#   h2d2.Sumw2()
#   h2d1.Add(h2d2,-1.00000001)
#   h2d1.SetTitle('')
#   h2d1.GetXaxis().SetTitle("EMTFSubsector")
#   h2d1.GetYaxis().SetTitle(str(h2dyaxisnames[i]))
#   h2d1.GetXaxis().SetTitleSize(0.05)
#   h2d1.GetYaxis().SetTitleSize(0.05)
#   h2d1.GetXaxis().SetTitleOffset(0.9)
#   h2d1.GetYaxis().SetTitleOffset(0.9)
#   if True: 
#     for j in range(0,12):
#       h2d1.GetYaxis().SetBinLabel(j+1,axislabels[j]) 
#   h2d1.Draw("COLZ")
#   c1.SaveAs(h2dnames[i]+"Diff.png")

#RatioHistNum = f.Get(hdir+"/h2CeVsCuChamberCuZoneCuOneHit")
#RatioHistDeno = f.Get(hdir+"/h2CeVsCuChamberCuZoneCu")
#
#print "Entries h2CeVsCuChamberCuZoneCuOneHit = ",RatioHistNum.GetEntries()
#print "Entries h2CeVsCuChamberCuZoneCu = ",RatioHistDeno.GetEntries()
#print "Entries h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit = ",f.Get(hdir+"/h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit").GetEntries()
#RatioHistNum.Divide(RatioHistDeno)
#RatioHistNum.SetTitle('')
#RatioHistNum.GetXaxis().SetTitle("EMTFSubsector(CPPF unpacked)")
#RatioHistNum.GetYaxis().SetTitle("")
#RatioHistNum.GetXaxis().SetTitleSize(0.05)
#RatioHistNum.GetYaxis().SetTitleSize(0.05)
#RatioHistNum.GetXaxis().SetTitleOffset(0.9)
#RatioHistNum.GetYaxis().SetTitleOffset(0.9)
#RatioHistNum.GetZaxis().SetRangeUser(0,5);
#for j in range(0,12):
#   RatioHistNum.GetYaxis().SetBinLabel(j+1,axislabels[j]) 
#RatioHistNum.Draw("COLZ")
#c1.SaveAs("Ratio_OneHit_Inclusive.png")

for i in range(0,len(RatioHistNames)):
   Numerator = f.Get(hdir+"/"+RatioHistNames[i][1])
   Denomenator = f.Get(hdir+"/"+RatioHistNames[i][0])
   print "Entries in ",RatioHistNames[i][1]," = ",Numerator.GetEntries()
   print "Entries in ",RatioHistNames[i][0]," = ",Denomenator.GetEntries()
   
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
   c1.SaveAs("Ratio_"+RatioHistNames[i][1]+"-"+RatioHistNames[i][0]+".png")
