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
#include "settings.h"
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>
#include <QtCore/quuid.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QtCore/qoperatingsystemversion.h>
#else
#include <QtCore/qsysinfo.h>
#endif
#include <QtGui/qpalette.h>
#include <QtGui/qicon.h>
#include <QtGui/qwindow.h>
#include <QtGui/qguiapplication.h>

static inline void initCoreResource()
{
    Q_INIT_RESOURCE(core);
}

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
    const auto winId = qvariant_cast<WId>(getWindowProperty(id, WindowProperty::Id));
    Q_ASSERT(winId);
    if (!winId) {
        return 0;
    }
    const qreal dpr = (dpiScale ? getWindowProperty(id, WindowProperty::DevicePixelRatio).toReal() : 1.0);
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

QVariant Utils::getWindowProperty(const QUuid &id, const WindowProperty prop)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return {};
    }
    const auto widget = qvariant_cast<const QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWidgetHandleFlag), 0));
    const auto window = qvariant_cast<const QObject *>(Core::Settings::get(id, QString::fromUtf8(Constants::kWindowHandleFlag), 0));
    const bool isWidget = (widget && widget->isWidgetType());
    const bool isWindow = (window && window->isWindowType());
    if (!isWidget && !isWindow) {
        qWarning() << "";
        return {};
    }
    const QObject *object = (isWidget ? widget : window);
    switch (prop) {
    case WindowProperty::ObjectName: {
        return object->objectName();
    }
    case WindowProperty::Id: {
        return Core::Settings::get(id, QString::fromUtf8(Constants::kWinIdFlag), 0);
    }
    case WindowProperty::X: {
        return object->property("x");
    }
    case WindowProperty::Y: {
        return object->property("y");
    }
    case WindowProperty::Position: {
        if (isWidget) {
            return object->property("pos");
        } else {
            const int x = getWindowProperty(id, WindowProperty::X).toInt();
            const int y = getWindowProperty(id, WindowProperty::Y).toInt();
            return QPoint(x, y);
        }
    }
    case WindowProperty::Width: {
        return object->property("width");
    }
    case WindowProperty::Height: {
        return object->property("height");
    }
    case WindowProperty::MinimumWidth: {
        return object->property("minimumWidth");
    }
    case WindowProperty::MaximumWidth: {
        return object->property("maximumWidth");
    }
    case WindowProperty::MinimumHeight: {
        return object->property("minimumHeight");
    }
    case WindowProperty::MaximumHeight: {
        return object->property("maximumHeight");
    }
    case WindowProperty::Size: {
        if (isWidget) {
            return object->property("size");
        } else {
            const int width = getWindowProperty(id, WindowProperty::Width).toInt();
            const int height = getWindowProperty(id, WindowProperty::Height).toInt();
            return QSize(width, height);
        }
    }
    case WindowProperty::MinimumSize: {
        if (isWidget) {
            return object->property("minimumSize");
        } else {
            const int minimumWidth = getWindowProperty(id, WindowProperty::MinimumWidth).toInt();
            const int minimumHeight = getWindowProperty(id, WindowProperty::MinimumHeight).toInt();
            return QSize(minimumWidth, minimumHeight);
        }
    }
    case WindowProperty::MaximumSize: {
        if (isWidget) {
            return object->property("maximumSize");
        } else {
            const int maximumWidth = getWindowProperty(id, WindowProperty::MaximumWidth).toInt();
            const int maximumHeight = getWindowProperty(id, WindowProperty::MaximumHeight).toInt();
            return QSize(maximumWidth, maximumHeight);
        }
    }
    case WindowProperty::Geometry: {
        if (isWidget) {
            return object->property("geometry");
        } else {
            const QPoint position = getWindowProperty(id, WindowProperty::Position).toPoint();
            const QSize size = getWindowProperty(id, WindowProperty::Size).toSize();
            return QRect(position, size);
        }
    }
    case WindowProperty::State: {
        if (isWidget) {
            const bool minimized = object->property("minimized").toBool();
            const bool maximized = object->property("maximized").toBool();
            const bool fullScreened = object->property("fullScreen").toBool();
            if (minimized) {
                return Qt::WindowMinimized;
            } else if (maximized) {
                return Qt::WindowMaximized;
            } else if (fullScreened) {
                return Qt::WindowFullScreen;
            } else {
                return Qt::WindowNoState;
            }
        } else {
            const auto visibility = qvariant_cast<QWindow::Visibility>(object->property("visibility"));
            switch (visibility) {
            case QWindow::Windowed: {
                return Qt::WindowNoState;
            }
            case QWindow::Minimized: {
                return Qt::WindowMinimized;
            }
            case QWindow::Maximized: {
                return Qt::WindowMaximized;
            }
            case QWindow::FullScreen: {
                return Qt::WindowFullScreen;
            }
            default:
                break;
            }
            return Qt::WindowNoState;
        }
    }
    case WindowProperty::Flags: {
        return (isWidget ? object->property("windowFlags") : object->property("flags"));
    }
    case WindowProperty::Active: {
        return (isWidget ? object->property("isActiveWindow") : object->property("active"));
    }
    case WindowProperty::Icon: {
        const auto userIcon = qvariant_cast<QIcon>(Core::Settings::get(id, QString::fromUtf8(Constants::kTitleBarIconFlag), {}));
        if (userIcon.isNull()) {
            QIcon icon = {};
            if (isWidget) {
                icon = qvariant_cast<QIcon>(object->property("windowIcon"));
            }
            if (icon.isNull()) {
                icon = QGuiApplication::windowIcon();
            }
            if (icon.isNull()) {
                initCoreResource();
                icon = QIcon(QStringLiteral(":/assets/common/qt-logo.svg"));
            }
            Q_ASSERT(!icon.isNull());
            if (icon.isNull()) {
                return {};
            }
            return icon;
        } else {
            return userIcon;
        }
    }
    case WindowProperty::Title: {
        return (isWidget ? object->property("windowTitle") : object->property("title"));
    }
    case WindowProperty::DotsPerInch: {
        const auto winId = qvariant_cast<WId>(getWindowProperty(id, WindowProperty::Id));
        return (winId ? getDPI(winId) : Constants::kDefaultScreenDPI);
    }
    case WindowProperty::DevicePixelRatio: {
        const quint32 dpi = getWindowProperty(id, WindowProperty::DotsPerInch).toUInt();
        return qreal(static_cast<qreal>(dpi) / static_cast<qreal>(Constants::kDefaultScreenDPI));
    }
    case WindowProperty::FrameBorderThickness: {
        return getPreferredSystemMetric(id, SystemMetric::FrameBorderThickness, false);
    }
    case WindowProperty::FrameBorderColor: {
        const bool active = getWindowProperty(id, WindowProperty::Active).toBool();
        if (active) {
            const ColorizationArea area = getColorizationArea();
            if ((area == ColorizationArea::TitleBar_WindowBorder) || (area == ColorizationArea::All)) {
                const auto borderColor = qvariant_cast<QColor>(Core::Settings::get(id, QString::fromUtf8(Constants::kFrameBorderColorFlag), {}));
                if (borderColor.isValid()) {
                    return borderColor;
                } else {
                    return getColorizationColor();
                }
            } else {
                return QColor(Qt::black);
            }
        } else {
            return QColor(Qt::darkGray);
        }
    }
    case WindowProperty::FrameBorderVisibility: {
        return Core::Settings::get(id, QString::fromUtf8(Constants::kFrameBorderVisibleFlag), true);
    }
    case WindowProperty::Resizable: {
        const auto flags = qvariant_cast<Qt::WindowFlags>(getWindowProperty(id, WindowProperty::Flags));
        if (flags & Qt::MSWindowsFixedSizeDialogHint) {
            return false;
        }
        const QSize minimumSize = getWindowProperty(id, WindowProperty::MinimumSize).toSize();
        const QSize maximumSize = getWindowProperty(id, WindowProperty::MaximumSize).toSize();
        if (!minimumSize.isEmpty() && !maximumSize.isEmpty() && (minimumSize == maximumSize)) {
            return false;
        }
        return Core::Settings::get(id, QString::fromUtf8(Constants::kWindowResizableFlag), true);
    }
    case WindowProperty::Font: {
        return {};
    }
    case WindowProperty::Palette: {
        return {};
    }
    case WindowProperty::TitleBarHeight: {
        return getPreferredSystemMetric(id, SystemMetric::TitleBarHeight, false);
    }
    case WindowProperty::TitleBarVisibility: {
        return Core::Settings::get(id, QString::fromUtf8(Constants::kTitleBarVisibleFlag), true);
    }
    case WindowProperty::TitleTextAlignment: {
        return Core::Settings::get(id, QString::fromUtf8(Constants::kTitleBarTextAlignmentFlag), {});
    }
    case WindowProperty::CaptionHeight: {
        return getPreferredSystemMetric(id, SystemMetric::CaptionHeight, false);
    }
    case WindowProperty::ResizeBorderThickness: {
        return getPreferredSystemMetric(id, SystemMetric::ResizeBorderThickness, false);
    }
    }
    return {};
}

QIcon Utils::getSystemButtonIcon(const SystemButtonType type, const SystemTheme theme)
{
    QString path = QStringLiteral(":/assets/");
    switch (theme) {
    case SystemTheme::Light: {
        path.append(QStringLiteral("light"));
    } break;
    case SystemTheme::Dark: {
        path.append(QStringLiteral("dark"));
    } break;
    case SystemTheme::HighContrast: {
        path.append(QStringLiteral("highcontrast"));
    } break;
    }
    path.append(u'/');
    switch (type) {
    case SystemButtonType::Minimize: {
        path.append(QStringLiteral("minimize"));
    } break;
    case SystemButtonType::Maximize: {
        path.append(QStringLiteral("maximize"));
    } break;
    case SystemButtonType::Restore: {
        path.append(QStringLiteral("restore"));
    } break;
    case SystemButtonType::Close: {
        path.append(QStringLiteral("close"));
    } break;
    }
    path.append(QStringLiteral(".svg"));
    initCoreResource();
    return QIcon(path);
}

CUSTOMWINDOW_END_NAMESPACE
