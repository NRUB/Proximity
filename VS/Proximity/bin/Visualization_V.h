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

	int notes = 12;
	double multipliers[8] = {0.10, 0.10, 0.05, 0.25, 0.70, 0.80, 0.80, 0.20};
	double frequencies[12][8] =
	{
		{32.70, 65.41, 130.8, 261.6, 523.3, 1047, 2093, 4186},
		{34.65, 69.30, 138.6, 277.2, 554.4, 1109, 2217, 4435},
		{36.71, 73.42, 146.8, 293.7, 587.3, 1175, 2349, 4699},
		{38.89, 77.78, 155.6, 311.1, 622.3, 1245, 2489, 4978},
		{41.20, 82.41, 164.8, 329.6, 659.3, 1319, 2637, 5274},
		{43.65, 87.31, 174.6, 349.2, 698.5, 1397, 2794, 5588},
		{46.25, 92.50, 185.0, 370.0, 740.0, 1480, 2960, 5920},
		{49.00, 98.00, 196.0, 392.0, 784.0, 1568, 3136, 6272},
		{51.91, 103.8, 207.7, 415.3, 830.6, 1661, 3322, 6645},
		{55.00, 110.0, 220.0, 440.0, 880.0, 1760, 3520, 7040},
		{58.27, 116.5, 233.1, 466.2, 932.3, 1865, 3729, 7459},
		{61.74, 123.5, 246.9, 493.9, 987.8, 1976, 3951, 7902}
	};
};