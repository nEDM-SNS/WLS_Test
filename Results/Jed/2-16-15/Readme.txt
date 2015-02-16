This run was produced after the problem with the fiber reflector was fixed.  A switch was added to turn the fiber reflector on and off.  The 2 root files and graphs correspond to the 2 states of this switch.

Source - 
/gps/particle opticalphoton
/gps/energy 2.95 eV #blue photon
/gps/position 0 1 0 cm
/gps/ang/type beam1d
/gps/direction 0 -1 0
/gps/polarization 0 0 1 # dummy polarization
/run/beamOn 10000

Readout - 
Any photon hitting the edge of the fiber core is recorded and then killed.