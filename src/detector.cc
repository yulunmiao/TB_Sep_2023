#include "detector.hh"
#include "event.hh"
MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack();
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	G4ThreeVector posPhoton =preStepPoint->GetPosition();
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
	
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	if (evt % 1000==0 && copyNo==1169){
	G4cout << "Event: " << evt <<G4endl;}
	G4double edep = aStep->GetTotalEnergyDeposit();  
	return true;
}
