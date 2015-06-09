#include <fstream>
#include <vector>
#include "BBallTrees.h"
#include "getMean.h"

int makeBBallTrees(const std::string fList, const std::string fList2);
Long64_t getGameID(const Int_t year, const Int_t month, const Int_t day, const Int_t teamNum, const Int_t teamNumVs, const Bool_t isHome);

int main(int argc, char* argv[])
{
  if(argc != 2 && argc != 3){
    std::cout << "usage: makeBBallTrees <inputList> <inputList2 (opt)>" << std::endl;
    std::cout << "argc: " << argc << std::endl;
    for(Int_t iter = 0; iter < argc; iter++){
      std::cout << "arg" << iter << ": "<< argv[iter] << std::endl;
    }

    return 1;
  }

  int jobStatus = -1;
  if(argc == 2) jobStatus = makeBBallTrees(argv[1], "");
  else jobStatus = makeBBallTrees(argv[1], argv[2]);
  return jobStatus;
}

int makeBBallTrees(const std::string fList, const std::string fList2)
{
  Bool_t fullSet = false;
  if(strcmp("", fList2.c_str()) != 0) fullSet = true;

  std::string outName = fList;
  const std::string cutString = ".txt";
  const std::string addString = "_BBallTrees.root";
  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString.length(), addString);


  std::string buffer;
  std::vector<std::string> listOfFiles;
  std::ifstream inFile(fList.data());

  std::cout << "inList: " << fList << std::endl;

  if(!inFile.is_open()){
    std::cout << "Error opening file 1. Exiting." << std::endl;
    return 1;
  }
  else{
    while(true){
      inFile >> buffer;
      if(inFile.eof()) break;
      listOfFiles.push_back(buffer);
    }
  }

  std::cout << "FileList Loaded." << std::endl;

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");
  InitBBallTrees(fullSet);

  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    if(fileIter%1 == 0) std::cout << "File Number: " << fileIter << std::endl;
    InitBBallVar();

    std::string fileStr = listOfFiles[fileIter];
    const std::string cullString = "/";
    while(true){
      std::size_t strIndex = fileStr.find(cullString);
      if(strIndex == std::string::npos) break;
      fileStr.replace(0, strIndex + 1, "");
    }
    teamStr_ = fileStr.substr(0, 3);

    for(Int_t teamIter = 0; teamIter < nTeams; teamIter++){
      if(!strcmp(teamStr_.c_str(), teams[teamIter].c_str())){
	teamNum_ = teamIter;
	break;
      }
    }

    year_ = std::atoi(fileStr.substr(3, 4).c_str());

    std::ifstream csvFile(listOfFiles[fileIter].c_str());
    std::string outVal;

    while(true){
      std::getline(csvFile, outVal, ',');
      if(csvFile.eof()) break;
      month_[nGames_] = Int_t(std::atoi(outVal.c_str()));

      std::getline(csvFile, outVal, ',');
      day_[nGames_] = Int_t(std::atoi(outVal.c_str()));

      std::getline(csvFile, outVal, ',');
      isHome_[nGames_] = Bool_t(std::atoi(outVal.c_str()));

      std::getline(csvFile, outVal, ',');
      teamNumVs_[nGames_] = Int_t(std::atoi(outVal.c_str()));

      gameID_[nGames_] = getGameID(year_, month_[nGames_], day_[nGames_], teamNum_, teamNumVs_[nGames_], isHome_[nGames_]);

      std::getline(csvFile, outVal, ',');
      ptFor_[nGames_] = Int_t(std::atoi(outVal.c_str()));

      for(Int_t ptIter = 0; ptIter < nGames_+1; ptIter++){
	getMean(ptIter+1, ptFor_, ptForPer_[ptIter], ptForPerErr_[ptIter]);
      }

      std::getline(csvFile, outVal, ',');
      ptVs_[nGames_] = Int_t(std::atoi(outVal.c_str()));

      for(Int_t ptIter = 0; ptIter < nGames_+1; ptIter++){
	getMean(ptIter+1, ptVs_, ptVsPer_[ptIter], ptVsPerErr_[ptIter]);
      }

      if(nGames_ == 0){
	win_[nGames_] = 0;
	loss_[nGames_] = 0;
      }
      else{
	win_[nGames_] = win_[nGames_-1];
	loss_[nGames_] = loss_[nGames_-1];
      }
      if(ptFor_[nGames_] > ptVs_[nGames_]) win_[nGames_]++;
      else loss_[nGames_]++;

      ptDiff_[nGames_] = ptFor_[nGames_] - ptVs_[nGames_];
      ptDiffErr_[nGames_] = TMath::Sqrt(ptFor_[nGames_] + ptVs_[nGames_]);

      for(Int_t ptIter = 0; ptIter < nGames_+1; ptIter++){
        getMean(ptIter+1, ptDiff_, ptDiffPer_[ptIter], ptDiffPerErr_[ptIter]);
      }

      nPeriod_[nGames_]= 0;
      for(Int_t periodIter = 0; periodIter < maxPeriod; periodIter++){
	ptForQuar_[nGames_][periodIter] = 0;
	ptVsQuar_[nGames_][periodIter] = 0;
      }

      std::getline(csvFile, outVal);
      nGames_++;
    }

    if(fullSet){
      std::cout << fList2 << std::endl;
      std::ifstream inFile2(fList2.data());

      if(!inFile2.is_open()){
	std::cout << "Error opening file 2. Exiting." << std::endl;
	return 1;
      }
      else{
	while(true){
	  inFile2 >> buffer;
	  std::size_t strIndex2 = buffer.find(Form("%d",year_));
	  if(inFile2.eof()) break;
	  if(strIndex2 != std::string::npos) break;
	}
      }

      std::string compTeamStr = Form("%d",teamNum_);
      if(teamNum_ < 10) compTeamStr = Form("0%d", teamNum_);

      for(Int_t iter = 0; iter < nGames_; iter++){
	std::ifstream csvFile2(buffer.c_str());

	while(true){
	  std::getline(csvFile2, outVal, ',');
	  if(csvFile2.eof()) break;

	  //	  if(gameID_[iter] != 1510290416) continue;
	  if(!strcmp(outVal.c_str(), Form("%lld", gameID_[iter]))){
	    std::cout << outVal << std::endl;

	    if(!strcmp(compTeamStr.c_str(), outVal.substr(outVal.size()-4, 2).c_str())) std::cout << outVal.substr(outVal.size()-4, 2).c_str() << std::endl;
	    else std::cout << outVal.substr(outVal.size()-2, 2).c_str() << std::endl;

	    std::getline(csvFile2, outVal, ',');
	    std::getline(csvFile2, outVal);
	    std::cout << "rest of string: " << outVal << std::endl;

	    
	    Int_t counter = 0;

	    while(counter < 4){
	      const std::string findBreak = ",,";
	      std::size_t breakIndex = outVal.find(findBreak.c_str());
	    
	      std::cout << "first section: " << outVal.substr(0, breakIndex) << std::endl;
	     
	      Int_t counter2 = 0; 

	      const std::string findBreak2 = ",";
	      std::size_t breakIndex2 = outVal.find(findBreak2.c_str());
	      Int_t tempPeriod = 0;
	      while(breakIndex != breakIndex2){
		if(counter == 0){
		  if(!isHome_[iter]) ptForQuar_[iter][tempPeriod] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else ptVsQuar_[iter][tempPeriod] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		}
		else if(counter == 1){
		  if(isHome_[iter]) ptForQuar_[iter][tempPeriod] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else ptVsQuar_[iter][tempPeriod] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		}
		else if(counter == 2 && !isHome_[iter]){
		  std::cout << "Counter " << counter << ": " << outVal << std::endl;
		  if(counter2 == 0) minutes_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 1) fgMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 2) fgAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 4) threeMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 5) threeAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 7) ftMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 8) ftAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 10) offReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 11) defReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 12) totReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 13) ast_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 14) stl_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 15) blk_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 16) tov_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 17) fouls_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());

		  counter2++;
		}
		else if (counter == 3 && isHome_[iter]){
		  std::cout << "Counter " << counter << ": " << outVal << std::endl;
		  if(counter2 == 0) minutes_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 1) fgMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 2) fgAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 4) threeMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 5) threeAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 7) ftMake_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 8) ftAttempt_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 10) offReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 11) defReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 12) totReb_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 13) ast_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 14) stl_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 15) blk_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 16) tov_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		  else if(counter2 == 17) fouls_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());

		  counter2++;
		}

		tempPeriod++;
		outVal.replace(0, breakIndex2+1, "");
		breakIndex = outVal.find(findBreak.c_str());
		breakIndex2 = outVal.find(findBreak2.c_str());
		std::cout << "breakIndices: " << breakIndex << ", " << breakIndex2 << std::endl;

		if(breakIndex == breakIndex2){
		  std::cout << "YOLO" << std::endl;
		  std::cout << outVal << std::endl;
		  //		  else if(counter == 3 && isHome_[iter] && counter2 == 17) fouls_[iter] = std::atoi(outVal.substr(0, breakIndex2).c_str());
		}
	      }

	      if(counter == 0) nPeriod_[iter] = tempPeriod;

	      outVal = outVal.substr(breakIndex+2);

	      std::cout << "rest: " << outVal << std::endl;
	      counter++;
	    }
	    
	    break;
	  }
	  else std::getline(csvFile2, outVal);
	}


      }
    }

    std::cout << "year: " << buffer << std::endl;
    FillBBallTrees();
  }

  WriteBBallTrees(outFile_p);
  CleanupBBallTrees();
  outFile_p->Close();
  delete outFile_p;

  return 0;
}


Long64_t getGameID(const Int_t year, const Int_t month, const Int_t day, const Int_t teamNum, const Int_t teamNumVs, const Bool_t isHome)
{
  Long64_t gameID = (year - 2000)*100000000 + month*1000000 + day*10000;  

  if(isHome) gameID += teamNum*100 + teamNumVs*1;
  else gameID += teamNumVs*100 + teamNum*1;

  return gameID;
}
