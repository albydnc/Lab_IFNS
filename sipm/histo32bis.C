

#include <iostream>           // ci serve per stampare a video il testo
#include <fstream>
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TMath.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
Double_t multipeak(Double_t *x,Double_t *par) {
  unsigned int npeaks = 9;
  float result = 0.;
  float mean = 0;
  for (unsigned int i = 0; i < npeaks; i++) {
    mean = par[0]*i;
    result += par[2*i+1]*TMath::Gaus(x[0],mean,par[2*i+2],false);
  }
  return result;
}

Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

void histo32bis()
{

  // --------------------------- DATI ------------------------------- //
  // Resistenza della lampadina, misurata in laboratorio con il multimetro

  // numero misure prese
  const int n = 1903;
  double ch[n];
  double count[n];
  double x = 0.;
  double y = 0.;
  ifstream a;
  a.open("D8_led32_reduce.txt");
  int i=0;
  while(!a.eof()&&i<n){
    a>>ch[i]>>count[i];
    cout << ch[i]<<"\t"<<count[i]<<endl;
    x += count[i]*ch[i];
    y += count[i];
    i++;
  }
  cout << "media "<< (x/y)<<endl;
  a.close();
  double sch[n];
  double scount[n];
  for(int j = 0; j<n;j++){
    ch[j]-=1105.;
    sch[j]=4;
    scount[j]=pow(count[j],0.5)+20;
  }
  TCanvas *ciV = new TCanvas("cVe","Ve",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV->cd();
  // Istanzio il grafico. Il costruttore che noi usiamo prende come argomenti:
  // il numero di misure da disegnare, l'array di misure x (=V), l'array di misure y (=i), e gli
  // array delle  rispettive incertezze
  TGraph *giV = new TGraphErrors(n,ch,count,sch,scount);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("epsM(V)");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("V [V]");
  //giV->GetXaxis()->SetAxisLimits(0,3000);
  giV->GetYaxis()->SetTitle("eps");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");

  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz0 = new TF1("funz0",multipeak,-200,3150,19);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1);
  funz0->SetLineColor(2);
  funz0->SetParameter(0,364);
  /*funz0->SetParameter(2,16);
  funz0->SetParameter(4,22);
  funz0->SetParameter(6,41);*/
  //funz0->SetParameter(2,30);
  funz0->SetParameter(2,35);
  funz0->SetParameter(4,55);
  funz0->SetParameter(6,46);
  funz0->SetParameter(8,46);
  funz0->SetParameter(10,51);
  funz0->SetParameter(12,53);
  funz0->SetParameter(14,59);
  funz0->SetParameter(16,56);
  /*funz0->SetParLimits(2,10,20);
  funz0->SetParLimits(4,15,30);
  funz0->SetParLimits(6,25,50);*/
  funz0->SetParLimits(2,20,40);
  funz0->SetParLimits(4,20,50);
  funz0->SetParLimits(6,25,60);
  funz0->SetParLimits(8,40,70);
  funz0->SetParLimits(10,40,80);
  funz0->SetParLimits(12,30,65);
  funz0->SetParLimits(14,30,75);
  funz0->SetParLimits(16,35,70);
  funz0->SetParLimits(18,30,65);
  /*funz0->SetParLimits(1,5,15);
  funz0->SetParLimits(3,10,20);
  funz0->SetParLimits(5,300,500);*/
  funz0->SetParLimits(1,650,850);
  funz0->SetParLimits(3,1350,1500);
  funz0->SetParLimits(5,2000,2300);
  funz0->SetParLimits(7,2500,2600);
  funz0->SetParLimits(9,2870,3000);
  funz0->SetParLimits(11,2750,2900);
  funz0->SetParLimits(13,2450,2550);
  funz0->SetParLimits(15,2000,2080);
  funz0->SetParLimits(17,1500,1610);
  giV->Fit(funz0,"RM+");
  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
    //
}
