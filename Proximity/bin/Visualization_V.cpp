#include "Visualization_V.h"

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

		int width = 408; //24*(16+1)
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
		int samples = 16;
		int step = 4;
		unsigned char* points = new unsigned char[samples + 2];
		points[0] = 0;
		points[samples + 1] = 0;
		int buffer = 0;
		int iter = 1;
		for (int n = 64; n < 128; ++n) {
			buffer += pLevel->frequency[0][n];
			buffer += pLevel->frequency[1][n];
			if (n % step == (step - 1)) {
				buffer /= 2; //channels
				buffer /= step; //sample size
				buffer /= 2; //scale
				points[iter] = ((int)(std::pow(buffer, 2) / std::pow(128, 2) * 64)) * 2;
				++iter;
				buffer = 0;
			}
		}

		//Draw main line
		Gdiplus::Point* draw = new Gdiplus::Point[samples + 2];
		for (int i = 0; i < samples + 2; ++i) {
			draw[i].X = left + i * 24;
			draw[i].Y = bottom - points[i];
		}
		delete points;
		graphics.DrawLines(mainLinePen, draw, samples + 2);
		delete draw;

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

		int width = 408; //24*(16+1)
		int left = 30;
		int bottom = (rc.bottom - rc.top) - 170;

		//Clear
		graphics.FillRectangle(clearBrush, Gdiplus::Rect(left - 4, bottom + 4, width + 8, 120));

		//Draw title background
		graphics.FillRectangle(mainLineBrush, Gdiplus::Rect(left, bottom + 10, width, 50));

		//Draw author outline
		SelectObject(hdc, fontAuthor);
		SIZE size;
		GetTextExtentPoint32(hdc, author.c_str(), author.size(), &size);
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