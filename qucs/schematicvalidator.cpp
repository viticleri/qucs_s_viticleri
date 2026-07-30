/// @file schematicvalidator.cpp
/// @brief Schematic validation class (implementation)
/// @author Andrés Martínez Mera
/// @date July 12, 2026

#include "schematicvalidator.h"
#include "schematic.h"
#include "components/component.h"

QVector<ValidationIssue> SchematicValidator::validate() const
{
  QVector<ValidationIssue> issues;

  issues += checkFrequencySweepType();
  issues += checkMinimumPortsInSPSimulation();
  issues += checkMissingSimulation();
  issues += checkDanglingWires();

  return issues;
}

QVector<ValidationIssue> SchematicValidator::checkFrequencySweepType() const
{
  QVector<ValidationIssue> issues; // It may be several SP/AC blocks

  if (backend.toLower() == "qucsator")
    return issues; // qucsator handles list sweeps fine

  for (Component *component : sch->a_DocComps) {
    if (!component->isActive)
      continue;

    bool isFrequencySweepBlock = (component->Model == ".AC" || component->Model == ".SP");
    if (!isFrequencySweepBlock)
      continue;

    Property *sweepType = component->getProperty("Type");
    bool usesListSweep = sweepType && sweepType->Value == "list";
    if (usesListSweep) {
      ValidationIssue issue;

      // Error message
      issue.message = QObject::tr("%1 uses a 'list' frequency sweep, which %2 does not support.")
                          .arg(component->Name, backend);

      // Issue relevance
      issue.severity = 1; // Critical - Simulation will fail

      // Suggested solution
      issue.suggestedFix = QObject::tr("Use lin or log frequency sweep in %1").arg(component->Name);
      issues.append(issue);
    }
  }

  return issues;
}


QVector<ValidationIssue> SchematicValidator::checkMinimumPortsInSPSimulation() const {
  QVector<ValidationIssue> issues;

  if (backend.toLower() != "ngspice")
    return issues; // only ngspice has this restriction


  bool hasActiveSPBlock = false;
  for (Component *component : sch->a_DocComps) {
    if (component->isActive && component->Model == ".SP") {
      hasActiveSPBlock = true;
      break;
    }
  }
  if (!hasActiveSPBlock)
    return issues; // no S-parameter simulation, nothing to check

  int acSourceCount = 0;
  for (Component *component : sch->a_DocComps) {
    if (!component->isActive) {
      // Ignore deactivated components
      continue;
    }

    if (component->Model.startsWith("Pac")) {
      // AC power source found. They start with VP for ngspice
      acSourceCount++;
    }
  }

  if (acSourceCount < 2) {
    ValidationIssue issue;
    issue.message = QObject::tr(
                        "The schematic has %1 AC power source."
                        "ngspice requires at least 2 for S-parameter analysis.").arg(acSourceCount);
    issue.severity = 1; // Critical - simulation will fail
    issue.suggestedFix = QObject::tr(
        "If this is a 1-port SP simulation, add another AC Power souce component with the negative port connected to GND");
    issues.append(issue);
  }

  return issues;
}

QVector<ValidationIssue> SchematicValidator::checkMissingSimulation() const
{
  QVector<ValidationIssue> issues;

  // Model strings for all recognized simulation controller blocks.
  static const QStringList kSimulationBlockModels = {
      ".AC", ".SP", ".TR", ".DC", ".HB", ".SW", ".NOISE", ".TF"
  };

  for (Component *component : sch->a_DocComps) {
    if (component->isActive && kSimulationBlockModels.contains(component->Model))
      // Found one block. It's ok
      return issues;
  }

  ValidationIssue issue;
  issue.message = QObject::tr(
      "The schematic does not contain any active simulation block.");
  issue.severity = 1; // Critical - nothing to simulate
  issue.suggestedFix = QObject::tr(
      "Add a simulation block (e.g. .AC, .SP, .TR, .DC) to the schematic.");

  issues.append(issue);

  return issues;
}

QVector<ValidationIssue> SchematicValidator::checkDanglingWires() const
{
  QVector<ValidationIssue> issues;
  // Inspect all wires
  for (Wire *wire : sch->a_DocWires) {
    Node *endpoint1 = wire->Port1;
    Node *endpoint2 = wire->Port2;

    bool endpoint1_isOpen = endpoint1->wires().size() <= 1
                            && endpoint1->components().empty();
    bool endpoint2_isOpen = endpoint2->wires().size() <= 1
                            && endpoint2->components().empty();

    if (endpoint1_isOpen && endpoint2_isOpen) {
      // Both ends open: the wire is entirely disconnected
      ValidationIssue issue;
      issue.message = QObject::tr(
                           "Wire is not connected to anything at either end (near (%1, %2) and (%3, %4)).")
                           .arg(endpoint1->cx).arg(endpoint1->cy)
                           .arg(endpoint2->cx).arg(endpoint2->cy);
      issue.severity = 3; // Minor, but the user should review it. In many cases this will be something merely aesthetic, but
      // it may happen that the user forgot to connect something
      issue.suggestedFix = QObject::tr(
          "Remove the wire.");
      issues.append(issue);
    }
    else if (endpoint1_isOpen) {
      // First end open
      ValidationIssue issue;

      QString message;
      if (endpoint1->hasLabel()){
        message = QObject::tr("Wire end at net '%1' is not connected to anything.").arg(endpoint1->Name);
      } else {
        message = QObject::tr("Wire has an unconnected end near (%1, %2).").arg(endpoint1->cx).arg(endpoint1->cy);
      }

      issue.message = message;
      issue.severity = 3;
      issue.suggestedFix = QObject::tr(
          "Terminate the open end or remove the dangling wire.");
      issues.append(issue);
    }
    else if (endpoint2_isOpen) {
      // Second end open
      ValidationIssue issue;

      QString message;
      if (endpoint2->hasLabel()){
        message = QObject::tr("Wire end at net '%1' is not connected to anything.").arg(endpoint2->Name);
      } else {
        message = QObject::tr("Wire has an unconnected end near (%1, %2).").arg(endpoint2->cx).arg(endpoint2->cy);
      }

      issue.message = message;
      issue.severity = 3;
      issue.suggestedFix = QObject::tr(
          "Terminate the open end or remove the dangling wire.");
      issues.append(issue);
    }
  }
  return issues;
}