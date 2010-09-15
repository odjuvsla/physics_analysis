   #ifndef ALIANALYSISET_H
#define ALIANALYSISET_H

#include "TString.h"
#include "TDatabasePDG.h"
#include "Rtypes.h"

class TH2F;
class TH1F;
class AliVEvent;
class TList;

class AliAnalysisEt
{
public:
   
    AliAnalysisEt();
    virtual ~AliAnalysisEt();

    /** Analyse the event! */
    virtual Int_t AnalyseEvent(AliVEvent *event) = 0;

    /** Fill the objects you want to output, classes which add new histograms should overload this. */
    virtual void FillOutputList(TList* list);

    /** Initialise the analysis, must be overloaded. */
    virtual void Init() = 0;

    /** 
    * Creates the histograms, must be overloaded if you want to add your own. 
    * Uses the fHistogramNameSuffix to create proper histogram names
    */
    virtual void CreateHistograms();
    
    /** Fills the histograms, must be overloaded if you want to add your own */
    virtual void FillHistograms();

    /** Reset event specific values (Et etc.) */
    virtual void ResetEventValues();
    
    /** Sum of the total Et for all events */
    Double_t GetSumEt() { return fSumEt; }

    /** Sum of the total Et within our acceptance for all events */
    Double_t GetSumEtAcc() { return fSumEtAcc; }

    /** Total Et in the event (without acceptance cuts) */
    Double_t GetTotEt() { return fTotEt; }

    /** Total Et in the event within the acceptance cuts */
    Double_t GetTotEtAcc() { return fTotEtAcc; }

   /** Total neutral Et in the event (without acceptance cuts) */
    Double_t GetTotNeutralEt() { return fTotNeutralEt; }

    /** Total neutral Et in the event within the acceptance cuts */
    Double_t GetTotNeutralEtAcc() { return fTotNeutralEtAcc; }
    
    /** Total charged Et in the event (without acceptance cuts) */
    Double_t GetTotChargedEt() { return fTotChargedEt; }

    /** Total charged Et in the event within the acceptance cuts */
    Double_t GetTotChargedEtAcc() { return fTotChargedEtAcc; }


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

    /** Multiplicity of particles in the event */
    Int_t fMultiplicity;
    
    /** Multiplicity of charged particles in the event */
    Int_t fChargedMultiplicity;
    
    /** Multiplicity of neutral particles in the event */
    Int_t fNeutralMultiplicity; 
    
    /** Et of identified baryons */
    Double_t fBaryonEt;
    
    /** Et of identified anti-baryons */
    Double_t fAntiBaryonEt;
    
    /** Et of identified mesons */
    Double_t fMesonEt;

    /** Et of identified baryons in calorimeter acceptance */
    Double_t fBaryonEtAcc;

    /** Et of identified anti-baryons in calorimeter acceptance */
    Double_t fAntiBaryonEtAcc;
    
    /** Et of identified mesons in calorimeter acceptance */
    Double_t fMesonEtAcc;

    /** Et of identified protons */
    Double_t fProtonEt;
    
    /** Et of identified charged kaons */
    Double_t fChargedKaonEt;
    
    /** Et of identified muons */
    Double_t fMuonEt;
    
    /** Et of identified electrons */
    Double_t fElectronEt;
    
    /** Et of identified protons in calorimeter acceptance */
    Double_t fProtonEtAcc;

    /** Et of identified charged kaons in calorimeter acceptance */
    Double_t fChargedKaonEtAcc;

    /** Et of identified muons in calorimeter acceptance */
    Double_t fMuonEtAcc;

    /** Et of identified electrons in calorimeter acceptance */
    Double_t fElectronEtAcc;
    
    /** Cut in eta ( normally |eta| < 0.5 */
    Double_t fEtaCut;

    /** Eta cut for our acceptance */
    Double_t fEtaCutAcc;

    /** Min phi cut for our acceptance in radians */
    Double_t fPhiCutAccMin;

    /** Max phi cut for our acceptance in radians */
    Double_t fPhiCutAccMax;

    /** Detector radius */
    Double_t fDetectorRadius;

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

    /** Minimum energy to cut on single cell cluster */
    Double_t fSingleCellEnergyCut;
    
    // Declare the histograms

    /** The full Et spectrum measured */
    TH1F *fHistEt; //Et spectrum

    /** The full charged Et spectrum measured */
    TH1F *fHistChargedEt; //Charged Et spectrum

    /** The full neutral Et spectrum measured */
    TH1F *fHistNeutralEt; //Neutral Et spectrum

    /** The Et spectrum within the calorimeter acceptance */
    TH1F *fHistEtAcc; //Et in acceptance

    /** The charged Et spectrum within the calorimeter acceptance */
    TH1F *fHistChargedEtAcc; //Charged Et in acceptance

    /** The neutral Et spectrum within the calorimeter acceptance */
    TH1F *fHistNeutralEtAcc; //Et in acceptance

    /** Multiplicity of particles in the events */
    TH1F *fHistMult; //Multiplicity

    /** Charged multiplicity of particles in the events */
    TH1F *fHistChargedMult; //Charged multiplicity

    /** Neutral multiplicity of particles in the events */
    TH1F *fHistNeutralMult; //Neutral multiplicity

    /* Acceptance plots */
    TH2F *fHistPhivsPtPos; //phi vs pT plot for positive tracks
    TH2F *fHistPhivsPtNeg; //phi vs pT plot for negative tracks

    /* PID plots */
    TH1F *fHistBaryonEt;
    TH1F *fHistAntiBaryonEt;
    TH1F *fHistMesonEt;

    TH1F *fHistBaryonEtAcc;
    TH1F *fHistAntiBaryonEtAcc;
    TH1F *fHistMesonEtAcc;

    TH1F *fHistProtonEt;
    TH1F *fHistChargedKaonEt;
    TH1F *fHistMuonEt;
    TH1F *fHistElectronEt;
    
    TH1F *fHistProtonEtAcc;
    TH1F *fHistChargedKaonEtAcc;
    TH1F *fHistMuonEtAcc;
    TH1F *fHistElectronEtAcc;
    
    /* Correction plots */
    TH2F *fHistEtRecvsEtMC; //Reconstructed Et versus MC Et

    /* Track matching plots */
    TH1F *fHistTMDeltaR;
    
    ClassDef(AliAnalysisEt, 0);
};

#endif // ALIANALYSISET_H
