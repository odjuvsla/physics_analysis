//_________________________________________________________________________
// Macro to create the tag files from:
// i)  AliESDs that are stored locally.
// ii) AliESDs that are stored in the CAF.
// iii)AliESDs that are stored in alien.
// The tag files are stored according to the value set in the SetStorage method
// Use Case : 
//          SetStorage(0) --> store the tags locally
//          SetStorage(1) --> store the tags in the grid
//          else the program will abort!!!                                    
//                                             
// As a final step the user can create a single merged tag file.         
//_________________________________________________________________________
void CreateTags() {
  //___________________________________//
  //create an AliTagCreator object
  AliESDTagCreator *t = new AliESDTagCreator(); 
    
  //___________________________________//
  //_____ Storage of the tag files:____//
  //________0-->local, 1-->alien_______//
  //___________________________________//
  t->SetStorage(0);
  //Define the grid's path where the tag files will be stored
  //t->SetGridPath("Tags/output");
  //Define the SE where the tag files will be stored
  //t->SetSE("ALICE::CERN::se01");

  //___________________________________//
  //________Locally stored ESDs________//
  //___________________________________//
  //create the tag files for the locally stored ESDs
  t->ReadLocalCollection("../ESDs/");
  //  t->ReadLocalCollection("../ESDs/960/");
//   t->ReadLocalCollection("../ESDs/961/");
//   t->ReadLocalCollection("../ESDs/962/");
//   t->ReadLocalCollection("../ESDs/963/");
//   t->ReadLocalCollection("../ESDs/964/");
//   t->ReadLocalCollection("../ESDs/965/");
//   t->ReadLocalCollection("../ESDs/966/");
//   t->ReadLocalCollection("../ESDs/967/");
//   t->ReadLocalCollection("../ESDs/968/");
//   t->ReadLocalCollection("../ESDs/969/");  
//   t->ReadLocalCollection("../ESDs/970/");  
//   t->ReadLocalCollection("../ESDs/988/");

  //___________________________________//
  //___________Merge the tags__________//
  //___________________________________//
  //Merge the tags and store the merged file
  t->MergeTags("ESD");
}

