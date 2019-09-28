/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     lq <longqi_cm@deepin.com>
 *
 * Maintainer: lq <longqi_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DISPLAYWIDGET_H_V20
#define DISPLAYWIDGET_H_V20

#include "window/namespace.h"

#include <DFloatingButton>
#include <DListView>

#include <QModelIndex>
#include <QPoint>

QT_BEGIN_NAMESPACE
class QListView;
class QStandardItemModel;
class QVBoxLayout;
QT_END_NAMESPACE

namespace dcc {

namespace display {
class DisplayModel;
}

}

namespace DCC_NAMESPACE {

namespace display {

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = nullptr);

public:
    void setModel(dcc::display::DisplayModel *model);
    void showPath(const QString &path);
    QPoint getRotateBtnPos();

private Q_SLOTS:
    void onMonitorListChanged();
    void onMenuClicked(const QModelIndex &);

public:
    static int convertToSlider(const float value);
    static float convertToScale(const int value);

Q_SIGNALS:
    void requestShowMultiScreenPage() const;
    void requestShowResolutionPage() const;
    void requestShowBrightnessPage() const;
    void requestShowScalingPage() const;
    void requestShowCustomConfigPage() const;
    void requestRotate() const;
    void requestShowRefreshRatePage() const;

private:
    void initMenuUI();

private:
    struct MenuMethod {
        QString menuText;
        QString iconName;
        QMetaMethod method;
    };
private:
    dcc::display::DisplayModel *m_model{nullptr};
    DTK_WIDGET_NAMESPACE::DFloatingButton *m_rotate{nullptr};
    QVBoxLayout *m_centralLayout{nullptr};
    DTK_WIDGET_NAMESPACE::DListView *m_menuList{nullptr};
    QModelIndex m_currentIdx;

    QStandardItemModel *m_multiModel{nullptr};
    QStandardItemModel *m_singleModel{nullptr};

    QList<MenuMethod> m_multMenuList;
    QList<MenuMethod> m_singleMenuList;

    bool m_isMultiScreen{true};
};

}  // namespace display

}  // namespace dcc

#endif  // DISPLAYWIDGET_H_V20