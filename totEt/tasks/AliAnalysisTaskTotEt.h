#ifndef AliAnalysisTaskTotEt_cxx
#define AliAnalysisTaskTotEt_cxx

class TH1F;
class TNtuple;
class TObjArray;
class AliESDEvent;

#include "/home/odjuvsla/Workspace/alice/aliroot/ANALYSIS/AliAnalysisTaskSE.h"

class AliAnalysisTaskTotEt : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskTotEt(const char *name = "AliAnalysisTaskTotEt");
  virtual ~AliAnalysisTaskTotEt() {}
  
  //  virtual void   ConnectInputData(Option_t *);
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
 private:
  AliESDEvent *fESD;    //ESD object
  TH1F        *fHistEt; //Et spectrum
  TH1F        *fHistTotE; //E spectrum
  TH1F        *fHistEtCells; //Et spectrum of cells
  TH1F        *fHistTotEtCells; //Total Et spectrum of cells
  TNtuple     *fEtNtuple; //Ntuple
  TObjArray   *fHistCell;    
  Int_t fCount;
  AliAnalysisTaskTotEt(const AliAnalysisTaskTotEt&); // not implemented
  AliAnalysisTaskTotEt& operator=(const AliAnalysisTaskTotEt&); // not implemented
  
  ClassDef(AliAnalysisTaskTotEt, 1); // example of analysis
};

#endif
