int totEtvstotEtCells(const char* filename = "Et.ESD.root")
{
  
  TFile* infile = TFile::Open(filename);
  
  TH2D* hist2d = new TH2D("totetvstotetcells", "(TotEtCells - TotEt)/TotEtCells vs TotETCells", 1000, 0, 10, 1000, 0, 10);
  TH1D* hist1d = new TH1D("totEtCellstotEt", "TotEtCells - TotEt", 1000, 0, 10);
  float totet = 0;
  float totetcells = 0;

  TNtuple* ntuple = dynamic_cast<TNtuple*>(infile->Get("fEtNtuple"));

  ntuple->SetBranchAddress("TotEt", &totet);
  ntuple->SetBranchAddress("TotEtCells", &totetcells);

  int nEntries = ntuple->GetEntries();

  cout << "Entries in nTuple: " << nEntries << endl;   
      
  for(int i = 0; i < nEntries; i++)
    {
      ntuple->GetEntry(i);
      hist1d->Fill(totetcells-totet);
      hist2d->Fill((totetcells-totet)/totetcells, totet);
    }

  //  TCanvas c1;
  hist1d->Draw();
  
  //  TCanvas c2;
  //hist2d->Draw();

  return 0;
}  
