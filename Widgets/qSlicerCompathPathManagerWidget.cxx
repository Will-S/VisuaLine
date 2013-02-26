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

#include "qSlicerCompathTreeModel.h"

// PathManager Widgets includes
#include "qSlicerCompathPathManagerWidget.h"
#include "ui_qSlicerCompathPathManagerWidget.h"

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
  int getRowFromNode(vtkMRMLAnnotationRulerNode* rulerNode);
  vtkMRMLAnnotationRulerNode* getRulerFromRow(int row);
  vtkMRMLAnnotationFiducialNode* getFiducialFromRow(int row);
  vtkMRMLAnnotationFiducialNode* getFiducialFromRuler(vtkMRMLAnnotationRulerNode* rulerNode);
  vtkMRMLAnnotationRulerNode* getRulerFromFiducial(vtkMRMLAnnotationFiducialNode* fiducialNode);
  void deleteRow(int row);
  QString convertCoordinatesToQString(double coord[3]);

  QModelIndex SelectedRow;
  QModelIndex TopLevelSelection;
  vtkMRMLAnnotationHierarchyNode* SelectedHierarchyNode;
  typedef std::pair<vtkMRMLAnnotationRulerNode*, 
    vtkMRMLAnnotationFiducialNode*> TargetPath;
  std::vector<TargetPath> TargetPathList;
  qSlicerCompathTreeModel* PathTreeModel;
};

// --------------------------------------------------------------------------
qSlicerCompathPathManagerWidgetPrivate
::qSlicerCompathPathManagerWidgetPrivate(
  qSlicerCompathPathManagerWidget& object)
  : q_ptr(&object)
{
  this->SelectedHierarchyNode = NULL;
  this->PathTreeModel = new qSlicerCompathTreeModel();
}

// --------------------------------------------------------------------------
void qSlicerCompathPathManagerWidgetPrivate
::setupUi(qSlicerCompathPathManagerWidget* widget)
{
  this->Ui_qSlicerCompathPathManagerWidget::setupUi(widget);
}

