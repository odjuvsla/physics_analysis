#ifndef ALIANALYSISETRECONSTRUCTED_H
#define ALIANALYSISETRECONSTRUCTED_H

#include "AliAnalysisEt.h"


class AliAnalysisEtReconstructed : public AliAnalysisEt
{

public:
    virtual Int_t AnalyseEvent(AliVEvent* event);
    
    
};

#endif // ALIANALYSISETRECONSTRUCTED_H
