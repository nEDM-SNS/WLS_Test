#ifndef WLS_TestDetectorConstruction_H
#define WLS_TestDetectorConstruction_H 1

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;

class WLS_TestMaterials;
class NedmDetectorParameters;


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



  private:

    WLS_TestMaterials* fMaterials;
    NedmDetectorParameters* fParams;
    
    G4VPhysicalVolume* ConstructDetector();
    void ConstructCellPlates();
    void ConstructOuterReflector();
    
    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;
    

    
};

#endif
