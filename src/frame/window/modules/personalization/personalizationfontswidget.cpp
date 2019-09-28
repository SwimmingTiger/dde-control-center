/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     LiLinling <lilinling_cm@deepin.com>
 *
 * Maintainer: LiLinling <lilinling_cm@deepin.com>
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
#include "personalizationfontswidget.h"

#include "widgets/titledslideritem.h"
#include "widgets/dccslider.h"
#include "widgets/settingsitem.h"
#include "modules/personalization/personalizationmodel.h"
#include "modules/personalization/model/fontsizemodel.h"
#include "modules/personalization/model/fontmodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QStandardItemModel>
#include <QDebug>

#include <DSlider>

using namespace DCC_NAMESPACE;
using namespace DCC_NAMESPACE::personalization;
using namespace dcc::widgets;

PersonalizationFontsWidget::PersonalizationFontsWidget(QWidget *parent)
    : QWidget(parent)
    , m_centralLayout(new QVBoxLayout())
    //~ contents_path /personalization/Font
    , m_fontSizeSlider(new TitledSliderItem(tr("Size")))
    , m_standardFontsCbBox(new QComboBox(this))
    , m_monoFontsCbBox(new QComboBox(this))
    , m_isAppend(false)
{
    m_centralLayout->setMargin(10);
    m_centralLayout->setSpacing(20);
    //font size
    m_fontSizeSlider->addBackground();
    m_fontSizeSlider->setObjectName("fontsizeslider");
    m_fontSizeSlider->slider()->setOrientation(Qt::Horizontal);

    DCCSlider *slider = m_fontSizeSlider->slider();
    QStringList annotions;
    annotions << "11" << "12" << "13" << "15" << "16" << "18" << "20";
    m_fontSizeSlider->setAnnotations(annotions);
    slider->setRange(0, 6);
    slider->setType(DCCSlider::Vernier);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);
    slider->setPageStep(1);

    m_centralLayout->addWidget(m_fontSizeSlider);

    QVBoxLayout *fontsLayout  = new QVBoxLayout;
    fontsLayout->setSpacing(10);
    //standard font
    QHBoxLayout *sfontLayout = new QHBoxLayout();
    SettingsItem *sfontitem = new SettingsItem;
    sfontitem->addBackground();
    sfontitem->setLayout(sfontLayout);
    //~ contents_path /personalization/Font
    sfontLayout->addWidget(new QLabel(tr("Standard Font"), this));
    sfontLayout->addWidget(m_standardFontsCbBox);

    m_standardFontsCbBox->setModel(new QStandardItemModel(this));
    m_standardFontsCbBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    fontsLayout->addWidget(sfontitem);

    //mono font
    QHBoxLayout *mfontLayout = new QHBoxLayout();
    SettingsItem *mfontitem = new SettingsItem;
    mfontitem->addBackground();
    mfontitem->setLayout(mfontLayout);
    //~ contents_path /personalization/Font
    mfontLayout->addWidget(new QLabel(tr("Monospaced Font"), this));
    mfontLayout->addWidget(m_monoFontsCbBox);
    m_monoFontsCbBox->setModel(new QStandardItemModel(this));
    m_monoFontsCbBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    fontsLayout->addWidget(mfontitem);
    m_centralLayout->addLayout(fontsLayout);
    m_centralLayout->addStretch();
    setLayout(m_centralLayout);

    connect(slider, &DCCSlider::valueChanged, this, &PersonalizationFontsWidget::requestSetFontSize);
}

