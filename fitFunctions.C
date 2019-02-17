#include <iostream>
#include <fstream>
#include <math.h>   

#include "TH1F.h"
#include "TF1.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"

Double_t FFFalling(Double_t *x,Double_t *par) {
  return par[0]/(1+exp((x[0] - par[1])/par[2]));
}

Double_t FFRising(Double_t *x,Double_t *par) {
  return par[0]/(1+exp((par[1] - x[0])/par[2]));
}

Double_t multipeak(Double_t *x,Double_t *par) {
  unsigned int npeaks = 12;
  float result = 0.;
  float mean = 0.;
  for (unsigned int i = 0; i < npeaks; i++) {
    mean = par[0]*i;
    result += par[2*i+1]*TMath::Gaus(x[0],mean,par[2*i+2],false);
  }
  return result;
}

Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

// ----------------------------------------------------
// Altre forme funzionali possibili per fit multi-picco
// ----------------------------------------------------

Double_t multipeak_noConstraints(Double_t *x,Double_t *par) {
  unsigned int npeaks = 12;
  float result = 0.;
  for (unsigned int i = 0; i < npeaks; i++) {
    result += par[3*i]*TMath::Gaus(x[0],par[3*i+1],par[3*i+2],false);
  }
  return result;
}

Double_t multipeak_sigmaConstraint(Double_t *x,Double_t *par) {
  unsigned int npeaks = 12;
  float result = 0.;
  float mean = 0.;
  float sigma = 0.;
  for (unsigned int i = 0; i < npeaks; i++) {
    mean = par[0]*i;
    sigma = sqrt(par[1]*par[1] + i*par[2]*par[2]);
    result += par[i+3]*TMath::Gaus(x[0],mean,sigma,false);
  }
  return result;
}
