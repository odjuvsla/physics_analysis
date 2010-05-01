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

#include "AliAnalysisTaskTotEt.h"

#include <iostream>
#include "AliStack.h"

using namespace std;

ClassImp(AliAnalysisTaskTotEt)

//________________________________________________________________________
AliAnalysisTaskTotEt::AliAnalysisTaskTotEt(const char *name) :
        AliAnalysisTaskSE(name)
        ,fESD(0)
        ,fOutputList(0)
        ,fHistEt(0)
        ,fHistEtAcc(0)
        ,fHistTotE(0)
        ,fHistMult(0)
        ,fHistMCEt(0)
        ,fHistMCEtAcc(0)
        ,fHistMCTotE(0)
        ,fEtNtuple(0)
        ,fEtRecMCNtuple(0)
        ,fEtRecGeomNtuple(0)
        ,fEtMCGeomNtuple(0)
        ,fHistNeutralEt(0)
        ,fHistNeutralTotE(0)
        ,fHistNeutralMult(0)
        ,fHistMCNeutralEt(0)
        ,fHistMCNeutralEtAcc(0)
        ,fHistMCNeutralTotE(0)
        ,fNeutralEtNtuple(0)
        ,fNeutralEtRecMCNtuple(0)
        ,fNeutralEtRecGeomNtuple(0)
        ,fNeutralEtMCGeomNtuple(0)
        ,fHistChargedEt(0)
        ,fHistChargedEtAcc(0)
        ,fHistChargedTotE(0)
        ,fHistChargedMult(0)
        ,fHistMCChargedEt(0)
        ,fHistMCChargedEtAcc(0)
        ,fHistMCChargedTotE(0)
        ,fChargedEtNtuple(0)
        ,fChargedEtRecMCNtuple(0)
        ,fChargedEtRecGeomNtuple(0)
        ,fChargedEtMCGeomNtuple(0)
        ,fHistPhivsPtPos(0)
        ,fHistPhivsPtNeg(0)
        ,fHistEtRecvsEtMC(0)
        ,fHistTMDeltaR(0)
        ,fSumEtRec(0)
        ,fSumEtMC(0)
        ,fEtaCut(0.5)
        ,fEtaCutAcc(0.12)
        ,fPhiCutAccMin(260.*TMath::Pi()/180.)
        ,fPhiCutAccMax(320.*TMath::Pi()/180.)
        ,fVertexXCut(0.5)
        ,fVertexYCut(0.5)
        ,fVertexZCut(12.0)
        ,fIPxyCut(1.5)
        ,fIPzCut(1.5)
        ,fTriggerSelection(false)
        ,fCount(0)
        ,fkPhotonPdg(22)

