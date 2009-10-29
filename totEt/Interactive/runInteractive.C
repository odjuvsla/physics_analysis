void runInteractive() {
  TStopwatch timer;
  timer.Start();
  gSystem->Load("libProofPlayer.so");

  printf("*** Connect to AliEn ***\n");
  TGrid::Connect("alien://", "odjuvsla");
  //  TGrid::Connect("alien://");
  
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
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISalice.so");

  gROOT->LoadMacro("AliAnalysisTaskTotEt.cxx+");
  
  //____________________________________________//
  AliTagAnalysis *TagAna = new AliTagAnalysis("ESD");
 
  AliRunTagCuts *runCuts = new AliRunTagCuts();
  AliLHCTagCuts *lhcCuts = new AliLHCTagCuts();
  AliDetectorTagCuts *detCuts = new AliDetectorTagCuts();
  AliEventTagCuts *evCuts = new AliEventTagCuts();
  //  evCuts->SetMultiplicityRange(11,12);
  //  evCuts->SetNPHOSClustersRange(1,100);  
  //grid tags
  TAlienCollection* coll = TAlienCollection::Open("pp_test.xml");
  TGridResult* TagResult = coll->GetGridResult("",0,0);
  TagAna->ChainGridTags(TagResult);
  TChain* chain = 0x0;
  chain = TagAna->QueryTags(runCuts,lhcCuts,detCuts,evCuts);
  // chain->SetBranchStatus("*Calo*",0);

  //____________________________________________//
  // Make the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("TestManager");
  AliVEventHandler* esdH = new AliESDInputHandler;
  mgr->SetInputEventHandler(esdH);  

  AliAnalysisTaskTotEt *task1 = new AliAnalysisTaskTotEt("TaskTotEt");
  mgr->AddTask(task1);
  // Create containers for input/output
  AliAnalysisDataContainer *cinput1 = mgr->CreateContainer("cchain1", TChain::Class(),AliAnalysisManager::kInputContainer);
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist1", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("chist2", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
  AliAnalysisDataContainer *coutput3 = mgr->CreateContainer("chist3", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
  AliAnalysisDataContainer *coutput4 = mgr->CreateContainer("chist4", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
  AliAnalysisDataContainer *coutput5 = mgr->CreateContainer("cnt", TNtuple::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
  //  AliAnalysisDataContainer *coutput4 = mgr->CreateContainer("carray1", TClonesArray::Class(), AliAnalysisManager::kOutputContainer,"Pt.ESD.root");
    //____________________________________________//
  mgr->ConnectInput(task1,0,cinput1);
  mgr->ConnectOutput(task1,1,coutput1);
  mgr->ConnectOutput(task1,2,coutput2);
  mgr->ConnectOutput(task1,3,coutput3);
  mgr->ConnectOutput(task1,4,coutput4);
  mgr->ConnectOutput(task1,5,coutput5);
  //  mgr->ConnectOutput(task1,3,coutput4);

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
