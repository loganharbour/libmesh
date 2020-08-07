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

#ifndef LIBMESH_NEIGHBOR_REFERENCE_MAP_H
#define LIBMESH_NEIGHBOR_REFERENCE_MAP_H

#include "libmesh/libmesh_config.h"

// Local Includes
#include "libmesh/libmesh_common.h"
#include "libmesh/point.h"

// C++ Includes
#include <vector>

namespace libMesh
{

// Forward Declarations
class Elem;

/**
 * Complete
 *
 * \author Logan Harbour
 * \date 2020
 * \brief Complete
 */
class NeighborReferenceMap
{
public:
  const std::vector<Point> & get_neighbor_points(const Elem * const elem,
                                                 const unsigned int side,
                                                 const Elem * const side_elem,
                                                 const std::vector<Point> & points,
                                                 const Elem * const neighbor,
                                                 const Elem * const neighbor_side_elem);

  void clear() { _data.clear(); }

private:
  std::vector<std::vector<std::vector<std::vector<Point>>>> _data;
};

} // namespace libMesh

#endif // LIBMESH_NEIGHBOR_REFERENCE_MAP_H
