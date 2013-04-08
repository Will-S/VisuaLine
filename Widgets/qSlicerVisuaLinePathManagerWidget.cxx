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

//#include "qSlicerVisuaLineTreeModel.h"

// PathManager Widgets includes
#include "qSlicerVisuaLinePathManagerWidget.h"
#include "ui_qSlicerVisuaLinePathManagerWidget.h"
#include "qSlicerVisuaLineTreeItem.h"

#include <vtkMRMLAnnotationFiducialNode.h>
#include <vtkMRMLAnnotationHierarchyNode.h>
#include <vtkMRMLAnnotationLineDisplayNode.h>
#include <vtkMRMLAnnotationPointDisplayNode.h>
#include <vtkMRMLAnnotationRulerNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_VisuaLine
class qSlicerVisuaLinePathManagerWidgetPrivate
  : public Ui_qSlicerVisuaLinePathManagerWidget
{
  Q_DECLARE_PUBLIC(qSlicerVisuaLinePathManagerWidget);
protected:
  qSlicerVisuaLinePathManagerWidget* const q_ptr;

public:
  qSlicerVisuaLinePathManagerWidgetPrivate(
    qSlicerVisuaLinePathManagerWidget& object);
  virtual void setupUi(qSlicerVisuaLinePathManagerWidget*);
  QString convertCoordinatesToQString(double coord[3]);

  QModelIndex SelectedRow;
  QModelIndex TopLevelSelection;
  QStandardItemModel* PathTreeModel;
  vtkMRMLAnnotationHierarchyNode* SelectedHierarchyNode;
};

// --------------------------------------------------------------------------
qSlicerVisuaLinePathManagerWidgetPrivate
::qSlicerVisuaLinePathManagerWidgetPrivate(
  qSlicerVisuaLinePathManagerWidget& object)
  : q_ptr(&object)
{
  this->SelectedHierarchyNode = NULL;
  this->PathTreeModel = new QStandardItemModel();
}

// --------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidgetPrivate
::setupUi(qSlicerVisuaLinePathManagerWidget* widget)
{
  this->Ui_qSlicerVisuaLinePathManagerWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
QString qSlicerVisuaLinePathManagerWidgetPrivate
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
// qSlicerVisuaLinePathManagerWidget methods

//-----------------------------------------------------------------------------
qSlicerVisuaLinePathManagerWidget
::qSlicerVisuaLinePathManagerWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerVisuaLinePathManagerWidgetPrivate(*this) )
{
  Q_D(qSlicerVisuaLinePathManagerWidget);
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
    connect(d->PathTreeModel, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(onItemChanged(QStandardItem*)));
    }

  connect(d->VirtualOffsetSlider, SIGNAL(valueChanged(double)),
          this, SLOT(onVirtualOffsetChanged(double)));
}

