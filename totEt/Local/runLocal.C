
void runLocal() {

  TStopwatch timer;
  timer.Start();
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");

  gROOT->LoadMacro("AliAnalysisEt.cxx+");
  gROOT->LoadMacro("AliAnalysisEtReconstructed.cxx+");
  gROOT->LoadMacro("AliAnalysisEtMonteCarlo.cxx+");
  gROOT->LoadMacro("AliAnalysisEtReconstructedPhos.cxx+");
  gROOT->LoadMacro("AliAnalysisEtMonteCarloPhos.cxx+");

  gROOT->LoadMacro("AliAnalysisTaskTotEt.cxx+g");

  TChain *chain = new TChain("esdTree");
  chain->Add("/media/wd/data/LHC10b/10000116134019.10/AliESDs.root");
  chain->Add("/media/wd/data/LHC10b/10000116134019.30/AliESDs.root");
  chain->Add("/media/wd/data/LHC10b/10000116134019.40/AliESDs.root");

  // Make the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("TotEtManager");
  AliVEventHandler* esdH = new AliESDInputHandler;
  mgr->SetInputEventHandler(esdH);  
  
  AliAnalysisTaskTotEt *task1 = new AliAnalysisTaskTotEt("TaskTotEt");
  task1->SetTriggerSelection(true);
  mgr->AddTask(task1);
 
  // Create containers for input/output
  //AliAnalysisDataContainer *cinput1 = mgr->CreateContainer("cchain1", TChain::Class(),AliAnalysisManager::kInputContainer);
   AliAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();
   
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("out1", TList::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.real.root");

  //____________________________________________//
  mgr->ConnectInput(task1,0,cinput1);
  mgr->ConnectOutput(task1,1,coutput1);
  
  mgr->SetDebugLevel(2);

  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("local",chain);

  timer.Stop();
  timer.Print();
}
