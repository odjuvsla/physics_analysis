
#include "AliAnalysisEtReconstructed.h"
#include "AliAnalysisEtCuts.h"
#include "AliESDtrack.h"
#include "AliESDCaloCluster.h"
#include "TVector3.h"
#include "AliVEvent.h"
#include "AliESDEvent.h"
#include "AliVParticle.h"
#include <iostream>
#include "TH2F.h"
#include "TDatabasePDG.h"
#include <TList.h>

AliAnalysisEtReconstructed::AliAnalysisEtReconstructed() :
        AliAnalysisEt()
        ,fNTpcClustersCut(EtReconstructedCuts::kNTpcClustersCut)
        ,fNItsClustersCut(EtReconstructedCuts::knItsClustersCut)
        ,fTrackDistanceCut(0)
        ,fPidCut(EtReconstructedCuts::kPidCut)
        ,fClusterType(0)
        ,fHistChargedPionEnergyDeposit(0)
        ,fHistProtonEnergyDeposit(0)
        ,fHistAntiProtonEnergyDeposit(0)
        ,fHistChargedKaonEnergyDeposit(0)
{

}

Int_t AliAnalysisEtReconstructed::AnalyseEvent(AliVEvent* ev)
{
    ResetEventValues();
    AliESDEvent *event = dynamic_cast<AliESDEvent*>(ev);

    Double_t protonMass = fPdgDB->GetParticle("proton")->Mass();
    Double_t kaonMass = fPdgDB->GetParticle("K+")->Mass();

    for (Int_t iTrack = 0; iTrack < event->GetNumberOfTracks(); iTrack++)
    {
        AliVParticle *track = event->GetTrack(iTrack);
        if (!track)
        {
            Printf("ERROR: Could not get track %d", iTrack);
            continue;
        }

        fMultiplicity++;

        Int_t nItsClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(0);
        Int_t nTPCClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(1);

        Float_t massPart = 0;

        Int_t maxpid = -1;



        const Double_t *pidWeights = track->PID();
        Double_t maxpidweight = 0;
        if (pidWeights)
        {
            for (Int_t p =0; p < AliPID::kSPECIES; p++)
            {
                if (pidWeights[p] > maxpidweight)
                {
                    maxpidweight = pidWeights[p];
                    maxpid = p;
                }
            }
            if (maxpid == AliPID::kProton)
            {
                //By definition of ET
                massPart = -protonMass*track->Charge();
            }
        }

        Double_t et = track->E() * TMath::Sin(track->Theta()) + massPart;

        if (TMath::Abs(track->Eta()) < fEtaCut && CheckGoodVertex(track) && nItsClusters > fNItsClustersCut && nTPCClusters > fNTpcClustersCut)
        {
            fTotChargedEt +=  et;
            fChargedMultiplicity++;

            if (maxpid != -1)
            {
                if (maxpid == AliPID::kPion)
                {
                    fProtonEt += et;
                }
                if (maxpid == AliPID::kKaon)
                {
                    fChargedKaonEt += et;
                }
                if (maxpid == AliPID::kMuon)
                {
                    fMuonEt += et;
                }
                if (maxpid == AliPID::kElectron)
                {
                    fElectronEt += et;
                }
            }

            if (TMath::Abs(track->Eta()) < fEtaCutAcc && track->Phi() < fPhiCutAccMax && track->Phi() > fPhiCutAccMin)
            {
                fTotChargedEtAcc += track->E()*TMath::Sin(track->Theta()) + massPart;
                if (maxpid != -1)
                {
                    if (maxpid == AliPID::kPion)
                    {
                        fProtonEtAcc += et;
                    }
                    if (maxpid == AliPID::kKaon)
                    {
                        fChargedKaonEtAcc += et;
                    }
                    if (maxpid == AliPID::kMuon)
                    {
                        fMuonEtAcc += et;
                    }
                    if (maxpid == AliPID::kElectron)
                    {
                        fElectronEtAcc += et;
                    }
                }
            }
        }

        Double_t phi = track->Phi();
        Double_t pt = track->Pt();
        if (TrackHitsCalorimeter(track, event->GetMagneticField()))
        {
            if (track->Charge() > 0) fHistPhivsPtPos->Fill(phi,pt);
            else fHistPhivsPtNeg->Fill(phi, pt);
        }
    }

    for (Int_t iCluster = 0; iCluster < event->GetNumberOfCaloClusters(); iCluster++)
    {
        AliESDCaloCluster* cluster = event->GetCaloCluster(iCluster);
        if (!cluster)
        {
            Printf("ERROR: Could not get cluster %d", iCluster);
            continue;
        }

        if (cluster->GetType() != fClusterType) continue;

        if (cluster->E() < fClusterEnergyCut) continue;
        Float_t pos[3];
        TVector3 cp(pos);
        cluster->GetPosition(pos);
        //if (pos[0] < -(32.0*2.2)) continue; //Ensure that modules 0 and 1 are not used
        // if(cp.Phi() < 260.*TMath::Pi()/180.) continue;
        fHistTMDeltaR->Fill(cluster->GetEmcCpvDistance());
        if (cluster->GetEmcCpvDistance() < fTrackDistanceCut)
        {

            if (cluster->GetNTracksMatched() == 1)
            {
                AliVTrack *track = event->GetTrack(cluster->GetTrackMatchedIndex());
                const Double_t *pidWeights = track->PID();
                
		Double_t maxpidweight = 0;
		Int_t maxpid = 0;
                
		if (pidWeights)
                {
                    for (Int_t p =0; p < AliPID::kSPECIES; p++)
                    {
                        if (pidWeights[p] > maxpidweight)
                        {
                            maxpidweight = pidWeights[p];
                            maxpid = p;
                        }
                    }
                    if(maxpidweight > fPidCut)
		    {
		       if(maxpid == AliPID::kProton)
		       {
			  if(track->Charge() == 1)
			  {
			     fHistProtonEnergyDeposit->Fill(cluster->E(), track->E());
			  }
			  else if(track->Charge() == -1)
			  {
			     fHistAntiProtonEnergyDeposit->Fill(cluster->E(), track->E());
			  }
		       }
		       else if(maxpid == AliPID::kPion)
		       {
			  fHistChargedPionEnergyDeposit->Fill(cluster->E(), track->E());
		       }
		       else if(maxpid == AliPID::kKaon)
		       {
			  fHistChargedKaonEnergyDeposit->Fill(cluster->E(), track->E());
		       }
		    }
                }
            }

            continue;
            //AliVParticle *matchedTrack = event->GetTrack(cluster->GetTrackMatched());
// 	    if(CheckGoodVertex(matchedTrack))
// 	    {
// 	       totChargedEnergy +=  matchedTrack->E();;
// 	       totChargedEt += matchedTrack->E()*TMath::Sin(matchedTrack);
// 	    }
        }

        if (cluster->E() >  fSingleCellEnergyCut && cluster->GetNCells() == EtCommonCuts::kSingleCell) continue;

        cluster->GetPosition(pos);

        // TODO: replace with TVector3, too lazy now...

        float dist = TMath::Sqrt(pos[0]*pos[0] + pos[1]*pos[1]);

        float theta = TMath::ATan(pos[2]/dist)+TMath::Pi()/2;
        // float eta = TMath::Log(TMath::Abs( TMath::Tan( 0.5 * theta ) ) );
        fTotNeutralEt += cluster->E() * TMath::Sin(theta);

        fMultiplicity++;

    }

    fTotNeutralEtAcc = fTotNeutralEt;
    fTotEt = fTotChargedEt + fTotNeutralEt;
    fTotEtAcc = fTotChargedEtAcc + fTotNeutralEtAcc;

    // Fill the histograms...
    FillHistograms();

    return 0;
}

