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
                                                G4Material::GetMaterial("G4_AIR"),
                                            //G4Material::GetMaterial("Vacuum"),
                                                "expHall_log",0,0,0);
    fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),fExperimentalHall_log,"expHall",0,false,0);
    
    fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    
    
    
    
    // number of fibers
    G4int nFibers = fParams->num_fibers();
    
    // spacing between fiber centers
    G4double spacing = fParams->fiber_spacing();
    
    // Rotation - use if needed
    G4RotationMatrix* rotY = new G4RotationMatrix();
    rotY->rotateY(-90*deg);
    
    // Create Cell Wall
    if (fParams->cell_wall_on())
    {
        new NedmCellSide(0,G4ThreeVector(0.,0.,0.),fExperimentalHall_log,0);
        
        if (fParams->embedded_fibers()==false)
        {
            G4double Y_pos = -1*(fParams->cell_size().y()+fParams->fiber_thick()/2+.001*cm);

            for(G4int i=0;i<nFibers;i++){
                G4double X_pos=-(spacing)*(nFibers-1)*0.5 + i*spacing;
                new NedmWLSFiber(0,G4ThreeVector(X_pos,Y_pos,0.),fExperimentalHall_log,false,0,fParams->fiber_reflector());
                
            }
        }
    }
    
    // No Cell Wall
    else {
        
        //Place fibers only
        for(G4int i=0;i<nFibers;i++){
            G4double X_pos=-(spacing)*(nFibers-1)*0.5 + i*spacing;
            new NedmWLSFiber(0,G4ThreeVector(X_pos,0.,0.),fExperimentalHall_log,false,0,fParams->fiber_reflector());
        }

    }
    
    return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSDandField() {}

