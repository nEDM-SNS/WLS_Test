#include "WLS_TestDetectorConstruction.hh"
#include "NedmDetectorParameters.hh"
#include "WLS_TestMaterials.hh"
#include "NedmWLSFiber.hh"
#include "NedmCellSide.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
WLS_TestDetectorConstruction::WLS_TestDetectorConstruction()
:
fMaterials(NULL)
{
    fExperimentalHall_box = NULL;
    fExperimentalHall_log = NULL;
    fExperimentalHall_phys = NULL;
    
    fParams = NedmDetectorParameters::instance();
    
    overlapCheck = true;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestDetectorConstruction::~WLS_TestDetectorConstruction() {
    if (fMaterials)         delete fMaterials;
    if (fParams)            delete fParams;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* WLS_TestDetectorConstruction::Construct(){
    
    if (fExperimentalHall_phys) {
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();
        G4LogicalSkinSurface::CleanSurfaceTable();
        G4LogicalBorderSurface::CleanSurfaceTable();
    }
    

    fMaterials = WLS_TestMaterials::GetInstance();


    return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* WLS_TestDetectorConstruction::ConstructDetector()
{
    //The experimental hall walls are all 1m away from housing walls
    G4double expHall_x = 10.*m;
    G4double expHall_y = 10.*m;
    G4double expHall_z = 10.*m;
    
    //Create experimental hall
    fExperimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
    fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box,
                                                G4Material::GetMaterial("LHe"),
                                            //G4Material::GetMaterial("Vacuum"),
                                                "expHall_log",0,0,0);
    
    fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),fExperimentalHall_log,"expHall",0,false,0);
    
    fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);
     
    ConstructSquarePMMA();
    ConstructCirclePMMAandFibers();
    if (fParams->outer_reflector()) ConstructCylindricalReflector();
    if (fParams->outer_reflector()) ConstructEndFiberReflector();
   
    ConstructSiliconWafersAndPhotDet();
    ConstructClearFibers();
    
    
    return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSquarePMMA()
{
    
    G4double displacement = -2.111375*cm;

  // Create Cell Wall //
    if (fParams->cell_wall_on())
    {
        new NedmCellSide(0,G4ThreeVector(0.,displacement,0.),fExperimentalHall_log,0);
    }
    
    // No Cell Wall
  //  else {
        
        //Place fibers only
    //    for(G4int i=0;i<nFibers;i++){
      //      G4double X_pos=-(spacing)*(nFibers-1)*0.5 + i*spacing;
        //    new NedmWLSFiber(0,G4ThreeVector(X_pos,0.,0.),fExperimentalHall_log,false,0,fParams->fiber_reflector());
      //  }
        
  //  }
 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructCirclePMMAandFibers()
{
// Circle PMMA //
    
    G4double radMin = fParams->cellCircleInner_rad();
    G4double radMax = fParams->cellCircleOuter_rad();
    G4double half_z = fParams->cell_half_z()+ 3.175*cm;

    G4Tubs* cellSide_solid = new G4Tubs("CircleCell",
                                        radMin,
					radMax,
					half_z,
					0*deg,
					360*deg);

    G4LogicalVolume* cellSide_log =
    new G4LogicalVolume(cellSide_solid, G4Material::GetMaterial("PMMA"), "CircleCell");
    
    G4ThreeVector circle_pos = G4ThreeVector(0, 0, -3.175*cm);

    new G4PVPlacement(0,               
                      circle_pos,   
		      cellSide_log,    
	              "CircularCell",        
                      fExperimentalHall_log, 
                      false,
		      0,
		      overlapCheck);

    G4VisAttributes* cellVis = new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    cellSide_log->SetVisAttributes(cellVis);



// Fibers //

    G4int nFibers = fParams->num_fibers();
    G4double R_pos = fParams->cellCircleOuter_rad() + (fParams->fiber_thick())/2;
    G4double Z_pos = -71.755*cm;

    for(G4int i=0;i<nFibers;i++)
        {
	G4double Phi_pos = (i*twopi)/nFibers;
	G4double X_pos = R_pos*cos(Phi_pos);
	G4double Y_pos = R_pos*sin(Phi_pos);
        new NedmWLSFiber(0, G4ThreeVector(X_pos,Y_pos,Z_pos), fExperimentalHall_log, false, 0, fParams->fiber_reflector());	
	}

    
}

void WLS_TestDetectorConstruction::ConstructCylindricalReflector()
{

    G4double refl_rad = 3.5925*cm + 0.35*mm;
    G4double refl_length = fParams->cell_half_z() + 6.985*cm;

    // Reflector Geometry
    G4Tubs* CylindricalReflector = new G4Tubs("CylindricalReflector", refl_rad,refl_rad + 0.1*mm, refl_length, 0.*deg, 360.*deg );
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(CylindricalReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 3;
    G4double the_photon_energies_[kEnergies] = {2.*eV, 4.*eV, 16*eV};
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0.96, 0.96, 0.96};
    
    //Table of Surface Properties for Mirror
    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);

    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,0.2,1.0));
    ReflectVis->SetVisibility(true);
    //ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    G4ThreeVector refl_pos = G4ThreeVector(0., 0.,-6.985*cm);

    new G4PVPlacement(0,                            //no rotation
                      refl_pos,              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      fExperimentalHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0,                        //copy number
                      overlapCheck);                    // Check Overlaps

}


