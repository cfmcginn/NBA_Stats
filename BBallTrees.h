//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// BBall Trees Class                                                    
//                                                                              
//=============================================  

#ifndef BBallTrees_h
#define BBallTrees_h

#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"

#include "BBallTeamVar.h"

TTree* statTree_p = 0;
TTree* bookTree_p = 0;

const Int_t maxGames = 130;
const Int_t maxPeriod = 8;

std::string teamStr_;
Int_t teamNum_;
Int_t year_;

Int_t nGames_;
Int_t month_[maxGames];
Int_t day_[maxGames];
Bool_t isHome_[maxGames];
Int_t teamNumVs_[maxGames];
Long64_t gameID_[maxGames];
Int_t ptFor_[maxGames];
Int_t ptVs_[maxGames];
Int_t ptDiff_[maxGames];
Float_t ptDiffErr_[maxGames];
Int_t win_[maxGames];
Int_t loss_[maxGames];

Float_t ptForPer_[maxGames];
Float_t ptForPerErr_[maxGames];
Float_t ptVsPer_[maxGames];
Float_t ptVsPerErr_[maxGames];
Float_t ptDiffPer_[maxGames];
Float_t ptDiffPerErr_[maxGames];

Int_t nPeriod_[maxGames];
Int_t ptForQuar_[maxGames][maxPeriod];
Int_t ptVsQuar_[maxGames][maxPeriod];

Int_t minutes_[maxGames];

Int_t fgMake_[maxGames];
Float_t fgMakePer_[maxGames];
Int_t fgAttempt_[maxGames];
Float_t fgAttemptPer_[maxGames];
Float_t fgPerc_[maxGames];
Float_t fgPercPer_[maxGames];

Int_t threeMake_[maxGames];
Float_t threeMakePer_[maxGames];
Int_t threeAttempt_[maxGames];
Float_t threeAttemptPer_[maxGames];
Float_t threePerc_[maxGames];
Float_t threePercPer_[maxGames];

Int_t ftMake_[maxGames];
Float_t ftMakePer_[maxGames];
Int_t ftAttempt_[maxGames];
Float_t ftAttemptPer_[maxGames];
Float_t ftPerc_[maxGames];
Float_t ftPercPer_[maxGames];

Int_t offReb_[maxGames];
Float_t offRebPer_[maxGames];
Int_t defReb_[maxGames];
Float_t defRebPer_[maxGames];
Int_t totReb_[maxGames];
Float_t totRebPer_[maxGames];

Int_t ast_[maxGames];
Float_t astPer_[maxGames];
Int_t stl_[maxGames];
Float_t stlPer_[maxGames];
Int_t blk_[maxGames];
Float_t blkPer_[maxGames];
Int_t tov_[maxGames];
Float_t tovPer_[maxGames];
Int_t fouls_[maxGames];
Float_t foulsPer_[maxGames];

void SetBBallBranches(Bool_t fullSet);
void OffBBallBranches();
void GetBBallBranches(Bool_t fullSet);
void InitBBallTrees(Bool_t fullSet);
void FillBBallTrees();
void WriteBBallTrees(TFile* outFile_p);
void CleanupBBallTrees();
void GetBBallTrees(TFile* inFile_p, Bool_t fullSet);
void GetBBallTreesEntry(Int_t entryNum);
void InitBBallVar();

