#include "Visualization.h"

Visualization::Visualization() {

}

Visualization::~Visualization() {

}

void Visualization::setHwnd(HWND handle) {
	hwnd = handle;
}

void Visualization::freeHGDI(HGDIOBJ obj) {
	if (obj != nullptr) {
		DeleteObject(obj);
	}
}