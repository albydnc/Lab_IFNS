

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
void CboxDeg()
{

  // --------------------------- DATI ------------------------------- //
  // Resistenza della lampadina, misurata in laboratorio con il multimetro

  // numero misure prese
  const int nmisure = 5;
  float deg[] = {0,15,30,45,60};
  float sdeg[nmisure];
  float time[]  = {420.3,420.2,420.2,420.4,420.2};
  float coin[]  = {213,205,153,95,48};
  float sTime[nmisure];
  float rate[nmisure];
  float srate[nmisure];
  float scoin[nmisure];
  // ciclo for (loop) sulle misure
  for(int j=0; j<nmisure;++j){
    sdeg[j]=5;
    sTime[j]=0.1;
    rate[j]=coin[j]/time[j];
    scoin[j]=pow(coin[j],0.5);
    srate[j]=rate[j]*pow(pow((scoin[j]/coin[j]),2)+pow(sTime[j]/time[j],2),0.5);
    // Stampa a video dei valori. \t inserisce un tab nel print out. Mettendo \n si va a capo invece
    //cout << "N Misura " << j << ":\t V = (" << V[j] << " +- " << sV[j] << ") V, \t H = (" << H[j] << " +- " << sH[j] << endl;
  }
  // ----------------------------------------------------------------- //

  TCanvas *ciV = new TCanvas("cdeg","deg",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV->cd();
  // Istanzio il grafico. Il costruttore che noi usiamo prende come argomenti:
  // il numero di misure da disegnare, l'array di misure x (=V), l'array di misure y (=i), e gli
  // array delle  rispettive incertezze
  TGraphErrors *giV = new TGraphErrors(nmisure,deg,rate,sdeg,srate);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("Distribuzione Angolare dei Raggi Cosmici");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("#theta [#circ]");
  giV->GetYaxis()->SetTitle("Rate [s^{-1}]");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");

  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz0 = new TF1("funz0","[0]*pow(cos(x*3.14/180),[1])",0,65);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1);
  funz0->SetLineColor(2);
  funz0->SetParameter(0,200);
  funz0->SetParameter(1,2);;
  giV->Fit(funz0,"RM+");

  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;


}
