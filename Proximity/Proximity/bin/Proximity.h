#pragma once
#include <atldef.h>
#include <atlbase.h>
#include <atlcom.h>
#include <effects.h>
#include <vector>
#include "../resource.h"
#include "../midl/Proximity_h.h"
#include "Visualization.h"

class ATL_NO_VTABLE CProximity :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CProximity, &CLSID_Proximity>,
	public IDispatchImpl<IProximity, &IID_IProximity, &LIBID_Proximity>,
	public IWMPEffects2 {
public:
	CProximity();
	~CProximity();

	DECLARE_REGISTRY_RESOURCEID(IDR_PROXIMITY)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CProximity)
		COM_INTERFACE_ENTRY(IWMPEffects)
		COM_INTERFACE_ENTRY(IWMPEffects2)
		COM_INTERFACE_ENTRY(IProximity)
	END_COM_MAP()

public:
	HRESULT FinalConstruct(VOID);
	VOID FinalRelease(VOID);

	//IWMPEffects
	virtual HRESULT STDMETHODCALLTYPE Render(TimedLevel* pLevels, HDC hdc, RECT* prc);
	virtual HRESULT STDMETHODCALLTYPE MediaInfo(LONG lChannelCount, LONG lSampleRate, BSTR bstrTitle);
	virtual HRESULT STDMETHODCALLTYPE GetCapabilities(DWORD* pdwCapabilities);
	virtual HRESULT STDMETHODCALLTYPE GetTitle(BSTR* bstrTitle);
	virtual HRESULT STDMETHODCALLTYPE GetPresetTitle(LONG nPreset, BSTR* bstrPresetTitle);
	virtual HRESULT STDMETHODCALLTYPE GetPresetCount(LONG* pnPresetCount);
	virtual HRESULT STDMETHODCALLTYPE SetCurrentPreset(LONG nPreset);
	virtual HRESULT STDMETHODCALLTYPE GetCurrentPreset(LONG* pnPreset);
	virtual HRESULT STDMETHODCALLTYPE DisplayPropertyPage(HWND hwndOwner);
	virtual HRESULT STDMETHODCALLTYPE GoFullscreen(BOOL fFullScreen);
	virtual HRESULT STDMETHODCALLTYPE RenderFullScreen(TimedLevel* pLevels);

	//IWMPEffects2
	virtual HRESULT STDMETHODCALLTYPE SetCore(IWMPCore* pPlayer);
	virtual HRESULT STDMETHODCALLTYPE Create(HWND hwndParent);
	virtual HRESULT STDMETHODCALLTYPE Destroy(void);
	virtual HRESULT STDMETHODCALLTYPE NotifyNewMedia(IWMPMedia* pMedia);
	virtual HRESULT STDMETHODCALLTYPE OnWindowMessage(UINT msg, WPARAM WParam, LPARAM LParam, LRESULT* plResultParam);
	virtual HRESULT STDMETHODCALLTYPE RenderWindowed(TimedLevel* pData, BOOL fRequiredRender);

private:
	HWND hwnd;
	Visualization* visual;
	std::vector<Visualization*> visuals;
	IWMPCore* pCore;
};

OBJECT_ENTRY_AUTO(CLSID_Proximity, CProximity)