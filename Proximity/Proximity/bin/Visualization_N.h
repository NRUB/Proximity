#pragma once
#include "Visualization.h"

class Visualization_N : public Visualization {
public:
	Visualization_N();
	~Visualization_N();
	virtual void update(TimedLevel* pLevel);
	virtual void updateAll(TimedLevel* pLevel);
	virtual void newMedia(IWMPMedia* pMedia);
};