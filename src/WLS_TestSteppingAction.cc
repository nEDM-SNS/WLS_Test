#include "WLS_TestSteppingAction.hh"

#include "WLS_TestAnalysis.hh"

#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4VProcess.hh"


void WLS_TestSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif

    
    // get point of entry and exit
    const G4StepPoint *p_in  = aStep->GetPreStepPoint();
    const G4StepPoint *p_out = aStep->GetPostStepPoint();

    // Ignore 1st and last step
    if (aStep->GetTrack()->GetCurrentStepNumber() != 1 && p_out->GetStepStatus()!= fWorldBoundary) {
        G4LogicalVolume* inVol;
        inVol = p_in->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        G4LogicalVolume* outVol;
        outVol = p_out->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        if( (inVol->GetName() == "Fiber" || inVol->GetName() == "Mirror") && outVol->GetName() == "expHall_log") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            G4double cosTheta = aStep->GetTrack()->GetMomentumDirection().x();
            
// If the following flag is set, the photons that reflect from the end of the tube propagate as normal
#if 0
            if (p_out->GetPosition().x() > 0. && cosTheta > 0.) {
                analysisManager->FillH1(0, 6);
                analysisManager->FillH1(3, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
            }
            else if (p_out->GetPosition().x() < 0. && cosTheta < 0.)
            {
                analysisManager->FillH1(0, 7);
                analysisManager->FillH1(4, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else {
                // G4cout << "!!!!! Reflection !!!!!" << G4endl;
            }
            
// If the above Flag is not set, then any photon that hits the end of the fiber will be counted as detected and killed
#else
            if (p_out->GetPosition().x() > 0.) {
                analysisManager->FillH1(0, 6);
                analysisManager->FillH1(3, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else {
                analysisManager->FillH1(0, 7);
                analysisManager->FillH1(4, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
            }
            
#endif
            
            // For debugging above
            /*
            G4cout << "Position: " << p_out->GetPosition() << G4endl;
            G4cout << "PreStepMomDir: " <<  p_in->GetMomentumDirection() << G4endl;
            G4cout << "PostStepMomDir: " <<  p_out->GetMomentumDirection() << G4endl;
            
            G4cout << "Step Process" << p_out->GetProcessDefinedStep()->GetProcessName() << G4endl;
             */
            
            
        }

    }
    
    
}

