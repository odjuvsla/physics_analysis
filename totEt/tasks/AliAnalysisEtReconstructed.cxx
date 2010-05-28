
#include "AliAnalysisEtReconstructed.h"
#include "AliAnalysisEtCuts.h"
#include "AliESDtrack.h"
#include "AliESDCaloCluster.h"
#include "TVector3.h"
#include "AliVEvent.h"
#include "AliESDEvent.h"
#include "AliVParticle.h"

AliAnalysisEtReconstructed::AliAnalysisEtReconstructed() :
AliAnalysisEt()
,fNTpcClustersCut(EtReconstructedCuts::kNTpcClustersCut)
,fNItsClustersCut(EtReconstructedCuts::knItsClustersCut)
{

}

Int_t AliAnalysisEtReconstructed::AnalyseEvent(AliVEvent* ev)
{
    
   AliESDEvent *event = dynamic_cast<AliESDEvent*>(ev);
    
    Int_t chargedMultiplicity = 0;

    Int_t nparticles = 0;

    for (Int_t iTrack = 0; iTrack < event->GetNumberOfTracks(); iTrack++)
    {
        AliVParticle *track = event->GetTrack(iTrack);
        if (!track)
        {
            Printf("ERROR: Could not get track %d", iTrack);
            continue;
        }

        nparticles++;

        Int_t nItsClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(0);
        Int_t nTPCClusters = dynamic_cast<AliESDtrack*>(track)->GetNcls(1);

        Float_t massPart = 0;

        const Double_t *pidWeights = track->PID();
        if (pidWeights)
        {
            Int_t maxpid = -1;
            Float_t maxpidweight = 0;
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
                //     massPart = -0.938*track->Charge();
            }

        }

        Double_t et = track->E() * TMath::Sin(track->Theta()) + massPart;

        if (TMath::Abs(track->Eta()) < fEtaCut && CheckGoodVertex(track) && nItsClusters > fNItsClustersCut && nTPCClusters > fNTpcClustersCut)
        {
            fTotChargedEt +=  track->E() * TMath::Sin(track->Theta()) + massPart;
            chargedMultiplicity++;

            if (TMath::Abs(track->Eta()) < fEtaCutAcc && track->Phi() < fPhiCutAccMax && track->Phi() > fPhiCutAccMin)
            {
                fTotChargedEtAcc += track->E()*TMath::Sin(track->Theta()) + massPart;
            }
        }

        Double_t phi = track->Phi();
        Double_t pt = track->Pt();
        if (TrackHitsCalorimeter(track, event->GetMagneticField()))
        {
            //if (track->Charge() > 0) fHistPhivsPtPos->Fill(phi,pt);
//            else fHistPhivsPtNeg->Fill(phi, pt);
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
        if (cluster->E() < fClusterEnergyCut) continue;
        Float_t pos[3];
        TVector3 cp(pos);
        cluster->GetPosition(pos);
        if (pos[0] < -(32.0*2.2)) continue; //Ensure that modules 0 and 1 are not used
        // if(cp.Phi() < 260.*TMath::Pi()/180.) continue;
//        fHistTMDeltaR->Fill(cluster->GetEmcCpvDistance());
        if (cluster->GetEmcCpvDistance() < 15.0)
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

	cluster->GetPosition(pos);

        float dist = TMath::Sqrt(pos[0]*pos[0] + pos[1]*pos[1]);

        float eta = TMath::Log(TMath::Abs( TMath::Tan( 0.5 * (TMath::ATan(pos[2]/dist) + TMath::Pi()/2) ) ) );
        float theta = TMath::ATan(pos[2]/dist)+TMath::Pi()/2;
        fTotNeutralEt += cluster->E() * TMath::Sin(theta);
        neutralMultiplicity++;

	nparticles++;

    }
}

bool AliAnalysisEtReconstructed::CheckGoodVertex(AliVParticle* track)
{

    Float_t bxy = 999.;
    Float_t bz = 999.;
    dynamic_cast<AliESDtrack*>(track)->GetImpactParametersTPC(bxy,bz);

    return TMath::Abs(track->Xv()) < fVertexXCut && TMath::Abs(track->Yv()) < fVertexYCut && TMath::Abs(track->Zv()) < fVertexZCut && TMath::Abs(bxy) < fIPxyCut && TMath::Abs(bz) < fIPzCut;;

}

// bool AliAnalysisEtReconstructed::TrackHitsPHOS(AliVParticle* track, Double_t magField)
// {
//     AliESDtrack *esdTrack = dynamic_cast<AliESDtrack*>(track);
//     // Printf("Propagating track: eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());
// 
//     Bool_t prop = esdTrack->PropagateTo(460.0, magField);
// 
//     //if(prop)Printf("Track propagated, eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());
//     return prop&& TMath::Abs(esdTrack->Eta()) < 0.12 && esdTrack->Phi() > 260.*TMath::Pi()/180. && esdTrack->Phi() < 320.*TMath::Pi()/180.;
// 
// }