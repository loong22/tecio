#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <algorithm>
    #include <array>
    #include <memory>
    #include <utility>
    #include <vector>
#include "ThirdPartyHeadersEnd.h"

#include "ClassMacros.h"
#include "CodeContract.h"
#include "TecUtil.h"
#include "tecutildataio_utils_Exports.h"

namespace tecplot::tecutildataio_utils {

/**
 * Simple structure to store the nodes of a surface element's edge and perform comparisons between
 * another edge. This structure is used to organize the edges of surface elements connected by nodes
 * for the purpose of computing the left and right element neighbors which can then be delivered to
 * Tecplot's face map. Note that edge comparisons are performed without regard to orientation. We
 * always compare two edges using the lowest ordinal value of the two nodes.
 */
template <typename NODE_TYPE>
struct EdgeNodes
{
    std::array<NODE_TYPE,2> m_nodes;

    EdgeNodes(NODE_TYPE node0, NODE_TYPE node1)
        : m_nodes({{node0, node1}})
    {
    }

    /**
     * Create an edge node pair that can be used for comparisons. The pair is created with the
     * lowest node value in first and the highest in second. Two edges are consider equal if they
     * have the same two nodes regardless of the node ordering, (4,5) == (5,4).
     */
    std::pair<NODE_TYPE,NODE_TYPE> makeComparableEdge() const
    {
        return m_nodes[0] < m_nodes[1]
            ? std::make_pair(m_nodes[0], m_nodes[1])
            : std::make_pair(m_nodes[1], m_nodes[0]);
    }

    bool operator==(EdgeNodes const& other) const
    {
        return makeComparableEdge() == other.makeComparableEdge();
    }

    bool operator<(EdgeNodes const& other) const
    {
        std::pair<NODE_TYPE,NODE_TYPE> const thisEdge  = makeComparableEdge();
        std::pair<NODE_TYPE,NODE_TYPE> const otherEdge = other.makeComparableEdge();

        /*
         * Special handling for equal edges. Two edges are consider equal if they have the same two
         * nodes regardless of the node ordering, (4,5) == (5,4). However we want a predictable
         * sort order so that the resulting meshes are predictable. Therefore, two edges that are
         * "equal" require further testing. The edge with the smaller first node wins such that
         * (4,5) < (5,4).
         */
        if (thisEdge == otherEdge)
            return m_nodes[0] < other.m_nodes[0];
        else
            return thisEdge < otherEdge;
    }
};

/**
 * Simple structure to store the nodes and the left and right element neighbor for a single edge of
 * a surface zone's element. This structure is used to organize the edges of surface elements
 * connected by nodes for the purpose of computing the left and right element neighbors which can
 * then be delivered to Tecplot's face map.
 */
template <typename ELEM_TYPE, typename NODE_TYPE>
struct SurfElemEdge
{
    EdgeNodes<NODE_TYPE> m_edgeNodes;
    ELEM_TYPE            m_leftElem;
    ELEM_TYPE            m_rightElem;

    /**
     * Constructs a surface element's edge.
     */
    SurfElemEdge(
        EdgeNodes<NODE_TYPE> const& edgeNodes,
        ELEM_TYPE                   surfElem,
        bool                        isStandardOrientation)
        : m_edgeNodes(edgeNodes)
        , m_leftElem(isStandardOrientation ? surfElem : NO_NEIGHBORING_ELEMENT)
        , m_rightElem(isStandardOrientation ? NO_NEIGHBORING_ELEMENT : surfElem)
    {
        REQUIRE((m_leftElem == surfElem && m_rightElem == NO_NEIGHBORING_ELEMENT) ||
                (m_leftElem == NO_NEIGHBORING_ELEMENT && m_rightElem == surfElem));
    }

    /**
     * Folds the two equivalent surface surface element faces together by this surface element edge
     * instance taking the element of the other surface element edge and marking it as no longer
     * unique.
     * @param other
     *     Other equivalent surface element edge to fold into this surface element edge.
     */
    void fold(SurfElemEdge& other)
    {
        REQUIRE(isUnique());
        REQUIRE(this->m_edgeNodes == other.m_edgeNodes);

        if (m_leftElem == NO_NEIGHBORING_ELEMENT)
        {
            if (other.m_leftElem == NO_NEIGHBORING_ELEMENT)
                m_leftElem = other.m_rightElem;
            else
                m_leftElem = other.m_leftElem;
        }
        else
        {
            if (other.m_leftElem == NO_NEIGHBORING_ELEMENT)
                m_rightElem = other.m_rightElem;
            else
                m_rightElem = other.m_leftElem;
        }
        CHECK(m_leftElem != m_rightElem);
        CHECK(m_leftElem  != NO_NEIGHBORING_ELEMENT);
        CHECK(m_rightElem != NO_NEIGHBORING_ELEMENT);

        // mark the other surface element edge as no longer unique
        other.markNotUnique();
    }

