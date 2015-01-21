#ifndef WLS_TestSteppingAction_h
#define WLS_TestSteppingAction_h 1

#include "G4UserSteppingAction.hh"

//! User defined actions to be taken for each simulation step.
class WLS_TestSteppingAction : public G4UserSteppingAction
{
  public:
    void UserSteppingAction(const G4Step*);
    
  private:


};

#endif
