/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef AvizoRectilinearCoordinateWriter_H_
#define AvizoRectilinearCoordinateWriter_H_

#include <string>

#include "MXA/Common/IO/MXAFileWriter64.h"

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/Common/IDataArray.h"

#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/Common/VoxelDataContainer.h"


/**
 * @class AvizoRectilinearCoordinateWriter AvizoRectilinearCoordinateWriter.h DREAM3DLib/IOFilters/AvizoRectilinearCoordinateWriter.h
 * @brief Writes out a native Avizo Rectilinear Coordinate file
 * @author Michael A. Jackson for BlueQuartz Software
 * @date Aug 9, 2012
 * @version 1.0
 */
class DREAM3DLib_EXPORT AvizoRectilinearCoordinateWriter : public AbstractFilter
{
  public:
    DREAM3D_SHARED_POINTERS(AvizoRectilinearCoordinateWriter);
    DREAM3D_STATIC_NEW_MACRO(AvizoRectilinearCoordinateWriter);
    DREAM3D_TYPE_MACRO_SUPER(AvizoRectilinearCoordinateWriter, AbstractFilter);

    virtual ~AvizoRectilinearCoordinateWriter();

    //------ Required Cell Data
    DREAM3D_INSTANCE_STRING_PROPERTY(GrainIdsArrayName)

    DREAM3D_INSTANCE_STRING_PROPERTY(OutputFile)
    DREAM3D_INSTANCE_PROPERTY(bool, WriteGrainIds)
    DREAM3D_INSTANCE_PROPERTY(bool, WriteBinaryFile)

    /**
    * @brief This returns the group that the filter belonds to. You can select
    * a different group if you want. The string returned here will be displayed
    * in the GUI for the filter
    */
    virtual const std::string getGroupName() { return DREAM3D::FilterGroups::IOFilters; }
	virtual const std::string getSubGroupName() { return DREAM3D::FilterSubGroups::OutputFilters; }

    /**
    * @brief This returns a string that is displayed in the GUI. It should be readable
    * and understandable by humans.
    */
    virtual const std::string getHumanLabel() { return "Avizo Rectilinear Coordinate Writer"; }

    /**
    * @brief This method will instantiate all the end user settable options/parameters
    * for this filter
    */
    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);
    
    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

	 /**
    * @brief Reimplemented from @see AbstractFilter class
    */
    virtual void execute();

    /**
    * @brief This function runs some sanity checks on the DataContainer and inputs
    * in an attempt to ensure the filter can process the inputs.
    */
    virtual void preflight();

  protected:
    AvizoRectilinearCoordinateWriter();

    /**
    * @brief Checks for the appropriate parameter values and availability of
    * arrays in the data container
    * @param preflight
    * @param voxels The number of voxels
    * @param fields The number of fields
    * @param ensembles The number of ensembles
    */
    void dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles);

    /**
     * @brief Generates the Avizo Header for this file
     * @return The header as a string
     */
    std::string generateHeader();

    /**
     * @brief Writes the data to the Avizo file
     * @param writer The MXAFileWriter object
     * @return Error code
     */
    int writeData(MXAFileWriter64 &writer);

  private:
    int32_t* m_GrainIds;

    AvizoRectilinearCoordinateWriter(const AvizoRectilinearCoordinateWriter&); // Copy Constructor Not Implemented
    void operator=(const AvizoRectilinearCoordinateWriter&); // Operator '=' Not Implemented
};

#endif /* AvizoRectilinearCoordinateWriter_H_ */
