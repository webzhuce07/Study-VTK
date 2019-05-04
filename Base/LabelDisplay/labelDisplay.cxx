#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkLookupTable.h>
#include <vtkContourFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <stdlib.h>
#include <vtkTextProperty.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkFreeTypeLabelRenderStrategy.h>
#include <vtkActor2D.h>
#include <vtkConeSource.h>
#include <vtkIdFilter.h>
#include <vtkLabeledDataMapper.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include <vtkTextSource.h>

int main()
{
	/*how to use vtkLabelPlacementMapper*/
	//create a vtkStringArry for saving labels, which is named as labels
	vtkSmartPointer<vtkStringArray> labels = vtkSmartPointer<vtkStringArray>::New();
	labels->SetName("labels");
	//create 100 points randomly to create a vtkPolyData
	vtkSmartPointer<vtkPoints> randpoints  = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertexs = vtkSmartPointer<vtkCellArray>::New();
	for (int i = 0; i < 100; i++)
	{
		randpoints->InsertNextPoint(float(rand()% 5),float(rand()% 5),float(rand()% 5));
		char str[8];
		sprintf(str,"%d", i);
		labels->InsertNextValue(str);
		vertexs->InsertNextCell(1);
		vertexs->InsertCellPoint(i);
	}
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(randpoints);
	polyData->SetVerts(vertexs);
	polyData->GetPointData()->AddArray(labels);

	vtkSmartPointer<vtkTextProperty> textProp = vtkSmartPointer<vtkTextProperty>::New();
	textProp->SetFontSize(12);
	textProp->SetColor(1.0, 0.0, 0.0);
	textProp->SetFontFamilyToArial();

	vtkSmartPointer<vtkPointSetToLabelHierarchy> hie = vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
	hie->SetInputData(polyData);
	hie->SetMaximumDepth(15);
	hie->SetLabelArrayName("labels");
	hie->SetTargetLabelCount(100);
	hie->SetTextProperty(textProp);

	//vtkLabelPlacementMapper在显示标签时，标签不会相互覆盖，如果后面的标签被挡住了，会自动隐藏，这样屏幕就不会因为标签太多导致拥挤
	//当显示大量标签时，速度慢足以影响用户使用
	vtkSmartPointer<vtkLabelPlacementMapper> labelMapper = vtkSmartPointer<vtkLabelPlacementMapper>::New();
	labelMapper->SetInputConnection(hie->GetOutputPort());
	vtkSmartPointer<vtkFreeTypeLabelRenderStrategy> strategy = vtkSmartPointer<vtkFreeTypeLabelRenderStrategy>::New();
	labelMapper->SetRenderStrategy(strategy);
	labelMapper->UseDepthBufferOn();
	labelMapper->SetShapeToNone();
	labelMapper->SetStyleToOutline();
	labelMapper->UseUnicodeStringsOff();

	vtkSmartPointer<vtkActor2D> labelActor2D = vtkSmartPointer<vtkActor2D>::New();
	labelActor2D->SetMapper(labelMapper);

	vtkSmartPointer<vtkPolyDataMapper> polyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	polyDataMapper->SetInputData(polyData);

	vtkSmartPointer<vtkActor> polyDataActor = vtkSmartPointer<vtkActor>::New();
	polyDataActor->SetMapper(polyDataMapper);
	polyDataActor->GetProperty()->SetColor(0.0, 0.0, 1.0);

    /*how to use vtkLabeledDataMapper*/
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	cone->SetHeight(3.0);
	cone->SetRadius(1.0);
	cone->SetResolution(10);
	
	vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());

	vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
	coneActor->SetMapper(coneMapper);

	vtkSmartPointer<vtkIdFilter> ids = vtkSmartPointer<vtkIdFilter>::New();
	ids->SetInputConnection(cone->GetOutputPort());
	ids->PointIdsOn();
	ids->CellIdsOn();
	ids->FieldDataOn();

	vtkSmartPointer<vtkLabeledDataMapper> idMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
	idMapper->SetInputConnection(ids->GetOutputPort());
	idMapper->SetLabelModeToLabelFieldData();
	idMapper->GetLabelTextProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkActor2D> pointLabelsActor = vtkSmartPointer<vtkActor2D>::New();
	pointLabelsActor->SetMapper(idMapper);

	/*how to use vtkVectorText*/
	vtkSmartPointer<vtkVectorText> vectorText = vtkSmartPointer<vtkVectorText>::New();
	vectorText->SetText("Zero");

	vtkSmartPointer<vtkPolyDataMapper> vectorTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vectorTextMapper->SetInputConnection(vectorText->GetOutputPort());
	
	vtkSmartPointer<vtkFollower> vectorTextActor = vtkSmartPointer<vtkFollower>::New();
	vectorTextActor->SetMapper(vectorTextMapper);
	vectorTextActor->SetScale(0.2, 0.2, 0.2);
	vectorTextActor->AddPosition(3.0, 3.0, 3.0);

	//vtkTextSource将字符串生转换成PolyData
	vtkSmartPointer<vtkTextSource> textSource =  vtkSmartPointer<vtkTextSource>::New();
	textSource->SetText("VTK");

	vtkSmartPointer<vtkPolyDataMapper> textSourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textSourceMapper->SetInputData(textSource->GetOutput());

	vtkSmartPointer<vtkActor> textSourceActor = vtkSmartPointer<vtkActor>::New();
	textSourceActor->SetMapper(textSourceMapper);
	textSourceActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> renderer= vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor2D(labelActor2D);
	renderer->AddActor(polyDataActor);
	renderer->AddActor(coneActor);
	renderer->AddActor(pointLabelsActor);
	renderer->AddActor(vectorTextActor);
	renderer->AddActor(textSourceActor);
	renderer->SetBackground(0.0, 0.0,0.0);

	//it's important
	vectorTextActor->SetCamera(renderer->GetActiveCamera());

	vtkSmartPointer<vtkRenderWindow> renWin= vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(300,300);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor= vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);

	interactor->Initialize();
	interactor->Start();








}