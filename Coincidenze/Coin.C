

#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TMath.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void Coin()
{

  // --------------------------- DATI ------------------------------- //
  // Resistenza della lampadina, misurata in laboratorio con il multimetro

  // numero misure prese
  const int nmisure = 34;
  float dt[]  = {30.2,29,28,26.20,24.20,21.80,20,18.2,16,14,12,10,8,6,4,2,-1.6,-3.2,-5.2,-7.2,-10,-12,-14,-16,-18,-20,-22,-24,-26,-28,-30,-32,-34,0}; // V
  float coin[]  = {17,23,16,60,123,444,1046,1391,1440,1411,1508,1422,1419,1459,1437,1469.5,1476,1437,1516,1452,1477,1465,1460,1461,1430,1379,1353,1089,393,118.5,56,39,23,1478}; // mA
  float rate[nmisure];
  float sdt[nmisure];
  float scoin[nmisure];
  float srate[nmisure];
  // ciclo for (loop) sulle misure
  for(int j=0; j<nmisure;++j){

    sdt[j]= 0.2;
    rate[j]=float(coin[j])/120.;
    scoin[j]=pow(coin[j],0.5);
    srate[j]=rate[j]*pow(pow((scoin[j]/coin[j]),2)+pow(1e-3/120,2),0.5);
  }
  // ----------------------------------------------------------------- //

  TCanvas *ciV = new TCanvas("cVe","Ve",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV->cd();
  // Istanzio il grafico. Il costruttore che noi usiamo prende come argomenti:
  // il numero di misure da disegnare, l'array di misure x (=V), l'array di misure y (=i), e gli
  // array delle  rispettive incertezze
  TGraphErrors *giV = new TGraphErrors(nmisure,dt,rate,sdt,srate);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("Curva di coincidenza");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("time [ns]");
  giV->GetYaxis()->SetTitle("rate [s^{-1}]");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");
  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz0 = new TF1("funz0","[0]*x+[1]",-19,17);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1);
  funz0->SetLineColor(2);
  funz0->SetParameter(0,0);
  funz0->SetParameter(1,12);

  TF1 *funz1 = new TF1("funz1","[0]/(1+exp((x-[1])/[2]))",13,25);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz1->SetLineStyle(1);
  funz1->SetLineColor(4);
  funz1->SetParameter(0,10);
  //funz1->SetParLimits(0,0,1);
  funz1->SetParameter(1,20);
  funz1->SetParameter(2,1);
  giV->Fit(funz1,"RM+");
  TF1 *funz2 = new TF1("funz2","[0]/(1+exp(([1]-x)/[2]))",-30,-10);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz2->SetLineStyle(1);
  funz2->SetLineColor(4);
  funz2->SetParameter(0,10);
  //funz2->SetParLimits(0,0,1);
  funz2->SetParameter(1,20);
  funz2->SetParameter(2,1);
  giV->Fit(funz2,"RM+");
  giV->Fit(funz0,"RM+");
  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
