#include "NedmCellSide.hh"

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4Material.hh"


//#include "NedmDetectorConstruction.hh"
//#include "NedmDetectorParameters.hh"

G4LogicalVolume* NedmCellSide::fCellSide_log=NULL;

NedmCellSide::NedmCellSide(G4RotationMatrix* pRot,
                           const G4ThreeVector& tlate,
                           G4LogicalVolume* pMotherLogical,
                           G4int copyNo)
:G4PVPlacement(pRot, tlate,
               new G4LogicalVolume(new G4Box("temp",1,1,1),
                                   G4Material::GetMaterial("Vacuum"),
                                   "temp",0,0,0),
               "CellSide", pMotherLogical, false, copyNo) {
    
    CopyValues();
    
    
    G4Box* cellSide_solid = new G4Box("Cell",
                                    fCell_Size.x(),
                                    fCell_Size.y(),
                                    fCell_Size.z());
    
    fCellSide_log =
    new G4LogicalVolume(cellSide_solid,G4Material::GetMaterial("PMMA"),
                        "Cell");

    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    cellVis->SetVisibility(true);
    fCellSide_log->SetVisAttributes(cellVis);

    
    ConstructTPBInterface();

    SetLogicalVolume(fCellSide_log);

}



G4VPhysicalVolume* NedmCellSide::ConstructTPBInterface() {
    
    G4Box* TPBInterface_solid = new G4Box("TPBInterface",
                                          fCell_Size.x(),
                                          fTPB_Thickness,
                                          fCell_Size.z());
    
    G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPBInterface_solid,G4Material::GetMaterial("TPBDopedPS"),"TPBInterface");

    G4ThreeVector TPBInterface_pos = G4ThreeVector(0.,fCell_Size.y()-fTPB_Thickness,0.);
    
    G4VPhysicalVolume* TPBInterface_phys
    = new G4PVPlacement(0,TPBInterface_pos,
                        TPBInterface_log,
                        "TPBInterface",
                        fCellSide_log,false,0);
    
    
    
    return TPBInterface_phys;
}


void NedmCellSide::CopyValues()
{
    G4double cell_x = 5.08*cm;
    G4double cell_y = 0.635*cm;
    G4double cell_z = 20.64*cm;
    
    fCell_Size = G4ThreeVector(cell_x, cell_y, cell_z);
    
    // Arbitrary thickness for now, shouldn't have effect, fix later
    fTPB_Thickness = 0.01*cm;
    
}

