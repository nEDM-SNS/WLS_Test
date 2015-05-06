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

//#include "NedmDetectorConstruction.hh"
//#include "NedmDetectorParameters.hh"

G4LogicalVolume* NedmCellSide::fSide_log=NULL;

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
    
    
    ConstructTPBInterface();
    ConstructSurfaceVolumes();
    
    //NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
  

    ConstructRearSurface();
}

G4LogicalVolume* NedmCellSide::ConstructLogicalVolume() {
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    
    G4VSolid* cellSide_solid;
    G4ThreeVector cell_size = parameters->cell_size();
    
    G4Box* cellSide_box = new G4Box("Cell",
                                    cell_size.x(),
                                    cell_size.y(),
                                    cell_size.z());
    
    if(parameters->use_hole() && side1) {
        G4VSolid* hole_ptr;
        cellSide_solid = MakeHole(cellSide_box, "CellWithHole", "CellHole", &hole_ptr);
        
        
        if(parameters->use_plug()) {
            
            the_plug_ = new G4LogicalVolume(hole_ptr,
                                            the_detector_->torlon(),
                                            "Plug");
            
            G4LogicalSkinSurface* torlonSurface =
            new G4LogicalSkinSurface("TorlonSurface", the_plug_,
                                     the_detector_->torlon_surface());
            
            NedmDetectorConstruction::RegisterSurface(torlonSurface, "plug");
        }
    } else {
        cellSide_solid = cellSide_box;
    }
    
    G4LogicalVolume* cellSide_log = new G4LogicalVolume(cellSide_solid,
                                                        the_detector_->acrylic(), "Cell");
    
    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    cellVis->SetVisibility(true);
    cellSide_log->SetVisAttributes(cellVis);
    
    return cellSide_log;
}

void NedmCellSide::CreateLogicalVolumes() {
    NedmCellSide::fSide_log = ConstructLogicalVolume();
}

G4VPhysicalVolume* NedmCellSide::ConstructTPBInterface() {
    
    G4Box* TPBInterface_solid = new G4Box("TPBInterface",
                                        fTPB_Size.x(),
                                        fTPB_Size.y(),
                                        fTPB_Size.z());
    
    G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPBInterface_solid,
                                                            FindMaterial("G4_AIR"),
                                                            "TPBInterface");
    
    G4int copy_no = GetCopyNo();
    G4RotationMatrix* rot = GetRotation();
    G4ThreeVector cell_position = GetTranslation();
    
    G4VPhysicalVolume* TPBInterface_phys
    = new G4PVPlacement(rot,
                        cell_position - cell_position.unit() *
                        (fTPB_Size.y() + fCell_Size.y()),
                        TPBInterface_log,
                        "TPBInterface",
                        the_detector_->get_logical_world(), false, copy_no);
    
    
    G4LogicalBorderSurface* TPBMachinedSurf =
    new G4LogicalBorderSurface("MachinedSurfaceTPB",
                               TPBInterface_phys,
                               this,
                               the_detector_->conversion_surface());
    
    G4LogicalBorderSurface* TPBMachinedSurf2 =
    new G4LogicalBorderSurface("MachinedSurfaceTPB2",
                               this,
                               TPBInterface_phys,
                               the_detector_->tpb_surface());
    
    NedmDetectorConstruction::RegisterSurface(TPBMachinedSurf, "tpb");
    NedmDetectorConstruction::RegisterSurface(TPBMachinedSurf2, "tpb");
    
    
    return TPBInterface_phys;
}

