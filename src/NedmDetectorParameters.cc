#include "NedmDetectorParameters.hh"

#include "G4SystemOfUnits.hh"


NedmDetectorParameters* NedmDetectorParameters::instance_;

NedmDetectorParameters::NedmDetectorParameters() {
    SetDefaults();
}

void NedmDetectorParameters::SetDefaults() {

    G4double cell_x = 5.08*cm;
    G4double cell_y = 0.635*cm;
    G4double cell_z = 20.64*cm;

    cell_size_ = G4ThreeVector(cell_x,cell_y,cell_z);
    
    
    cell_wall_on_ = true;
    embedded_fibers_ = true;
    fiber_reflector_ = false;
    
    num_fibers_ = 98;
    fiber_thick_ = 0.100*cm;
    fiber_spacing_ = 0.101*cm;
    
}