// --------------------------------------------------------------------------
int qSlicerCompathPathManagerWidgetPrivate
::getRowFromNode(vtkMRMLAnnotationRulerNode* rulerNode)
{
  if (!rulerNode)
    {
    return -1;
    }

  if (this->TargetPathList.size() > 0)
    {
    for (int i = 0; i < this->TargetPathList.size(); i++)
      {
      if (this->TargetPathList[i].first == rulerNode)
        {
        return i;
        }
      }
    }
  return -1;
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* 
qSlicerCompathPathManagerWidgetPrivate::getRulerFromRow(int row)
{
  if (row < 0)
    {
    return NULL;
    }

  if (this->TargetPathList.size() >= row)
    {
    vtkMRMLAnnotationRulerNode* rulerNode =
      this->TargetPathList[row].first;
    if (rulerNode)
      {
      return rulerNode;
      }
    }
  return NULL;
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* 
qSlicerCompathPathManagerWidgetPrivate::getFiducialFromRow(int row)
{
  if (row < 0)
    {
    return NULL;
    }

  if (this->TargetPathList.size() >= row)
    {
    vtkMRMLAnnotationFiducialNode* fiduNode =
      this->TargetPathList[row].second;
    if (fiduNode)
      {
      return fiduNode;
      }
    }
  return NULL;
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* 
qSlicerCompathPathManagerWidgetPrivate::getFiducialFromRuler(vtkMRMLAnnotationRulerNode* rulerNode)
{
  if (!rulerNode)
    {
    return NULL;
    }

  if (this->TargetPathList.size() > 0)
    {
    for (int i = 0; i < this->TargetPathList.size(); i++)
      {
      if (this->TargetPathList[i].first == rulerNode)
        {
        return this->TargetPathList[i].second;
        }
      }
    }
  return NULL;
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* 
qSlicerCompathPathManagerWidgetPrivate::getRulerFromFiducial(vtkMRMLAnnotationFiducialNode* fiducialNode)
{
  if (!fiducialNode)
    {
    return NULL;
    }

  if (this->TargetPathList.size() > 0)
    {
    for (int i = 0; i < this->TargetPathList.size(); i++)
      {
      if (this->TargetPathList[i].second == fiducialNode)
        {
        return this->TargetPathList[i].first;
        }
      }
    }
  return NULL;
}

// --------------------------------------------------------------------------
void qSlicerCompathPathManagerWidgetPrivate
::deleteRow(int row)
{
  Q_Q(qSlicerCompathPathManagerWidget);

  if (row < 0)
    {
    return;
    }

  // Get Nodes
  vtkMRMLAnnotationRulerNode* rulerNode =
    this->TargetPathList[row].first;
  vtkMRMLAnnotationFiducialNode* fiduNode =
    this->TargetPathList[row].second;

  if (fiduNode && rulerNode)
    {
    // TODO: Remove from TableWidget

    // Remove from vector
    this->TargetPathList.erase(this->TargetPathList.begin() + row);

    // Remove nodes
    if (q->mrmlScene())
      {
      q->mrmlScene()->RemoveNode(rulerNode);
      q->mrmlScene()->RemoveNode(fiduNode);      
      }
    }
}

//-----------------------------------------------------------------------------
QString qSlicerCompathPathManagerWidgetPrivate
::convertCoordinatesToQString(double coord[3])
{
  std::stringstream tmp;
  tmp << "(" 
      << setprecision(1) << coord[0] << ","
      << setprecision(1) << coord[1] << ","
      << setprecision(1) << coord[2] 
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

  connect(d->HierarchySelectorWidget, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
          this, SLOT(onHierarchyNodeChanged(vtkMRMLNode*)));

  connect(d->DeleteButton, SIGNAL(clicked()),
          this, SLOT(onDeleteButtonClicked()));

  connect(d->ClearButton, SIGNAL(clicked()),
          this, SLOT(onClearButtonClicked()));

  if (d->PathTreeView && d->PathTreeModel)
    {
    d->PathTreeView->setModel(d->PathTreeModel);
    //d->PathTreeView->expandAll();
    connect(d->PathTreeView, SIGNAL(clicked(const QModelIndex)),
            this, SLOT(onRowSelected(const QModelIndex)));
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
  
  if (!d->PathTreeModel)
    {
    return;
    }

  if (d->TopLevelSelection.isValid())
    {
    // Remove node from scene first
    int position = d->TopLevelSelection.row();
    QModelIndex parentItem = d->TopLevelSelection.parent();

    // Select item on top before deleting
    QModelIndex nextParent = d->PathTreeModel
      ->index(parentItem.row()-1, 0, QModelIndex());
    d->PathTreeView->selectionModel()->select(nextParent, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    d->SelectedRow = nextParent;
    d->TopLevelSelection = nextParent;

    // Remove from model
    d->PathTreeModel->removeRows(position, 1, parentItem);
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onClearButtonClicked()
{
  Q_D(qSlicerCompathPathManagerWidget);

  // Turn off visibility
  d->PathTreeView->selectionModel()->select(QModelIndex(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  d->SelectedRow = QModelIndex();
  d->TopLevelSelection = QModelIndex();
  
  // Remove from model
  d->PathTreeModel->removeRows(0, d->PathTreeModel->rowCount(QModelIndex()), QModelIndex());

  // Clear arrays

  if (d->PathProjectionGroup)
    {
    d->PathProjectionGroup->setEnabled(0);
    }

  if (d->TargetProjectionGroup)
    {
    d->TargetProjectionGroup->setEnabled(0);
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::populateTreeView()
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->PathTreeView || !d->PathTreeModel 
      || !d->SelectedHierarchyNode)
    {
    return;
    } 

  for (int i = 0; i < d->SelectedHierarchyNode->GetNumberOfChildrenNodes(); i++)
    {
    // Get Ruler
    vtkMRMLAnnotationRulerNode* rulerNode =
      vtkMRMLAnnotationRulerNode::SafeDownCast(d->SelectedHierarchyNode
                                               ->GetNthChildNode(i)->GetAssociatedNode());

    if (rulerNode)
      {
      double targetPosition[3];
      rulerNode->GetPosition2(targetPosition);
      // Create fiducial at target position
      vtkSmartPointer<vtkMRMLAnnotationFiducialNode> targetPoint
        = vtkSmartPointer<vtkMRMLAnnotationFiducialNode>::New();
      targetPoint->Initialize(this->mrmlScene());
      targetPoint->SetFiducialWorldCoordinates(targetPosition);
      
      d->PathTreeModel->addTrajectory(rulerNode, targetPoint.GetPointer());
      }
    }

  if (d->PathTreeModel->rowCount() > 0)
    {
    if (d->PathProjectionGroup)
      {
      d->PathProjectionGroup->setEnabled(1);
      }
    
    if (d->TargetProjectionGroup)
      {
      d->TargetProjectionGroup->setEnabled(1);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onRowSelected(const QModelIndex& index)
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->PathTreeModel)
    {
    return;
    }
  
  d->SelectedRow = index;
  
  QModelIndex topLevelSelection
    = d->PathTreeModel->parent(d->SelectedRow);

  if (topLevelSelection.isValid())
    {
    // Child selected. Use parent node to set projection
    d->TopLevelSelection = topLevelSelection;
    }
  else
    {
    d->TopLevelSelection = d->SelectedRow;
    }
  qSlicerCompathTreeItem* topLevelItem
    = d->PathTreeModel->getItem(d->TopLevelSelection);

  if (topLevelItem)
    {
    this->setMRMLNodeProjectionWidget(topLevelItem);
    }

  // Update slider if offset alread set
  for (int i = 0; i < topLevelItem->childCount(); i++)
    {
    if (topLevelItem->child(i)->getPathNode())
      {
      double offsetValue = topLevelItem->child(i)->getVirtualOffset();
      d->VirtualOffsetSlider->setValue(offsetValue);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::setMRMLNodeProjectionWidget(qSlicerCompathTreeItem* item)
{
  Q_D(qSlicerCompathPathManagerWidget);
  
  if (!d->PathProjectionWidget || !d->TargetProjectionWidget)
    {
    return;
    }

  if (item->childCount() > 0)
    {
    // Check node is a top level
    for (int i = 0; i < item->childCount(); i++)
      {
      qSlicerCompathTreeItem* childItem
        = item->child(i);
      if (childItem->getPathNode())
        {
        d->PathProjectionWidget->setMRMLRulerNode(childItem->getPathNode());        
        }
      if (childItem->getTargetNode())
        {
        d->TargetProjectionWidget->setMRMLFiducialNode(childItem->getTargetNode());        
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onVirtualOffsetChanged(double newOffset)
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->PathTreeModel)
    {
    return;
    }

  qSlicerCompathTreeItem* parentItem
    = d->PathTreeModel->getItem(d->TopLevelSelection);
  qSlicerCompathTreeItem* pathItem = NULL;

  // Find node with a PathNode
  for (int i = 0; i < parentItem->childCount(); i++)
    {
    if (parentItem->child(i)->getPathNode())
      {
      pathItem = parentItem->child(i);
      }
    }

  if (pathItem)
    {
    if (!pathItem->getVirtualOffsetNode() && newOffset != 0)
      {
      vtkSmartPointer<vtkMRMLAnnotationRulerNode> virtualTip =
        vtkSmartPointer<vtkMRMLAnnotationRulerNode>::New();
      virtualTip->HideFromEditorsOn();
      virtualTip->Initialize(this->mrmlScene());
      pathItem->setVirtualOffsetNode(virtualTip.GetPointer());
      }
    pathItem->setVirtualOffset(newOffset);
    }
}
