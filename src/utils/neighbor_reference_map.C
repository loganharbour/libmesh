// The libMesh Finite Element Library.
// Copyright (C) 2002-2020 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "libmesh/neighbor_reference_map.h"

// Local Includes
#include "libmesh/elem.h"
#include "libmesh/fe_interface.h"

namespace libMesh
{

const std::vector<Point> &
NeighborReferenceMap::get_neighbor_points(const Elem * const elem,
                                          const unsigned int side,
                                          const Elem * const side_elem,
                                          const std::vector<Point> & points,
                                          const Elem * const neighbor,
                                          const Elem * const neighbor_side_elem)
{
  libmesh_assert_equal_to(side_elem->type(), neighbor_side_elem->type());
  libmesh_assert_equal_to(elem->level() == neighbor->level());

  const std::size_t elem_type = elem->type();

  if (_data.size() <= elem_type)
    _data.resize(elem_type + 1);

  auto & elem_type_entry = _data[elem_type];
  if (elem_type_entry.size() <= side)
    elem_type_entry.resize(side + 1);

  const auto neighbor_node0 = neighbor_side_elem->get_node_index(side_elem->node(0));
  auto & side_entry = elem_type_entry[side];
  if (side_entry.size() <= neighbor_node0)
    side_entry.resize(neighbor_node0 + 1);

  auto & neighbor_points = side_entry[neighbor_node0];
  if (neighbor_points.empty()) // have to generate ref points
    FEInterface::inverse_map(elem->dim(), FEType(), neighbor, points, neighbor_points);

  libmesh_assert_equal_to(neighbor_points.size(), points.size());

#ifdef DEBUG
  // Confirm every time in debug that we have the right points cached
  std::vector<Point> confirm_points;
  FEInterface::inverse_map(elem->dim(), FEType(), neighbor, points, confirm_points);
  for (std::size_t i = 0; i < points.size(); ++i)
    libmesh_assert(confirm_points[i].relative_fuzzy_equals(neighbor_points[i]));
#endif

  return neighbor_points;
}

} // namespace libMesh
