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

protected:
  QScopedPointer<qSlicerVisuaLinePathManagerWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerVisuaLinePathManagerWidget);
  Q_DISABLE_COPY(qSlicerVisuaLinePathManagerWidget);
};

#endif
