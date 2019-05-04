/*=========================================================================

Program:   Visualization Toolkit
Module:
author:    A.Bin
Purpose:   Test vtkCellPicker¡¢vtkPointPicker...

=========================================================================*/

#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdTypeArray.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkSphereSource.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>

class CellPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static CellPickerInteractorStyle* New();

	CellPickerInteractorStyle()
	{
		selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedActor = vtkSmartPointer<vtkActor>::New();
	}

	virtual void OnLeftButtonDown()
	{
		int *pos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.0005);
		picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

		if (picker->GetCellId() != -1)
		{
			vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
			ids->SetNumberOfComponents(1);
			ids->InsertNextValue(picker->GetCellId());

			vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
			selectionNode->SetFieldType(vtkSelectionNode::CELL);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(ids);

			vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
			extractSelection->SetInputData(0, polyData);
			extractSelection->SetInputData(1, selection);
			extractSelection->Update();

			selectedMapper->SetInputData((vtkDataSet*)extractSelection->GetOutput());
			selectedActor->SetMapper(selectedMapper);
			selectedActor->GetProperty()->EdgeVisibilityOn();
			selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
			selectedActor->GetProperty()->SetLineWidth(3);

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);
		}
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

	}

	vtkSmartPointer<vtkPolyData>      polyData;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	vtkSmartPointer<vtkActor>	      selectedActor;
};

vtkStandardNewMacro(CellPickerInteractorStyle);

int main (int, char *[])
{
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(sphereSource->GetOutput());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor(0,1,0);
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1,1,1);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->Render();
	renderWindow->SetWindowName("CellPicker");
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	vtkSmartPointer<CellPickerInteractorStyle> style =
		vtkSmartPointer<CellPickerInteractorStyle>::New();
	style->SetDefaultRenderer(renderer);
	style->polyData = sphereSource->GetOutput();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}