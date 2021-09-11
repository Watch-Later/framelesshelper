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

#include "core.h"
#include "utils.h"
#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>
#include <QtCore/quuid.h>
#include <QtGui/qpalette.h>
#include <QtGui/qicon.h>
#include <QtGui/qwindow.h>
#include <QtGui/qguiapplication.h>

static inline void initCoreResource()
{
    Q_INIT_RESOURCE(core);
}

CUSTOMWINDOW_BEGIN_NAMESPACE

QPalette Core::Window::getStandardPalette(const SystemTheme theme)
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

quint32 Core::Window::getPreferredSystemMetric(const QUuid &id, const SystemMetric metric, const bool dpiScale)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return 0;
    }
    quint32 userValue = 0;
    switch (metric) {
    case SystemMetric::ResizeBorderThickness: {
        userValue = Core::Settings::get(id, Constants::kResizeBorderThicknessFlag, 0).toUInt();
    }
    case SystemMetric::CaptionHeight: {
        userValue = Core::Settings::get(id, Constants::kCaptionHeightFlag, 0).toUInt();
    }
    case SystemMetric::TitleBarHeight: {
        userValue = Core::Settings::get(id, Constants::kTitleBarHeightFlag, 0).toUInt();
    }
    case SystemMetric::FrameBorderThickness: {
        userValue = Core::Settings::get(id, Constants::kFrameBorderThicknessFlag, 0).toUInt();
    }
    }
    if (userValue > 0) {
        const qreal dpr = (dpiScale ? getProperty(id, WindowProperty::DevicePixelRatio).toReal() : 1.0);
        return (dpiScale ? qRound(static_cast<qreal>(userValue) * dpr) : userValue);
    } else {
        const auto winId = qvariant_cast<WId>(getProperty(id, WindowProperty::Id));
        Q_ASSERT(winId);
        if (!winId) {
            return 0;
        }
        return Core::Utils::getSystemMetric(winId, metric, dpiScale);
    }
}

QVariant Core::Window::getProperty(const QUuid &id, const WindowProperty prop)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return {};
    }
    const auto widget = qvariant_cast<const QObject *>(Core::Settings::get(id, Constants::kWidgetHandleFlag, 0));
    const auto window = qvariant_cast<const QObject *>(Core::Settings::get(id, Constants::kWindowHandleFlag, 0));
    const bool isWidget = (widget && widget->isWidgetType());
    const bool isWindow = (window && window->isWindowType());
    if (!isWidget && !isWindow) {
        qWarning() << "";
        return {};
    }
    const QObject *object = (isWidget ? widget : window);
    switch (prop) {
    case WindowProperty::Name: {
        return object->objectName();
    }
    case WindowProperty::Id: {
        // todo
        return {};
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
            const int x = getProperty(id, WindowProperty::X).toInt();
            const int y = getProperty(id, WindowProperty::Y).toInt();
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
            const int width = getProperty(id, WindowProperty::Width).toInt();
            const int height = getProperty(id, WindowProperty::Height).toInt();
            return QSize(width, height);
        }
    }
    case WindowProperty::MinimumSize: {
        if (isWidget) {
            return object->property("minimumSize");
        } else {
            const int minimumWidth = getProperty(id, WindowProperty::MinimumWidth).toInt();
            const int minimumHeight = getProperty(id, WindowProperty::MinimumHeight).toInt();
            return QSize(minimumWidth, minimumHeight);
        }
    }
    case WindowProperty::MaximumSize: {
        if (isWidget) {
            return object->property("maximumSize");
        } else {
            const int maximumWidth = getProperty(id, WindowProperty::MaximumWidth).toInt();
            const int maximumHeight = getProperty(id, WindowProperty::MaximumHeight).toInt();
            return QSize(maximumWidth, maximumHeight);
        }
    }
    case WindowProperty::Geometry: {
        if (isWidget) {
            return object->property("geometry");
        } else {
            const QPoint position = getProperty(id, WindowProperty::Position).toPoint();
            const QSize size = getProperty(id, WindowProperty::Size).toSize();
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
        const auto userIcon = qvariant_cast<QIcon>(Core::Settings::get(id, Constants::kTitleBarIconFlag, {}));
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
        const auto winId = qvariant_cast<WId>(getProperty(id, WindowProperty::Id));
        return (winId ? Core::Utils::getDotsPerInch(winId) : Constants::kDefaultScreenDPI);
    }
    case WindowProperty::DevicePixelRatio: {
        const quint32 dpi = getProperty(id, WindowProperty::DotsPerInch).toUInt();
        return qreal(static_cast<qreal>(dpi) / static_cast<qreal>(Constants::kDefaultScreenDPI));
    }
    case WindowProperty::FrameBorderThickness: {
        return getPreferredSystemMetric(id, SystemMetric::FrameBorderThickness, false);
    }
    case WindowProperty::FrameBorderColor: {
        const bool active = getProperty(id, WindowProperty::Active).toBool();
        if (active) {
            const ColorizationArea area = Core::Utils::getColorizationArea();
            if ((area == ColorizationArea::TitleBar_WindowBorder) || (area == ColorizationArea::All)) {
                const auto borderColor = qvariant_cast<QColor>(Core::Settings::get(id, Constants::kFrameBorderColorFlag, {}));
                if (borderColor.isValid()) {
                    return borderColor;
                } else {
                    return Core::Utils::getColorizationColor();
                }
            } else {
                return QColor(Qt::black);
            }
        } else {
            return QColor(Qt::darkGray);
        }
    }
    case WindowProperty::FrameBorderVisibility: {
        return Core::Settings::get(id, Constants::kFrameBorderVisibleFlag, true);
    }
    case WindowProperty::Resizable: {
        const auto flags = qvariant_cast<Qt::WindowFlags>(getProperty(id, WindowProperty::Flags));
        if (flags & Qt::MSWindowsFixedSizeDialogHint) {
            return false;
        }
        const QSize minimumSize = getProperty(id, WindowProperty::MinimumSize).toSize();
        const QSize maximumSize = getProperty(id, WindowProperty::MaximumSize).toSize();
        if (!minimumSize.isEmpty() && !maximumSize.isEmpty() && (minimumSize == maximumSize)) {
            return false;
        }
        return Core::Settings::get(id, Constants::kWindowResizableFlag, true);
    }
    case WindowProperty::Font: {
        // todo
        return {};
    }
    case WindowProperty::Palette: {
        // todo
        return {};
    }
    case WindowProperty::TitleBarHeight: {
        return getPreferredSystemMetric(id, SystemMetric::TitleBarHeight, false);
    }
    case WindowProperty::TitleBarVisibility: {
        return Core::Settings::get(id, Constants::kTitleBarVisibleFlag, true);
    }
    case WindowProperty::TitleTextAlignment: {
        return Core::Settings::get(id, Constants::kTitleBarTextAlignmentFlag, QVariant::fromValue(Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter)));
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

QIcon Core::Window::getSystemButtonIcon(const SystemButton type, const SystemTheme theme)
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
    case SystemButton::Minimize: {
        path.append(QStringLiteral("minimize"));
    } break;
    case SystemButton::Maximize: {
        path.append(QStringLiteral("maximize"));
    } break;
    case SystemButton::Restore: {
        path.append(QStringLiteral("restore"));
    } break;
    case SystemButton::Close: {
        path.append(QStringLiteral("close"));
    } break;
    }
    path.append(QStringLiteral(".svg"));
    initCoreResource();
    return QIcon(path);
}

CUSTOMWINDOW_END_NAMESPACE
