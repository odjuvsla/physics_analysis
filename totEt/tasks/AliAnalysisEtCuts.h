#ifndef ALIANALYSISETCOMMONCUTS_H
#define ALIANALYSISETCOMMONCUTS_H

#include "Rtypes.h"

namespace EtCommonCuts
{
   
   const Double_t kEtaCut = 0.5;
   const Double_t kClusterEnergyCut = 0.0;
   const Double_t kTrackPtCut = 0.0;
   
   const Int_t kSingleCell = 1;
   
};

namespace EtGeometryCutsPhos
{
   const Double_t kEtaAccCut = 0.12;
   const Double_t kPhiAccMinCut = 260.0;
   const Double_t kPhiAccMaxCut = 320.0;
   const Double_t kDetectorRadius = 460.0;
};

namespace EtGeometryCutsEmcal
{
   const Double_t kEtaAccCut = 0.12;
   const Double_t kPhiAccMinCut = 260.0;
   const Double_t kPhiAccMaxCut = 320.0;
   const Double_t kDetectorRadius = 460.0;
};

namespace EtReconstructedCuts
{
   const Double_t kVertexXCut = 0.5;
   const Double_t kVertexYCut = 0.5;
   const Double_t kVertexZCut = 12.0;
   const Double_t kIPxyCut = 1.5;
   const Double_t kIPzCut = 1.5;
   
   const Int_t kNTpcClustersCut = 30;
   const Int_t knItsClustersCut = 3;
   
};

namespace EtReconstructedCutsPhos
{
   const Char_t kClusterType = -1;
   
   const Double_t kClusterEnergyCut = 0.0;
   const Double_t kSingleCellEnergyCut = 0.5;
   
   const Double_t kTrackDistanceCut = 15.0;
   
};

namespace EtMonteCarloCuts
{
   
   const Double_t kSingleChargedParticle = 3;
   const Double_t kNeutralParticle = 0;
   
};

#endif