#include "DXUT.h"
#include "SurroundCameraHandler.h"

void SurroundCameraHandler::OnD3D9FrameRender(IDirect3DDevice9 * pd3dDevice, double fTime, float fElapsedTime, void * pUserContext)
{
	D3DXMATRIX matView;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookat = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	eye.x = distance*sin(angleDirection)*cos(anglePitch);
	eye.z = distance*cos(angleDirection)*cos(anglePitch);
	eye.y = distance*sin(anglePitch);

	D3DXMatrixLookAtLH(&matView, &eye, &lookat, &up);
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}

void SurroundCameraHandler::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void * pUserContext)
{
}

void SurroundCameraHandler::OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void * pUserContext)
{
	activate = bMiddleButtonDown;
	this->distance *= (1 - nMouseWheelDelta*sensitive*0.05f);	
}

LRESULT SurroundCameraHandler::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool * pbNoFurtherProcessing, void * pUserContext)
{
	static int lastx = 0;
	static int lasty = 0;

	if (uMsg == WM_MOUSEMOVE)
	{
		int xPos = (short)LOWORD(lParam);
		int yPos = (short)HIWORD(lParam);

		if (!activate)
		{
			lastx = xPos;
			lasty = yPos;
		}

		float dx = xPos - lastx;
		float dy = yPos - lasty;

		lastx = xPos;
		lasty = yPos;

		this->angleDirection += dx*sensitive;
		this->anglePitch += dy*sensitive;

	}
	return 0;
}

HRESULT SurroundCameraHandler::OnD3D9ResetDevice(IDirect3DDevice9 * pd3dDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 3, fAspectRatio, 0.1, 100);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

SurroundCameraHandler::SurroundCameraHandler()
{
}


SurroundCameraHandler::~SurroundCameraHandler()
{
}
