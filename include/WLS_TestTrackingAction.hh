#ifndef WLS_TestTrackingAction_h
#define WLS_TestTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class WLS_TestTrackingAction : public G4UserTrackingAction {
    
public:
    
    WLS_TestTrackingAction() { };
    virtual ~WLS_TestTrackingAction() { };
    
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    
    
};

#endif
