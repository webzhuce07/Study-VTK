 /**********************************************************************

 Copyright (c) Mr.Bin. All rights reserved.
 For more information visit: http://blog.csdn.net/webzhuce

  **********************************************************************/

#ifndef Project_MainWindow_H
#define Project_MainWindow_H

#include <QMainWindow>
#include "ui_CTViewer.h"
#include <vtkSmartPointer.h>

class vtkImageViewer2;
class vtkRenderer;
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;
class vtkResliceImageViewer;
class vtkImagePlaneWidget;
class vtkDistanceWidget;
class vtkResliceImageViewerMeasurements;

class ProjectMainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	ProjectMainWindow();
	~ProjectMainWindow();

private slots:
	//响应打开图像文件的槽函数
	void onOpenSlot();
	//响应鼠标移动的消息，实时输出鼠标的当前位置
	void updateCoords(vtkObject* obj);

protected:
    vtkSmartPointer< vtkResliceImageViewer> riw[3];
    vtkSmartPointer< vtkImagePlaneWidget> planeWidget[3];
    vtkSmartPointer< vtkDistanceWidget> DistanceWidget[3];
    vtkSmartPointer< vtkResliceImageViewerMeasurements> ResliceMeasurements;

private:
	vtkSmartPointer<vtkImageViewer2> m_pImageViewer;
	vtkSmartPointer<vtkRenderer> m_pRenderder;
	vtkEventQtSlotConnect* m_Connections;
};
#endif
