#pragma once

#include "DXUT.h"
class DXUTEventHandler
{
public:

	virtual bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
		bool bWindowed, void* pUserContext);
	virtual HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	virtual HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext);
	virtual void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime,
		void* pUserContext);
	virtual void CALLBACK OnD3D9LostDevice(void* pUserContext);
	virtual void CALLBACK OnD3D9DestroyDevice(void* pUserContext);

	virtual bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
	virtual void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext);
	virtual void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	virtual void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext);

	virtual bool CALLBACK OnDeviceRemoved(void* pUserContext);

	void Set();

	DXUTEventHandler();
	~DXUTEventHandler();
};

