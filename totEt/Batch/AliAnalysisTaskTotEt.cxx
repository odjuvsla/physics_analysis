#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TObjArray.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"

#include "AliESDEvent.h"
#include "AliESDCaloCluster.h"
#include "AliESDCaloCells.h"
#include "AliESDInputHandler.h"

#include "AliAnalysisTaskTotEt.h"

#include <iostream>

using namespace std;

ClassImp(AliAnalysisTaskTotEt)

//________________________________________________________________________
AliAnalysisTaskTotEt::AliAnalysisTaskTotEt(const char *name) : 
AliAnalysisTask(name, ""), 
  fESD(0), 
  fHistEt(0), 
  fHistTotEtCells(0)

{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  DefineOutput(0, TH1F::Class());
  DefineOutput(1, TH1F::Class());
}

//________________________________________________________________________
void AliAnalysisTaskTotEt::ConnectInputData(Option_t *) 
{
  // Connect ESD or AOD here
  // Called once

  TTree* tree = dynamic_cast<TTree*> (GetInputData(0));
  if (!tree) 
    {
      Printf("ERROR: Could not read chain from input slot 0");
    } 
  else 
    {
      // Disable all branches and enable only the needed ones
      // The next two lines are different when data produced as AliESDEvent is read
      //tree->SetBranchStatus("*", kFALSE);
      //    tree->SetBranchStatus("Tracks.*", kTRUE);
      tree->SetBranchStatus("CaloClusters.*", kTRUE);
      
      AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
      
      if (!esdH) 
	{
	  Printf("ERROR: Could not get ESDInputHandler");
	} 
      else
	{
	  fESD = esdH->GetEvent();
	}
    }
}

//________________________________________________________________________
void AliAnalysisTaskTotEt::CreateOutputObjects()
{
  // Create histograms
  // Called once

  fHistEt = new TH1F("fHistEt", "Total E_{T} distribution", 100, 0, 20);
  fHistEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistEt->SetMarkerStyle(kFullCircle);
  fHistEt->SetMarkerColor(kMagenta);

  fHistTotEtCells = new TH1F("fHistTotEtCells", "Total E_{T} distribution (sum of cells)", 200, 150, 249);
  fHistTotEtCells->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
  fHistTotEtCells->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
  fHistTotEtCells->SetMarkerStyle(kFullCircle);
  fHistTotEtCells->SetMarkerColor(kBlue);
}

//________________________________________________________________________
void AliAnalysisTaskTotEt::Exec(Option_t *) 
{
  // Main loop
  // Called for each event

  Float_t totEt = 0;
  Float_t totEtCells = 0;

  if (!fESD) 
    {
      Printf("ERROR: fESD not available");
      return;
    }
  
  //  Printf("There are %d clusters in this event", fESD->GetNumberOfCaloClusters());
  //Printf("There are %d tracks in this event", fESD->GetNumberOfTracks());

  for(Int_t iCluster = 0; iCluster < fESD->GetNumberOfCaloClusters(); iCluster++) 
  // for(Int_t iCluster = 0; iCluster < 2; iCluster++) 
    {
      AliESDCaloCluster* cluster = fESD->GetCaloCluster(iCluster);
      if (!cluster) 
	{
	  Printf("ERROR: Could not receive cluster %d", iCluster);
	  continue;
	}
      if(cluster->IsPHOS())
	{
	  totEt += cluster->E(); //* TMath::Sin(TMath::ATan((cluster->fGlobalPos)[2]/460.0));
	}
    }
  AliESDCaloCells* cells = fESD->GetPHOSCells();
  
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
	  if(cellAmplitude && fESD->GetNumberOfCaloClusters() >0)
	    {
	      //      cout << "Cell amplitude: " << cellAmplitude << endl;
	      totEtCells += cellAmplitude;
	    }
	}
    }
  if(fESD->GetNumberOfCaloClusters() == 0)
  //    cout << "Total cell amplitude: " << totEtCells << endl;
  fHistEt->Fill(totEt);
  fHistTotEtCells->Fill(totEtCells);//- 6167.59004237288173);

   // Post output data.
  PostData(0, fHistEt);
  PostData(1, fHistTotEtCells);
}      

//________________________________________________________________________
void AliAnalysisTaskTotEt::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query

  fHistEt = dynamic_cast<TH1F*> (GetOutputData(0)); 
  fHistTotEtCells = dynamic_cast<TH1F*> (GetOutputData(1)); 
  
//   cout << fCount << endl;
//   if (!fHistEt) {
//     Printf("ERROR: fHistEt not available");
//     return;
//   }
//   if (!fHistTotEtCells) {
//     Printf("ERROR: fHistTotEtCells not available");
//     return;
//   }
//   if (!fHistEtCells) {
//     Printf("ERROR: fHistEtCells not available");
//     return;
//   }
  
//   TCanvas *c1 = new TCanvas("AliAnalysisTaskTotEt","Et",10,10,510,510);
//   c1->cd(1)->SetLogy();
//   fHistEt->Draw("E");

//   //  TCanvas *c2 = new TCanvas("AliAnalysisTaskTotEt","EtTotCells",10,10,510,510);
//   //  c2->cd(1)->SetLogy();
//   fHistTotEtCells->Draw("same");
  
//   // TCanvas *c3 = new TCanvas("AliAnalysisTaskTotEt","EtCells",10,10,510,510);
//   //  c2->cd(1)->SetLogy();
//   fHistEtCells->Draw("same");
}
