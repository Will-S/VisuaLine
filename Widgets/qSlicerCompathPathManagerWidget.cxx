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

#include <sstream>
#include <vector>
#include <iomanip>

//#include "qSlicerCompathTreeModel.h"

// PathManager Widgets includes
#include "qSlicerCompathPathManagerWidget.h"
#include "ui_qSlicerCompathPathManagerWidget.h"
#include "qSlicerCompathTreeItem.h"

#include <vtkMRMLAnnotationFiducialNode.h>
#include <vtkMRMLAnnotationHierarchyNode.h>
#include <vtkMRMLAnnotationLineDisplayNode.h>
#include <vtkMRMLAnnotationPointDisplayNode.h>
#include <vtkMRMLAnnotationRulerNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Compath
class qSlicerCompathPathManagerWidgetPrivate
  : public Ui_qSlicerCompathPathManagerWidget
{
  Q_DECLARE_PUBLIC(qSlicerCompathPathManagerWidget);
protected:
  qSlicerCompathPathManagerWidget* const q_ptr;

public:
  qSlicerCompathPathManagerWidgetPrivate(
    qSlicerCompathPathManagerWidget& object);
  virtual void setupUi(qSlicerCompathPathManagerWidget*);
  QString convertCoordinatesToQString(double coord[3]);

  QModelIndex SelectedRow;
  QModelIndex TopLevelSelection;
  QStandardItemModel* PathTreeModel;
  vtkMRMLAnnotationHierarchyNode* SelectedHierarchyNode;
};

// --------------------------------------------------------------------------
qSlicerCompathPathManagerWidgetPrivate
::qSlicerCompathPathManagerWidgetPrivate(
  qSlicerCompathPathManagerWidget& object)
  : q_ptr(&object)
{
  this->SelectedHierarchyNode = NULL;
  this->PathTreeModel = new QStandardItemModel();
}

// --------------------------------------------------------------------------
void qSlicerCompathPathManagerWidgetPrivate
::setupUi(qSlicerCompathPathManagerWidget* widget)
{
  this->Ui_qSlicerCompathPathManagerWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
QString qSlicerCompathPathManagerWidgetPrivate
::convertCoordinatesToQString(double coord[3])
{
  std::stringstream tmp;
  tmp.precision(2);
  tmp.setf(std::ios::fixed);
  tmp << "(R: " 
      << coord[0] << ", A: "
      << coord[1] << ", S: "
      << coord[2] 
      << ")";
  QString coordString = QString(tmp.str().c_str());
  return coordString;
}

//-----------------------------------------------------------------------------
// qSlicerCompathPathManagerWidget methods

//-----------------------------------------------------------------------------
qSlicerCompathPathManagerWidget
::qSlicerCompathPathManagerWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerCompathPathManagerWidgetPrivate(*this) )
{
  Q_D(qSlicerCompathPathManagerWidget);
  d->setupUi(this);

  connect(d->HierarchySelectorWidget, SIGNAL(nodeActivated(vtkMRMLNode*)),
          this, SLOT(onHierarchyNodeChanged(vtkMRMLNode*)));

  connect(d->DeleteButton, SIGNAL(clicked()),
          this, SLOT(onDeleteButtonClicked()));

  connect(d->ClearButton, SIGNAL(clicked()),
          this, SLOT(onClearButtonClicked()));

  if (d->PathTreeModel && d->PathTreeView)
    {
    d->PathTreeView->setModel(d->PathTreeModel);
    connect(d->PathTreeView, SIGNAL(clicked(const QModelIndex&)),
	    this, SLOT(onRowSelected(const QModelIndex&)));
    }

  connect(d->VirtualOffsetSlider, SIGNAL(valueChanged(double)),
          this, SLOT(onVirtualOffsetChanged(double)));
}

//-----------------------------------------------------------------------------
qSlicerCompathPathManagerWidget
::~qSlicerCompathPathManagerWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onHierarchyNodeChanged(vtkMRMLNode* hierarchy)
{
  Q_D(qSlicerCompathPathManagerWidget);
  
  if (!hierarchy || (d->SelectedHierarchyNode == hierarchy))
    {
    return;
    }

  d->SelectedHierarchyNode = 
    vtkMRMLAnnotationHierarchyNode::SafeDownCast(hierarchy);
  if (!d->SelectedHierarchyNode)
    {
    return;
    }

  d->ActiveNodeLabel->setText(d->SelectedHierarchyNode->GetName());

  double nOfChildren = d->SelectedHierarchyNode->GetNumberOfChildrenNodes();
  if (nOfChildren > 0)
    {
    // Clear table
    this->onClearButtonClicked();

    // Load new hierarchy table
    this->populateTreeView();
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onDeleteButtonClicked()
{
  Q_D(qSlicerCompathPathManagerWidget);
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onClearButtonClicked()
{
  Q_D(qSlicerCompathPathManagerWidget);
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onRowSelected(const QModelIndex& index)
{
  Q_D(qSlicerCompathPathManagerWidget);
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onVirtualOffsetChanged(double newOffset)
{
  Q_D(qSlicerCompathPathManagerWidget);
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::populateTreeView()
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->SelectedHierarchyNode || !d->PathTreeModel)
    {
    return;
    }

  double childCount = d->SelectedHierarchyNode->GetNumberOfChildrenNodes();
  for (int i = 0; i < childCount; i++)
    {
    // Get ruler
    vtkMRMLAnnotationRulerNode* ruler = 
      vtkMRMLAnnotationRulerNode::SafeDownCast(d->SelectedHierarchyNode->GetNthChildNode(i)->GetAssociatedNode());
    
    if (ruler)
      {
      // Create a top node
      qSlicerCompathTreeItem* topNode = new qSlicerCompathTreeItem(ruler->GetName());
      topNode->setCheckable(true);
      d->PathTreeModel->appendRow(topNode);

      // Create Path node
      qSlicerCompathTreeItem* pathNode = new qSlicerCompathTreeItem("Path");
      pathNode->setCheckable(true);
      topNode->appendRow(pathNode);

      // Create target node
      double targetPosition[3];
      QString targetName = QString("Target ");
      ruler->GetPosition2(targetPosition);
      targetName.append(d->convertCoordinatesToQString(targetPosition));
      qSlicerCompathTreeItem* targetNode = new qSlicerCompathTreeItem(targetName);
      targetNode->setCheckable(true);
      topNode->appendRow(targetNode);

      // Create fiducial at target point
      vtkSmartPointer<vtkMRMLAnnotationFiducialNode> targetFiducial =
	vtkSmartPointer<vtkMRMLAnnotationFiducialNode>::New();
      targetFiducial->Initialize(this->mrmlScene());
      targetFiducial->SetFiducialCoordinates(targetPosition);
      
      // Set nodes to top level node
      topNode->setPathNode(ruler);
      topNode->setTargetNode(targetFiducial);
      }
    }
}
