#include "DXUT.h"
#include "DXUTEventHandler.h"
#include<memory>

static std::unique_ptr<DXUTEventHandler> handler = NULL;

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
	bool bWindowed, void* pUserContext)
{
	return handler->IsD3D9DeviceAcceptable(pCaps, AdapterFormat, BackBufferFormat, bWindowed, pUserContext);
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{

	return handler->OnD3D9CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	return handler->OnD3D9ResetDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, 
	void* pUserContext)
{
	handler->OnD3D9FrameRender(pd3dDevice, fTime, fElapsedTime, pUserContext);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice(void* pUserContext)
{
	handler->OnD3D9LostDevice(pUserContext);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice(void* pUserContext)
{
	handler->OnD3D9DestroyDevice(pUserContext);
}




//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return handler->ModifyDeviceSettings(pDeviceSettings,pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	handler->OnFrameMove(fTime, fElapsedTime, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext)
{
	return handler->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	handler->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
	int xPos, int yPos, void* pUserContext)
{
	handler->OnMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down,
		bSideButton2Down, nMouseWheelDelta, xPos, yPos, pUserContext);
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved(void* pUserContext)
{
	return handler->OnDeviceRemoved(pUserContext);
}


bool DXUTEventHandler::IsD3D9DeviceAcceptable(D3DCAPS9 * pCaps, D3DFORMAT AdapterFormat,
	D3DFORMAT BackBufferFormat, bool bWindowed, void * pUserContext)
{
	// Typically want to skip back buffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if (FAILED(pD3D->CheckDeviceFormat(pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat)))
		return false;

	return true;
}

HRESULT DXUTEventHandler::OnD3D9CreateDevice(IDirect3DDevice9 * pd3dDevice, 
	const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	return S_OK;
}

HRESULT DXUTEventHandler::OnD3D9ResetDevice(IDirect3DDevice9 * pd3dDevice, 
	const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	return S_OK;
}

void DXUTEventHandler::OnD3D9FrameRender(IDirect3DDevice9 * pd3dDevice, double fTime, float fElapsedTime, 
	void * pUserContext)
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	// Render the scene
	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		V(pd3dDevice->EndScene());
	}
}

void DXUTEventHandler::OnD3D9LostDevice(void * pUserContext)
{
}

void DXUTEventHandler::OnD3D9DestroyDevice(void * pUserContext)
{
}






bool DXUTEventHandler::ModifyDeviceSettings(DXUTDeviceSettings * pDeviceSettings, void * pUserContext)
{
	return true;
}

void DXUTEventHandler::OnFrameMove(double fTime, float fElapsedTime, void * pUserContext)
{
}

LRESULT DXUTEventHandler::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
	bool * pbNoFurtherProcessing, void * pUserContext)
{
	return 0;
}

void DXUTEventHandler::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void * pUserContext)
{
}

void DXUTEventHandler::OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, 
	bool bSideButton1Down, bool bSideButton2Down, 
	int nMouseWheelDelta, 
	int xPos, int yPos, void * pUserContext)
{
}

bool DXUTEventHandler::OnDeviceRemoved(void * pUserContext)
{
	return true;
}

void DXUTEventHandler::Set()
{
	if (handler.get() != this)
		handler.reset(this);
}

DXUTEventHandler::DXUTEventHandler()
{
	if (handler.get() == NULL)
		Set();
}


DXUTEventHandler::~DXUTEventHandler()
{
}

//typedef std::shared_ptr<DXUTEventHandler> DXUTEventHandler;

