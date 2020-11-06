/*
 * MemMapEquation.h
 *
 *  Created on: 4 mar 2015
 *      Author: Daniel
 */

#ifndef _MEMMAPEQUATION_H_
#define _MEMMAPEQUATION_H_

#include "MapEquation.h"
#include <vector>
#include <map>

namespace infomap {

class InfoNode;
struct MemDeltaFlow;

namespace detail { struct MemNodeSet; }

class MemMapEquation final : public MapEquation {
  using Base = MapEquation;

public:
  using DeltaFlowDataType = MemDeltaFlow;

  // ===================================================
  // Getters
  // ===================================================

  static bool haveMemory() noexcept { return true; }

  // ===================================================
  // IO
  // ===================================================

  std::ostream& print(std::ostream& out) const noexcept final;

  friend std::ostream& operator<<(std::ostream&, const MemMapEquation&);

  // ===================================================
  // Init
  // ===================================================

  void init(const Config& config) noexcept final;

  void initNetwork(InfoNode& root) noexcept final;

  void initPartition(std::vector<InfoNode*>& nodes) noexcept final;

  // ===================================================
  // Codelength
  // ===================================================

  double calcCodelength(const InfoNode& parent) const final;

  // We need to insert the base class declarations so the derived class
  // can over load them with different DeltaFlowDataTypes
  using Base::addMemoryContributions;
  using Base::getDeltaCodelengthOnMovingNode;
  using Base::updateCodelengthOnMovingNode;

  void addMemoryContributions(InfoNode& current, DeltaFlowDataType& oldModuleDelta, VectorMap<DeltaFlowDataType>& moduleDeltaFlow);

  double getDeltaCodelengthOnMovingNode(InfoNode& current,
                                        DeltaFlowDataType& oldModuleDelta,
                                        DeltaFlowDataType& newModuleDelta,
                                        std::vector<FlowDataType>& moduleFlowData,
                                        std::vector<unsigned int>& moduleMembers) const;

  // ===================================================
  // Consolidation
  // ===================================================

  void updateCodelengthOnMovingNode(InfoNode& current,
                                    DeltaFlowDataType& oldModuleDelta,
                                    DeltaFlowDataType& newModuleDelta,
                                    std::vector<FlowDataType>& moduleFlowData,
                                    std::vector<unsigned int>& moduleMembers);

  void consolidateModules(std::vector<InfoNode*>& modules) final;

  // ===================================================
  // Debug
  // ===================================================

  void printDebug() const final;

protected:
  // ===================================================
  // Protected member functions
  // ===================================================
  double calcCodelengthOnModuleOfLeafNodes(const InfoNode& parent) const final;

  // ===================================================
  // Init
  // ===================================================

  void initPhysicalNodes(InfoNode& root);

  void initPartitionOfPhysicalNodes(std::vector<InfoNode*>& nodes);

  // ===================================================
  // Codelength
  // ===================================================

  void calculateCodelength(std::vector<InfoNode*>& nodes) final;

  void calculateNodeFlow_log_nodeFlow();

  // ===================================================
  // Consolidation
  // ===================================================

  void updatePhysicalNodes(InfoNode& current, unsigned int oldModuleIndex, unsigned int bestModuleIndex);

  void addMemoryContributionsAndUpdatePhysicalNodes(InfoNode& current, DeltaFlowDataType& oldModuleDelta, DeltaFlowDataType& newModuleDelta);

  // ===================================================
  // Protected member variables
  // ===================================================

  using ModuleToMemNodes = std::map<unsigned int, detail::MemNodeSet>;

  std::vector<ModuleToMemNodes> m_physToModuleToMemNodes; // vector[physicalNodeID] map<moduleID, {#memNodes, sumFlow}>
  unsigned int m_numPhysicalNodes = 0;
  bool m_memoryContributionsAdded = false;
};

namespace detail {

  struct MemNodeSet {
    MemNodeSet(unsigned int numMemNodes, double sumFlow) : numMemNodes(numMemNodes), sumFlow(sumFlow) { }

    unsigned int numMemNodes; // use counter to check for zero to avoid round-off errors in sumFlow
    double sumFlow;
  };

} // namespace detail

} // namespace infomap

#endif /* _MEMMAPEQUATION_H_ */
