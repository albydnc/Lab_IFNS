  #include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;

Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

void poisson33(){

int nmisure = 12;
float xmax[12];
float in[] = {434, 5644, 22475, 56315, 114844, 194631, 252640, 329908, 361779, 362773, 330997, 290574};
float sxmax[12];
float sin[] = {178.9766174719, 1032.6229824174, 3069.6388666976, 6592.7930392548, 11902.0066597002, 17477.6002213492, 21656.7764015973, 24383.852469708, 23675.006640244, 21994.0284216855, 18172.2655204594, 14713.7061091661};
for (int j=0;j<12;j++){
xmax[j]=j;
sxmax[j]=0.5;
}






  // --------------------- Grafico i(V) ------------------------------ //
  // Creo il canvas (la tela) su cui disegnare il grafico. "ciV" è il il suo nome in memoria
  // è bene che sia unico dentro al programma. "i(v)" è semplicemente il titolo del canvas
  // le due coppie di numeri che seguono sono le coordinate dell'angolo superiore sinistro e le seconde
  // la dimensione in pixel lungo x e lungo y
  TCanvas *ciV = new TCanvas("ciV","Poisson",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV->cd();
  // Istanzio il grafico. Il costruttore che noi usiamo prende come argomenti:
  // il numero di misure da disegnare, l'array di misure x (=V), l'array di misure y (=i), e gli
  // array delle  rispettive incertezze
  TGraphErrors *giV = new TGraphErrors(nmisure,xmax,in,sxmax,sin);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("Poisson LED 3.3");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("# Picco");
  giV->GetYaxis()->SetTitle("Integrale");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");

  cout << "\n\n --- Fit I(V) pow([0],1/([1]+1))*pow(x,([1]-1)/([1]+1)) \n" <<endl;
  TF1 *funz0 = new TF1("funz0",fpoisson,0,12,2);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineStyle(1); // Blu
  funz0->SetLineColor(2);
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione.
  funz0->SetParameter(0,10^5);
  funz0->SetParameter(1,9);
  giV->Fit(funz0,"RM+");


  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