bool AliAnalysisEtReconstructed::CheckGoodVertex(AliVParticle* track)
{

    Float_t bxy = 999.;
    Float_t bz = 999.;
    dynamic_cast<AliESDtrack*>(track)->GetImpactParametersTPC(bxy,bz);

    return TMath::Abs(track->Xv()) < fVertexXCut && TMath::Abs(track->Yv()) < fVertexYCut && TMath::Abs(track->Zv()) < fVertexZCut && TMath::Abs(bxy) < fIPxyCut && TMath::Abs(bz) < fIPzCut;;

}

void AliAnalysisEtReconstructed::Init()
{

    AliAnalysisEt::Init();

    fVertexXCut = EtReconstructedCuts::kVertexXCut;
    fVertexYCut = EtReconstructedCuts::kVertexYCut;
    fVertexZCut = EtReconstructedCuts::kVertexZCut;
    fIPxyCut = EtReconstructedCuts::kIPxyCut;
    fIPzCut = EtReconstructedCuts::kIPzCut;

}

void AliAnalysisEtReconstructed::FillOutputList(TList* list)
{
    AliAnalysisEt::FillOutputList(list);

    list->Add(fHistChargedPionEnergyDeposit);
    list->Add(fHistProtonEnergyDeposit);
    list->Add(fHistAntiProtonEnergyDeposit);
    list->Add(fHistChargedKaonEnergyDeposit);
}