{
    // Constructor

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

    fOutputList = new TList;

    fHistEt = new TH1F("fHistEt", "Total E_{T} distribution", 1000, 0.0001, 99);
    fHistEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistEt->SetMarkerStyle(kFullCircle);
    fHistEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistEt);
   
    fHistEtAcc = new TH1F("fHistEtAcc", "Total E_{T} distribution", 1000, 0.0001, 99);
    fHistEtAcc->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistEtAcc->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistEtAcc->SetMarkerStyle(kFullCircle);
    fHistEtAcc->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistEtAcc);
    
    fHistTotE = new TH1F("fHistTotE", "Total Energy distribution", 1000, 0, 99);
    fHistTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistTotE->SetMarkerStyle(kFullCircle);
    fHistTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistTotE);

    fHistMult = new TH1F("fHistMult", "Total Multiplicity", 100, 0, 199);
    fHistMult->GetXaxis()->SetTitle("N");
    fHistMult->GetYaxis()->SetTitle("Multiplicity");
    fHistMult->SetMarkerStyle(kFullCircle);
    fHistMult->SetMarkerColor(kRed);
    fOutputList->Add(fHistMult);

    fHistMCEt = new TH1F("fHistMCEt", "MC Total E_{T} distribution", 1000, 0.0001, 99);
    fHistMCEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCEt->SetMarkerStyle(kFullCircle);
    fHistMCEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCEt);

    fHistMCEtAcc = new TH1F("fHistMCEtAcc", "MC Total E_{T} distribution in acceptance", 1000, 0.0001, 99);
    fHistMCEtAcc->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCEtAcc->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCEtAcc->SetMarkerStyle(kFullCircle);
    fHistMCEtAcc->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCEtAcc);

    fHistMCTotE = new TH1F("fHistMCTotE", "MC Total Energy distribution", 1000, 0, 99);
    fHistMCTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCTotE->SetMarkerStyle(kFullCircle);
    fHistMCTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistMCTotE);

    fEtNtuple = new TNtuple("fEtNtuple", "EtNtuple", "TotE:TotEt");
    fOutputList->Add(fEtNtuple);

    fEtRecMCNtuple = new TNtuple("fEtRecMCNtuple", "EtRecMCNtuple", "RecTotE:RecTotEt:MCTotE:MCTotEt");
    fOutputList->Add(fEtRecMCNtuple);

    fEtRecGeomNtuple = new TNtuple("fEtRecGeomNtuple", "EtRecGeomNtuple", "RecE:RecEt:Eta:Phi");
    fOutputList->Add(fEtRecGeomNtuple);

    fEtMCGeomNtuple = new TNtuple("fEtMCGeomNtuple", "EtMCGeomNtuple", "MCE:MCEt:MCEta:MCPhi");
    fOutputList->Add(fEtMCGeomNtuple);

    fHistNeutralEt = new TH1F("fHistNeutralEt", "Total Neutral E_{T} distribution", 1000, 0.0001, 99);
    fHistNeutralEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistNeutralEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistNeutralEt->SetMarkerStyle(kFullCircle);
    fHistNeutralEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistNeutralEt);

    fHistNeutralTotE = new TH1F("fNeutralHistTotE", "Total Neutral Energy distribution", 1000, 0, 99);
    fHistNeutralTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistNeutralTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistNeutralTotE->SetMarkerStyle(kFullCircle);
    fHistNeutralTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistNeutralTotE);

    fHistNeutralMult = new TH1F("fHistNeutralMult", "Neutral Multiplicity", 100, 0, 199);
    fHistNeutralMult->GetXaxis()->SetTitle("N");
    fHistNeutralMult->GetYaxis()->SetTitle("Multiplicity");
    fHistNeutralMult->SetMarkerStyle(kFullCircle);
    fHistNeutralMult->SetMarkerColor(kRed);
    fOutputList->Add(fHistNeutralMult);

    fHistMCNeutralEt = new TH1F("fHistMCNeutralEt", "MC Total Neutral E_{T} distribution", 1000, 0.0001, 99);
    fHistMCNeutralEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCNeutralEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCNeutralEt->SetMarkerStyle(kFullCircle);
    fHistMCNeutralEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCNeutralEt);
 
    fHistMCNeutralEtAcc = new TH1F("fHistMCNeutralEtAcc", "MC Total Neutral E_{T} distribution", 1000, 0.0001, 99);
    fHistMCNeutralEtAcc->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCNeutralEtAcc->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCNeutralEtAcc->SetMarkerStyle(kFullCircle);
    fHistMCNeutralEtAcc->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCNeutralEtAcc);
    
    fHistMCNeutralTotE = new TH1F("fNeutralHistTotE", "MC Total Neutral Energy distribution", 1000, 0, 99);
    fHistMCNeutralTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCNeutralTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCNeutralTotE->SetMarkerStyle(kFullCircle);
    fHistMCNeutralTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistMCNeutralTotE);

    fNeutralEtNtuple = new TNtuple("fNeutralEtNtuple", "NeutralEtNtuple", "TotE:TotEt");
    fOutputList->Add(fNeutralEtNtuple);

    fNeutralEtRecMCNtuple = new TNtuple("fNeutralEtRecMCNtuple", "NeutralEtRecMCNtuple", "RecTotE:RecTotEt:MCTotE:MCTotEt");
    fOutputList->Add(fNeutralEtRecMCNtuple);

    fNeutralEtRecGeomNtuple = new TNtuple("fNeutralEtRecGeomNtuple", "NeutralEtRecGeomNtuple", "RecE:RecEt:Eta:Phi");
    fOutputList->Add(fNeutralEtRecGeomNtuple);

    fNeutralEtMCGeomNtuple = new TNtuple("fNeutralEtMCGeomNtuple", "NeutralEtMCGeomNtuple", "MCE:MCEt:MCEta:MCPhi");
    fOutputList->Add(fNeutralEtMCGeomNtuple);

    fHistChargedEt = new TH1F("fHistChargedEt", "Total Charged E_{T} distribution", 1000, 0.0001, 99);
    fHistChargedEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistChargedEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistChargedEt->SetMarkerStyle(kFullCircle);
    fHistChargedEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistChargedEt);

    fHistChargedEtAcc = new TH1F("fHistChargedEtAcc", "Total Charged E_{T} distribution", 1000, 0.0001, 99);
    fHistChargedEtAcc->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistChargedEtAcc->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistChargedEtAcc->SetMarkerStyle(kFullCircle);
    fHistChargedEtAcc->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistChargedEtAcc);

    fHistChargedTotE = new TH1F("fChargedHistTotE", "Total Charged Energy distribution", 1000, 0, 99);
    fHistChargedTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistChargedTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistChargedTotE->SetMarkerStyle(kFullCircle);
    fHistChargedTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistChargedTotE);

    fHistChargedMult = new TH1F("fHistChargedMult", "Charged Multiplicity", 100, 0, 199);
    fHistChargedMult->GetXaxis()->SetTitle("N");
    fHistChargedMult->GetYaxis()->SetTitle("Multiplicity");
    fHistChargedMult->SetMarkerStyle(kFullCircle);
    fHistChargedMult->SetMarkerColor(kRed);
    fOutputList->Add(fHistChargedMult);

    fHistMCChargedEt = new TH1F("fHistMCChargedEt", "MC Total Charged E_{T} distribution", 1000, 0.0001, 99);
    fHistMCChargedEt->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCChargedEt->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCChargedEt->SetMarkerStyle(kFullCircle);
    fHistMCChargedEt->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCChargedEt);

    fHistMCChargedEtAcc = new TH1F("fHistMCChargedEtAcc", "MC Total Charged E_{T} distribution within acceptance", 1000, 0.0001, 99);
    fHistMCChargedEtAcc->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCChargedEtAcc->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCChargedEtAcc->SetMarkerStyle(kFullCircle);
    fHistMCChargedEtAcc->SetMarkerColor(kMagenta);
    fOutputList->Add(fHistMCChargedEtAcc);
    
    fHistMCChargedTotE = new TH1F("fChargedHistTotE", "MC Total Charged Energy distribution", 1000, 0, 99);
    fHistMCChargedTotE->GetXaxis()->SetTitle("E_{T} (GeV/c^{2})");
    fHistMCChargedTotE->GetYaxis()->SetTitle("dN/dE_{T} (c^{2}/GeV)");
    fHistMCChargedTotE->SetMarkerStyle(kFullCircle);
    fHistMCChargedTotE->SetMarkerColor(kRed);
    fOutputList->Add(fHistMCChargedTotE);

    fChargedEtNtuple = new TNtuple("fChargedEtNtuple", "ChargedEtNtuple", "TotE:TotEt");
    fOutputList->Add(fChargedEtNtuple);

    fChargedEtRecMCNtuple = new TNtuple("fChargedEtRecMCNtuple", "ChargedEtRecMCNtuple", "RecTotE:RecTotEt:MCTotE:MCTotEt");
    fOutputList->Add(fChargedEtRecMCNtuple);

    fChargedEtRecGeomNtuple = new TNtuple("fChargedEtRecGeomNtuple", "ChargedEtRecGeomNtuple", "RecE:RecEt:Eta:Phi");
    fOutputList->Add(fChargedEtRecGeomNtuple);

    fChargedEtMCGeomNtuple = new TNtuple("fChargedEtMCGeomNtuple", "ChargedEtMCGeomNtuple", "MCE:MCEt:MCEta:MCPhi");
    fOutputList->Add(fChargedEtMCGeomNtuple);

    fHistPhivsPtPos = new TH2F("fHistPhivsPtPos", "Phi vs pT of positively charged tracks in PHOS", 	200, 0, 2*TMath::Pi(), 2000, 0, 100);
    fOutputList->Add(fHistPhivsPtPos);
    fHistPhivsPtNeg = new TH2F("fHistPhivsPtNeg", "Phi vs pT of negatively charged tracks in PHOS", 	200, 0, 2*TMath::Pi(), 2000, 0, 100);
    fOutputList->Add(fHistPhivsPtNeg);

    fHistEtRecvsEtMC = new TH2F("fHistEtRecvsEtMC", "Reconstructed E_{t} vs MC E_{t}", 1000, 0.000, 100, 1000, 0.0001, 100);
    fOutputList->Add(fHistEtRecvsEtMC);

    fHistTMDeltaR = new TH1F("fHistTMDeltaR", "#Delta R for PHOS clusters", 200, 0, 50);
    fOutputList->Add(fHistTMDeltaR);

}

