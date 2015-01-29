#include "WLS_TestStackingAction.hh"
//#include "WLS_TestEventInformation.hh"
#include "WLS_TestSteppingAction.hh"
#include "WLS_TestAnalysis.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestStackingAction::WLS_TestStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestStackingAction::~WLS_TestStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
WLS_TestStackingAction::ClassifyNewTrack(const G4Track * aTrack){
    
    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // The following is now implemented in the Tracking Action
    /*
     
    if (aTrack->GetParentID()==0) {
        analysisManager->FillH1(0, 1);
    }

    //Count what process generated the optical photons
    if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
        // particle is optical photon
        if(aTrack->GetParentID()>0){
            // particle is secondary

            if(aTrack->GetCreatorProcess()->GetProcessName()=="OpWLS")
            {
                analysisManager->FillH1(0, 2);
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
                analysisManager->FillH1(2, aTrack->GetDynamicParticle()->GetKineticEnergy()/eV);
            }

        }
    }
     
    else{}
    */
    
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestStackingAction::NewStage() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestStackingAction::PrepareNewEvent() {}
