#ifndef NedmWLSFiber_H
#define NedmWLSFiber_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"


class NedmWLSFiber : public G4PVPlacement
{
  public:

    NedmWLSFiber(G4RotationMatrix *pRot,
                const G4ThreeVector &tlate,
                G4LogicalVolume *pMotherLogical,
                G4bool pMany,
                G4int pCopyNo);

  private:

    void CopyValues();

    static G4LogicalVolume* fClad2_log;

    G4double fFiber_rmin;
    G4double fFiber_rmax;
    G4double fFiber_z;
    G4double fFiber_sphi;
    G4double fFiber_ephi;

    G4double fClad1_rmin;
    G4double fClad1_rmax;    
    G4double fClad1_z;
    G4double fClad1_sphi;
    G4double fClad1_ephi; 

    G4double fClad2_rmin;
    G4double fClad2_rmax;    
    G4double fClad2_z;
    G4double fClad2_sphi;
    G4double fClad2_ephi;
    
    G4double fMirrorRmax;
    G4double fMirrorRmin;
    G4double fMirrorZ;
    G4double fMirrorSPhi;
    G4double fMirrorEPhi;
    
    G4double fMirrorPosZ;
    G4double fMirrorReflectivity;
    
};

#endif
