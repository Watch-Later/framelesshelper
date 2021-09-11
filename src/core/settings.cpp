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
#include <QtCore/qdebug.h>
#include <QtCore/qmutex.h>
#include <QtCore/qvariant.h>
#include <QtCore/quuid.h>
#include <QtGui/qpalette.h>
#include <QtGui/qicon.h>

CUSTOMWINDOW_BEGIN_NAMESPACE

struct CustomPalette
{
    QPalette light = {};
    QPalette dark = {};
    QPalette highContrast = {};
};

struct CustomSystemButton
{
    QIcon light = {};
    QIcon dark = {};
    QIcon highContrast = {};
};

struct SettingsData
{
    QHash<QUuid, QVariantHash *> optionList = {};
    CustomPalette customPalette = {};
    CustomSystemButton customMinimizeButton = {};
    CustomSystemButton customMaximizeButton = {};
    CustomSystemButton customRestoreButton = {};
    CustomSystemButton customCloseButton = {};
};

Q_GLOBAL_STATIC(SettingsData, globalData)

static QMutex mutex = {};

QUuid Core::Settings::create(const QVariantHash &initialValue)
{
    const auto options = new QVariantHash;
    if (initialValue.isEmpty()) {
        options->insert(Constants::kCustomWindowFrameFlag, false); // bool
        options->insert(Constants::kResizeBorderThicknessFlag, 0); // quint32
        options->insert(Constants::kCaptionHeightFlag, 0); // quint32
        options->insert(Constants::kTitleBarHeightFlag, 0); // quint32
        options->insert(Constants::kHitTestVisibleObjectsFlag, {}); // QObjectList
        options->insert(Constants::kWindowResizableFlag, true); // bool
        options->insert(Constants::kAutoDetectHighContrastFlag, true); // bool
        options->insert(Constants::kAutoDetectColorSchemeFlag, true); // bool
        options->insert(Constants::kFrameBorderVisibleFlag, true); // bool
        options->insert(Constants::kFrameBorderThicknessFlag, 0); // quint32
        options->insert(Constants::kFrameBorderColorFlag, {}); // QColor
        options->insert(Constants::kTitleBarVisibleFlag, true); // bool
        options->insert(Constants::kTitleBarIconVisibleFlag, true); // bool
        options->insert(Constants::kTitleBarIconFlag, {}); // QIcon
        options->insert(Constants::kTitleBarTextAlignmentFlag, QVariant::fromValue(Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter))); // Qt::Alignment
        options->insert(Constants::kTitleBarBackgroundColorFlag, {}); // QColor
        options->insert(Constants::kWidgetHandleFlag, {}); // QWidget*
        options->insert(Constants::kWindowHandleFlag, {}); // QWindow* or QQuickWindow*
    } else {
        *options = initialValue;
    }
    const QUuid id = QUuid::createUuid();
    QMutexLocker locker(&mutex);
    globalData()->optionList.insert(id, options);
    return id;
}

bool Core::Settings::destroy(const QUuid &id)
{
    Q_ASSERT(!id.isNull());
    if (id.isNull()) {
        return false;
    }
    QMutexLocker locker(&mutex);
    if (globalData()->optionList.isEmpty()) {
        return false;
    }
    if (!globalData()->optionList.contains(id)) {
        return false;
    }
    const auto options = globalData()->optionList.value(id);
    if (options) {
        delete options;
    }
    globalData()->optionList.remove(id);
    return true;
}

QVariant Core::Settings::get(const QUuid &id, const QString &name, const QVariant &defaultValue)
{
    Q_ASSERT(!id.isNull());
    Q_ASSERT(!name.isEmpty());
    if (id.isNull() || name.isEmpty()) {
        return defaultValue;
    }
    QMutexLocker locker(&mutex);
    if (globalData()->optionList.isEmpty()) {
        return defaultValue;
    }
    if (!globalData()->optionList.contains(id)) {
        return defaultValue;
    }
    const auto options = globalData()->optionList.value(id);
    if (!options) {
        return defaultValue;
    }
    if (options->isEmpty()) {
        return defaultValue;
    }
    return (options->contains(name) ? options->value(name) : defaultValue);
}

bool Core::Settings::set(const QUuid &id, const QString &name, const QVariant &value)
{
    Q_ASSERT(!id.isNull());
    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(value.isValid());
    if (id.isNull() || name.isEmpty() || !value.isValid()) {
        return false;
    }
    QMutexLocker locker(&mutex);
    if (globalData()->optionList.isEmpty()) {
        return false;
    }
    if (!globalData()->optionList.contains(id)) {
        return false;
    }
    const auto options = globalData()->optionList.value(id);
    if (!options) {
        return false;
    }
    options->insert(name, value);
    return true;
}

CUSTOMWINDOW_END_NAMESPACE
