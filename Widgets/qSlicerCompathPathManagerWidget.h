/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerCompathPathManagerWidget_h
#define __qSlicerCompathPathManagerWidget_h

// Qt includes
#include <QCheckBox>

#include <ctkVTKObject.h>
// PathManager Widgets includes
#include "qSlicerCompathModuleWidgetsExport.h"

#include "qSlicerWidget.h"

class qSlicerCompathPathManagerWidgetPrivate;
class vtkMRMLScene;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_Compath
class Q_SLICER_MODULE_COMPATH_WIDGETS_EXPORT qSlicerCompathPathManagerWidget
  : public qSlicerWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerCompathPathManagerWidget(QWidget *parent=0);
  virtual ~qSlicerCompathPathManagerWidget();

public slots:
  void onMRMLSceneChanged(vtkMRMLScene* scene);

protected slots:
  void onHierarchyNodeChanged(vtkMRMLNode* hierarchy);
  void onRowSelected(int row, int column);
  void onDeleteButtonClicked();
  void onClearButtonClicked();
  void populatePathTable();
  void onRulerNodeModified(vtkObject* ruler, void* data);
  void onRulerDisplayNodeModified(vtkObject* ruler, void* data);
  void onFiducialNodeModified(vtkObject* ruler, void* data);
  void onFiducialDisplayNodeModified(vtkObject* ruler, void* data);

protected:
  QScopedPointer<qSlicerCompathPathManagerWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerCompathPathManagerWidget);
  Q_DISABLE_COPY(qSlicerCompathPathManagerWidget);
};

#endif