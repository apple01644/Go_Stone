#include "GameManager.h"
#include "LevelManager.h"
#include "Player.h"

ExceptionHandler& operator << (ExceptionHandler& eh, HRESULT hr)
{
	if (FAILED(hr))
	{
		throw "exception";
	}
	return eh;
}

GameManager::GameManager(HWND hWnd, int client_w, int client_h)
{
	this->client_w = client_w;
	this->client_h = client_h;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = client_w;
	sd.BufferDesc.Height = client_h;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	
	ex <<  D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
		D3D10_SDK_VERSION, &sd, swapchain.GetAddressOf(), device.GetAddressOf());

	ID3D10Texture2D* pBackBuffer;
	ex << swapchain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);

	ex << device->CreateRenderTargetView(pBackBuffer, NULL, rtv.GetAddressOf());
	pBackBuffer->Release();

	device->OMSetRenderTargets(1, rtv.GetAddressOf(), NULL);

	D3D10_VIEWPORT vp;
	vp.Width = client_w;
	vp.Height = client_h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	device->RSSetViewports(1, &vp);

	D3D10_RASTERIZER_DESC rsd = {};
	rsd.CullMode = D3D10_CULL_MODE::D3D10_CULL_NONE;
	rsd.FillMode = D3D10_FILL_MODE::D3D10_FILL_SOLID;

	ex << device->CreateRasterizerState(&rsd, rts.GetAddressOf());
	device->RSSetState(rts.Get());

	D3D10_BLEND_DESC bsd = {};
	bsd.BlendEnable[0] = TRUE;
	bsd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	bsd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	bsd.DestBlend = D3D10_BLEND::D3D10_BLEND_INV_SRC_ALPHA;
	bsd.BlendOp = D3D10_BLEND_OP_ADD;
	bsd.SrcBlendAlpha = D3D10_BLEND_ZERO;
	bsd.DestBlendAlpha = D3D10_BLEND_ZERO;
	bsd.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	
	ex << device->CreateBlendState(&bsd, blend.GetAddressOf());
	device->OMSetBlendState(blend.Get(), nullptr, 0xffff);

	ComPtr<ID3D10Blob> vs_blob;
	ComPtr<ID3D10Blob> ps_blob;
	ComPtr<ID3D10Blob> err_blob;
	{
		HRESULT hr = D3DX10CompileFromFileA("poly.hlsl", nullptr, nullptr, "VS", "vs_4_0", D3D10_SHADER_DEBUG, NULL, nullptr, vs_blob.GetAddressOf(), err_blob.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			const char* buf = (char*)err_blob->GetBufferPointer();
			ex << hr;
		}
		hr = D3DX10CompileFromFileA("poly.hlsl", nullptr, nullptr, "PS", "ps_4_0", D3D10_SHADER_DEBUG, NULL, nullptr, ps_blob.GetAddressOf(), err_blob.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			const char* buf = (char*)err_blob->GetBufferPointer();
			ex << hr;
		}
		ex << device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), poly_vs.GetAddressOf());
		ex << device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), poly_ps.GetAddressOf());

		const D3D10_INPUT_ELEMENT_DESC poly_layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0}
		};

		ex << device->CreateInputLayout(poly_layout, 1, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), common_il.GetAddressOf());
	}
	{
		HRESULT hr = D3DX10CompileFromFileA("sprite.hlsl", nullptr, nullptr, "VS", "vs_4_0", D3D10_SHADER_DEBUG, NULL, nullptr, vs_blob.GetAddressOf(), err_blob.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			const char* buf = (char*)err_blob->GetBufferPointer();
			ex << hr;
		}
		hr = D3DX10CompileFromFileA("sprite.hlsl", nullptr, nullptr, "PS", "ps_4_0", D3D10_SHADER_DEBUG, NULL, nullptr, ps_blob.GetAddressOf(), err_blob.GetAddressOf(), nullptr);
		if (FAILED(hr))
		{
			const char* buf = (char*)err_blob->GetBufferPointer();
			ex << hr;
		}
		ex << device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), sprite_vs.GetAddressOf());
		ex << device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), sprite_ps.GetAddressOf());
	}

	ex << D3DX10CreateShaderResourceViewFromFileA(device.Get(), "circle512.png", nullptr, nullptr, sprite_map["circle"].GetAddressOf(), nullptr);
	
	ex << D3DX10CreateFontA(device.Get(), 0, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN | DEFAULT_PITCH, "Consolas",
		font_map["sample"].GetAddressOf());

	{
		D3D10_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(RectangleBuffer);
		bd.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		bd.Usage = D3D10_USAGE_DYNAMIC;

		ex << device->CreateBuffer(&bd, nullptr, object_buffer.GetAddressOf());
	}
	{
		
		D3D10_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(XMFLOAT3) * rect_vertices.size();
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		D3D10_SUBRESOURCE_DATA sbd = {};
		sbd.pSysMem = rect_vertices.data();

		ex << device->CreateBuffer(&bd, &sbd, rect_vertices_buffer.GetAddressOf());
	}
}

