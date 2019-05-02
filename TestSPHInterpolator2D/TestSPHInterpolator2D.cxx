#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkPlaneSource.h"
#include "vtkSPHQuinticKernel.h"
#include "vtkSPHInterpolator.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkProperty.h"
#include "vtkLookupTable.h"

int main(int argc, char *argv[])  
{
	double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };
	vtkNew<vtkRenderer> renderer;
	renderer->SetViewport(rightViewport);
	vtkNew<vtkRenderer> renderer0;
	renderer0->SetViewport(leftViewport);
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->AddRenderer(renderer0);
	vtkNew<vtkRenderWindowInteractor> interactor;
	interactor->SetRenderWindow(renderWindow);
	

	vtkNew<vtkXMLUnstructuredGridReader> reader;
	reader->SetFileName("F:/VTK/Bulid-VTK-8.2.0/ExternalData/Testing/Data/SPH_Points2D.vtu");
	reader->Update();
	vtkUnstructuredGrid*  output = reader->GetOutput();
	auto scalarRange = output->GetPointData()->GetArray("Rho")->GetRange();

	double *center = output->GetCenter();
	double *bounds = output->GetBounds();
	double length = output->GetLength();
	vtkNew<vtkPlaneSource> plane;
	plane->SetResolution(250, 250);
	plane->SetOrigin(bounds[0], bounds[2], bounds[4]);
	plane->SetPoint1(bounds[1], bounds[2], bounds[4]);
	plane->SetPoint2(bounds[0], bounds[3], bounds[4]);
	plane->SetCenter(center);
	plane->SetNormal(0, 0, 1);
	plane->Update();

	//SPH kernel
	vtkNew<vtkSPHQuinticKernel> sphKernel;
	sphKernel->SetSpatialStep(0.00002); //smoothing length
	sphKernel->SetDimension(2);
	vtkNew<vtkSPHInterpolator> interpolator;
	interpolator->SetInputConnection(plane->GetOutputPort());
	interpolator->SetSourceConnection(reader->GetOutputPort());
	interpolator->SetKernel(sphKernel);
	interpolator->Update();

	vtkNew<vtkLookupTable> colorTable;
	colorTable->SetNumberOfColors(2);
	colorTable->SetTableValue(0, 0.0, 0.0, 0.0, 1.0);
	colorTable->SetTableValue(1, 1.0, 1.0, 1.0, 1.0);
	colorTable->Build();

	//vtkNew<vtkPolyDataMapper> interpolatorMapper;
	vtkNew<vtkDataSetMapper> interpolatorMapper;
	interpolatorMapper->SetInputConnection(interpolator->GetOutputPort());
	interpolatorMapper->SetScalarModeToUsePointFieldData();
	interpolatorMapper->SelectColorArray("Rho");
	interpolatorMapper->SetScalarRange(750, 1050);
	//interpolatorMapper->SetLookupTable(colorTable);
	vtkNew<vtkActor> interpolatorActor;
	interpolatorActor->SetMapper(interpolatorMapper);
	interpolatorActor->GetProperty()->SetRepresentationToWireframe();

	vtkNew<vtkOutlineFilter> outline;
	outline->SetInputData(output);
	vtkNew<vtkPolyDataMapper> outlineMapper;
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	vtkNew<vtkActor> outlineActor;
	outlineActor->SetMapper(outlineMapper);
	renderer->AddActor(interpolatorActor);
	//renderer->AddActor(outlineActor);
	renderer->SetBackground(0.1, 0.2, 0.4);

	vtkNew<vtkDataSetMapper> pointsMapper;
	pointsMapper->SetInputData(output);
	pointsMapper->SetScalarModeToUsePointFieldData();
	pointsMapper->SelectColorArray("Rho");
	pointsMapper->SetScalarRange(750, 1050);
	//pointsMapper->SetLookupTable(colorTable);
	vtkNew<vtkActor> pointsActor;
	pointsActor->SetMapper(pointsMapper);
	pointsActor->GetProperty()->SetRepresentationToWireframe();
	renderer0->AddActor(pointsActor);
	renderer0->SetBackground(0.1, 0.2, 0.4);

	interactor->Initialize();
	renderWindow->Render();
	interactor->Start();
	return EXIT_SUCCESS;
}