/// @file netpropertiesdialog.h
/// @brief Dialog for setting the style and the name of a net (class definition)
/// @author Andrés Martínez Mera - andresmmera@protonmail.com
/// @date July 25, 2026
/// @copyright Copyright (C) 2026 Andrés Martínez Mera
/// @license GPL-3.0-or-later

#ifndef NETPROPERTIESDIALOG_H
#define NETPROPERTIESDIALOG_H

#include <QDialog>
#include <QColor>
#include <QLineEdit>
#include <QSpinBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

/// @class NetPropertiesDialog
/// @brief Dialog for setting the style and name of a net
class NetPropertiesDialog : public QDialog
{
  Q_OBJECT
public:
  /// @brief Class constructor
  NetPropertiesDialog(const QColor& initialColor, int initialLineWidth,
                      const QString& initialName, const QString& initialValue,
                      QWidget *parent = nullptr);

         /// @brief Class destructor
  ~NetPropertiesDialog() {}

         /// @brief Get the color of the wire
         /// @return QColor variable
  QColor resultColor() const { return m_netcolor; }

         /// @brief Get the witdth of the wire
         /// @return int variable
  int resultLineWidth() const { return m_lineWidth; }

  QString resultName() const { return NodeName->text().trimmed(); }
  QString resultInitValue() const { return InitValue->text().trimmed(); }

private slots:
  /// @brief Opens up the color-picking dialog for color selection
  /// @details It also updates the background color of the color-picking button
  void slotChooseColor();

  /// @brief Set the default style of the net (Qt::darkBlue + linewidth = 2)
  /// @details It also updates the background color of the color-picking button
  void slotUseDefaultStyle();

  void slotOk();
  void slotCancel();

private:
  /// Color of the net (wire or node)
  QColor m_netcolor;

  /// Linewidth
  QSpinBox *m_widthSpin;
  int m_lineWidth;

  /// Button for color picking
  QPushButton *chooseColorBtn;

  /// @brief Helper for setting the background color of the color-picking button
  void updateColorButtonBackground(QColor background_color);

  /// Label
  QLineEdit *NodeName, *InitValue;
  QRegularExpressionValidator *Validator1, *Validator2;
  QRegularExpression Expr1, Expr2;
};

#endif