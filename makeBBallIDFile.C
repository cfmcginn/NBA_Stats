//=============================================
// Author: Chris McGinn
//
// BBall IDFile Maker
//
//=============================================

#include "BBallTrees.h"
#include <fstream>
#include <vector>

int makeBBallIDFile(const std::string inName, const Int_t year);

int runMakeBBallIDFile(const std::string inName)
{
  Int_t jobStatus = -1;

  const Int_t nYears = 11;
  const Int_t years[nYears] = {2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015};

  for(int iter = 0; iter < nYears; iter++){
    jobStatus = makeBBallIDFile(inName, years[iter]);
    if(jobStatus != 0) break;
  }

  return jobStatus;
}

int makeBBallIDFile(const std::string inName, const Int_t year)
{
  std::string outName = inName;
  const std::string cutString[2] = {"Tree", ".root"};
  const std::string repString[2] = {"IDFile", ""};

  for(Int_t iter = 0; iter < 2; iter++){
    std::size_t strIndex = outName.find(cutString[iter]);
    if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString[iter].length(), repString[iter]);
  }

  TFile* inFile_p = new TFile(inName.c_str(), "READ");
  GetBBallTrees(inFile_p, false, false);

  statTree_p->SetBranchStatus("year", 1);
  statTree_p->SetBranchStatus("nGames", 1);
  statTree_p->SetBranchStatus("gameID", 1);

  statTree_p->SetBranchAddress("year", &year_);
  statTree_p->SetBranchAddress("nGames", &nGames_);
  statTree_p->SetBranchAddress("gameID", gameID_);

  Int_t nEntries = statTree_p->GetEntries();

  std::ofstream outFile;
  outFile.open(Form("csvDir/%dID.csv", year));
  outFile.close();

  outFile.open(Form("csvDir/%dID.csv", year), std::ios_base::app);

  std::vector<Long64_t>* duplicate_p = new std::vector<Long64_t>;

  for(Int_t jEntry = 0; jEntry < nEntries; jEntry++){
    GetBBallTreesEntry(jEntry);
    if(year_ != year) continue;

    for(Int_t iter = 0; iter < nGames_; iter++){

      Bool_t isDup = false;
      Int_t nDup = duplicate_p->size();
      for(Int_t dupIter = 0; dupIter < nDup; dupIter++){
	if(duplicate_p->at(dupIter) == gameID_[iter]){
	  isDup = true;
	  break;
	}
      }

      if(isDup) continue;
      else{
	outFile << gameID_[iter]  << ",\n";
	duplicate_p->push_back(gameID_[iter]);
      }	
    }
  }

  duplicate_p->clear();

  outFile.close();

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