void GameManager::draw_rect(XMFLOAT2 pos, XMFLOAT2 size, XMINT4 color)
{
	device->IASetInputLayout(common_il.Get());
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->IASetVertexBuffers(0, 1, rect_vertices_buffer.GetAddressOf(), &rect_stride, &rect_offset);
	device->VSSetConstantBuffers(0, 1, object_buffer.GetAddressOf());
	device->VSSetShader(poly_vs.Get());
	device->PSSetShader(poly_ps.Get());
	device->GSSetShader(nullptr);
	{
		RectangleBuffer* mat = nullptr;
		ex << object_buffer->Map(D3D10_MAP::D3D10_MAP_WRITE_DISCARD, NULL, (void**)&mat);
		mat->transform = {
			size.x / client_w * 2, 0.f, 0.f, -1.f + pos.x / client_w * 2,
			0.f, -size.y / client_h * 2, 0.f, 1.f - pos.y / client_h * 2,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		mat->color = { color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f };
		object_buffer->Unmap();
	}
	device->Draw(6, 0);
}

void GameManager::draw_sprite(string name, XMFLOAT2 pos, XMFLOAT2 size, XMINT4 diffuse, bool mirrored)
{
	device->IASetInputLayout(common_il.Get());
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->IASetVertexBuffers(0, 1, rect_vertices_buffer.GetAddressOf(), &rect_stride, &rect_offset);
	device->VSSetConstantBuffers(0, 1, object_buffer.GetAddressOf());
	device->VSSetShader(sprite_vs.Get());
	device->PSSetShader(sprite_ps.Get());
	device->PSSetShaderResources(0, 1, sprite_map[name].GetAddressOf());
	device->GSSetShader(nullptr);
	{
		RectangleBuffer* mat = nullptr;
		ex << object_buffer->Map(D3D10_MAP::D3D10_MAP_WRITE_DISCARD, NULL, (void**)&mat);
		XMMATRIX matrix = XMMatrixIdentity();
		if (mirrored)
		{
			matrix *= XMMatrixTranslation(-0.5f, 0.f, 0.f);
			matrix *= XMMatrixScaling(-1.f, 1.f, 1.f);
			matrix *= XMMatrixTranslation(0.5f, 0.f, 0.f);
		}
		matrix *= XMMatrixScaling(size.x / client_w * 2, -size.y / client_h * 2, 1.f);
		matrix *= XMMatrixTranslation(-1.f + pos.x / client_w * 2, 1.f - pos.y / client_h * 2, 0.f);
		XMStoreFloat4x4(&mat->transform, XMMatrixTranspose(matrix));

		mat->color = { diffuse.x / 255.f, diffuse.y / 255.f, diffuse.z / 255.f, diffuse.w / 255.f };
		object_buffer->Unmap();
	}
	device->Draw(6, 0);
}
void GameManager::draw_sprite_angle(string name, XMFLOAT2 pos, XMFLOAT2 size, float angle, XMINT4 diffuse, bool mirrored)
{
	device->IASetInputLayout(common_il.Get());
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->IASetVertexBuffers(0, 1, rect_vertices_buffer.GetAddressOf(), &rect_stride, &rect_offset);
	device->VSSetConstantBuffers(0, 1, object_buffer.GetAddressOf());
	device->VSSetShader(sprite_vs.Get());
	device->PSSetShader(sprite_ps.Get());
	device->PSSetShaderResources(0, 1, sprite_map[name].GetAddressOf());
	device->GSSetShader(nullptr);
	{
		RectangleBuffer* mat = nullptr;
		ex << object_buffer->Map(D3D10_MAP::D3D10_MAP_WRITE_DISCARD, NULL, (void**)&mat);
		XMMATRIX matrix = XMMatrixIdentity();
		matrix *= XMMatrixTranslation(-0.5f, -0.5f, 0.f);
		matrix *= XMMatrixRotationZ(angle);
		matrix *= XMMatrixTranslation(0.5f, 0.5f, 0.f);
		matrix *= XMMatrixScaling(size.x / client_w * 2, -size.y / client_h * 2, 1.f);
		matrix *= XMMatrixTranslation(-1.f + pos.x / client_w * 2, 1.f - pos.y / client_h * 2, 0.f);
		XMStoreFloat4x4(&mat->transform, XMMatrixTranspose(matrix));

		mat->color = { diffuse.x / 255.f, diffuse.y / 255.f, diffuse.z / 255.f, diffuse.w / 255.f };
		object_buffer->Unmap();
	}
	device->Draw(6, 0);
}

void GameManager::draw_text(string name, string content, XMFLOAT2 pos, XMINT4 color)
{
	RECT rect = {
		pos.x, pos.y,
		client_w, client_h
	};
	font_map[name]->DrawTextA(nullptr, content.data(), -1, &rect, NULL, D3DXCOLOR(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f));
}

void GameManager::draw_text(string name, wstring content, XMFLOAT2 pos, XMINT4 color)
{
	RECT rect = {
		pos.x, pos.y,
		client_w, client_h
	};
	font_map[name]->DrawTextW(nullptr, content.data(), -1, &rect, NULL, D3DXCOLOR(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f));
}

void GameManager::Awake()
{
	level_map["start_menu"] = new LvStartMenu();
	level_map["tutorial"] = new LvTutorial();
	level_map["gameover"] = new LvGameOver();
	level_map["stage1-1"] = new LvStage1_1();
	level_map["stage1-2"] = new LvStage1_2();
	level_map["stage1-3"] = new LvStage1_3();
	level_map["stage2-1"] = new LvStage2_1();
	level_map["stage2-2"] = new LvStage2_2();
	level_map["stage2-3"] = new LvStage2_3();
	level_map["ranking"] = new LvRanking();
	for (auto pair : level_map) pair.second->OnStart();
	next_level_name = "start_menu";
}

void GameManager::Loop() {
	if (current_level != nullptr)
	{
		current_level->mtx.lock();
		current_level->OnLoop();
		current_level->mtx.unlock();
	}
	swapchain->Present(0, 0);
	if (next_level_name != current_level_name)
	{
		GotoLevel(next_level_name);
	}
}

void GameManager::Peek(const MSG& msg) {
	static Player& player = Player::get_instance();
	if (msg.message == WM_KEYDOWN)
	{
		key_states[msg.wParam] = true;
		if (msg.wParam == VK_F1)
		{
			player.unbreakable = !player.unbreakable;
		}
		else if (msg.wParam == VK_F2)
		{
			player.GrowUp();
		}
		else if (msg.wParam == VK_F4)
		{
			next_level_name = "start_menu";
		}
		else if (msg.wParam == VK_F5)
		{
			next_level_name = "stage1-1";
		}
		else if (msg.wParam == VK_F6)
		{
			next_level_name = "stage2-1";
		}
	}
	else if (msg.message == WM_KEYUP)
	{
		key_states[msg.wParam] = false;
	}
}


void GameManager::GotoLevel(string lv_name)
{
	if (current_level != nullptr)
	{
		current_level->closing = true;
		current_level->OnExit();
	}
	current_level = level_map[lv_name];
	current_level_name = lv_name;
	current_level->closing = false;
	current_level->OnEnter();
}

GameManager::~GameManager()
{
	for (auto pair : level_map) delete pair.second;	
}

void GameManager::BindSprite(string name, string filename)
{
	filename.push_back('\0');
	ex << D3DX10CreateShaderResourceViewFromFileA(device.Get(), filename.c_str(), nullptr, nullptr, sprite_map[name].GetAddressOf(), nullptr);
}

float operator- (DirectX::XMFLOAT2& lhs, DirectX::XMFLOAT2& rhs)
{
	return sqrtf(powf(lhs.x - rhs.x, 2) + powf(lhs.y - rhs.y, 2));
}