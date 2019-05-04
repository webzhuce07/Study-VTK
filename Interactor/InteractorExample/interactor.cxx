#pragma once
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>
#include <vtkSmartPointer.h>
#include <vtkFollower.h>
#include <vtkAppendPolyData.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkLookupTable.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>

int main()
{   
	vtkSmartPointer<vtkAppendPolyData> appendData = vtkSmartPointer<vtkAppendPolyData>::New();
	//DrawLines
	vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
	point->InsertNextPoint(0.0, 0.0, 0.0);
	point->InsertNextPoint(2.0, 2.0, 0.0);
	point->InsertNextPoint(4.0, 0.0, 0.0);
	point->InsertNextPoint(2.0, -2.0, 0.0);
	vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId(0,0);
	line0->GetPointIds()->SetId(1,1);

	vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId(0,1);
	line1->GetPointIds()->SetId(1,2);

	vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId(0,2);
	line2->GetPointIds()->SetId(1,3);

	vtkSmartPointer<vtkLine> line3 = vtkSmartPointer<vtkLine>::New();
	line3->GetPointIds()->SetId(0,3);
	line3->GetPointIds()->SetId(1,0);

	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(line0);
	lines->InsertNextCell(line1);
	lines->InsertNextCell(line2);
	lines->InsertNextCell(line3);

	vtkSmartPointer<vtkPolyData> linePolyData = vtkSmartPointer<vtkPolyData>::New();
	linePolyData->SetPoints(point);
	linePolyData->SetLines(lines);

	vtkSmartPointer<vtkUnsignedCharArray> pointColor = vtkSmartPointer<vtkUnsignedCharArray>::New();
	pointColor->SetNumberOfComponents(1);
	unsigned char black[3] = {0, 0, 0};
	pointColor->InsertNextTupleValue(black);
	linePolyData->GetCellData()->SetScalars(pointColor);
	
	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetNumberOfColors(1);
	colorTable->SetTableValue(0,0.0, 0.0, 0.0);
	colorTable->Build();


	// end draw lines

	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
	plane->SetOrigin(0.0, 0.0, -0.01);
	plane->SetPoint1(2.0, 2.0, -0.01);
	plane->SetPoint2(2.0, -2.0, -0.01);
		
	vtkSmartPointer<vtkVectorText> vectorText = vtkSmartPointer<vtkVectorText>::New();
	vectorText->SetText(". 8");

	appendData->AddInputData(linePolyData);
	appendData->AddInputData(vectorText->GetOutput());
	//appendData->AddInput(plane->GetOutput());
	appendData->Update();

	vtkSmartPointer<vtkPolyDataMapper> textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textMapper->SetInputData(appendData->GetOutput());
	
	vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
	textActor->SetMapper(textMapper);
	//textActor->GetProperty()->SetColor(0.0, 0.0, 0.0);


	vtkSmartPointer<vtkPolyDataMapper> planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper->SetInputData(plane->GetOutput());

	vtkSmartPointer<vtkFollower> planeActor = vtkSmartPointer<vtkFollower>::New();
	planeActor->SetMapper(planeMapper);


	vtkConeSource *cone=vtkConeSource::New();
	cone->SetResolution (10);
	cone->SetHeight(3);
	cone->SetRadius(1);

	vtkPolyDataMapper *coneMapper=vtkPolyDataMapper::New();
	coneMapper->SetInputData(cone->GetOutput());

	vtkActor *coneActor=vtkActor::New();
	coneActor->SetMapper(coneMapper);

	vtkRenderer *renderer=vtkRenderer::New();
	//renderer->AddActor(coneActor);
	renderer->AddActor(textActor);
	renderer->AddActor(planeActor);
	renderer->SetBackground(.1,0.2,0.4);

	vtkRenderWindow *renWin=vtkRenderWindow::New();
	renWin->SetSize(400,200);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	interactor->SetInteractorStyle(style);
	interactor->Initialize();
	interactor->Start();

	/*
	for (int i=0; i=3600; i++)
	{
		renWin->Render();
		renderer->GetActiveCamera()->Azimuth(1);
	}
	*/

	cone->Delete();
	coneActor->Delete();
	coneMapper->Delete();
	renderer->Delete();
	renWin->Delete();
	style->Delete();

	return 0;

}