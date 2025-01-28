#include "event.hh"
#include "G4SystemOfUnits.hh"
#include <chrono>
#include "Randomize.hh"
#include "variables.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0.;
	for (G4int i = 0; i < 192; ++i) {
        fEdep0[i] = 0.;
    }	
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*){
	//Set a new random seed based on the current system time
  	long seeds[2];
  	auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
  	seeds[0] = std::chrono::duration_cast<std::chrono::seconds>(now).count();
  	seeds[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
  	CLHEP::HepRandom::setTheSeeds(seeds);

	fEdep = 0.;
	
	for (G4int i = 0; i < 192; ++i) {
        fEdep0[i] = 0.;
    }
	
}

void MyEventAction::EndOfEventAction(const G4Event*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4double layer_dep=0;
    for (G4int i = 0; i < 192; ++i) {
        layer_dep=layer_dep + fEdep0[i] ;
    }
    man->FillNtupleDColumn( 0,0, layer_dep);
    man->AddNtupleRow(0);

    for (G4int i = 0; i <192; ++i) {
        man->FillNtupleDColumn( 1,i, fEdep0[i]);
    }
    man->AddNtupleRow(1);
}


void MyEventAction::AddEdep0(G4int copyNo, G4double edep) {
    if (copyNo >= 0 && copyNo <= 191) {
        fEdep0[copyNo] += edep; // Store energy deposition in corresponding array element
    }
}

