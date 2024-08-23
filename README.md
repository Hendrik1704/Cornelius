# Cornelius
Modern implementation of the Cornelius algorithm to find freeze-out hypersurfaces from relativistic hydrodynamics.

This code is based on cornelius++ version 1.3 (Copyright 2012) by Pasi Huovinen and Hannu Holopainen.
The Cornelius algorithm is aimed to be used as a part of the fluid dynamical models of the heavy-ion physics community.
Publications using this algorithm should cite:
 * P. Huovinen and H. Petersen, Eur.Phys.J.A 48 (2012) 171, arXiv:1206.3371 [nucl-th]

This new version leverages new C++17 features and introduces a few unit tests.
The main change is the one from manual dynamic memory management to automatic 
dynamic memory management in some parts of the code using `std::vector`, while 
most arrays are implemented as `std::array` now.
There are also tests making sure that the results obtained by the new version 
are compatible with the old one.

The speed test comparing the old and the new version is implemented in the `main`
executable. It performs the surface finding for 10 different cubes and repeats 
this 50k times for each cube to obtain a better time measurement. 
The average total execution time is then printed to the terminal.
In the 3D case the new version needs roughly 75% of the execution time of the old
version. The 4D case is only slightly faster compared to the old version.