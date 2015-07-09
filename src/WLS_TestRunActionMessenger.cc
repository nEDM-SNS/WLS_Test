#include "WLS_TestRunActionMessenger.hh"
#include "WLS_TestRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

WLS_TestRunActionMessenger::WLS_TestRunActionMessenger(WLS_TestRunAction* runAction)
 : G4UImessenger(),
   fRunAction(runAction),
   fDirectory(0),
   fSetFileNameCmd(0)
{
  fDirectory = new G4UIdirectory("/WLS_Test/run/");
  fDirectory->SetGuidance("run control");
   
  fSetFileNameCmd 
    = new G4UIcmdWithAString("/WLS_Test/run/setFileName",this);
  fSetFileNameCmd->SetGuidance("Set the analysis file name (default: WLS_Test");
  fSetFileNameCmd->SetParameterName("fileName",false);
  fSetFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

WLS_TestRunActionMessenger::~WLS_TestRunActionMessenger()
{
  delete fSetFileNameCmd;
  delete fDirectory;   
}

void WLS_TestRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == fSetFileNameCmd ) {
    fRunAction->SetFileName(newValue);
  }

}