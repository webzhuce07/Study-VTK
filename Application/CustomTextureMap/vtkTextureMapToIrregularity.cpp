#include "vtkTextureMapToIrregularity.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkDataSet.h"
#include "vtkCellData.h"
#include "vtkMath.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

vtkStandardNewMacro(vtkTextureMapToIrregularity);

vtkTextureMapToIrregularity::vtkTextureMapToIrregularity()
{
	this->Vector[0] = 0.0;
	this->Vector[1] = 1.0;
	this->Vector[2] = 0.0;
}

int vtkTextureMapToIrregularity::RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
	//get th info objects
	vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation* outInfo = outputVector->GetInformationObject(0);

	//get the input and output
	vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkDataSet *output = vtkDataSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	// First, copy the input to the output as a starting point
	output->CopyStructure(input);

	if (input->GetNumberOfPoints() == 0)
	{
		return 1;
	}

	double minProjection = VTK_DOUBLE_MAX;
	double maxProjection = VTK_DOUBLE_MIN;
	for (int i = 0; i < input->GetNumberOfPoints(); i++) {
		double pos[3];
		input->GetPoint(i, pos);
		double projection = vtkMath::Dot(pos, this->Vector);
		if (projection < minProjection)
			minProjection = projection;
		if (projection > maxProjection)
			maxProjection = projection;
    }

	vtkFloatArray* tCoords = vtkFloatArray::New();
	tCoords->SetNumberOfComponents(2);
	for (int i = 0; i < input->GetNumberOfPoints(); i++){
		double pos[3];
		input->GetPoint(i, pos);
		double projection = vtkMath::Dot(pos, this->Vector);
		double t = (projection - minProjection) / (maxProjection - minProjection);
		if (t > 1.0)
			t = 1.0;
		if (t < 0.0)
			t = 0.0;
		tCoords->InsertNextTuple2(0, t);
	}

	// Update ourselves
	output->GetPointData()->CopyTCoordsOff();
	output->GetPointData()->PassData(input->GetPointData());
	output->GetCellData()->PassData(input->GetCellData());

	output->GetPointData()->SetTCoords(tCoords);
	tCoords->Delete();

	return 1;


}

void vtkTextureMapToIrregularity::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent << "Origin: (" << this->Vector[0] << ", "
		<< this->Vector[1] << ", " << this->Vector[2] << " )\n";
}

void vtkTextureMapToIrregularity::SetVector(double vector[3])
{
	if (vector)	{
		for (size_t i = 0; i < 3; i++)
			this->Vector[0] = vector[0];
		vtkMath::Normalize(this->Vector);
	}

}
