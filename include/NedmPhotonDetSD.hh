#ifndef NedmPhotonDetSD_h
#define NedmPhotonDetSD_h 1

#include "NedmPhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class NedmPhotonDetSD : public G4VSensitiveDetector
{
  public:

    NedmPhotonDetSD(G4String );
    virtual ~NedmPhotonDetSD();

    virtual void Initialize(G4HCofThisEvent* );

    virtual G4bool ProcessHits(G4Step* , G4TouchableHistory* );
    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* ,
                                 G4TouchableHistory* );

  private:

    NedmPhotonDetHitsCollection* fPhotonDetHitCollection;
};

#endif
