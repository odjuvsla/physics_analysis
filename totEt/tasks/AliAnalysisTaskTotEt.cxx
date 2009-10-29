#include "TChain.h" 
#include "TTree.h"
#include "TH1F.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TObjArray.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"

#include "AliESDEvent.h"
#include "AliAODEvent.h"
#include "AliVEvent.h"
#include "AliESDCaloCluster.h"
#include "AliESDCaloCells.h"
#include "AliESDInputHandler.h"

#include "AliAnalysisTaskTotEt.h"

#include <iostream>

using namespace std;

ClassImp(AliAnalysisTaskTotEt)

//________________________________________________________________________
AliAnalysisTaskTotEt::AliAnalysisTaskTotEt(const char *name) : 
AliAnalysisTaskSE(name), 
  fESD(0), 
  fHistEt(0), 
  fHistTotE(0),
  fHistEtCells(0),
  fHistCell(0),
  fEtNtuple(0),
  fCount(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  //  DefineInput(0, TChain::Class());
  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TH1F::Class());
  DefineOutput(2, TH1F::Class());
  DefineOutput(3, TH1F::Class());
  DefineOutput(4, TH1F::Class());
  DefineOutput(5, TNtuple::Class());
}

// //________________________________________________________________________
// void AliAnalysisTaskTotEt::ConnectInputData(Option_t *) 
// {
//   // Connect ESD or AOD here
//   // Called once

//   TTree* tree = dynamic_cast<TTree*> (GetInputData(0));
//   if (!tree) 
//     {
//       Printf("ERROR: Could not read chain from input slot 0");
//     } 
//   else 
//     {
//       // Disable all branches and enable only the needed ones
//       // The next two lines are different when data produced as AliESDEvent is read
//       //tree->SetBranchStatus("*", kFALSE);
//       //    tree->SetBranchStatus("Tracks.*", kTRUE);
//       tree->SetBranchStatus("CaloClusters.*", kTRUE);
      
//       AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
      
//       if (!esdH) 
// 	{
// 	  Printf("ERROR: Could not get ESDInputHandler");
// 	} 
//       else
// 	{
// 	  fESD = esdH->GetEvent();
// 	}
//     }
// }

//________________________________________________________________________
void AliAnalysisTaskTotEt::UserCreateOutputObjects()
{
  // Create histograms
  // Called once

  fHistEt = new TH1F("fHistEt", "Total E_{T} distribution", 1000, 0, 99);
  fHistEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistEt->SetMarkerStyle(kFullCircle);
  fHistEt->SetMarkerColor(kMagenta);

  fHistTotEtCells = new TH1F("fHistTotEtCells", "Total E_{T} distribution (sum of cells)", 1000, 0, 99);
  fHistTotEtCells->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistTotEtCells->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistTotEtCells->SetMarkerStyle(kFullCircle);
  fHistTotEtCells->SetMarkerColor(kBlue);

  fHistEtCells = new TH1F("fHistEtCells", "E_{T} distribution for single cells)", 100, 0.001, 0.1);
  fHistEtCells->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistEtCells->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistEtCells->SetMarkerStyle(kFullCircle);
  fHistEtCells->SetMarkerColor(kGreen);

  fHistTotE = new TH1F("fHistTotE", "Total Energy distribution", 1000, 0, 99);
  fHistTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistTotE->SetMarkerStyle(kFullCircle);
  fHistTotE->SetMarkerColor(kRed);
  
//   char title[128];
//   fHistCell = new TObjArray(17920);
//   fHistCell->SetOwner(kTRUE);
//   for(int i = 0; i < 17920; i++)
//     {
//       sprintf(title, "Cell_%d", i);
//       fHistCell->Add(new TH1F(title, title, 100, 0.001, 0.1));
//     }

  fEtNtuple = new TNtuple("fEtNtuple", "EtNtuple", "TotE:TotEt:TotEtCells");

}

