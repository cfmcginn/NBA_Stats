//=============================================
// Author: Chris McGinn
//
// BBall Plotter
//
//=============================================

#include "BBallHists.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TDatime.h"
#include <iostream>

int makeBBallPlots(const std::string inName, const Int_t year);
void makePerPlot(const std::string outName, const std::string plotName, TH1* pt, TH1* ptPer, TLine* mean, Bool_t isDiff, Bool_t isPurePer);
void TwoFormat(TH1* inHist1_p, TH1* inHist2_p, Bool_t isDiff, Bool_t isPurePer);
void niceTH1(TH1* uglyTH1, float max , float min, float ndivX, float ndivY, Int_t col = 1, Float_t size = 1, Int_t style = 20);
void handsomeTH1(TH1 *a = 0, Int_t col = 1, Float_t size = 1, Int_t markerstyle = 20);
void claverCanvasSaving(TCanvas* c, TString s,TString format = "gif");

int runMakeBBallPlots(const std::string inName)
{
  int rStatus = -1;

  const Int_t nYears = 11;
  const Int_t years[nYears] = {2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015};

  for(int iter = 0; iter < nYears; iter++){
    rStatus = makeBBallPlots(inName, years[iter]);
  }

  return rStatus;
}

int makeBBallPlots(const std::string inName, const Int_t year)
{
  TH1::SetDefaultSumw2();

  std::string outName = inName;
  const std::string cutString = "Hists";
  const std::string repString = "Plots";

  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString.length(), repString);

  TFile* inFile_p = new TFile(inName.c_str(), "READ");
  GetBBallHists(inFile_p, year);

  for(Int_t iter = 0; iter < nTeams; iter++){
    makePerPlot(outName, Form("%s%d_ptForPlot", teams[iter].c_str(), year), ptFor_h[iter], ptForPer_h[iter], meanTotPt_, false, false);
    makePerPlot(outName, Form("%s%d_ptVsPlot", teams[iter].c_str(), year), ptVs_h[iter], ptVsPer_h[iter], meanTotPt_, false, false);
    makePerPlot(outName, Form("%s%d_ptDiffPlot", teams[iter].c_str(), year), ptDiff_h[iter], ptDiffPer_h[iter], meanDiffPt_, true, false);
    makePerPlot(outName, Form("%s%d_ptForVsPERPlot", teams[iter].c_str(), year), ptForPer_h[iter], ptVsPer_h[iter], meanTotPt_, false, true);
  }

  inFile_p->Close();
  delete inFile_p;

  return 0;
}


void makePerPlot(const std::string outName, const std::string plotName, TH1* pt, TH1* ptPer, TLine* mean, Bool_t isDiff, Bool_t isPurePer)
{
  TwoFormat(pt, ptPer, isDiff, isPurePer);

  TCanvas* plotCanvas_p = new TCanvas(Form("%s_c", plotName.c_str()), Form("%s_c", plotName.c_str()), 1*300, 1*350);

  if(isPurePer) pt->DrawCopy("E1");
  else pt->DrawCopy("HIST P");
  ptPer->DrawCopy("E1 SAME");
  mean->DrawLine(-0.5, mean->GetY1(), pt->GetXaxis()->GetXmax(), mean->GetY1());

  TLine* line82_p;
  Int_t seasonLength = 82;
  if(!strcmp("2012", Form("%s", plotName.substr(3, 4).c_str()))) seasonLength = 60;
  if(pt->GetXaxis()->GetXmax() > seasonLength){
    line82_p = new TLine(82, pt->GetMinimum(), 82, pt->GetMaximum());
    line82_p->SetLineStyle(2);
    line82_p->Draw();
  }

  TLegend* leg_p = new TLegend(0.20, 0.85, 0.50, 0.95);
  leg_p->SetFillColor(0);
  leg_p->SetFillStyle(0);
  leg_p->SetTextFont(43);
  leg_p->SetTextSizePixels(18);
  leg_p->SetBorderSize(0);

  if(!isPurePer){
    leg_p->AddEntry(pt, "Pts.", "P L");
    leg_p->AddEntry(ptPer, "Pts./Game", "P L");
  }
  else{
    leg_p->AddEntry(pt, "Pts./Game (For)", "P L");
    leg_p->AddEntry(ptPer, "Pts./Game (Vs.)", "P L");
  }

  leg_p->Draw("SAME");

  TLatex* teamLabel_p = new TLatex();
  teamLabel_p->SetTextFont(43);
  teamLabel_p->SetTextSize(20);

  Int_t yPos = 115;
  Int_t yPos2 = 112;
  if(isDiff){
    yPos = 40;
    yPos2 = 35;
  }
  if(!isDiff && !isPurePer){
    yPos = 140;
    yPos2 = 135;
  }

  teamLabel_p->DrawLatex(60, yPos, Form("%s", plotName.substr(0, 3).c_str()));
  teamLabel_p->DrawLatex(60, yPos2, Form("%s", plotName.substr(3, 4).c_str()));

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");
  plotCanvas_p->Write("", TObject::kOverwrite);
  claverCanvasSaving(plotCanvas_p, Form("pdfDir/%s", plotName.c_str()), "pdf");
  outFile_p->Close();
  delete outFile_p;

  delete plotCanvas_p;

  return;
}

void TwoFormat(TH1* inHist1_p, TH1* inHist2_p, Bool_t isDiff, Bool_t isPurePer)
{
  /*
  Float_t maxVal = inHist1_p->GetBinContent(inHist1_p->GetMaximumBin());
  if(maxVal < inHist2_p->GetBinContent(inHist2_p->GetMaximumBin())) maxVal = inHist2_p->GetBinContent(inHist2_p->GetMaximumBin());

  Float_t minVal = inHist1_p->GetBinContent(inHist1_p->GetMinimumBin());
  if(minVal > inHist2_p->GetBinContent(inHist2_p->GetMinimumBin())) minVal = inHist2_p->GetBinContent(inHist2_p->GetMinimumBin());
  */
  //  maxVal += TMath::Sqrt(maxVal) + 1;
  //  minVal -= TMath::Sqrt(minVal) + 1;
  //  if(minVal < 0) minVal = 0;

  Int_t base = 100;
  if(isDiff) base = 0;
  
  Float_t range = 50;
  if(isPurePer) range = 20;

  niceTH1(inHist1_p, base + range, base - range, 505, 505, kRed+2, 0.5, 20);
  niceTH1(inHist2_p, base + range, base - range, 505, 505, kBlue, 0.5, 21);

  return;
}

void niceTH1(TH1* uglyTH1, float max , float min, float ndivX, float ndivY, Int_t col, Float_t size, Int_t style)
{
  handsomeTH1(uglyTH1, col, size, style);
  uglyTH1->SetMaximum(max);
  uglyTH1->SetMinimum(min);
  uglyTH1->SetNdivisions(ndivX);
  uglyTH1->SetNdivisions(ndivY, "Y");

  return;
}

void handsomeTH1(TH1 *a, Int_t col, Float_t size, Int_t markerstyle)
{
  a->SetMarkerColor(col);
  a->SetMarkerSize(size);
  a->SetMarkerStyle(markerstyle);
  a->SetLineColor(col);
  a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->SetTitleOffset(1.0);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();

  return;
}

void claverCanvasSaving(TCanvas* c, TString s,TString format)
{
  TDatime* date = new TDatime();
  c->SaveAs(Form("%s_%d.%s",s.Data(),date->GetDate(), format.Data()));

  return;
}
