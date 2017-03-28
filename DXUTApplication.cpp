#include "DXUT.h"
#include "DXUTApplication.h"


DXUTApplication::DXUTApplication()
{
}

DXUTApplication * DXUTApplication::Instance()
{
	static DXUTApplication theApp;
	return &theApp;
}

void DXUTApplication::AddHandler(std::shared_ptr<DXUTEventHandler> handler)
{
	this->handlers.push_back(handler);
}

template<class T>
void DXUTApplication::AddHandler() {
	DXUTEventHandler* p = new T;
	std::shared_ptr<DXUTEventHandler> ptr(p);

	Instance()->AddHandler(ptr);
}


DXUTApplication::~DXUTApplication()
{
}

bool DXUTApplication::IsD3D9DeviceAcceptable(D3DCAPS9 * pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
	bool bWindowed, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++) {
		if (!i->get()->IsD3D9DeviceAcceptable(pCaps, AdapterFormat, BackBufferFormat, bWindowed, pUserContext))
			return false;
	}
	return true;
}

HRESULT DXUTApplication::OnD3D9CreateDevice(IDirect3DDevice9 * pd3dDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	HRESULT hr;

	for (auto i = handlers.begin(); i != handlers.end(); i++)
		V(i->get()->OnD3D9CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));

	return S_OK;
}

HRESULT DXUTApplication::OnD3D9ResetDevice(IDirect3DDevice9 * pd3dDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	HRESULT hr;

	for (auto i = handlers.begin(); i != handlers.end(); i++)
		V(i->get()->OnD3D9ResetDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));

	return S_OK;
}

void DXUTApplication::OnD3D9FrameRender(IDirect3DDevice9 * pd3dDevice, double fTime, float fElapsedTime, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnD3D9FrameRender(pd3dDevice, fTime, fElapsedTime, pUserContext);
}

void DXUTApplication::OnD3D9LostDevice(void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnD3D9LostDevice(pUserContext);
}

void DXUTApplication::OnD3D9DestroyDevice(void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnD3D9DestroyDevice(pUserContext);
}

bool DXUTApplication::ModifyDeviceSettings(DXUTDeviceSettings * pDeviceSettings, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++) {
		if (!i->get()->ModifyDeviceSettings(pDeviceSettings, pUserContext))
			return false;
	}
	return true;
}

void DXUTApplication::OnFrameMove(double fTime, float fElapsedTime, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnFrameMove(fTime, fElapsedTime, pUserContext);
}

LRESULT DXUTApplication::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool * pbNoFurtherProcessing, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);

	return 0;
}

void DXUTApplication::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
}

void DXUTApplication::OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void * pUserContext)
{
	for (auto i = handlers.begin(); i != handlers.end(); i++)
		i->get()->OnMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down,
			bSideButton2Down, nMouseWheelDelta, xPos, yPos, pUserContext);

}

bool DXUTApplication::OnDeviceRemoved(void * pUserContext)
{
	bool r = true;

	for (auto i = handlers.begin(); i != handlers.end(); i++) {
		if (!i->get()->OnDeviceRemoved(pUserContext))
			r = false;
	}
	return r;
}
