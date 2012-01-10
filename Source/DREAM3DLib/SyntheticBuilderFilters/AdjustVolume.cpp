/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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

#include "AdjustVolume.h"


#include "DREAM3DLib/Common/DREAM3DMath.h"
#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/SyntheticBuilderFilters/PackGrainsGen2.h"


#define NEW_SHARED_ARRAY(var, type, size)\
  boost::shared_array<type> var##Array(new type[size]);\
  type* var = var##Array.get();

const static float m_pi = M_PI;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AdjustVolume::AdjustVolume() :
m_GrainIds(NULL),
m_EquivalentDiameters(NULL)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AdjustVolume::~AdjustVolume()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AdjustVolume::preflight()
{
  int err = 0;
  std::stringstream ss;
  DataContainer::Pointer m = DataContainer::New();
  IDataArray::Pointer d = m->getVoxelData(DREAM3D::VoxelData::GrainIds);
  if(d.get() == NULL)
  {
	  ss << "GrainIds Array Not Initialized At Beginning of AdjustVolume Filter" << std::endl;
	  err = -300;
  }
  d = m->getFieldData(DREAM3D::FieldData::EquivalentDiameters);
  if(d.get() == NULL)
  {
	  PFFloatArrayType::Pointer p = PFFloatArrayType::CreateArray(1);
	  m->addFieldData(DREAM3D::FieldData::EquivalentDiameters, p);
  }

  setErrorCondition(err);
  setErrorMessage(ss.str());
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AdjustVolume::execute()
{
  setErrorCondition(0);

  DREAM3D_RANDOMNG_NEW()
  DataContainer* m = getDataContainer();
  if (NULL == m)
  {
    setErrorCondition(-1);
    std::stringstream ss;
    ss << getNameOfClass() << " DataContainer was NULL";
    setErrorMessage(ss.str());
    return;
  }
  int64_t totalPoints = m->totalPoints();

  m_GrainIds = m->getVoxelDataSizeCheck<int32_t, Int32ArrayType, AbstractFilter>(DREAM3D::VoxelData::GrainIds, totalPoints, this);
  if (NULL == m_GrainIds) { return; }

  size_t udims[3] = {0,0,0};
  m->getDimensions(udims);
#if (CMP_SIZEOF_SIZE_T == 4)
  typedef int32_t DimType;
#else
  typedef int64_t DimType;
#endif
  DimType dims[3] = {
    static_cast<DimType>(udims[0]),
    static_cast<DimType>(udims[1]),
    static_cast<DimType>(udims[2]),
  };

  DimType neighpoints[6];
  neighpoints[0] = -dims[0]*dims[1];
  neighpoints[1] = -dims[0];
  neighpoints[2] = -1;
  neighpoints[3] = 1;
  neighpoints[4] = dims[0];
  neighpoints[5] = dims[0]*dims[1];
  int iterations = 0;
  size_t selectedgrain = 0;
  int good = 0;
  int growth = 1;
  int nucleus;
  int bad = 0;
  float random, oldsizedisterror, currentsizedisterror, diam;
  size_t x, y, z;
  int neighpoint, index;
  size_t count, affectedcount;
  int vListSize = 1000;

  float voxtovol = m->getXRes()*m->getYRes()*m->getZRes()*(3.0/4.0)*(1.0/m_pi);

  gsizes.resize(m->getTotalFields());

  std::vector<int> voxellist(vListSize,-1);
  std::vector<int> affectedvoxellist(vListSize,-1);
  for(size_t i=1;i<m->getTotalFields();i++)
  {
    gsizes[i] = 0;
  }
  NEW_SHARED_ARRAY(reassigned, int, totalPoints)

  for(int i=0;i<totalPoints;i++)
  {
    reassigned[i] = 0;
    gsizes[m_GrainIds[i]]++;
  }
  PackGrainsGen2::Pointer packGrains = PackGrainsGen2::New();
  packGrains->setDataContainer(getDataContainer());
  packGrains->setObservers(this->getObservers());
  oldsizedisterror = packGrains->check_sizedisterror(-1000,-1000);
  while(iterations < 1)
  {
    iterations++;
    good = 0;
    while (good == 0)
    {
      good = 1;
      selectedgrain = int(rg.genrand_res53() * m->getTotalFields());
      if (selectedgrain >= m->getTotalFields()) selectedgrain = m->getTotalFields()-1;
      if (selectedgrain == 0) selectedgrain = 1;
    }
    growth = 1;
    random = rg.genrand_res53();
    if(random < 0.5) growth = -1;
    nucleus = 0;
    count = 0;
    affectedcount = 0;
    while(m_GrainIds[nucleus] != selectedgrain)
    {
      nucleus++;
      if(nucleus >= totalPoints) selectedgrain++, nucleus = 0;
    }
    voxellist[count] = nucleus;
    count++;
    for(size_t i=0;i<count;++i)
    {
      index = voxellist[i];
      x = index%dims[0];
      y = (index/dims[0])%dims[1];
      z = index/(dims[0]*dims[1]);
      for(size_t j=0;j<6;j++)
      {
        good = 1;
        neighpoint = index+neighpoints[j];
        if(j == 0 && z == 0) good = 0;
        if(j == 5 && z == (dims[2]-1)) good = 0;
        if(j == 1 && y == 0) good = 0;
        if(j == 4 && y == (dims[1]-1)) good = 0;
        if(j == 2 && x == 0) good = 0;
        if(j == 3 && x == (dims[0]-1)) good = 0;
        if(good == 1 && m_GrainIds[neighpoint] == selectedgrain && reassigned[neighpoint] == 0)
        {
	        voxellist[count] = neighpoint;
	        reassigned[neighpoint] = -1;
	        count++;
	        if(count >= voxellist.size()) voxellist.resize(voxellist.size()+vListSize,-1);
        }
        if(good == 1 && m_GrainIds[neighpoint] != selectedgrain && m_GrainIds[index] == selectedgrain)
        {
	        if(growth == 1 && reassigned[neighpoint] <= 0)
	        {
	          reassigned[neighpoint] = m_GrainIds[neighpoint];
	          m_GrainIds[neighpoint] = m_GrainIds[index];
	          affectedvoxellist[affectedcount] = neighpoint;
	          affectedcount++;
	          if(affectedcount >= affectedvoxellist.size()) affectedvoxellist.resize(affectedvoxellist.size()+vListSize,-1);
	        }
	        if(growth == -1 && reassigned[neighpoint] <= 0)
	        {
	          reassigned[index] = m_GrainIds[index];
	          m_GrainIds[index] = m_GrainIds[neighpoint];
	          affectedvoxellist[affectedcount] = index;
	          affectedcount++;
	          if(affectedcount >= affectedvoxellist.size()) affectedvoxellist.resize(affectedvoxellist.size()+vListSize,-1);
	        }
        }
      }
    }
    for(size_t i=0;i<affectedcount;i++)
    {
      index = affectedvoxellist[i];
      if(reassigned[index] > 0)
      {
        gsizes[m_GrainIds[index]]++;
        gsizes[reassigned[index]] = gsizes[reassigned[index]]-1;
      }
    }
    for(size_t i=1;i<m->getTotalFields();i++)
    {
      index = i;
      diam = 2.0f*powf((gsizes[index]*voxtovol),(1.0f/3.0f));
      m_EquivalentDiameters[index] = diam;
    }
    PackGrainsGen2::Pointer packGrains = PackGrainsGen2::New();
    packGrains->setDataContainer(getDataContainer());
    currentsizedisterror = packGrains->check_sizedisterror(-1000,-1000);

    if(currentsizedisterror <= oldsizedisterror)
    {
      oldsizedisterror = currentsizedisterror;
      for(size_t i=1;i<m->getTotalFields();i++)
      {
        if(gsizes[i] == 0) m->m_Grains.erase(m->m_Grains.begin() + i);
      }
    }
    if(currentsizedisterror > oldsizedisterror)
    {
      bad++;
      for(size_t i=0;i<affectedcount;i++)
      {
        index = affectedvoxellist[i];
        if(reassigned[index] > 0)
        {
          gsizes[m_GrainIds[index]] = gsizes[m_GrainIds[index]]-1;
          m_GrainIds[index] = reassigned[index];
          gsizes[m_GrainIds[index]]++;
        }
      }
      for(size_t i=1;i<m->getTotalFields();i++)
      {
        index = i;
        diam = 2.0f*powf((gsizes[index]*voxtovol),(1.0f/3.0f));
        m_EquivalentDiameters[index] = diam;
      }
    }
    for(int i=0;i<totalPoints;i++)
    {
      reassigned[i] = 0;
    }
  }
  NEW_SHARED_ARRAY(newnames, int, m->getTotalFields())

  for (size_t i=1;i<m->getTotalFields();i++)
  {
    newnames[i] = i;
  }
  for(int i=0;i<totalPoints;i++)
  {
    m_GrainIds[i] = newnames[m_GrainIds[i]];
  }

  // If there is an error set this to something negative and also set a message
  notify("AdjustVolume Completed", 0, Observable::UpdateProgressMessage);
}
