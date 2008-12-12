#ifndef AliAnalysisTaskTotEt_cxx
#define AliAnalysisTaskTotEt_cxx

class TH1F;
class TObjArray;
class AliESDEvent;

#include "/home/odjuvsla/Workspace/alice/aliroot/ANALYSIS/AliAnalysisTask.h"

class AliAnalysisTaskTotEt : public AliAnalysisTask {
 public:
  AliAnalysisTaskTotEt(const char *name = "AliAnalysisTaskTotEt");
  virtual ~AliAnalysisTaskTotEt() {}
  
  virtual void   ConnectInputData(Option_t *);
  virtual void   CreateOutputObjects();
  virtual void   Exec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
 private:
  AliESDEvent *fESD;    //ESD object
  TH1F        *fHistEt; //Et spectru
  TH1F        *fHistEtCells; //Et spectrum of cells
  TH1F        *fHistTotEtCells; //Total Et spectrum of cells
  TObjArray* fHistCell;    
  Int_t fCount;
  AliAnalysisTaskTotEt(const AliAnalysisTaskTotEt&); // not implemented
  AliAnalysisTaskTotEt& operator=(const AliAnalysisTaskTotEt&); // not implemented
  
  ClassDef(AliAnalysisTaskTotEt, 1); // example of analysis
};

#endif
