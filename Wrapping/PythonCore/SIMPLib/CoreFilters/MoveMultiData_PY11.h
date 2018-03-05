#pragma once

/**
* @brief This header file is genererate from a program. Make changes with caution.
* 
* This header file wraps the SIMPL class MoveMultiData located at
* SIMPLib/CoreFilters/MoveMultiData.h. 
* The Python bindings are created using Pybind11.
*/

#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "SIMPLib/CoreFilters/MoveMultiData.h"

/**
* @brief This defines a C++11 alias so loading the file by itself into an IDE
* will allow the proper code completion for a wrapped std::shared_ptr<> class.
*/
#ifndef PySharedPtrClass_TEMPLATE
#define PySharedPtrClass_TEMPLATE
template <typename T>
using PySharedPtrClass = py::class_<T, std::shared_ptr<T> >;
#endif

/**
* @brief
* @param m The Python module
* @param parent The parent class of the wrapped class
* @return A std::shared_ptr<T> wrapped instance of the wrapped class properly 
* initialized.
*/
PySharedPtrClass<MoveMultiData> pybind11_init_SIMPLib_MoveMultiData(py::module &m, PySharedPtrClass<AbstractFilter>& parent)
{
  PySharedPtrClass<MoveMultiData> instance(m, "MoveMultiData", parent);
  instance
  .def(py::init([]()
    { 
      return MoveMultiData::New();
    }
))
  .def_static("New", &MoveMultiData::New)
  /* Property accessors for WhatToMove */
  .def_property("WhatToMove", &MoveMultiData::getWhatToMove, &MoveMultiData::setWhatToMove)
  /* Property accessors for DataContainerDestination */
  .def_property("DataContainerDestination", &MoveMultiData::getDataContainerDestination, &MoveMultiData::setDataContainerDestination)
  /* Property accessors for AttributeMatrixSources */
  .def_property("AttributeMatrixSources", &MoveMultiData::getAttributeMatrixSources, &MoveMultiData::setAttributeMatrixSources)
  /* Property accessors for AttributeMatrixDestination */
  .def_property("AttributeMatrixDestination", &MoveMultiData::getAttributeMatrixDestination, &MoveMultiData::setAttributeMatrixDestination)
  /* Property accessors for DataArraySources */
  .def_property("DataArraySources", &MoveMultiData::getDataArraySources, &MoveMultiData::setDataArraySources)
  ;
  return instance;
}


