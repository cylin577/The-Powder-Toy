#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_THMS()
{
	Identifier = "DEFAULT_PT_THMS";
	Name = "THMS";
	Colour = 0xAA00FF_rgb;
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 0;
	HeatCapacity = 100.0f; // High capacity as requested
	Description = "Thermal Master. Infinite thermal capacity, forces adjacent particles to its temperature.";

	Properties = TYPE_SOLID | PROP_NOAMBHEAT;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	float myTemp = parts[i].temp;
	for (int rx = -1; rx <= 1; rx++)
	{
		for (int ry = -1; ry <= 1; ry++)
		{
			if (rx == 0 && ry == 0) continue;
			int nx = x + rx;
			int ny = y + ry;
			if (nx >= 0 && nx < XRES && ny >= 0 && ny < YRES)
			{
				int r = pmap[ny][nx];
				if (r)
				{
					parts[ID(r)].temp = myTemp;
				}
				r = sim->photons[ny][nx];
				if (r)
				{
					parts[ID(r)].temp = myTemp;
				}
			}
		}
	}
	return 0;
}