void AliAnalysisEtReconstructed::CreateHistograms()
{
    AliAnalysisEt::CreateHistograms();

    TString histname;
    histname = "fHistChargedPionEnergyDeposit" + fHistogramNameSuffix;
    fHistChargedPionEnergyDeposit = new TH2F(histname.Data(), "Energy deposited by #pi^{+/-}", 1000, 0, 10, 1000, 0, 10);
    fHistChargedPionEnergyDeposit->SetXTitle("Energy deposited in calorimeter");
    fHistChargedPionEnergyDeposit->SetYTitle("Energy of track");
    
    histname = "fHistProtonEnergyDeposit" + fHistogramNameSuffix;
    fHistProtonEnergyDeposit = new TH2F(histname.Data(), "Energy deposited by protons", 1000, 0, 10, 1000, 0, 10);
    fHistProtonEnergyDeposit->SetXTitle("Energy deposited in calorimeter");
    fHistProtonEnergyDeposit->SetYTitle("Energy of track");
    
    histname = "fHistAntiProtonEnergyDeposit" + fHistogramNameSuffix;
    fHistAntiProtonEnergyDeposit = new TH2F(histname.Data(), "Energy deposited by anti-protons", 1000, 0, 10, 1000, 0, 10);
    fHistAntiProtonEnergyDeposit->SetXTitle("Energy deposited in calorimeter");
    fHistAntiProtonEnergyDeposit->SetYTitle("Energy of track");
    
    histname = "fHistChargedKaonEnergyDeposit" + fHistogramNameSuffix;
    fHistChargedKaonEnergyDeposit = new TH2F(histname.Data(), "Energy deposited by K^{+/-}", 1000, 0, 10, 1000, 0, 10);
    fHistChargedKaonEnergyDeposit->SetXTitle("Energy deposited in calorimeter");
    fHistChargedKaonEnergyDeposit->SetYTitle("Energy of track");
    

}


bool AliAnalysisEtReconstructed::TrackHitsCalorimeter(AliVParticle* track, Double_t magField)
{

    AliESDtrack *esdTrack = dynamic_cast<AliESDtrack*>(track);
    // Printf("Propagating track: eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());

    Bool_t prop = esdTrack->PropagateTo(fDetectorRadius, magField);

    //if(prop)Printf("Track propagated, eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());
    return prop &&
           TMath::Abs(esdTrack->Eta()) < fEtaCutAcc &&
           esdTrack->Phi() > fPhiCutAccMin*TMath::Pi()/180. &&
           esdTrack->Phi() < fPhiCutAccMax*TMath::Pi()/180.;
}

