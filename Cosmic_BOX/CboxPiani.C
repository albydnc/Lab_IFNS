

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
void CboxPiani()
{

  // --------------------------- DATI ------------------------------- //
  // Resistenza della lampadina, misurata in laboratorio con il multimetro

  // numero misure prese
  const int nmisure = 5;
  //float piano[]  = {0,8,15,22.70,29.49}; // V
  float piano[]  = {0,1.39,2.32,3.02,3.7};
  float time12[] = {420.6,299.8,301.2,302.9,299.8};
  float time30[] = {420.3,420.2,423.0,440.3,419.8};
  int c12[]  = {641,327,273,256,242}; // mA
  int c30[] = {213,170,135,83,87};

  float r12[nmisure];
  float r30[nmisure];
  float stime12[nmisure];
  float stime30[nmisure];
  float sc12[nmisure];
  float sc30[nmisure];
  float sr12[nmisure];
  float sr30[nmisure];
  float spiano[nmisure];
  // ciclo for (loop) sulle misure
  for(int j=0; j<nmisure;++j){
    spiano[j] = 0.01;
    r12[j]=c12[j]/time12[j];
    r30[j]=c30[j]/time30[j];
    stime12[j]=0.1;
    stime30[j]=0.1;
    sc12[j]=pow(c12[j],0.5);
    sc30[j]=pow(c30[j],0.5);
    sr12[j]=r12[j]*pow(pow((sc12[j]/c12[j]),2)+pow(stime12[j]/time12[j],2),0.5);
    sr30[j]=r30[j]*pow(pow((sc30[j]/c30[j]),2)+pow(stime30[j]/time30[j],2),0.5);
    // Stampa a video dei valori. \t inserisce un tab nel print out. Mettendo \n si va a capo invece
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
  TGraphErrors *giV = new TGraphErrors(nmisure,piano,r12,spiano,sr12);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("exp12");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("piano (m)");
  giV->GetYaxis()->SetTitle("rate(s-1)");

  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  //giV2->Draw("AP");
  giV->Draw("AP");

  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz0 = new TF1("funz0","[0]*exp(-[1]*x)",-1,4);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1);
  funz0->SetLineColor(2);
  funz0->SetParameter(0,1.5);
  //funz0->SetParameter(1,0.02);
  //funz0->SetParameter(1,0.5);;
  giV->Fit(funz0,"RM+");

  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  //
  TCanvas *ciV2 = new TCanvas("cVe","Ve",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV2->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV2->cd();
  TGraphErrors *giV2 = new TGraphErrors(nmisure,piano,r30,spiano,sr30);
  giV2->SetMarkerSize(0.6);
  giV2->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV2->SetTitle("exp30");
  // Titoli degli assi
  giV2->GetXaxis()->SetTitle("piano (m)");
  giV2->GetYaxis()->SetTitle("rate(s-1)");
  giV2->Draw("AP");

  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz1 = new TF1("funz1","[0]*exp(-[1]*x)",-1,4);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz1->SetLineStyle(1);
  funz1->SetLineColor(2);
  funz1->SetParameter(0,0.5);
  //funz0->SetParameter(1,0.02);
  //funz0->SetParameter(1,0.5);;
  giV2->Fit(funz1,"RM+");

  cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  //
}
