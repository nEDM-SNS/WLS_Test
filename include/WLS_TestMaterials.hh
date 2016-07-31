#ifndef WLS_TestMaterials_h
#define WLS_TestMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class WLS_TestMaterials
{
  public:

    virtual ~WLS_TestMaterials();
 
    static WLS_TestMaterials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:
 
    WLS_TestMaterials();

    void CreateMaterials();

  private:

    static WLS_TestMaterials* fInstance;

    G4NistManager*     fNistMan;

    G4Material*        fAir;
    G4Material*        fLHe;
    G4Material*        fPMMA;
    G4Material*        fWLSPMMA;
    G4Material*        fPethylene;
    G4Material*        fFPethylene;
    G4Material*        fPolystyrene;
    G4Material*        fTPB_outer;
    G4Material*        fTPB_inner;
    G4Material*        fSilicone;
    G4Material*        fClearCore;
    G4Material*        fClearClad;
    G4Material*        fTPB_evap;
};

#endif /*WLS_TestMaterials_h*/
