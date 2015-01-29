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
    //const G4VProcess *p1 = p_in->GetProcessDefinedStep();
    //const G4VProcess *p2 = p_out->GetProcessDefinedStep();

/*    const G4VProcess * creatorProcess = aStep->GetTrack()->GetCreatorProcess();
    if (creatorProcess && aStep->GetTrack()->GetCurrentStepNumber() == 1) {
            G4cout << "Creator Process: " << aStep->GetTrack()->GetCreatorProcess()->GetProcessName() << G4endl;
    }
    else { G4cout << "!!!NULL Creator Process!!!" << G4endl;}
*/
    
    if (aStep->GetTrack()->GetCurrentStepNumber() != 1 && p_out->GetStepStatus()!= fWorldBoundary) {
        G4LogicalVolume* inVol;
        inVol = p_in->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        G4LogicalVolume* outVol;
        outVol = p_out->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        if( inVol->GetName() == "Fiber" && outVol->GetName() == "expHall_log") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            G4double cosTheta = aStep->GetTrack()->GetMomentumDirection().x();
            
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
            /*
            G4cout << "Position: " << p_out->GetPosition() << G4endl;
            G4cout << "PreStepMomDir: " <<  p_in->GetMomentumDirection() << G4endl;
            G4cout << "PostStepMomDir: " <<  p_out->GetMomentumDirection() << G4endl;
            
            G4cout << "Step Process" << p_out->GetProcessDefinedStep()->GetProcessName() << G4endl;
             */
            
            
        }

    }
    
    
}

