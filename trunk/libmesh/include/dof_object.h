// $Id: dof_object.h,v 1.17 2003-09-02 18:02:37 benkirk Exp $

// The Next Great Finite Element Library.
// Copyright (C) 2002-2003  Benjamin S. Kirk, John W. Peterson
  
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



#ifndef __dof_object_h__
#define __dof_object_h__

// C++ includes

// Local includes
#include "mesh_config.h"
#include "mesh_common.h"
#include "libmesh.h" // libMesh::invalid_uint

// Forward declarations


/**
 * The \p DofObject defines an abstract base class for objects that
 * have degrees of freedom associated with them.  Examples of such
 * objects are the \p Node and \p Elem classes.  This class can
 * not be instantiated, only derived from.
 *
 * This class is intended to be extremely lightweight.  To help acheive
 * this goal no \p std::vector<> or anything else that might be heavy
 * is used to store the degree of freedom indices.  If the library is
 * configured with \p --enable-expensive then there can be multiple
 * components in any variable, if the library is configured with
 * \p --disable-expensive then there can be at MOST one component
 * per variable, i.e n_comp(sys,var) <= 1.
 *
 * \author Benjamin S. Kirk
 * \date 2003
 * \version $Revision: 1.17 $
 */

class DofObject
{
  
protected:
  
  /**
   * Constructor. Protected so that you can't instantiate one of these.
   */
  DofObject ();

public:
  
  /**
   * Copy-constructor.
   */
  DofObject (const DofObject&);

  /**
   * Destructor.
   */ 
  virtual ~DofObject ();

#ifdef ENABLE_AMR
  
  /**
   * This object on the last mesh.  Useful for projecting
   * solutions from one mesh to another.
   */
  DofObject* old_dof_object;

  /**
   * Sets the \p old_dof_object to NULL
   */
  void clear_old_dof_object ();

  /**
   * Sets the \p old_dof_object to a copy of \p this
   */
  void set_old_dof_object ();

#endif
  
  /**
   * Clear the \p DofMap data structures and return to
   * a pristine state.
   */
  void clear_dofs ();

  /**
   * Sets all degree of freedom numbers to \p invalid_id
   */
  void invalidate_dofs (const unsigned int sys_num = libMesh::invalid_uint);

  /**
   * Sets the id to \p invalid_id
   */
  void invalidate_id ();

  /**
   * Sets the processor id to \p invalid_processor_id
   */
  void invalidate_processor_id ();

  /**
   * Invalidates all the indices for this \p DofObject
   */
  void invalidate ();
  
  /**
   * @returns the number of degrees of freedom associated with
   * system \p s for this object. Optionally only counts degrees
   * of freedom for variable number \p var
   */
  unsigned int n_dofs (const unsigned int s, 
		       const unsigned int var =
		       libMesh::invalid_uint) const;
  
  /**
   * \returns the \p id for this \p DofObject
   */
  unsigned int id () const;

  /**
   * \returns the \p id for this \p DofObject as a writeable reference.
   */
  unsigned int & set_id ();

  /**
   * Sets the \p id for this \p DofObject
   */
  void set_id (const unsigned int id)
  { this->set_id() = id; }

  /**
   * @returns the processor that this element belongs to.
   * To conserve space this is stored as a short integer.
   */
  unsigned short int processor_id () const;
  
  /**
   * @returns the processor that this element belongs to as a
   * writeable reference.
   */
  unsigned short int & set_processor_id ();

  /**
   * Sets the \p processor_id for this \p DofObject.
   */  
  void set_processor_id (const unsigned int id);
  
  /**
   * @returns the number of systes associated with this
   * \p DofObject
   */
  unsigned int n_systems() const;

  /**
   *  Sets the number of systems for this \p DofObject
   */
  void set_n_systems (const unsigned int s);

  /**
   * Adds an additional system to the \p DofObject
   */
  void add_system ();
  
  /**
   * @returns the number of variables associated with system \p s
   * for this \p DofObject
   */
  unsigned int n_vars(const unsigned int s) const;
  
  /**
   * Sets number of variables associated with system \p s for this
   * \p DofObject
   */
  void set_n_vars(const unsigned int s,
		  const unsigned int nvars);
  
