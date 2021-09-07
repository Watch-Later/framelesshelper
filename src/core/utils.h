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

#include "customwindow_global.h"
#include <QtCore/qpoint.h>
#include <QtGui/qwindowdefs.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

enum class SystemMetric : int
{
    ResizeBorderThickness = 0,
    CaptionHeight,
    TitleBarHeight
};

enum class ColorizationArea : int
{
    None = 0,
    StartMenu_TaskBar_ActionCenter,
    TitleBar_WindowBorder,
    All
};

enum class DPIAwareness : int
{
    Invalid = -1,
    Unaware,
    System,
    PerMonitor,
    PerMonitorV2
};

enum class SystemTheme : int
{
    Light = 0,
    Dark,
    HighContrast
};

namespace Utils
{

[[nodiscard]] CUSTOMWINDOW_API bool isWin8OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isWin8Point1OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isWin10OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isWin11OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isCompositionEnabled();
[[nodiscard]] CUSTOMWINDOW_API quint32 getSystemMetric(const WId winId, const SystemMetric metric, const bool dpiScale);
[[nodiscard]] CUSTOMWINDOW_API bool triggerFrameChange(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API bool updateFrameMargins(const WId winId, const bool reset);
[[nodiscard]] CUSTOMWINDOW_API bool updateQtInternalFrameMargins(QWindow *window, const bool enable);
[[nodiscard]] CUSTOMWINDOW_API QString getSystemErrorMessage(const QString &function, const HRESULT hr);
[[nodiscard]] CUSTOMWINDOW_API QString getSystemErrorMessage(const QString &function);
[[nodiscard]] CUSTOMWINDOW_API quint32 getDPIForWindow(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API bool isMinimized(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API bool isMaximized(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API bool isFullScreened(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API bool isWindowNoState(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API QColor getColorizationColor();
[[nodiscard]] CUSTOMWINDOW_API quint32 getWindowVisibleFrameBorderThickness(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API ColorizationArea getColorizationArea();
[[nodiscard]] CUSTOMWINDOW_API DPIAwareness getDPIAwarenessForWindow(const WId winId);
[[nodiscard]] CUSTOMWINDOW_API SystemTheme getSystemTheme();
[[nodiscard]] CUSTOMWINDOW_API QPalette getStandardPalette(const SystemTheme theme);
[[nodiscard]] CUSTOMWINDOW_API bool displaySystemMenu(const WId winId, const QPoint &pos = {});

} // namespace Utils

CUSTOMWINDOW_END_NAMESPACE