void SetBBallBranches(Bool_t fullSet)
{
  std::cout << "Setting stat tree branches..." << std::endl;

  statTree_p->Branch("teamStr", &teamStr_);
  statTree_p->Branch("teamNum", &teamNum_, "teamNum/I");
  statTree_p->Branch("year", &year_, "year/I");

  statTree_p->Branch("nGames", &nGames_, "nGames/I");

  statTree_p->Branch("month", month_, "month[nGames]/I");
  statTree_p->Branch("day", day_, "day[nGames]/I");
  statTree_p->Branch("isHome", isHome_, "isHome[nGames]/O");
  statTree_p->Branch("teamNumVs", teamNumVs_, "teamNumVs[nGames]/I");
  statTree_p->Branch("gameID", gameID_, "gameID[nGames]/L");
  statTree_p->Branch("win", win_, "win[nGames]/I");
  statTree_p->Branch("loss", loss_, "loss[nGames]/I");
  statTree_p->Branch("ptFor", ptFor_, "ptFor[nGames]/I");
  statTree_p->Branch("ptVs", ptVs_, "ptVs[nGames]/I");
  statTree_p->Branch("ptDiff", ptDiff_, "ptDiff[nGames]/I");
  statTree_p->Branch("ptDiffErr", ptDiffErr_, "ptDiffErr[nGames]/F");

  statTree_p->Branch("ptForPer", ptForPer_, "ptForPer[nGames]/F");
  statTree_p->Branch("ptForPerErr", ptForPerErr_, "ptForPerErr[nGames]/F");
  statTree_p->Branch("ptVsPer", ptVsPer_, "ptVsPer[nGames]/F");
  statTree_p->Branch("ptVsPerErr", ptVsPerErr_, "ptVsPerErr[nGames]/F");
  statTree_p->Branch("ptDiffPer", ptDiffPer_, "ptDiffPer[nGames]/F");
  statTree_p->Branch("ptDiffPerErr", ptDiffPerErr_, "ptDiffPerErr[nGames]/F");

  if(fullSet){
    statTree_p->Branch("nPeriod", nPeriod_, "nPeriod[nGames]/I");
    statTree_p->Branch("ptForQuar", ptForQuar_, Form("ptForQuar[nGames][%d]/I", maxPeriod));
    statTree_p->Branch("ptVsQuar", ptVsQuar_, Form("ptVsQuar[nGames][%d]/I", maxPeriod));

    statTree_p->Branch("minutes", minutes_, "minutes[nGames]/I");

    statTree_p->Branch("fgMake", fgMake_, "fgMake[nGames]/I");
    statTree_p->Branch("fgMakePer", fgMakePer_, "fgMakePer[nGames]/F");
    statTree_p->Branch("fgAttempt", fgAttempt_, "fgAttempt[nGames]/I");
    statTree_p->Branch("fgAttemptPer", fgAttemptPer_, "fgAttemptPer[nGames]/F");
    statTree_p->Branch("fgPerc", fgPerc_, "fgPerc[nGames]/F");
    statTree_p->Branch("fgPercPer", fgPercPer_, "fgPercPer[nGames]/F");

    statTree_p->Branch("threeMake", threeMake_, "threeMake[nGames]/I");
    statTree_p->Branch("threeMakePer", threeMakePer_, "threeMakePer[nGames]/F");
    statTree_p->Branch("threeAttempt", threeAttempt_, "threeAttempt[nGames]/I");
    statTree_p->Branch("threeAttemptPer", threeAttemptPer_, "threeAttemptPer[nGames]/F");
    statTree_p->Branch("threePerc", threePerc_, "threePerc[nGames]/F");
    statTree_p->Branch("threePercPer", threePercPer_, "threePercPer[nGames]/F");

    statTree_p->Branch("ftMake", ftMake_, "ftMake[nGames]/I");
    statTree_p->Branch("ftMakePer", ftMakePer_, "ftMakePer[nGames]/F");
    statTree_p->Branch("ftAttempt", ftAttempt_, "ftAttempt[nGames]/I");
    statTree_p->Branch("ftAttemptPer", ftAttemptPer_, "ftAttemptPer[nGames]/F");
    statTree_p->Branch("ftPerc", ftPerc_, "ftPerc[nGames]/F");
    statTree_p->Branch("ftPercPer", ftPercPer_, "ftPercPer[nGames]/F");

    statTree_p->Branch("offReb", offReb_, "offReb[nGames]/I");
    statTree_p->Branch("offRebPer", offRebPer_, "offRebPer[nGames]/F");
    statTree_p->Branch("defReb", defReb_, "defReb[nGames]/I");
    statTree_p->Branch("defRebPer", defRebPer_, "defRebPer[nGames]/F");
    statTree_p->Branch("totReb", totReb_, "totReb[nGames]/I");
    statTree_p->Branch("totRebPer", totRebPer_, "totRebPer[nGames]/F");

    statTree_p->Branch("ast", ast_, "ast[nGames]/I");
    statTree_p->Branch("astPer", astPer_, "astPer[nGames]/F");
    statTree_p->Branch("stl", stl_, "stl[nGames]/I");
    statTree_p->Branch("stlPer", stlPer_, "stlPer[nGames]/F");
    statTree_p->Branch("blk", blk_, "blk[nGames]/I");
    statTree_p->Branch("blkPer", blkPer_, "blkPer[nGames]/F");
    statTree_p->Branch("tov", tov_, "tov[nGames]/I");
    statTree_p->Branch("tovPer", tovPer_, "tovPer[nGames]/F");
    statTree_p->Branch("fouls", fouls_, "fouls[nGames]/I");
    statTree_p->Branch("foulsPer", foulsPer_, "foulsPer[nGames]/F");
  }

  std::cout << "Setting book tree branches..." << std::endl;

  bookTree_p->Branch("nGames", &nGames_, "nGames/I");

  return;
}


