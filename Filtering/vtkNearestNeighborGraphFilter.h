/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkNearestNeighborGraphFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkNearestNeighborGraphFilter - Creates a KNN graph on a vtkPointSet.
//
// .SECTION Description
// vtkNearestNeighborGraphFilter derives from vtkPointGraphFilter and implements
// the CreateEdgesOnPoints() function to create a graph with every point connected
// to its K nearest neighbors.
//
// .SECTION See Also

#ifndef __vtkNearestNeighborGraphFilter_h
#define __vtkNearestNeighborGraphFilter_h

#include "vtkPointGraphFilter.h"

class vtkNearestNeighborGraphFilter : public vtkPointGraphFilter
{
public:
  static vtkNearestNeighborGraphFilter *New();
  vtkTypeMacro(vtkNearestNeighborGraphFilter, vtkGraphAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Determines which edges to how many neighbors to connect each point to.
  vtkSetMacro(KNeighbors, unsigned int);
  vtkGetMacro(KNeighbors, unsigned int);

  vtkNearestNeighborGraphFilter();

protected:

  // Description:
  // This is the function that the superclass requires that we implement.
  // It creates an edge between every vertex and its KNeighbors nearest
  // neighbors.
  void CreateEdgesOnPoints();

  // Description:
  // Determines which edges to how many neighbors to connect each point to.
  unsigned int KNeighbors;

private:

  vtkNearestNeighborGraphFilter(const vtkNearestNeighborGraphFilter&);  // Not implemented.
  void operator=(const vtkNearestNeighborGraphFilter&);  // Not implemented.
};

#endif
