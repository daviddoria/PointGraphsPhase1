/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBivariateStatisticsAlgorithm.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2011 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/

#include "vtkBivariateStatisticsAlgorithm.h"
#include "vtkStatisticsAlgorithmPrivate.h"

#include "vtkDoubleArray.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkObjectFactory.h"
#include "vtkStdString.h"
#include "vtkStringArray.h"
#include "vtkTable.h"
#include "vtkVariantArray.h"

#include <vtksys/stl/set>
#include <vtksys/ios/sstream>

// ----------------------------------------------------------------------
vtkBivariateStatisticsAlgorithm::vtkBivariateStatisticsAlgorithm()
{
}

// ----------------------------------------------------------------------
vtkBivariateStatisticsAlgorithm::~vtkBivariateStatisticsAlgorithm()
{
}

// ----------------------------------------------------------------------
void vtkBivariateStatisticsAlgorithm::PrintSelf( ostream &os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
}

// ----------------------------------------------------------------------
void vtkBivariateStatisticsAlgorithm::AddColumnPair( const char* namColX, const char* namColY )
{
  if ( this->Internals->AddColumnPairToRequests( namColX, namColY ) )
    {
    this->Modified();
    }
}

// ----------------------------------------------------------------------
int vtkBivariateStatisticsAlgorithm::RequestSelectedColumns()
{
  return this->Internals->AddBufferEntryPairsToRequests();
}
