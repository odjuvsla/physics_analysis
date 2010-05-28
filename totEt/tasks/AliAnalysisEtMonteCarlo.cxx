#include "AliAnalysisEtMonteCarlo.h"
#include "AliAnalysisEtCuts.h"

#include "AliStack.h"
#include "AliMCEvent.h"

Int_t AliAnalysisEtMonteCarlo::AnalyseEvent(AliVEvent* ev)
{
   
   // Get us a mc event
   AliMCEvent *event = dynamic_cast<AliMCEvent*>(ev);
   
    // Let's play with the stack!
    AliStack *stack = event->Stack();

    Int_t nPrim = stack->GetNtrack();

    Int_t nparticles = 0;

    Double_t particleMassPart = 0; //The mass part in the Et calculation for this particle
    
    for (Int_t iPart = 0; iPart < nPrim; iPart++)
    {

        TParticle *part = stack->Particle(iPart);

	if (!part)
        {
            Printf("ERROR: Could not get particle %d", iPart);
            continue;
        }

        TParticlePDG *pc = part->GetPDG(0);

	 // Check if it is a primary particle
        if (!stack->IsPhysicalPrimary(iPart)) continue;

	// Check for reasonable (for now neutral and singly charged) charge on the particle 
	//TODO:Maybe not only singly charged?
        if (TMath::Abs(pc->Charge()) != EtMonteCarloCuts::kSingleChargedParticle && pc->Charge() != EtMonteCarloCuts::kNeutralParticle) continue;

        nparticles++;

        if (TMath::Abs(part->Eta()) < fEtaCut)
        {

	   TParticlePDG *pdgCode =  part->GetPDG(0);

            if (pdgCode->Charge() == EtMonteCarloCuts::kNeutralParticle)
            {
                fTotNeutralEt += part->Energy()*TMath::Sin(part->Theta());

                if (TMath::Abs(part->Eta()) < fEtaCutAcc && part->Phi() < fPhiCutAccMax && part->Phi() > fPhiCutAccMin)
                {
                    fTotNeutralEtAcc += part->Energy()*TMath::Sin(part->Theta());
                    fTotEtAcc += part->Energy()*TMath::Sin(part->Theta());
                }
            }
            else if (pdgCode->Charge() != EtMonteCarloCuts::kNeutralParticle)
            {
                fTotChargedEt += part->Energy()*TMath::Sin(part->Theta());
                if (TMath::Abs(part->Eta()) < fEtaCutAcc && part->Phi() < fPhiCutAccMax && part->Phi() > fPhiCutAccMin)
                {
                    fTotChargedEtAcc += part->Energy()*TMath::Sin(part->Theta());
                    fTotEtAcc += part->Energy()*TMath::Sin(part->Theta());
                }
                if (
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("proton")->PdgCode() ||
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("neutron")->PdgCode() ||
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("Lambda0")->PdgCode() ||
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("Xi-")->PdgCode() ||
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("Xi0")->PdgCode() ||
                    TMath::Abs(pdgCode->PdgCode()) == fPdgDB->GetParticle("Omega-")->PdgCode()
                )
                {
                    particleMassPart = -TMath::Sign(pdgCode->PdgCode(), pdgCode->PdgCode())*pdgCode->Mass();
                }
            }
        }
    }
}

