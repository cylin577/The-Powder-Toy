#include "simulation/ToolCommon.h"

#include "gui/game/Brush.h"
#include <cmath>

static void performDraw(SimTool *tool, Simulation * sim, const Brush &brush, ui::Point position);
static void performDrawLine(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point position1, ui::Point position2, bool dragging);
static void performDrag(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point position1, ui::Point position2);

void SimTool::Tool_STCK()
{
	Identifier = "DEFAULT_TOOL_STCK";
	Name = "STCK";
	Colour = 0x8080A0_rgb;
	Description = "Pulls particles toward the cursor center.";
	PerformDraw = &performDraw;
	PerformDrawLine = &performDrawLine;
	PerformDrag = &performDrag;
}

static void stackAt(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point center, float strength)
{
	ui::Point minOffset = brush.GetRadius() * -1;
	ui::Point maxOffset = brush.GetRadius();

	int steps = std::max(1, int(strength));

	for (int i = 0; i < sim->parts.active; i++)
	{
		if (!sim->parts[i].type)
			continue;

		int px = int(sim->parts[i].x + 0.5f);
		int py = int(sim->parts[i].y + 0.5f);

		if (px < center.X + minOffset.X || px > center.X + maxOffset.X || py < center.Y + minOffset.Y || py > center.Y + maxOffset.Y)
			continue;

		int dx = 0;
		int dy = 0;
		if (center.X > px)
			dx = std::min(center.X - px, steps);
		else if (center.X < px)
			dx = std::max(center.X - px, -steps);
		if (center.Y > py)
			dy = std::min(center.Y - py, steps);
		else if (center.Y < py)
			dy = std::max(center.Y - py, -steps);

		sim->parts[i].x += float(dx);
		sim->parts[i].y += float(dy);
	}
}

static void performDraw(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point position)
{
	stackAt(tool, sim, brush, position, tool->Strength);
}

static void performDrawLine(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point position1, ui::Point position2, bool dragging)
{
	stackAt(tool, sim, brush, position2, tool->Strength);
}

static void performDrag(SimTool *tool, Simulation *sim, const Brush &brush, ui::Point position1, ui::Point position2)
{
	stackAt(tool, sim, brush, position2, tool->Strength);
}
