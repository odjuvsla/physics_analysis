
#include "TH1F.h"
#include "TFile.h"
#include "Rtypes.h"
#include "TCanvas.h"
#include "TH2F.h"

TCanvas c1;
TCanvas c2;
      
Int_t drawHistograms()
{
   c1.Divide(3,2);
   c2.Divide(2,2);
   TFile *realfile = TFile::Open("Et.ESD_real.root", "READ");
   TList *list = dynamic_cast<TList*>(realfile->Get("out1"));
   if(list)
   {
      TH1F *totEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistEt"));
      totEtHist->Rebin(6);
      cout << "TOTHIST" << endl;
      if(totEtHist)
      {
	 for(Int_t n = 1; n < totEtHist->GetNbinsX()-1; n ++)
	 {
	    totEtHist->SetBinContent(n, totEtHist->GetBinContent(n)/totEtHist->GetEntries());
	 }
	 totEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(1);
	 c1.SetLogy();
	 totEtHist->Draw();
      }
    
      TH1F *totNeutralEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistNeutralEt"));
      totNeutralEtHist->Rebin(5);
      if(totNeutralEtHist)
      {
	 for(Int_t n = 1; n < totNeutralEtHist->GetNbinsX()-1; n ++)
	 {
	    totNeutralEtHist->SetBinContent(n, totNeutralEtHist->GetBinContent(n)/totNeutralEtHist->GetEntries());
	 }
	 totNeutralEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(2);
	 c1.SetLogy();
	 totNeutralEtHist->Draw();
      }
      TH1F *totChargedEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistChargedEt"));
      totChargedEtHist->Rebin(6);
      if(totChargedEtHist)
      {
	 for(Int_t n = 1; n < totChargedEtHist->GetNbinsX()-1; n ++)
	 {
	    totChargedEtHist->SetBinContent(n, totChargedEtHist->GetBinContent(n)/totChargedEtHist->GetEntries());
	 }
	 totChargedEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(3);
	 c1.SetLogy();
	 totChargedEtHist->Draw();
      }
      
      TH1F *multHist = dynamic_cast<TH1F*>(list->FindObject("fHistMult"));
      if(multHist)
      {
	 for(Int_t n = 1; n < multHist->GetNbinsX()-1; n ++)
	 {
	    multHist->SetBinContent(n, multHist->GetBinContent(n)/multHist->GetEntries());
	 }
	 multHist->SetYTitle("N/(NEvents)");
	 c1.cd(4);
	 c1.SetLogy();
	 multHist->Draw();
      }
      
      TH1F *neutralMultHist = dynamic_cast<TH1F*>(list->FindObject("fHistNeutralMult"));
      if(neutralMultHist)
      {
	 for(Int_t n = 1; n < neutralMultHist->GetNbinsX()-1; n ++)
	 {
	    neutralMultHist->SetBinContent(n, neutralMultHist->GetBinContent(n)/neutralMultHist->GetEntries());
	 }
	 neutralMultHist->SetYTitle("N/(NEvents)");
	 c1.cd(5);
	 c1.SetLogy();
	 neutralMultHist->Draw();
      }
      
      TH1F *chargedMultHist = dynamic_cast<TH1F*>(list->FindObject("fHistChargedMult"));
      if(chargedMultHist)
      {
	 for(Int_t n = 1; n < chargedMultHist->GetNbinsX()-1; n ++)
	 {
	    chargedMultHist->SetBinContent(n, chargedMultHist->GetBinContent(n)/chargedMultHist->GetEntries());
	 }
	 chargedMultHist->SetYTitle("N/(NEvents)");
	 c1.cd(6);
	 c1.SetLogy();
	 chargedMultHist->Draw();
      }
      
      // Acceptance plots real data
      
      
      TH2F *phiptneg = dynamic_cast<TH2F*>(list->FindObject("fHistPhivsPtNeg"));
      if(phiptneg)
      {
	 phiptneg->SetYTitle("P_{T}");
	 phiptneg->SetXTitle("#phi");
	 c2.cd(1);
	 phiptneg->SetMarkerStyle(20);
	 phiptneg->SetMarkerSize(0.7);
	 phiptneg->SetMarkerColor(kBlack);
	 phiptneg->Draw();
      }
      
      TH2F *phiptpos = dynamic_cast<TH2F*>(list->FindObject("fHistPhivsPtPos"));
      if(phiptpos)
      {
	 phiptpos->SetYTitle("P_{T}");
	 phiptpos->SetXTitle("#phi");
	 c2.cd(2);
	 phiptpos->SetMarkerStyle(20);
	 phiptpos->SetMarkerSize(0.7);
	 phiptpos->SetMarkerColor(kBlack);
	 phiptpos->Draw();
      }
       
      
      
      
      //SIMULATED DATA
      
      TFile *simfile = TFile::Open("Et.ESD_sim.root", "READ");	
      list = dynamic_cast<TList*>(simfile->Get("out1"));
      totEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistEt"));
      totEtHist->Rebin(6);
      if(totEtHist)
      {
	 for(Int_t n = 1; n < totEtHist->GetNbinsX()-1; n ++)
	 {
	    totEtHist->SetBinContent(n, totEtHist->GetBinContent(n)/totEtHist->GetEntries());
	 }
	 totEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(1);
	 c1.SetLogy();
	 totEtHist->SetLineColor(kRed);
	 totEtHist->Draw("SAME");
      }
      totNeutralEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistNeutralEt"));
      totNeutralEtHist->Rebin(5);
      if(totNeutralEtHist)
      {
	 for(Int_t n = 1; n < totNeutralEtHist->GetNbinsX()-1; n ++)
	 {
	    totNeutralEtHist->SetBinContent(n, totNeutralEtHist->GetBinContent(n)/totNeutralEtHist->GetEntries());
	 }
	 totNeutralEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(2);
	 c1.SetLogy();
	 totNeutralEtHist->SetLineColor(kRed);
	 totNeutralEtHist->Draw("SAME");
      }
      totChargedEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistChargedEt"));
      totChargedEtHist->Rebin(6);
      if(totChargedEtHist)
      {
	 for(Int_t n = 1; n < totChargedEtHist->GetNbinsX()-1; n ++)
	 {
	    totChargedEtHist->SetBinContent(n, totChargedEtHist->GetBinContent(n)/totChargedEtHist->GetEntries());
	 }
	 totChargedEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(3);
	 c1.SetLogy();
	 totChargedEtHist->SetLineColor(kRed);
	 totChargedEtHist->Draw("SAME");
      }
      
      multHist = dynamic_cast<TH1F*>(list->FindObject("fHistMult"));
      if(multHist)
      {
	 for(Int_t n = 1; n < multHist->GetNbinsX()-1; n ++)
	 {
	    multHist->SetBinContent(n, multHist->GetBinContent(n)/multHist->GetEntries());
	 }
	 multHist->SetYTitle("N/(NEvents)");
	 c1.cd(4);
	 c1.SetLogy();
	 multHist->SetLineColor(kRed);
	 multHist->Draw("SAME");
      }
      
      neutralMultHist = dynamic_cast<TH1F*>(list->FindObject("fHistNeutralMult"));
      if(neutralMultHist)
      {
	 for(Int_t n = 1; n < neutralMultHist->GetNbinsX()-1; n ++)
	 {
	    neutralMultHist->SetBinContent(n, neutralMultHist->GetBinContent(n)/neutralMultHist->GetEntries());
	 }
	 neutralMultHist->SetYTitle("N/(NEvents)");
	 c1.cd(5);
	 c1.SetLogy();
	 neutralMultHist->SetLineColor(kRed);
	 neutralMultHist->Draw("SAME");
      }
      
      chargedMultHist = dynamic_cast<TH1F*>(list->FindObject("fHistChargedMult"));
      if(chargedMultHist)
      {
	 for(Int_t n = 1; n < chargedMultHist->GetNbinsX()-1; n ++)
	 {
	    chargedMultHist->SetBinContent(n, chargedMultHist->GetBinContent(n)/chargedMultHist->GetEntries());
	 }
	 chargedMultHist->SetYTitle("N/(NEvents)");
	 c1.cd(6);
	 c1.SetLogy();
	 chargedMultHist->SetLineColor(kRed);
	 chargedMultHist->Draw("SAME");
      }
      
      phiptneg = dynamic_cast<TH2F*>(list->FindObject("fHistPhivsPtNeg"));
      if(phiptneg)
      {
	 phiptneg->SetYTitle("P_{T}");
	 phiptneg->SetXTitle("#phi");
	 c2.cd(3);
	 phiptneg->SetMarkerStyle(20);
	 phiptneg->SetMarkerSize(0.7);
	 phiptneg->SetMarkerColor(kRed);
	 phiptneg->Draw();
      }
      
      phiptpos = dynamic_cast<TH2F*>(list->FindObject("fHistPhivsPtPos"));
      if(phiptpos)
      {
	 phiptpos->SetYTitle("P_{T}");
	 phiptpos->SetXTitle("#phi");
	 c2.cd(4);
	 phiptpos->SetMarkerStyle(20);
	 phiptpos->SetMarkerSize(0.7);
	 phiptpos->SetMarkerColor(kRed);
	 phiptpos->Draw();
      }
       
      
      // Monte carlo data
      
      totEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistMCEt"));
      totEtHist->Rebin(6);
      if(totEtHist)
      {
	 for(Int_t n = 0; n < totEtHist->GetNbinsX(); n ++)
	 {
	    totEtHist->SetBinContent(n, totEtHist->GetBinContent(n)/totEtHist->GetEntries());
	 }
	 totEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(1);
	 c1.SetLogy();
	 totEtHist->SetLineColor(kBlue);
	 totEtHist->Draw("SAME");
      }
      totNeutralEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistMCNeutralEt"));
      totNeutralEtHist->Rebin(5);
      if(totNeutralEtHist)
      {
	 for(Int_t n = 0; n < totNeutralEtHist->GetNbinsX(); n ++)
	 {
	    totNeutralEtHist->SetBinContent(n, totNeutralEtHist->GetBinContent(n)/totNeutralEtHist->GetEntries());
	 }
	 totNeutralEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(2);
	 c1.SetLogy();
	 totNeutralEtHist->SetLineColor(kBlue);
	 totNeutralEtHist->Draw("SAME");
      }
      totChargedEtHist = dynamic_cast<TH1F*>(list->FindObject("fHistMCChargedEt"));
      totChargedEtHist->Rebin(6);
      if(totChargedEtHist)
      {
	 for(Int_t n = 0; n < totChargedEtHist->GetNbinsX(); n ++)
	 {
	    totChargedEtHist->SetBinContent(n, totChargedEtHist->GetBinContent(n)/totChargedEtHist->GetEntries());
	 }
	 totChargedEtHist->SetYTitle("dN/(NEvents*dE_{T}) ");
	 c1.cd(3);
	 c1.SetLogy();
	 totChargedEtHist->SetLineColor(kBlue);
	 totChargedEtHist->Draw("SAME");
      }

      
   }

return 0;
}
