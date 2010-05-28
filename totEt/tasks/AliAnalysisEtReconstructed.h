#ifndef ALIANALYSISETRECONSTRUCTED_H
#define ALIANALYSISETRECONSTRUCTED_H

#include "AliAnalysisEt.h"

class AliVParticle;

class AliAnalysisEtReconstructed : public AliAnalysisEt
{

public:
   
    AliAnalysisEtReconstructed();
   
    virtual Int_t AnalyseEvent(AliVEvent* event);

protected:

    bool CheckGoodVertex(AliVParticle *track);
    virtual bool TrackHitsCalorimeter(AliVParticle *track, Double_t magField) = 0;

    Int_t fNTpcClustersCut;
    Int_t fNItsClustersCut;

};

#endif // ALIANALYSISETRECONSTRUCTED_H
