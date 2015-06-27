#include "WLS_TestSteppingAction.hh"

#include "WLS_TestAnalysis.hh"

#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4VProcess.hh"

#include "G4ParallelWorldProcess.hh"
#include "G4TransportationManager.hh"



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

    // Ignore steps at world boundary
    if (p_out->GetStepStatus()!= fWorldBoundary) {
        G4LogicalVolume* inVol;
        inVol = p_in->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        G4LogicalVolume* outVol;
        outVol = p_out->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        if( (inVol->GetName() == "Fiber" || inVol->GetName() == "Mirror") && outVol->GetName() == "expHall_log") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();


//New default behavior is to record all photons that reach the end of the fiber, and read out the angle with which they HIT the fiber.  This should mimick optical coupling much better.
            G4double cosTheta = p_in->GetMomentumDirection().z();
            
            if (p_out->GetPosition().z() > 0.) {
                analysisManager->FillH1(0, 6);
                analysisManager->FillH1(3, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
            }
            else if (p_out->GetPosition().z() < 0.)
            {
                analysisManager->FillH1(0, 7);
                analysisManager->FillH1(4, cosTheta);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            
            else
            {
                G4cout << "!!! Recorded Photon not exiting Fiber End !!!!" << G4endl;
            }

            
        
            
            // For debugging above
            /*
            G4cout << "Position: " << p_out->GetPosition() << G4endl;
            G4cout << "PreStepMomDir: " <<  p_in->GetMomentumDirection() << G4endl;
            G4cout << "PostStepMomDir: " <<  p_out->GetMomentumDirection() << G4endl;
            
            G4cout << "Step Process" << p_out->GetProcessDefinedStep()->GetProcessName() << G4endl;
             */
            
            
        }

        else if( (inVol->GetName() == "Cell" || inVol->GetName() == "TPBInterface") && outVol->GetName() == "expHall_log") {
            
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            G4ThreeVector theGlobalPoint = p_out->GetPosition();
            
            G4bool valid;
            //  Use the new method for Exit Normal in global coordinates,
            //    which provides the normal more reliably.
            
            // ID of Navigator which limits step
            
            G4int hNavId = G4ParallelWorldProcess::GetHypNavigatorID();
            std::vector<G4Navigator*>::iterator iNav =
            G4TransportationManager::GetTransportationManager()->
            GetActiveNavigatorsIterator();
            G4ThreeVector theGlobalNormal =
            (iNav[hNavId])->GetGlobalExitNormal(theGlobalPoint,&valid);
            
            if (!valid) {
                G4ExceptionDescription ed;
                ed << " NedmOpTPB/PostStepDoIt(): "
                << " The Navigator reports that it returned an invalid normal"
                << G4endl;
                G4Exception("NedmOpTPB::PostStepDoIt", "NedmOpTPB",
                            EventMustBeAborted,ed,
                            "Invalid Surface Normal - Geometry must return valid surface normal");
            }
            
            G4double ReflectionCheck;
            ReflectionCheck =p_in->GetMomentumDirection()*theGlobalNormal + p_out->GetMomentumDirection()*theGlobalNormal;
            
            if (ReflectionCheck>1.0e-12) {
                
                
                if (theGlobalNormal == G4ThreeVector(1,0,0) || theGlobalNormal == G4ThreeVector(-1,0,0)) {
                    G4cout << "***X-Plate Exit***" << G4endl;
                }
                else if (theGlobalNormal == G4ThreeVector(0,1,0) || theGlobalNormal == G4ThreeVector(0,-1,0)) {
                    G4cout << "***Y-Plate Exit***" << G4endl;
                }
                else if (theGlobalNormal == G4ThreeVector(0,0,1) || theGlobalNormal == G4ThreeVector(0,0,-1)) {
                    G4cout << "***Z-Plate Exit***" << G4endl;
                }
                
                
                /*
                G4cout << "***GLobal Normal for Plate Exit***" << theGlobalNormal << G4endl;
                G4cout << "Position: " << p_out->GetPosition() << G4endl;
                G4cout << "PreStepMomDir: " <<  p_in->GetMomentumDirection() << G4endl;
                G4cout << "PostStepMomDir: " <<  p_out->GetMomentumDirection() << G4endl;
                G4cout << "Step Process: " << p_out->GetProcessDefinedStep()->GetProcessName() << G4endl;
                G4cout << "Exit Trigger Attempt: " << p_in->GetMomentumDirection()*theGlobalNormal + p_out->GetMomentumDirection()*theGlobalNormal << G4endl;
                G4cout << "TrackStatus: " << aStep->GetTrack()->GetTrackStatus() << G4endl;
                 */
            }

            
            
            
        }

        else if (p_out->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption")
        {
            if( inVol->GetName() == "Cell" && outVol->GetName() == "Cell")
            {
                G4cout << "*****Absorption In Cell*****" << G4endl;
                
            }
            
            if( inVol->GetName() == "TPBInterface" && outVol->GetName() == "TPBInterface")
            {
                G4cout << "*****Absorption In TPBInterface*****" << G4endl;
            }


        }

    }
    
    
}

