/*
 * MIT License
 *
 * Copyright (C) 2021 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utils.h"
#include "core_windows.h"
#include <QtCore/qdebug.h>
#include <QtCore/qsettings.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QtCore/qoperatingsystemversion.h>
#else
#include <QtCore/qsysinfo.h>
#endif
#include <QtGui/qguiapplication.h>
#include <QtGui/qpa/qplatformwindow.h>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QtGui/qpa/qplatformnativeinterface.h>
#else
#include <QtGui/qpa/qplatformwindow_p.h>
#endif

Q_DECLARE_METATYPE(QMargins)

CUSTOMWINDOW_BEGIN_NAMESPACE

[[nodiscard]] static inline bool isWin10RS1OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion(QOperatingSystemVersion::Windows, 10, 0, 14393));
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS10);
#endif
    return result;
}

[[nodiscard]] static inline bool isWin1019H1OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion(QOperatingSystemVersion::Windows, 10, 0, 18362));
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS10);
#endif
    return result;
}

[[nodiscard]] static inline quint32 __GetSystemMetricsForDpi(const int nIndex, const quint32 dpi)
{
    Q_ASSERT(dpi != 0);
    if (dpi == 0) {
        return 0;
    }
    static bool tried = false;
    using GetSystemMetricsForDpiSig = decltype(&::GetSystemMetricsForDpi);
    static GetSystemMetricsForDpiSig GetSystemMetricsForDpiFunc = nullptr;
    if (isWin10RS1OrGreater()) {
        if (!GetSystemMetricsForDpiFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"User32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    GetSystemMetricsForDpiFunc = reinterpret_cast<GetSystemMetricsForDpiSig>(GetProcAddress(dll, "GetSystemMetricsForDpi"));
                    if (!GetSystemMetricsForDpiFunc) {
                        qWarning() << Utils::getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << Utils::getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
    }
    if (GetSystemMetricsForDpiFunc) {
        const int result = GetSystemMetricsForDpiFunc(nIndex, dpi);
        if (result > 0) {
            return result;
        } else {
            qWarning() << Utils::getSystemErrorMessage(QStringLiteral("GetSystemMetricsForDpi"));
            return 0;
        }
    } else {
        const auto value = static_cast<qreal>(GetSystemMetrics(nIndex));
        if (value <= 0.0) {
            qWarning() << Utils::getSystemErrorMessage(QStringLiteral("GetSystemMetrics"));
            return 0;
        }
        const DPIAwareness dpiAwareness = Utils::getDPIAwarenessForWindow(reinterpret_cast<WId>(nullptr));
        if (dpiAwareness == DPIAwareness::Invalid) {
            qWarning() << "Failed to retrieve the DPI awareness for the current process.";
            return 0;
        }
        const qreal dpr = (static_cast<qreal>(dpi) / static_cast<qreal>(USER_DEFAULT_SCREEN_DPI));
        if (dpiAwareness == DPIAwareness::Unaware) {
            return qRound(value * dpr);
        } else {
            const quint32 currentDPI = Utils::getDPIForWindow(reinterpret_cast<WId>(nullptr));
            if (currentDPI == 0) {
                qWarning() << "Failed to retrieve the DPI for the current process.";
                return 0;
            }
            const qreal currentDPR = (static_cast<qreal>(currentDPI) / static_cast<qreal>(USER_DEFAULT_SCREEN_DPI));
            if (currentDPR == dpr) {
                return qRound(value);
            } else {
                return qRound((value / currentDPR) * dpr);
            }
        }
    }
}

bool Utils::isWin8OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8);
#endif
    return result;
}

bool Utils::isWin8Point1OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8_1);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8_1);
#endif
    return result;
}

bool Utils::isWin10OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS10);
#endif
    return result;
}

bool Utils::isCompositionEnabled()
{
    // DWM composition is always enabled and can't be disabled since Windows 8.
    if (isWin8OrGreater()) {
        return true;
    }
    BOOL enabled = FALSE;
    const HRESULT hr = DwmIsCompositionEnabled(&enabled);
    if (SUCCEEDED(hr)) {
        return (enabled != FALSE);
    } else {
        qWarning() << getSystemErrorMessage(QStringLiteral("DwmIsCompositionEnabled"), hr);
        const QSettings registry(QString::fromUtf8(Constants::kDwmRegistryKey), QSettings::NativeFormat);
        bool ok = false;
        const DWORD value = registry.value(QStringLiteral("Composition"), 0).toUInt(&ok);
        return (ok && (value != 0));
    }
}

quint32 Utils::getSystemMetric(const WId winId, const SystemMetric metric, const bool dpiScale)
{
    Q_ASSERT(winId);
    if (!winId) {
        return 0;
    }
    const quint32 dpi = (dpiScale ? getDPIForWindow(winId) : USER_DEFAULT_SCREEN_DPI);
    const qreal dpr = (dpiScale ? (static_cast<qreal>(dpi) / static_cast<qreal>(USER_DEFAULT_SCREEN_DPI)) : 1.0);
    switch (metric) {
    case SystemMetric::ResizeBorderThickness: {
        const quint32 result = __GetSystemMetricsForDpi(SM_CXSIZEFRAME, dpi) + __GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
        if (result > 0) {
            return result;
        } else {
            // The padded border will disappear if DWM composition is disabled.
            const quint32 defaultResizeBorderThickness = (isCompositionEnabled() ?
                                                          Constants::kDefaultResizeBorderThicknessAero :
                                                          Constants::kDefaultResizeBorderThicknessClassic);
            if (dpiScale) {
                return qRound(static_cast<qreal>(defaultResizeBorderThickness) * dpr);
            } else {
                return defaultResizeBorderThickness;
            }
        }
    }
    case SystemMetric::CaptionHeight: {
        const quint32 result = __GetSystemMetricsForDpi(SM_CYCAPTION, dpi);
        if (result > 0) {
            return result;
        } else {
            if (dpiScale) {
                return qRound(static_cast<qreal>(Constants::kDefaultCaptionHeight) * dpr);
            } else {
                return Constants::kDefaultCaptionHeight;
            }
        }
    }
    case SystemMetric::TitleBarHeight: {
        const quint32 captionHeight = getSystemMetric(winId, SystemMetric::CaptionHeight, dpiScale);
        const quint32 resizeBorderThickness = getSystemMetric(winId, SystemMetric::ResizeBorderThickness, dpiScale);
        return ((isMaximized(winId) || isFullScreened(winId)) ? captionHeight : (captionHeight + resizeBorderThickness));
    }
    }
    return 0;
}

bool Utils::triggerFrameChange(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    constexpr UINT flags = (SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    if (SetWindowPos(reinterpret_cast<HWND>(winId), nullptr, 0, 0, 0, 0, flags) == FALSE) {
        qWarning() << getSystemErrorMessage(QStringLiteral("SetWindowPos"));
        return false;
    }
    return true;
}

bool Utils::updateFrameMargins(const WId winId, const bool reset)
{
    // DwmExtendFrameIntoClientArea() will always fail if DWM composition is disabled.
    // No need to try in this case.
    if (!isCompositionEnabled()) {
        return false;
    }
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    const int margin = (reset ? 0 : getWindowVisibleFrameBorderThickness(winId));
    const MARGINS margins = {margin, margin, margin, margin};
    const HRESULT hr = DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(winId), &margins);
    if (FAILED(hr)) {
        qWarning() << getSystemErrorMessage(QStringLiteral("DwmExtendFrameIntoClientArea"), hr);
        return false;
    }
    return true;
}

bool Utils::updateQtInternalFrameMargins(QWindow *window, const bool enable)
{
    Q_ASSERT(window);
    if (!window) {
        return false;
    }
    const WId winId = window->winId();
    const bool useCustomFrameMargin = (enable && !isMaximized(winId) && !isFullScreened(winId));
    const int resizeBorderThickness = useCustomFrameMargin ? Utils::getSystemMetric(winId, SystemMetric::ResizeBorderThickness, true) : 0;
    const int titleBarHeight = enable ? Utils::getSystemMetric(winId, SystemMetric::TitleBarHeight, true) : 0;
    const QMargins margins = {-resizeBorderThickness, -titleBarHeight, -resizeBorderThickness, -resizeBorderThickness}; // left, top, right, bottom
    const QVariant marginsVar = QVariant::fromValue(margins);
    window->setProperty("_q_windowsCustomMargins", marginsVar);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPlatformWindow *platformWindow = window->handle();
    if (platformWindow) {
        QGuiApplication::platformNativeInterface()->setWindowProperty(platformWindow, QStringLiteral("WindowsCustomMargins"), marginsVar);
    } else {
        qWarning() << "Failed to retrieve the platform window.";
    }
#else
    auto *platformWindow = dynamic_cast<QNativeInterface::Private::QWindowsWindow *>(
        window->handle());
    if (platformWindow) {
        platformWindow->setCustomMargins(margins);
    } else {
        qWarning() << "Failed to retrieve the platform window.";
    }
#endif
    return true;
}

QString Utils::getSystemErrorMessage(const QString &function, const HRESULT hr)
{
    Q_ASSERT(!function.isEmpty());
    if (function.isEmpty()) {
        return {};
    }
    if (SUCCEEDED(hr)) {
        return QStringLiteral("Operation succeeded.");
    }
    const DWORD dwError = HRESULT_CODE(hr);
    LPWSTR buf = nullptr;
    if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 0, nullptr) == 0) {
        return QStringLiteral("Failed to retrieve the error message from system.");
    }
    const QString message = QStringLiteral("%1 failed with error %2: %3.")
                             .arg(function, QString::number(dwError), QString::fromWCharArray(buf));
    LocalFree(buf);
    return message;
}

QString Utils::getSystemErrorMessage(const QString &function)
{
    Q_ASSERT(!function.isEmpty());
    if (function.isEmpty()) {
        return {};
    }
    const HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
    if (SUCCEEDED(hr)) {
        return QStringLiteral("Operation succeeded.");
    }
    return getSystemErrorMessage(function, hr);
}

QColor Utils::getColorizationColor()
{
    COLORREF color = RGB(0, 0, 0);
    BOOL opaque = FALSE;
    const HRESULT hr = DwmGetColorizationColor(&color, &opaque);
    if (FAILED(hr)) {
        qWarning() << getSystemErrorMessage(QStringLiteral("DwmGetColorizationColor"), hr);
        const QSettings registry(QString::fromUtf8(Constants::kDwmRegistryKey), QSettings::NativeFormat);
        bool ok = false;
        color = registry.value(QStringLiteral("ColorizationColor"), 0).toUInt(&ok);
        if (!ok || (color == 0)) {
            color = RGB(128, 128, 128); // Dark gray
        }
    }
    return QColor::fromRgba(color);
}

quint32 Utils::getWindowVisibleFrameBorderThickness(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return 1;
    }
    if (!isWin10OrGreater()) {
        return 1;
    }
    UINT value = 0;
    const HRESULT hr = DwmGetWindowAttribute(reinterpret_cast<HWND>(winId), Constants::_DWMWA_VISIBLE_FRAME_BORDER_THICKNESS, &value, sizeof(value));
    if (SUCCEEDED(hr)) {
        return value;
    } else {
        // We just eat this error because this enum value was introduced in a very
        // late Windows 10 version, so querying it's value will always result in
        // a "parameter error" (code: 87) on systems before that value was introduced.
    }
    return 1;
}

bool Utils::shouldAppsUseDarkMode()
{
    if (!isWin10RS1OrGreater()) {
        return false;
    }
    const auto resultFromRegistry = []() -> bool {
        const QSettings registry(QString::fromUtf8(Constants::kPersonalizeRegistryKey), QSettings::NativeFormat);
        bool ok = false;
        const DWORD value = registry.value(QStringLiteral("AppsUseLightTheme"), 0).toUInt(&ok);
        return (ok && (value == 0));
    };
    // Starting from Windows 10 19H1, ShouldAppsUseDarkMode() always return "TRUE"
    // (actually, a random non-zero number at runtime), so we can't use it due to
    // this unreliability. In this case, we just simply read the user's setting from
    // the registry instead, it's not elegant but at least it works well.
    if (isWin1019H1OrGreater()) {
        return resultFromRegistry();
    } else {
        static bool tried = false;
        using ShouldAppsUseDarkModeSig = BOOL(WINAPI *)();
        static ShouldAppsUseDarkModeSig ShouldAppsUseDarkModeFunc = nullptr;
        if (!ShouldAppsUseDarkModeFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"UxTheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    ShouldAppsUseDarkModeFunc = reinterpret_cast<ShouldAppsUseDarkModeSig>(GetProcAddress(dll, MAKEINTRESOURCEA(132)));
                    if (!ShouldAppsUseDarkModeFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
        if (ShouldAppsUseDarkModeFunc) {
            return (ShouldAppsUseDarkModeFunc() != FALSE);
        } else {
            return resultFromRegistry();
        }
    }
}

ColorizationArea Utils::getColorizationArea()
{
    if (!isWin10OrGreater()) {
        return ColorizationArea::None;
    }
    const QString keyName = QStringLiteral("ColorPrevalence");
    const QSettings themeRegistry(QString::fromUtf8(Constants::kPersonalizeRegistryKey), QSettings::NativeFormat);
    const DWORD themeValue = themeRegistry.value(keyName, 0).toUInt();
    const QSettings dwmRegistry(QString::fromUtf8(Constants::kDwmRegistryKey), QSettings::NativeFormat);
    const DWORD dwmValue = dwmRegistry.value(keyName, 0).toUInt();
    const bool theme = (themeValue != 0);
    const bool dwm = (dwmValue != 0);
    if (theme && dwm) {
        return ColorizationArea::All;
    } else if (theme) {
        return ColorizationArea::StartMenu_TaskBar_ActionCenter;
    } else if (dwm) {
        return ColorizationArea::TitleBar_WindowBorder;
    }
    return ColorizationArea::None;
}

#if 0
bool Utils::isThemeChanged(const void *data)
{
    Q_ASSERT(data);
    if (!data) {
        return false;
    }
    const auto msg = static_cast<const MSG *>(data);
    if (msg->message == WM_THEMECHANGED) {
        return true;
    } else if (msg->message == WM_DWMCOLORIZATIONCOLORCHANGED) {
        return true;
    } else if (msg->message == WM_SETTINGCHANGE) {
        if ((msg->wParam == 0) && (_wcsicmp(reinterpret_cast<LPCWSTR>(msg->lParam), L"ImmersiveColorSet") == 0)) {
            return true;
        }
    }
    return false;
}

bool Utils::isSystemMenuRequested(const void *data, QPointF *pos)
{
    Q_ASSERT(data);
    if (!data) {
        return false;
    }
    bool result = false;
    const auto msg = static_cast<const MSG *>(data);
    if (msg->message == WM_NCRBUTTONUP) {
        if (msg->wParam == HTCAPTION) {
            result = true;
        }
    } else if (msg->message == WM_SYSCOMMAND) {
        const WPARAM filteredWParam = (msg->wParam & 0xFFF0);
        if ((filteredWParam == SC_KEYMENU) && (msg->lParam == VK_SPACE)) {
            result = true;
        }
    } else if (msg->message == WM_CONTEXTMENU) {
        //
    }
    if (result) {
        if (pos) {
            *pos = extractMousePositionFromLParam(msg->lParam);
        }
    }
    return result;
}

bool Utils::showSystemMenu(const WId winId, const QPointF &pos)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    const auto hWnd = reinterpret_cast<HWND>(winId);
    const HMENU menu = GetSystemMenu(hWnd, FALSE);
    if (!menu) {
        qWarning() << getSystemErrorMessage(QStringLiteral("GetSystemMenu"));
        return false;
    }
    // Update the options based on window state.
    MENUITEMINFOW mii;
    SecureZeroMemory(&mii, sizeof(mii));
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    mii.fType = MFT_STRING;
    const auto setState = [&mii, menu](const UINT item, const bool enabled) -> bool {
        mii.fState = (enabled ? MF_ENABLED : MF_DISABLED);
        if (SetMenuItemInfoW(menu, item, FALSE, &mii) == FALSE) {
            qWarning() << getSystemErrorMessage(QStringLiteral("SetMenuItemInfoW"));
            return false;
        }
        return true;
    };
    const bool max = isMaximized(winId);
    if (!setState(SC_RESTORE, max)) {
        return false;
    }
    if (!setState(SC_MOVE, !max)) {
        return false;
    }
    if (!setState(SC_SIZE, !max)) {
        return false;
    }
    if (!setState(SC_MINIMIZE, true)) {
        return false;
    }
    if (!setState(SC_MAXIMIZE, !max)) {
        return false;
    }
    if (!setState(SC_CLOSE, true)) {
        return false;
    }
    if (SetMenuDefaultItem(menu, UINT_MAX, FALSE) == FALSE) {
        qWarning() << getSystemErrorMessage(QStringLiteral("SetMenuDefaultItem"));
        return false;
    }
    const QPoint roundedPos = pos.toPoint();
    const auto ret = TrackPopupMenu(menu, TPM_RETURNCMD, roundedPos.x(), roundedPos.y(), 0, hWnd, nullptr);
    if (ret != 0) {
        if (PostMessageW(hWnd, WM_SYSCOMMAND, ret, 0) == FALSE) {
            qWarning() << getSystemErrorMessage(QStringLiteral("PostMessageW"));
            return false;
        }
    }
    return true;
}
#endif

quint32 Utils::getDPIForWindow(const WId winId)
{
    static bool tried = false;
    using GetDpiForWindowSig = decltype(&::GetDpiForWindow);
    static GetDpiForWindowSig GetDpiForWindowFunc = nullptr;
    using GetSystemDpiForProcessSig = decltype(&::GetSystemDpiForProcess);
    static GetSystemDpiForProcessSig GetSystemDpiForProcessFunc = nullptr;
    using GetDpiForSystemSig = decltype(&::GetDpiForSystem);
    static GetDpiForSystemSig GetDpiForSystemFunc = nullptr;
    using GetDpiForMonitorSig = decltype(&::GetDpiForMonitor);
    static GetDpiForMonitorSig GetDpiForMonitorFunc = nullptr;
    if (isWin10RS1OrGreater()) {
        if (!GetDpiForWindowFunc || !GetSystemDpiForProcessFunc || !GetDpiForSystemFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"User32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    GetDpiForWindowFunc = reinterpret_cast<GetDpiForWindowSig>(GetProcAddress(dll, "GetDpiForWindow"));
                    if (!GetDpiForWindowFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                    GetSystemDpiForProcessFunc = reinterpret_cast<GetSystemDpiForProcessSig>(GetProcAddress(dll, "GetSystemDpiForProcess"));
                    if (!GetSystemDpiForProcessFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                    GetDpiForSystemFunc = reinterpret_cast<GetDpiForSystemSig>(GetProcAddress(dll, "GetDpiForSystem"));
                    if (!GetDpiForSystemFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
    } else if (isWin8Point1OrGreater()) {
        if (!GetDpiForMonitorFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"SHCore.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    GetDpiForMonitorFunc = reinterpret_cast<GetDpiForMonitorSig>(GetProcAddress(dll, "GetDpiForMonitor"));
                    if (!GetDpiForMonitorFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
    }
    if (winId && GetDpiForWindowFunc) {
        const quint32 result = GetDpiForWindowFunc(reinterpret_cast<HWND>(winId));
        if (result > 0) {
            return result;
        } else {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetDpiForWindow"));
            return USER_DEFAULT_SCREEN_DPI;
        }
    } else if (winId && GetDpiForMonitorFunc) {
        const HMONITOR screen = MonitorFromWindow(reinterpret_cast<HWND>(winId), MONITOR_DEFAULTTONEAREST);
        if (!screen) {
            qWarning() << getSystemErrorMessage(QStringLiteral("MonitorFromWindow"));
            return USER_DEFAULT_SCREEN_DPI;
        }
        UINT dpiX = 0;
        UINT dpiY = 0;
        const HRESULT hr = GetDpiForMonitorFunc(screen, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
        if (FAILED(hr)) {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetDpiForMonitor"), hr);
            return USER_DEFAULT_SCREEN_DPI;
        }
        return qRound(static_cast<qreal>(dpiX + dpiY) / 2.0);
    } else if (GetSystemDpiForProcessFunc || GetDpiForSystemFunc) {
        if (GetSystemDpiForProcessFunc) {
            const HANDLE hProcess = GetCurrentProcess();
            if (!hProcess) {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetCurrentProcess"));
                return USER_DEFAULT_SCREEN_DPI;
            }
            const quint32 result = GetSystemDpiForProcessFunc(hProcess);
            if (result > 0) {
                return result;
            } else {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetSystemDpiForProcess"));
                return USER_DEFAULT_SCREEN_DPI;
            }
        } else {
            const quint32 result = GetDpiForSystemFunc();
            if (result > 0) {
                return result;
            } else {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetDpiForSystem"));
                return USER_DEFAULT_SCREEN_DPI;
            }
        }
    } else if (GetDpiForMonitorFunc) {
        POINT pos = {0, 0};
        if (GetCursorPos(&pos) == FALSE) {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetCursorPos"));
            return USER_DEFAULT_SCREEN_DPI;
        }
        const HMONITOR screen = MonitorFromPoint(pos, MONITOR_DEFAULTTONEAREST);
        if (!screen) {
            qWarning() << getSystemErrorMessage(QStringLiteral("MonitorFromPoint"));
            return USER_DEFAULT_SCREEN_DPI;
        }
        UINT dpiX = 0;
        UINT dpiY = 0;
        const HRESULT hr = GetDpiForMonitorFunc(screen, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
        if (FAILED(hr)) {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetDpiForMonitor"), hr);
            return USER_DEFAULT_SCREEN_DPI;
        }
        return qRound(static_cast<qreal>(dpiX + dpiY) / 2.0);
    } else {
        const HDC hDC = GetDC(nullptr);
        if (hDC) {
            const int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
            if (dpiX <= 0) {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetDeviceCaps"));
                // release dc
                return USER_DEFAULT_SCREEN_DPI;
            }
            const int dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
            if (dpiY <= 0) {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetDeviceCaps"));
                // release dc
                return USER_DEFAULT_SCREEN_DPI;
            }
            if (ReleaseDC(nullptr, hDC) == 0) {
                qWarning() << getSystemErrorMessage(QStringLiteral("ReleaseDC"));
                // The DPI data is still valid, no need to return early.
            }
            return qRound(static_cast<qreal>(dpiX + dpiY) / 2.0);
        } else {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetDC"));
            return USER_DEFAULT_SCREEN_DPI;
        }
    }
}

bool Utils::isMinimized(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    return IsMinimized(reinterpret_cast<HWND>(winId));
}

bool Utils::isMaximized(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    return IsMaximized(reinterpret_cast<HWND>(winId));
}

bool Utils::isFullScreened(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    const auto hWnd = reinterpret_cast<HWND>(winId);
    RECT windowGeometry = {0, 0, 0, 0};
    if (GetWindowRect(hWnd, &windowGeometry) == FALSE) {
        qWarning() << getSystemErrorMessage(QStringLiteral("GetWindowRect"));
        return false;
    }
    const HMONITOR screen = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
    if (!screen) {
        qWarning() << getSystemErrorMessage(QStringLiteral("MonitorFromWindow"));
        return false;
    }
    MONITORINFO screenInfo;
    SecureZeroMemory(&screenInfo, sizeof(screenInfo));
    screenInfo.cbSize = sizeof(screenInfo);
    if (GetMonitorInfoW(screen, &screenInfo) == FALSE) {
        qWarning() << getSystemErrorMessage(QStringLiteral("GetMonitorInfoW"));
        return false;
    }
    const RECT screenGeometry = screenInfo.rcMonitor;
    return ((windowGeometry.top == screenGeometry.top)
            && (windowGeometry.bottom == screenGeometry.bottom)
            && (windowGeometry.left == screenGeometry.left)
            && (windowGeometry.right == screenGeometry.right));
}

bool Utils::isWindowNoState(const WId winId)
{
    Q_ASSERT(winId);
    if (!winId) {
        return false;
    }
    WINDOWPLACEMENT wp;
    SecureZeroMemory(&wp, sizeof(wp));
    wp.length = sizeof(wp);
    if (GetWindowPlacement(reinterpret_cast<HWND>(winId), &wp) == FALSE) {
        qWarning() << getSystemErrorMessage(QStringLiteral("GetWindowPlacement"));
        return false;
    }
    return (wp.showCmd == SW_NORMAL);
}

DPIAwareness Utils::getDPIAwarenessForWindow(const WId winId)
{
    static bool tried = false;
    using GetWindowDpiAwarenessContextSig = decltype(&::GetWindowDpiAwarenessContext);
    static GetWindowDpiAwarenessContextSig GetWindowDpiAwarenessContextFunc = nullptr;
    using GetThreadDpiAwarenessContextSig = decltype(&::GetThreadDpiAwarenessContext);
    static GetThreadDpiAwarenessContextSig GetThreadDpiAwarenessContextFunc = nullptr;
    using GetAwarenessFromDpiAwarenessContextSig = decltype(&::GetAwarenessFromDpiAwarenessContext);
    static GetAwarenessFromDpiAwarenessContextSig GetAwarenessFromDpiAwarenessContextFunc = nullptr;
    using GetProcessDpiAwarenessSig = decltype(&::GetProcessDpiAwareness);
    static GetProcessDpiAwarenessSig GetProcessDpiAwarenessFunc = nullptr;
    if (isWin10RS1OrGreater()) {
        if (!GetWindowDpiAwarenessContextFunc || !GetThreadDpiAwarenessContextFunc || !GetAwarenessFromDpiAwarenessContextFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"User32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    GetWindowDpiAwarenessContextFunc = reinterpret_cast<GetWindowDpiAwarenessContextSig>(GetProcAddress(dll, "GetWindowDpiAwarenessContext"));
                    if (!GetWindowDpiAwarenessContextFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                    GetThreadDpiAwarenessContextFunc = reinterpret_cast<GetThreadDpiAwarenessContextSig>(GetProcAddress(dll, "GetThreadDpiAwarenessContext"));
                    if (!GetThreadDpiAwarenessContextFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                    GetAwarenessFromDpiAwarenessContextFunc = reinterpret_cast<GetAwarenessFromDpiAwarenessContextSig>(GetProcAddress(dll, "GetAwarenessFromDpiAwarenessContext"));
                    if (!GetAwarenessFromDpiAwarenessContextFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
    } else if (isWin8Point1OrGreater()) {
        if (!GetProcessDpiAwarenessFunc) {
            if (!tried) {
                tried = true;
                const HMODULE dll = LoadLibraryExW(L"SHCore.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
                if (dll) {
                    GetProcessDpiAwarenessFunc = reinterpret_cast<GetProcessDpiAwarenessSig>(GetProcAddress(dll, "GetProcessDpiAwareness"));
                    if (!GetProcessDpiAwarenessFunc) {
                        qWarning() << getSystemErrorMessage(QStringLiteral("GetProcAddress"));
                    }
                } else {
                    qWarning() << getSystemErrorMessage(QStringLiteral("LoadLibraryExW"));
                }
            }
        }
    }
    if (winId && GetWindowDpiAwarenessContextFunc && GetAwarenessFromDpiAwarenessContextFunc) {
        const auto context = GetWindowDpiAwarenessContextFunc(reinterpret_cast<HWND>(winId));
        if (context) {
            const auto awareness = GetAwarenessFromDpiAwarenessContextFunc(context);
            if (awareness == DPI_AWARENESS_INVALID) {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetAwarenessFromDpiAwarenessContext"));
                return DPIAwareness::Invalid;
            } else {
                return static_cast<DPIAwareness>(awareness);
            }
        } else {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetWindowDpiAwarenessContext"));
            return DPIAwareness::Invalid;
        }
    } else if (GetThreadDpiAwarenessContextFunc && GetAwarenessFromDpiAwarenessContextFunc) {
        const auto context = GetThreadDpiAwarenessContextFunc();
        if (context) {
            const auto awareness = GetAwarenessFromDpiAwarenessContextFunc(context);
            if (awareness == DPI_AWARENESS_INVALID) {
                qWarning() << getSystemErrorMessage(QStringLiteral("GetAwarenessFromDpiAwarenessContext"));
                return DPIAwareness::Invalid;
            } else {
                return static_cast<DPIAwareness>(awareness);
            }
        } else {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetThreadDpiAwarenessContext"));
            return DPIAwareness::Invalid;
        }
    } else if (GetProcessDpiAwarenessFunc) {
        PROCESS_DPI_AWARENESS awareness = PROCESS_DPI_UNAWARE;
        const HRESULT hr = GetProcessDpiAwarenessFunc(nullptr, &awareness);
        if (SUCCEEDED(hr)) {
            return static_cast<DPIAwareness>(awareness);
        } else {
            qWarning() << getSystemErrorMessage(QStringLiteral("GetProcessDpiAwareness"), hr);
            return DPIAwareness::Invalid;
        }
    } else {
        return ((IsProcessDPIAware() == FALSE) ? DPIAwareness::Unaware : DPIAwareness::System);
    }
}

CUSTOMWINDOW_END_NAMESPACE
