﻿/***********************************************************************
程序：D2D1内置特效之高斯模糊
功能：实例化D2D1App子类，利用现成的程序框架，并在子类中实现自己的功能
作者：Ray1024
网址：http://www.cnblogs.com/Ray1024/
***********************************************************************/

// 如果使用Direct2D1.3的API，需要定义下面这个宏
//#define USE_D2D1_3

#include "../Common/D2D1App.h"
#include "../Common/D2D1Func.h"

//--------------------------------------------------------------------
// D2D1EffectGaussianBlur
//--------------------------------------------------------------------
class D2D1EffectGaussianBlur : public D2D1App
{
public:
	// 构造函数
	D2D1EffectGaussianBlur();
	// 析构函数
	~D2D1EffectGaussianBlur();
	// 重写：初始化
	virtual HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);
	// 重写：渲染
	virtual void DrawScene();
	// 重写：鼠标滚轮消息处理
	virtual void OnMouseWheel(UINT nFlags, short zDelta, int x, int y);

private:
	// D2D位图
	ID2D1Bitmap1*						m_pBitmap = nullptr;
	// 测试特效
	ID2D1Effect*                        m_pEffectTest = nullptr;
	// 浮点数据
	float                               m_afData = 0.f;
};


//-------------------------------------------------------------------
// 程序入口WinMain
//-------------------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow) 
{
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    if (SUCCEEDED(::CoInitialize(nullptr)))
    {
        {
			D2D1EffectGaussianBlur app;
            if (SUCCEEDED(app.Initialize(hInstance, nCmdShow)))
            {
                app.Run();
            }
        }
        ::CoUninitialize();
    }

    return 0;
}


//-------------------------------------------------------------------
// D2D1EffectGaussianBlur Implement
//-------------------------------------------------------------------

D2D1EffectGaussianBlur::D2D1EffectGaussianBlur()
{
	m_wndCaption = "D2D1内置特效之高斯模糊（鼠标滚轮改变模糊值）";
}

D2D1EffectGaussianBlur::~D2D1EffectGaussianBlur()
{
	SafeRelease(m_pBitmap);
	SafeRelease(m_pEffectTest);
}

HRESULT D2D1EffectGaussianBlur::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (!SUCCEEDED(D2D1App::Initialize(hInstance, nCmdShow)))
	{
		return S_FALSE;
	}

	HRESULT hr = S_OK;

	// 创建位图
	if (SUCCEEDED(hr))
	{
		hr = LoadBitmapFromFile(
			m_pD2DDeviceContext, m_pWICFactory, L"leopard.png", 0, 0, &m_pBitmap);
	}

	// 创建特效
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_pEffectTest);
	}

	// 设置输入
	if (SUCCEEDED(hr))
	{
		m_pEffectTest->SetInput(0, m_pBitmap);
	}

	return true;
}

void D2D1EffectGaussianBlur::DrawScene()
{
	if (m_pD2DDeviceContext != nullptr)
	{
		// 开始渲染
		m_pD2DDeviceContext->BeginDraw();
		// 清屏
		m_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightBlue));

		// 设置模糊程度，并显示图片
		m_pEffectTest->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_afData*0.233f);
		m_pD2DDeviceContext->DrawImage(m_pEffectTest);

		// 结束渲染
		m_pD2DDeviceContext->EndDraw();
		// 呈现目标
		m_pSwapChain->Present(0, 0);
	}
}

void D2D1EffectGaussianBlur::OnMouseWheel(UINT nFlags, short zDelta, int x, int y)
{
	if (zDelta>0)
	{
		m_afData += 10;
	}
	else
	{
		m_afData -= 10;
		if (m_afData<0)
		{
			m_afData = 0;
		}
	}
}