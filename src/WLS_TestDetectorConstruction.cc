#include "WLS_TestDetectorConstruction.hh"
#include "WLS_TestMaterials.hh"
#include "NedmWLSFiber.hh"

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
    
    SetDefaults();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestDetectorConstruction::~WLS_TestDetectorConstruction() {
  if (fMaterials)         delete fMaterials;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::DefineMaterials(){ }

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
    
    //DefineMaterials();
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
                                                FindMaterial("G4_AIR"),
                                                //FindMaterial("Vacuum"),
                                                "expHall_log",0,0,0);
    fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),fExperimentalHall_log,"expHall",0,false,0);
    
    fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    // Placeholder - redifine if using more than 1 fiber
    G4double spacing = 1.*cm;
    
    // Demonstration of rotation - use if needed
    G4RotationMatrix* rotY = new G4RotationMatrix();
    rotY->rotateY(-90*deg);
    
    // Create Cell Wall
    if (fCellWallOn)
    {
        if (fEmbeddedFiber)
        {
            
        }
        
        else
        {
            
        }
    }
    

    //Place fibers
    for(G4int i=0;i<fNfibers;i++){
        G4double Y=-(spacing)*(fNfibers-1)*0.5 + i*spacing;
        new NedmWLSFiber(rotY,G4ThreeVector(0.,Y,0.),fExperimentalHall_log,false,0,fibRefl);
    }
    
    return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSDandField() {
    
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::SetDefaults() {
    
    //Resets to default values

    fCellWallOn = false;
    fEmbeddedFiber = false;
    fibRefl = false;
    
    fNfibers=1;
    
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::SetNFibers(G4int n) {
    fNfibers=n;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void WLS_TestDetectorConstruction::SetCellWallOn(G4bool b) {
    fCellWallOn=b;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void WLS_TestDetectorConstruction::SetEmbeddedFiberOn(G4bool b) {
    fEmbeddedFiber=b;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void WLS_TestDetectorConstruction::SetFibeReflrOn(G4bool b) {
    fibRefl=b;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* WLS_TestDetectorConstruction::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}

