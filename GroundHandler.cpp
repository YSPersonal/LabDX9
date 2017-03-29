#include "DXUT.h"
#include "SDKmisc.h"
#include "GroundHandler.h"

//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
extern HRESULT LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh);
//{
//	ID3DXMesh* pMesh = NULL;
//
//	WCHAR str[MAX_PATH];
//	HRESULT hr;
//
//	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this
//	// sample we'll ignore the X file's embedded materials since we know 
//	// exactly the model we're loading.  See the mesh samples such as
//	// "OptimizedMesh" for a more generic mesh loading example.
//	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, strFileName));
//	V_RETURN(D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh));
//
//	DWORD* rgdwAdjacency = NULL;
//
//	// Make sure there are normals which are required for lighting
//	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
//	{
//		ID3DXMesh* pTempMesh;
//		V(pMesh->CloneMeshFVF(pMesh->GetOptions(),
//			pMesh->GetFVF() | D3DFVF_NORMAL,
//			pd3dDevice, &pTempMesh));
//		V(D3DXComputeNormals(pTempMesh, NULL));
//
//		SAFE_RELEASE(pMesh);
//		pMesh = pTempMesh;
//	}
//
//	// Optimize the mesh for this graphics card's vertex cache 
//	// so when rendering the mesh's triangle list the vertices will 
//	// cache hit more often so it won't have to re-execute the vertex shader 
//	// on those vertices so it will improve perf.     
//	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
//	if (rgdwAdjacency == NULL)
//		return E_OUTOFMEMORY;
//	V(pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency));
//	V(pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL));
//	delete[]rgdwAdjacency;
//
//	*ppMesh = pMesh;
//
//	return S_OK;
//}

struct VF {
	D3DXVECTOR3 position;
	DWORD color;
};

#define FVF D3DFVF_XYZ | D3DFVF_DIFFUSE

void GroundHandler::CreateGridBuffer(int halfWidth, int halfHeight, IDirect3DDevice9 * pd3dDevice,
	IDirect3DVertexBuffer9 ** out)
{
	int size = (halfWidth + halfHeight + 1) * 4 * sizeof(VF);

	pd3dDevice->CreateVertexBuffer(size, 0, FVF, D3DPOOL_DEFAULT, out, NULL);
	void* data = NULL;
	(*out)->Lock(0, size, &data, 0);
	VF* varray = (VF*)data;

	int count = 0;
	for (int i = 0; i < halfWidth * 2 + 1; i++) {
		varray[count++].position = D3DXVECTOR3(i - halfWidth, 0, halfHeight);
		varray[count++].position = D3DXVECTOR3(i - halfWidth, 0, -halfHeight);
	}

	for (int i = 0; i < halfHeight * 2 + 1; i++) {
		varray[count++].position = D3DXVECTOR3(-halfWidth, 0, i - halfHeight);
		varray[count++].position = D3DXVECTOR3(halfWidth, 0, i - halfHeight);
	}

	for (int i = 0; i < (halfWidth + halfHeight + 1) * 4; i++) {
		varray[i].color = 0xFFFFFFFF;
	}



}

HRESULT GroundHandler::OnD3D9CreateDevice(IDirect3DDevice9 * pd3dDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext)
{
	HRESULT hr;

	CreateGridBuffer(halfWidth, halfHeight, pd3dDevice, &pVB);
	V(LoadMesh(pd3dDevice, L"indoor.x", &objMesh));

	return S_OK;
}

void GroundHandler::OnD3D9FrameRender(IDirect3DDevice9 * pd3dDevice, double fTime, float fElapsedTime, void * pUserContext)
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Render the scene

	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld, 0, 0, 0);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(VF));
		pd3dDevice->SetFVF(FVF);
		pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, halfWidth * 2 + halfHeight * 2 + 2);
		
		objMesh->DrawSubset(1);
		// End the scene
			

		pd3dDevice->EndScene();

	}
}

void GroundHandler::OnD3D9DestroyDevice(void * pUserContext)
{
	SAFE_RELEASE(pVB);
	SAFE_RELEASE(objMesh);
}

GroundHandler::GroundHandler()
{
}


GroundHandler::~GroundHandler()
{
}


#include "DXUTApplication.h"
#include "SurroundCameraHandler.h"

int main() {

	DXUTApplication::Instance()->AddHandler(new SurroundCameraHandler);
	DXUTApplication::Instance()->AddHandler(new GroundHandler);
	DXUTApplication::Run();

	return 0;
}