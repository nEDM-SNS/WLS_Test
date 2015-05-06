#ifndef NedmCellSide_h
#define NedmCellSide_h 1

#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"


class G4LogicalVolume;
class G4VSolid;
class NedmDetectorConstruction;

class NedmCellSide : public G4PVPlacement {

  public:
    NedmCellSide(G4RotationMatrix* pRot,
        const G4ThreeVector& tlate,
        G4LogicalVolume* pMotherLogical,
        G4int copyNo);

    static void CreateLogicalVolumes();
    
    G4Material* FindMaterial(G4String);

  private:
    void CopyValues();
    
    static G4LogicalVolume* ConstructLogicalVolume();

    static G4LogicalVolume* fSide_log;

    G4VPhysicalVolume* ConstructTPBInterface();
    void ConstructSurfaceVolumes();
    G4VPhysicalVolume* ConstructRearSurface();
    
    G4ThreeVector fCell_Size;
    G4ThreeVector fTPB_Size;
    

};

#endif
