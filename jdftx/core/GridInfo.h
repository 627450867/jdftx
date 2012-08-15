/*-------------------------------------------------------------------
Copyright 2011 Ravishankar Sundararaman

This file is part of JDFTx.

JDFTx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JDFTx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JDFTx.  If not, see <http://www.gnu.org/licenses/>.
-------------------------------------------------------------------*/

#ifndef JDFTX_CORE_GRIDINFO_H
#define JDFTX_CORE_GRIDINFO_H

/** @file GridInfo.h
@brief Geometry of the simulation grid
*/

#include <core/matrix3.h>
#include <core/Data.h>
#include <core/GpuUtil.h>
#include <map>
#include <fftw3.h>
#include <stdint.h>
#include <cstdio>

/** @brief Simulation grid descriptor
//! @ingroup griddata

To setup a simulation grid, create a blank GridInfo object,
set the public data members #S and #R, and call #initialize().

This sets up all the auxilliary grid information, and  a bunch
of shared utilities such as a random number generator,
plans for Fourier transforms etc.
*/
class GridInfo
{
public:

	matrix3<> R; //!< lattice vectors
	double Gmax; //!< radius of wavefunction G-sphere, whode density sphere (double the radius) must be inscribable within the FFT box
	vector3<int> S; //!< sample points in each dimension (if 0, will be determined automatically based on Gmax)

	//! Initialize the dependent quantities below.
	//! If S is specified and is too small for the given Gmax, the call will abort.
	void initialize();


	double detR; //!< cell volume
	matrix3<> RT, RTR, invR, invRT, invRTR; //!< various combinations of lattice-vectors
	matrix3<> G, GT, GGT, invGGT; //!< various combinations of reciporcal lattice-vectors

	double dV; //!< volume per grid point
	vector3<> h[3]; //!< real space sample vectors
	int nr; //!< position space grid count = S[0]*S[1]*S[2]
	int nG; //!< reciprocal lattice count = S[0]*S[1]*(S[2]/2+1) [on account of using r2c and c2r ffts]

	//FFT plans:
	fftw_plan planForwardSingle; //!< Single-thread Forward complex transform
	fftw_plan planInverseSingle; //!< Single-thread Inverse complex transform
	fftw_plan planForwardInPlaceSingle; //!< Single-thread Forward in-place complex transform
	fftw_plan planInverseInPlaceSingle; //!< Single-thread Inverse in-place complex transform
	fftw_plan planRtoCsingle; //!< Single-thread FFTW plan for R -> G
	fftw_plan planCtoRsingle; //!< Single-thread FFTW plan for G -> R
	fftw_plan planForwardMulti; //!< Multi-threaded Forward complex transform
	fftw_plan planInverseMulti; //!< Multi-threaded Inverse complex transform
	fftw_plan planForwardInPlaceMulti; //!< Multi-threaded Forward in-place complex transform
	fftw_plan planInverseInPlaceMulti; //!< Multi-threaded Inverse in-place complex transform
	fftw_plan planRtoCmulti; //!< Multi-threaded FFTW plan for R -> G
	fftw_plan planCtoRmulti; //!< Multi-threaded FFTW plan for G -> R
	#ifdef GPU_ENABLED
	cufftHandle planZ2Z; //!< CUFFT plan for all the complex transforms
	cufftHandle planD2Z; //!< CUFFT plan for R -> G
	cufftHandle planZ2D; //!< CUFFT plan for G -> R
	cufftHandle planZ2Dcompat; //!< CUFFT plan for G -> R in FFTW compatibility mode (required when nyquist component is assymetric)
	#endif

	//Indexing utilities (inlined for efficiency)
	inline vector3<int> wrapGcoords(const vector3<int> iG) const //!< wrap negative G-indices to the positive side
	{	vector3<int> iGwrapped = iG;
		for(int k=0; k<3; k++)
			if(iGwrapped[k]<0)
				iGwrapped[k] += S[k];
		return iGwrapped;
	}
	inline int fullRindex(const vector3<int> iR) const //!< Index into the full real-space box:
	{	return iR[2] + S[2]*(iR[1] + S[1]*iR[0]);
	}
	inline int fullGindex(const vector3<int> iG) const //!< Index into the full reciprocal-space box:
	{	return fullRindex(wrapGcoords(iG));
	}
	inline int halfGindex(const vector3<int> iG) const //!< Index into the half-reduced reciprocal-space box:
	{	vector3<int> iGwrapped = wrapGcoords(iG);
		return iGwrapped[2] + (S[2]/2+1)*(iGwrapped[1] + S[1]*iGwrapped[0]);
	}

	GridInfo();
	~GridInfo();
	
private:
	bool initialized; //!< keep track of whether initialize() has been called
};

#endif //JDFTX_CORE_GRIDINFO_H