#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include "vtkReportProgressFilter.h"

void ProgressFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);

int main(int, char *[])
{ 
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();

	vtkSmartPointer<vtkCallbackCommand> progressCallback = 
		vtkSmartPointer<vtkCallbackCommand>::New();
	progressCallback->SetCallback(ProgressFunction);

	vtkSmartPointer<vtkReportProgressFilter> testFilter = 
		vtkSmartPointer<vtkReportProgressFilter>::New();
	testFilter->SetInputConnection(sphereSource->GetOutputPort());
	testFilter->AddObserver(vtkCommand::ProgressEvent, progressCallback);
	testFilter->Update();

	system("pause");
	return EXIT_SUCCESS;
}

void ProgressFunction ( vtkObject* caller,
					   long unsigned int vtkNotUsed(eventId),
					   void* vtkNotUsed(clientData),
					   void* vtkNotUsed(callData) )
{
	vtkReportProgressFilter* testFilter = static_cast<vtkReportProgressFilter*>(caller);
	std::cout << "Progress: " << testFilter->GetProgress() << std::endl;
}