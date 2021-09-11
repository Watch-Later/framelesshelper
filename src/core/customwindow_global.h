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

#pragma once

#include <QtCore/qglobal.h>

#ifndef CUSTOMWINDOW_API
#ifdef CUSTOMWINDOW_STATIC
#define CUSTOMWINDOW_API
#else
#ifdef CUSTOMWINDOW_BUILD_LIBRARY
#define CUSTOMWINDOW_API Q_DECL_EXPORT
#else
#define CUSTOMWINDOW_API Q_DECL_IMPORT
#endif
#endif
#endif

#if defined(Q_OS_WIN) && !defined(Q_OS_WINDOWS)
#define Q_OS_WINDOWS
#endif

#ifndef Q_DISABLE_COPY_MOVE
#define Q_DISABLE_COPY_MOVE(Class) \
    Q_DISABLE_COPY(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
#define qAsConst(i) std::as_const(i)
#endif

#if !defined(Q_OS_WINDOWS) || defined(CUSTOMWINDOW_TEST_UNIX)
#define CUSTOMWINDOW_USE_UNIX_VERSION
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
#define Q_NODISCARD [[nodiscard]]
#else
#define Q_NODISCARD
#endif

#ifndef CUSTOMWINDOW_NAMESPACE
#define CUSTOMWINDOW_NAMESPACE __wwx190::custom_window_ns
#endif

#ifndef CUSTOMWINDOW_BEGIN_NAMESPACE
#define CUSTOMWINDOW_BEGIN_NAMESPACE namespace CUSTOMWINDOW_NAMESPACE {
#endif

#ifndef CUSTOMWINDOW_END_NAMESPACE
#define CUSTOMWINDOW_END_NAMESPACE }
#endif

#ifndef CUSTOMWINDOW_USE_NAMESPACE
#define CUSTOMWINDOW_USE_NAMESPACE using namespace CUSTOMWINDOW_NAMESPACE;
#endif

#ifndef CUSTOMWINDOW_PREPEND_NAMESPACE
#define CUSTOMWINDOW_PREPEND_NAMESPACE(X) ::CUSTOMWINDOW_NAMESPACE::X
#endif

CUSTOMWINDOW_BEGIN_NAMESPACE

Q_NAMESPACE_EXPORT(CUSTOMWINDOW_API)

namespace Constants
{

[[maybe_unused]] constexpr quint32 kDefaultScreenDPI = 96;

[[maybe_unused]] constexpr QString kCustomWindowFrameFlag = QStringLiteral("__CUSTOMWINDOW_CUSTOM_WINDOW_FRAME");
[[maybe_unused]] constexpr QString kResizeBorderThicknessFlag = QStringLiteral("__CUSTOMWINDOW_RESIZE_BORDER_THICKNESS");
[[maybe_unused]] constexpr QString kCaptionHeightFlag = QStringLiteral("__CUSTOMWINDOW_CAPTION_HEIGHT");
[[maybe_unused]] constexpr QString kTitleBarHeightFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_HEIGHT");
[[maybe_unused]] constexpr QString kHitTestVisibleObjectsFlag = QStringLiteral("__CUSTOMWINDOW_HIT_TEST_VISIBLE_OBJECTS");
[[maybe_unused]] constexpr QString kWindowResizableFlag = QStringLiteral("__CUSTOMWINDOW_WINDOW_RESIZABLE");
[[maybe_unused]] constexpr QString kAutoDetectHighContrastFlag = QStringLiteral("__CUSTOMWINDOW_AUTO_DETECT_HIGH_CONTRAST");
[[maybe_unused]] constexpr QString kAutoDetectColorSchemeFlag = QStringLiteral("__CUSTOMWINDOW_AUTO_DETECT_COLOR_SCHEME");
[[maybe_unused]] constexpr QString kFrameBorderVisibleFlag = QStringLiteral("__CUSTOMWINDOW_FRAME_BORDER_VISIBLE");
[[maybe_unused]] constexpr QString kFrameBorderThicknessFlag = QStringLiteral("__CUSTOMWINDOW_FRAME_BORDER_THICKNESS");
[[maybe_unused]] constexpr QString kFrameBorderColorFlag = QStringLiteral("__CUSTOMWINDOW_FRAME_BORDER_COLOR");
[[maybe_unused]] constexpr QString kTitleBarVisibleFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_VISIBLE");
[[maybe_unused]] constexpr QString kTitleBarIconVisibleFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_ICON_VISIBLE");
[[maybe_unused]] constexpr QString kTitleBarIconFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_ICON");
[[maybe_unused]] constexpr QString kTitleBarTextAlignmentFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_TEXT_ALIGNMENT");
[[maybe_unused]] constexpr QString kTitleBarBackgroundColorFlag = QStringLiteral("__CUSTOMWINDOW_TITLE_BAR_BACKGROUND_COLOR");
[[maybe_unused]] constexpr QString kWidgetHandleFlag = QStringLiteral("__CUSTOMWINDOW_WIDGET_HANDLE");
[[maybe_unused]] constexpr QString kWindowHandleFlag = QStringLiteral("__CUSTOMWINDOW_WINDOW_HANDLE");
[[maybe_unused]] constexpr QString kWinIdFlag = QStringLiteral("__CUSTOMWINDOW_WINID");

} // namespace Constants

enum class SystemMetric : int
{
    ResizeBorderThickness = 0,
    CaptionHeight,
    TitleBarHeight,
    FrameBorderThickness
};
Q_ENUM_NS(SystemMetric)

enum class ColorizationArea : int
{
    None = 0,
    StartMenu_TaskBar_ActionCenter,
    TitleBar_WindowBorder,
    All
};
Q_ENUM_NS(ColorizationArea)

enum class DPIAwareness : int
{
    Invalid = -1,
    Unaware,
    System,
    PerMonitor,
    PerMonitorV2
};
Q_ENUM_NS(DPIAwareness)

enum class SystemTheme : int
{
    Light = 0,
    Dark,
    HighContrast
};
Q_ENUM_NS(SystemTheme)

enum class WindowProperty : int
{
    Name = 0,
    Id,
    X,
    Y,
    Position,
    Width,
    Height,
    MinimumWidth,
    MaximumWidth,
    MinimumHeight,
    MaximumHeight,
    Size,
    MinimumSize,
    MaximumSize,
    Geometry,
    State,
    Flags,
    Active,
    Icon,
    Title,
    DotsPerInch,
    DevicePixelRatio,
    FrameBorderThickness,
    FrameBorderColor,
    FrameBorderVisibility,
    Resizable,
    Font,
    Palette,
    TitleBarHeight,
    TitleBarVisibility,
    TitleTextAlignment,
    CaptionHeight,
    ResizeBorderThickness,
    IconVisibility,
    MinimizeButtonAvailability,
    MinimizeButtonVisibility,
    MaximizeButtonAvailability,
    MaximizeButtonVisibility,
    CloseButtonAvailability,
    CloseButtonVisibility,
    MinimizeButtonIcon,
    MaximizeButtonIcon,
    RestoreButtonIcon,
    CloseButtonIcon,
    HitTestVisibleObjects,
    AutoDetectHighContrast,
    AutoDetectColorScheme,
    WidgetHandle,
    WindowHandle,
    StyleSheet,
    CustomWindowFrame,
    Visibility = State,
    DPI = DotsPerInch,
    DPR = DevicePixelRatio
};
Q_ENUM_NS(WindowProperty)

enum class SystemButton : int
{
    Minimize = 0,
    Maximize,
    Restore,
    Close
};
Q_ENUM_NS(SystemButton)

CUSTOMWINDOW_END_NAMESPACE
