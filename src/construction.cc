#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // Define all materials
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Silicon= nist->FindOrBuildMaterial("G4_Si");
    G4Material *KaptonMat= nist->FindOrBuildMaterial("G4_KAPTON");
    G4Material *CopperMat= nist->FindOrBuildMaterial("G4_Cu");
    G4Material *TungstenMat= nist->FindOrBuildMaterial("G4_W");
    G4Material *LeadMat= nist->FindOrBuildMaterial("G4_Pb");
    G4Material *BaseplateMat =new G4Material("BaseplateMat", 16.715*g/cm3, 2);
    BaseplateMat->AddMaterial(TungstenMat,75*perCent);
    BaseplateMat->AddMaterial(CopperMat,25*perCent);
    G4Material *ScintillatorMat =nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    G4Material *AluminiumMat =nist->FindOrBuildMaterial("G4_Al");
    //Epoxy (for FR4 )
    //from http://www.physi.uni-heidelberg.de/~adler/TRD/TRDunterlagen/RadiatonLength/tgc2.htm //???
    G4Material* Epoxy = new G4Material("Epoxy" ,1.2*g/cm3 , 2);
    Epoxy->AddElement(nist->FindOrBuildElement("H"),2);
    Epoxy->AddElement(nist->FindOrBuildElement("C"),2);
    //SiO2 (Quarz)
    G4Material* SiO2 = new G4Material("SiO2",2.200*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O") , 2);

    //FR4 (Glass + Epoxy)
    G4Material* FR4 = new G4Material("FR4" , 1.86*g/cm3, 2);
    FR4->AddMaterial(Epoxy, 0.472);
    FR4->AddMaterial(SiO2, 0.528);
    //fr4Material = FR4;
    //Define all materials

    //Define center of cells in the wafer
    std::vector<G4ThreeVector> cells;

    G4double r = 101.6 / 12 * mm;
    for(G4int iu=0;iu<=7;iu++){
        for(G4int iv=0;iv<=iu+7;iv++){
            G4double x = ((iv - 7) + (7 - iu) * 0.5) * sqrt(3) * r;
            G4double y = ((7 - iu) * 0.5 ) * 3 * r +  1 * r;
            cells.emplace_back(x,y,0);
        }
    }
    for(G4int iu=8;iu<=15;iu++){
        for(G4int iv=iu-8;iv<=15;iv++){
            G4double x = ((iv - 7) + (7 - iu) * 0.5) * sqrt(3) * r;
            G4double y = ((7 - iu) * 0.5) * 3 * r + 1 * r;
            cells.emplace_back(x,y,0);
        }
    }
    //Define cells in the wafer
    
    G4Material *worldMat =nist->FindOrBuildMaterial("G4_Galactic");

    G4Box *solidWorld = new G4Box("solidWorld",20.0*m,20.0*m,20.0*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume( solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);

    // Define logic volume for Scintillators
    G4Box *Scintillator = new G4Box("Scintillator",1.0*cm,1.0*cm,0.5*cm);
    G4LogicalVolume *logicScintillator = new G4LogicalVolume( Scintillator, ScintillatorMat, "logicScintillator");
    // Define logic volume for Scintillators



	// Create and configure visualization attributes
	G4VisAttributes* ScintillatorVisAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0)); // Light Blue color
	ScintillatorVisAttr->SetVisibility(true); // Ensure the volume is visible
	ScintillatorVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicScintillator->SetVisAttributes(ScintillatorVisAttr);
	
    // Defining Logic volume for Aluminium
	G4Box *Aluminium = new G4Box("Aluminium",5.0*cm,5.0*cm,1.0*mm);
    G4LogicalVolume *logicAluminium = new G4LogicalVolume( Aluminium, AluminiumMat, "logicAluminium");



	// Create and configure visualization attributes
	G4VisAttributes* AluminiumVisAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // Grey
	AluminiumVisAttr->SetVisibility(true); // Ensure the volume is visible
	AluminiumVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicAluminium->SetVisAttributes(AluminiumVisAttr);
	
    // Defining Logic volume for Silicon Sensors
    G4double zPlane[] = { 0*um, 300*um }; // Z planes
    G4double rInner[] = { 0*cm, 0*cm };  // Inner radius
    G4double rOuter[] = { 6.98*mm, 6.98*mm };  // Outer radius
 
    G4Polyhedra* solidDetector = new G4Polyhedra("solidDetector", 0.*deg, 360.*deg, 6, 2, zPlane, rInner, rOuter);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(30.0 * CLHEP::degree);
    
    logicDetector = new G4LogicalVolume(solidDetector, Silicon, "logicDetector");
    fScoringVolume = logicDetector;
    // Define Logic Volume for Silicon Sensors

    // Define Logic Volume for PCB
    G4double zPlanePCB[] = { 0*um, 1.6*mm }; // Z planes
    G4double rInnerPCB[] = { 0*cm, 0*cm };  // Inner radius
    G4double rOuterPCB[] = { 101.6*mm, 101.6*mm };  // Outer radius
 
    G4Polyhedra* PCB = new G4Polyhedra("PCB", 0.*deg, 360.*deg, 6, 2, zPlanePCB, rInnerPCB, rOuterPCB);
    G4LogicalVolume *logicPCB = new G4LogicalVolume(PCB, FR4, "logicPCB");
    // Define Logic Volume for PCB

	// Create and configure visualization attributes
	G4VisAttributes* pcbVisAttr = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // Green color
	pcbVisAttr->SetVisibility(true); // Ensure the volume is visible
	pcbVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicPCB->SetVisAttributes(pcbVisAttr);

    // Defining Logic Volume for Kapton
    G4double zPlaneKapton[] = { 0*um, 300*um }; // Z planes
    G4double rInnerKapton[] = { 0*cm, 0*cm };  // Inner radius
    G4double rOuterKapton[] = { 101.6*mm, 101.6*mm };  // Outer radius
 
    G4Polyhedra* Kapton = new G4Polyhedra("Kapton", 0.*deg, 360.*deg, 6, 2, zPlaneKapton, rInnerKapton, rOuterKapton);

    G4LogicalVolume *logicKapton = new G4LogicalVolume(Kapton, KaptonMat, "logicKapton");
    
    
    // Create and configure visualization attributes
	G4VisAttributes* kaptonVisAttr = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0)); // Yellow color
	kaptonVisAttr->SetVisibility(true); // Ensure the volume is visible
	kaptonVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicKapton->SetVisAttributes(kaptonVisAttr);





    // Defining Logic Volume for Baseplate
    G4double zPlaneBaseplate[] = { 0*um, 2*mm }; // Z planes
    G4double rInnerBaseplate[] = { 0*cm, 0*cm };  // Inner radius
    G4double rOuterBaseplate[] = { 101.6*mm, 101.6*mm };  // Outer radius
 
    G4Polyhedra* Baseplate = new G4Polyhedra("Baseplate", 0.*deg, 360.*deg, 6, 2, zPlaneBaseplate, rInnerBaseplate, rOuterBaseplate);

    G4LogicalVolume *logicBaseplate = new G4LogicalVolume(Baseplate, BaseplateMat, "logicBaseplate");
    
    
    
    // Create and configure visualization attributes
	G4VisAttributes* baseVisAttr = new G4VisAttributes(G4Colour(0.6, 0.0, 0.0)); // Dark Yellow color
	baseVisAttr->SetVisibility(true); // Ensure the volume is visible
	baseVisAttr->SetForceSolid(true); // Render the volume as a solid object
    
    // Assign the visualization attributes to the logical volume
	logicBaseplate->SetVisAttributes(baseVisAttr);
    
    // Defining Logic Volume for Copper Plate
    G4double zPlaneCopper[] = { 0*um, 6*mm }; // Z planes
    G4double rInnerCopper[] = { 0*cm, 0*cm };  // Inner radius
    G4double rOuterCopper[] = { 101.6*mm, 101.6*mm };  // Outer radius
 
    G4Polyhedra* Copper_Plate = new G4Polyhedra("Copper_Plate", 0.*deg, 360.*deg, 6, 2, zPlaneCopper, rInnerCopper, rOuterCopper);

    G4LogicalVolume *logicCopper = new G4LogicalVolume(Copper_Plate, CopperMat, "logicCopper");
    
    
    
    // Create and configure visualization attributes
	G4VisAttributes* copperVisAttr = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)); // orange color
	copperVisAttr->SetVisibility(true); // Ensure the volume is visible
	copperVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicCopper->SetVisAttributes(copperVisAttr);

    // Define Logic Volume for Absorbers
    G4double Absorber_Thickness = LeadMat->GetRadlen() * 10;
 
    G4Box* Absorber_Plate = new G4Box("Absorber_Plate", 20. * cm, 20. * cm, Absorber_Thickness);

    G4LogicalVolume *logicAbsorber = new G4LogicalVolume(Absorber_Plate, LeadMat, "logicAbsorber");
    
    
    
    // Create and configure visualization attributes
	G4VisAttributes* AbsorberVisAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // grey color
	AbsorberVisAttr->SetVisibility(true); // Ensure the volume is visible
	AbsorberVisAttr->SetForceSolid(true); // Render the volume as a solid object

	// Assign the visualization attributes to the logical volume
	logicAbsorber->SetVisAttributes(AbsorberVisAttr);
    
    /* 
    Copy No. definition
    Scintilator:
        Absorber: 1000
        PCB: 1001
        BasePlate: 1002
        Copper: 1003
        Cells: 0-191
    
    */    
    G4double z = 0. * cm;
    G4VPhysicalVolume *physAbsorber = new G4PVPlacement(0,G4ThreeVector(0.,0.,z),logicAbsorber,"physAbsorber",logicWorld,false,1000,true);
    z += Absorber_Thickness + 10 * cm;
    G4VPhysicalVolume *physPCB =        new G4PVPlacement(0,G4ThreeVector(0.,   0., -1.6*mm +   z),logicPCB,"physPCB",logicWorld,false,1001,true);	
    G4VPhysicalVolume *physKapton =     new G4PVPlacement(0,G4ThreeVector(0.,   0., 300*um  +   z),logicKapton,"physKapton",logicWorld,false,1002,true);
    G4VPhysicalVolume *physBaseplate =  new G4PVPlacement(0,G4ThreeVector(0.,   0., 600*um  +   z),logicBaseplate,"physBaseplate",logicWorld,false,1003,true);		
    G4VPhysicalVolume *physCopper =     new G4PVPlacement(0,G4ThreeVector(0.,   0., 2.0*mm+600*um   +   z),logicCopper,"physCopper",logicWorld,false,1004,true);
    // for (G4int i=0;i<192;i++){
    //     G4VPhysicalVolume *physDetector = new G4PVPlacement(rotation,vectors1[i]+G4ThreeVector(0.,0.,z),logicDetector,"physDetector",logicWorld,false,i,true);
    // }
    for (G4int i=0;i<192;i++){
        G4VPhysicalVolume *physDetector = new G4PVPlacement(rotation,cells[i]+G4ThreeVector(0.,0.,z),logicDetector,"physDetector",logicWorld,false,i,true);
    }
    return physWorld;
}

void MyDetectorConstruction :: ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
}
