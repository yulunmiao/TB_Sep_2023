#include "run.hh"
#include "variables.hh"

#include <sstream>

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
        // G4AnalysisManager *man = G4AnalysisManager::Instance();
        // man->OpenFile("100GeV_Electron_Run_10000_Events.root");

        // man->CreateNtuple("Total_E_in_Wafers","Total_E_in_wafers_in_MeV");
        // man->CreateNtupleDColumn("Wafer1");
        // man->FinishNtuple(0);

        // man->CreateNtuple("Layer_1","Layer_1");
        // for(G4int k =0 ;k<192;k++){
        //         std::stringstream ss;   
        //         ss << "Pad_" << k;
        //         man->CreateNtupleDColumn(ss.str());
        // }
        // man->FinishNtuple(1);

        G4AnalysisManager *man = G4AnalysisManager::Instance();
        std::stringstream fileStream;
        fileStream << std::setprecision(2) 
                << "E_" << (int)beamE << "GeV_" 
                << particleName<<"_"
                << "X_" << beamX << "cm_" 
                << "Y_" << beamY << "cm_"
                << "Run_" << nEvents << ".root";
        std::string fileName = fileStream.str();
        // fileName.replace(fileName.begin(), fileName.end(), ".", "p");
        // fileName += ".root";
        G4cout<<fileName<<std::endl;
        man->OpenFile(fileName);
        man->CreateNtuple("Events","");
        for(G4int k =0 ;k<192;k++){
                std::stringstream ss;   
                ss << "Cell_" << k;
                man->CreateNtupleDColumn(ss.str());
        }
        man->FinishNtuple(0);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->Write();
        man->CloseFile();
}

