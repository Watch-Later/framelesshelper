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

namespace Constants
{

[[maybe_unused]] constexpr char kFramelessModeFlag[] = "__CUSTOMWINDOW_FRAMELESS_MODE_ENABLED";
[[maybe_unused]] constexpr char kResizeBorderThicknessFlag[] = "__CUSTOMWINDOW_RESIZE_BORDER_THICKNESS";
[[maybe_unused]] constexpr char kCaptionHeightFlag[] = "__CUSTOMWINDOW_CAPTION_HEIGHT";
[[maybe_unused]] constexpr char kTitleBarHeightFlag[] = "__CUSTOMWINDOW_TITLE_BAR_HEIGHT";
[[maybe_unused]] constexpr char kHitTestVisibleInChromeFlag[] = "__CUSTOMWINDOW_HIT_TEST_VISIBLE_IN_CHROME";
[[maybe_unused]] constexpr char kWindowFixedSizeFlag[] = "__CUSTOMWINDOW_WINDOW_FIXED_SIZE";

} // namespace Constants

CUSTOMWINDOW_END_NAMESPACE
