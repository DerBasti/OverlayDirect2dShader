#include "WaveEffect.h"
#include "../IO/FileReader.h"
#include "../ProjectFilePathHandler.h"

extern "C" {
	const GUID CLSID_WaveEffect = { 0x4ab0f341, 0x473b, 0x4924, { 0xb4, 0x2a, 0xda, 0x65, 0x86, 0x0, 0x43, 0x53 } };
	const GUID CLSID_WaveEffectVertexShader = { 0x1d2c6971, 0xf2fd, 0x4d26, { 0xb1, 0xa5, 0xe7, 0x52, 0x98, 0x27, 0xa6, 0xfb } };
};

WaveEffect::WaveEffect() {
	vertexAmount = 0;
	waveStrength = 1.0f;
	waveOffset = 0;
	m_refCount = 1;
	drawInfo = nullptr;
	drawingConstants = { 0 };
	inputRect = D2D1::RectL();
}

// 2.1 Declare ID2D1EffectImpl implementation methods.
IFACEMETHODIMP WaveEffect::Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {
	FileInputReader fir((ProjectFilePathHandler::GetDefaultFilePath() + std::string("Design/WaveEffect.cso")).c_str());
	uint32_t fileLength = (uint32_t)fir.getFileSize();
	std::shared_ptr<char> shaderData = fir.readStringWrapped(fileLength);
	auto hr = pContextInternal->LoadVertexShader(CLSID_WaveEffectVertexShader, (const byte*)shaderData.get(), fileLength);
	if (hr != S_OK) {
		return hr;
	}

	Vertex* vertex = nullptr;
	generateVertexMesh(&vertex);

	D2D1_VERTEX_BUFFER_PROPERTIES vbProp = { 0 };
	vbProp.byteWidth = sizeof(Vertex) * vertexAmount;
	vbProp.data = reinterpret_cast<BYTE*>(vertex);
	vbProp.inputCount = 1;
	vbProp.usage = D2D1_VERTEX_USAGE_STATIC;
	static const D2D1_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{"MESH_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0},
	};
	D2D1_CUSTOM_VERTEX_BUFFER_PROPERTIES cvbProp = { 0 };
	cvbProp.elementCount = ARRAYSIZE(vertexLayout);
	cvbProp.inputElements = vertexLayout;
	cvbProp.stride = sizeof(Vertex);
	cvbProp.shaderBufferWithInputSignature = (const byte*)shaderData.get();
	cvbProp.shaderBufferSize = fileLength;
	hr = pContextInternal->CreateVertexBuffer(&vbProp, nullptr, &cvbProp, &vertexBuffer);
	
	delete[] vertex;
	vertex = nullptr;
	hr = pTransformGraph->SetSingleTransformNode(this);
	return S_OK;
}

IFACEMETHODIMP WaveEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType) {
	drawingConstants.matrix = D2D1::Matrix4x4F::RotationX(3);
	drawingConstants.waveOffset = waveOffset;
	drawingConstants.waveStrength = waveStrength;
	return drawInfo->SetVertexShaderConstantBuffer(reinterpret_cast<const BYTE*>(&drawingConstants), sizeof(drawingConstants));

}

HRESULT WaveEffect::generateVertexMesh(Vertex** vertexBuffer) {
	vertexAmount = 6 * TESSELATION_AMOUNT * TESSELATION_AMOUNT;

	Vertex* tesselationBuffer = new (std::nothrow) Vertex[vertexAmount];

	if (tesselationBuffer == nullptr)
	{
		return E_OUTOFMEMORY;
	}

	float offset = 1.0f / TESSELATION_AMOUNT;

	for (int i = 0; i < TESSELATION_AMOUNT; i++)
	{
		for (int j = TESSELATION_AMOUNT - 1; j >= 0; j--)
		{
			int index = (i * TESSELATION_AMOUNT + j) * 6;

			// This mesh consists of pairs of triangles forming squares. Each square contains
			// six vertices, three for each triangle. 'offset' represents the distance between each vertice
			// in the triangles. In this mesh, we only set the x and y coordinates of the vertices, since
			// they are the only variable part of the geometry. In the vertex shader, z is generated
			// based on x, and w is defined to be '1'. The actual coordinates here range from 0 to 1,
			// these values are scaled up based on the size of the image in the vertex shader.

			tesselationBuffer[index].x = i * offset;
			tesselationBuffer[index].y = j * offset;
			tesselationBuffer[index + 1].x = i * offset;
			tesselationBuffer[index + 1].y = j * offset + offset;
			tesselationBuffer[index + 2].x = i * offset + offset;
			tesselationBuffer[index + 2].y = j * offset;
			tesselationBuffer[index + 3].x = i * offset + offset;
			tesselationBuffer[index + 3].y = j * offset;
			tesselationBuffer[index + 4].x = i * offset;
			tesselationBuffer[index + 4].y = j * offset + offset;
			tesselationBuffer[index + 5].x = i * offset + offset;
			tesselationBuffer[index + 5].y = j * offset + offset;
		}
	}
	*vertexBuffer = tesselationBuffer;
	return S_OK;
}

