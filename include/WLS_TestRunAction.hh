#ifndef WLS_TestRunAction_h
#define WLS_TestRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;

class WLS_TestRunAction : public G4UserRunAction
{
  public:
    WLS_TestRunAction();
    virtual ~WLS_TestRunAction();

  public:
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
    
    void SetFileName(G4String name);


  private:
    G4Timer* timer;
    G4String fileName = "";

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void WLS_TestRunAction::SetFileName(G4String name)
{
    fileName = name;
}

#endif /*WLS_TestRunAction_h*/
