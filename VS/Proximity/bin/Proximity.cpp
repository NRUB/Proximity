#include "Proximity.h"
#include "Visualization_V.h"
#include "Visualization_N.h"

CProximity::CProximity() {
	hwnd = nullptr;
	visual = nullptr;
	visuals = std::vector<Visualization*>();
}

CProximity::~CProximity() {

}

HRESULT CProximity::FinalConstruct() {
	visuals.push_back(new Visualization_V());
	visuals.push_back(new Visualization_N());
	return S_OK;
}

VOID CProximity::FinalRelease() {

}

HRESULT STDMETHODCALLTYPE CProximity::Render(TimedLevel* pLevels, HDC hdc, RECT* prc) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::MediaInfo(LONG lChannelCount, LONG lSampleRate, BSTR bstrTitle) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::GetCapabilities(DWORD* pdwCapabilities) {
	if (pdwCapabilities != nullptr) {
		*pdwCapabilities = EFFECT_CANGOFULLSCREEN;
		return S_OK;
	}
	return E_POINTER;
}

HRESULT STDMETHODCALLTYPE CProximity::GetTitle(BSTR* bstrTitle) {
	if (bstrTitle != nullptr) {
		CComBSTR bstr = L"Proximity";
		*bstrTitle = bstr.Detach();
		return S_OK;
	}
	return E_POINTER;
}

HRESULT STDMETHODCALLTYPE CProximity::GetPresetTitle(LONG nPreset, BSTR* bstrPresetTitle) {
	if (bstrPresetTitle != nullptr) {
		CComBSTR bstrTitle;
		switch (nPreset) {
			case 0:
				bstrTitle = L"V";
				break;
			case 1:
				bstrTitle = L"N";
				break;
			default:
				return E_INVALIDARG;
		}
		*bstrPresetTitle = bstrTitle.Detach();
		return S_OK;

	}
	return E_POINTER;
}

HRESULT STDMETHODCALLTYPE CProximity::GetPresetCount(LONG* pnPresetCount) {
	if (pnPresetCount != nullptr) {
		*pnPresetCount = (long)visuals.size();
		return S_OK;
	}
	return E_POINTER;
}

HRESULT STDMETHODCALLTYPE CProximity::SetCurrentPreset(LONG nPreset) {
	visual = visuals[nPreset];
	IWMPMedia* media;
	pCore->get_currentMedia(&media);
	visual->newMedia(media);
	visual->updateAll(nullptr);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::GetCurrentPreset(LONG* pnPreset) {
	if (pnPreset != nullptr) {
		if (visual != nullptr) {
			for (unsigned int i = 0; i < visuals.size(); ++i) {
				if (visuals[i] == visual) {
					*pnPreset = i;
					return S_OK;
				}
			}
			*pnPreset = (-1);
			return S_OK;
		}
		else {
			*pnPreset = (-1);
			return S_OK;
		}
	}
	return E_POINTER;
}

HRESULT STDMETHODCALLTYPE CProximity::DisplayPropertyPage(HWND hwndOwner) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::GoFullscreen(BOOL fFullScreen) {
	if (visual != nullptr) {
		visual->updateAll(nullptr);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::RenderFullScreen(TimedLevel* pLevels) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CProximity::SetCore(IWMPCore* pPlayer) {
	pCore = pPlayer;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::Create(HWND hwndParent) {
	hwnd = hwndParent;
	for (unsigned int i = 0; i < visuals.size(); ++i) {
		visuals[i]->setHwnd(hwnd);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::Destroy(void) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::NotifyNewMedia(IWMPMedia* pMedia) {
	if (visual != nullptr) {
		visual->newMedia(pMedia);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProximity::OnWindowMessage(UINT msg, WPARAM WParam, LPARAM LParam, LRESULT* plResultParam) {
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CProximity::RenderWindowed(TimedLevel* pData, BOOL fRequiredRender) {
	if (visual != nullptr) {
		if (fRequiredRender == TRUE) {
			visual->updateAll(pData);
		}
		visual->update(pData);
	}
	return S_OK;
}