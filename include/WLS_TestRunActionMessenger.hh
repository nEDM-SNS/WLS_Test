#ifndef WLS_TestRunActionMessenger_h
#define WLS_TestRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class WLS_TestRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class WLS_TestRunActionMessenger: public G4UImessenger
{
  public:
    WLS_TestRunActionMessenger(WLS_TestRunAction* runAction);
    virtual ~WLS_TestRunActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    WLS_TestRunAction* 	 fRunAction;
    G4UIdirectory*           fDirectory;
    G4UIcmdWithAString*      fSetFileNameCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
