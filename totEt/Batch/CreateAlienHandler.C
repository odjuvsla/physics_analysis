
AliAnalysisGrid* CreateAlienHandler()
{
// Check if user has a valid token, otherwise make one. This has limitations.
// One can always follow the standard procedure of calling alien-token-init then
//   source /tmp/gclient_env_$UID in the current shell.
   if (!AliAnalysisGrid::CreateToken()) return NULL;
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
// Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode("full");
   //plugin->SetRunMode("terminate");
// Set versions of used packages
   plugin->SetAPIVersion("V1.1x");
   plugin->SetROOTVersion("v5-26-00b-6");
   plugin->SetAliROOTVersion("v4-20-05-AN");
// Declare input data to be processed.
// Method 1: Create automatically XML collections using alien 'find' command.
// Define production directory LFN
   //plugin->SetGridDataDir("/alice/sim/LHC10a6");
   // On real reconstructed data:
   
 plugin->SetGridDataDir("/alice/data/2010/LHC10e");
   // plugin->SetGridDataDir("/alice/data/2010/LHC10b");
// Set data search pattern
 //  plugin->SetDataPattern("*ESDs.root");
// Data pattern for reconstructed data
   plugin->SetDataPattern("*ESDs/pass1/*ESDs.root");
   plugin->SetRunPrefix("000");   // real data
// ...then add run numbers to be considered
   //plugin->AddRunNumber(125020);
//   plugin->AddRunNumber(104065);  // real data
   //  plugin->AddRunNumber(116197);
   //plugin->AddRunNumber(117112);
//plugin->AddRunNumber(119067);
   plugin->AddRunNumber(128369);
//   plugin->AddRunNumber(128452);
   //plugin->AddRunNumber(128486);
   //plugin->AddRunNumber(128498);
   //plugin->AddRunNumber(128503);
   //plugin->AddRunNumber(128504);
   
   
   
//   plugin->SetOutputSingleFolder("output");
//   plugin->SetOutputToRunNo();
// Method 2: Declare existing data files (raw collections, xml collections, root file)
// If no path mentioned data is supposed to be in the work directory (see SetGridWorkingDir())
// XML collections added via this method can be combined with the first method if
// the content is compatible (using or not tags)
//   plugin->AddDataFile("tag.xml");
//   plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");
// Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir("analysis/totEt2");
// Declare alien output directory. Relative to working directory.
   plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
// Declare the analysis source files names separated by blancs. To be compiled runtime
// using ACLiC on the worker nodes.
   //plugin->SetAnalysisSource("AliAnalysisEt.cxx AliAnalysisEtReconstructed.cxx AliAnalysisEtMonteCarlo.cxx AliAnalysisEtReconstructedPhos.cxx AliAnalysisEtMonteCarloPhos.cxx AliAnalysisTaskTotEt.cxx");
   plugin->SetAnalysisSource("AliAnalysisTaskTotEt.cxx");
// Declare all libraries (other than the default ones for the framework. These will be
// loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
   plugin->SetAdditionalLibs("AliAnalysisEtCuts.h AliAnalysisEt.h AliAnalysisEtReconstructed.h AliAnalysisEtMonteCarlo.h AliAnalysisEtReconstructedPhos.h AliAnalysisEtMonteCarloPhos.h AliAnalysisEt.h AliAnalysisEtReconstructed.h AliAnalysisEtMonteCarlo.h AliAnalysisEtReconstructedPhos.h AliAnalysisEtMonteCarloPhos.h AliAnalysisEt.cxx AliAnalysisEtReconstructed.cxx AliAnalysisEtMonteCarlo.cxx AliAnalysisEtReconstructedPhos.cxx AliAnalysisEtMonteCarloPhos.cxx AliAnalysisTaskTotEt.h AliAnalysisTaskTotEt.cxx");
// Declare the output file names separated by blancs.
// (can be like: file.root or file.root@ALICE::Niham::File)
   //plugin->SetDefaultOutputs(kFALSE);
   plugin->SetDefaultOutputs();
   //plugin->SetOutputFiles("Et.ESD.root");
// Optionally define the files to be archived.
   //plugin->SetOutputArchive("log_archive.zip:stdout,stderr@ALICE::NIHAM::File root_archive.zip:*.root@ALICE::NIHAM::File");
   //plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
// Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro("TaskTotEt.C");
// Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   plugin->SetSplitMaxInputFileNumber(100);
// Optionally modify the executable name (default analysis.sh)
   plugin->SetExecutable("TaskTotEt.sh");
// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
//   plugin->SetMaxInitFailed(5);
// Optionally resubmit threshold.
//   plugin->SetMasterResubmitThreshold(90);
// Optionally set time to live (default 30000 sec)
   plugin->SetTTL(30000);
// Optionally set input format (default xml-single)
   plugin->SetInputFormat("xml-single");
// Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskTotEt.jdl");
// Optionally modify job price (default 1)
   plugin->SetPrice(1);      
// Optionally modify split mode (default 'se')    
   plugin->SetSplitMode("se");
   return plugin;
}
