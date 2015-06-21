#include "NedmWLSFiber.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

G4LogicalVolume* NedmWLSFiber::fClad2_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NedmWLSFiber::NedmWLSFiber(G4RotationMatrix *pRot,
                           const G4ThreeVector &tlate,
                           G4LogicalVolume *pMotherLogical,
                           G4bool pMany,
                           G4int pCopyNo,
                           G4bool Reflector)
:G4PVPlacement(pRot,tlate,
               new G4LogicalVolume(new G4Box("temp",1,1,1),
<<<<<<< HEAD
                                   G4Material::GetMaterial("G4_AIR"),
=======
                                   FindMaterial("G4_AIR"),
>>>>>>> master
                                   "temp",0,0,0),
               "Cladding2",pMotherLogical,pMany,pCopyNo)
{
    CopyValues();
    
    // Boundary Surface Properties
    G4OpticalSurface* opSurface = NULL;
    
    if (fSurfaceRoughness < 1.)
        opSurface = new G4OpticalSurface("RoughSurface",          // Surface Name
                                         glisur,                  // SetModel
                                         ground,                  // SetFinish
                                         dielectric_dielectric,   // SetType
                                         fSurfaceRoughness);      // SetPolish
    
    
    G4RotationMatrix* zRot = new G4RotationMatrix;
    zRot->rotateZ(90*deg);
    
    // The Fiber
    //
    G4Tubs* fiber_tube =
    new G4Tubs("Fiber",fFiber_rmin,fFiber_rmax,fFiber_z,fFiber_sphi,fFiber_ephi);
    
    G4LogicalVolume* fiber_log =
<<<<<<< HEAD
    new G4LogicalVolume(fiber_tube,G4Material::GetMaterial("WLSPMMA"),
=======
    new G4LogicalVolume(fiber_tube,
                        //FindMaterial("PMMA_OLD"),
                        FindMaterial("PMMA"),
>>>>>>> master
                        "Fiber",0,0,0);
   
    // Cladding (first layer)
    //
    G4Tubs* clad1_tube =
    new G4Tubs("Cladding1",fClad1_rmin,fClad1_rmax,fClad1_z,fClad1_sphi,
               fClad1_ephi);
    
    G4LogicalVolume* clad1_log =
<<<<<<< HEAD
    new G4LogicalVolume(clad1_tube,G4Material::GetMaterial("Pethylene"),
                        "Cladding1",0,0,0);
=======
    new G4LogicalVolume(clad1_tube,FindMaterial("Pethylene"),"Cladding1",0,0,0);
>>>>>>> master
    
    // Cladding (second layer)
    //
    G4Tubs* clad2_tube =
    new G4Tubs("Cladding2",fClad2_rmin,fClad2_rmax,fClad2_z,fClad2_sphi,
               fClad2_ephi);
    
    fClad2_log =
<<<<<<< HEAD
    new G4LogicalVolume(clad2_tube,G4Material::GetMaterial("FPethylene"),
=======
    new G4LogicalVolume(clad2_tube,FindMaterial("FPethylene"),
>>>>>>> master
                        "Cladding2",0,0,0);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiber_log,
                      "Fiber", clad1_log,false,0);
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                      "Cladding1",fClad2_log,false,0);
    
    // Place the rough surface only if needed
    if (opSurface) {
        new G4LogicalSkinSurface("Clad2_Surface", fClad2_log, opSurface);
    }
    
    G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
    FiberVis->SetVisibility(true);
    fClad2_log->SetVisAttributes(FiberVis);
    
    
    SetLogicalVolume(fClad2_log);
    
    if (Reflector) {
        // Fiber Reflector
        G4Tubs* solidMirror = new G4Tubs("Mirror",
                                         fMirrorRmin,
                                         fMirrorRmax,
                                         fMirrorThick,
                                         fMirrorSPhi,
                                         fMirrorEPhi);
        
<<<<<<< HEAD
        G4LogicalVolume* logicMirror = new G4LogicalVolume(solidMirror,G4Material::GetMaterial("PMMA"),"Mirror");
=======
        G4LogicalVolume* logicMirror = new G4LogicalVolume(solidMirror,FindMaterial("PMMA"),"Mirror");
>>>>>>> master
        
        // Photon Energies for which mirror properties will be given
        const G4int kEnergies = 3;
        G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
        
        // Optical Surface for mirror
        G4OpticalSurface* mirror_surface_ =
        new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                             dielectric_dielectric);
        
        // Reflectivity of mirror for each photon energy
        G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0.998};
        
        //Table of Surface Properties for Mirror
        G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
        mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
        mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
        
        // Place Mirror
        new G4PVPlacement(0,                                 //no rotation
                          G4ThreeVector(0.,0.,fMirrorPosZ),   //position
                          logicMirror,                  //its logical volume
                          "Mirror",                     //its name
                          //Clad2_log,                  //its mother  volume
                          fiber_log,                   //its mother  volume
                          false,                        //no boolean operation
                          0);                           //copy number
        
        // Create Skin Surface to link logical surface and optical surface
        new G4LogicalSkinSurface("MirrorSurface",logicMirror,mirror_surface_);
        
        // Set Visualization Properties of the Mirror
        G4VisAttributes* MirrorVis=new G4VisAttributes(G4Color(0.0,0.0,1.0));
        MirrorVis->SetVisibility(true);
        logicMirror->SetVisAttributes(MirrorVis);
        
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NedmWLSFiber::CopyValues(){
    
    fFiber_rmin = 0.00*cm;
    fFiber_rmax = 0.088/2*cm;
    //  fFiber_z    = 112./2*cm;
    fFiber_z    = 20./2*cm;
    fFiber_sphi = 0.00*deg;
    fFiber_ephi = 360.*deg;
    
    fSurfaceRoughness = 0.9;
    
    fClad1_rmin = 0.;// fFiber_rmax;
    fClad1_rmax = fFiber_rmax + 0.003*cm;
    
    fClad1_z    = fFiber_z;
    fClad1_sphi = fFiber_sphi;
    fClad1_ephi = fFiber_ephi;
    
    fClad2_rmin = 0.;//fClad1_rmax;
    fClad2_rmax = fClad1_rmax + 0.003*cm;
    
    fClad2_z    = fFiber_z;
    fClad2_sphi = fFiber_sphi;
    fClad2_ephi = fFiber_ephi;
    
    //fMirrorRmax  = fClad2_rmax;
    fMirrorRmax  = fFiber_rmax;
    fMirrorRmin  = 0.*cm;
    fMirrorThick     = 1./2*mm;
    fMirrorSPhi  = fFiber_sphi;
    fMirrorEPhi  = fFiber_ephi;
    
    fMirrorPosZ  = -1*(fFiber_z - fMirrorThick);
    fMirrorReflectivity = 1;
    
    
}

<<<<<<< HEAD
=======
G4Material* NedmWLSFiber::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}


>>>>>>> master
