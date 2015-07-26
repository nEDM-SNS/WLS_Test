#ifndef NedmPhotonDetHit_h
#define NedmPhotonDetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

//--------------------------------------------------
// NedmPhotonDetHit Class
//--------------------------------------------------

class NedmPhotonDetHit : public G4VHit
{
  public:

    NedmPhotonDetHit();
    NedmPhotonDetHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4double pTime);
    virtual ~NedmPhotonDetHit();

    NedmPhotonDetHit(const NedmPhotonDetHit &right);
    const NedmPhotonDetHit& operator=(const NedmPhotonDetHit& right);

    G4int operator==(const NedmPhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
    inline G4ThreeVector GetArrivalPos() { return fPosArrive; }

    inline void SetExitPos(G4ThreeVector xyz) { fPosExit = xyz; }
    inline G4ThreeVector GetExitPos() { return fPosExit; }

    inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
    inline G4double GetArrivalTime() { return fArrivalTime; }
 
  private:

    // the arrival time of the photon
    G4double      fArrivalTime;
    // where the photon hit the detector (detector's coordinate)
    G4ThreeVector fPosArrive;
    // where the photon exited the fiber (world's coordinate)
    G4ThreeVector fPosExit;

};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<NedmPhotonDetHit> NedmPhotonDetHitsCollection;

extern G4ThreadLocal G4Allocator<NedmPhotonDetHit>* NedmPhotonDetHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* NedmPhotonDetHit::operator new(size_t)
{
  if(!NedmPhotonDetHitAllocator)
      NedmPhotonDetHitAllocator = new G4Allocator<NedmPhotonDetHit>;
  return (void *) NedmPhotonDetHitAllocator->MallocSingle();
}

inline void NedmPhotonDetHit::operator delete(void *aHit)
{
  NedmPhotonDetHitAllocator->FreeSingle((NedmPhotonDetHit*) aHit);
}

#endif
