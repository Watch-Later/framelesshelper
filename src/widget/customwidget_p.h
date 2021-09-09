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

#include <customwindow_global.h>
#include <QtCore/quuid.h>
#include <QtGui/qwindowdefs.h>
#include <QtGui/qpalette.h>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QIcon)
QT_FORWARD_DECLARE_CLASS(QColor)
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_END_NAMESPACE

CUSTOMWINDOW_BEGIN_NAMESPACE

class CustomWidget;

class CustomWidgetPrivate
{
    Q_DECLARE_PUBLIC(CustomWidget)
    Q_DISABLE_COPY_MOVE(CustomWidgetPrivate)

public:
    explicit CustomWidgetPrivate(CustomWidget *q);
    ~CustomWidgetPrivate();

    void initialize();
    void uninitialize();

    [[nodiscard]] bool customFrameEnabled() const;
    void setCustomFrameEnabled(const bool value);

    [[nodiscard]] quint32 resizeBorderThickness() const;
    void setResizeBorderThickness(const quint32 value);

    [[nodiscard]] quint32 captionHeight() const;
    void setCaptionHeight(const quint32 value);

    [[nodiscard]] quint32 titleBarHeight() const;
    void setTitleBarHeight(const quint32 value);

    [[nodiscard]] bool resizable() const;
    void setResizable(const bool value);

    [[nodiscard]] bool autoDetectHighContrast() const;
    void setAutoDetectHighContrast(const bool value);

    [[nodiscard]] bool autoDetectColorScheme() const;
    void setAutoDetectColorScheme(const bool value);

    [[nodiscard]] bool frameBorderVisible() const;
    void setFrameBorderVisible(const bool value);

    [[nodiscard]] quint32 frameBorderThickness() const;
    void setFrameBorderThickness(const quint32 value);

    [[nodiscard]] QColor frameBorderColor() const;
    void setFrameBorderColor(const QColor &value);

    [[nodiscard]] bool titleBarVisible() const;
    void setTitleBarVisible(const bool value);

    [[nodiscard]] bool titleBarIconVisible() const;
    void setTitleBarIconVisible(const bool value);

    [[nodiscard]] QIcon titleBarIcon() const;
    void setTitleBarIcon(const QIcon &value);

    [[nodiscard]] Qt::Alignment titleBarTextAlignment() const;
    void setTitleBarTextAlignment(const Qt::Alignment value);

    [[nodiscard]] QColor titleBarBackgroundColor() const;
    void setTitleBarBackgroundColor(const QColor &value);

private:
    void emitAllSignals();
    void updateContentsMargins();

private:
    CustomWidget *q_ptr = nullptr;
    bool m_initialized = false;
    QUuid m_id = {};
    WId m_winId = reinterpret_cast<WId>(nullptr);
    QWindow *m_window = nullptr;
    bool m_autoFillBackground = false;
    QPalette m_palette = {};
};

CUSTOMWINDOW_END_NAMESPACE
