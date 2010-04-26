#ifndef AliAnalysisTaskTotEt_cxx
#define AliAnalysisTaskTotEt_cxx

class AliVParticle;
class TH1F;
class TH2F;
class TNtuple;
class TObjArray;
class AliESDEvent;
class AliMCParticle;

#include "AliAnalysisTaskSE.h"
#include "Rtypes.h"

class AliAnalysisTaskTotEt : public AliAnalysisTaskSE {
public:
    AliAnalysisTaskTotEt(const char *name = "AliAnalysisTaskTotEt");
    virtual ~AliAnalysisTaskTotEt() {}

    //  virtual void   ConnectInputData(Option_t *);
    virtual void   UserCreateOutputObjects();
    virtual void   UserExec(Option_t *option);
    virtual void   Terminate(Option_t *);

    virtual void SetTriggerSelection(Bool_t v) {
        fTriggerSelection = true;
    }

private:

    Float_t CorrectForCaloAcceptance(Float_t energy);
    bool CheckGoodVertex(AliVParticle *track);
    bool TrackHitsPHOS(AliVParticle *track, Double_t magField);
    bool ParticleInPHOS(AliMCParticle *part);
    bool ParticleInEMCAL(AliMCParticle *part);

    AliESDEvent *fESD;    //ESD object

    TList *fOutputList;

    /* Neutral and charged Et */
    TH1F        *fHistEt; //Et spectrum
    TH1F        *fHistTotE; //E spectrum
    TH1F        *fHistMult; //Multiplicity
    TH1F        *fHistMCEt; //Et spectrum
    TH1F        *fHistMCTotE; //E spectrum
    TNtuple     *fEtNtuple; //Ntuple
    TNtuple     *fEtRecMCNtuple;
    TNtuple     *fEtRecGeomNtuple;
    TNtuple     *fEtMCGeomNtuple;

    /* Neutral Et */
    TH1F        *fHistNeutralEt; //Et spectrum
    TH1F        *fHistNeutralTotE; //E spectrum
    TH1F        *fHistNeutralMult; //Multiplicity
    TH1F        *fHistMCNeutralEt; //Et spectrum
    TH1F        *fHistMCNeutralTotE; //E spectrum
    TNtuple     *fNeutralEtNtuple; //Ntuple
    TNtuple     *fNeutralEtRecMCNtuple;
    TNtuple     *fNeutralEtRecGeomNtuple;
    TNtuple     *fNeutralEtMCGeomNtuple;

    /* Charged Et */
    TH1F        *fHistChargedEt; //Et spectrum
    TH1F        *fHistChargedTotE; //E spectrum
    TH1F        *fHistChargedMult; //Multiplicity
    TH1F        *fHistMCChargedEt; //Et spectrum
    TH1F        *fHistMCChargedTotE; //E spectrum
    TNtuple     *fChargedEtNtuple; //Ntuple
    TNtuple     *fChargedEtRecMCNtuple;
    TNtuple     *fChargedEtRecGeomNtuple;
    TNtuple     *fChargedEtMCGeomNtuple;

    /* Acceptance plots */
    TH2F         *fHistPhivsPtPos; //phi vs pT plot for positive tracks
    TH2F         *fHistPhivsPtNeg; //phi vs pT plot for negative tracks

   /* Correction plots */
    TH2F          *fHistEtRecvsEtMC; //Reconstructed Et versus MC Et
    
    /* Track matching plots */
    TH1F          *fHistTMDeltaR;

    Double_t fSumEtRec;
    Double_t fSumEtRecAcc;
    Double_t fSumEtMC;
    
    Float_t fEtaCut;
    Float_t fEtaCutAcc;
    Float_t fPhiCutAccMin;
    Float_t fPhiCutAccMax; 	
    Float_t fVertexXCut;
    Float_t fVertexYCut;
    Float_t fVertexZCut;

    Float_t fIPxyCut;
    Float_t fIPzCut;

    Bool_t fTriggerSelection;

    Int_t fCount;
    AliAnalysisTaskTotEt(const AliAnalysisTaskTotEt&); // not implemented
    AliAnalysisTaskTotEt& operator=(const AliAnalysisTaskTotEt&); // not implemented

    const int fkPhotonPdg;

    ClassDef(AliAnalysisTaskTotEt, 1); // example of analysis
};

#endif
