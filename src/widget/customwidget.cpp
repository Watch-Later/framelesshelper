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

#include "customwidget.h"
#include "customwidget_p.h"
#include <QtCore/qdebug.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    d_ptr.reset(new CustomWidgetPrivate(this));
}

CustomWidget::~CustomWidget()
{
}

bool CustomWidget::customFrameEnabled() const
{
    return false;
}

void CustomWidget::setCustomFrameEnabled(const bool value)
{

}

quint32 CustomWidget::resizeBorderThickness() const
{
    return 0;
}

void CustomWidget::setResizeBorderThickness(const quint32 value)
{

}

quint32 CustomWidget::captionHeight() const
{
    return 0;
}

void CustomWidget::setCaptionHeight(const quint32 value)
{

}

quint32 CustomWidget::titleBarHeight() const
{
    return 0;
}

void CustomWidget::setTitleBarHeight(const quint32 value)
{

}

QObjectList CustomWidget::hitTestVisibleObjects() const
{
    return {};
}

void CustomWidget::setHitTestVisible(QWidget *object, const bool visible)
{

}

bool CustomWidget::resizable() const
{
    return false;
}

void CustomWidget::setResizable(const bool value)
{

}

bool CustomWidget::autoDetectHighContrast() const
{
    return false;
}

void CustomWidget::setAutoDetectHighContrast(const bool value)
{

}

bool CustomWidget::autoDetectColorScheme() const
{
    return false;
}

void CustomWidget::setAutoDetectColorScheme(const bool value)
{

}

bool CustomWidget::frameBorderVisible() const
{
    return false;
}

void CustomWidget::setFrameBorderVisible(const bool value)
{

}

quint32 CustomWidget::frameBorderThickness() const
{
    return 0;
}

void CustomWidget::setFrameBorderThickness(const quint32 value)
{

}

QColor CustomWidget::frameBorderColor() const
{
    return {};
}

void CustomWidget::setFrameBorderColor(const QColor &value)
{

}

bool CustomWidget::titleBarVisible() const
{
    return false;
}

void CustomWidget::setTitleBarVisible(const bool value)
{

}

bool CustomWidget::titleBarIconVisible() const
{
    return false;
}

void CustomWidget::setTitleBarIconVisible(const bool value)
{

}

QIcon CustomWidget::titleBarIcon() const
{
    return {};
}

void CustomWidget::setTitleBarIcon(const QIcon &value)
{

}

Qt::Alignment CustomWidget::titleBarTextAlignment() const
{
    return {};
}

void CustomWidget::setTitleBarTextAlignment(const Qt::Alignment value)
{

}

QColor CustomWidget::titleBarBackgroundColor() const
{
    return {};
}

void CustomWidget::setTitleBarBackgroundColor(const QColor &value)
{

}

void CustomWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void CustomWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool CustomWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool CustomWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    return false;
}

CUSTOMWINDOW_END_NAMESPACE
