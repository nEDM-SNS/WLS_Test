#include "globals.hh"
#include "G4RunManager.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "WLS_TestTrackingAction.hh"
#include "WLS_TestAnalysis.hh"

void WLS_TestTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    if (aTrack->GetParentID()==0) {
        // particle is a primary
        analysisManager->FillH1(0, 1);
    }
    
    else if(aTrack->GetParentID()>0){
        // particle is secondary
        
        if(aTrack->GetCreatorProcess()->GetProcessName()=="OpTPB")
        {
            analysisManager->FillH1(0, 2);
        }
        
        else if(aTrack->GetCreatorProcess()->GetProcessName()=="OpWLS")
        {
            analysisManager->FillH1(0, 3);
            analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            analysisManager->FillH1(2, aTrack->GetDynamicParticle()->GetKineticEnergy()/eV);
        }

        
    }
    
    else {
        G4cout << "!!!!! Negative Parent ID? !!!!" << G4endl;
    }

    
}


void WLS_TestTrackingAction::PostUserTrackingAction(const G4Track * aTrack)
{
    
}
