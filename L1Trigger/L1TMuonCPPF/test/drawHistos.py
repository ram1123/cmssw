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

f = TFile.Open('DQM_CPPF_EMTF-2018-200k-BugFixTanggy.root','read')
c1 = TCanvas('c1', '',1100,1000)

h = []
h2d = []
hdir = "EMTF_CPPF_DQM"
#Declaration of the list of the different setting of the histograms

hnames = ["h1CeVsCuMatches",  "h1CeVsCuPhiCePhiCuDiff", 
          "h1CeVsEuPhiCePhiEuDiff", "h1CeVsEePhiCePhiEeDiff", 
          "h1CuVsEuPhiCuPhiEuDiff", "h1EeVsEuPhiEePhiEuDiff",
          "h1CeVsCuBxCeBxCuDiff", "h1CeVsEuBxCeBxEuDiff", 
          "h1CeVsEeBxCeBxEeDiff", "h1CuVsEuBxCuBxEuDiff", 
          "h1EeVsEuBxEeBxEuDiff"]
hxaxisnames =  ["", "\phi_{CPPF emulated} - \phi_{CPPF unpacked}", 
                "\phi_{CPPF emulated} - \phi_{EMTF unpacked}", "\phi_{CPPF emulated} - \phi_{EMTF emulated}", 
                "\phi_{CPPF unpacked} - \phi_{EMTF unpacked}", "\phi_{EMTF emulated} - \phi_{EMTF unpacked}",
                "BX_{CPPF emulated} - BX_{CPPF unpacked}", "BX_{CPPF emulated} - BX_{EMTF unpacked}",
                "BX_{CPPF emulated} - BX_{EMTF emulated}", "BX_{CPPF unpacked} - BX_{EMTF unpacked}",
                "BX_{EMTF emulated} - BX_{EMTF unpacked}"]
hyaxisnames =  ["", "", 
                "", "", 
                "", "",
                "", "", 
                "", "", 
                ""]


h2dnames = [
            "h2CeVsCuChamberCuZoneCu","h2CeVsCuChamberCeZoneCe",
            "h2EeVsEuChamberEuZoneEu","h2EeVsEuChamberEeZoneEe",
            "h2CeVsCuChamberCuZoneCuOneHit", "h2CeVsCuChamberCeZoneCeOneHit",
            "h2CeVsCuChamberCuZoneCuNotOneHit", "h2CeVsCuChamberCeZoneCeNotOneHit",
            "h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalNotOneHit", "h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalNotOneHit",
            "h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalNotOneHit", "h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalNotOneHit",
            "h2CeVsCuBxCuZoneCuOccupancyInPhiDiagonalOneHit", "h2CeVsCuBxCeZoneCeOccupancyInPhiDiagonalOneHit",
            "h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit", "h2CeVsCuBxCeZoneCeOccupancyOffPhiDiagonalOneHit",
            "h2EeVsEuBxEuZoneEuOccupancyOffPhiDiagonalOneHit","h2EeVsEuBxEeZoneEeOccupancyOffPhiDiagonalOneHit",
            "h2CuVsEuChamberCuZoneCu","h2CuVsEuChamberEuZoneEu",
            "h2CeVsEeChamberCeZoneCe","h2CeVsEeChamberEeZoneEe",
            "h2CuVsEuChamberCuZoneCuBox","h2CuVsEuChamberCuChamberEuBox", 
            "h2EeVsEuPhiEePhiEu", "h2CuVsEuPhiCuPhiEu",
            "h2CeVsCuPhiCePhiCu","h2CeVsCuThetaCeThetaCu",
            "h2EeVsEuPhiEePhiEuOneHit", "h2EeVsEuThetaEeThetaEuOneHit",
            "h2CeVsCuPhiCePhiCuOneHit","h2CeVsCuThetaCeThetaCuOneHit",
            "h2EeVsEuPhiEePhiEuOneHit_bx", "h2EeVsEuThetaEeThetaEuOneHit_bx",
            "h2CeVsCuPhiCePhiCuOneHit_bx","h2CeVsCuThetaCeThetaCuOneHit_bx",
            "h2CeVsCuBxCeBxCu", "h2CeVsEuBxCeBxEu", 
            "h2CeVsEeBxCeBxEe", "h2CuVsEuBxCuBxEu", 
            "h2EeVsEuBxEeBxEu"
            ]

h2dxaxisnames = [
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)", 
                 "EMTFSubsector(EMTF unpacked)", "EMTFSubsector(EMTF emulated)", 
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(CPPF emulated)", 
                 "EMTFSubsector(EMTF unpacked)", "EMTFSubsector(EMTF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(EMTF unpacked)", 
                 "EMTFSubsector(CPPF emulated)", "EMTFSubsector(EMTF emulated)",
                 "EMTFSubsector(CPPF unpacked)", "EMTFSubsector(EMTF unpacked)", 
                 "\phi_{EMTF emulated}", "\phi_{CPPF unpacked}",
                 "\phi_{CPPF emulated}","#theta_{CPPF emulated}", 
                 "\phi_{EMTF emulated}", "#theta_{EMTF emulated}",
                 "\phi_{CPPF emulated}","#theta_{CPPF emulated}",
                 "\phi_{EMTF emulated}", "#theta_{EMTF emulated}",
                 "\phi_{CPPF emulated}","#theta_{CPPF emulated}",
                 "BX_{CPPF emulated}", "BX_{CPPF emulated}", 
                 "BX_{CPPF emulated}", "BX_{CPPF unpacked}", 
                 "BX_{EMTF emulated}"
                 ]
