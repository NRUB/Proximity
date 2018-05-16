#pragma once
#include "Visualization.h"
#include <vector>
#include <string>
#include <math.h>

class Visualization_V : public Visualization {
public:
	Visualization_V();
	~Visualization_V();
	virtual void update(TimedLevel* pLevel);
	virtual void updateAll(TimedLevel* pLevel);
	virtual void newMedia(IWMPMedia* pMedia);
	void drawMedia();
private:
	Gdiplus::Color* clearColor;
	Gdiplus::SolidBrush* clearBrush;
	Gdiplus::SolidBrush* lightBrush;
	Gdiplus::SolidBrush* mainLineBrush;
	Gdiplus::Pen* baseLinePen;
	Gdiplus::Pen* mainLinePen;
	HFONT fontTitle;
	HFONT fontAuthor;

	std::wstring author;
	std::wstring title;
	double duration;
};