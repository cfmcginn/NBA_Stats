//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// BBall Hists Class                                                    
//                                                                              
//============================================= 

#ifndef BBallHists_h
#define BBallHists_h

#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TLine.h"
#include "TMath.h"
#include "BBallTeamVar.h"

TLine* meanTotPt_;
TLine* meanDiffPt_;

TH1I* ptFor_h[nTeams];
TH1F* ptForPer_h[nTeams];

TH1I* ptVs_h[nTeams];
TH1F* ptVsPer_h[nTeams];

TH1I* ptDiff_h[nTeams];
TH1F* ptDiffPer_h[nTeams];

void BookBBallHists(const Int_t year, Int_t gamesPlayed[])
{
  for(Int_t iter = 0; iter < nTeams; iter++){
    ptFor_h[iter] = new TH1I(Form("ptFor_%s%d_h", teams[iter].c_str(), year), Form("ptFor_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);
    ptForPer_h[iter] = new TH1F(Form("ptForPer_%s%d_h", teams[iter].c_str(), year), Form("ptForPer_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);

    ptVs_h[iter] = new TH1I(Form("ptVs_%s%d_h", teams[iter].c_str(), year), Form("ptVs_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);
    ptVsPer_h[iter] = new TH1F(Form("ptVsPer_%s%d_h", teams[iter].c_str(), year), Form("ptVsPer_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);

    ptDiff_h[iter] = new TH1I(Form("ptDiff_%s%d_h", teams[iter].c_str(), year), Form("ptDiff_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);
    ptDiffPer_h[iter] = new TH1F(Form("ptDiffPer_%s%d_h", teams[iter].c_str(), year), Form("ptDiffPer_%s%d_h", teams[iter].c_str(), year), gamesPlayed[iter], -0.5, Float_t(gamesPlayed[iter]) - 0.5);

    ptFor_h[iter]->SetXTitle("Games");
    ptForPer_h[iter]->SetXTitle("Games");
    ptVs_h[iter]->SetXTitle("Games");
    ptVsPer_h[iter]->SetXTitle("Games");
    ptDiff_h[iter]->SetXTitle("Games");
    ptDiffPer_h[iter]->SetXTitle("Games");

    ptFor_h[iter]->SetYTitle("Points");
    ptForPer_h[iter]->SetYTitle("Points");
    ptVs_h[iter]->SetYTitle("Points");
    ptVsPer_h[iter]->SetYTitle("Points");
    ptDiff_h[iter]->SetYTitle("Points");
    ptDiffPer_h[iter]->SetYTitle("Points");
  }

  return;
}

void FillBBallHists(const Int_t teamNum, const Int_t nGames, Int_t ptFor[], Float_t ptForPer[], Float_t ptForPerErr[], Int_t ptVs[], Float_t ptVsPer[], Float_t ptVsPerErr[], Int_t ptDiff[], Float_t ptDiffErr[], Float_t ptDiffPer[], Float_t ptDiffPerErr[])
{
  for(Int_t iter = 0; iter < nGames; iter++){
    ptFor_h[teamNum]->SetBinContent(iter+1, ptFor[iter]);
    ptFor_h[teamNum]->SetBinError(iter+1, TMath::Sqrt(ptFor[iter]));

    ptForPer_h[teamNum]->SetBinContent(iter+1, ptForPer[iter]);
    ptForPer_h[teamNum]->SetBinError(iter+1, ptForPerErr[iter]);

    ptVs_h[teamNum]->SetBinContent(iter+1, ptVs[iter]);
    ptVs_h[teamNum]->SetBinError(iter+1, TMath::Sqrt(ptVs[iter]));

    ptVsPer_h[teamNum]->SetBinContent(iter+1, ptVsPer[iter]);
    ptVsPer_h[teamNum]->SetBinError(iter+1, ptVsPerErr[iter]);

    ptDiff_h[teamNum]->SetBinContent(iter+1, ptDiff[iter]);
    ptDiff_h[teamNum]->SetBinError(iter+1, ptDiffErr[iter]);

    ptDiffPer_h[teamNum]->SetBinContent(iter+1, ptDiffPer[iter]);
    ptDiffPer_h[teamNum]->SetBinError(iter+1, ptDiffPerErr[iter]);
  }

  ptFor_h[teamNum]->GetXaxis()->SetRange(1, nGames);
  ptForPer_h[teamNum]->GetXaxis()->SetRange(1, nGames);

  ptVs_h[teamNum]->GetXaxis()->SetRange(1, nGames);
  ptVsPer_h[teamNum]->GetXaxis()->SetRange(1, nGames);

  ptDiff_h[teamNum]->GetXaxis()->SetRange(1, nGames);
  ptDiffPer_h[teamNum]->GetXaxis()->SetRange(1, nGames);

  return;
}



void WriteBBallHists(const std::string outName, const Int_t year)
{
  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");

  meanTotPt_->Write(Form("meanTotPt%d_", year), TObject::kOverwrite);
  meanDiffPt_->Write(Form("meanDiffPt%d_", year), TObject::kOverwrite);

  for(Int_t iter = 0; iter < nTeams; iter++){
    ptFor_h[iter]->Write("", TObject::kOverwrite);
    ptForPer_h[iter]->Write("", TObject::kOverwrite);

    ptVs_h[iter]->Write("", TObject::kOverwrite);
    ptVsPer_h[iter]->Write("", TObject::kOverwrite);

    ptDiff_h[iter]->Write("", TObject::kOverwrite);
    ptDiffPer_h[iter]->Write("", TObject::kOverwrite);
  }
  outFile_p->Close();
  delete outFile_p;

  return;
}

void CleanupBBallHists()
{
  delete meanTotPt_;
  delete meanDiffPt_;

  for(Int_t iter = 0; iter < nTeams; iter++){
    delete ptFor_h[iter];
    delete ptForPer_h[iter];

    delete ptVs_h[iter];
    delete ptVsPer_h[iter];

    delete ptDiff_h[iter];
    delete ptDiffPer_h[iter];
  }

  return;
}

void GetBBallHists(TFile* histFile_p, const Int_t year)
{
  meanTotPt_ = (TLine*)histFile_p->Get(Form("meanTotPt%d_", year));
  meanDiffPt_ = (TLine*)histFile_p->Get(Form("meanDiffPt%d_", year));

  for(Int_t iter = 0; iter < nTeams; iter++){
    ptFor_h[iter] = (TH1I*)histFile_p->Get(Form("ptFor_%s%d_h", teams[iter].c_str(), year));
    ptForPer_h[iter] = (TH1F*)histFile_p->Get(Form("ptForPer_%s%d_h", teams[iter].c_str(), year));

    ptVs_h[iter] = (TH1I*)histFile_p->Get(Form("ptVs_%s%d_h", teams[iter].c_str(), year));
    ptVsPer_h[iter] = (TH1F*)histFile_p->Get(Form("ptVsPer_%s%d_h", teams[iter].c_str(), year));

    ptDiff_h[iter] = (TH1I*)histFile_p->Get(Form("ptDiff_%s%d_h", teams[iter].c_str(), year));
    ptDiffPer_h[iter] = (TH1F*)histFile_p->Get(Form("ptDiffPer_%s%d_h", teams[iter].c_str(), year));
  }

  return;
}

#endif
