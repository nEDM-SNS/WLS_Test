#include "WLS_TestSteppingAction.hh"

#include "WLS_TestAnalysis.hh"

#include "G4Step.hh"
#include "G4StepStatus.hh"

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

    if (aStep->GetTrack()->GetCurrentStepNumber() != 1 && p_out->GetStepStatus()!= fWorldBoundary) {
        G4LogicalVolume* inVol;
        inVol = p_in->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        G4LogicalVolume* outVol;
        outVol = p_out->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        if( inVol->GetName() == "Fiber" && outVol->GetName() == "expHall_log") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            if (p_out->GetPosition().x() > 0.) {
                analysisManager->FillH1(0, 1);
            }
            else
            {
                analysisManager->FillH1(0, 2);
            }
            
            //G4cout << "Position: " << p_out->GetPosition() << G4endl;
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }

    }
    
    
}

