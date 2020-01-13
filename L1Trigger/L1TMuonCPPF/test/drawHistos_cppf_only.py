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
               ["Matches_unpacker", "Inclusive total #hits", ""],
               ["Matches_unpacker_ch", "Inclusive #hits", ""],
               ["Matches_unpacker_bx", "#hits (same bunch crossing)", ""], 
               ["Matches_unpacker_int", "#hits (same bunch && same phi)", ""]
             ]

h2dnames = [
            ["bx_occupancy_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "Bunch crossing", "Number of Sub-Stations unpacker"],
            ["bx_occupancy_emu_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "Bunch crossing", "Number of Sub-Stations emulator"],
            ["occupancy_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["occupancy_emu_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["chamber_emu_unpacker", "Basic Cuts", 
                "Number of Sub-Stations (emulator)", "Number of Sub-Stations (unpacker)"],
            ["bx_sector_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "Number of Sub-Stations (unpacker)", "Emulator Bunch crossing"],
            ["bx_sector_emu_unpacker", "Basic Cuts && Same Bx && Same Phi", 
                "Number of Sub-Stations (emulator)", "Emulator Bunch crossing"],
            ["phi_emu_unpacker", "Basic Cuts", 
                "#phi emulator", "#phi unpacker"],
            ["phi_emu_unpacker_bx", "Basic Cuts && Same Bx", 
                "#phi emulator", "#phi unpacker"],
            ["phi_emu_unpacker_int", "Basic Cuts && Same Bx && Same Phi", 
                "#phi emulator", "#phi unpacker"],
            ["theta_emu_unpacker", "Basic Cuts", 
                "#theta emulator", "#theta unpacker"],
            ["theta_emu_unpacker_bx", "Basic Cuts && Same Bx", 
                "#theta emulator", "#theta unpacker"],
            ["theta_emu_unpacker_int", "Basic Cuts && Same Bx && Same Phi", 
                "#theta emulator", "#theta unpacker"],
            ["bx_emu_unpacker", "Basic Cuts", 
                "Bunch crossing emulator", "Bunch crossing unpacker"],
            ["bx_emu_unpacker_bx", "Basic Cuts && Same Bx", 
                "Bunch crossing emulator", "Bunch crossing unpacker"],
           ]

RatioHistNames = [
                  ["bx_occupancy_unpacker", "bx_occupancy_emu_unpacker"],
                  ["occupancy_unpacker", "occupancy_emu_unpacker"],
                  ["bx_sector_unpacker", "bx_sector_emu_unpacker"],
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
   Numerator = f.Get(hdir+"/"+RatioHistNames[i][0])
   Denomenator = f.Get(hdir+"/"+RatioHistNames[i][1])
   print "Entries in ",RatioHistNames[i][0]," = ",Numerator.GetEntries()
   print "Entries in ",RatioHistNames[i][1]," = ",Denomenator.GetEntries()
   
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
   c1.SaveAs("Ratio_"+RatioHistNames[i][0]+"_"+RatioHistNames[i][1]+".png")