//________________________________________________________________________
void AliAnalysisTaskTotEt::UserExec(Option_t *)
{
    // Main loop
    // Called for each event

    Float_t totEt = 0;
    Float_t totEnergy = 0;

    Float_t totNeutralEt = 0;
    Float_t totNeutralEnergy = 0;

    Float_t totChargedEt = 0;
    Float_t totChargedEtAcc = 0;
    Float_t totChargedEnergy = 0;
    Float_t totChargedEnergyAcc = 0;

    Float_t totNeutralEtCells = 0;
    Float_t position[3];

    Float_t mcTotEt = 0;
    Float_t mcTotEtAcc = 0;
    Float_t mcTotEnergy = 0;
    Float_t mcTotNeutralEnergy = 0;
    Float_t mcTotNeutralEt = 0;
    Float_t mcTotNeutralEtAcc = 0;
    Float_t mcTotChargedEnergy = 0;
    Float_t mcTotChargedEt = 0;
    Float_t mcTotChargedEtAcc = 0;

    //AliVEvent *event = InputEvent();
    AliESDEvent *event = dynamic_cast<AliESDEvent*>(InputEvent());
    if (!event) {
        Printf("ERROR: Could not retrieve event");
        return;
    }

    //fCount++;

    if (fTriggerSelection && !event->IsTriggerClassFired("CINT1B-ABCE-NOPF-ALL")) return;

    AliMCEvent* mcEvent = MCEvent();
    Double_t totZ = 0;
    Double_t totY = 0;
    Double_t totX = 0;
    if (mcEvent)
    {

        //     Printf("Got MC event with %d particles!", mcEvent->GetNumberOfTracks());

        AliStack *stack = mcEvent->Stack();

        //Int_t nPrim = stack->GetNprimary();
        Int_t nPrim = stack->GetNtrack();
 //       Printf("Number of particles in MC event: %d", nPrim);

        for (Int_t iPart = 0; iPart < nPrim; iPart++)
        {
            //Printf("Got %d particles!", mcEvent->GetNumberOfTracks());
            //AliMCParticle *part = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iPart));

            TParticle *part = stack->Particle(iPart);
            //part->Print();
            //Printf("Daugthers: %d %d", part->GetDaughter(0), part->GetDaughter(1)) ;
            //if(part->GetDaughter(0) > 0) Printf("PDG: %s %s", stack->Particle(part->GetDaughter(0))->GetPDG(0)->GetName(), stack->Particle(part->GetDaughter(1))->GetPDG(0)->GetName());
            //if(part->GetDaughter(0) == -1 && part->GetDaughter(1) == -1) {


            TParticlePDG *pc = part->GetPDG(0);

	    if(!stack->IsPhysicalPrimary(iPart)) continue;
	    if(TMath::Abs(pc->Charge()) != 3 && pc->Charge() != 0)continue;
	    //part->Print();
	    totZ += part->Pz();
            totY += part->Py();
            totX += part->Px();
            //Printf("particle index: %d, energy: %f, mother: %d", iPart,  part->Energy(), part->GetMother(0));

//	    part->Print();

            if (!part)
            {
                Printf("ERROR: Could not receive particle %d", iPart);
                continue;
            }

            if (TMath::Abs(part->Eta()) < fEtaCut)
            {
                fCount++;

                TParticlePDG *pdgCode =  part->GetPDG(0);


                mcTotEnergy += part->Energy();
                //mcTotEt += part->Energy() * TMath::Sin(part->Theta());
                //if (pdgCode->PdgCode() == fkPhotonPdg || pdgCode->PdgCode() == 111)

                if (pdgCode->Charge() == 0)
                {
                    //             mcTotEt += part->Energy()*TMath::Sin(part->Theta());
                    mcTotNeutralEnergy += part->Energy();
                    mcTotNeutralEt += part->Energy()*TMath::Sin(part->Theta());
                    if (TMath::Abs(part->Eta()) < fEtaCutAcc && part->Phi() < fPhiCutAccMax && part->Phi() > fPhiCutAccMin)
                    {
                        mcTotNeutralEtAcc += part->Energy()*TMath::Sin(part->Theta());
                        mcTotEtAcc += part->Energy()*TMath::Sin(part->Theta());
                    }
                }
                else if (pdgCode->Charge() != 0)
                {
                    //          mcTotEt += part->Energy()*TMath::Sin(part->Theta());
                    mcTotChargedEnergy += part->Energy();
                    mcTotChargedEt += part->Energy()*TMath::Sin(part->Theta());
                    if (TMath::Abs(part->Eta()) < fEtaCutAcc && part->Phi() < fPhiCutAccMax && part->Phi() > fPhiCutAccMin)
                    {
                        mcTotChargedEtAcc += part->Energy()*TMath::Sin(part->Theta());
                        mcTotEtAcc += part->Energy()*TMath::Sin(part->Theta());
                    }
                }
                //    if (ParticleInPHOS(part))
                {
                }
                // else if (ParticleInEMCAL(part))
                {
                }
                // else
                {
                }
            }
        }
     //   Printf("Number of primaries: %d", nnn);
    }
  //  Printf("Total mom: px = %f, py = %f, pz = %f", totX, totY, totZ);
    Int_t chargedMultiplicity = 0;

    for (Int_t iTrack = 0; iTrack < event->GetNumberOfTracks(); iTrack++)
    {
        AliVParticle *track = event->GetTrack(iTrack);
        if (!track)
        {
            Printf("ERROR: Could not get track %d", iTrack);
            continue;
        }



//      if(TMath::Abs(track->Eta()) < fEtaCut && track->E() > 0.5)
        Int_t nItsClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(0);
        Int_t nTPCClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(1);

        if (TMath::Abs(track->Eta()) < fEtaCut && CheckGoodVertex(track) && nItsClusters > 3 && nTPCClusters > 15)
        {
            totChargedEnergy += track->E();
            totChargedEt +=  track->E() * TMath::Sin(track->Theta());
            chargedMultiplicity++;

            if (TMath::Abs(track->Eta()) < fEtaCutAcc && track->Phi() < fPhiCutAccMax && track->Phi() > fPhiCutAccMin)
            {
       //         Printf("Track in acceptance: eta = %f, phi = %f", track->Eta(), track->Phi());
                totChargedEtAcc += track->E()*TMath::Sin(track->Theta());
            }
        }

        Double_t phi = track->Phi();
        Double_t pt = track->Pt();
        if (TrackHitsPHOS(track, event->GetMagneticField()))
        {
            if (track->Charge() > 0) fHistPhivsPtPos->Fill(phi,pt);
            else fHistPhivsPtNeg->Fill(phi, pt);
        }
    }

    Int_t neutralMultiplicity = 0;

    for (Int_t iCluster = 0; iCluster < event->GetNumberOfCaloClusters(); iCluster++)
    {
        AliESDCaloCluster* cluster = event->GetCaloCluster(iCluster);
        if (!cluster)
        {
            Printf("ERROR: Could not get cluster %d", iCluster);
            continue;
        }

        if (!cluster->IsPHOS()) continue;
        Float_t pos[3];
	TVector3 cp(pos);
        cluster->GetPosition(pos);
        if (pos[0] < -(32.0*2.2)) continue; //Ensure that modules 0 and 1 are not used
       // if(cp.Phi() < 260.*TMath::Pi()/180.) continue;
        fHistTMDeltaR->Fill(cluster->GetEmcCpvDistance());
        if (cluster->GetEmcCpvDistance() < 10.0)
        {
            continue;
            //AliVParticle *matchedTrack = event->GetTrack(cluster->GetTrackMatched());
// 	    if(CheckGoodVertex(matchedTrack))
// 	    {
// 	       totChargedEnergy +=  matchedTrack->E();;
// 	       totChargedEt += matchedTrack->E()*TMath::Sin(matchedTrack);
// 	    }
        }
        if (cluster->E() >  0.5 && cluster->GetNCells() == 1) continue;
        totNeutralEnergy += cluster->E();
        cluster->GetPosition(position);

        float dist = TMath::Sqrt(position[0]*position[0] + position[1]*position[1]);

        float eta = TMath::Log(TMath::Abs( TMath::Tan( 0.5 * (TMath::ATan(position[2]/dist) + TMath::Pi()/2) ) ) );
        float theta = TMath::ATan(position[2]/dist)+TMath::Pi()/2;
        totNeutralEt += cluster->E() * TMath::Sin(theta);
        neutralMultiplicity++;
    }



//    totEt = CorrectForCaloAcceptance(totNeutralEt) + totChargedEt;
    //totEnergy =CorrectForCaloAcceptance(totNeutralEnergy) + totChargedEnergy;
    totEt = totNeutralEt + totChargedEt;
    totEnergy =totNeutralEnergy + totChargedEnergy;
    mcTotEt = mcTotChargedEt + mcTotNeutralEt;


    fSumEtMC += mcTotEt;
    fSumEtRec += totEt;
    fSumEtRecAcc += totChargedEtAcc+totNeutralEt;

    fHistEt->Fill(totEt);
    fHistEtAcc->Fill(totChargedEtAcc+totNeutralEt);
    fHistTotE->Fill(totEnergy);
    fHistMult->Fill(chargedMultiplicity+neutralMultiplicity);
    fHistMCEt->Fill(mcTotEt);
    fHistMCEtAcc->Fill(mcTotEtAcc);
    fHistMCTotE->Fill(mcTotEnergy);

    fHistNeutralEt->Fill(totNeutralEt);
    fHistNeutralTotE->Fill(totNeutralEnergy);
    fHistNeutralMult->Fill(neutralMultiplicity);
    fHistMCNeutralEt->Fill(mcTotNeutralEt);
    fHistMCNeutralEtAcc->Fill(mcTotNeutralEtAcc);
    fHistMCNeutralTotE->Fill(mcTotEnergy);

    fHistChargedEt->Fill(totChargedEt);
    fHistChargedEtAcc->Fill(totChargedEtAcc);
    fHistChargedTotE->Fill(totChargedEnergy);
    fHistChargedMult->Fill(chargedMultiplicity);
    fHistMCChargedEt->Fill(mcTotChargedEt);
    fHistMCChargedEtAcc->Fill(mcTotChargedEtAcc);
    fHistMCChargedTotE->Fill(mcTotChargedEnergy);

    fEtNtuple->Fill(totEnergy, totEt);
    //fEtRecGeomNtuple->Fill(totEnergy, totEt,
    if (mcEvent)
    {
        fEtRecMCNtuple->Fill(totEnergy, totEt, mcTotEnergy, mcTotEt);
    }

    fHistEtRecvsEtMC->Fill(totChargedEtAcc + totNeutralEnergy, mcTotEt);

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
    fHistEt = dynamic_cast<TH1F*> (fOutputList->At(0));

    Printf("Correction factor: %f", fSumEtMC/fSumEtRecAcc);

    TCanvas *c1 = new TCanvas("AliAnalysisTaskTotEt","Et",10,10,510,510);
    c1->cd(1)->SetLogy();
    fHistEt->Draw("E");
}

