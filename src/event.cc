#include "event.hh"
#include "G4SystemOfUnits.hh"
#include <chrono>
#include "Randomize.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0.;
	for (G4int i = 0; i < 192*2; ++i) {
        fEdep0[i] = 0.;
    }	
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
	
	// Set a new random seed based on the current system time
  	long seeds[2];
  	auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
  	seeds[0] = std::chrono::duration_cast<std::chrono::seconds>(now).count();
  	seeds[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
  	CLHEP::HepRandom::setTheSeeds(seeds);

	fEdep = 0.;
	
	for (G4int i = 0; i < 192*2; ++i) {
        fEdep0[i] = 0.;
    }
	
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
G4AnalysisManager *man = G4AnalysisManager::Instance();

  for (G4int j=1; j < 3 ;j++){
    G4double layer_dep=0;
    for (G4int i = 1000*j+0; i < 1000*j+192; ++i) {
        layer_dep=layer_dep + fEdep0[i-1000*j+192*(j-1)] ;
    }
        man->FillNtupleDColumn( 0,j-1, layer_dep);
    }

 man->AddNtupleRow(0);




  for (G4int i = 0; i <192; ++i) {
       man->FillNtupleDColumn( 1,i, fEdep0[i]);
    }
    man->AddNtupleRow(1);


  for (G4int i = 0; i <192; ++i) {
       man->FillNtupleDColumn( 2,i, fEdep0[i+192]);
    }
    man->AddNtupleRow(2);
    
    
    if(fEdep0[169]!=0){
    man->FillNtupleDColumn( 3,0, fEdep0[169]);
    man->FillNtupleDColumn( 3,1, fEdep0[41]);
    man->FillNtupleDColumn( 3,2, fEdep0[42]);
    man->FillNtupleDColumn( 3,3, fEdep0[105]);
    man->FillNtupleDColumn( 3,4, fEdep0[168]);
    man->FillNtupleDColumn( 3,5, fEdep0[170]);
    man->FillNtupleDColumn( 3,6, fEdep0[171]);
    
    
    man->FillNtupleDColumn( 3,7, fEdep0[40]);
    man->FillNtupleDColumn( 3,8, fEdep0[43]);
    man->FillNtupleDColumn( 3,9, fEdep0[45]);
    man->FillNtupleDColumn( 3,10, fEdep0[46]);
    man->FillNtupleDColumn( 3,11, fEdep0[104]);
    man->FillNtupleDColumn( 3,12, fEdep0[106]);
    man->FillNtupleDColumn( 3,13, fEdep0[107]);
    man->FillNtupleDColumn( 3,14, fEdep0[161]);
    man->FillNtupleDColumn( 3,15, fEdep0[163]);
    man->FillNtupleDColumn( 3,16, fEdep0[165]);
    man->FillNtupleDColumn( 3,17, fEdep0[173]);
    man->FillNtupleDColumn( 3,18, fEdep0[174]);
    man->AddNtupleRow(3);
    }
    
    
    
    if(fEdep0[192+169]!=0){
    man->FillNtupleDColumn( 4,0, fEdep0[192+169]);
    man->FillNtupleDColumn( 4,1, fEdep0[192+41]);
    man->FillNtupleDColumn( 4,2, fEdep0[192+42]);
    man->FillNtupleDColumn( 4,3, fEdep0[192+105]);
    man->FillNtupleDColumn( 4,4, fEdep0[192+168]);
    man->FillNtupleDColumn( 4,5, fEdep0[192+170]);
    man->FillNtupleDColumn( 4,6, fEdep0[192+171]);
    
    
    man->FillNtupleDColumn( 4,7, fEdep0[192+40]);
    man->FillNtupleDColumn( 4,8, fEdep0[192+43]);
    man->FillNtupleDColumn( 4,9, fEdep0[192+45]);
    man->FillNtupleDColumn( 4,10, fEdep0[192+46]);
    man->FillNtupleDColumn( 4,11, fEdep0[192+104]);
    man->FillNtupleDColumn( 4,12, fEdep0[192+106]);
    man->FillNtupleDColumn( 4,13, fEdep0[192+107]);
    man->FillNtupleDColumn( 4,14, fEdep0[192+161]);
    man->FillNtupleDColumn( 4,15, fEdep0[192+163]);
    man->FillNtupleDColumn( 4,16, fEdep0[192+165]);
    man->FillNtupleDColumn( 4,17, fEdep0[192+173]);
    man->FillNtupleDColumn( 4,18, fEdep0[192+174]);
    man->AddNtupleRow(4);
    }
}


void MyEventAction::AddEdep0(G4int copyNo, G4double edep) {
for (G4int i=1; i < 3 ;i++){
    if (copyNo >= 1000*i+0 && copyNo < 1000*i+192) {
        fEdep0[copyNo-1000*i+192*(i-1)] += edep; // Store energy deposition in corresponding array element
        
    }
    }
    
}

