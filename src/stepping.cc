#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{

fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

if(volume != fScoringVolume)
	return;
G4double edep = step->GetTotalEnergyDeposit();
 // Get the copy number of the volume
    G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();


for (G4int i=1; i < 3 ;i++){
    // Add the energy deposition to the event action
    if (copyNo >= 1000*i+0 && copyNo < 1000*i+192) {
        fEventAction->AddEdep0(copyNo, edep);
    }
    }
}
