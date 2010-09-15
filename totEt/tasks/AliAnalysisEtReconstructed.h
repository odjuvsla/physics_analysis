#ifndef ALIANALYSISETRECONSTRUCTED_H
#define ALIANALYSISETRECONSTRUCTED_H

#include "AliAnalysisEt.h"

class TH2F;
class AliVParticle;
class TList;

class AliAnalysisEtReconstructed : public AliAnalysisEt
{

public:
   
    AliAnalysisEtReconstructed();
   
    virtual Int_t AnalyseEvent(AliVEvent* event);

    virtual void Init();

    /** Fill the objects you want to output, classes which add new histograms should overload this. */
    virtual void FillOutputList(TList *list);

    /** Create the histograms, must be overloaded if you want to add your own */
    virtual void CreateHistograms();
    
protected:

    bool CheckGoodVertex(AliVParticle *track);
    virtual bool TrackHitsCalorimeter(AliVParticle *track, Double_t magField);

    Int_t fNTpcClustersCut;
    Int_t fNItsClustersCut;
   
    Double_t fTrackDistanceCut;
    
    Double_t fPidCut;
    
    Char_t fClusterType;
    
    /** Energy deposited in calorimeter by charged pions */
    TH2F *fHistChargedPionEnergyDeposit;
    
    /** Energy deposited in calorimeter by protons */
    TH2F *fHistProtonEnergyDeposit;
    
    /** Energy deposited in calorimeter by anti-protons */
    TH2F *fHistAntiProtonEnergyDeposit;

    /** Energy deposited in calorimeter by charged kaons */
    TH2F *fHistChargedKaonEnergyDeposit;
    
};

#endif // ALIANALYSISETRECONSTRUCTED_H