h2dyaxisnames = [
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "","",
                 "", "EMTFSubsector(CPPF unpacked)", 
                 "\phi_{EMTF unpacked}", "\phi_{EMTF unpacked}",
                 "\phi_{CPPF unpacked}", "#theta_{CPPF unpacked}",
                 "\phi_{EMTF unpacked}", "#theta_{EMTF unpacked}",
                 "\phi_{CPPF unpacked}","#theta_{CPPF unpacked}",
                 "\phi_{EMTF unpacked}", "#theta_{EMTF unpacked}",
                 "\phi_{CPPF unpacked}","#theta_{CPPF unpacked}",
                 "BX_{CPPF unpacked}", "BX_{EMTF unpacked}", 
                 "BX_{EMTF emulated}", "BX_{EMTF unpacked}", 
                 "BX_{EMTF unpacked}"
                 ]


axislabels = ["RE-4/3", "RE-4/2", "RE-3/3", "RE-3/2", "RE-2/2", "RE-1/2", "RE+1/2", "RE+2/2", "RE+3/2", "RE+3/3", "RE+4/2", "RE+4/3"]

#Drawing 1D histograms
for i in range(0,len(hnames)):
  h.append(f.Get(hdir+"/"+hnames[i]))
  h[i].SetLineWidth(6)
  h[i].SetTitle('')
  h[i].GetXaxis().SetTitle(str(hxaxisnames[i]))
  h[i].GetYaxis().SetTitle(str(hyaxisnames[i]))
  h[i].GetXaxis().SetTitleSize(0.05)
  h[i].GetYaxis().SetTitleSize(0.05)
  h[i].GetXaxis().SetTitleOffset(0.9)
  h[i].GetYaxis().SetTitleOffset(0.9)
  h[i].Draw()
  c1.SaveAs(hnames[i]+".png")
  c1.SetLogy(1)
  c1.SaveAs(hnames[i]+"Log.png")
  c1.SetLogy(0)
  
#Drawing 2D histograms
c1.SetRightMargin(0.15)
for i in range(0,len(h2dnames)):
  h2d.append(f.Get(hdir+"/"+h2dnames[i]))
  h2d[i].SetTitle('')
  h2d[i].GetXaxis().SetTitle(str(h2dxaxisnames[i]))
  h2d[i].GetYaxis().SetTitle(str(h2dyaxisnames[i]))
  h2d[i].GetXaxis().SetTitleSize(0.05)
  h2d[i].GetYaxis().SetTitleSize(0.05)
  h2d[i].GetXaxis().SetTitleOffset(0.9)
  h2d[i].GetYaxis().SetTitleOffset(0.9)
  h2d[i].GetZaxis().SetRangeUser(0,999);
  if i < 24 : 
    for j in range(0,12):
      h2d[i].GetYaxis().SetBinLabel(j+1,axislabels[j]) 
  h2d[i].Draw("COLZ")
  c1.SaveAs(h2dnames[i]+".png")

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

RatioHistNum = f.Get(hdir+"/h2CeVsCuChamberCuZoneCuOneHit")
RatioHistDeno = f.Get(hdir+"/h2CeVsCuChamberCuZoneCu")

print "Entries h2CeVsCuChamberCuZoneCuOneHit = ",RatioHistNum.GetEntries()
print "Entries h2CeVsCuChamberCuZoneCu = ",RatioHistDeno.GetEntries()
print "Entries h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit = ",f.Get(hdir+"/h2CeVsCuBxCuZoneCuOccupancyOffPhiDiagonalOneHit").GetEntries()
RatioHistNum.Divide(RatioHistDeno)
RatioHistNum.SetTitle('')
RatioHistNum.GetXaxis().SetTitle("EMTFSubsector(CPPF unpacked)")
RatioHistNum.GetYaxis().SetTitle("")
RatioHistNum.GetXaxis().SetTitleSize(0.05)
RatioHistNum.GetYaxis().SetTitleSize(0.05)
RatioHistNum.GetXaxis().SetTitleOffset(0.9)
RatioHistNum.GetYaxis().SetTitleOffset(0.9)
RatioHistNum.GetZaxis().SetRangeUser(0,5);
for j in range(0,12):
   RatioHistNum.GetYaxis().SetBinLabel(j+1,axislabels[j]) 
RatioHistNum.Draw("COLZ")
c1.SaveAs("Ratio_OneHit_Inclusive.png")
