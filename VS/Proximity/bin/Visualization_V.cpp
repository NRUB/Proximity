#include "Visualization_V.h"
#include <iostream>

Visualization_V::Visualization_V() {
	clearColor = new Gdiplus::Color(10, 10, 10);

	clearBrush = new Gdiplus::SolidBrush(*clearColor);
	lightBrush = new Gdiplus::SolidBrush(Gdiplus::Color(30, 30, 30));
	mainLineBrush = new Gdiplus::SolidBrush(Gdiplus::Color(230, 230, 230));

	baseLinePen = new Gdiplus::Pen(Gdiplus::Color(130, 130, 130), 1);
	mainLinePen = new Gdiplus::Pen(Gdiplus::Color(230, 230, 230), 3);

	fontTitle = CreateFont(48, 14, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Nova Flat");
	fontAuthor = CreateFont(26, 10, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Nova Flat");

	duration = DBL_MAX;
}

Visualization_V::~Visualization_V() {
	freeHGDI(clearColor);
	freeHGDI(clearBrush);
	freeHGDI(lightBrush);
	freeHGDI(mainLineBrush);
	freeHGDI(baseLinePen);
	freeHGDI(mainLinePen);
	freeHGDI(fontTitle);
	freeHGDI(fontAuthor);

	delete clearColor;
	delete clearBrush;
	delete lightBrush;
	delete mainLineBrush;
	delete baseLinePen;
	delete mainLinePen;
}

void Visualization_V::update(TimedLevel* pLevel) {
	if (pLevel != nullptr && pLevel->state == play_state && hwnd != nullptr) {
		RECT rc;
		GetClientRect(hwnd, &rc);
		HDC hdc = GetDC(hwnd);
		Gdiplus::Graphics gdc(hdc);

		Gdiplus::Bitmap bitmap(rc.right - rc.left, rc.bottom - rc.top);
		Gdiplus::Graphics graphics(&bitmap);
		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);

		int width = 416; //32 * (notes + 1);
		int left = 30;
		int bottom = (rc.bottom - rc.top) - 170;

		//Clear
		graphics.FillRectangle(clearBrush, Gdiplus::Rect(left - 5, bottom - 128, width + 12, 135));

		//Draw base line
		graphics.DrawLine(baseLinePen, Gdiplus::Point(left, bottom), Gdiplus::Point(left + width, bottom));

		//Draw timestamp
		long long time = pLevel->timeStamp / 10000;
		int progress = (int)(width * time / duration) + left;
		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
		graphics.DrawEllipse(baseLinePen, Gdiplus::Rect(progress - 5, bottom - 5, 10, 10));

		//Calculate spectrogram
		std::vector<unsigned char> points;
		points.resize(notes + 2);
		points[0] = 0;
		points[notes + 1] = 0;
		for (int note = 0; note < notes; ++note) {
			double buffer = 0;
			for (int octave = 0; octave < 8; ++octave)
			{
				double index = ((frequencies[note][octave] - 20) / 22030) * 1024;
				double fraction = index - ((int)index);
				unsigned char Lf = (pLevel->frequency[0][(int)index] + pLevel->frequency[1][(int)index]) / 2;
				unsigned char Rf = (pLevel->frequency[0][(int)index + 1] + pLevel->frequency[1][(int)index + 1]) / 2;
				double interpolated = ((Lf - Rf) * fraction) + Rf;
				buffer += interpolated * multipliers[octave];
			}
			buffer /= 4;
			points[note + 1] = ((int)(pow(buffer, 2) / pow(128, 2) * 64));
		}

		//Draw main line
		std::unique_ptr<Gdiplus::Point[]> draw = std::make_unique<Gdiplus::Point[]>(notes + 2);
		for (int i = 0; i < notes + 2; ++i) {
			draw[i].X = left + i * 32;
			draw[i].Y = bottom - points[i];
		}
		graphics.DrawLines(mainLinePen, draw.get(), notes + 2);

		//Flush
		gdc.DrawImage(&bitmap, 0, 0);

		freeHGDI(&bitmap);
		freeHGDI(&graphics);
		freeHGDI(&gdc);
		ReleaseDC(hwnd, hdc);
	}
}

void Visualization_V::updateAll(TimedLevel* pLevel) {
	if (hwnd != nullptr) {
		HDC hdc = GetDC(hwnd);
		Gdiplus::Graphics gdc(hdc);
		gdc.Clear(*clearColor);
		drawMedia();

		freeHGDI(&gdc);
		ReleaseDC(hwnd, hdc);
	}
}

void Visualization_V::newMedia(IWMPMedia* pMedia) {
	BSTR buffer;
	pMedia->getItemInfo(L"Author", &buffer);
	author = buffer;
	pMedia->getItemInfo(L"Title", &buffer);
	title = buffer;
	pMedia->get_duration(&duration);
	duration *= 1000;
	if (duration <= 0) {
		duration = DBL_MAX;
	}
	drawMedia();
}

void Visualization_V::drawMedia() {
	if (hwnd != nullptr) {
		RECT rc;
		GetClientRect(hwnd, &rc);
		HDC hdc = GetDC(hwnd);
		Gdiplus::Graphics gdc(hdc);

		Gdiplus::Bitmap bitmap(rc.right - rc.left, rc.bottom - rc.top);
		Gdiplus::Graphics graphics(&bitmap);
		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);

		int width = 416;
		int left = 30;
		int bottom = (rc.bottom - rc.top) - 170;

		//Clear
		graphics.FillRectangle(clearBrush, Gdiplus::Rect(left - 4, bottom + 4, width + 8, 120));

		//Draw title background
		graphics.FillRectangle(mainLineBrush, Gdiplus::Rect(left, bottom + 10, width, 50));

		//Draw author outline
		SelectObject(hdc, fontAuthor);
		SIZE size;
		GetTextExtentPoint32(hdc, author.c_str(), (int)author.size(), &size);
		int w = ((size.cx > 0) ? (size.cx + 28) : 0);
		graphics.FillRectangle(mainLineBrush, Gdiplus::Rect((int)left, (int)bottom + 60, w, (int)40));
		graphics.FillRectangle(clearBrush, Gdiplus::Rect((int)left + 2, (int)bottom + 60, w - 4, (int)38));

		//Flush
		gdc.DrawImage(&bitmap, 0, 0);

		//Draw title
		SetTextColor(hdc, RGB(10, 10, 10));
		SetBkColor(hdc, RGB(230, 230, 230));
		SetBkMode(hdc, TRANSPARENT);
		GetClientRect(hwnd, &rc);
		rc.left = 44;
		rc.right = 440;
		rc.top = rc.bottom - rc.top - 162;
		rc.bottom = rc.top + 48;
		SelectObject(hdc, fontTitle);
		DrawText(hdc, title.c_str(), -1, &rc, DT_SINGLELINE | DT_NOCLIP);

		//Draw author
		SetTextColor(hdc, RGB(230, 230, 230));
		SetBkColor(hdc, RGB(10, 10, 10));
		SetBkMode(hdc, TRANSPARENT);
		rc.top = rc.bottom + 8;
		rc.bottom = rc.bottom + 30;
		SelectObject(hdc, fontAuthor);
		DrawText(hdc, author.c_str(), -1, &rc, DT_SINGLELINE | DT_NOCLIP);

		freeHGDI(hdc);
		freeHGDI(&gdc);
		freeHGDI(&bitmap);
		freeHGDI(&graphics);
		ReleaseDC(hwnd, hdc);
	}
}