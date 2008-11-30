#ifndef AliAnalysisTaskPt_cxx
#define AliAnalysisTaskPt_cxx

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

class TH1F;
class AliESDEvent;

#include "AliAnalysisTask.h"

class AliAnalysisTaskPt : public AliAnalysisTask {
 public:
  AliAnalysisTaskPt(const char *name = "AliAnalysisTaskPt");
  virtual ~AliAnalysisTaskPt() {}
  
  virtual void   ConnectInputData(Option_t *);
  virtual void   CreateOutputObjects();
  virtual void   Exec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
 private:
  AliESDEvent *fESD;    //ESD object
  TH1F        *fHistPt; //Pt spectrum
   
  AliAnalysisTaskPt(const AliAnalysisTaskPt&); // not implemented
  AliAnalysisTaskPt& operator=(const AliAnalysisTaskPt&); // not implemented
  
  ClassDef(AliAnalysisTaskPt, 1); // example of analysis
};

#endif
