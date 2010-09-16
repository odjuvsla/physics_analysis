{
  // Load common libraries
  gSystem->Load("libCore.so");  
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");   
  // Use AliRoot includes to compile our task
  gROOT->ProcessLine(".include $ALICE_ROOT/include");

  // Create and configure the alien handler plugin
  gROOT->LoadMacro("CreateAlienHandler.C");
  AliAnalysisGrid *alienHandler = CreateAlienHandler();  
  
  if (!alienHandler) return;

  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("TotEtAnalysis");

  // Connect plug-in to the analysis manager
  mgr->SetGridHandler(alienHandler);
   gROOT->LoadMacro("AliAnalysisEt.cxx+g");
    gROOT->LoadMacro("AliAnalysisEtReconstructed.cxx+g");
    gROOT->LoadMacro("AliAnalysisEtMonteCarlo.cxx+g");
    gROOT->LoadMacro("AliAnalysisEtReconstructedPhos.cxx+g");
    gROOT->LoadMacro("AliAnalysisEtMonteCarloPhos.cxx+g");
   gROOT->LoadMacro("AliAnalysisTaskTotEt.cxx++g");   
  
  AliAnalysisTask *task = new AliAnalysisTaskTotEt("TaskTotEt");
  ((AliAnalysisTaskTotEt*)task)->SetTriggerSelection(true);
  mgr->AddTask(task);

  AliESDInputHandler* esdH = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdH);

// No need to create a chain - this is handled by the plug-in
//  gROOT->LoadMacro("$ALICE_ROOT/PWG0/CreateESDChain.C");
//  TChain* chain = CreateESDChain("ESD82XX_30K.txt", 10);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("out1", TList::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput1);

  // Enable debug printouts
  mgr->SetDebugLevel(2);

  if (!mgr->InitAnalysis())
    return;

  mgr->PrintStatus();
  // Start analysis in grid.
  mgr->StartAnalysis("grid");
};
