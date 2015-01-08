#ifndef WLS_TestDetectorConstruction_H
#define WLS_TestDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class G4Sphere;

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

    //void SetMainScintYield(G4double );
    //void SetWLSScintYield(G4double );

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    //Materials & Elements
    //G4Material* fAl;
    G4Element* fN;
    G4Element* fO;
    G4Material* fAir;
    G4Material* fVacuum;
    G4Element* fC;
    G4Element* fH;
    //G4Material* fGlass;
    G4Material* fPstyrene;
    G4Material* fPMMA;
    G4Material* fPethylene1;
    G4Material* fPethylene2;
    
    //Geometry
    G4int fNfibers;

    G4MaterialPropertiesTable* fMPTPStyrene;

};

#endif
