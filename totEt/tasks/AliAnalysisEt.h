#ifndef ALIANALYSISET_H
#define ALIANALYSISET_H

#include "TString.h"
#include "Rtypes.h"

class AliVEvent;
class TList;

class AliAnalysisEt
{
public:
    AliAnalysisEt();
    virtual ~AliAnalysisEt();

    virtual Int_t AnalyseEvent(AliVEvent *event) = 0;

    virtual void FillOutputList(TList &list);

    virtual void Init();

protected:

    TString fHistogramNameSuffix;

    Double_t fSumEtRec;
    Double_t fSumEtRecAcc;
    Double_t fSumEtMC;

    Double_t fEtaCut;
    Double_t fEtaCutAcc;
    Double_t fPhiCutAccMin;
    Double_t fPhiCutAccMax;
    Double_t fVertexXCut;
    Double_t fVertexYCut;
    Double_t fVertexZCut;

    Double_t fIPxyCut;
    Double_t fIPzCut;

    Double_t fClusterEnergyCut;
    
    ClassDef(AliAnalysisEt, 0);
};

#endif // ALIANALYSISET_H
