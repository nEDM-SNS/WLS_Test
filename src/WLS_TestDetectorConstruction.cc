//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: WLS_TestDetectorConstruction.cc 82853 2014-07-14 09:07:11Z gcosmo $
//
/// \file optical/WLS_Test/src/WLS_TestDetectorConstruction.cc
/// \brief Implementation of the WLS_TestDetectorConstruction class
//
//
#include "WLS_TestDetectorConstruction.hh"
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

//G4bool WLS_TestDetectorConstruction::fSphereOn = true;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestDetectorConstruction::WLS_TestDetectorConstruction()
: fMPTPStyrene(NULL)
{
    fExperimentalHall_box = NULL;
    fExperimentalHall_log = NULL;
    fExperimentalHall_phys = NULL;
    
    fAir = fVacuum = NULL;
    fPstyrene = fPMMA = fPethylene1 = fPethylene2 = NULL;
    
    //fN = fO = fC = fH = NULL;
    
    SetDefaults();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestDetectorConstruction::~WLS_TestDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::DefineMaterials(){
    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;
    G4int nelements;
    
    
    G4int polyPMMA = 1;
    G4int nC_PMMA = 3+2*polyPMMA;
    G4int nH_PMMA = 6+2*polyPMMA;
    
    G4int polyeth = 1;
    G4int nC_eth = 2*polyeth;
    G4int nH_eth = 4*polyeth;
    
    //***Elements
    fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
    fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
    fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
    fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);
    
    //***Materials
    //Vacuum
    fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                             density=universe_mean_density,kStateGas,0.1*kelvin,
                             1.e-19*pascal);
    //Air
    fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
    fAir->AddElement(fN, 70*perCent);
    fAir->AddElement(fO, 30*perCent);
    //Polystyrene
    fPstyrene = new G4Material("Polystyrene", density= 1.03*g/cm3, 2);
    fPstyrene->AddElement(fC, 8);
    fPstyrene->AddElement(fH, 8);
    //Fiber(PMMA)
    fPMMA = new G4Material("PMMA", density=1190*kg/m3,3);
    fPMMA->AddElement(fH,nH_PMMA);
    fPMMA->AddElement(fC,nC_PMMA);
    fPMMA->AddElement(fO,2);
    //Cladding(polyethylene)
    fPethylene1 = new G4Material("Pethylene1", density=1200*kg/m3,2);
    fPethylene1->AddElement(fH,nH_eth);
    fPethylene1->AddElement(fC,nC_eth);
    //Double cladding(flourinated polyethylene)
    fPethylene2 = new G4Material("Pethylene2", density=1400*kg/m3,2);
    fPethylene2->AddElement(fH,nH_eth);
    fPethylene2->AddElement(fC,nC_eth);
    
    //***Material properties tables
    
    G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};
    const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
    G4double vacuum_RIND[]={1.,1.,1.};
    assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
    G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
    vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
    fVacuum->SetMaterialPropertiesTable(vacuum_mt);
    fAir->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex
    
    G4double wls_Energy[] = {2.00*eV,2.87*eV,2.90*eV,3.47*eV};
    const G4int wlsnum = sizeof(wls_Energy)/sizeof(G4double);
    
    G4double rIndexPstyrene[]={ 1.5, 1.5, 1.5, 1.5};
    assert(sizeof(rIndexPstyrene) == sizeof(wls_Energy));
    G4double absorption1[]={2.*cm, 2.*cm, 2.*cm, 2.*cm};
    assert(sizeof(absorption1) == sizeof(wls_Energy));
    G4double scintilFast[]={0.00, 0.00, 1.00, 1.00};
    assert(sizeof(scintilFast) == sizeof(wls_Energy));
    fMPTPStyrene = new G4MaterialPropertiesTable();
    fMPTPStyrene->AddProperty("RINDEX",wls_Energy,rIndexPstyrene,wlsnum);
    fMPTPStyrene->AddProperty("ABSLENGTH",wls_Energy,absorption1,wlsnum);
    fMPTPStyrene->AddProperty("FASTCOMPONENT",wls_Energy, scintilFast,wlsnum);
    fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
    fMPTPStyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
    fMPTPStyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
    fPstyrene->SetMaterialPropertiesTable(fMPTPStyrene);
    
    // Set the Birks Constant for the Polystyrene scintillator
    
    fPstyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    
    G4double RefractiveIndexFiber[]={ 1.60, 1.60, 1.60, 1.60};
    assert(sizeof(RefractiveIndexFiber) == sizeof(wls_Energy));
    G4double AbsFiber[]={9.00*m,9.00*m,0.1*mm,0.1*mm};
    assert(sizeof(AbsFiber) == sizeof(wls_Energy));
    G4double EmissionFib[]={1.0, 1.0, 0.0, 0.0};
    assert(sizeof(EmissionFib) == sizeof(wls_Energy));
    G4MaterialPropertiesTable* fiberProperty = new G4MaterialPropertiesTable();
    fiberProperty->AddProperty("RINDEX",wls_Energy,RefractiveIndexFiber,wlsnum);
    fiberProperty->AddProperty("WLSABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fiberProperty->AddProperty("WLSCOMPONENT",wls_Energy,EmissionFib,wlsnum);
    fiberProperty->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
    fPMMA->SetMaterialPropertiesTable(fiberProperty);
    
    G4double RefractiveIndexClad1[]={ 1.49, 1.49, 1.49, 1.49};
    assert(sizeof(RefractiveIndexClad1) == sizeof(wls_Energy));
    G4MaterialPropertiesTable* clad1Property = new G4MaterialPropertiesTable();
    clad1Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad1,wlsnum);
    clad1Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fPethylene1->SetMaterialPropertiesTable(clad1Property);
    
    G4double RefractiveIndexClad2[]={ 1.42, 1.42, 1.42, 1.42};
    assert(sizeof(RefractiveIndexClad2) == sizeof(wls_Energy));
    G4MaterialPropertiesTable* clad2Property = new G4MaterialPropertiesTable();
    clad2Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad2,wlsnum);
    clad2Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fPethylene2->SetMaterialPropertiesTable(clad2Property);
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
    
    DefineMaterials();
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
                                                fVacuum,"expHall_log",0,0,0);
    fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                               fExperimentalHall_log,"expHall",0,false,0);
    
    fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    // Placeholder - redifine if using more than 1 fiber
    G4double spacing = 1.*cm;
    
    // Demonstration of rotation - use if needed
    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateY(90*deg);
    
    //Place fibers
    for(G4int i=0;i<fNfibers;i++){
        G4double Y=-(spacing)*(fNfibers-1)*0.5 + i*spacing;
        new NedmWLSFiber(rm,G4ThreeVector(0.,Y,0.),fExperimentalHall_log,false,0);
    }

