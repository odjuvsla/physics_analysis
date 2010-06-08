#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TObjArray.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"

#include "AliESDEvent.h"
#include "AliAODEvent.h"
#include "AliVEvent.h"
#include "AliVTrack.h"
#include "AliVParticle.h"
#include "AliESDCaloCluster.h"
#include "AliESDCaloCells.h"
#include "AliESDtrack.h"
#include "AliESDInputHandler.h"
#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "TDatabasePDG.h"
#include "AliAnalysisTaskTotEt.h"
#include "AliAnalysisEtReconstructedPhos.h"
#include "AliAnalysisEtMonteCarloPhos.h"

#include <iostream>
#include "AliStack.h"

using namespace std;

ClassImp(AliAnalysisTaskTotEt)

//________________________________________________________________________
AliAnalysisTaskTotEt::AliAnalysisTaskTotEt(const char *name) :
        AliAnalysisTaskSE(name)
        ,fESD(0)
        ,fOutputList(0)
        ,fPhosRecAnalysis(0)
        ,fPhosMCAnalysis(0)
        ,fHistEtRecvsEtMC(0)
        ,fTriggerSelection(false)
        ,fCount(0)
        ,fkPhotonPdg(22)
        ,fkProtonMass(.938)
        ,fPdgDB(0)
        ,fRecEventVars(0)
        ,fSimEventVars(0)
        ,fRecParticleArray(0)
        ,fSimParticleArray(0)
{
    // Constructor

    fPhosRecAnalysis = new AliAnalysisEtReconstructedPhos();
    fPhosRecAnalysis->Init();
    fPhosMCAnalysis = new AliAnalysisEtMonteCarloPhos();
    fPhosMCAnalysis->Init();

    fPdgDB = new TDatabasePDG();


    // Define input and output slots here
    // Input slot #0 works with a TChain
    DefineInput(0, TChain::Class());
    // Output slot #1 writes into a TH1 container

    DefineOutput(1, TList::Class());

}


//________________________________________________________________________
void AliAnalysisTaskTotEt::UserCreateOutputObjects()
{
    // Create histograms
    // Called once
    fPhosMCAnalysis->CreateHistograms();
    fPhosRecAnalysis->CreateHistograms();
    fOutputList = new TList;
    fPhosRecAnalysis->FillOutputList(fOutputList);
    fPhosMCAnalysis->FillOutputList(fOutputList);
    fHistEtRecvsEtMC = new TH2F("fHistEtRecvsEtMC", "Reconstructed E_{t} vs MC E_{t}", 1000, 0.000, 100, 1000, 0.0001, 100);
    fOutputList->Add(fHistEtRecvsEtMC);
}

//________________________________________________________________________
void AliAnalysisTaskTotEt::UserExec(Option_t *)
{
    AliESDEvent *event = dynamic_cast<AliESDEvent*>(InputEvent());
    if (!event) {
        Printf("ERROR: Could not retrieve event");
        return;
    }

    fPhosRecAnalysis->AnalyseEvent(event);

    AliMCEvent* mcEvent = MCEvent();
    if (mcEvent)
    {
        fPhosMCAnalysis->AnalyseEvent(mcEvent);
    }

    fHistEtRecvsEtMC->Fill(fPhosRecAnalysis->GetTotEtAcc(), fPhosMCAnalysis->GetTotEt());

// Post output data.
    PostData(1, fOutputList);

}

//________________________________________________________________________
void AliAnalysisTaskTotEt::Terminate(Option_t *)
{
    // Draw result to the screen
    // Called once at the end of the query

    fOutputList = dynamic_cast<TList*> (GetOutputData(1));
    if (!fOutputList) {
        printf("ERROR: Output list not available\n");
        return;
    }
}



