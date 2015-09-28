#include "NedmCellSide.hh"
#include "NedmWLSFiber.hh"

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
#include "NedmDetectorParameters.hh"

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

    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    fCellSide_log->SetVisAttributes(cellVis);

    
    if (fTPB_On) ConstructTPBInterface();
    if (fEmbedded_fibers) ConstructEmbeddedFibers();

    SetLogicalVolume(fCellSide_log);

}



void NedmCellSide::ConstructTPBInterface() {
    
    G4Box* TPBInterface_solid = new G4Box("TPBInterface",
                                          fCell_Size.x(),
                                          fTPB_Thickness,
                                          fCell_Size.z());
    
    G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPBInterface_solid,G4Material::GetMaterial("TPB_inner"),"TPBInterface");

    G4ThreeVector TPBInterface_pos = G4ThreeVector(0.,fCell_Size.y()-fTPB_Thickness,0.);
    
    new G4PVPlacement(0,TPBInterface_pos,
                        TPBInterface_log,
                        "TPBInterface",
                        fCellSide_log,false,0);
    
    G4VisAttributes* tpbVis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpbVis->SetVisibility(true);
    TPBInterface_log->SetVisAttributes(tpbVis);

    
    // Create outer TPB layer with low index of refraction
    
    G4double TPB_outerThickness = fTPB_outerFraction*fTPB_Thickness;
    
    G4Box* TPBInterface_outer_solid = new G4Box("TPBInterface_outer",
                                          fCell_Size.x(),
                                          TPB_outerThickness,
                                          fCell_Size.z());

    
    G4LogicalVolume* TPBInterface_outer_log = new G4LogicalVolume(TPBInterface_outer_solid,G4Material::GetMaterial("TPB_outer"),"TPBInterface_outer");
    
    G4ThreeVector TPBInterface_outer_pos = G4ThreeVector(0.,fTPB_Thickness-TPB_outerThickness,0.);
    
    new G4PVPlacement(0,TPBInterface_outer_pos,
                      TPBInterface_outer_log,
                      "TPBInterface_outer",
                      TPBInterface_log,false,0);
    
    G4VisAttributes* tpbOuterVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuterVis->SetVisibility(true);
    TPBInterface_outer_log->SetVisAttributes(tpbOuterVis);



}

void NedmCellSide::ConstructEmbeddedFibers() {
    
    G4double Y_pos = -1*(fCell_Size.y()-fFiber_Thickness/2-0.001*cm);
    
    for(G4int i=0;i<fNum_fibers;i++){
        G4double X_pos=-(fFiber_spacing)*(fNum_fibers-1)*0.5 + i*fFiber_spacing;
        new NedmWLSFiber(0,G4ThreeVector(X_pos,Y_pos,0.),fCellSide_log,false,0,fFiber_Reflector);
    }
}


void NedmCellSide::CopyValues()
{
    NedmDetectorParameters* params = NedmDetectorParameters::instance();

    
    fCell_Size = params->cell_size();
    
    // Arbitrary thickness for now, shouldn't have effect, fix later
    fTPB_Thickness = 0.001*cm;
    // Thickness of TPB layer outside optical medium
    fTPB_outerFraction = 0.5;
    
    fEmbedded_fibers = params->embedded_fibers();
    fTPB_On = params->tpb_layer_on();
    fFiber_Reflector = params->fiber_reflector();
    fFiber_spacing = params->fiber_spacing();
    fNum_fibers = params->num_fibers();
    fFiber_Thickness = params->fiber_thick();
}