/*
    //Place the WLS slab
    if(fWLSslab){
        G4VPhysicalVolume* slab = new WLS_TestWLSSlab(0,G4ThreeVector(0.,0.,
                                                                      -fScint_z/2.-fSlab_z-1.*cm),
                                                      fExperimentalHall_log,false,0,
                                                      this);
        
        //Surface properties for the WLS slab
        G4OpticalSurface* scintWrap = new G4OpticalSurface("ScintWrap");
        
        new G4LogicalBorderSurface("ScintWrap", slab,
                                   fExperimentalHall_phys,
                                   scintWrap);
        
        scintWrap->SetType(dielectric_metal);
        scintWrap->SetFinish(polished);
        scintWrap->SetModel(glisur);
        
        G4double pp[] = {2.0*eV, 3.5*eV};
        const G4int num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {1., 1.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {0.0, 0.0};
        assert(sizeof(efficiency) == sizeof(pp));
        
        G4MaterialPropertiesTable* scintWrapProperty
        = new G4MaterialPropertiesTable();
        
        scintWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        scintWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        scintWrap->SetMaterialPropertiesTable(scintWrapProperty);
    }
    */
    
    return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::ConstructSDandField() {
    
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::SetDefaults() {
    
    //Resets to default values
    fNfibers=1;
    
    if(fMPTPStyrene)fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
    
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestDetectorConstruction::SetNFibers(G4int n) {
    fNfibers=n;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......