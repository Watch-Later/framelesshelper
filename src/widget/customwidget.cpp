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
#include <settings.h>
#include <utils.h>
#include <QtCore/qdebug.h>
#include <QtCore/qcoreapplication.h>
#include <QtGui/qicon.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

CustomWidgetPrivate::CustomWidgetPrivate(CustomWidget *q)
{
    Q_ASSERT(q);
    if (!q) {
        qFatal("CustomWidgetPrivate's q is null.");
        return;
    }
    q_ptr = q;
}

CustomWidgetPrivate::~CustomWidgetPrivate()
{
}

void CustomWidgetPrivate::initialize()
{
    if (m_initialized) {
        return;
    }

    if (!QCoreApplication::testAttribute(Qt::AA_DontCreateNativeWidgetSiblings)) {
        QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    }
    if (QCoreApplication::testAttribute(Qt::AA_NativeWindows)) {
        QCoreApplication::setAttribute(Qt::AA_NativeWindows, false);
    }

    Q_Q(CustomWidget);
    q->setAttribute(Qt::WA_DontCreateNativeAncestors);
    q->setAttribute(Qt::WA_NativeWindow);
    q->createWinId();
    q->setAutoFillBackground(true);
    q->setPalette(Utils::getStandardPalette(Utils::getSystemTheme()));

    m_id = Core::Settings::create({});

    m_initialized = true;
}

void CustomWidgetPrivate::uninitialize()
{
    if (!m_initialized) {
        return;
    }
    // todo: remove settings cache
    m_initialized = false;
}

bool CustomWidgetPrivate::customFrameEnabled() const
{
}

void CustomWidgetPrivate::setCustomFrameEnabled(const bool value)
{
}

quint32 CustomWidgetPrivate::resizeBorderThickness() const
{
}

void CustomWidgetPrivate::setResizeBorderThickness(const quint32 value)
{
}

quint32 CustomWidgetPrivate::captionHeight() const
{
}

void CustomWidgetPrivate::setCaptionHeight(const quint32 value)
{
}

quint32 CustomWidgetPrivate::titleBarHeight() const
{
}

void CustomWidgetPrivate::setTitleBarHeight(const quint32 value)
{
}

bool CustomWidgetPrivate::resizable() const
{
}

void CustomWidgetPrivate::setResizable(const bool value)
{
}

bool CustomWidgetPrivate::autoDetectHighContrast() const
{
}

void CustomWidgetPrivate::setAutoDetectHighContrast(const bool value)
{
}

bool CustomWidgetPrivate::autoDetectColorScheme() const
{
}

void CustomWidgetPrivate::setAutoDetectColorScheme(const bool value)
{
}

bool CustomWidgetPrivate::frameBorderVisible() const
{
}

void CustomWidgetPrivate::setFrameBorderVisible(const bool value)
{
}

quint32 CustomWidgetPrivate::frameBorderThickness() const
{
}

void CustomWidgetPrivate::setFrameBorderThickness(const quint32 value)
{
}

QColor CustomWidgetPrivate::frameBorderColor() const
{
}

void CustomWidgetPrivate::setFrameBorderColor(const QColor &value)
{
}

bool CustomWidgetPrivate::titleBarVisible() const
{
}

void CustomWidgetPrivate::setTitleBarVisible(const bool value)
{
}

bool CustomWidgetPrivate::titleBarIconVisible() const
{
}

void CustomWidgetPrivate::setTitleBarIconVisible(const bool value)
{
}

QIcon CustomWidgetPrivate::titleBarIcon() const
{
}

void CustomWidgetPrivate::setTitleBarIcon(const QIcon &value)
{
}

Qt::Alignment CustomWidgetPrivate::titleBarTextAlignment() const
{
}

void CustomWidgetPrivate::setTitleBarTextAlignment(const Qt::Alignment value)
{
}

QColor CustomWidgetPrivate::titleBarBackgroundColor() const
{
}

void CustomWidgetPrivate::setTitleBarBackgroundColor(const QColor &value)
{
}

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    d_ptr.reset(new CustomWidgetPrivate(this));
    Q_D(CustomWidget);
    d->initialize();
}

CustomWidget::~CustomWidget()
{
    Q_D(CustomWidget);
    d->uninitialize();
}

