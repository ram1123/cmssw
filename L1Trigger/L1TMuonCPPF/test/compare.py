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

os.system("rm -rf plots/compare_hist")
os.system("mkdir -p  plots/compare_hist")
#g = f.cd("DQM_CPPF")
keys = f.GetListOfKeys()
key_Table_Histo_Details = []
for k in keys:
   print k.GetName()
   ROOT.gDirectory.cd(k.GetName())
   for key2 in ROOT.gDirectory.GetListOfKeys():
     if ("h1_nHits_each_chamber_" in key2.GetName() and "unpacker" in key2.GetName()):
       if( "sector0" in key2.GetTitle()  or "subsector0" in key2.GetTitle() or "station0" in key2.GetTitle() or "ring0" in key2.GetTitle() or "ring1" in key2.GetTitle() ):
        pass;
       else:
        #print key2.GetName()
        hist1 = f.Get(hdir+"/"+key2.GetName())
        hist1.SetTitle(key2.GetTitle())
        hist1.GetXaxis().SetTitle("#Hits")
        hist1.GetYaxis().SetTitle("")
        hist1.GetXaxis().SetTitleSize(0.05) 
        hist1.GetYaxis().SetTitleSize(0.05)
        hist1.GetXaxis().SetTitleOffset(0.9)
        hist1.GetYaxis().SetTitleOffset(0.9)
        hist1.GetXaxis().SetRangeUser(0,8);
        hist1.SetLineColor(1)
        hist1.SetLineWidth(2)

        hist2 = f.Get((hdir+"/"+key2.GetName()).replace("unpacker","emulator"))
        hist2.SetTitle(key2.GetTitle())
        hist2.GetXaxis().SetTitle("#Hits")
        hist2.GetYaxis().SetTitle("")
        hist2.GetXaxis().SetTitleSize(0.05) 
        hist2.GetYaxis().SetTitleSize(0.05)
        hist2.GetXaxis().SetTitleOffset(0.9)
        hist2.GetYaxis().SetTitleOffset(0.9)
        hist2.GetXaxis().SetRangeUser(0,8);
        hist2.SetLineColor(2)
        hist2.SetLineWidth(2)

        tlegend2 = ROOT.TLegend(0.50, 0.65, 0.88, 0.88)#x1,y1,x2,y2
        tlegend2.SetFillColor(0)
        tlegend2.SetTextFont(42)
        tlegend2.SetTextSize(0.035)
        tlegend2.SetLineColor(0)
        tlegend2.SetShadowColor(0)
        tlegend2.AddEntry(hist1,"unpacker","L")
        tlegend2.AddEntry(hist2,"emulator","L")

        TH1_histo_details = []
        TH1_histo_details.append(key2.GetTitle())
        TH1_histo_details.append("TH1F")
        TH1_histo_details.append(hist1.GetEntries())
       #TH1_histo_details.append(hist1.GetBinContent(0))
        TH1_histo_details.append(hist1.GetBinContent(1))
        TH1_histo_details.append(float(hist1.GetBinContent(2)/hist2.GetBinContent(2))) if hist2.GetBinContent(2) > 0 else TH1_histo_details.append("N/A")
        TH1_histo_details.append(float(hist1.GetBinContent(3)/hist2.GetBinContent(3))) if hist2.GetBinContent(3) > 0 else TH1_histo_details.append("N/A")
        TH1_histo_details.append(float(hist1.GetBinContent(4)/hist2.GetBinContent(4))) if hist2.GetBinContent(4) > 0 else TH1_histo_details.append("N/A")
        TH1_histo_details.append(float(hist1.GetBinContent(5)/hist2.GetBinContent(5))) if hist2.GetBinContent(5) > 0 else TH1_histo_details.append("N/A")
        TH1_histo_details.append(float(hist1.GetBinContent(6)/hist2.GetBinContent(6))) if hist2.GetBinContent(6) > 0 else TH1_histo_details.append("N/A")
        key_Table_Histo_Details.append(TH1_histo_details)
        hist1.Draw()
        hist2.Draw("same")
        tlegend2.Draw()
        c1.SaveAs("plots/compare_hist/"+(str(key2.GetTitle())).replace("unpacker","compare_U_E")+".png")
        c1.SetLogy(1)
        c1.SaveAs("plots/compare_hist/"+(str(key2.GetTitle())).replace("unpacker","compare_U_E")+"Log.png")
        c1.SetLogy(0)

# Markdown table generation
file = open("plots/compare_hist/yield-info.md", 'w')
file.write("|Hist-Name | Hist-type | GetEntries() | Bin1 | Bin2 | Bin3 | Bin4 | Bin5 | Bin6 |\n")
file.write("|---       |---        |---           |---   |---   |---   |---   |---   |---   |\n")
for info in key_Table_Histo_Details:
   file.write("| "+str(info[0])+str(" | ")+str(info[1])+str(" | ")+str(info[2])+str(" | ")+str(info[3])+str(" | ")+str(info[4])+str(" | ")+str(info[5])+str(" | ")+str(info[6])+str(" | ")+str(info[7])+str(" | ")+str(info[8])+str(" | \n"))
file.close()
os.system("pandoc -t html -o plots/compare_hist/yield-info.html plots/compare_hist/yield-info.md")
os.system("sed -i 's/<table>/<table border=\"1\">/' plots/compare_hist/yield-info.html")
