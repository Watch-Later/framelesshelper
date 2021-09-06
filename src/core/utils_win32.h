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
#include <QtGui/qwindow.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

namespace Utils
{

[[nodiscard]] CUSTOMWINDOW_API bool isWin8OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isWin8Point1OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isWin10OrGreater();
[[nodiscard]] CUSTOMWINDOW_API bool isDwmCompositionAvailable();
CUSTOMWINDOW_API void triggerFrameChange(const WId winId);
CUSTOMWINDOW_API void updateFrameMargins(const WId winId, const bool reset);
CUSTOMWINDOW_API void updateQtFrameMargins(QWindow *window, const bool enable);
[[nodiscard]] CUSTOMWINDOW_API QString getSystemErrorMessage(const QString &function, const HRESULT hr);
[[nodiscard]] CUSTOMWINDOW_API QString getSystemErrorMessage(const QString &function);

} // namespace Utils

CUSTOMWINDOW_END_NAMESPACE
