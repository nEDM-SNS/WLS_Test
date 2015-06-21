#ifndef WLS_TestDetectorConstruction_H
#define WLS_TestDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class G4Sphere;

class G4OpticalSurface;

class WLS_TestMaterials;


#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

class WLS_TestDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    WLS_TestDetectorConstruction();
    virtual ~WLS_TestDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    //Functions to modify the geometry
    void SetDefaults();

    //Get values

    void SetNFibers(G4int n);
    G4int GetNFibers(){return fNfibers;}

    void SetCellWallOn(G4bool b);
    G4bool GetCellWallOn(){return fCellWallOn;}
    
    void SetEmbeddedFiberOn(G4bool b);
    G4bool GetEmbeddedFiberOn(){return fEmbeddedFiber;}

    void SetFibeReflrOn(G4bool b);
    G4bool GetFiberReflOn(){return fEmbeddedFiber;}

  private:

    WLS_TestMaterials* fMaterials;
    
    G4VPhysicalVolume* ConstructDetector();
    
    static const G4int kEnergies;

    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;
    
    G4bool fCellWallOn;
    G4bool fEmbeddedFiber;
    G4bool fibRefl;

    
    //Geometry
    G4int fNfibers;


};

#endif