bool CustomWidget::customFrameEnabled() const
{
    Q_D(const CustomWidget);
    return d->customFrameEnabled();
}

void CustomWidget::setCustomFrameEnabled(const bool value)
{
    Q_D(CustomWidget);
    d->setCustomFrameEnabled(value);
}

quint32 CustomWidget::resizeBorderThickness() const
{
    Q_D(const CustomWidget);
    return d->resizeBorderThickness();
}

void CustomWidget::setResizeBorderThickness(const quint32 value)
{
    Q_D(CustomWidget);
    d->setResizeBorderThickness(value);
}

quint32 CustomWidget::captionHeight() const
{
    Q_D(const CustomWidget);
    return d->captionHeight();
}

void CustomWidget::setCaptionHeight(const quint32 value)
{
    Q_D(CustomWidget);
    d->setCaptionHeight(value);
}

quint32 CustomWidget::titleBarHeight() const
{
    Q_D(const CustomWidget);
    return d->titleBarHeight();
}

void CustomWidget::setTitleBarHeight(const quint32 value)
{
    Q_D(CustomWidget);
    d->setTitleBarHeight(value);
}

bool CustomWidget::resizable() const
{
    Q_D(const CustomWidget);
    return d->resizable();
}

void CustomWidget::setResizable(const bool value)
{
    Q_D(CustomWidget);
    d->setResizable(value);
}

bool CustomWidget::autoDetectHighContrast() const
{
    Q_D(const CustomWidget);
    return d->autoDetectHighContrast();
}

void CustomWidget::setAutoDetectHighContrast(const bool value)
{
    Q_D(CustomWidget);
    d->setAutoDetectHighContrast(value);
}

bool CustomWidget::autoDetectColorScheme() const
{
    Q_D(const CustomWidget);
    return d->autoDetectColorScheme();
}

void CustomWidget::setAutoDetectColorScheme(const bool value)
{
    Q_D(CustomWidget);
    d->setAutoDetectColorScheme(value);
}

bool CustomWidget::frameBorderVisible() const
{
    Q_D(const CustomWidget);
    return d->frameBorderVisible();
}

void CustomWidget::setFrameBorderVisible(const bool value)
{
    Q_D(CustomWidget);
    d->setFrameBorderVisible(value);
}

quint32 CustomWidget::frameBorderThickness() const
{
    Q_D(const CustomWidget);
    return d->frameBorderThickness();
}

void CustomWidget::setFrameBorderThickness(const quint32 value)
{
    Q_D(CustomWidget);
    d->setFrameBorderThickness(value);
}

QColor CustomWidget::frameBorderColor() const
{
    Q_D(const CustomWidget);
    return d->frameBorderColor();
}

void CustomWidget::setFrameBorderColor(const QColor &value)
{
    Q_D(CustomWidget);
    d->setFrameBorderColor(value);
}

bool CustomWidget::titleBarVisible() const
{
    Q_D(const CustomWidget);
    return d->titleBarVisible();
}

void CustomWidget::setTitleBarVisible(const bool value)
{
    Q_D(CustomWidget);
    d->setTitleBarVisible(value);
}

bool CustomWidget::titleBarIconVisible() const
{
    Q_D(const CustomWidget);
    return d->titleBarIconVisible();
}

void CustomWidget::setTitleBarIconVisible(const bool value)
{
    Q_D(CustomWidget);
    d->setTitleBarIconVisible(value);
}

QIcon CustomWidget::titleBarIcon() const
{
    Q_D(const CustomWidget);
    return d->titleBarIcon();
}

void CustomWidget::setTitleBarIcon(const QIcon &value)
{
    Q_D(CustomWidget);
    d->setTitleBarIcon(value);
}

Qt::Alignment CustomWidget::titleBarTextAlignment() const
{
    Q_D(const CustomWidget);
    return d->titleBarTextAlignment();
}

void CustomWidget::setTitleBarTextAlignment(const Qt::Alignment value)
{
    Q_D(CustomWidget);
    d->setTitleBarTextAlignment(value);
}

QColor CustomWidget::titleBarBackgroundColor() const
{
    Q_D(const CustomWidget);
    return d->titleBarBackgroundColor();
}

void CustomWidget::setTitleBarBackgroundColor(const QColor &value)
{
    Q_D(CustomWidget);
    d->setTitleBarBackgroundColor(value);
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