//-----------------------------------------------------------------------------
qSlicerVisuaLinePathManagerWidget
::~qSlicerVisuaLinePathManagerWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::onHierarchyNodeChanged(vtkMRMLNode* hierarchy)
{
  Q_D(qSlicerVisuaLinePathManagerWidget);
  
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
void qSlicerVisuaLinePathManagerWidget
::onDeleteButtonClicked()
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

  if (!d->TopLevelSelection.isValid())
    {
    return;
    }
  
  if (!d->PathTreeModel || !d->PathTreeView ||
      !d->PathProjectionWidget || !d->TargetProjectionWidget ||
      !d->VirtualOffsetSlider)
    {
    return;
    }

  // Update index before removing
  QModelIndex newTopLevel = d->TopLevelSelection.sibling(d->TopLevelSelection.row()-1,0);

  d->PathTreeModel->removeRows(d->TopLevelSelection.row(), 1);

  if (newTopLevel.isValid())
    {
    d->PathTreeView->selectionModel()->select(newTopLevel, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    d->TopLevelSelection = newTopLevel;
    d->SelectedRow = newTopLevel;
    this->onRowSelected(d->TopLevelSelection);
    }

  if (d->PathTreeModel->rowCount() == 0)
    {
    d->VirtualOffsetSlider->setValue(0);
    }
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::onClearButtonClicked()
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

  if (!d->PathTreeModel ||
      !d->PathProjectionWidget || !d->TargetProjectionWidget ||
      !d->VirtualOffsetSlider)
    {
    return;
    }

  d->PathTreeModel->removeRows(0, d->PathTreeModel->rowCount());
  d->VirtualOffsetSlider->setValue(0);
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::onRowSelected(const QModelIndex& index)
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

  if (!index.isValid())
    {
    return;
    }

  if (!d->PathProjectionWidget || !d->TargetProjectionWidget ||
      !d->VirtualOffsetSlider)
    {
    return;
    }

  d->SelectedRow = index;

  // Get top level selection
  d->TopLevelSelection = index;
  if (index.parent().isValid())
    {
    d->TopLevelSelection = index.parent();
    }
  
  // Get item
  qSlicerVisuaLineTreeItem* topLevelItem
    = dynamic_cast<qSlicerVisuaLineTreeItem*>(d->PathTreeModel->itemFromIndex(d->TopLevelSelection));
  if (topLevelItem)
    {
    if (topLevelItem->getPathNode())
      {
      d->PathProjectionWidget->setMRMLRulerNode(topLevelItem->getPathNode());
      }
    if (topLevelItem->getTargetNode())
      {
      d->TargetProjectionWidget->setMRMLFiducialNode(topLevelItem->getTargetNode());
      }
    if (!topLevelItem->getVirtualOffsetNode())
      {
      d->VirtualOffsetSlider->setValue(0);
      }
    else
      {
      if (topLevelItem->getVirtualOffset() != 0)
        {
        d->VirtualOffsetSlider->setValue(topLevelItem->getVirtualOffset());
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::onVirtualOffsetChanged(double newOffset)
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

  if (!d->TopLevelSelection.isValid())
    {
    return;
    }

  if (!d->PathTreeModel)
    {
    return;
    }

  // Get Item
  qSlicerVisuaLineTreeItem* item
    = dynamic_cast<qSlicerVisuaLineTreeItem*>(d->PathTreeModel->itemFromIndex(d->TopLevelSelection));

  if (item)
    {
    if (!item->getVirtualOffsetNode())
      {
      // Create new ruler for virtual tip
      vtkSmartPointer<vtkMRMLAnnotationRulerNode> virtualTip
        = vtkSmartPointer<vtkMRMLAnnotationRulerNode>::New();
      virtualTip->HideFromEditorsOn();
      virtualTip->Initialize(this->mrmlScene());
      item->setVirtualOffsetNode(virtualTip);
      }
    item->setVirtualOffset(newOffset);
    }
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::onItemChanged(QStandardItem* item)
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

  if (!item || !d->PathTreeModel)
    {
    return;
    }

  qSlicerVisuaLineTreeItem* itemModified
    = dynamic_cast<qSlicerVisuaLineTreeItem*>(item);

  if (!itemModified)
    {
    return;
    }

  // Get index of item
  QModelIndex itemIndex =
    d->PathTreeModel->indexFromItem(itemModified);

  if (!itemIndex.isValid())
    {
    return;
    }

  if (itemModified->hasChildren())
    {
    // Top level item
    itemModified->setVisibility(itemModified->checkState() == Qt::Checked);
    
    // Update children too
    for (int i = 0; i < itemModified->rowCount(); i++)
      {
      QStandardItem* childItem = itemModified->child(i);
      if (childItem)
        {
        childItem->setCheckState(itemModified->checkState());
        }
      }
    }
  else
    {
    // Child item
    if (!itemIndex.parent().isValid())
      {
      return;
      }

    // Get parent
    qSlicerVisuaLineTreeItem* parentItem
      = dynamic_cast<qSlicerVisuaLineTreeItem*>(d->PathTreeModel->itemFromIndex(itemIndex.parent()));
    
    // Update visibility
    if (parentItem)
      {
      if (itemModified->isPathItem())
        {
        parentItem->setPathVisibility(itemModified->checkState() == Qt::Checked);
        parentItem->setOffsetVisibility(itemModified->checkState() == Qt::Checked);
        }
      else
        {
        parentItem->setTargetVisibility(itemModified->checkState() == Qt::Checked);
        }

      // If all childs are turned off, turn off parent
      bool allDifferent = true;
      Qt::CheckState newState = parentItem->checkState();
      for (int i = 0; i < parentItem->rowCount(); i++)
        {
        if (parentItem->child(i)->checkState() == parentItem->checkState())
          {
          // At least one has the same state. Exit the loop.
          allDifferent &= false;
          break;
          }
        newState = parentItem->child(i)->checkState();
        }
      
      if (allDifferent)
        {
        // All children have different state. Update parent.
        parentItem->setCheckState(newState);
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLinePathManagerWidget
::populateTreeView()
{
  Q_D(qSlicerVisuaLinePathManagerWidget);

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
      qSlicerVisuaLineTreeItem* topNode = new qSlicerVisuaLineTreeItem(ruler->GetName());
      topNode->setCheckable(true);
      topNode->setCheckState(Qt::Checked);
      d->PathTreeModel->appendRow(topNode);

      // Create Path node
      qSlicerVisuaLineTreeItem* pathNode = new qSlicerVisuaLineTreeItem("Path");
      pathNode->setCheckable(true);
      pathNode->setPathItem(true);
      pathNode->setCheckState(Qt::Checked);
      topNode->appendRow(pathNode);

      // Create target node
      double targetPosition[3];
      QString targetName = QString("Target ");
      ruler->GetPosition2(targetPosition);
      targetName.append(d->convertCoordinatesToQString(targetPosition));
      qSlicerVisuaLineTreeItem* targetNode = new qSlicerVisuaLineTreeItem(targetName);
      targetNode->setCheckable(true);
      targetNode->setCheckState(Qt::Unchecked);
      topNode->appendRow(targetNode);

      // Create fiducial at target point (hiden by default)
      vtkSmartPointer<vtkMRMLAnnotationFiducialNode> targetFiducial =
	vtkSmartPointer<vtkMRMLAnnotationFiducialNode>::New();
      targetFiducial->Initialize(this->mrmlScene());
      targetFiducial->SetFiducialCoordinates(targetPosition);
      
      // Set nodes to top level node
      topNode->setPathNode(ruler);
      topNode->setTargetNode(targetFiducial);
      topNode->setTargetVisibility(false);
      }
    }
}
