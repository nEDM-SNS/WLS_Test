#ifndef WLS_TestStackingAction_H
#define WLS_TestStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class WLS_TestStackingAction : public G4UserStackingAction
{
  public:

    WLS_TestStackingAction();
    virtual ~WLS_TestStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
