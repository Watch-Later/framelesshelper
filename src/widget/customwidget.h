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
#include <QtGui/qicon.h>
#include <QtWidgets/qwidget.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

class CustomWidgetPrivate;

class CUSTOMWINDOW_API CustomWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(CustomWidget)
    Q_PROPERTY(bool customFrameEnabled READ customFrameEnabled WRITE setCustomFrameEnabled NOTIFY customFrameEnabledChanged)
    Q_PROPERTY(quint32 resizeBorderThickness READ resizeBorderThickness WRITE setResizeBorderThickness NOTIFY resizeBorderThicknessChanged)
    Q_PROPERTY(quint32 captionHeight READ captionHeight WRITE setCaptionHeight NOTIFY captionHeightChanged)
    Q_PROPERTY(quint32 titleBarHeight READ titleBarHeight WRITE setTitleBarHeight NOTIFY titleBarHeightChanged)
    //Q_PROPERTY(QObjectList hitTestVisibleObjects READ hitTestVisibleObjects WRITE setHitTestVisibleObjects NOTIFY hitTestVisibleObjectsChanged)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
    Q_PROPERTY(bool autoDetectHighContrast READ autoDetectHighContrast WRITE setAutoDetectHighContrast NOTIFY autoDetectHighContrastChanged)
    Q_PROPERTY(bool autoDetectColorScheme READ autoDetectColorScheme WRITE setAutoDetectColorScheme NOTIFY autoDetectColorSchemeChanged)
    Q_PROPERTY(bool frameBorderVisible READ frameBorderVisible WRITE setFrameBorderVisible NOTIFY frameBorderVisibleChanged)
    Q_PROPERTY(quint32 frameBorderThickness READ frameBorderThickness WRITE setFrameBorderThickness NOTIFY frameBorderThicknessChanged)
    Q_PROPERTY(QColor frameBorderColor READ frameBorderColor WRITE setFrameBorderColor NOTIFY frameBorderColorChanged)
    Q_PROPERTY(bool titleBarVisible READ titleBarVisible WRITE setTitleBarVisible NOTIFY titleBarVisibleChanged)
    Q_PROPERTY(bool titleBarIconVisible READ titleBarIconVisible WRITE setTitleBarIconVisible NOTIFY titleBarIconVisibleChanged)
    Q_PROPERTY(QIcon titleBarIcon READ titleBarIcon WRITE setTitleBarIcon NOTIFY titleBarIconChanged)
    Q_PROPERTY(Qt::Alignment titleBarTextAlignment READ titleBarTextAlignment WRITE setTitleBarTextAlignment NOTIFY titleBarTextAlignmentChanged)
    Q_PROPERTY(QColor titleBarBackgroundColor READ titleBarBackgroundColor WRITE setTitleBarBackgroundColor NOTIFY titleBarBackgroundColorChanged)

public:
    explicit CustomWidget(QWidget *parent = nullptr);
    virtual ~CustomWidget();

    Q_NODISCARD bool customFrameEnabled() const;
    void setCustomFrameEnabled(const bool value);

    Q_NODISCARD quint32 resizeBorderThickness() const;
    void setResizeBorderThickness(const quint32 value);

    Q_NODISCARD quint32 captionHeight() const;
    void setCaptionHeight(const quint32 value);

    Q_NODISCARD quint32 titleBarHeight() const;
    void setTitleBarHeight(const quint32 value);

    Q_NODISCARD QObjectList hitTestVisibleObjects() const;
    void setHitTestVisible(QWidget *object, const bool visible);

    Q_NODISCARD bool resizable() const;
    void setResizable(const bool value);

    Q_NODISCARD bool autoDetectHighContrast() const;
    void setAutoDetectHighContrast(const bool value);

    Q_NODISCARD bool autoDetectColorScheme() const;
    void setAutoDetectColorScheme(const bool value);

    Q_NODISCARD bool frameBorderVisible() const;
    void setFrameBorderVisible(const bool value);

    Q_NODISCARD quint32 frameBorderThickness() const;
    void setFrameBorderThickness(const quint32 value);

    Q_NODISCARD QColor frameBorderColor() const;
    void setFrameBorderColor(const QColor &value);

    Q_NODISCARD bool titleBarVisible() const;
    void setTitleBarVisible(const bool value);

    Q_NODISCARD bool titleBarIconVisible() const;
    void setTitleBarIconVisible(const bool value);

    Q_NODISCARD QIcon titleBarIcon() const;
    void setTitleBarIcon(const QIcon &value);

    Q_NODISCARD Qt::Alignment titleBarTextAlignment() const;
    void setTitleBarTextAlignment(const Qt::Alignment value);

    Q_NODISCARD QColor titleBarBackgroundColor() const;
    void setTitleBarBackgroundColor(const QColor &value);

Q_SIGNALS:
    void customFrameEnabledChanged(bool);
    void resizeBorderThicknessChanged(quint32);
    void captionHeightChanged(quint32);
    void titleBarHeightChanged(quint32);
    void hitTestVisibleObjectsChanged(QObjectList);
    void resizableChanged(bool);
    void autoDetectHighContrastChanged(bool);
    void autoDetectColorSchemeChanged(bool);
    void frameBorderVisibleChanged(bool);
    void frameBorderThicknessChanged(quint32);
    void frameBorderColorChanged(QColor);
    void titleBarVisibleChanged(bool);
    void titleBarIconVisibleChanged(bool);
    void titleBarIconChanged(QIcon);
    void titleBarTextAlignmentChanged(Qt::Alignment);
    void titleBarBackgroundColorChanged(QColor);

protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif

private:
    QScopedPointer<CustomWidgetPrivate> d_ptr;
};

CUSTOMWINDOW_END_NAMESPACE
