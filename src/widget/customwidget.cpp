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
#include <QtCore/qvariant.h>
#include <QtCore/qcoreapplication.h>
#include <QtGui/qicon.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

CustomWidgetPrivate::CustomWidgetPrivate(CustomWidget *q)
{
    Q_ASSERT(q);
    if (q) {
        q_ptr = q;
    } else {
        qFatal("CustomWidgetPrivate's q is null.");
    }
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

    m_winId = q->winId();
    m_window = q->windowHandle();
    m_autoFillBackground = q->autoFillBackground();
    m_palette = q->palette();

    QVariantHash options = {};
    options.insert(QString::fromUtf8(Constants::kWidgetHandleFlag), QVariant::fromValue(static_cast<QWidget *>(q)));
    options.insert(QString::fromUtf8(Constants::kWindowHandleFlag), QVariant::fromValue(m_window));
    options.insert(QString::fromUtf8(Constants::kWinIdFlag), m_winId);
    m_id = Core::Settings::create(options);

    m_initialized = !m_id.isNull();
}

void CustomWidgetPrivate::uninitialize()
{
    if (!m_initialized) {
        return;
    }
    // todo: remove settings cache
    m_id = QUuid();
    m_initialized = false;
}

bool CustomWidgetPrivate::customFrameEnabled() const
{
    if (!m_initialized) {
        return false;
    }
    return Core::Settings::get(m_id, QString::fromUtf8(Constants::kCustomWindowFrameFlag), false).toBool();
}

void CustomWidgetPrivate::setCustomFrameEnabled(const bool value)
{
    if (!m_initialized) {
        return;
    }
    if (!Core::Settings::set(m_id, QString::fromUtf8(Constants::kCustomWindowFrameFlag), true)) {
        qWarning() << "";
        return;
    }
    if (!Utils::triggerFrameChange(m_winId)) {
        qWarning() << "";
        return;
    }
    updateContentsMargins();
    Q_Q(CustomWidget);
    q->update();
    Q_EMIT q->customFrameEnabledChanged(value);
    emitAllSignals();
}

quint32 CustomWidgetPrivate::resizeBorderThickness() const
{
    if (!m_initialized) {
        return 0;
    }
    return Utils::getPreferredSystemMetric(m_id, m_winId, SystemMetric::ResizeBorderThickness, false);
}

void CustomWidgetPrivate::setResizeBorderThickness(const quint32 value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kResizeBorderThicknessFlag), value)) {
        Q_Q(CustomWidget);
        q->update();
        Q_EMIT q->resizeBorderThicknessChanged(value);
    } else {
        qWarning() << "";
    }
}

quint32 CustomWidgetPrivate::captionHeight() const
{
    if (!m_initialized) {
        return 0;
    }
    return Utils::getPreferredSystemMetric(m_id, m_winId, SystemMetric::CaptionHeight, false);
}

void CustomWidgetPrivate::setCaptionHeight(const quint32 value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kCaptionHeightFlag), value)) {
        Q_Q(CustomWidget);
        q->update();
        Q_EMIT q->captionHeightChanged(value);
    } else {
        qWarning() << "";
    }
}

quint32 CustomWidgetPrivate::titleBarHeight() const
{
    if (!m_initialized) {
        return 0;
    }
    return Utils::getPreferredSystemMetric(m_id, m_winId, SystemMetric::TitleBarHeight, false);
}

void CustomWidgetPrivate::setTitleBarHeight(const quint32 value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kTitleBarHeightFlag), value)) {
        Q_Q(CustomWidget);
        q->update();
        Q_EMIT q->titleBarHeightChanged(value);
    } else {
        qWarning() << "";
    }
}

bool CustomWidgetPrivate::resizable() const
{
    if (!m_initialized) {
        return true;
    }
    Q_Q(const CustomWidget);
    if (q->windowFlags() & Qt::MSWindowsFixedSizeDialogHint) {
        return true;
    }
    const QSize min = q->minimumSize();
    const QSize max = q->maximumSize();
    if (!min.isEmpty() && !max.isEmpty() && (min == max)) {
        return true;
    }
    return Core::Settings::get(m_id, QString::fromUtf8(Constants::kWindowResizableFlag), true).toBool();
}

bool CustomWidgetPrivate::autoDetectHighContrast() const
{
    if (!m_initialized) {
        return true;
    }
    return Core::Settings::get(m_id, QString::fromUtf8(Constants::kAutoDetectHighContrastFlag), true).toBool();
}