  /**
   * @returns the number of components for variable \p var
   * of system \p s associated with this \p DofObject. 
   * For example, the \p HIERARCHIC shape functions may
   * have @e multiple dof's associated with @e one node.  Another
   * examples are the \p MONOMIALs, where only the elements
   * hold the dof's, but for the different spatial directions,
   * and orders, see \p FE.
   */
  unsigned int n_comp(const unsigned int s,
		      const unsigned int var) const;
  
  /**
   * Sets the number of components for variable \p var
   * of system \p s associated with this \p DofObject
   */
  void set_n_comp(const unsigned int s,
		  const unsigned int var,
		  const unsigned int ncomp);
  
  /**
   * @returns the global degree of freedom number variable \p var,
   * component \p comp for system \p s associated with this \p DofObject
   */
  unsigned int dof_number(const unsigned int s,
			  const unsigned int var,
			  const unsigned int comp) const;
  
  /**
   * Sets the global degree of freedom number variable \p var,
   * component \p comp for system \p s associated with this \p DofObject
   */
  void set_dof_number(const unsigned int s,
		      const unsigned int var,
		      const unsigned int comp,
		      const unsigned int dn);
  
  /**
   * An invaild \p id to distinguish an uninitialized \p DofObject
   */
  static const unsigned int invalid_id;

  /**
   * An invalid \p processor_id to distinguish DOFs that have
   * not been assigned to a processor.
   */
  static const unsigned short int invalid_processor_id;

  
private:

  
  /**
   * The \p id of the \p DofObject
   */
  unsigned int _id;

  /**
   * The \p processor_id of the \p DofObject.
   * Degrees of freedom are wholly owned by processors,
   * however they may be duplicated on other processors.
   *
   * This is stored as an unsigned short int since we cannot
   * expect to be solving on 65000+ processors any time soon,
   * can we??
   */
  unsigned short int _processor_id;

  /**
   * The number of systems.
   */
  unsigned char _n_systems;
  
  /**
   * The number of variables associated with this
   * \p DofObject.  This is stored as an unsigned char
   * for storage efficiency.
   */
  unsigned char *_n_vars;

#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES
  
  /**
   * The number of components for each variable of each system
   * associated with this \p DofObject.  This is stored as an
   * unsigned char for storage efficiency.
   */
  unsigned char **_n_comp;

  /**
   * The global degree of freedom numbers for each component
   * of each variable of each system.
   */
  unsigned int ***_dof_ids;

#else

  /**
   * The global degree of freedom numbers for each variable
   * of each system. (Only one component allowed.)
   */
  unsigned int **_dof_ids;

#endif

};



//------------------------------------------------------
// Inline functions
inline
DofObject::DofObject () :
#ifdef ENABLE_AMR
  old_dof_object(NULL),
#endif
  _id (invalid_id),
  _processor_id (invalid_processor_id),
  _n_systems  (0),
  _n_vars (NULL),
#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES
  _n_comp (NULL),
#endif
  _dof_ids (NULL)
{
  this->invalidate();
}



inline
DofObject::DofObject (const DofObject& dof_obj) :
#ifdef ENABLE_AMR
  old_dof_object(NULL),
#endif
  _id (dof_obj._id),
  _processor_id (dof_obj._processor_id),
  _n_systems  (dof_obj._n_systems),
  _n_vars (NULL),
#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES
  _n_comp (NULL),
