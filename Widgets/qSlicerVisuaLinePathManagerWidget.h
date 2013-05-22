/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 8P41EB015898

==============================================================================*/

#ifndef __qSlicerVisuaLinePathManagerWidget_h
#define __qSlicerVisuaLinePathManagerWidget_h

// Qt includes
#include <QCheckBox>
#include <QModelIndex>
#include <QStandardItem>

#include <ctkVTKObject.h>

// PathManager Widgets includes
#include "qSlicerVisuaLineModuleWidgetsExport.h"

#include "qSlicerWidget.h"

class qSlicerVisuaLinePathManagerWidgetPrivate;
//class qSlicerVisuaLineTreeItem;
class vtkMRMLScene;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_VisuaLine
class Q_SLICER_MODULE_VISUALINE_WIDGETS_EXPORT qSlicerVisuaLinePathManagerWidget
  : public qSlicerWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerVisuaLinePathManagerWidget(QWidget *parent=0);
  virtual ~qSlicerVisuaLinePathManagerWidget();

public slots:

protected slots:
  void onHierarchyNodeChanged(vtkMRMLNode* hierarchy);
  void onDeleteButtonClicked();
  void onClearButtonClicked();
  void onRowSelected(const QModelIndex& index);
  void onVirtualOffsetChanged(double newOffset);
  void onItemChanged(QStandardItem*);
  void populateTreeView();
  void refreshView();

protected:
  QScopedPointer<qSlicerVisuaLinePathManagerWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerVisuaLinePathManagerWidget);
  Q_DISABLE_COPY(qSlicerVisuaLinePathManagerWidget);
};

#endif
