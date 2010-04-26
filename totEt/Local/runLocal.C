void runLocal() {
  TStopwatch timer;
  timer.Start();
  
  gSystem->Load("libTree.so");
  //____________________________________________________//
  //_____________Setting up STEERBase.par_______________//
  //____________________________________________________//
  setupPar("STEERBase");
  gSystem->Load("libSTEERBase.so");

  //____________________________________________________//
  //_____________Setting up ESD.par_____________________//
  //____________________________________________________//
  setupPar("ESD");
  gSystem->Load("libVMC.so");
  gSystem->Load("libESD.so");
  //____________________________________________________//
  //_____________Setting up AOD.par_____________________//
  //____________________________________________________//
  setupPar("AOD");
  gSystem->Load("libAOD.so");
                                                                
  //_____________________________________________________________//
  //_____________Setting up ANALYSIS_NEW.par_____________________//
  //_____________________________________________________________//
  setupPar("ANALYSIS");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  
  //  gROOT->LoadMacro("../tasks/AliAnalysisTaskTotEt.cxx+");

  gSystem->AddIncludePath("-I$ALICE_ROOT/include");

  gROOT->LoadMacro("AliAnalysisTaskTotEt.cxx+g");

  //____________________________________________//
  /*AliTagAnalysis *TagAna = new AliTagAnalysis("ESD"); 
  TagAna->ChainLocalTags("../Tags");

  AliRunTagCuts *runCuts = new AliRunTagCuts();
  AliLHCTagCuts *lhcCuts = new AliLHCTagCuts();
  AliDetectorTagCuts *detCuts = new AliDetectorTagCuts();
  AliEventTagCuts *evCuts = new AliEventTagCuts();
  //   evCuts->SetMultiplicityRange(11,12);  
  evCuts->SetNPHOSClustersRange(0,10000);  
  
  TChain* chain = 0x0;
  chain = TagAna->QueryTags(runCuts,lhcCuts,detCuts,evCuts);
  *///  chain->SetBranchStatus("*Calo*",0);
  TChain *chain = new TChain("esdTree");
  chain->Add("/media/wd/data/LHC10b/10000116134019.10/AliESDs.root");

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

Int_t setupPar(const char* pararchivename) {
  ///////////////////
  // Setup PAR File//
  ///////////////////
  if (pararchivename) {
    char processline[1024];
    sprintf(processline,".! tar xvzf %s.par",pararchivename);
    gROOT->ProcessLine(processline);
    const char* ocwd = gSystem->WorkingDirectory();
    gSystem->ChangeDirectory(pararchivename);
                                                                                                                                               
    // check for BUILD.sh and execute
    if (!gSystem->AccessPathName("PROOF-INF/BUILD.sh")) {
      printf("*******************************\n");
      printf("*** Building PAR archive    ***\n");
      printf("*******************************\n");
                                                                                                                                               
      if (gSystem->Exec("PROOF-INF/BUILD.sh")) {
        Error("runAnalysis","Cannot Build the PAR Archive! - Abort!");
        return -1;
      }
    }
    // check for SETUP.C and execute
    if (!gSystem->AccessPathName("PROOF-INF/SETUP.C")) {
      printf("*******************************\n");
      printf("*** Setup PAR archive       ***\n");
      printf("*******************************\n");
      gROOT->Macro("PROOF-INF/SETUP.C");
    }
                                                                                                                                               
    gSystem->ChangeDirectory("../");
  }                                                                                                                                               
  return 1;
}