#endif
  _dof_ids (NULL)
{

#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES

  // Allocate storage for the dof numbers and copy
  // the values.
  _n_vars  = new unsigned char  [this->n_systems()];
  _n_comp  = new unsigned char* [this->n_systems()];
  _dof_ids = new unsigned int** [this->n_systems()];

  for (unsigned int s=0; s<this->n_systems(); s++)
    {      
      _n_vars[s]  = dof_obj.n_vars(s);
      _n_comp[s]  = new unsigned char [this->n_vars(s)];
      _dof_ids[s] = new unsigned int* [this->n_vars(s)];
  
      for (unsigned int v=0; v<this->n_vars(s); v++)
	{
	  _n_comp[s][v]  = dof_obj.n_comp(s,v);
	  
	  _dof_ids[s][v] = new unsigned int [dof_obj.n_comp(s,v)];
	  
	  for (unsigned int c=0; c<this->n_comp(s,v); c++)
	    _dof_ids[s][v][c] = dof_obj.dof_number(s,v,c);
	}
    }

#else

  // Allocate storage for the dof numbers and copy
  // the values.
  _n_vars  = new unsigned char [this->n_systems()];
  _dof_ids = new unsigned int* [this->n_systems()];

  for (unsigned int s=0; s<this->n_systems(); s++)
    {  
      _n_vars[s]  = dof_obj.n_vars(s);
      _dof_ids[s] = new unsigned int [this->n_vars(s)];
  
      for (unsigned int v=0; v<this->n_vars(s); v++)
	for (unsigned int c=0; c<this->n_comp(s,v); c++)
	  {
	    assert (c == 0);
	    _dof_ids[s][v] = dof_obj.dof_number(s,v,c);
	  }
    }
  
#endif

  // Check that everything worked
#ifdef DEBUG

  assert (this->n_systems() == dof_obj.n_systems());

  for (unsigned int s=0; s<this->n_systems(); s++)
    {
      assert (this->n_vars(s) == dof_obj.n_vars(s));

      for (unsigned int v=0; v<this->n_vars(s); v++)
	{
	  assert (this->n_comp(s,v) == dof_obj.n_comp(s,v));

	  for (unsigned int c=0; c<this->n_comp(s,v); c++)
	    assert (this->dof_number(s,v,c) == dof_obj.dof_number(s,v,c));
	}
    }
  
#endif
}



inline
DofObject::~DofObject ()
{
  // Free all memory.
#ifdef ENABLE_AMR
  this->clear_old_dof_object ();
#endif
  this->clear_dofs ();
}



inline
void DofObject::invalidate_dofs (const unsigned int sys_num)
{
  // If the user does not specify the system number...
  if (sys_num >= this->n_systems()) 
    for (unsigned int s=0; s<n_systems(); s++)
      for (unsigned int v=0; v<n_vars(s); v++)
	for (unsigned int c=0; c<n_comp(s,v); c++)
	  this->set_dof_number(s,v,c,invalid_id);

  // ...otherwise invalidate the dofs for all systems
  else
    for (unsigned int v=0; v<n_vars(sys_num); v++)
      for (unsigned int c=0; c<n_comp(sys_num,v); c++)
	this->set_dof_number(sys_num,v,c,invalid_id);
}



inline
void DofObject::invalidate_id ()
{
  this->set_id (invalid_id);
}



inline
void DofObject::invalidate_processor_id ()
{
  this->set_processor_id (invalid_processor_id);
}



inline
void DofObject::invalidate ()
{
  this->invalidate_dofs ();
  this->invalidate_id ();
  this->invalidate_processor_id ();
}



inline
void DofObject::clear_dofs ()
{
#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES

  // Only clear if there is data
  if (this->n_systems() != 0)
    {
      for (unsigned int s=0; s<this->n_systems(); s++)
	if (this->n_vars(s) != 0) // This has only been allocated if 
	  {                       // variables were declared
	    for (unsigned int v=0; v<this->n_vars(s); v++)
	      if (this->n_comp(s,v) != 0)
		{
		  assert (_dof_ids[s][v] != NULL); delete [] _dof_ids[s][v]; _dof_ids[s][v] = NULL;
		}
	  
	    assert (_dof_ids[s] != NULL); delete [] _dof_ids[s]; _dof_ids[s] = NULL;
	    assert (_n_comp[s]  != NULL); delete [] _n_comp[s];  _n_comp[s]  = NULL;
	  }
      
      assert (_n_vars  != NULL); delete [] _n_vars;  _n_vars  = NULL; 
      assert (_n_comp  != NULL); delete [] _n_comp;  _n_comp  = NULL;
      assert (_dof_ids != NULL); delete [] _dof_ids; _dof_ids = NULL;
    }
  
  // Make sure we cleaned up
  // (or there was nothing there)
  assert (_n_vars  == NULL);
  assert (_n_comp  == NULL);
  assert (_dof_ids == NULL);
  
#else

  // Only clear if there is data
  if (this->n_systems() != 0)
    {
      for (unsigned int s=0; s<this->n_systems(); s++)
	if (this->n_vars(s) != 0) // This has only been allocated if
	  {                       // variables were declared
	    assert(_dof_ids[s] != NULL); delete [] _dof_ids[s] ; _dof_ids[s] = NULL;
	  }
      
      assert (_n_vars  != NULL); delete [] _n_vars;  _n_vars  = NULL; 
      assert (_dof_ids != NULL); delete [] _dof_ids; _dof_ids = NULL;
    }
  
  // Make sure we cleaned up
  // (or there was nothing there)
  assert (_n_vars  == NULL);
  assert (_dof_ids == NULL);
  
#endif
  

  // No systems now.
  _n_systems = 0;
}
  


