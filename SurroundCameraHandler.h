#pragma once
#include "DXUTEventHandler.h"
class SurroundCameraHandler :
	public DXUTEventHandler
{
	float angleDirection=D3DX_PI/3;
	float anglePitch=D3DX_PI/6;
	float distance=5;
	float sensitive=0.01f;

	bool activate = false;	

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) override;

	void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext) override;
	void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext) override;

	LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext) override;

	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override;
	
public:

	SurroundCameraHandler();
	~SurroundCameraHandler();
};

