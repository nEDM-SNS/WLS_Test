#include "WLS_TestActionInitialization.hh"

#include "GPSPrimaryGeneratorAction.hh"

#include "WLS_TestRunAction.hh"
//#include "WLS_TestEventAction.hh"
#include "WLS_TestSteppingAction.hh"
#include "WLS_TestStackingAction.hh"
#include "WLS_TestTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestActionInitialization::WLS_TestActionInitialization()
: G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestActionInitialization::~WLS_TestActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestActionInitialization::BuildForMaster() const
{
    //SetUserAction(new WLS_TestRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestActionInitialization::Build() const
{
    SetUserAction(new WLS_TestRunAction());
    
    SetUserAction(new GPSPrimaryGeneratorAction());
    
    //SetUserAction(new WLS_TestEventAction());
    
    SetUserAction(new WLS_TestStackingAction());
    
    SetUserAction(new WLS_TestTrackingAction());

    SetUserAction(new WLS_TestSteppingAction());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......