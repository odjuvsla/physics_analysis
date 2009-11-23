void ana()
{
  // Main
  LoadLibraries(kFALSE) ; 

  TChain * chain   = new TChain("esdTree") ;
  //chain->AddFile("/data/test/AliESDs.root");
  chain->AddFile("/home/odjuvsla/Workspace/alice/simulations/AliRoot-v4-18-10-AN-sim/pp/AliESDs.root");
  

  cout<<"---->>>> CHAIN CREATED? "<<chain->GetEntries()<<endl;

    AliLog::SetGlobalLogLevel(AliLog::kError);//Minimum prints on screen
    
    //--------------------------------------
    // Make the analysis manager
    //-------------------------------------
    AliAnalysisManager *mgr  = new AliAnalysisManager("Jet Manager", "Jet Manager");

    cout <<"---->>>> MC HANDLER INITIALIZED"<<endl;
  
      // ESD handler
      AliESDInputHandler *esdHandler = new AliESDInputHandler();
      mgr->SetInputEventHandler(esdHandler);

    cout <<"---->>>> ESD INPUT HANDLER INITIALIZED"<<endl; 

    //mgr->SetDebugLevel(3); // For debugging, do not uncomment if you want no messages.
   
    cout <<"---->>>> INIT TASK"<<endl;

    gSystem->AddIncludePath("-I$ALICE_ROOT/include");
    
    gROOT->LoadMacro("AliAnalysisTaskTotEt.cxx+g");

    AliAnalysisTaskTotEt *task1 = new AliAnalysisTaskTotEt("TaskTotEt");
    mgr->AddTask(task1);



  // Enable MC event handler
    AliMCEventHandler* handler = new AliMCEventHandler;
    handler->SetReadTR(kFALSE);
    mgr->SetMCtruthEventHandler(handler);


  

    // Get common containers for input
    AliAnalysisDataContainer *cinput1  = mgr->GetCommonInputContainer();
    mgr->ConnectInput (task1,0,cinput1);
    
    //Output containers
    AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("chist1", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    AliAnalysisDataContainer *coutput3 = mgr->CreateContainer("chist2", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    AliAnalysisDataContainer *coutput4 = mgr->CreateContainer("chist3", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    AliAnalysisDataContainer *coutput5 = mgr->CreateContainer("chist4", TH1::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    AliAnalysisDataContainer *coutput6 = mgr->CreateContainer("cnt", TNtuple::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    AliAnalysisDataContainer *coutput7 = mgr->CreateContainer("cnt2", TNtuple::Class(), AliAnalysisManager::kOutputContainer,"Et.ESD.root");
    
    mgr->ConnectOutput(task1,1,coutput2);
    mgr->ConnectOutput(task1,2,coutput3);
    mgr->ConnectOutput(task1,3,coutput4);
    mgr->ConnectOutput(task1,4,coutput5);
    mgr->ConnectOutput(task1,5,coutput6);
    mgr->ConnectOutput(task1,6,coutput7);

    //-----------------------
    // Run the analysis
    //-----------------------  
    cout <<"---->>>> RUN THE ANALYSIS"<<endl; 
    
    //mgr->ResetAnalysis();
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("local",chain);
    
    cout <<" Analysis ended sucessfully "<< endl ;

}

void  LoadLibraries( bool usepar = kFALSE) {
 

    //--------------------------------------
    // Load the needed libraries most of them already loaded by aliroot
    //--------------------------------------
    gSystem->Load("libTree.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libVMC.so");
    gSystem->Load("libXMLIO.so");
    //gSystem->Load("libCGAL");
    //gSystem->Load("libfastjet");
    //gSystem->Load("libSISConePlugin");

    if(usepar){
      //--------------------------------------------------------
      //If you want to use root and par files from aliroot
      //--------------------------------------------------------  
      SetupPar("STEERBase");
      SetupPar("ESD");
      SetupPar("AOD");
      SetupPar("ANALYSIS");
      SetupPar("ANALYSISalice");
      //cerr<<"Now Loading JETAN"<<endl;
      //SetupPar("JETAN");
      //cerr<<"Done Loading JETAN"<<endl;
      //cerr<<"Now Loading FASTJETAN"<<endl;
      //SetupPar("FASTJETAN");
      //cerr<<"Done Loading FASTJETAN"<<endl;
      //SetupPar("PHOSUtils");
      //SetupPar("EMCALUtils");
      //SetupPar("PWG4PartCorrBase");
      //SetupPar("PWG4PartCorrDep");
    }
    else{
      //--------------------------------------------------------
      // If you want to use already compiled libraries 
      // in the aliroot distribution
      //--------------------------------------------------------
      gSystem->Load("libSTEERBase");
      gSystem->Load("libESD");
      gSystem->Load("libAOD");
      gSystem->Load("libANALYSIS");
      gSystem->Load("libANALYSISalice");
      //gSystem->Load("libEMCALUtils");
      //gSystem->Load("libPHOSUtils");
      //gSystem->Load("libJETAN");
      //gSystem->Load("libFASTJETAN");
      //gSystem->Load("libPWG4PartCorrBase");
      //gSystem->Load("libPWG4PartCorrDep");
    }


}

void SetupPar(char* pararchivename)
{
  //Load par files, create analysis libraries
  //For testing, if par file already decompressed and modified
  //classes then do not decompress.
 
  TString cdir(Form("%s", gSystem->WorkingDirectory() )) ;  
  TString parpar(Form("%s.par", pararchivename)) ; 
  if ( gSystem->AccessPathName(pararchivename) ) { 
    TString processline = Form(".! tar xvzf %s",parpar.Data()) ;
    gROOT->ProcessLine(processline.Data());
  }
  
  TString ocwd = gSystem->WorkingDirectory();
  gSystem->ChangeDirectory(pararchivename);
  
  // check for BUILD.sh and execute
  if (!gSystem->AccessPathName("PROOF-INF/BUILD.sh")) {
    printf("*******************************\n");
    printf("*** Building PAR archive    ***\n");
    cout<<pararchivename<<endl;
    printf("*******************************\n");
    
    if (gSystem->Exec("PROOF-INF/BUILD.sh")) {
      Error("runProcess","Cannot Build the PAR Archive! - Abort!");
      return -1;
    }
  }
  // check for SETUP.C and execute
  if (!gSystem->AccessPathName("PROOF-INF/SETUP.C")) {
    printf("*******************************\n");
    printf("*** Setup PAR archive       ***\n");
    cout<<pararchivename<<endl;
    printf("*******************************\n");
    gROOT->Macro("PROOF-INF/SETUP.C");
  }
  
  gSystem->ChangeDirectory(ocwd.Data());
  printf("Current dir: %s\n", ocwd.Data());
}
