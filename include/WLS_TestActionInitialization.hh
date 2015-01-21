#ifndef WLS_TestActionInitialization_h
#define WLS_TestActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class B4DetectorConstruction;

/// Action initialization class.
///

class WLS_TestActionInitialization : public G4VUserActionInitialization
{
  public:
    WLS_TestActionInitialization();
    virtual ~WLS_TestActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};

#endif