inline
unsigned int DofObject::n_dofs (const unsigned int s,
				const unsigned int var) const
{
  assert (s < this->n_systems());
  
  unsigned int num = 0;

  // Count all variables
  if (var == libMesh::invalid_uint)
    for (unsigned int v=0; v<this->n_vars(s); v++)
      num += this->n_comp(s,v);
  
  // Only count specified variable
  else
    {
      num = this->n_comp(s,var);
    }

  return num;
}



inline
unsigned int DofObject::id () const
{
  return _id;
}



inline
unsigned int & DofObject::set_id ()
{
  return _id;
}



inline
unsigned short int DofObject::processor_id () const
{
  return _processor_id;
}



inline
unsigned short int & DofObject::set_processor_id ()
{
  return _processor_id;
}



inline
void DofObject::set_processor_id (const unsigned int id)
{
#ifdef DEBUG
  
  if (id != static_cast<unsigned int>(static_cast<unsigned short int>(id)))
    {
      std::cerr << "ERROR: id too large for unsigned short int!" << std::endl
		<< "Recompile with DofObject::_processor_id larger!" << std::endl;
      
      error();
    }

#endif
  
  this->set_processor_id() = id;
}



inline
unsigned int DofObject::n_systems () const
{
  return static_cast<unsigned int>(_n_systems);
}



inline
unsigned int DofObject::n_vars(const unsigned int s) const
{
  assert (s < this->n_systems());
  assert (_n_vars != NULL);
  
  return static_cast<unsigned int>(_n_vars[s]);
}



inline
unsigned int DofObject::n_comp(const unsigned int s,
			       const unsigned int var) const
{
  assert (s < this->n_systems());
  assert (_dof_ids != NULL);
  assert (_dof_ids[s] != NULL);

#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES

  assert (_n_comp != NULL);
  assert (_n_comp[s] != NULL);

  // Does this ever happen?  I doubt it... 3/7/2003 (BSK)
  if (var >= this->n_vars(s))
    {
      here();
      std::cout << "s=" << s << ", var=" << var << std::endl
		<< "this->n_vars(s)=" << this->n_vars(s) << std::endl
		<< "this->n_systems()=" << this->n_systems() << std::endl;
	
      return 0;
    }
  
  return static_cast<unsigned int>(_n_comp[s][var]);

#else

  // If the dof isn't numbered yet there are effectively
  // zero dofs for this variable
  if (_dof_ids[s][var] == (invalid_id-1))
    return 0;
  
  return 1;
  
#endif
}



inline
unsigned int DofObject::dof_number(const unsigned int s,
				   const unsigned int var,
				   const unsigned int comp) const
{
  assert (s < this->n_systems());
  assert (var  < this->n_vars(s));
  assert (comp < this->n_comp(s,var));
  assert (_dof_ids != NULL);
  assert (_dof_ids[s] != NULL);
  
#ifdef ENABLE_EXPENSIVE_DATA_STRUCTURES

  assert (_dof_ids[s][var] != NULL);
  
  return _dof_ids[s][var][comp];

#else

  return _dof_ids[s][var];

#endif
}
  



#endif // #ifndef __dof_object_h__
