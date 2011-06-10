/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkNearestNeighborGraphFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkAdjacentVertexIterator.h>
#include <vtkCellArray.h>
#include <vtkDataSet.h>
#include <vtkDoubleArray.h>
#include <vtkEdgeListIterator.h>
#include <vtkExecutive.h>
#include <vtkGraph.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTree.h>

#include "vtkNearestNeighborGraphFilter.h"

vtkStandardNewMacro(vtkNearestNeighborGraphFilter);

vtkNearestNeighborGraphFilter::vtkNearestNeighborGraphFilter()
{
  this->KNeighbors = 15;
}

void vtkNearestNeighborGraphFilter::CreateEdgesOnPoints()
{
  // Create a KDTree of the points
  vtkSmartPointer<vtkKdTreePointLocator> kdTree =
    vtkSmartPointer<vtkKdTreePointLocator>::New();
  kdTree->SetDataSet(this->Input);
  kdTree->BuildLocator();

  if(this->Input->GetNumberOfPoints() < this->KNeighbors)
    {
    std::cerr << "You are trying to request " << this->KNeighbors << " points but there are only "
              << this->Input->GetNumberOfPoints() << " in the tree." << std::endl;
    return;
    }

  // Find the nearest neighbors to each point and add adges between them,
  // if they do not already exist and they are not self loops
  for(vtkIdType pointID = 0; pointID < this->Input->GetNumberOfPoints(); pointID++)
    {
    vtkSmartPointer<vtkIdList> result =
      vtkSmartPointer<vtkIdList>::New();

    kdTree->FindClosestNPoints(this->KNeighbors, pointID, result);

    for(unsigned int neighbor = 0; neighbor < this->KNeighbors; neighbor++)
      {
      vtkIdType neighborID = result->GetId(neighbor);
      if(this->G->GetEdgeId(pointID, neighborID) == -1) // Edge does not already exist
        {
        this->G->AddEdge(pointID, neighborID);
        }
      }
    }
}

void vtkNearestNeighborGraphFilter::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "KNeighbors: " << this->KNeighbors << std::endl;
}
