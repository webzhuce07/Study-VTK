#ifndef vtkImageRangeMarchingCubes_h
#define vtkImageRangeMarchingCubes_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class vtkCellArray;
class vtkFloatArray;
class vtkDoubleArray;
class vtkImageData;
class vtkPoints;

class vtkImageRangeMarchingCubes : public vtkPolyDataAlgorithm
{
public:
	static vtkImageRangeMarchingCubes *New();
	vtkTypeMacro(vtkImageRangeMarchingCubes, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	//@{
	/**
	* Methods to set contour range
	*/
	void SetContourRange(double range[2]);
	void GetContourRange(double *range);
	//@}

	/**
	* Because we refer to vtkImplicitFunction
	*/
	vtkMTimeType GetMTime() override;

	//@{
	/**
	* Set/Get the computation of scalars.
	*/
	vtkSetMacro(ComputeScalars, int);
	vtkGetMacro(ComputeScalars, int);
	vtkBooleanMacro(ComputeScalars, int);
	//@}

	//@{
	/**
	* Set/Get the computation of normals. Normal computation is fairly expensive
	* in both time and storage. If the output data will be processed by filters
	* that modify topology or geometry, it may be wise to turn Normals and Gradients off.
	*/
	vtkSetMacro(ComputeNormals, int);
	vtkGetMacro(ComputeNormals, int);
	vtkBooleanMacro(ComputeNormals, int);
	//@}

	//@{
	/**
	* Set/Get the computation of gradients. Gradient computation is fairly expensive
	* in both time and storage. Note that if ComputeNormals is on, gradients will
	* have to be calculated, but will not be stored in the output dataset.
	* If the output data will be processed by filters that modify topology or
	* geometry, it may be wise to turn Normals and Gradients off.
	*/
	vtkSetMacro(ComputeGradients, int);
	vtkGetMacro(ComputeGradients, int);
	vtkBooleanMacro(ComputeGradients, int);
	//@}

	// Should be protected, but the templated functions need these
	int ComputeScalars;
	int ComputeNormals;
	int ComputeGradients;
	int NeedGradients;

	vtkCellArray *Triangles;
	vtkFloatArray *Scalars;
	vtkPoints *Points;
	vtkFloatArray *Normals;
	vtkFloatArray *Gradients;

	vtkIdType GetLocatorPoint(int cellX, int cellY, int edge);
	void AddLocatorPoint(int cellX, int cellY, int edge, vtkIdType ptId);
	void IncrementLocatorZ();

	//@{
	/**
	* The InputMemoryLimit determines the chunk size (the number of slices
	* requested at each iteration).  The units of this limit is KiloBytes.
	* For now, only the Z axis is split.
	*/
	vtkSetMacro(InputMemoryLimit, vtkIdType);
	vtkGetMacro(InputMemoryLimit, vtkIdType);
	//@}

protected:
	vtkImageRangeMarchingCubes();
	~vtkImageRangeMarchingCubes() override;

	int NumberOfSlicesPerChunk;
	vtkIdType InputMemoryLimit;

	vtkDoubleArray *ContourRange;

	vtkIdType *LocatorPointIds;
	int LocatorDimX;
	int LocatorDimY;
	int LocatorMinX;
	int LocatorMinY;

	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
	int FillInputPortInformation(int port, vtkInformation *info) override;

	void March(vtkImageData *inData, int chunkMin, int chunkMax);
	void InitializeLocator(int min0, int max0, int min1, int max1);
	void DeleteLocator();
	vtkIdType *GetLocatorPointer(int cellX, int cellY, int edge);

private:
	vtkImageRangeMarchingCubes(const vtkImageRangeMarchingCubes&) = delete;
	void operator=(const vtkImageRangeMarchingCubes&) = delete;
};

#endif