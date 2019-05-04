#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkCamera.h>

int main()
{
	vtkConeSource *cone=vtkConeSource::New();
	cone->SetResolution (10);
	cone->SetHeight(3);
	cone->SetRadius(1);

	vtkPolyDataMapper *coneMapper=vtkPolyDataMapper::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());

	vtkActor *coneActor=vtkActor::New();
	coneActor->SetMapper(coneMapper);

	vtkRenderer *renderer=vtkRenderer::New();
	renderer->AddActor(coneActor);
	renderer->SetBackground(.1,0.2,0.4);

	vtkRenderWindow *renWin=vtkRenderWindow::New();
	renWin->SetSize(400,200);
	renWin->AddRenderer(renderer);

	for (int i=0; i=3600; i++)
	{
		renWin->Render();
		renderer->GetActiveCamera()->Azimuth(1);
	}

	cone->Delete();
	coneActor->Delete();
	coneMapper->Delete();
	renderer->Delete();
	renWin->Delete();

	return 0;

}