//________________________________________________________________________
void AliAnalysisTaskTotEt::UserExec(Option_t *) 
{
  // Main loop
  // Called for each event

  Float_t totEt = 0;
  Float_t totEnergy = 0;
  Float_t totEtCells = 0;
  Float_t position[3];

  //AliVEvent *event = InputEvent();
  AliESDEvent *event = dynamic_cast<AliESDEvent*>(InputEvent());
  if (!event) {
     Printf("ERROR: Could not retrieve event");
     return;
  }

  // if(Entry()==0){
  //   AliESDEvent* esd = dynamic_cast<AliESDEvent*>(event);
  //   AliAODEvent* aod = dynamic_cast<AliAODEvent*>(event);
  //   if(esd){
  //     Printf("We are reading from ESD");
  //   }
  //   else if(aod){
  //     Printf("We are reading from AOD");
  //   }
  // }
  
  //  Printf("There are %d clusters in this event", event->GetNumberOfCaloClusters());
  //Printf("There are %d tracks in this event", event->GetNumberOfTracks());

  for(Int_t iCluster = 0; iCluster < event->GetNumberOfCaloClusters(); iCluster++) 
  // for(Int_t iCluster = 0; iCluster < 2; iCluster++) 
    {
      AliESDCaloCluster* cluster = event->GetCaloCluster(iCluster);
      if (!cluster) 
	{
	  Printf("ERROR: Could not get cluster %d", iCluster);
	  continue;
	}
      if(cluster->IsPHOS())
	{
	  totEnergy += cluster->E(); 

	  cluster->GetPosition(position);

	  float dist = TMath::Sqrt(position[0]*position[0] + position[1]*position[1]);

	  float eta = TMath::Log(TMath::Abs( TMath::Tan( 0.5 * (TMath::ATan(position[2]/dist) + TMath::Pi()/2) ) ) );
	  float theta = TMath::ATan(position[2]/dist)+TMath::Pi()/2;
	  //	  float eta = - TMath::Log(0.5*TMath::ATan(position[2]/dist));
	  //'	  cout << eta << endl;
	  //	  totEt += cluster->E() * (TMath::Sin(TMath::ATan(position[2]/dist)));
	  totEt += cluster->E() * TMath::Sin(theta);
	}
    }

  AliESDCaloCells* cells = event->GetPHOSCells();
  
  if(cells) 
    {
      Short_t cellNumber = 0;
      Double_t cellAmplitude = 0;
      Double_t cellTime = 0;
      
      for(Short_t iCell = 0; iCell < cells->GetNumberOfCells(); iCell++)
	{
	  if(!cells->GetCell(iCell, cellNumber, cellAmplitude, cellTime))
	    {
	      Printf("ERROR: Could not receive cell %d", iCell);
	      continue;
	    }
	  //	  if(cellAmplitude && event->GetNumberOfCaloClusters() == 0)
	  if(cellAmplitude)
	    {
	      //      cout << "Cell amplitude: " << cellAmplitude << endl;
	      totEtCells += cellAmplitude;
	      fHistEtCells->Fill(cellAmplitude);
	      //	      dynamic_cast<TH1F*>(fHistCell->At(cellNumber-1))->Fill(cellAmplitude);
	      cellAmplitude = 0;
	    }
	}
    }
  if(event->GetNumberOfCaloClusters() == 0)
    fCount++;

  fHistEt->Fill(totEt);
  fHistTotE->Fill(totEnergy);
  fHistTotEtCells->Fill(totEtCells);//- 6167.59004237288173);
  fEtNtuple->Fill(totEnergy, totEt, totEtCells);
   // Post output data.
  PostData(1, fHistEt);
  PostData(2, fHistTotEtCells);
  PostData(3, fHistEtCells);
  PostData(4, fHistTotE);
  PostData(5, fEtNtuple);
  //PostData(3, fHistCell);
}      

//________________________________________________________________________
void AliAnalysisTaskTotEt::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query

  fHistEt = dynamic_cast<TH1F*> (GetOutputData(1)); 
  fHistTotEtCells = dynamic_cast<TH1F*> (GetOutputData(2)); 
  fHistEtCells = dynamic_cast<TH1F*> (GetOutputData(3)); 
  fHistTotE = dynamic_cast<TH1F*> (GetOutputData(4)); 
  fEtNtuple = dynamic_cast<TNtuple*>(GetOutputData(5));

  if (!fHistEt) {
    Printf("ERROR: fHistEt not available");
    return;
  }
  if (!fHistTotEtCells) {
    Printf("ERROR: fHistTotEtCells not available");
    return;
  }
  if (!fHistEtCells) {
    Printf("ERROR: fHistEtCells not available");
    return;
  }
  if (!fEtNtuple) {
    Printf("ERROR: fEtNtuple not available");
    return;
  }
  
  TCanvas *c1 = new TCanvas("AliAnalysisTaskTotEt","Et",10,10,510,510);
  c1->cd(1)->SetLogy();
  fHistEt->Draw("E");

  //  TCanvas *c2 = new TCanvas("AliAnalysisTaskTotEt","EtTotCells",10,10,510,510);
  //  c2->cd(1)->SetLogy();
  //fHistTotEtCells->Draw("same");
  
  // TCanvas *c3 = new TCanvas("AliAnalysisTaskTotEt","EtCells",10,10,510,510);
  //  c2->cd(1)->SetLogy();
  //fHistEtCells->Draw("same");
}
