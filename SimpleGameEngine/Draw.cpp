#include "Draw.h"

Draw::Draw(SimpleGameEngine* pEngine, HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, FVector2D resolution) :
    pEngine(pEngine),
    hInstance(hInstance),
    wndProc(wndProc),
    lpParam(lpParam),
    resolution(resolution),
    hWnd(NULL),
    pD2DFactory(NULL),
    pRenderTarget(NULL)
{
    registerWindow();
    createD2D();
    createWICImagingFactory();
    createIDWriteFactory();
}

void Draw::registerWindow() {
    WNDCLASSEX wndClassEx = { sizeof(WNDCLASSEX) };
     
    wndClassEx.style         = CS_HREDRAW | CS_VREDRAW;
    wndClassEx.lpfnWndProc   = wndProc;
    wndClassEx.cbClsExtra    = 0;
    wndClassEx.cbWndExtra    = sizeof(LONG_PTR);
    wndClassEx.hInstance     = this->hInstance;
    wndClassEx.hbrBackground = NULL;
    wndClassEx.lpszClassName = NULL;
    wndClassEx.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
    wndClassEx.lpszClassName = L"My App";

    RegisterClassEx(&wndClassEx);
}

void Draw::createD2D() {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pD2DFactory);

    hWnd = CreateWindow(
        L"My App",
        L"My Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (int)resolution.X,
        (int)resolution.Y,
        NULL,
        NULL,
        hInstance,
        lpParam
    );

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    RECT rect;
    GetClientRect(hWnd, &rect);

    pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)),
        D2D1::HwndRenderTargetProperties(this->hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
        &pRenderTarget
    );
}

void Draw::createWICImagingFactory() {
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)&pWICImagingFactory
    );
}

void Draw::createIDWriteFactory() {
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(pWriteFactory),
        reinterpret_cast<IUnknown**>(&pWriteFactory)
    );
}

void Draw::beginPaint() {
    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void Draw::endPaint() {
    pRenderTarget->EndDraw();
    clearBrushes();
}

ID2D1SolidColorBrush* Draw::createBrush(int r, int g, int b, float a) {
    ID2D1SolidColorBrush* brush;

    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, a), &brush);
    vBrushes.push_back(brush);

    return brush;
}

void Draw::clearBrushes() {
    for (auto brush : vBrushes) {
        brush->Release();
    }

    vBrushes.clear();
}

HWND Draw::getHwnd() {
    return hWnd;
}

void Draw::drawLine(float x1, float y1, float x2, float y2, ID2D1SolidColorBrush* pBrush, float strokeWidth) {
    pRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), pBrush, strokeWidth);
}

void Draw::drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush, float strokeWidth) {
    pRenderTarget->DrawLine(D2D1::Point2F(pt1.X, pt1.Y), D2D1::Point2F(pt2.X, pt2.Y), pBrush, strokeWidth);
}

void Draw::drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    drawLine(pt1, pt2, pBrush);
    drawLine(pt2, pt3, pBrush);
    drawLine(pt3, pt1, pBrush);
}

void Draw::drawFilledTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush) {
    ID2D1PathGeometry* pGeometry;
    ID2D1GeometrySink* pSink;

    pD2DFactory->CreatePathGeometry(&pGeometry);

    if (!pGeometry)
        return;

    pGeometry->Open(&pSink);

    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
    pSink->BeginFigure(D2D1::Point2F(pt1.X, pt1.Y), D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(D2D1::Point2F(pt2.X, pt2.Y));
    pSink->AddLine(D2D1::Point2F(pt3.X, pt3.Y));
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

    pSink->Close();
    pSink->Release();

    pRenderTarget->DrawGeometry(pGeometry, pBrush);
    pRenderTarget->FillGeometry(pGeometry, pBrush);

    pGeometry->Release();
}

void Draw::drawFilledRectangle(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush) {
    ID2D1PathGeometry* pGeometry;
    ID2D1GeometrySink* pSink;

    pD2DFactory->CreatePathGeometry(&pGeometry);

    if (!pGeometry)
        return;

    pGeometry->Open(&pSink);

    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
    pSink->BeginFigure(D2D1::Point2F(pt1.X, pt1.Y), D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(D2D1::Point2F(pt2.X, pt1.Y));
    pSink->AddLine(D2D1::Point2F(pt2.X, pt2.Y));
    pSink->AddLine(D2D1::Point2F(pt1.X, pt2.Y));

    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

    pSink->Close();
    pSink->Release();

    pRenderTarget->DrawGeometry(pGeometry, pBrush);
    pRenderTarget->FillGeometry(pGeometry, pBrush);

    pGeometry->Release();
}

ID2D1Bitmap* Draw::loadBitmapFromFile(std::wstring path) {
    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICStream* pStream = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICBitmapScaler* pScaler = NULL;
    ID2D1Bitmap* ppBitmap = NULL;

    HRESULT hr = pWICImagingFactory->CreateDecoderFromFilename(
        path.c_str(),
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );

    if (SUCCEEDED(hr)) {
        hr = pDecoder->GetFrame(0, &pSource);
    }

    if (SUCCEEDED(hr)) {
        hr = pWICImagingFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr)) {
        hr = pConverter->Initialize(
            pSource,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut
        );
    }

    if (SUCCEEDED(hr)) {
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            &ppBitmap
        );
    }

    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

    return ppBitmap;
}

void Draw::drawBitmap(ID2D1Bitmap* pBitmap, FVector2D pt1) {
    
    auto bitmapSize = pBitmap->GetSize();
    
    pRenderTarget->DrawBitmap(
        pBitmap,
        D2D1::RectF(
            pt1.X,
            pt1.Y,
            pt1.X + bitmapSize.width,
            pt1.Y + bitmapSize.height
        )
    );
}

void Draw::drawBitmapPart(ID2D1Bitmap* pBitmap, FVector2D pt1, FVector2D size, D2D1_RECT_F rect) {
    pRenderTarget->DrawBitmap(
        pBitmap,
        D2D1::RectF(
            pt1.X,
            pt1.Y,
            pt1.X + size.X,
            pt1.Y + size.Y
        ),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        rect
    );
}


IDWriteTextFormat* Draw::createTextFormat(std::wstring fontName, float fontSize) {

    IDWriteTextFormat* pTextFormat;

    HRESULT hr = pWriteFactory->CreateTextFormat(
        fontName.c_str(),
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"",
        &pTextFormat
    );

    return SUCCEEDED(hr) ? pTextFormat : nullptr;
}

void Draw::drawText(std::wstring text, FVector2D pt1, IDWriteTextFormat* pTextFormat, ID2D1SolidColorBrush* pBrush) {
    pRenderTarget->DrawTextW(
        text.c_str(),
        text.length(),
        pTextFormat,
        D2D1::RectF(pt1.X, pt1.Y, resolution.X, resolution.Y),
        pBrush
    );
}

FVector2D Draw::getScreenSize() {
    return resolution;
}

FVector2D Draw::getTextSize(std::wstring text, FVector2D pt1, IDWriteTextFormat* textFormat) {
    IDWriteTextLayout* textLayout;
    DWRITE_TEXT_METRICS  metrics;
    FVector2D ret(0, 0);

    HRESULT hr = pWriteFactory->CreateTextLayout(
        text.c_str(),
        text.length(),
        textFormat,
        resolution.X, resolution.Y,
        &textLayout
    );

    if (SUCCEEDED(hr)) {
        hr = textLayout->GetMetrics(&metrics);
    }

    if (SUCCEEDED(hr)) {
        ret.X = metrics.width;
        ret.Y = metrics.height;
    }

    return ret;
}