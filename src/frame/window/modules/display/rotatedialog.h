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

#ifndef ROTATEDIALOG_H_V20
#define ROTATEDIALOG_H_V20

#include "../../namespace.h"

#include <QDialog>

namespace dcc {

namespace display {
class Monitor;
class DisplayModel;
}

}

namespace DCC_NAMESPACE {

namespace display {

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RotateDialog(dcc::display::Monitor *mon, QWidget *parent = 0);
    ~RotateDialog();

public:
    void setModel(dcc::display::DisplayModel *model);
Q_SIGNALS:
    void requestRotate(dcc::display::Monitor *mon, const quint16 nextValue);
    void requestRotateAll(const quint16 nextValue);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private:
    void rotate();
private:
    bool m_changed{false};
    dcc::display::DisplayModel *m_model{nullptr};
    dcc::display::Monitor *m_mon{nullptr};
};

} // namespace display

} // namespace dcc

#endif // ROTATEDIALOG_H