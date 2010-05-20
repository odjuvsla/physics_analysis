void ConfigureCuts(AliRunTagCuts *runCuts, 
                   AliLHCTagCuts *lhcCuts, 
                   AliDetectorTagCuts *detCuts,
                   AliEventTagCuts *evCuts) 
{
// Configure cuts. This file is to be used and modified for plug-in based
// grid analysis on ESD tag files. In all other cases it will be ignored.
// ##########
// Note that the cuts in this case are applied run-time to select ESD events.
// ##########
   //evCuts->SetMultiplicityRange(10,20);
}
         
