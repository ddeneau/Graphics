#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory.h>

template<typename T>
class ConstantBuffer {
public:
	ConstantBuffer(ID3D11Device* device) {
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = device->CreateBuffer(&bd, nullptr, m_buffer.GetAddressOf());
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create constant buffer on GPU");
		}
	}

	void Update(ID3D11DeviceContext* context, const T& data) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, &data, sizeof(T));

		context->Unmap(m_buffer.Get(), 0);
	}

	void BindVS(ID3D11DeviceContext* context, UINT slot) {
		context->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	ID3D11Buffer** GetAddressOf() {
		return m_buffer.GetAddressOf();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};