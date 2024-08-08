# Cornelius
Modern implementation of the Cornelius algorithm to find freeze-out hypersurfaces from relativistic hydrodynamics.


This code is based on cornelius++ version 1.3 (Copyright 2012) by Pasi Huovinen and Hannu Holopainen.
The Cornelius algorithm is aimed to be used as a part of the fluid dynamical models of the heavy-ion physics community.
Publications using this algorithm should cite:
 * P. Huovinen and H. Petersen, Eur.Phys.J.A 48 (2012) 171, arXiv:1206.3371 [nucl-th]

This new version leverages the newest C++17 features, makes the code slightly more memory efficient with faster performance and unit tests.
There are also tests making sure that the results obtained by the new version are compatible with the old one.