void NedmCellSide::ConstructSurfaceVolumes() {
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    
    G4int copy_no = GetCopyNo();
    G4RotationMatrix* rot = GetRotation();
    G4ThreeVector cell_position = GetTranslation();
    
    G4ThreeVector x_surface_size = parameters->cell_x_surface_size();
    G4ThreeVector y_surface_size = parameters->cell_y_surface_size();
    
    
    //Should make these static variables to avoid recreating this all the time.
    G4Box* surfVolX_box = new G4Box("cellSurfaceX",
                                    x_surface_size.x(),
                                    x_surface_size.y(),
                                    x_surface_size.z());
    
    G4LogicalVolume* surfVolX_log =
    new G4LogicalVolume(surfVolX_box, the_detector_->air(), "cellSurfaceX");
    
    G4ThreeVector displacement = parameters->cell_x_surface_position();
    if (rot)
        displacement *= (*rot);
    
    G4ThreeVector pos1 = cell_position + displacement;
    
    
    G4VPhysicalVolume* surfVolPx_phys =
    new G4PVPlacement(rot,
                      pos1,
                      surfVolX_log,
                      "cellSurfacePx",
                      the_detector_->get_logical_world(),
                      false,
                      2*copy_no);
    
    G4ThreeVector pos2 = cell_position - displacement;
    
    G4VPhysicalVolume* surfVolNx_phys =
    new G4PVPlacement(rot,
                      pos2,
                      surfVolX_log,
                      "cellSurfaceNx",
                      the_detector_->get_logical_world(),
                      false,
                      2*copy_no + 1);
    
    G4VSolid* surfVolY_solid;
    
    G4Box* surfVolY_box = new G4Box("cellSurfaceY",
                                    y_surface_size.x(),
                                    y_surface_size.y(),
                                    y_surface_size.z());
    
    if(parameters->use_hole() && side1) {
        G4Tubs* surfVol4_hole = new G4Tubs("cellSurfaceY_hole",
                                           0., parameters->hole_radius(),
                                           y_surface_size.y(), 0., twopi);
        
        G4RotationMatrix* Rot = new G4RotationMatrix;
        Rot->rotateX(90.*deg);
        G4ThreeVector displacement = parameters->hole_position();
        
        G4SubtractionSolid* surfVolY_w_hole =
        new G4SubtractionSolid("cellSurfaceYWithHole", surfVolY_box,
                               surfVol4_hole, Rot, displacement);
        
        surfVolY_solid = surfVolY_w_hole;
    } else {
        surfVolY_solid = surfVolY_box;
    }
    
    G4LogicalVolume* surfVolY_log =
    new G4LogicalVolume(surfVolY_solid, the_detector_->air(), "cellSurfaceY");
    
    G4VPhysicalVolume* surfVolY_phys =
    new G4PVPlacement(rot,
                      cell_position + cell_position.unit() *
                      parameters->cell_y_surface_position().y(),
                      surfVolY_log,
                      "cellSurfaceY",
                      the_detector_->get_logical_world(),
                      false,
                      copy_no);
    
    
    //G4LogicalBorderSurface* cellMachinedSurface1 =
    new G4LogicalBorderSurface("cellMachinedSurfacePx", this,
                               surfVolPx_phys, the_detector_->machined_surface());
    
    //G4LogicalBorderSurface* cellMachinedSurface2 =
    new G4LogicalBorderSurface("cellMachinedSurfaceNx", this,
                               surfVolNx_phys, the_detector_->machined_surface());
    
    //G4LogicalBorderSurface* cellCastSurface4 =
    new G4LogicalBorderSurface("cellCastSurfaceY", this,
                               surfVolY_phys, the_detector_->cast_surface());
    
}

/*! @warning Do not call this method if you are calling NedmCellSide::ConstructMirror
 */
G4VPhysicalVolume* NedmCellSide::ConstructRearSurface() {
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    G4RotationMatrix* rot = GetRotation();
    G4ThreeVector translation = GetTranslation();
    G4int copy_no = GetCopyNo();

    G4Box* rearSurf_box = new G4Box("rearSurf",
                                    mirror_size.x(),
                                    mirror_size.y(),
                                    mirror_size.z());
    
    G4LogicalVolume* rearSurf_log = new G4LogicalVolume(rearSurf_box,
                                                        the_detector_->acrylic(),
                                                        "rear_surface");
    
    G4ThreeVector position = translation + G4ThreeVector(0.,0.,
                                                         parameters->mirror_z_position());
    
    G4VPhysicalVolume* rearSurf_phys
    = new G4PVPlacement(rot, position, rearSurf_log, "rearSurf",
                        the_detector_->get_logical_world(), false, copy_no);
    
    new G4LogicalBorderSurface("RearMachinedSurface", this,
                               rearSurf_phys, the_detector_->machined_surface());
    
    return rearSurf_phys;
}

NedmCellSide::CopyValues()
{
    G4double cell_x = 5.08*cm;
    G4double cell_y = 0.635*cm;
    G4double cell_z = 20.0*cm;
    
    G4ThreeVector fCell_Size = G4ThreeVector(cell_x, cell_y, cell_z);
    
    G4double surface_thickness = 0.01*cm;
    
    //The 4 here comes from the fact that there are two full surfaces
    //of thickness 2*surface_thickness between the plates. These are the
    //surface volumes
    G4double cell_distx = cell_x + cell_y + 4*surface_thickness;
    G4double cell_disty = cell_x + cell_y + 4*surface_thickness;
    
    G4ThreeVector cell_x_surface_size = G4ThreeVector(surface_thickness, cell_y, cell_z);
    G4ThreeVector cell_y_surface_size = G4ThreeVector(cell_x, surface_thickness, cell_z);
    
    G4double cell_x_surface_position_.setX(cell_x + surface_thickness);
    G4double cell_y_surface_position_.setY(cell_y + surface_thickness);
    
    fTPB_Size = cell_y_surface_size;

}