void WLS_TestDetectorConstruction::ConstructEndFiberReflector()
{
       
    // Reflector Wrapping
    G4double R_min = fParams->cellCircleOuter_rad();
    G4double R_max = 3.5925*cm + 0.35*mm;

    G4Tubs* endReflector_solid = new G4Tubs("EndReflector",
                                            R_min,
					    R_max,
					    0.1*mm,
					    0.,
					    360*deg);
    
    G4LogicalVolume* endReflector_log = new G4LogicalVolume(endReflector_solid, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 3;
    G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0.96, 0.96, 0.96};
    
    //Table of Surface Properties for Mirror
    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
    
    new G4LogicalSkinSurface("Reflector_surface", endReflector_log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,0.2,1.0));
    ReflectVis->SetVisibility(true);
    //ReflectVis->SetForceWireframe(true);
    endReflector_log->SetVisAttributes(ReflectVis);
    
    G4ThreeVector reflectorPos = G4ThreeVector(0.,0.,fParams->cell_half_z() + 0.11*mm);
    
    new G4PVPlacement(0,                            //no rotation
                      reflectorPos,              //at (0,0,0)
                      endReflector_log,                     //its logical volume
                     "Reflector",            //its name
                      fExperimentalHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0,
		      overlapCheck);                            //copy number
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSiliconWafersAndPhotDet(){

    G4double silicon_xy = 4.*cm;
    G4double silicon_thick = 0.5*mm;

    G4Box* silicon_solid = new G4Box("SiliconCookie",
                                     silicon_xy,
				     silicon_xy,
				     silicon_thick);

    G4LogicalVolume* silicon_log1 = new G4LogicalVolume(silicon_solid, G4Material::GetMaterial("Silicone"), "SiliconCookie");

    G4LogicalVolume* silicon_log2 = new G4LogicalVolume(silicon_solid, G4Material::GetMaterial("Silicone"), "SiliconCookie");
    
    
    // Create Photon Detector inside silicon wafer 2
    G4Tubs* photDet_Solid = new G4Tubs("photDet",
                                       3.2*cm,
                                       3.8*cm,
                                       0.5*mm,
                                       0.,
                                       360*deg);
    
    G4LogicalVolume* photonDet_log = new G4LogicalVolume(photDet_Solid,G4Material::GetMaterial("PMMA"),"photDet");
    
    new G4PVPlacement(0,G4ThreeVector(0,0,0),photonDet_log,"photDet1",silicon_log2,false,0,overlapCheck);
    
    G4VisAttributes* DetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
    DetVis->SetVisibility(true);
    DetVis->SetForceWireframe(true);
    photonDet_log->SetVisAttributes(DetVis);

    // Place Silicon Wafers
    G4ThreeVector wafer_pos1 = G4ThreeVector(0.,0.,-1*(silicon_thick + 152.4*cm - fParams->cell_half_z()));
    G4ThreeVector wafer_pos2 = wafer_pos1 - G4ThreeVector(0.,0., 2*silicon_thick + 30.48*cm);

    new G4PVPlacement(0,                            //no rotation
                      wafer_pos1,              //at (0,0,0)
                      silicon_log1,                     //its logical volume
                      "SiliconeCookie1",            //its name
                      fExperimentalHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0,
		      overlapCheck);                            //copy number

    new G4PVPlacement(0,                            //no rotation
                      wafer_pos2,              //at (0,0,0)
		      silicon_log2,                     //its logical volume
		      "SiliconeCookie2",            //its name
                      fExperimentalHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0,
                      overlapCheck);                            //copy number



    G4VisAttributes* siliconVis = new G4VisAttributes(G4Color(0.18,0.22,0.02));
    siliconVis->SetVisibility(true);
    silicon_log1->SetVisAttributes(siliconVis);
    silicon_log2->SetVisAttributes(siliconVis);


}

void WLS_TestDetectorConstruction::ConstructClearFibers(){

    G4double half_z = 15.24*cm; // 6 inches
    G4Tubs* clearFiberClad_solid = new G4Tubs("clearFiberClad",
                                              0,
					      0.75*mm,
					      half_z,
					      0,
					      360.*deg);
    
    G4Tubs* clearFiberCore_solid = new G4Tubs("ClearFiberCore",
                                              0,
					      0.72*mm,
					      half_z,
					      0,
					      360.*deg);

    G4LogicalVolume* clearFiberCore_log = new G4LogicalVolume(clearFiberCore_solid, G4Material::GetMaterial("ClearCore"), "ClearFiberCore");

    G4LogicalVolume* clearFiberClad_log = new G4LogicalVolume(clearFiberClad_solid, G4Material::GetMaterial("ClearClad"), "ClearFiberCladding");

    new G4PVPlacement(0,
                      G4ThreeVector(0.,0.,0.),
                      clearFiberCore_log,
                      "ClearFiberCore",
                      clearFiberClad_log,
                      false,
                      0,
                      overlapCheck);
    
    G4int nFibers = fParams->num_fibers();
    G4double R_pos = fParams->cellCircleOuter_rad() + (fParams->fiber_thick())/2;

    for(G4int i=0;i<nFibers;i++)
    {
        G4double Phi_pos = (i*twopi)/nFibers;
        G4double X_pos = R_pos*cos(Phi_pos);
        G4double Y_pos = R_pos*sin(Phi_pos);
        G4ThreeVector fiber_pos = G4ThreeVector(X_pos, Y_pos, -1*(half_z + 152.5*cm - fParams->cell_half_z()));

        new G4PVPlacement(0,                            
                          fiber_pos,              
                          clearFiberClad_log,                     
                          "ClearFiberCladding",            
                          fExperimentalHall_log,                
                          false,                        
                          0,
                          overlapCheck);
    };


    G4VisAttributes* clearCladVis = new G4VisAttributes(G4Color(1.0,1.0,1.0));
    clearCladVis->SetVisibility(true);
    clearFiberClad_log->SetVisAttributes(clearCladVis);
    clearFiberCore_log->SetVisAttributes(clearCladVis);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSDandField() {}

