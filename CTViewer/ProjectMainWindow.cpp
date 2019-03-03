/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:  http://blog.csdn.net/webzhuce 

**********************************************************************/

#include "ProjectMainWindow.h"
#include <QFileDialog>
#include <QDir>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <QVTKWidget.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkCommand.h>
#include <vtkDICOMImageReader.h>
#include "vtkSmartPointer.h"
#include "vtkResliceImageViewer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkDistanceWidget.h"
#include "vtkResliceImageViewerMeasurements.h"
#include <QMainWindow>
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkImageData.h"
#include "vtkCellPicker.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkCallbackCommand.h"

int pressCounts = 0;

void MyCallbackFunc(vtkObject*, unsigned long eid, void* clientdata, void *calldata)
{
	std::cout << "You have clicked: " << ++pressCounts << " times." << std::endl;
}

ProjectMainWindow::ProjectMainWindow()
{
	setupUi(this);
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer< vtkDICOMImageReader >::New();
    reader->SetDirectoryName("E:\\TestData\\dcm");
    reader->Update();
    int imageDims[3];
    reader->GetOutput()->GetDimensions(imageDims);
	
    for(int i = 0; i < 3; i++)
    {
        riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    }

    this->qvtkWidget->SetRenderWindow(riw[0]->GetRenderWindow());
    riw[0]->SetupInteractor(this->qvtkWidget->GetRenderWindow()->GetInteractor());

    this->qvtkWidget_2->SetRenderWindow(riw[1]->GetRenderWindow());
    riw[1]->SetupInteractor(this->qvtkWidget_2->GetRenderWindow()->GetInteractor());

    this->qvtkWidget_3->SetRenderWindow(riw[2]->GetRenderWindow());
    riw[2]->SetupInteractor(this->qvtkWidget_3->GetRenderWindow()->GetInteractor());

    for(int i = 0; i < 3; i++)
    {
        // make them all share the same reslice cursor object.
        vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(riw[i]->GetResliceCursorWidget()->GetRepresentation());
        riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

        rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);
		riw[i]->SetInputData(reader->GetOutput());
        riw[i]->SetSliceOrientation(i);
        riw[i]->SetResliceModeToAxisAligned();
    }

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.005);
	vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();
	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	this->qvtkWidget_4->GetRenderWindow()->AddRenderer(ren);
    vtkRenderWindowInteractor *iren = this->qvtkWidget_4->GetInteractor();

	vtkSmartPointer<vtkCallbackCommand> mouseCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	mouseCallback->SetCallback(MyCallbackFunc);
	iren->AddObserver(vtkCommand::MouseMoveEvent, mouseCallback);


    for(int i = 0; i < 3; i++)
    {
        planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        planeWidget[i]->SetInteractor(iren);
        planeWidget[i]->SetPicker(picker);
        planeWidget[i]->RestrictPlaneToVolumeOn();
        double color[3] = {0, 0, 0};
        color[i] = 1;
        planeWidget[i]->GetPlaneProperty()->SetColor(color);

        color[0] /= 4.0;
        color[1] /= 4.0;
        color[2] /= 4.0;
        riw[i]->GetRenderer()->SetBackground(color);

        planeWidget[i]->SetTexturePlaneProperty(ipwProp);
        planeWidget[i]->TextureInterpolateOff();
        planeWidget[i]->SetResliceInterpolateToLinear();
        planeWidget[i]->SetInputConnection(reader->GetOutputPort());
        planeWidget[i]->SetPlaneOrientation(i);
        planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
        planeWidget[i]->DisplayTextOn();
        planeWidget[i]->SetDefaultRenderer(ren);
        planeWidget[i]->SetWindowLevel(1358, -27);
        planeWidget[i]->On();
        planeWidget[i]->InteractionOn();
    }

	//// 设置m_QVTKWidget的渲染器
	//m_QVTKWidget->GetRenderWindow()->AddRenderer(m_pRenderder);

	////连接打开的信号与相应的槽
	//connect( m_OpenAction, SIGNAL( triggered() ), this, SLOT( onOpenSlot() ) ); 

	m_Connections = vtkEventQtSlotConnect::New();
	//m_Connections->Connect(m_QVTKWidget->GetRenderWindow()->GetInteractor(),
	//	vtkCommand::MouseMoveEvent,
	//	this,
	//	SLOT(updateCoords(vtkObject*)));
}

ProjectMainWindow::~ProjectMainWindow()
{
}

void ProjectMainWindow::onOpenSlot()
{
	QString filter;
	filter = "DICM image file (*.dcm)";

	QDir dir;
	QString fileName = QFileDialog::getOpenFileName( this, QString(tr("打开图像")), dir.absolutePath() , filter );
	if ( fileName.isEmpty() == true ) 
        return;

	// 支持带中文路径的读取
	QByteArray ba = fileName.toLocal8Bit();
	const char *fileName_str = ba.data();
}

void ProjectMainWindow::updateCoords(vtkObject* obj)
{
	// 获取交互器
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);

	// 获取鼠标的当前位置
	int event_pos[2];
	iren->GetEventPosition(event_pos);

	QString str;
	str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
	//m_StatusBar->showMessage(str);
}
