#include "DXUT.h"
#include "DXUTApplication.h"

//--------------------------------------------------------------------------------------
// Extern declarations 
//--------------------------------------------------------------------------------------
extern bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
	bool bWindowed, void* pUserContext);
extern HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
extern HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext);
extern void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime,
	void* pUserContext);
extern void CALLBACK OnD3D9LostDevice(void* pUserContext);
extern void CALLBACK OnD3D9DestroyDevice(void* pUserContext);


extern bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
extern void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
extern LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext);
extern void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
extern void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
	int xPos, int yPos, void* pUserContext);

extern bool CALLBACK OnDeviceRemoved(void* pUserContext);



DXUTApplication::DXUTApplication()
{
}

int DXUTApplication::Run()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// DXUT will create and use the best device (either D3D9 or D3D11) 
	// that is available on the system depending on which D3D callbacks are set below

	// Set general DXUT callbacks
	DXUTSetCallbackFrameMove(::OnFrameMove);
	DXUTSetCallbackKeyboard(::OnKeyboard);
	DXUTSetCallbackMouse(::OnMouse);
	DXUTSetCallbackMsgProc(::MsgProc);
	DXUTSetCallbackDeviceChanging(::ModifyDeviceSettings);
	DXUTSetCallbackDeviceRemoved(::OnDeviceRemoved);

	// Set the D3D9 DXUT callbacks. Remove these sets if the app doesn't need to support D3D9
	DXUTSetCallbackD3D9DeviceAcceptable(::IsD3D9DeviceAcceptable);
	DXUTSetCallbackD3D9DeviceCreated(::OnD3D9CreateDevice);
	DXUTSetCallbackD3D9DeviceReset(::OnD3D9ResetDevice);
	DXUTSetCallbackD3D9FrameRender(::OnD3D9FrameRender);
	DXUTSetCallbackD3D9DeviceLost(::OnD3D9LostDevice);
	DXUTSetCallbackD3D9DeviceDestroyed(::OnD3D9DestroyDevice);

	// Perform any application-level initialization here

	DXUTInit(true, true, NULL); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
	DXUTCreateWindow(L"DX9");

	// Only require 10-level hardware
	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, 640, 480);
	DXUTMainLoop(); // Enter into the DXUT ren  der loop

					// Perform any application-level cleanup here

	return DXUTGetExitCode();
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
