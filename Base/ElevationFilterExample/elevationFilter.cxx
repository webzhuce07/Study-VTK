#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"
#include "vtkElevationFilter.h"
#include "vtkLookupTable.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

int main()
{
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetThetaResolution(12);
	sphere->SetPhiResolution(12);

	vtkSmartPointer<vtkTransform>  transform = vtkSmartPointer<vtkTransform>::New();
	transform->Scale(1.5, 2, 1);

	vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
	transformFilter->SetInputConnection(sphere->GetOutputPort());
	transformFilter->SetTransform(transform);

	vtkSmartPointer<vtkElevationFilter>  elevationFilter = vtkSmartPointer<vtkElevationFilter>::New();
	elevationFilter->SetInputConnection(transformFilter->GetOutputPort());
	elevationFilter->SetLowPoint(0, -1, 0);
	elevationFilter->SetHighPoint(0, 1.0, 0);

	vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
	table->SetHueRange(0.667, 0.0);
	table->SetSaturationRange(1, 1);
	table->SetValueRange(1, 1);

	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetLookupTable(table);
	mapper->SetInputConnection(elevationFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1, 1, 1);
	vtkSmartPointer<vtkRenderWindow> renWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWindow);
	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}