/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTextureMapToSurface.h

  Copyright (c) A.Bin
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkTextureMapToSurface - generate texture coordinates by mapping points to nonparametric surface
// .SECTION Description
// vtkTextureMapToSurface is a filter that generates 2D texture coordinates
// by mapping input dataset points onto a surface. The surface must either be
// user specified.
//
// .SECTION See Also
//  vtkTextureMapToPlane vtkTextureMapToCylinder
// vtkTextureMapToSphere vtkThresholdTextureCoords 

#ifndef __vtkTextureMapToSurface_h
#define __vtkTextureMapToSurface_h

#include "vtkDataSetAlgorithm.h"

class  vtkTextureMapToSurface : public vtkDataSetAlgorithm 
{
public:
  vtkTypeMacro(vtkTextureMapToSurface,vtkDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);
  //void SetOrigin(double pt[3]);
  //void SetPoint1(double pt[3]);
  //void SetPoint2(double pt[3]);

  // Description:
  // Construct with s,t range=(0,1) and automatic plane generation turned on.
  static vtkTextureMapToSurface *New();

  // Description:
  // Specify a point defining the origin of the datum plane. Used in conjunction with
  // the Point1 and Point2 to specify a map datum plane.It's one of points of input.
  vtkSetVector3Macro(Origin,double);
  vtkGetVectorMacro(Origin,double,3);

  // Description:
  // Specify a point defining the first axis of the datum plane.
  vtkSetVector3Macro(Point1,double);
  vtkGetVectorMacro(Point1,double,3);

  // Description:
  // Specify a point defining the second axis of the datum plane.
  vtkSetVector3Macro(Point2,double);
  vtkGetVectorMacro(Point2,double,3);

  // Description:
  // Specify datum plane normal. 
  vtkSetVector3Macro(Normal,double);
  vtkGetVectorMacro(Normal,double,3);

  // Description:
  // Specify s-coordinate range for texture s-t coordinate pair.
  vtkSetVector2Macro(SRange,double);
  vtkGetVectorMacro(SRange,double,2);

  // Description:
  // Specify t-coordinate range for texture s-t coordinate pair.
  vtkSetVector2Macro(TRange,double);
  vtkGetVectorMacro(TRange,double,2);

  // Description:
  // Turn on/off automatic plane generation.
  //vtkSetMacro(AutomaticPlaneGeneration,int);
  //vtkGetMacro(AutomaticPlaneGeneration,int);
  //vtkBooleanMacro(AutomaticPlaneGeneration,int);

protected:
  vtkTextureMapToSurface();
  ~vtkTextureMapToSurface() {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  void ComputeNormal();

  double Origin[3];
  double Point1[3];
  double Point2[3];
  double Normal[3];
  double SRange[2];
  double TRange[2];

private:
  vtkTextureMapToSurface(const vtkTextureMapToSurface&);  // Not implemented.
  void operator=(const vtkTextureMapToSurface&);  // Not implemented.
};

#endif
