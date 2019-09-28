/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     justforlxz <justforlxz@outlook.com>
 *
 * Maintainer: justforlxz <justforlxz@outlook.com>
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
#pragma once

#include "window/namespace.h"

#include "modules/sync/utils.h"

#include <DListView>

#include <QWidget>
#include <QMap>

#include <utility>

QT_BEGIN_NAMESPACE
class QListView;
class QVBoxLayout;
class QStandardItemModel;
class QStandardItem;
class QLabel;
QT_END_NAMESPACE

namespace dcc {
namespace widgets {
class SwitchWidget;
}
namespace accounts {
class AvatarWidget;
}

namespace cloudsync {
class SyncModel;
class SyncStateIcon;
//enum SyncType : int;
//enum SyncState : int;
}
}

namespace DCC_NAMESPACE {
namespace sync {
class DownloadUrl;
class IndexPage : public QWidget
{
    Q_OBJECT
public:
    IndexPage(QWidget *parent = nullptr);
    void setModel(dcc::cloudsync::SyncModel *model);
    ~IndexPage();

Q_SIGNALS:
    void requestSetAutoSync(bool enable) const;
    void requestLogout() const;
    void requestSetModuleState(std::pair<dcc::cloudsync::SyncType, bool> state);

private:
    void onListViewClicked(const QModelIndex &index);
    void onUserInfoChanged(const QVariantMap &infos);
    void onStateChanged(const std::pair<qint32, QString> &state);
    void onLastSyncTimeChanged(const qlonglong lastSyncTime);
    void onModuleStateChanged(std::pair<dcc::cloudsync::SyncType, bool> state);
    void onModuleItemSwitched(const bool checked);
    void onAutoSyncChanged(bool autoSync);

private:
    QVBoxLayout *m_mainLayout;
    dcc::cloudsync::SyncModel *m_model;
    dcc::accounts::AvatarWidget *m_avatar;
    QLabel *m_username;
    dcc::widgets::SwitchWidget *m_autoSyncSwitch;
    DTK_WIDGET_NAMESPACE::DListView *m_listView;
    dcc::cloudsync::SyncStateIcon *m_stateIcon;
    QLabel *m_stateLbl;
    QLabel *m_lastSyncTimeLbl;
    QStandardItemModel *m_listModel;
    QMap<dcc::cloudsync::SyncType, QStandardItem *> m_itemMap;
    DownloadUrl *m_downloader;
    QString m_avatarPath;
};
} // namespace sync
} // namespace DCC_NAMESPACE