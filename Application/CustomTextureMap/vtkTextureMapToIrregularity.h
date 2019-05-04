#ifndef __vtkTextureMapToIrregularity_h
#define __vtkTextureMapToIrregularity_h

#include "vtkDataSetAlgorithm.h"

class vtkTextureMapToIrregularity : public vtkDataSetAlgorithm
{
public:
	vtkTypeMacro(vtkTextureMapToIrregularity, vtkDataSetAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);
	static vtkTextureMapToIrregularity *New();
	// Description: project points of dataset to textureAxis
	void SetVector(double vector[3]);
	vtkGetVectorMacro(Vector, double, 3);

protected:
	vtkTextureMapToIrregularity();
	~vtkTextureMapToIrregularity() {}
	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
	vtkTextureMapToIrregularity(const vtkTextureMapToIrregularity&);  // Not implemented.
	void operator=(const vtkTextureMapToIrregularity&);  // Not implemented.
	double Vector[3];

};
#endif // !__vtkTextureMapToIrregularity_h
