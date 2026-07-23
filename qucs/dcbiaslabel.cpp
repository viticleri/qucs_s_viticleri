#include "dcbiaslabel.h"

bool shouldSuppressDcBiasLabel(const QString& componentModel,
                               const QRect& labelRect,
                               const QRect& componentRect) {
  if (componentModel != QLatin1String("VProbe")) {
    return false;
  }
  return labelRect.intersects(componentRect);
}
