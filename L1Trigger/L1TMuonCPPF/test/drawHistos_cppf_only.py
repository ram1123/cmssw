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
h2d = []
hdir = "DQM_CPPF"

#Declaration of the list of the different setting of the histograms

# hnames pattern ["hist_name as defined in input root file", "x-axis title", "y-axis title"]
hnames     = [ 
               ["h1_total_hits_unpacker", "Inclusive total #hits", "", "Basic Cuts"],
               ["h1_total_hits_unpacker_bx", "#hits (same bunch crossing)", "", "Basic Cuts && Same BX"], 
               ["h1_total_hits_unpacker_bx_phi", "#hits (same bunch && same phi)", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_total_hits_emulator", "Inclusive total #hits", "", "Basic Cuts"],
               ["h1_total_hits_emulator_bx", "#hits (same bunch crossing)", "", "Basic Cuts && Same BX"], 
               ["h1_total_hits_emulator_bx_phi", "#hits (same bunch && same phi)", "", "Basic Cuts && Same BX && Same #phi"],
               ["h1_bx_emulated", "Emulated Bunch crossing", "", "Basic Cuts"],
               ["h1_bx_unpacker", "Unpacker Bunch crossing", "", "Basic Cuts"],
               ["h1_bx_diff_emu_unpacker", "BX_{emulator} - BX_{unpacker}", "", "Basic Cuts"],
               ["h1_phi_diff_emu_unpacker", "#phi_{emulator}-#phi_{unpacker}", "", "Basic Cuts"],

               ["h1CeVsCuPhiCePhiCuDiff_OneHit", "#phi_{emulator}-#phi_{unpacker}", "", "Basic Cuts && One hit"],
               ["h1CeVsCuPhiCePhiCuDiff_OneHit_bx", "#phi_{emulator}-#phi_{unpacker}", "", "Basic Cuts && Same Bx && One hit"],
               ["h1CeVsCuThetaCeThetaCuDiff_OneHit", "#theta_{emulator}-#theta_{unpacker}", "", "Basic Cuts && One hit"],
               ["h1CeVsCuThetaCeThetaCuDiff_OneHit_bx", "#theta_{emulator}-#theta_{unpacker}", "", "Basic Cuts && Same Bx && One hit"],
               ["h1CeVsCuBxCeBxCuDiff_OneHit", "BX_{emulator} - BX_{unpacker}", "", "Basic Cuts && One hit"],
               ["h1CeVsCuPhi_InDiagonal_OneHit", " ", "", "Basic Cut && One hit && Same Phi"],
               ["h1CeVsCuPhi_InDiagonal_OneHit_bx", " ", "", "Basic Cut && One hit && Same Phi && Same BX"],
               ["h1CeVsCuPhi_OffDiagonal_OneHit", "", "", "Basic Cut && One hit && !(Same Phi)"],
               ["h1CeVsCuPhi_OffDiagonal_OneHit_bx", "", "", "Basic Cut && One hit && !(Same Phi) && Same BX"],
               ["h1CeVsCuTheta_InDiagonal_OneHit", "", "", "Basic Cut && One hit && Same Theta"],
               ["h1CeVsCuTheta_InDiagonal_OneHit_bx", "", "", "Basic Cut && One hit && Same Theta && Same BX"],
               ["h1CeVsCuTheta_OffDiagonal_OneHit", "", "", "Basic Cut && One hit && !(Same Theta)"],
               ["h1CeVsCuTheta_OffDiagonal_OneHit_bx", "", "", "Basic Cut && One hit && !(Same Theta) && Same BX"],
               ["h1CeVsCuPhi_InDiagonal_NotOneHit_bx", "", "", "Basic Cut && (> 1 hit) && Same Phi && Same BX"],
               ["h1CeVsCuPhi_OffDiagonal_NotOneHit_bx", "", "", "Basic Cut && (> 1 hit) && !(Same Phi) && Same BX"],
               ["h1CeVsCuThetaPhiCeThetaPhiCuDiff_OneHit_bx", "#phi_{emulator}-#phi_{unpacker}", "", "Basic Cut && (#theta_{emulator} == #theta_{unpacker}) && (#phi_{emulator} != #phi_{unpacker}) && One Hit && Same BX"],
             ]

h2dnames = [
            #["h2_bx_occupancy_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
            #  "EMTF Bunch crossing (Emulator)", "Number of Sub-Stations unpacker"],
            #["h2_bx_occupancy_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
            #  "EMTF Bunch crossing (Emulator)", "Number of Sub-Stations emulator"],
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
            #["h2_bx_sector_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
            #  "EMTF sector (unpacker)", "EMTF Bunch crossing (Emulator)"],
            #["h2_bx_sector_emu_unpacker_bx_phi", "Basic Cuts && Same Bx && Same Phi", 
            #  "EMTF sector (emulator)", "EMTF Bunch crossing (Emulator)"],
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
            
            # One hit histograms
            ["h2CeVsCuChamberCuChamberCe_OneHit", "Basic Cuts && One hit",
              "Chamber ID (Unpacker)", "Chamber ID (Emulator)"],
            ["h2CeVsCuPhiCePhiCu_OneHit", "Basic Cuts && One hit",
              "#phi_{unpacker}", "#phi_{emulator}"],
            ["h2CeVsCuPhiCePhiCu_OneHit_bx", "Basic Cuts && One hit && Same BX",
              "#phi_{unpacker}", "#phi_{emulator}"], 
            ["h2CeVsCuThetaCeThetaCu_OneHit", "Basic Cuts && One hit",
              "#theta_{unpacker}", "#theta_{emulator}"],
            ["h2CeVsCuThetaCeThetaCu_OneHit_bx", "Basic Cuts && One hit && Same BX",
              "#theta_{unpacker}", "#theta_{emulator}"],
            ["h2CeVsCuChamberCuZoneCu_OneHit_bx", "Basic Cuts && One hit && Same BX",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuChamberCeZoneCe_OneHit_bx", "Basic Cuts && One hit && Same BX",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancy_OneHit", "Basic Cuts && One hit",
              "Bunch crossing unpacker",  "Number of Sub-Stations unpacker"],
            ["h2CeVsCuBxCeZoneCeOccupancy_OneHit", "Basic Cuts && One hit", 
              "Bunch crossing emulator",  "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCu_OneHit", "Basic Cuts && One hit",
              "RPC sub-sectors unpacker", "Bunch crossing unpacker"],
            ["h2CeVsCuBxCeZoneCe_OneHit", "Basic Cuts && One hit", 
              "RPC sub-sectors emulator", "Bunch crossing emulator"],
            ["h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx", "Basic Cuts && One hit && #phi_e==#phi_u && Same BX",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx", "Basic Cuts && One hit && #phi_e==#phi_u && Same BX",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx", "Basic Cuts && One hit && #phi_e!=#phi_u && Same BX",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx", "Basic Cuts && One hit && #phi_e!=#phi_u && Same BX",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx", "Basic Cuts && One hit && #theta_e!=#theta_u && Same BX",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx", "Basic Cuts && One hit && #theta_e!=#theta_u && Same BX",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuChamberCeZoneCe_NotOneHit", "Basic Cuts && (> 1 hit) ",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuChamberCuZoneCu_NotOneHit", "Basic Cuts && (> 1 hit)",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit", "Basic Cuts && (> 1 hit) && #phi_e==#phi_u",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit", "Basic Cuts && (> 1 hit) && #phi_e==#phi_u",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
            ["h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit", "Basic Cuts && (> 1 hit) && #phi_e!=#phi_u",
              "RPC sub-sectors emulator", "Number of Sub-Stations emulator"],
            ["h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit", "Basic Cuts && (> 1 hit) && #phi_e!=#phi_u",
              "RPC sub-sectors unpacker", "Number of Sub-Stations unpacker"],
           ]

RatioHistNames = [
                  #["h2_bx_occupancy_unpacker_bx_phi", "h2_bx_occupancy_emu_unpacker_bx_phi"],
                  ["h2_occupancy_unpacker_bx_phi", "h2_occupancy_emu_unpacker_bx_phi"],
                  #["h2_bx_sector_unpacker_bx_phi", "h2_bx_sector_emu_unpacker_bx_phi"],
                  ["h2CeVsCuChamberCuZoneCu_OneHit_bx", "h2CeVsCuChamberCeZoneCe_OneHit_bx"],
                  ["h2CeVsCuBxCuZoneCuOccupancy_OneHit", "h2CeVsCuBxCeZoneCeOccupancy_OneHit"],
                  ["h2CeVsCuBxCuZoneCu_OneHit", "h2CeVsCuBxCeZoneCe_OneHit"],
                  ["h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_OneHit_bx", "h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_OneHit_bx"],
                  ["h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_OneHit_bx", "h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_OneHit_bx"],
                  ["h2CeVsCuBxCeZoneCeOccupancyOffThetaDiagonal_OneHit_bx", "h2CeVsCuBxCuZoneCuOccupancyOffThetaDiagonal_OneHit_bx"],
                  ["h2CeVsCuChamberCuZoneCu_NotOneHit", "h2CeVsCuChamberCeZoneCe_NotOneHit"],
                  ["h2CeVsCuBxCeZoneCeOccupancy_InPhiDiagonal_NotOneHit", "h2CeVsCuBxCuZoneCuOccupancy_InPhiDiagonal_NotOneHit"],
                  ["h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonal_NotOneHit", "h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonal_NotOneHit"],
                 ]

axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

Table_Histo_Details = []

#Drawing 1D histograms
for i in range(0,len(hnames)):
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
  c1.SaveAs("plots/"+hnames[i][0]+".png")
  #c1.SaveAs("plots/"+hnames[i][0]+".txt")
  #print h[i]
  #h[i].Print("all")
  #print "===\n",output
  #file = open("plots/"+hnames[i][0]+".txt", 'w')
  #file.write(h[i].Print("all"))
  #file.write(output)
  #file.close()
  c1.SaveAs("plots/"+hnames[i][0]+".root")
  c1.SetLogy(1)
  c1.SaveAs("plots/"+hnames[i][0]+"Log.png")
  #c1.SaveAs("plots/"+hnames[i][0]+"Log.root")
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
  c1.SaveAs("plots/"+h2dnames[i][0]+".png")
  c1.SaveAs("plots/"+h2dnames[i][0]+".root")
  TH1_histo_details = []
  TH1_histo_details.append(h2dnames[i][0])
  TH1_histo_details.append("TH2F")
  TH1_histo_details.append(h2dnames[i][2])
  TH1_histo_details.append(h2dnames[i][3])
  TH1_histo_details.append(h2dnames[i][1])
  TH1_histo_details.append(h2d[i].Integral())
  Table_Histo_Details.append(TH1_histo_details)

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
file = open("plots/yield-info.md", 'w')
file.write("|Hist-Name | Hist-type | X-axis | Y-Axis | Title | GetEntries() |\n")
file.write("|---       |---        |---     |---     |---    |---           |\n")
for info in Table_Histo_Details:
   file.write("| "+str(info[0])+str(" | ")+str(info[1])+str(" | ")+str(info[2])+str(" | ")+str(info[3])+str(" | ")+str(info[4])+str(" | ")+str(info[5])+str(" | \n"))
file.close()
os.system("pandoc -t html -o plots/yield-info.html plots/yield-info.md")
os.system("sed -i 's/<table>/<table border=\"1\">/' plots/yield-info.html")