Float_t AliAnalysisTaskTotEt::CorrectForCaloAcceptance(Float_t energy)
{
    return energy;
    Float_t etaFactor = 1.0/0.12;
    Float_t phiFactor = 360.0/60.0;
    Float_t branchCorrFactor = 1.0/(1.0/12.0);
    return energy*etaFactor*phiFactor*branchCorrFactor;
}


bool AliAnalysisTaskTotEt::CheckGoodVertex(AliVParticle* track)
{
    Float_t bxy = 999.;
    Float_t bz = 999.;
    dynamic_cast<AliESDtrack*>(track)->GetImpactParametersTPC(bxy,bz);

    return TMath::Abs(track->Xv()) < fVertexXCut && TMath::Abs(track->Yv()) < fVertexYCut && TMath::Abs(track->Zv()) < fVertexZCut && TMath::Abs(bxy) < fIPxyCut && TMath::Abs(bz) < fIPzCut;;
}

bool
AliAnalysisTaskTotEt::ParticleInPHOS(AliMCParticle *particle)
{
    return false;
}

bool
AliAnalysisTaskTotEt::ParticleInEMCAL(AliMCParticle *particle)
{
    return false;
}

bool AliAnalysisTaskTotEt::TrackHitsPHOS(AliVParticle* track, Double_t magField)
{
    AliESDtrack *esdTrack = dynamic_cast<AliESDtrack*>(track);
    // Printf("Propagating track: eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());

    Bool_t prop = esdTrack->PropagateTo(460.0, magField);

    //if(prop)Printf("Track propagated, eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());
    return prop&& TMath::Abs(esdTrack->Eta()) < 0.12 && esdTrack->Phi() > 260.*TMath::Pi()/180. && esdTrack->Phi() < 320.*TMath::Pi()/180.;

}











