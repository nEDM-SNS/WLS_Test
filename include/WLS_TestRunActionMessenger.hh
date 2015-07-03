#ifndef SNS_TestRunActionMessenger_h
#define SNS_TestRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class SNS_TestRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class SNS_TestRunActionMessenger: public G4UImessenger
{
  public:
    SNS_TestRunActionMessenger(SNS_TestRunAction* runAction);
    virtual ~SNS_TestRunActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    SNS_TestRunAction* 	 fRunAction;
    G4UIdirectory*           fDirectory;
    G4UIcmdWithAString*      fSetFileNameCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
