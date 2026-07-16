/// @file netcolordialog.cpp
/// @brief Dialog for setting the color of a wire (implementation)
/// @author Andrés Martínez Mera - andresmmera@protonmail.com
/// @date July 17, 2026
/// @copyright Copyright (C) 2026 Andrés Martínez Mera
/// @license GPL-3.0-or-later

#include "netcolordialog.h"
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

NetColorDialog::NetColorDialog(const QColor& initialColor, QWidget *parent)
    : QDialog(parent), netcolor(initialColor)
{
  setWindowTitle(tr("Net Style"));

  QHBoxLayout *row = new QHBoxLayout;
  row->addWidget(new QLabel(tr("Net color:"), this));

  chooseColorBtn = new QPushButton();
  connect(chooseColorBtn, &QPushButton::clicked, this, &NetColorDialog::slotChooseColor);
  updateColorButtonBackground(netcolor); // Set the background color of the color-picking button

  row->addWidget(chooseColorBtn);

  QPushButton *defaultBtn = new QPushButton(tr("Use Default"), this);
  connect(defaultBtn, &QPushButton::clicked, this, &NetColorDialog::slotUseDefaultColor);
  row->addWidget(defaultBtn);
  row->addStretch();

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addLayout(row);
  layout->addWidget(buttonBox);
}

void NetColorDialog::slotChooseColor()
{
  QColor initial_color;

  if (netcolor.isValid()){
    initial_color = netcolor;
  } else {
    // Default color
    initial_color = Qt::darkBlue;
  }
  QColor chosen = QColorDialog::getColor(initial_color, this, tr("Select Net Color"));
  if (chosen.isValid()) {
    netcolor = chosen;
    // Update the background color of the color-picking button
    updateColorButtonBackground(netcolor);
  }
}

void NetColorDialog::slotUseDefaultColor()
{
  // Default color
  netcolor = Qt::darkBlue;

  // Update the background color of the color-picking button
  updateColorButtonBackground(netcolor);
}


void NetColorDialog::updateColorButtonBackground(QColor background_color){
  QString styleSheet = QStringLiteral("QPushButton { background-color: %1; }")
  .arg(background_color.name());
  chooseColorBtn->setStyleSheet(styleSheet);
  chooseColorBtn->setAttribute(Qt::WA_TranslucentBackground);
}
