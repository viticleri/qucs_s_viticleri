#ifndef DCBIASLABEL_H
#define DCBIASLABEL_H

#include <QRect>
#include <QString>

bool shouldSuppressDcBiasLabel(const QString& componentModel,
                               const QRect& labelRect,
                               const QRect& componentRect);

#endif
