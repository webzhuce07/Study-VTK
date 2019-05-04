/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTexture.h>
#include <vtkTextureMapToSphere.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkBMPReader.h>
#include <vtkTexturedSphereSource.h>
#include <vtkCylinderSource.h>

int main (int argc, char *argv[])
{
	vtkSmartPointer<vtkBMPReader> imageReader = vtkSmartPointer<vtkBMPReader>::New();
	imageReader->SetFileName("E:/TestData/lena.bmp");

	vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(imageReader->GetOutputPort());

	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	cylinder->SetHeight(20.0);
	cylinder->SetRadius(3.0);
	//cylinder->SetHeight(20.0);
	//cylinder->SetRadius(15.0);
	cylinder->SetCenter(0, 0, 0);
	cylinder->SetResolution(100);

	vtkSmartPointer<vtkTextureMapToCylinder> texturemap = vtkSmartPointer<vtkTextureMapToCylinder>::New();
	texturemap->SetInputConnection(cylinder->GetOutputPort());
	texturemap->SetPreventSeam(0);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(texturemap->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetTexture(texture);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renWinInteractor->SetRenderWindow( renderWindow );

	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("TextureExample");
	renderWindow->Render();
	renderWindow->Render();
	renWinInteractor->Start();

	return EXIT_SUCCESS;
}