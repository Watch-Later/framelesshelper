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

CUSTOMWINDOW_BEGIN_NAMESPACE

bool Utils::isWin8OrGreater()
{
#ifdef Q_OS_WINDOWS
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8);
#endif
    return result;
#else
    return false;
#endif
}

bool Utils::isWin8Point1OrGreater()
{
#ifdef Q_OS_WINDOWS
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8_1);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8_1);
#endif
    return result;
#else
    return false;
#endif
}

bool Utils::isWin10OrGreater()
{
#ifdef Q_OS_WINDOWS
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    static const bool result = (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10);
#else
    static const bool result = (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS10);
#endif
    return result;
#else
    return false;
#endif
}

bool Utils::isWin11OrGreater()
{
#ifdef Q_OS_WINDOWS
    // ### TO BE IMPLEMENTED
    static const bool result = false;
    return result;
#else
    return false;
#endif
}

QPalette Utils::getStandardPalette(const SystemTheme theme)
{
    QPalette palette = {};
    switch (theme) {
    case SystemTheme::Light: {
        // ### TO BE IMPLEMENTED
    } break;
    case SystemTheme::Dark: {
        // ### TO BE IMPLEMENTED
    } break;
    case SystemTheme::HighContrast: {
        // ### TO BE IMPLEMENTED
    } break;
    }
    return palette;
}

quint32 Utils::getPreferredSystemMetric(const QUuid &id, const SystemMetric metric, const bool dpiScale)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return 0;
    }
    const auto winId = qvariant_cast<WId>(Core::Settings::get(id, QString::fromUtf8(Constants::kWinIdFlag), 0));
    Q_ASSERT(winId);
    if (!winId) {
        return 0;
    }
    const quint32 dpi = (dpiScale ? getDPIForWindow(winId) : USER_DEFAULT_SCREEN_DPI);
    const qreal dpr = (dpiScale ? (static_cast<qreal>(dpi) / static_cast<qreal>(USER_DEFAULT_SCREEN_DPI)) : 1.0);
    quint32 userValue = 0;
    switch (metric) {
    case SystemMetric::ResizeBorderThickness: {
        userValue = Core::Settings::get(id, QString::fromUtf8(Constants::kResizeBorderThicknessFlag), 0).toUInt();
    }
    case SystemMetric::CaptionHeight: {
        userValue = Core::Settings::get(id, QString::fromUtf8(Constants::kCaptionHeightFlag), 0).toUInt();
    }
    case SystemMetric::TitleBarHeight: {
        userValue = Core::Settings::get(id, QString::fromUtf8(Constants::kTitleBarHeightFlag), 0).toUInt();
    }
    case SystemMetric::FrameBorderThickness: {
        userValue = Core::Settings::get(id, QString::fromUtf8(Constants::kFrameBorderThicknessFlag), 0).toUInt();
    }
    }
    if (userValue > 0) {
        return (dpiScale ? qRound(static_cast<qreal>(userValue) * dpr) : userValue);
    } else {
        return getSystemMetric(winId, metric, dpiScale);
    }
}

bool Utils::isWindowResizable(const QUuid &id)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return false;
    }
    Qt::WindowFlags flags = {};
    QSize min = {}, max = {};
    // Size policy?
    const auto widget = qvariant_cast<QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWidgetHandleFlag), 0));
    if (widget && widget->isWidgetType()) {
        flags = qvariant_cast<Qt::WindowFlags>(widget->property("windowFlags"));
        min = widget->property("minimumSize").toSize();
        max = widget->property("maximumSize").toSize();
    } else {
        const auto window = qvariant_cast<QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWindowHandleFlag), 0));
        if (window && window->isWindowType()) {
            flags = qvariant_cast<Qt::WindowFlags>(window->property("flags"));
            min = QSize(window->property("minimumWidth").toInt(), window->property("minimumHeight").toInt());
            max = QSize(window->property("maximumWidth").toInt(), window->property("maximumHeight").toInt());
        } else {
            return true;
        }
    }
    if (flags & Qt::MSWindowsFixedSizeDialogHint) {
        return false;
    }
    if (!min.isEmpty() && !max.isEmpty() && (min == max)) {
        return false;
    }
    return Core::Settings::get(id, QString::fromUtf8(Constants::kWindowResizableFlag), true).toBool();
}

QColor Utils::getFrameBorderColor(const QUuid &id)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return Qt::black;
    }
    bool active = true;
    const auto widget = qvariant_cast<QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWidgetHandleFlag), 0));
    if (widget && widget->isWidgetType()) {
        active = (widget->property("focus").toBool() || widget->property("isActiveWindow").toBool());
    } else {
        const auto window = qvariant_cast<QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWindowHandleFlag), 0));
        if (window && window->isWindowType()) {
            active = window->property("active").toBool();
        } else {
            return Qt::black;
        }
    }
    if (active) {
        const ColorizationArea area = getColorizationArea();
        if ((area == ColorizationArea::TitleBar_WindowBorder) || (area == ColorizationArea::All)) {
            const auto borderColor = qvariant_cast<QColor>(Core::Settings::get(id, QString::fromUtf8(Constants::kFrameBorderColorFlag), QVariant::fromValue(Qt::black)));
            if (borderColor.isValid()) {
                return borderColor;
            } else {
                return getColorizationColor();
            }
        } else {
            return Qt::black;
        }
    } else {
        return Qt::darkGray;
    }
}

CUSTOMWINDOW_END_NAMESPACE
