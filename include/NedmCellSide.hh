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

    void ConstructTPBInterface();
    void ConstructEmbeddedFibers();
    
    G4ThreeVector fCell_Size;
    G4double fTPB_Thickness;
    G4double fTPB_outerFraction;
 
    G4bool fTPB_On;
    G4bool fEmbedded_fibers;
    G4bool fFiber_Reflector;
    G4bool fFiber_spacing;
    G4int fNum_fibers;
    G4double fFiber_Thickness;
    

};

#endif
