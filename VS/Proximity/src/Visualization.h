#pragma once
#include <atldef.h>
#include <atlbase.h>
#include <effects.h>
#include <gdiplus.h>

class Visualization {
public:
	Visualization();
	virtual ~Visualization();
	virtual void update(TimedLevel* pLevel) = 0;
	virtual void updateAll(TimedLevel* pLevel) = 0;
	virtual void newMedia(IWMPMedia* pMedia) = 0;
	void setHwnd(HWND handle);
protected:
	HWND hwnd;
	void freeHGDI(HGDIOBJ obj);
};