void OffBBallBranches()
{
  statTree_p->SetBranchStatus("*", 0);
  bookTree_p->SetBranchStatus("*", 0);

  return;
}


void GetBBallBranches(Bool_t fullSet)
{
  std::cout << "Getting stat tree branches..." << std::endl;

  statTree_p->SetBranchStatus("*", 1);
  statTree_p->SetBranchAddress("teamNum", &teamNum_);
  statTree_p->SetBranchAddress("year", &year_);

  statTree_p->SetBranchAddress("nGames", &nGames_);
  statTree_p->SetBranchAddress("month", month_);
  statTree_p->SetBranchAddress("day", day_);
  statTree_p->SetBranchAddress("isHome", isHome_);
  statTree_p->SetBranchAddress("teamNumVs", teamNumVs_);
  statTree_p->SetBranchAddress("gameID", gameID_);
  statTree_p->SetBranchAddress("win", win_);
  statTree_p->SetBranchAddress("loss", loss_);
  statTree_p->SetBranchAddress("ptFor", ptFor_);
  statTree_p->SetBranchAddress("ptVs", ptVs_);
  statTree_p->SetBranchAddress("ptDiff", ptDiff_);
  statTree_p->SetBranchAddress("ptDiffErr", ptDiffErr_);

  statTree_p->SetBranchAddress("ptForPer", ptForPer_);
  statTree_p->SetBranchAddress("ptForPerErr", ptForPerErr_);
  statTree_p->SetBranchAddress("ptVsPer", ptVsPer_);
  statTree_p->SetBranchAddress("ptVsPerErr", ptVsPerErr_);
  statTree_p->SetBranchAddress("ptDiffPer", ptDiffPer_);
  statTree_p->SetBranchAddress("ptDiffPerErr", ptDiffPerErr_);

  if(fullSet){
    statTree_p->SetBranchAddress("nPeriod", nPeriod_);
    statTree_p->SetBranchAddress("ptForQuar", ptForQuar_);
    statTree_p->SetBranchAddress("ptVsQuar", ptVsQuar_);

    statTree_p->SetBranchAddress("minutes", minutes_);

    statTree_p->SetBranchAddress("fgMake", fgMake_);
    statTree_p->SetBranchAddress("fgMakePer", fgMakePer_);
    statTree_p->SetBranchAddress("fgAttempt", fgAttempt_);
    statTree_p->SetBranchAddress("fgAttemptPer", fgAttemptPer_);
    statTree_p->SetBranchAddress("fgPerc", fgPerc_);
    statTree_p->SetBranchAddress("fgPercPer", fgPercPer_);

    statTree_p->SetBranchAddress("threeMake", threeMake_);
    statTree_p->SetBranchAddress("threeMakePer", threeMakePer_);
    statTree_p->SetBranchAddress("threeAttempt", threeAttempt_);
    statTree_p->SetBranchAddress("threeAttemptPer", threeAttemptPer_);
    statTree_p->SetBranchAddress("threePerc", threePerc_);
    statTree_p->SetBranchAddress("threePercPer", threePercPer_);

    statTree_p->SetBranchAddress("ftMake", ftMake_);
    statTree_p->SetBranchAddress("ftMakePer", ftMakePer_);
    statTree_p->SetBranchAddress("ftAttempt", ftAttempt_);
    statTree_p->SetBranchAddress("ftAttemptPer", ftAttemptPer_);
    statTree_p->SetBranchAddress("ftPerc", ftPerc_);
    statTree_p->SetBranchAddress("ftPercPer", ftPercPer_);

    statTree_p->SetBranchAddress("offReb", offReb_);
    statTree_p->SetBranchAddress("offRebPer", offRebPer_);
    statTree_p->SetBranchAddress("defReb", defReb_);
    statTree_p->SetBranchAddress("defRebPer", defRebPer_);
    statTree_p->SetBranchAddress("totReb", totReb_);
    statTree_p->SetBranchAddress("totRebPer", totRebPer_);

    statTree_p->SetBranchAddress("ast", ast_);
    statTree_p->SetBranchAddress("astPer", astPer_);
    statTree_p->SetBranchAddress("stl", stl_);
    statTree_p->SetBranchAddress("stlPer", stlPer_);
    statTree_p->SetBranchAddress("blk", blk_);
    statTree_p->SetBranchAddress("blkPer", blkPer_);
    statTree_p->SetBranchAddress("tov", tov_);
    statTree_p->SetBranchAddress("tovPer", tovPer_);
    statTree_p->SetBranchAddress("fouls", fouls_);
    statTree_p->SetBranchAddress("foulsPer", foulsPer_);
  }

  std::cout << "Getting book tree branches..." << std::endl;

  bookTree_p->SetBranchStatus("*", 1);
  bookTree_p->SetBranchAddress("nGames", &nGames_);

  return;
}

