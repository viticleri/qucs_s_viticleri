#include "dcbiaslabel.h"

#include <cassert>

namespace testShouldSuppressDcBiasLabel {

void run() {
  const QRect labelRect(0, 0, 20, 10);
  const QRect overlappingRect(5, 5, 20, 20);
  const QRect farRect(500, 500, 20, 20);

  assert(shouldSuppressDcBiasLabel(QStringLiteral("VProbe"), labelRect,
                                   overlappingRect));
  assert(!shouldSuppressDcBiasLabel(QStringLiteral("R"), labelRect,
                                    overlappingRect));
  assert(!shouldSuppressDcBiasLabel(QStringLiteral("IProbe"), labelRect,
                                    overlappingRect));
  assert(
      !shouldSuppressDcBiasLabel(QStringLiteral("VProbe"), labelRect, farRect));
}

} // namespace testShouldSuppressDcBiasLabel

int main() {
  testShouldSuppressDcBiasLabel::run();
  return 0;
}
