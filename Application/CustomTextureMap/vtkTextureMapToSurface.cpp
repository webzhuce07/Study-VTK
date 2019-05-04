/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTextureMapToSurface.cxx

  Copyright (c) A.Bin
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkTextureMapToSurface.h"

#include "vtkCellData.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"

vtkStandardNewMacro(vtkTextureMapToSurface);

// Construct with s,t range=(0,1) and automatic plane generation turned on.
vtkTextureMapToSurface::vtkTextureMapToSurface()
{
  // all zero - indicates that using normal is preferred and automatic is off
  this->Origin[0] = this->Origin[1] = this->Origin[2] = 0.0;
  this->Point1[0] = this->Point1[1] = this->Point1[2] = 0.0;
  this->Point2[0] = this->Point2[1] = this->Point2[2] = 0.0;

  this->Normal[0] = 0.0;
  this->Normal[1] = 0.0;
  this->Normal[2] = 1.0;

  this->SRange[0] = 0.0;
  this->SRange[1] = 1.0;

  this->TRange[0] = 0.0;
  this->TRange[1] = 1.0;
}

int vtkTextureMapToSurface::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkDataSet *input = vtkDataSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *output = vtkDataSet::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double tcoords[2];
  vtkIdType numPts;
  vtkFloatArray *newTCoords;
  vtkFloatArray *tempTCoords;
  vtkIdType i;
  int dir = 0;
  double p[3];
  int abort=0;

  vtkDebugMacro(<<"Generating texture coordinates!");

  // First, copy the input to the output as a starting point
  output->CopyStructure( input );

  if ( (numPts=input->GetNumberOfPoints()) < 3)
    {
    vtkErrorMacro(<< "Not enough points for surface mapping\n");
    return 1;
    }

  //  Allocate texture data
  newTCoords = vtkFloatArray::New();
  newTCoords->SetName("Texture Coordinates");
  newTCoords->SetNumberOfComponents(2);
  newTCoords->SetNumberOfTuples(numPts);

  tempTCoords = vtkFloatArray::New();
  tempTCoords->SetName("Temp Texture Coordinate");
  tempTCoords->SetNumberOfComponents(2);
  tempTCoords->SetNumberOfTuples(numPts);


  //  Poitnt1 can not be equal to Point2
  if ( this->Point1[0] == this->Point2[0]  && this->Point1[1] == this->Point2[1])
    {
		vtkErrorMacro(<< "Error: point1 or point2\n");
		return 1;
    }
  this->ComputeNormal();

  //  Now project each point onto datum plane 
  double Smax = 0;
  double Tmax = 0;
  for (int i = 0; i < numPts; i++)
  {
	  output->GetPoint(i, p);
	  double projectPoint[3];
	  double k;
	  k = (this->Normal[0]*(this->Origin[0] - p[0]) + this->Normal[1]*(this->Origin[1] - p[1]) + this->Normal[2]*(this->Origin[2] - p[2])) /(this->Normal[0] * this->Normal[0] + this->Normal[1] * this->Normal[1] + this->Normal[2] * this->Normal[2]);
	  for (int j = 0; j<3; j++)
	  {
		  projectPoint[j] = p[j] + k * this->Normal[j];
		  //Calculate texture coordinate
		  double PQ[3];
		  PQ[0] = projectPoint[0] - this->Origin[0];
		  PQ[1] = projectPoint[1] - this->Origin[1];
		  PQ[2] = projectPoint[2] - this->Origin[2];
		  double S[3];
		  S[0] = projectPoint[0] - this->Point2[0];
		  S[1] = projectPoint[1] - this->Point2[1];
		  S[2] = projectPoint[2] - this->Point2[2];
		  double L = sqrt(PQ[0]*PQ[0] + PQ[1]*PQ[1]  + PQ[2]*PQ[2]);
		  double SL = sqrt(S[0]*S[0] + S[1]*S[1]  + S[2]*S[2]);
          double cos =  (PQ[0]* S[0] + PQ[1]* S[1] + PQ[2]* S[2]) / (SL * L);
		  double sin = sqrt(1- cos*cos);
		  tcoords[0] = L *cos;
		  tcoords[1] = L * sin;
          if (Smax < tcoords[0])
          {
			  Smax = tcoords[0];
          }
		  if (Tmax < tcoords[1])
		  {
			  Tmax = tcoords[1];
		  }
		  tempTCoords->SetTuple(i, tcoords);

	  }
  }
   
    // compute s-t coordinates
    for (i=0; i < numPts; i++) 
    {
		tempTCoords->GetTuple(i,tcoords);
		//s-coordinate
		tcoords[0] = tcoords[0] / Smax;
		//t-coordinate
		tcoords[1] = tcoords[1] / Tmax;;

		newTCoords->SetTuple(i,tcoords);
    
    }
	// Update ourselves
	output->GetPointData()->CopyTCoordsOff();
	output->GetPointData()->PassData(input->GetPointData());
	output->GetCellData()->PassData(input->GetCellData());

	output->GetPointData()->SetTCoords(newTCoords);
	newTCoords->Delete();
	tempTCoords->Delete();

	return 1;
}

#define VTK_TOLERANCE 1.0e-03

void vtkTextureMapToSurface::ComputeNormal()
{
	double M[3], N[3];
	for (int i=0;i<3; i++)
	{
		M[i] = this->Point1[i] - this->Origin[i];
		N[i] = this->Point2[i] - this->Origin[i];
	}
	vtkMath::Cross(M,N,this->Normal);
	vtkMath::Normalize(this->Normal);
}

void vtkTextureMapToSurface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Origin: (" << this->Origin[0] << ", "
     << this->Origin[1] << ", " << this->Origin[2] << " )\n";

  os << indent << "Axis Point 1: (" << this->Point1[0] << ", "
     << this->Point1[1] << ", " << this->Point1[2] << " )\n";

  os << indent << "Axis Point 2: (" << this->Point2[0] << ", "
     << this->Point2[1] << ", " << this->Point2[2] << " )\n";

  os << indent << "S Range: (" << this->SRange[0] << ", "
                               << this->SRange[1] << ")\n";
  os << indent << "T Range: (" << this->TRange[0] << ", "
                               << this->TRange[1] << ")\n";
  os << indent << "Normal: (" << this->Normal[0] << ", "
                                << this->Normal[1] << ", "
                                << this->Normal[2] << ")\n";
}