void PersonalizationFontsWidget::setModel(dcc::personalization::PersonalizationModel *const model)
{
    m_model = model;

    //font size
    connect(m_model->getFontSizeModel(), &dcc::personalization::FontSizeModel::sizeChanged, this, &PersonalizationFontsWidget::setFontSize);
    setFontSize(m_model->getFontSizeModel()->getFontSize());

    //standard font & mono font
    dcc::personalization::FontModel *standmodel = model->getStandFontModel();
    dcc::personalization::FontModel *monomodel  = model->getMonoFontModel();
    //set fonts when they are already available
    if (standmodel->getFontList().size() > 0)
        setList(standmodel->getFontList(), standmodel);

    if (monomodel->getFontList().size() > 0)
        setList(monomodel->getFontList(), monomodel);

    connect(standmodel, &dcc::personalization::FontModel::defaultFontChanged, this,
            reinterpret_cast<void (PersonalizationFontsWidget::*)(const QString &)>(&PersonalizationFontsWidget::onDefaultFontChanged));
    connect(monomodel, &dcc::personalization::FontModel::defaultFontChanged, this,
            reinterpret_cast<void (PersonalizationFontsWidget::*)(const QString &)>(&PersonalizationFontsWidget::onDefaultFontChanged));

    connect(standmodel, &dcc::personalization::FontModel::listChanged, this,
            reinterpret_cast<void (PersonalizationFontsWidget::*)(const QList<QJsonObject> &)>(&PersonalizationFontsWidget::setList));
    connect(monomodel, &dcc::personalization::FontModel::listChanged, this,
            reinterpret_cast<void (PersonalizationFontsWidget::*)(const QList<QJsonObject> &)>(&PersonalizationFontsWidget::setList));

    connect(m_standardFontsCbBox, &QComboBox::currentTextChanged, this, &PersonalizationFontsWidget::onSelectChanged);
    connect(m_monoFontsCbBox, &QComboBox::currentTextChanged, this, &PersonalizationFontsWidget::onSelectChanged);
}

void PersonalizationFontsWidget::setFontSize(int size)
{
    m_fontSizeSlider->blockSignals(true);
    m_fontSizeSlider->slider()->setValue(size);
    m_fontSizeSlider->blockSignals(false);
}

void PersonalizationFontsWidget::setList(const QList<QJsonObject> &list, dcc::personalization::FontModel *model)
{
    QStandardItemModel *fontModel{nullptr};
    if (sender())
        model = qobject_cast<dcc::personalization::FontModel *>(sender());

    QComboBox *combox{nullptr};
    combox = (model == m_model->getStandFontModel()) ? m_standardFontsCbBox : m_monoFontsCbBox;
    fontModel = dynamic_cast<QStandardItemModel *>(combox->model());

    //clear the model
    fontModel->clear();
    m_isAppend = true;
    for (QJsonObject item : list) {
        QString name = item["Name"].toString();
        QStandardItem *modelItem = new QStandardItem(name);
        modelItem->setFont(QFont(name));
        fontModel->appendRow(modelItem);
    }
    m_isAppend = false;

    onDefaultFontChanged(model->getFontName(), model);
}

void PersonalizationFontsWidget::onSelectChanged(const QString &name)
{
    // combobox appends data, ignoring signal currentTextChanged
    if (m_isAppend)
        return;

    auto combox  = qobject_cast<QComboBox *>(sender());
    auto list = (combox == m_standardFontsCbBox) ? m_model->getStandFontModel()->getFontList() : m_model->getMonoFontModel()->getFontList();

    for (QJsonObject obj : list) {
        if (obj["Name"].toString() == name) {
            Q_EMIT requestSetDefault(obj);
            return;
        }
    }
}

void PersonalizationFontsWidget::onDefaultFontChanged(const QString &name, dcc::personalization::FontModel *sender)
{
    if (this->sender())
        sender = qobject_cast<dcc::personalization::FontModel *>(this->sender());
    auto comboBox = (sender == m_model->getMonoFontModel()) ? m_monoFontsCbBox : m_standardFontsCbBox;

    for (const QJsonObject &obj : sender->getFontList()) {
        if (obj["Id"].toString() == name) {
            comboBox->setCurrentText(name);
            return;
        }
    }

    comboBox->setCurrentText(sender->getFontName() + tr(" (Unsupported font)"));
}