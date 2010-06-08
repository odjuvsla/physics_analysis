#ifndef AliAnalysisTaskTotEt_cxx
#define AliAnalysisTaskTotEt_cxx

class AliAnalysisEt;
class TTree;
class AliVParticle;
class TH1F;
class TH2F;
class TNtuple;
class TObjArray;
class AliESDEvent;
class AliMCParticle;
class TDatabasePDG;

#include "AliAnalysisTaskSE.h"
#include "TObject.h"

class ParticleVars : public TObject        // Inherit from TObject to put in TClonesArray
    {
       public:
	  
	  ParticleVars() : TObject(){}
       Int_t fPdgCode; // from MC
       Int_t fPid; //from ESDs
       Int_t fMass;
       Int_t fCharge;
       Double_t fEt;
       Double_t fPhi;
       Double_t fEta;
       
       ClassDef(ParticleVars, 1);
       
    };

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

    AliAnalysisEt *fPhosRecAnalysis;
    AliAnalysisEt *fPhosMCAnalysis;

    TH2F *fHistEtRecvsEtMC;
    
    Int_t fCount;

    Bool_t fTriggerSelection;
    
    //AliAnalysisTaskTotEt(const AliAnalysisTaskTotEt&); // not implemented
    //AliAnalysisTaskTotEt& operator=(const AliAnalysisTaskTotEt&); // not implemented

    const int fkPhotonPdg;

    const Float_t fkProtonMass;

    TDatabasePDG *fPdgDB;

    class EventVars
    {
       public:
        Double_t fTotEt;
	Double_t fTotEtAcc;
        Double_t fTotEnergy;

        Double_t fTotNeutralEt;
	Double_t fTotNeutralEtAcc;

        Double_t fTotChargedEt;
        Double_t fTotChargedEtAcc;

        Int_t fChargedMultiplicity;
        Int_t fNeutralMultiplicity;

    };
    
    EventVars *fRecEventVars;
    EventVars *fSimEventVars;
    
    
    TClonesArray *fRecParticleArray;
    TClonesArray *fSimParticleArray;
    
    ClassDef(AliAnalysisTaskTotEt, 1); // example of analysis
};

#endif