void InitBBallTrees(Bool_t fullSet)
{
  std::cout << "Init Trees" << std::endl;

  statTree_p = new TTree("statTree_p", "statTree_p");
  bookTree_p = new TTree("bookTree_p", "bookTree_p");
  SetBBallBranches(fullSet);

  return;
}


void FillBBallTrees()
{
  statTree_p->Fill();
  bookTree_p->Fill();
  return;
}


void WriteBBallTrees(TFile* outFile_p)
{
  outFile_p->cd();
  statTree_p->Write("", TObject::kOverwrite);
  bookTree_p->Write("", TObject::kOverwrite);

  return;
}


void CleanupBBallTrees()
{
  if(statTree_p != 0) delete statTree_p;
  if(bookTree_p != 0) delete bookTree_p;
  return;
}

void GetBBallTrees(TFile* inFile_p, Bool_t fullSet, Bool_t isOn = true)
{
  std::cout << "Get Trees" << std::endl;

  statTree_p = (TTree*)inFile_p->Get("statTree_p");
  bookTree_p = (TTree*)inFile_p->Get("bookTree_p");
  if(isOn) GetBBallBranches(fullSet);
  else OffBBallBranches();

  return;
}

void GetBBallTreesEntry(Int_t entryNum)
{
  statTree_p->GetEntry(entryNum);
  bookTree_p->GetEntry(entryNum);
  return;
}


void InitBBallVar()
{
  teamStr_ = "";
  teamNum_ = -1;
  year_ = -1;

  nGames_ = 0;

  return;
}

#endif
