#include "AliAnalysisEtReconstructedPhos.h"
#include "AliAnalysisEtCuts.h"
#include "AliESDtrack.h"

AliAnalysisEtReconstructedPhos::AliAnalysisEtReconstructedPhos() :
AliAnalysisEtReconstructed()
{
   fHistogramNameSuffix = TString("PhosRec");    
}


void AliAnalysisEtReconstructedPhos::Init()
{
    AliAnalysisEtReconstructed::Init();
    
    fClusterType = EtReconstructedCutsPhos::kClusterType;
    
    fEtaCutAcc = EtGeometryCutsPhos::kEtaAccCut;
    fPhiCutAccMax = EtGeometryCutsPhos::kPhiAccMaxCut*TMath::Pi()/180.;
    fPhiCutAccMin = EtGeometryCutsPhos::kPhiAccMinCut*TMath::Pi()/180.;
    fClusterEnergyCut = EtReconstructedCutsPhos::kClusterEnergyCut;
    fSingleCellEnergyCut = EtReconstructedCutsPhos::kSingleCellEnergyCut;
    fTrackDistanceCut = EtReconstructedCutsPhos::kTrackDistanceCut;
	 
}

bool AliAnalysisEtReconstructedPhos::TrackHitsCalorimeter(AliVParticle* track, Double_t magField)
{

   AliESDtrack *esdTrack = dynamic_cast<AliESDtrack*>(track);
    // Printf("Propagating track: eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());

    Bool_t prop = esdTrack->PropagateTo(EtGeometryCutsPhos::kDetectorRadius, magField);

    //if(prop)Printf("Track propagated, eta: %f, phi: %f, pt: %f", esdTrack->Eta(), esdTrack->Phi(), esdTrack->Pt());
    return prop && 
		   TMath::Abs(esdTrack->Eta()) < EtGeometryCutsPhos::kEtaAccCut && 
		   esdTrack->Phi() > EtGeometryCutsPhos::kPhiAccMinCut *TMath::Pi()/180. && 
		   esdTrack->Phi() < EtGeometryCutsPhos::kPhiAccMaxCut*TMath::Pi()/180.;
}
