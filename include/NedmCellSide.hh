#ifndef NedmCellSide_h
#define NedmCellSide_h 1

#include "G4PVPlacement.hh"

class G4LogicalVolume;
class G4VSolid;
class G4Material;

class NedmCellSide : public G4PVPlacement {

  public:
    NedmCellSide(G4RotationMatrix* pRot,
        const G4ThreeVector& tlate,
        G4LogicalVolume* pMotherLogical,
        G4int copyNo);

    static void CreateLogicalVolumes();
    
  private:
    void CopyValues();
    
    static G4LogicalVolume* fCellSide_log;

    G4VPhysicalVolume* ConstructTPBInterface();
    void ConstructSurfaceVolumes();
    G4VPhysicalVolume* ConstructRearSurface();
    
    G4ThreeVector fCell_Size;
    G4double fTPB_Thickness;
    

};

#endif
