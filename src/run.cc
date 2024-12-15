#include "run.hh"
#include <sstream>

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
G4AnalysisManager *man = G4AnalysisManager::Instance();
man->OpenFile("100GeV_Electron_Run_10000_Events.root");

man->CreateNtuple("Total_E_in_layers","Total_E_in_layers_in_MeV");
man->CreateNtupleDColumn("Layer_1");
man->CreateNtupleDColumn("Layer_2");
man->FinishNtuple(0);

man->CreateNtuple("Layer_1","Layer_1");
for(G4int k =0 ;k<192;k++){
std::stringstream ss;
        ss << "Pad_" << k;
man->CreateNtupleDColumn(ss.str());
}
man->FinishNtuple(1);

man->CreateNtuple("Layer_2","Layer_2");
for(G4int k =0 ;k<192;k++){
std::stringstream ss;
        ss << "Pad_" << k;
man->CreateNtupleDColumn(ss.str());
}
man->FinishNtuple(2);



man->CreateNtuple("Layer_1_seed_and_nearest_neighbour","Layer_1_seed_and_nearest_neighbour");
for(G4int k =0 ;k<19;k++){
std::stringstream ss;
        ss << "Cell_" << k <<"Layer_1";
man->CreateNtupleDColumn(ss.str());
}
man->FinishNtuple(3);


man->CreateNtuple("Layer_2_seed_and_nearest_neighbour","Layer_2_seed_and_nearest_neighbour");
for(G4int k =0 ;k<19;k++){
std::stringstream ss;
        ss << "Cell_" << k <<"Layer_2";
man->CreateNtupleDColumn(ss.str());
}
man->FinishNtuple(4);

}

void MyRunAction::EndOfRunAction(const G4Run*)
{
G4AnalysisManager *man = G4AnalysisManager::Instance();
man->Write();
man->CloseFile();

}

