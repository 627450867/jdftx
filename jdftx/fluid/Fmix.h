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

#ifndef JDFTX_FLUID_FMIX_H
#define JDFTX_FLUID_FMIX_H

#include <core/DataCollection.h>
#include <string>

class FluidMixture;

//! Abstract base class for mixing functionals: interactions between fluids (beyond hard sphere and scaled coulomb)
class Fmix
{
public:
	FluidMixture& fluidMixture;
	const GridInfo& gInfo;
	const double T;

	//! Initialize base and register with this fluidMixture
	Fmix(FluidMixture& fluidMixture);

	virtual string getName() const=0; //!< A string identifier for this mixing functional (used in EnergyComponent label)

	//! Return the interaction free energy given the reciprocal space site densities
	//! and accumulate the gradient (functional derivative) w.r.t them in grad_Ntilde
	//! Note that unlike Fex, all site densities are handed to an Fmix and it
	//! is Fmix's responsibility to pick up the correct site densities
	//! (perhaps using FluidMixture::get_offsetDensity())
	virtual double compute(const DataGptrCollection& Ntilde, DataGptrCollection& grad_Ntilde) const=0;

	//! Return the uniform fluid interaction free energy density given the site densities N
	//! and accumulate the derivative w.r.t them in grad_N. This MUST return the
	//! result corresponding to calling compute() with a uniform scalar field.
	//! This is called several times during FluidMixture::setPressure() to get the desired bulk properties
	//! Note that unlike Fex, all site densities are handed to an Fmix and it
	//! is Fmix's responsibility to pick up the correct site densities
	//! (perhaps using FluidMixture::get_offsetDensity())
	virtual double computeUniform(const std::vector<double>& N, std::vector<double>& grad_N) const=0;
};

#endif // JDFTX_FLUID_FMIX_H