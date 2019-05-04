#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h" 
#include "vtkProperty.h"
#include "vtkTubeFilter.h"
#include "vtkParametricSpline.h"
#include "vtkParametricFunctionSource.h"

int main()
{
	vtkPoints *points = vtkPoints::New();

	points->InsertPoint(0, 0.0, 0.0, 0.0);
	points->InsertPoint(1, 1.0, 1.0, 1.0);
	points->InsertPoint(2, 1.0, 0.0, 0.0);
	points->InsertPoint(3, 1.0, 0.0, 1.0);

	//////////////////////////////////////////////////////////////////////////
	//插值为参数样条曲线
	vtkParametricSpline *spline = vtkParametricSpline::New();
	spline->SetPoints(points);
	spline->ClosedOff();
	vtkParametricFunctionSource *splineSource = vtkParametricFunctionSource::New();
	splineSource->SetParametricFunction(spline);

	vtkPolyDataMapper *splineMapper = vtkPolyDataMapper::New();
	splineMapper->SetInputConnection(splineSource->GetOutputPort());

	vtkActor *splineActor = vtkActor::New();
	splineActor->SetMapper(splineMapper);
	splineActor->GetProperty()->SetColor(0.0, 1.0, 0.0);

	//////////////////////////////////////////////////////////////////////////    

	vtkTubeFilter *tube = vtkTubeFilter::New();
	tube->SetInputConnection(splineSource->GetOutputPort());
	tube->SetNumberOfSides(20);
	tube->SetRadius(0.05);
	tube->Update();

	vtkPolyDataMapper *tubeMapper = vtkPolyDataMapper::New();
	tubeMapper->SetInputData(tube->GetOutput());

	vtkActor *tubeActor = vtkActor::New();
	tubeActor->SetMapper(tubeMapper);
	tubeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->AddActor(splineActor);
	//ren1->AddActor(tubeActor);

	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(500, 500);
	renWin->Render();

	iren->Start();
	return EXIT_SUCCESS;
}