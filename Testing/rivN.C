

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
void rivN()
{

  // --------------------------- DATI ------------------------------- //
  // Resistenza della lampadina, misurata in laboratorio con il multimetro

  // numero misure prese
  const int nmisure = 8;
  float V[]  = {1402,1462,1531,1603,1652,1734,1794,1840}; // V
  int H[]  = {56,127,413,955,1630,3379,5436,7663}; // mA
  int NM[] = {277,295,271,283,293,279,275,316};
  int NHM[] = {9,15,66,146,217,258,263,309};
  float eps[nmisure];
  float seps[nmisure];
  float sV[nmisure];
  float sH[nmisure];
  float sNM[nmisure];
  float sNHM[nmisure];
  // ciclo for (loop) sulle misure
  for(int j=0; j<nmisure;++j){

    sV[j]=V[j]*0.005+1;
    sH[j]=pow(H[j],0.5);
    sNM[j]=pow(NM[j],0.5);
    sNHM[j]=pow(NHM[j],0.5);
    eps[j] = float(NHM[j])/float(NM[j]);
    seps[j]=pow((eps[j]*(1-eps[j])/NM[j]),0.5);
    // Stampa a video dei valori. \t inserisce un tab nel print out. Mettendo \n si va a capo invece
    cout << "N Misura " << j << ":\t V = (" << V[j] << " +- " << sV[j] << ") V, \t H = (" << H[j] << " +- " << sH[j] << "),\t NM = ("
	 << setprecision(4) << NM[j] << " +- " << sNM[j] << "), \t NHM = ("
	 << NHM[j] << " " << sNHM[j] << ") , \t eps = ("
	 << eps[j] << " " << seps[j] << ")" << endl;
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
  TGraphErrors *giV = new TGraphErrors(nmisure,V,eps,sV,seps);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("Efficienza di N");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("Tensione HV [V]");
  giV->GetYaxis()->SetTitle("#epsilon");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");

  cout << "\n\n --- Fit I(V) \n" <<endl;
  TF1 *funz0 = new TF1("funz0","[0]/(1+exp(([1]-x)/[2]))",1400,1850);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1);
  funz0->SetLineColor(2);
  funz0->SetParameter(0,1);
  funz0->SetParLimits(0,0,1);
  funz0->SetParameter(1,1700);
  funz0->SetParameter(2,50);
  giV->Fit(funz0,"RM+");

  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;


}
