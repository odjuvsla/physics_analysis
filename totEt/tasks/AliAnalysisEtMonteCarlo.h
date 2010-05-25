#ifndef ALIANALYSISETMONTECARLO_H
#define ALIANALYSISETMONTECARLO_H

#include "AliAnalysisEt.h"


class AliAnalysisEtMonteCarlo : public AliAnalysisEt
{

public:
    virtual Int_t AnalyseEvent(AliVEvent* event);
    
};

#endif // ALIANALYSISETMONTECARLO_H
