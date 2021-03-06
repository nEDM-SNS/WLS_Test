#include "globals.hh"
#include "G4RunManager.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "WLS_TestTrackingAction.hh"
#include "WLS_TestTrackInformation.hh"
#include "WLS_TestAnalysis.hh"

void WLS_TestTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    WLS_TestTrackInformation* trackInfo = new WLS_TestTrackInformation();
    
    fpTrackingManager->SetUserTrackInformation(trackInfo);

    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    if (aTrack->GetParentID()==0) {
        // particle is a primary
        analysisManager->FillH1(0, 1);
    }
    
    else if(aTrack->GetParentID()>0){
        // particle is secondary
        
        
        if (aTrack->GetCreatorProcess()->GetProcessName() == "OpWLS") {
 
            
            if (aTrack->GetOriginTouchable()->GetVolume()->GetName()=="TPBInterface") {
                analysisManager->FillH1(0, 2);
                analysisManager->FillH1(0, 4);
                analysisManager->FillH1(2, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }
            else if (aTrack->GetOriginTouchable()->GetVolume()->GetName()=="TPBInterface_outer")
            {
                analysisManager->FillH1(0, 3);
                analysisManager->FillH1(0, 4);
                analysisManager->FillH1(2, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }
            else
            {
                analysisManager->FillH1(0, 5);
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            
            }
            
        }
        else{
            G4cout << "Process Name = " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
        }
        
        
    }
    
    else {
        G4cout << "!!!!! Negative Parent ID? !!!!" << G4endl;
    }

    
}


void WLS_TestTrackingAction::PostUserTrackingAction(const G4Track * aTrack)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    WLS_TestTrackInformation* trackInfo = (WLS_TestTrackInformation*)aTrack->GetUserInformation();
    
    G4int status = trackInfo->GetStatus();
    
    analysisManager->FillH1(5, status);

    if (status == 3 || status ==4 || status == 5) {
        analysisManager->FillH1(5, 9);
    }
    else if (status == 1 || status ==2)
    {
        analysisManager->FillH1(5, 8);
    }
}
