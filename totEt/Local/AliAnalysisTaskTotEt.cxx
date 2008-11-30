#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TMath.h"

#include "/home/odjuvsla/Workspace/alice/aliroot/ANALYSIS/AliAnalysisTask.h"
#include "/home/odjuvsla/Workspace/alice/aliroot/ANALYSIS/AliAnalysisManager.h"

#include "/home/odjuvsla/Workspace/alice/aliroot/STEER/AliESDEvent.h"
#include "/home/odjuvsla/Workspace/alice/aliroot/STEER/AliESDCaloCluster.h"
#include "/home/odjuvsla/Workspace/alice/aliroot/STEER/AliESDCaloCells.h"
#include "/home/odjuvsla/Workspace/alice/aliroot/STEER/AliESDInputHandler.h"

#include "AliAnalysisTaskTotEt.h"

ClassImp(AliAnalysisTaskTotEt)

//________________________________________________________________________
AliAnalysisTaskTotEt::AliAnalysisTaskTotEt(const char *name) : 
AliAnalysisTask(name, ""), 
  fESD(0), 
  fHistEt(0), 
  fHistEtCells(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  DefineOutput(0, TH1F::Class());
  //  DefineOutput(1, TH1F::Class());
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

  fHistEt = new TH1F("fHistEt", "E_{T} distribution", 30, 0, 4);
  fHistEt->GetXaxis()->SetTitle("E_{T} (GeV/c^2)");
  fHistEt->GetYaxis()->SetTitle("dN/dE_{T} (c^2/GeV)");
  fHistEt->SetMarkerStyle(kFullCircle);
  fHistEt->SetMarkerColor(kMagenta);

  fHistEtCells = new TH1F("fHistEtCells", "E_{T} distribution (sum of cells)", 15, 0.1, 3.1);
  fHistEtCells->GetXaxis()->SetTitle("E_{T} (GeV/c^2)");
  fHistEtCells->GetYaxis()->SetTitle("dN/dE_{T} (c^2/GeV)");
  fHistEtCells->SetMarkerStyle(kFullCircle);
  fHistEtCells->SetMarkerColor(kBlue);
  
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
//   AliESDCaloCells* cells = fESD->GetPHOSCells();
  
//   Short_t cellNumber = 0;
//   Double_t cellAmplitude = 0;
//   Double_t cellTime = 0;
  
//   for(Short_t iCell = 0; iCell < cells->GetNumberOfCells(); iCell++)
//     {
//       if(!cells->GetCell(iCell, cellNumber, cellAmplitude, cellTime))
// 	{
// 	  Printf("ERROR: Could not receive cell %d", iCell);
// 	  continue;
// 	}
   
//       totEtCells += cellAmplitude;
//     }

  fHistEt->Fill(totEt);
  //  fHistEtCells->Fill(totEtCells);

   // Post output data.
  PostData(0, fHistEt);
  //  PostData(1, fHistEtCells);
}      

//________________________________________________________________________
void AliAnalysisTaskTotEt::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query

  fHistEt = dynamic_cast<TH1F*> (GetOutputData(0)); 
  //  fHistEtCells = dynamic_cast<TH1F*> (GetOutputData(1)); 
  if (!fHistEt) {
    Printf("ERROR: fHistEt not available");
    return;
  }
//   if (!fHistEtCells) {
//     Printf("ERROR: fHistEtCells not available");
//     return;
//   }
  
  TCanvas *c1 = new TCanvas("AliAnalysisTaskTotEt","Et",10,10,510,510);
  c1->cd(1)->SetLogy();
  fHistEt->DrawCopy("E");

  //TCanvas *c2 = new TCanvas("AliAnalysisTaskTotEt","EtCells",10,10,510,510);
  //c2->cd(1)->SetLogy();
  //  fHistEtCells->DrawCopy("same");
}
