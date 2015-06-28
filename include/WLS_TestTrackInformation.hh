#ifndef WLS_TestTrackInformation_h
#define WLS_TestTrackInformation_h

#include "G4VUserTrackInformation.hh"


class WLS_TestTrackInformation : public G4VUserTrackInformation{
    
public:
    WLS_TestTrackInformation();
    virtual ~WLS_TestTrackInformation();
    
    void SetStatus(G4int newStatus) {fStatus = newStatus;}
    G4int GetStatus() {return fStatus;}

private:
    G4int fStatus;
    
};

#endif
