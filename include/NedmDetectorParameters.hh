#ifndef NedmDetectorParameters_h
#define NedmDetectorParameters_h

#include "G4ThreeVector.hh"

class NedmDetectorParameters
{
    
public:
    
    // Getters
    inline static NedmDetectorParameters* instance();

    inline G4ThreeVector cell_size() const { return cell_size_; }

    inline G4bool cell_wall_on() const { return cell_wall_on_;}
    inline G4bool tpb_layer_on() const { return tpb_layer_on_;}
    inline G4bool embedded_fibers() const { return embedded_fibers_;}
    inline G4bool fiber_reflector() const { return fiber_reflector_;}
    inline G4bool outer_reflector() const { return outer_reflector_;}
    
    inline G4int num_fibers() const { return num_fibers_;}
    inline G4double fiber_thick() const { return fiber_thick_; }
    inline G4double fiber_spacing() const { return fiber_spacing_;}
    inline G4double tpb_outer_fraction() const {return tpb_outer_fraction_;}
    // Setters

private:
    
    NedmDetectorParameters();
    static NedmDetectorParameters* instance_;
    
    void SetDefaults();
    
    G4ThreeVector cell_size_;
    
    G4bool cell_wall_on_;
    G4bool tpb_layer_on_;
    G4bool embedded_fibers_;
    G4bool fiber_reflector_;
    G4bool outer_reflector_;
    
    G4int num_fibers_;
    G4double fiber_thick_;
    G4double fiber_spacing_;
    G4double tpb_outer_fraction_;


    
};


inline
NedmDetectorParameters* NedmDetectorParameters::instance() {
    if (!instance_)
        instance_ = new NedmDetectorParameters;
    return instance_;
}



#endif