    /**
     * Identifies if the surface element edge is unique. Note the algorithm that uses this class
     * surveys all the surface zone's element edges and modifies the state before asking this
     * question. In other words the result of this method may change after surveying all the surface
     * zone's element edges and eliminating duplicates.
     */
    bool isUnique() const
    {
        return !(m_leftElem  == NO_NEIGHBORING_ELEMENT &&
                 m_rightElem == NO_NEIGHBORING_ELEMENT);
    }

private:
    /**
     * Used while surveying surface element edges to mark a surface element edge as not unique.
     */
    void markNotUnique()
    {
        m_leftElem  = NO_NEIGHBORING_ELEMENT;
        m_rightElem = NO_NEIGHBORING_ELEMENT;
    }
};

/**
 * After surveying and building the surface zone element edges, make a pass through the collected
 * edges identifying which edges are unique.
 * @param surfZoneElemEdges
 *     All the surface zone's element edge information.
 * @return
 *     Number of unique element edges. Each edge has two nodes and a left and right element neighbor.
 * @throws
 *     std::bad_alloc if sufficient resources are not available
 */
template <typename ELEM_TYPE, typename NODE_TYPE>
LgIndex_t markUniqueSurfZoneElemEdges(std::vector<SurfElemEdge<ELEM_TYPE,NODE_TYPE>>& surfZoneElemEdges)
{
    /*
     * Sort the surface element edges using the edge nodes as the keys. After sorting, the
     * neighboring edge pairs are adjacent to one another.
     */
    std::sort(surfZoneElemEdges.begin(), surfZoneElemEdges.end(),
              [](auto const& a, auto const& b) {
                  return a.m_edgeNodes < b.m_edgeNodes;
              });

    /*
     * Scan the sorted edge pairs, counting the unique edges and marking the matching edge as no
     * longer unique.
     */
    LgIndex_t const totalNumElemEdges = checked_numeric_cast<LgIndex_t>(surfZoneElemEdges.size());
    LgIndex_t numUniqueSurfZoneElemEdges = 0;
    for (LgIndex_t eeOffset = 0; eeOffset < totalNumElemEdges-1; ++eeOffset)
    {
        auto& elemEdge0 = surfZoneElemEdges[eeOffset];
        auto& elemEdge1 = surfZoneElemEdges[eeOffset+1];
        CHECK(elemEdge0.isUnique() && elemEdge1.isUnique()); // ...all edges are initially marked as unique
        if (elemEdge0.m_edgeNodes == elemEdge1.m_edgeNodes) // ...interior edge neighbors
        {
            /*
             * Found a matching edge pair; fold the two edges together. Folding copies the element
             * connection from elemEdge1 into elemEdge0 and then marks elemEdge1 as no
             * longer unique.
             */
            elemEdge0.fold(elemEdge1);
            ++eeOffset; // ...no further processing of elemEdge1 is required
        }
        else
        {
            // no matching surface element edge pair, this is a boundary edge
            CHECK(elemEdge0.m_leftElem  == NO_NEIGHBORING_ELEMENT ||
                  elemEdge0.m_rightElem == NO_NEIGHBORING_ELEMENT);
        }
        ++numUniqueSurfZoneElemEdges;
    }

    /*
     * If the last edge was not marked as "not unique", either because there is an odd number of
     * edges in the surface zone, or because the last edge wasn't a match with the edge preceding
     * it, then count it as a unique edge.
     */
    if (totalNumElemEdges != 0 &&
        surfZoneElemEdges[totalNumElemEdges-1].isUnique())
    {
        ++numUniqueSurfZoneElemEdges;
    }

    ENSURE(numUniqueSurfZoneElemEdges <= totalNumElemEdges);
    return numUniqueSurfZoneElemEdges;
}

/**
 * Loads the surface zone's face map into Tecplot using the collected element edge information.
 * @tparam ELEM_TYPE
 *     Tecplot element type.
 * @tparam NODE_TYPE
 *     Tecplot node type.
 * @param faceMap
 *     Target face map to receive the connectivity.
 * @param surfZoneElemEdges
 *     A collection of the surface zone's unique and non-unique element edge information. The client
 *     is responsible for collecting the surface zone's element edge information and marking the
 *     unique and non-unique edges by calling markUniqueSurfZoneElemEdges().
 * @throws
 *     std::bad_alloc if sufficient resources are not available
 *     FileFormatException if unable to read the connectivity
 */
template <typename ELEM_TYPE, typename NODE_TYPE>
tecutildataio_utils_API void loadSurfZoneFaceMap(
    FaceMap_pa                                            faceMap,
    std::vector<SurfElemEdge<ELEM_TYPE,NODE_TYPE>> const& surfZoneElemEdges);

/**
 * Loads the FE classic line segment zone's node map into Tecplot using the edge nodes.
 * @tparam NODE_TYPE
 *     Tecplot node type.
 * @param edgeNodes
 *     A collection of FE line segment edge nodes to transfer to Tecplot's node map.
 * @param nodeMap
 *     Node map to receive the connectivity.
 */
template <typename NODE_TYPE>
void loadFELineSegNodeMap(
    std::vector<EdgeNodes<NODE_TYPE>> const& edgeNodes,
    NODE_TYPE*                               nodeMap)
{
    REQUIRE(VALID_REF(nodeMap));

    size_t baseNodeOffset = 0;
    for (auto const& edge : edgeNodes)
        for (auto const& node : edge.m_nodes)
            nodeMap[baseNodeOffset++] = node;
}

}
