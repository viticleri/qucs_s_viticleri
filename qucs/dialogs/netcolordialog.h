/// @file netcolordialog.h
/// @brief Dialog for setting the color of a wire (class definition)
/// @author Andrés Martínez Mera - andresmmera@protonmail.com
/// @date July 17, 2026
/// @copyright Copyright (C) 2026 Andrés Martínez Mera
/// @license GPL-3.0-or-later

#ifndef NETCOLORDIALOG_H
#define NETCOLORDIALOG_H

#include <QDialog>
#include <QColor>
#include <QSpinBox>

class QFrame;

/// @class NetColorDialog
/// @brief Dialog for setting the color of a wire
class NetColorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit NetColorDialog(const QColor& initialColor, int initialLineWidth, QWidget *parent = nullptr);

  /// @brief Get the color of the wire
  /// @return QColor variable
  QColor resultColor() const { return netcolor; }

  /// @brief Get the witdth of the wire
  /// @return int variable
  int resultLineWidth() const { return m_lineWidth; }

private slots:
  /// @brief Opens up the color-picking dialog for color selection
  /// @details It also updates the background color of the color-picking button
  void slotChooseColor();

  /// @brief Set the default color of the net (Qt::darkBlue)
  /// @details It also updates the background color of the color-picking button
  void slotUseDefaultColor();

private:
  /// Color of the net (wire or node)
  QColor netcolor;

  /// Linewidth
  QSpinBox *m_widthSpin;
  int m_lineWidth;

  /// Button for color piching
  QPushButton *chooseColorBtn;

  /// @brief Helper for setting the background color of the color-picking button
  void updateColorButtonBackground(QColor background_color);
};

#endif
