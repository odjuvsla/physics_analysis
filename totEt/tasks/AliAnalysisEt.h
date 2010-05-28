#ifndef ALIANALYSISET_H
#define ALIANALYSISET_H

#include "TString.h"
#include "TDatabasePDG.h"
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

    virtual void Init() = 0;

protected:
   
    /** The suffix for the histogram names */
    TString fHistogramNameSuffix;

    /** PDG Database */
    TDatabasePDG *fPdgDB;
    
    /** Sum of the total Et for all events */
    Double_t fSumEt;
    
    /** Sum of the total Et within our acceptance for all events */
    Double_t fSumEtAcc;
   
    /** Total Et in the event (without acceptance cuts) */
    Double_t fTotEt;
    
    /** Total Et in the event within the acceptance cuts */
    Double_t fTotEtAcc;

    /** Total neutral Et in the event */
    Double_t fTotNeutralEt;
    
    /** Total neutral Et in the event within the acceptance cuts */
    Double_t fTotNeutralEtAcc;

    /** Total charged Et in the event */
    Double_t fTotChargedEt;
    
    /** Total charged Et in the event within the acceptance cuts */
    Double_t fTotChargedEtAcc;

    /** Cut in eta ( normally |eta| < 0.5 */
    Double_t fEtaCut;
    
    /** Eta cut for our acceptance */
    Double_t fEtaCutAcc;
    
    /** Min phi cut for our acceptance */
    Double_t fPhiCutAccMin;
    
    /** Max phi cut for our acceptance */
    Double_t fPhiCutAccMax;
    
    /** Vertex cuts */
    Double_t fVertexXCut;
    Double_t fVertexYCut;
    Double_t fVertexZCut;

    /** Impact parameter cuts */
    Double_t fIPxyCut;
    Double_t fIPzCut;

    /** Cut on the cluster energy */
    Double_t fClusterEnergyCut;
    
    /** Cut on track pt */
    Double_t fTrackPtCut;
    
    ClassDef(AliAnalysisEt, 0);
};

#endif // ALIANALYSISET_H
