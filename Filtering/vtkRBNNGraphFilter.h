/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRBNNGraphFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkRBNNGraphFilter - creates a RBNN graph on a vtkPointSet
//
// .SECTION Description
// vtkRBNNGraphFilter creates a Radially Bounded Nearest Neighbor (RBNN) graph on a
// vtkPointSet. That is, it and creates a graph with every point connected to all of its
// neighbors within Radius.
//
// .SECTION See Also

#ifndef __vtkRBNNGraphFilter_h
#define __vtkRBNNGraphFilter_h

#include "vtkPointGraphFilter.h" //superclass

class vtkRBNNGraphFilter : public vtkPointGraphFilter
{
public:
  static vtkRBNNGraphFilter *New();
  vtkTypeMacro(vtkRBNNGraphFilter, vtkGraphAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // The radius of the sphere to look for neighbors.
  vtkSetMacro(Radius, double);
  vtkGetMacro(Radius, double);

  // Description:
  // The maximum number of neighbors to find for each point. This is
  // zero by default, indicating we want all of the neighbors within
  // Radius.
  vtkSetMacro(MaxNeighbors, int);
  vtkGetMacro(MaxNeighbors, int);

protected:
  // Description:
  // This is the function that the superclass requires that we implement.
  // It creates an edge between every vertex and all of its neigbors within
  // Radius (up to MaxNeighbors).
  void CreateEdgesOnPoints();

  vtkRBNNGraphFilter();
  ~vtkRBNNGraphFilter() {};

  // Description:
  // The radius of the sphere to look for neighbors.
  double Radius;

  // Description:
  // The maximum number of neighbors to find for each point.
  int MaxNeighbors;

private:

  vtkRBNNGraphFilter(const vtkRBNNGraphFilter&);  // Not implemented.
  void operator=(const vtkRBNNGraphFilter&);  // Not implemented.
};

#endif