void CustomWidgetPrivate::setAutoDetectHighContrast(const bool value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kAutoDetectHighContrastFlag), value)) {
        Q_Q(CustomWidget);
        if (customFrameEnabled()) {
            if (value) {
                q->setAutoFillBackground(true);
                q->setPalette(Utils::getStandardPalette(Utils::getSystemTheme()));
                q->update();
            } else {
                if (!autoDetectColorScheme()) {
                    q->setAutoFillBackground(m_autoFillBackground);
                    q->setPalette(m_palette);
                    q->update();
                }
            }
        }
        Q_EMIT q->autoDetectHighContrastChanged(value);
    } else {
        qWarning() << "";
    }
}

bool CustomWidgetPrivate::autoDetectColorScheme() const
{
    if (!m_initialized) {
        return true;
    }
    return Core::Settings::get(m_id, QString::fromUtf8(Constants::kAutoDetectColorSchemeFlag), true).toBool();
}

void CustomWidgetPrivate::setAutoDetectColorScheme(const bool value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kAutoDetectColorSchemeFlag), value)) {
        Q_Q(CustomWidget);
        if (customFrameEnabled()) {
            if (value) {
                q->setAutoFillBackground(true);
                q->setPalette(Utils::getStandardPalette(Utils::getSystemTheme()));
                q->update();
            } else {
                if (!autoDetectHighContrast()) {
                    q->setAutoFillBackground(m_autoFillBackground);
                    q->setPalette(m_palette);
                    q->update();
                }
            }
        }
        Q_EMIT q->autoDetectColorSchemeChanged(value);
    } else {
        qWarning() << "";
    }
}

bool CustomWidgetPrivate::frameBorderVisible() const
{
    if (!m_initialized) {
        return true;
    }
    return Core::Settings::get(m_id, QString::fromUtf8(Constants::kFrameBorderVisibleFlag), true).toBool();
}

void CustomWidgetPrivate::setFrameBorderVisible(const bool value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kFrameBorderVisibleFlag), value)) {
        Q_Q(CustomWidget);
        q->update();
        Q_EMIT q->frameBorderVisibleChanged(value);
    } else {
        qWarning() << "";
    }
}

quint32 CustomWidgetPrivate::frameBorderThickness() const
{
    if (!m_initialized) {
        return 0;
    }
    const quint32 userValue = Core::Settings::get(m_id, QString::fromUtf8(Constants::kFrameBorderThicknessFlag), 0).toUInt();
    return ((userValue > 0) ? userValue : Utils::getWindowVisibleFrameBorderThickness(m_winId));
}

void CustomWidgetPrivate::setFrameBorderThickness(const quint32 value)
{
    if (!m_initialized) {
        return;
    }
    if (Core::Settings::set(m_id, QString::fromUtf8(Constants::kFrameBorderThicknessFlag), value)) {
        Q_Q(CustomWidget);
        q->update();
        Q_EMIT q->frameBorderThicknessChanged(value);
    } else {
        qWarning() << "";
    }
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

void CustomWidgetPrivate::emitAllSignals()
{
    Q_Q(CustomWidget);
    //Q_EMIT q->customFrameEnabledChanged(customFrameEnabled());
    Q_EMIT q->resizeBorderThicknessChanged(resizeBorderThickness());
    Q_EMIT q->captionHeightChanged(captionHeight());
    Q_EMIT q->titleBarHeightChanged(titleBarHeight());
    Q_EMIT q->resizableChanged(resizable());
    Q_EMIT q->autoDetectHighContrastChanged(autoDetectHighContrast());
    Q_EMIT q->autoDetectColorSchemeChanged(autoDetectColorScheme());
    Q_EMIT q->frameBorderVisibleChanged(frameBorderVisible());
    Q_EMIT q->frameBorderThicknessChanged(frameBorderThickness());
    Q_EMIT q->frameBorderColorChanged(frameBorderColor());
    Q_EMIT q->titleBarVisibleChanged(titleBarVisible());
    Q_EMIT q->titleBarIconVisibleChanged(titleBarIconVisible());
    Q_EMIT q->titleBarIconChanged(titleBarIcon());
    Q_EMIT q->titleBarTextAlignmentChanged(titleBarTextAlignment());
    Q_EMIT q->titleBarBackgroundColorChanged(titleBarBackgroundColor());
}

void CustomWidgetPrivate::updateContentsMargins()
{
    const int margin = ((Utils::isMaximized(m_winId) || Utils::isFullScreened(m_winId)) ? 0 : Utils::getWindowVisibleFrameBorderThickness(m_winId));
    Q_Q(CustomWidget);
    q->setContentsMargins(margin, margin, margin, margin);
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
