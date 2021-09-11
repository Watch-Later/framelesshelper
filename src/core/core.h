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

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QUuid)
QT_FORWARD_DECLARE_CLASS(QIcon)
QT_FORWARD_DECLARE_CLASS(QPalette)
QT_FORWARD_DECLARE_CLASS(QPainter)
QT_END_NAMESPACE

CUSTOMWINDOW_BEGIN_NAMESPACE

namespace Core
{

namespace SystemEvent
{
[[nodiscard]] CUSTOMWINDOW_API bool handler(const QUuid &id, const void *event, qintptr *result);
} // namespace SystemEvent

namespace Settings
{
[[nodiscard]] CUSTOMWINDOW_API QUuid create(const QVariantHash &initialValue);
[[nodiscard]] CUSTOMWINDOW_API bool destroy(const QUuid &id);
[[nodiscard]] CUSTOMWINDOW_API QVariant get(const QUuid &id, const QString &name, const QVariant &defaultValue);
[[nodiscard]] CUSTOMWINDOW_API bool set(const QUuid &id, const QString &name, const QVariant &value);
} // namespace Settings

namespace Window
{
[[nodiscard]] CUSTOMWINDOW_API QVariant getProperty(const QUuid &id, const WindowProperty prop);
[[nodiscard]] CUSTOMWINDOW_API bool setProperty(const QUuid &id, const WindowProperty prop, const QVariant &value);
[[nodiscard]] CUSTOMWINDOW_API QIcon getSystemButtonIcon(const SystemButton type, const SystemTheme theme);
[[nodiscard]] CUSTOMWINDOW_API bool setSystemButtonIcon(const SystemButton type, const SystemTheme theme, const QIcon &icon);
[[nodiscard]] CUSTOMWINDOW_API QPalette getStandardPalette(const SystemTheme theme);
[[nodiscard]] CUSTOMWINDOW_API bool setStandardPalette(const SystemTheme theme, const QPalette &palette);
[[nodiscard]] CUSTOMWINDOW_API quint32 getPreferredSystemMetric(const QUuid &id, const SystemMetric metric, const bool dpiScale);
} // namespace Window

namespace Draw
{
[[nodiscard]] bool customWindowFrame(const QUuid &id, QPainter *painter);
[[nodiscard]] bool customTitleBar(const QUuid &id, QPainter *painter);
} // namespace Draw

} // namespace Core

CUSTOMWINDOW_END_NAMESPACE
