/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataReader.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h>
#include <vtkMaskPoints.h>
#include <vtkProperty.h>

//test data : ../data/fran_cut.vtk
int main(int argc, char *argv[])
{
	vtkNew<vtkPolyDataReader> reader;
	reader->SetFileName("E:/github/Study-VTK/data/fran_cut.vtk");
	reader->Update();

	vtkNew<vtkPolyDataNormals> normfilter;
	normfilter->SetInputData(reader->GetOutput());
	normfilter->SetComputePointNormals(1);
	normfilter->SetComputeCellNormals(0);
	normfilter->SetAutoOrientNormals(1);
	normfilter->SetSplitting(0);
	normfilter->Update();

	vtkNew<vtkMaskPoints> mask;
	mask->SetInputData(normfilter->GetOutput());
	mask->SetMaximumNumberOfPoints(300);
	mask->RandomModeOn();
	mask->Update();

	vtkNew<vtkArrowSource> arrow;
	arrow->Update();
	vtkNew<vtkGlyph3D> glyph;
	glyph->SetInputData(mask->GetOutput());
	glyph->SetSourceData(arrow->GetOutput()); 
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleFactor(0.01);
	glyph->Update();

	vtkNew<vtkPolyDataMapper> originmapper;
	originmapper->SetInputData(reader->GetOutput());
	vtkNew<vtkActor> originactor;
	originactor->SetMapper(originmapper);

	vtkNew<vtkPolyDataMapper> normedmapper;
	normedmapper->SetInputData(normfilter->GetOutput());
	vtkNew<vtkActor> normedactor;
	normedactor->SetMapper(normedmapper);

	vtkNew<vtkPolyDataMapper> glyphmapper;
	glyphmapper->SetInputData(glyph->GetOutput());

	vtkNew<vtkActor> glyphactor;
	glyphactor->SetMapper(glyphmapper);
	glyphactor->GetProperty()->SetColor(1., 0.,0.);

	double originalviewport[4] = {0.0, 0.0, 0.33, 1.0};
	double normviewport[4] = {0.33, 0.0, 0.66, 1.0};
	double glphviewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkNew<vtkRenderer> originalrenderer;
	originalrenderer->SetViewport(originalviewport);
	originalrenderer->AddActor(originactor);
	originalrenderer->ResetCamera();
	originalrenderer->SetBackground(1.0, 1.0, 1.0);

	vtkNew<vtkRenderer> normedrenderer;
	normedrenderer->SetViewport(normviewport);
	normedrenderer->AddActor(normedactor);
	normedrenderer->ResetCamera();
	normedrenderer->SetBackground(1.0, 1.0, 1.0);

	vtkNew<vtkRenderer> glyphRenderer;
	glyphRenderer->SetViewport(glphviewport);
	glyphRenderer->AddActor(glyphactor);
	glyphRenderer->AddActor(normedactor);
	glyphRenderer->ResetCamera();
	glyphRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkNew<vtkRenderWindow> renderwindow;
	renderwindow->AddRenderer(originalrenderer);
	renderwindow->AddRenderer(normedrenderer);
	renderwindow->AddRenderer(glyphRenderer);
	renderwindow->SetSize(640, 320);
	renderwindow->Render();
	renderwindow->SetWindowName("PolyDataNormal");

	vtkNew<vtkRenderWindowInteractor> renderwindowinteractor;
	renderwindowinteractor->SetRenderWindow(renderwindow);
	renderwindowinteractor->Initialize();
	renderwindowinteractor->Start();

	return EXIT_SUCCESS;
}