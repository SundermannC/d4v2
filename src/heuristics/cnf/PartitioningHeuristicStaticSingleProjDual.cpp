/*
 * d4
 * Copyright (C) 2020  Univ. Artois & CNRS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PartitioningHeuristicStaticSingleProjDual.hpp"

#include <ostream>

#include "src/hyperGraph/HyperGraphExtractorDualProj.hpp"
#include "src/utils/EquivExtractorProj.hpp"

namespace d4 {

/**
   Constructor.

   @param[in] vm, the option list.
   @param[in] s, a wrapper on a solver.
   @param[in] om, a structure manager.
 */
PartitioningHeuristicStaticSingleProjDual::
    PartitioningHeuristicStaticSingleProjDual(po::variables_map &vm,
                                              WrapperSolver &s, SpecManager &om,
                                              std::ostream &out)
    : PartitioningHeuristicStaticSingleProjDual(
          vm, s, om, dynamic_cast<SpecManagerCnf &>(om).getNbClause(),
          dynamic_cast<SpecManagerCnf &>(om).getNbVariable(),
          dynamic_cast<SpecManagerCnf &>(om).getSumSizeClauses(), out) {

} // constructor

/**
   Constructor.

   @param[in] vm, the option list.
   @param[in] s, a wrapper on a solver.
   @param[in] om, a structure manager.
   @param[in] nbClause, the number of clauses.
   @param[in] nbVar, the number of variables.
   @param[in] sumSize, which give the number of literals.
 */
PartitioningHeuristicStaticSingleProjDual::
    PartitioningHeuristicStaticSingleProjDual(po::variables_map &vm,
                                              WrapperSolver &s, SpecManager &om,
                                              int nbClause, int nbVar,
                                              int sumSize, std::ostream &out)
    : PartitioningHeuristicStaticSingleProj(vm, s, om, nbClause, nbVar, sumSize,
                                            out) {
  out << "c [CONSTRUCTOR] Static partitioner: proj dual\n";

  m_pm = PartitionerManager::makePartitioner(vm, m_nbClause, m_nbVar, sumSize,
                                             out);

  m_hypergraph.init(m_nbVar + m_nbClause + sumSize + 1, m_nbVar + 1);
  m_hypergraphExtractor = new HyperGraphExtractorDualProj(
      m_nbVar, m_nbClause, (SpecManagerCnf &)om,
      vm["partitioning-heuristic-partitioner-np-cost"].as<int>());
  m_maxNbNodes = m_nbClause + 1;
  m_maxNbEdges = m_nbVar + 1;
  m_em = new EquivExtractorProj(&m_om);
  //m_em = new EquivExtractor(m_nbVar);
} // constructor

/**
   Destructor.
 */
PartitioningHeuristicStaticSingleProjDual::
    ~PartitioningHeuristicStaticSingleProjDual() {} // destructor

/**
   Set the elements given by indices in the bucketNumber structure.

   @param[in] hypergraph, the set of edges.
   @param[in] indices, the indices we want to transfer.
   @param[in] mapping, to map the edges to variables.
   @param[in] level, the level we want to assign the varaibles.
 */
void PartitioningHeuristicStaticSingleProjDual::setBucketLevelFromEdges(
    std::vector<std::vector<unsigned>> &hypergraph,
    std::vector<unsigned> &indices, std::vector<int> &mapping, unsigned level) {
  for (auto &id : indices)
    m_bucketNumber[mapping[id]] = level;
} // setBucketLevelFromEdges

bool PartitioningHeuristicStaticSingleProjDual::isStillOk(
    std::vector<Var> &component) {
  return m_phaseSelector->isStillOk(component);
} // isStillOk

} // namespace d4
