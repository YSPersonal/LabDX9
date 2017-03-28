#pragma once
#include<vector>
#include<memory>
#include<list>

#include "DXUTEventHandler.h"


class DXUTApplication :
	public DXUTEventHandler
{

	std::vector<std::shared_ptr<DXUTEventHandler>> handlers;
	bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
		bool bWindowed, void* pUserContext) override;


	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override;


	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext) override;


	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime,
		void* pUserContext) override;


	void CALLBACK OnD3D9LostDevice(void* pUserContext) override;


	void CALLBACK OnD3D9DestroyDevice(void* pUserContext) override;


	bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext) override;


	void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext) override;


	LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext) override;


	void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext) override;


	void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext) override;


	bool CALLBACK OnDeviceRemoved(void* pUserContext) override;

	DXUTApplication();
public:

	static int Run();

	static DXUTApplication* Instance();
	
	void AddHandler(std::shared_ptr<DXUTEventHandler> handler);

	template<class T>
	static void AddHandler();

	~DXUTApplication();
};