// 2.3 Declare IUnknown implementation methods.
IFACEMETHODIMP_(ULONG) WaveEffect::AddRef() {
	this->m_refCount++;
	return this->m_refCount;
}
IFACEMETHODIMP_(ULONG) WaveEffect::Release() {
	this->m_refCount--;
	if (m_refCount <= 0) {
		delete this;
		return 0;
	}
	return m_refCount;
}

#define XML(X) TEXT(#X)

// 2.2 Declare effect registration methods.
HRESULT WaveEffect::Register(_In_ ID2D1Factory1* pFactory) {

	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
			<!--System Properties-->
				<Property name='DisplayName' type='string' value='Wave Effect'/>
				<Property name='Author' type='string' value='Microsoft Corporation'/>
				<Property name='Category' type='string' value='Sample'/>
				<Property name='Description' type='string' value='Animation of a wave.'/>
				<Inputs>
					<Input name='Source'/>
				</Inputs>
				<!--Effect - specific Properties-->
				<Property name='WaveOffset' type = 'float' value='0'>
					<Property name='DisplayName' type='string' value='Wave Offset'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='WaveStrength' type='float' value='0.5'>
					<Property name='DisplayName' type='string' value='Wave Strength'/>
					<Property name='Min' type='float' value='0.0'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.5'/>
				</Property>
			</Effect>
		);

	// Property bindings are performed automatically for the system properties.
	// For effect-specific properties (i.e. WaveOffset), you must direct
	// Direct2D to the specific method in your implementation for manipulating
	// the property. This is done by assigning each property a 'setter' and 'getter'
	// in this table. Direct2D provides a set of helpers for doing this in
	// D2D1EffectHelpers.h, such as D2D1_VALUE_TYPE_BINDING used here.
	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"WaveOffset",  &setWaveOffset,  &getWaveOffset),
		D2D1_VALUE_TYPE_BINDING(L"WaveStrength",  &setWaveStrength,  &getWaveStrength)
		//D2D1_VALUE_TYPE_BINDING(L"SkewX",  &SetSkewX,  &GetSkewX),
		//D2D1_VALUE_TYPE_BINDING(L"SkewY",  &SetSkewY,  &GetSkewY)
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_WaveEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateEffect
	);
}
HRESULT WaveEffect::CreateEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new WaveEffect();
	return S_OK;
}

IFACEMETHODIMP WaveEffect::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput) {
	*ppOutput = nullptr;
	HRESULT hr = S_OK;

	if (riid == __uuidof(ID2D1EffectImpl))
	{
		*ppOutput = reinterpret_cast<ID2D1EffectImpl*>(this);
	}
	else if (riid == __uuidof(ID2D1DrawTransform))
	{
		*ppOutput = static_cast<ID2D1DrawTransform*>(this);
	}
	else if (riid == __uuidof(ID2D1Transform))
	{
		*ppOutput = static_cast<ID2D1Transform*>(this);
	}
	else if (riid == __uuidof(ID2D1TransformNode))
	{
		*ppOutput = static_cast<ID2D1TransformNode*>(this);
	}
	else if (riid == __uuidof(IUnknown))
	{
		*ppOutput = this;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	if (*ppOutput != nullptr)
	{
		AddRef();
	}

	return hr;
}

// Declare ID2D1DrawTransform implementation methods.
IFACEMETHODIMP WaveEffect::SetDrawInfo(_In_ ID2D1DrawInfo* pDrawInfo) {
	drawInfo = pDrawInfo;

	D2D1_VERTEX_RANGE range;
	range.startVertex = 0;
	range.vertexCount = vertexAmount;

	return drawInfo->SetVertexProcessing(
		vertexBuffer.Get(),
		D2D1_VERTEX_OPTIONS_USE_DEPTH_BUFFER,
		nullptr, // Setting the blend description to nullptr draws the geometry using a simple source-over blend.
		&range,
		&CLSID_WaveEffectVertexShader
	);
}

// Declare ID2D1Transform implementation methods.
IFACEMETHODIMP WaveEffect::MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const {
	if (inputRectCount != 1)
	{
		return E_FAIL;
	}
	pInputRects[0] = *pOutputRect;
	return S_OK;

}

IFACEMETHODIMP WaveEffect::MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L *pInputRects, _In_reads_(inputRectCount) CONST D2D1_RECT_L *pInputOpaqueSubRects, UINT32 inputRectCount,
	_Out_ D2D1_RECT_L *pOutputRect, _Out_ D2D1_RECT_L *pOutputOpaqueSubRect) {
	HRESULT hr = S_OK;

	if (inputRectCount != 1)
	{
		return E_FAIL;
	}

	inputRect = pInputRects[0];
	*pOutputRect = pInputRects[0];

	drawingConstants.sizeX = float(inputRect.right - inputRect.left);
	drawingConstants.sizeY = float(inputRect.bottom - inputRect.top);
	ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
	return hr;

}

IFACEMETHODIMP WaveEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L *pInvalidOutputRect) const {
	// Indicate that the entire output may be invalid.
	*pInvalidOutputRect = inputRect;

	return S_OK;
}