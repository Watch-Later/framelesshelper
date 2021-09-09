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

#include "settings.h"
#include <QtCore/qdebug.h>
#include <QtCore/qmutex.h>
#include <QtCore/qvariant.h>
#include <QtCore/quuid.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

struct SettingsData
{
    QMutex mutex = {};
    QHash<QUuid, QVariantHash> optionList = {};
};

Q_GLOBAL_STATIC(SettingsData, globalData)

QUuid Core::Settings::create(const QVariantHash &initialValue)
{
    QVariantHash options = {};
    if (initialValue.isEmpty()) {
        options.insert(QString::fromUtf8(Constants::kCustomWindowFrameFlag), false); // bool
        options.insert(QString::fromUtf8(Constants::kResizeBorderThicknessFlag), 0); // quint32
        options.insert(QString::fromUtf8(Constants::kCaptionHeightFlag), 0); // quint32
        options.insert(QString::fromUtf8(Constants::kTitleBarHeightFlag), 0); // quint32
        options.insert(QString::fromUtf8(Constants::kHitTestVisibleObjectsFlag), {}); // QObjectList
        options.insert(QString::fromUtf8(Constants::kWindowResizableFlag), true); // bool
        options.insert(QString::fromUtf8(Constants::kAutoDetectHighContrastFlag), true); // bool
        options.insert(QString::fromUtf8(Constants::kAutoDetectColorSchemeFlag), true); // bool
        options.insert(QString::fromUtf8(Constants::kFrameBorderVisibleFlag), true); // bool
        options.insert(QString::fromUtf8(Constants::kFrameBorderThicknessFlag), 0); // quint32
        options.insert(QString::fromUtf8(Constants::kFrameBorderColorFlag), {}); // QColor
        options.insert(QString::fromUtf8(Constants::kTitleBarVisibleFlag), false); // bool
        options.insert(QString::fromUtf8(Constants::kTitleBarIconVisibleFlag), false); // bool
        options.insert(QString::fromUtf8(Constants::kTitleBarIconFlag), {}); // QIcon
        options.insert(QString::fromUtf8(Constants::kTitleBarTextAlignmentFlag), {}); // Qt::Alignment
        options.insert(QString::fromUtf8(Constants::kTitleBarBackgroundColorFlag), {}); // QColor
        options.insert(QString::fromUtf8(Constants::kWidgetHandleFlag), {}); // QWidget*
        options.insert(QString::fromUtf8(Constants::kWindowHandleFlag), {}); // QWindow* or QQuickWindow*
    } else {
        options = initialValue;
    }
    const QUuid id = QUuid::createUuid();
    QMutexLocker locker(&globalData()->mutex);
    globalData()->optionList.insert(id, options);
    return id;
}

QVariant Core::Settings::get(const QUuid &id, const QString &name, const QVariant &defaultValue)
{
    Q_ASSERT(!id.isNull());
    Q_ASSERT(!name.isEmpty());
    if (id.isNull() || name.isEmpty()) {
        return defaultValue;
    }
    QMutexLocker locker(&globalData()->mutex);
    if (globalData()->optionList.isEmpty()) {
        return defaultValue;
    }
    if (!globalData()->optionList.contains(id)) {
        return defaultValue;
    }
    const QVariantHash options = globalData()->optionList.value(id);
    if (options.isEmpty()) {
        return defaultValue;
    }
    return (options.contains(name) ? options.value(name) : defaultValue);
}

bool Core::Settings::set(const QUuid &id, const QString &name, const QVariant &value)
{
    Q_ASSERT(!id.isNull());
    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(value.isValid());
    if (id.isNull() || name.isEmpty() || !value.isValid()) {
        return false;
    }
    QMutexLocker locker(&globalData()->mutex);
    if (globalData()->optionList.isEmpty()) {
        return false;
    }
    if (!globalData()->optionList.contains(id)) {
        return false;
    }
    QVariantHash options = globalData()->optionList.take(id);
    if (options.contains(name)) {
        options.remove(name);
    }
    options.insert(name, value);
    globalData()->optionList.insert(id, options);
    return true;
}

CUSTOMWINDOW_END_NAMESPACE
