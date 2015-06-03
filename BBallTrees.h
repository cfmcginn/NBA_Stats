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
