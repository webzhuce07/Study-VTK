#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h" 
#include "vtkProperty.h"
#include "vtkLine.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"


int main()
{
	//创建点数据
	vtkSmartPointer<vtkPoints>points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(0.0, 0.0, 1.0);
	points->InsertNextPoint(0.0, 0.0, 0.0);

	//每两个坐标点之间分别创建一条线
	//SetId()的第一个参数是线段的端点ID，第二参数是连接的的点的ID
	vtkSmartPointer<vtkLine>line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId(0, 0);
	line0->GetPointIds()->SetId(1, 1);

	vtkSmartPointer<vtkLine>line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId(0, 1);
	line1->GetPointIds()->SetId(1, 2);

	vtkSmartPointer<vtkLine>line2 = vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId(0, 2);
	line2->GetPointIds()->SetId(1, 0);

	//创建单元数组，用于存储以上创建的线段
	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(line0);
	lines->InsertNextCell(line1);
	lines->InsertNextCell(line2);

	//将点和线加入数据集中，前者定义数据集的几何结构，后者定义拓扑结构
	//创建vtkPolyData类型的数据，是一种数据集
	vtkSmartPointer<vtkPolyData>polydata = vtkSmartPointer<vtkPolyData>::New();

	//将创建的点数据加入vtkPolyData数据里
	polydata->SetPoints(points);  //点数据定义了polydata数据集的几何结构。
	polydata->SetLines(lines);  //定义拓扑结构

    //显示数据
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polydata);
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->AddActor(actor);
	ren->SetBackground(1, 1, 1);
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renWin->SetSize(500, 500);
	renWin->Render();
	iren->Start();
	return EXIT_SUCCESS;
}