/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkButterflySubdivisionFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkLinearSubdivisionFilter.h>
#include <string>

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkPolyData> originalMesh;
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->Update();
	originalMesh = sphereSource->GetOutput();

	double numberOfViewports = 3;
	int numberOfSubdivisions = 2;

	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->SetSize(200* numberOfViewports,200); 
	renderWindow->SetWindowName("Multiple ViewPorts");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	for(unsigned i = 0; i < numberOfViewports; i++)
	{
		vtkSmartPointer<vtkPolyDataAlgorithm> subdivisionFilter;
		switch(i)
		{
		case 0:
			subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
			dynamic_cast<vtkLinearSubdivisionFilter *> (subdivisionFilter.GetPointer())->SetNumberOfSubdivisions(numberOfSubdivisions);
			break;
		case 1:
			subdivisionFilter =  vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
			dynamic_cast<vtkLoopSubdivisionFilter *> (subdivisionFilter.GetPointer())->SetNumberOfSubdivisions(numberOfSubdivisions);
			break;
		case 2: 
			subdivisionFilter = vtkSmartPointer<vtkButterflySubdivisionFilter>::New();
			dynamic_cast<vtkButterflySubdivisionFilter *> (subdivisionFilter.GetPointer())->SetNumberOfSubdivisions(numberOfSubdivisions);
			break;
		default:
			break;
		}

		subdivisionFilter->SetInputData(originalMesh);
		subdivisionFilter->Update();

		vtkNew<vtkRenderer> renderer;

		renderWindow->AddRenderer(renderer);
		renderer->SetViewport(static_cast<double>(i)/numberOfViewports,0,static_cast<double>(i+1)/numberOfViewports,1);
		renderer->SetBackground(1.0, 1.0, 1.0);

		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(subdivisionFilter->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		renderer->AddActor(actor);
		renderer->ResetCamera();
	}

	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataSubdivision");

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}