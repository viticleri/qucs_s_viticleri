/// @file schematicvalidator.h
/// @brief Schematic validation class (definition)
/// @author Andrés Martínez Mera
/// @date July 12, 2026

#ifndef SCHEMATICVALIDATOR_H
#define SCHEMATICVALIDATOR_H

#include "wire.h"
#include "node.h"

#include <QString>
#include <QVector>

class Schematic;

/// @brief A single problem found in a schematic
/// @param message Human-readable description for the user
/// @param severity Relevance of the issue
///        1: Critical - The simulation backend will fail
///        2: Warning  - Simulation may proceed, but care must be taken.
///        3: Minor    - Not relevant
/// @param suggestedFix Message containing with a suggestion about how to solve the problem
struct ValidationIssue
{
  QString message;
  QString suggestedFix;
  int severity;
};

/// @class SchematicValidator
/// @brief Identify issues in an schematic given a simulation backend.
/// @details  Validation runs before the simulation is started, so the user
/// is warned about potential incompatibilities
class SchematicValidator
{
public:
  /// @brief Run all checks against an schematic given a certain simulation backend,
  /// e.g. qucsator-RF, ngspice, xyce
  QVector<ValidationIssue> validate() const;

  /// @brief Set simulation backend
  /// @param SimulationBackend Name of the backend simulator
  /// @details Used by Qucs-S main app to set the simulation backend for the validation
  void setSimulationBackend(QString SimulationBackend) {backend = SimulationBackend;}

  /// @brief Set the schematic to validate
  /// @param Schematic Schematic object
  /// @details Used by Qucs-S main app to set the schematic under validation
  void setSchematic(Schematic *Schematic){sch = Schematic;}

private:

  Schematic *sch;
  QString backend; /// Simulation backend

  /// Checks @{

  /// @brief SP/AC frequency sweep must not be a list if the simulation backend is ngspice or xyce
  /// @return A list of ValidationIssue entries..
  QVector<ValidationIssue> checkFrequencySweepType() const;

  /// @brief ngspice needs at least two AC power sources in SP simulation
  /// @return A list of ValidationIssue entries.
  QVector<ValidationIssue> checkMinimumPortsInSPSimulation() const;

  /// @brief Simulation backends require a simulation block
  /// @return A list of ValidationIssue entries.
  QVector<ValidationIssue> checkMissingSimulation() const;

  /// @brief Check if the schematic contain wires with at least one endpoint open
  /// @details Iterates over every wire in the schematic and inspects both of its endpoints (Port1 and Port2).
  /// An endpoint is considered "open" when it is not shared with any other wire and no component pin is attached to it.
  /// @return A list of ValidationIssue entries, one per unconnected port found.
  QVector<ValidationIssue> checkDanglingWires() const;

  /// @brief Checks whether a net reaches any component other than a given owner.
  /// @param node  The starting node of the net to traverse. May be null, in which case the function returns false.
  /// @param owner The component that should be excluded from the search.
  /// @return true if at least one component other than @a owner is reachable from @a node via wires; false if the net is empty,
  /// dangling, or only reaches @a owner itself.
  /// @details Starting from @a node, performs a hops from node to node across every wire segment. At each node visited, all attached components are
  /// inspected. This is used to detect ports that only appear connected because they touch a single wire, when in fact that wire (or chain of wires) leads
  /// nowhere
  bool netReachesOtherComponent(Node *node, Component *owner) const;

  /// @brief Detect subcircuit/component ports that have no wire or net attached
  /// @return A list of ValidationIssue entries, one per unconnected port found.
  QVector<ValidationIssue> checkUnconnectedPorts() const;

  /// @}
};

#endif // SCHEMATICVALIDATOR_H
