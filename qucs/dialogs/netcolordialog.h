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

class QFrame;

/// @class NetColorDialog
/// @brief Dialog for setting the color of a wire
class NetColorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit NetColorDialog(const QColor& initialColor, QWidget *parent = nullptr);

  /// @brief Get the color of the wire
  /// @return QColor variable
  QColor resultColor() const { return netcolor; }

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

  /// Button for color piching
  QPushButton *chooseColorBtn;

  /// @brief Helper for setting the background color of the color-picking button
  void updateColorButtonBackground(QColor background_color);
};

#endif
