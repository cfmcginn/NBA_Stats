#ifndef getMean_h
#define getMean_h

#include "TMath.h"
#include <iostream>

void getMean(Int_t nVal, Int_t val[], Float_t& mean, Float_t& err)
{
  mean = 0;
  err = 0;

  for(Int_t iter = 0; iter < nVal; iter++){
    mean+= val[iter];
    err+= val[iter]*val[iter];
  }
  mean /= nVal;
  err /= nVal;

  err -= mean*mean;

  err = TMath::Sqrt(err);
  if(nVal != 1) err /= TMath::Sqrt(nVal-1);
  else err = TMath::Sqrt(mean);

  return;
}

#endif
