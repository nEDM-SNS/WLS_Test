#include "NedmDetectorParameters.hh"

#include "G4SystemOfUnits.hh"


NedmDetectorParameters* NedmDetectorParameters::instance_;

NedmDetectorParameters::NedmDetectorParameters() {
    SetDefaults();
}

void NedmDetectorParameters::SetDefaults() {

    // Old geometry
    // G4double cell_x = 5.08*cm;
    // G4double cell_y = 0.635*cm;
    // G4double cell_z = 20.64*cm;
    // cell_size_ = G4ThreeVector(cell_x,cell_y,cell_z);
    //

    cell_half_z_ = 4.445*cm;
    cellSquareInner_half_ = 2.032*cm;
    cellSquareOuter_half_ = cellSquareInner_half_ + 0.15875*cm;
    cellCircleInner_rad_ = 3.175*cm;
    cellCircleOuter_rad_ = cellCircleInner_rad_ + 0.3175*cm;
    
    G4double TPBouterFraction_ = 0.01;

    // Do not change
    cell_wall_on_ = true; //true
    tpb_layer_on_ = true; //true
    embedded_fibers_ = false; //false
    fiber_reflector_ = false; //false
    outer_reflector_ = true; //false
    
    num_fibers_ = 100;
    fiber_thick_ = 0.100*cm;
    fiber_spacing_ = 0.103*cm;
   
    cFiber_half_z_ = 15.24*cm;
    cFiber_rad_ = 0.75*mm;
}
