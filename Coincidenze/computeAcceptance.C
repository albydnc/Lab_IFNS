#include <iostream>
#include <iomanip>
#include <math.h>

#include "TH1F.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TPad.h"

double pi = 3.141592;

void computeAcceptance(unsigned int events = 50000000,
		       double length = 14.5,     // in cm
		       double width = 14.5,      // in cm
		       double gapWidth = 0.,     // in cm (if the scintillators are in two pieces)
		       double zSpacing = 7,   // in cm
		       double tiltAngle = 0.)    // in degrees
{
  // compute cosmic ray acceptance of a two-slab scintillator detector
  // (slabs assumed as infinitely thin)
  // one parameter can be used to vary without re-compiling

  bool drawTracks = true;

  // sanity check
  bool isTilted = false;
  if (fabs(tiltAngle) > 0.1) {
    isTilted = true;
    std::cout << "The tilted geometry is not yet implemented. Giving up..." << std::endl;
    return;
  }

  // histograms
  TH1F* hxUp = new TH1F("hxUp", "x on upper slab", 100, -0.55*length, 0.55*length);
  TH1F* hyUp = new TH1F("hyUp", "y on upper slab", 100, -0.6*width, 0.6*width);
  TH1F* hxDown = new TH1F("hxDown", "x on lower slab", 100, -0.55*length, 0.55*length);
  TH1F* hyDown = new TH1F("hyDown", "y on lower slab", 100, -0.6*width, 0.6*width);
  TH1F* hphi = new TH1F("hphi", "muon phi", 100, -3.5, 3.5);
  TH1F* hcosTheta = new TH1F("hcosTheta", "muon cos(theta)", 100, -0.05, 1.05);

  // tracks
  TLine* longTrack[1000];
  TLine* trasvTrack[1000];

  // miscellaneous initialization
  TRandom3 *rnd = new TRandom3(1234);
  unsigned int npassed = 0;
  unsigned int ndrawn = 0;
  double meanTime = 0.;
  double meanTimeSq = 0.;
  bool drawThisOne = true;

  for (unsigned int ievent = 0; ievent < events; ievent++) {

    // track color: red if in acceptance, green if out
    int icolor = 0;

    // randomly choose hit point on upper slab
    Double_t xUp = rnd->Uniform(length) - length/2.;   // rnd between -l/2 and l/2
    Double_t yUp = rnd->Uniform(width) - width/2.;     // rnd between -w/2 and w/2
    if (yUp >= 0.) yUp += gapWidth/2.;    // consider gap in between
    else yUp -= gapWidth/2.;
    hxUp->Fill(xUp);
    hyUp->Fill(yUp);

    // randomly choose theta / phi of muon track
    Double_t phi = rnd->Uniform(2.*pi) - pi;

    // For theta use inverted-cumulative MonteCarlo method
    Double_t cosTheta = cbrt(rnd->Uniform());

    // Now propagate track from upper hit
    Double_t xDown = xUp + zSpacing*cos(phi)*sqrt(1-cosTheta*cosTheta)/cosTheta;
    Double_t yDown = yUp + zSpacing*sin(phi)*sqrt(1-cosTheta*cosTheta)/cosTheta;

    if (fabs(xDown) < length/2. && fabs(yDown) < (width+gapWidth)/2. && fabs(yDown) > gapWidth/2.) {
      npassed++;          // is in acceptance
      hxDown->Fill(xDown);
      hyDown->Fill(yDown);
      icolor = 2;
      // compute ToF
      double tof = zSpacing/(cosTheta*29.9);     // c = 29.9 cm/ns
      meanTime += tof;         meanTimeSq += tof*tof;
    } else icolor = 3;   // is out

    hphi->Fill(phi);
    hcosTheta->Fill(cosTheta);

    if (ndrawn < 1000) {
      float xDraw = xDown;            float yDraw = yDown;
      float zDraw1 = -zSpacing/2.;    float zDraw2 = -zSpacing/2.;
      if (icolor == 3) {
	if (xUp > xDown) xDraw = TMath::Max(xDown,(double)-0.85*length);
	else xDraw = TMath::Min(xDown,(double)0.85*length);
	if (yUp > yDown) yDraw = TMath::Max(yDown,(double)-0.85*width);
	else yDraw = TMath::Min(yDown,(double)0.85*width);
	zDraw1 = zSpacing*(xDraw-xDown)/(xUp-xDown) - zSpacing/2.;
        zDraw2 = zSpacing*(yDraw-yDown)/(yUp-yDown) - zSpacing/2.;
      }
      longTrack[ndrawn] = new TLine(xUp,zSpacing/2.,xDraw,zDraw1);
      longTrack[ndrawn]->SetLineWidth(1);
      longTrack[ndrawn]->SetLineColor(icolor);
      trasvTrack[ndrawn] = new TLine(yUp,zSpacing/2.,yDraw,zDraw2);
      trasvTrack[ndrawn]->SetLineWidth(1);
      trasvTrack[ndrawn]->SetLineColor(icolor);
      ndrawn++;
    }
  }

  // Results: acceptance and mean time-of-flight
  double acceptance = (double)npassed/(double)events;
  double accErr = sqrt(acceptance*(1-acceptance)/(double)events);
  meanTime /= (double)npassed;   meanTimeSq /= (double)npassed;
   double meanTimeErr = sqrt((meanTimeSq - meanTime*meanTime)/double(npassed));

  std::cout << "ACCEPTANCE = " << std::setprecision(8) << acceptance << " +/- " << std::setprecision(2) << accErr << std::endl;

  std::cout << "mean Time-Of-Flight = (" << std::setprecision(5) << meanTime << " +/- " << std::setprecision(2) << meanTimeErr << ") ns " << endl;

  char fileString[200];

  if (drawTracks) {

    // Draw tracks and scintillators

    // Define axes
    double dummyGraphX[4] = {(double)-0.8*length, (double)-0.8*length, (double)0.8*length, (double)0.8*length};
    double dummyGraphY[4] = {(double)-0.8*width, (double)-0.8*width, (double)0.8*width, (double)0.8*width};
    double dummyGraphZ[4] = {(double)-0.6*zSpacing, (double)0.6*zSpacing, (double)-0.6*zSpacing, (double)0.6*zSpacing};
    TGraph* dummyGraphLong = new TGraph(4,dummyGraphX,dummyGraphZ);
    dummyGraphLong->SetMarkerColor(0);
    dummyGraphLong->SetTitle("Longitudinal view");
    TGraph* dummyGraphTrasv = new TGraph(4,dummyGraphY,dummyGraphZ);
    dummyGraphTrasv->SetMarkerColor(0);
    dummyGraphTrasv->SetTitle("Transverse view");

    // best draw settings (?)
    unsigned int lineDraw = (ndrawn > 1000) ? 1000 : ndrawn;
    int canvHor = 800;        int canvVer = 800.;
    if (length > 4.*zSpacing) {canvHor = 1000; canvVer = 200.;}

    // longitudinal view
    TCanvas cl("cl","Longitudinal view",10,10,canvHor,canvVer);
    cl.SetGrid();
    dummyGraphLong->Draw("AP");
    TLine scintUpL(-length/2.,zSpacing/2.,length/2.,zSpacing/2.);
    scintUpL.SetLineWidth(8);   scintUpL.SetLineColor(4);
    TLine scintDownL(-length/2.,-zSpacing/2.,length/2.,-zSpacing/2.);
    scintDownL.SetLineWidth(8);   scintDownL.SetLineColor(4);
    scintUpL.Draw("same");
    scintDownL.Draw("same");
    for (unsigned int il = 0; il < lineDraw; il++) {
      longTrack[il]->Draw("same");
    }
    sprintf(fileString,"longitView_l%d_w%d_zsp%d_tilt%d.gif",(int)length,(int)width,(int)zSpacing,(int)tiltAngle);
    cl.SaveAs(fileString);

    // best draw settings (?)
    canvHor = 800;            canvVer = 800.;
    if (width > 4.*zSpacing) {canvHor = 1000; canvVer = 200.;}

    // transverse view
    TCanvas ct("ct","Transverse view",20,20,canvHor,canvVer);
    ct.SetGrid();
    dummyGraphTrasv->Draw("AP");
    TLine scintUpT1(-(width+gapWidth)/2.,zSpacing/2.,-gapWidth/2.,zSpacing/2.);
    scintUpT1.SetLineWidth(8);    scintUpT1.SetLineColor(4);
    TLine scintUpT2((width+gapWidth)/2.,zSpacing/2.,gapWidth/2.,zSpacing/2.);
    scintUpT2.SetLineWidth(8);    scintUpT2.SetLineColor(4);
    TLine scintDownT1(-(width+gapWidth)/2.,-zSpacing/2.,-gapWidth/2.,-zSpacing/2.);
    scintDownT1.SetLineWidth(8);     scintDownT1.SetLineColor(4);
    TLine scintDownT2((width+gapWidth)/2.,-zSpacing/2.,gapWidth/2.,-zSpacing/2.);
    scintDownT2.SetLineWidth(8);    scintDownT2.SetLineColor(4);
    scintUpT1.Draw("same");
    scintUpT2.Draw("same");
    scintDownT1.Draw("same");
    scintDownT2.Draw("same");
    for (unsigned int il = 0; il < lineDraw; il++) {
      trasvTrack[il]->Draw("same");
    }
    sprintf(fileString,"transvView_l%d_w%d_zsp%d_tilt%d.gif",(int)length,(int)width,(int)zSpacing,(int)tiltAngle);
    ct.SaveAs(fileString);
  }

  // output histograms
  sprintf(fileString,"outputHists_l%d_w%d_zsp%d_tilt%d.root",(int)length,(int)width,(int)zSpacing,(int)tiltAngle);
  TFile f(fileString,"RECREATE");
  hxUp->Write();
  hyUp->Write();
  hxDown->Write();
  hyDown->Write();
  hphi->Write();
  hcosTheta->Write();
  f.Close();

}
