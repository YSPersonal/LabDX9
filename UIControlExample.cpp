#include "DXUT.h"
#include "DXUTgui.h"
#include "SDKmisc.h"
#include "DXUTEventHandler.h"

#define ID_BUTTON 1001
int clickCount = 0;
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	if (nControlID == ID_BUTTON)
		clickCount++;
}

class UIControlExample : public DXUTEventHandler {

	ID3DXFont* font;
	CDXUTDialogResourceManager drManager;
	CDXUTDialog dialog;
	

	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override {

		HRESULT hr;

		V_RETURN(D3DXCreateFont(pd3dDevice, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			L"Arial", &font));


		V_RETURN(drManager.OnD3D9CreateDevice(pd3dDevice));
		
		return S_OK;
	}

	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext) override {
		
		HRESULT hr;
		V_RETURN(drManager.OnD3D9ResetDevice());

		return S_OK;
	}


	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) override {

		CDXUTTextHelper txtHelper(font, NULL, 24);
		HRESULT hr;

		// Clear the render target and the zbuffer 
		V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

		// Render the scene
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			// Output statistics
			txtHelper.Begin();
			txtHelper.SetInsertionPos(2, 0);

			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

			wchar_t buffer[24];
			wsprintf(buffer, L"Click count: %d", clickCount);
			txtHelper.DrawTextLine(buffer);

			dialog.OnRender(fElapsedTime);

			V(pd3dDevice->EndScene());
		}		
		
	}

	void CALLBACK OnD3D9DestroyDevice(void * pUserContext)
	{
		SAFE_RELEASE(font);
		drManager.OnD3D9DestroyDevice();
	}

	void CALLBACK OnD3D9LostDevice(void* pUserContext) override {
		drManager.OnD3D9LostDevice();
	}

	LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext) override{

		*pbNoFurtherProcessing = drManager.MsgProc(hWnd, uMsg, wParam, lParam);
		if (*pbNoFurtherProcessing)
			return 0;

		*pbNoFurtherProcessing = dialog.MsgProc(hWnd, uMsg, wParam, lParam);
		if (*pbNoFurtherProcessing)
			return 0;


		return 0;

	}
public:
	UIControlExample() {
		dialog.Init(&drManager);
		dialog.SetCallback(OnGUIEvent);
		dialog.AddButton(ID_BUTTON, L"Click", 100, 100, 200, 50);
	}
};

//auto p=new UIControlExample();