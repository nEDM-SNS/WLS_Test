#include "SNS_TestRunActionMessenger.hh"
#include "SNS_TestRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

SNS_TestRunActionMessenger::SNS_TestRunActionMessenger(SNS_TestRunAction* runAction)
 : G4UImessenger(),
   fRunAction(runAction),
   fDirectory(0),
   fSetFileNameCmd(0)
{
  fDirectory = new G4UIdirectory("/SNS_Test/run/");
  fDirectory->SetGuidance("run control");
   
  fSetFileNameCmd 
    = new G4UIcmdWithAString("/SNS_Test/run/setFileName",this);
  fSetFileNameCmd->SetGuidance("Set the analysis file name (default: SNS_Test");
  fSetFileNameCmd->SetParameterName("fileName",false);
  fSetFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

SNS_TestRunActionMessenger::~SNS_TestRunActionMessenger()
{
  delete fSetFileNameCmd;
  delete fDirectory;   
}

void SNS_TestRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == fSetFileNameCmd ) {
    fRunAction->SetFileName(newValue);
  }

}