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
  vtkMRMLAnnotationRulerNode* getNodeFromRow(int row);

  double SelectedRow;
  vtkMRMLAnnotationHierarchyNode* SelectedHierarchyNode;
  std::vector<vtkMRMLAnnotationRulerNode*> RulerList;
};

// --------------------------------------------------------------------------
qSlicerCompathPathManagerWidgetPrivate
::qSlicerCompathPathManagerWidgetPrivate(
  qSlicerCompathPathManagerWidget& object)
  : q_ptr(&object)
{
  this->SelectedRow = -1;
  this->SelectedHierarchyNode = NULL;
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

  if (this->RulerList.size() > 0)
    {
    for (int i = 0; i < this->RulerList.size(); i++)
      {
      if (this->RulerList[i] == rulerNode)
        {
        return i;
        }
      }
    }
  return -1;
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* 
qSlicerCompathPathManagerWidgetPrivate::getNodeFromRow(int row)
{
  if (row < 0)
    {
    return NULL;
    }

  if (this->RulerList.size() >= row)
    {
    vtkMRMLAnnotationRulerNode* rulerNode =
      this->RulerList[row];
    if (rulerNode)
      {
      return rulerNode;
      }
    }
  return NULL;
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

  connect(d->PathTable, SIGNAL(cellClicked(int,int)),
          this, SLOT(onRowSelected(int, int)));

  connect(d->DeleteButton, SIGNAL(clicked()),
          this, SLOT(onDeleteButtonClicked()));

  connect(d->ClearButton, SIGNAL(clicked()),
          this, SLOT(onClearButtonClicked()));

}

//-----------------------------------------------------------------------------
qSlicerCompathPathManagerWidget
::~qSlicerCompathPathManagerWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qSlicerCompathPathManagerWidget);
  if (d->HierarchySelectorWidget)
    {
    d->HierarchySelectorWidget->setMRMLScene(scene);
    }

  if (d->PathProjectionWidget)
    {
    d->PathProjectionWidget->setMRMLScene(scene);
    }

  if (d->TargetProjectionWidget)
    {
    d->TargetProjectionWidget->setMRMLScene(scene);
    }

  if (d->PathProjectionWidget)
    {
    d->PathProjectionWidget->setMRMLScene(scene);
    }

  if (d->TargetProjectionWidget)
    {
    d->TargetProjectionWidget->setMRMLScene(scene);
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onHierarchyNodeChanged(vtkMRMLNode* hierarchy)
{
  Q_D(qSlicerCompathPathManagerWidget);
  
  if (!hierarchy)
    {
    return;
    }

  d->SelectedHierarchyNode = 
    vtkMRMLAnnotationHierarchyNode::SafeDownCast(hierarchy);
  if (!d->SelectedHierarchyNode)
    {
    return;
    }

  double nOfChildren = d->SelectedHierarchyNode->GetNumberOfChildrenNodes();
  if (nOfChildren > 0)
    {
    // Clear table
    this->onClearButtonClicked();

    // Load new hierarchy table
    this->populatePathTable();
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onRowSelected(int row, int column)
{
  Q_D(qSlicerCompathPathManagerWidget);
  
  std::cerr << "Row selected: " << row << std::endl;

  vtkMRMLAnnotationRulerNode* selectedNode =
    d->getNodeFromRow(row);
  if (selectedNode)
    {
    d->PathProjectionWidget->setMRMLRulerNode(selectedNode);
    d->PathProjectionGroup->setEnabled(1);
    d->SelectedRow = row;
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onDeleteButtonClicked()
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->PathTable)
    {
    return;
    }

  if (d->SelectedRow >= 0)
    {
    // Turn off visibility
    vtkMRMLAnnotationRulerNode* selectedNode =
      d->getNodeFromRow(d->SelectedRow);
    if (selectedNode)
      {
      selectedNode->SetDisplayVisibility(0);
      }
    
    // Remove row and node
    d->PathTable->removeRow(d->SelectedRow);
    d->RulerList.erase(d->RulerList.begin() + d->SelectedRow);
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onClearButtonClicked()
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->PathTable)
    {
    return;
    }

  // Turn off visibility
  for (int i = 0; i < d->RulerList.size(); i++)
    {
    vtkMRMLAnnotationRulerNode* selectedNode =
      d->getNodeFromRow(i);
    if (selectedNode)
      {
      selectedNode->SetDisplayVisibility(0);
      }    
    }

  // Clear arrays
  d->PathTable->clear();
  d->RulerList.clear();
  d->SelectedRow = -1;

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
::populatePathTable()
{
  Q_D(qSlicerCompathPathManagerWidget);

  if (!d->SelectedHierarchyNode || !d->PathTable)
    {
    return;
    } 

  double nOfChildren = d->SelectedHierarchyNode->GetNumberOfChildrenNodes();  
  for (int i = 0; i < nOfChildren; i++)
    {
    // Get Child nodes
    vtkMRMLHierarchyNode* hNode =
      d->SelectedHierarchyNode->GetNthChildNode(i);
    
    // Check if node is ruler
    vtkMRMLAnnotationRulerNode* rulerNode =
      vtkMRMLAnnotationRulerNode::SafeDownCast(hNode->GetAssociatedNode());

    if (rulerNode)
      {      
      // Convention:
      // Point1 -> Entry point
      // Point2 -> Target
      double targetPosition[4], entryPosition[4];
      rulerNode->GetPositionWorldCoordinates1(entryPosition);
      rulerNode->GetPositionWorldCoordinates2(targetPosition);

      QString nodeName = QString(rulerNode->GetName());
      std::stringstream targetStream, entryStream;
      targetStream << "(" 
                   << targetPosition[0] << ","
                   << targetPosition[1] << ","
                   << targetPosition[2] 
                   << ")";

      entryStream << "(" 
                  << entryPosition[0] << ","
                  << entryPosition[1] << ","
                  << entryPosition[2] 
                  << ")";

      QCheckBox *nodeVisibility = new QCheckBox("", d->PathTable);
      QTableWidgetItem* nameItem = new QTableWidgetItem(nodeName);
      QTableWidgetItem* targetItem = new QTableWidgetItem(QString(targetStream.str().c_str()));
      QTableWidgetItem* entryItem = new QTableWidgetItem(QString(entryStream.str().c_str()));

      // Insert new row
      d->PathTable->insertRow(i);
      d->PathTable->setItem(i, 0, nameItem);
      d->PathTable->setItem(i, 1, targetItem);
      d->PathTable->setItem(i, 2, entryItem);
      d->PathTable->setCellWidget(i, 3, nodeVisibility);
      d->RulerList.push_back(rulerNode);
      this->qvtkConnect(rulerNode, vtkCommand::ModifiedEvent,
                        this, SLOT(onRulerNodeModified(vtkObject*, void*)));
      this->qvtkConnect(rulerNode, vtkMRMLDisplayableNode::DisplayModifiedEvent,
                        this, SLOT(onRulerDisplayNodeModified(vtkObject*, void*)));
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onRulerNodeModified(vtkObject* ruler, void* data)
{
  Q_D(qSlicerCompathPathManagerWidget);

  vtkMRMLAnnotationRulerNode* rulerNode =
    vtkMRMLAnnotationRulerNode::SafeDownCast(ruler);
  if (rulerNode)
    {
    // Update data
    double currentRow = d->getRowFromNode(rulerNode);
    if (currentRow < 0)
      {
      return;
      }

    // Convention:
    // Point1 -> Entry point
    // Point2 -> Target
    double targetPosition[4], entryPosition[4];
    rulerNode->GetPositionWorldCoordinates1(entryPosition);
    rulerNode->GetPositionWorldCoordinates2(targetPosition);
    
    QString nodeName = QString(rulerNode->GetName());
    std::stringstream targetStream, entryStream;
    targetStream << "(" 
                 << targetPosition[0] << ","
                 << targetPosition[1] << ","
                 << targetPosition[2] 
                 << ")";
    
    entryStream << "(" 
                << entryPosition[0] << ","
                << entryPosition[1] << ","
                << entryPosition[2] 
                << ")";
    
    QTableWidgetItem* nameItem = new QTableWidgetItem(nodeName);
    QTableWidgetItem* targetItem = new QTableWidgetItem(QString(targetStream.str().c_str()));
    QTableWidgetItem* entryItem = new QTableWidgetItem(QString(entryStream.str().c_str()));

    d->PathTable->setItem(currentRow, 0, nameItem);
    d->PathTable->setItem(currentRow, 1, targetItem);
    d->PathTable->setItem(currentRow, 2, entryItem);
    }
}

//-----------------------------------------------------------------------------
void qSlicerCompathPathManagerWidget
::onRulerDisplayNodeModified(vtkObject* ruler, void* data)
{
  Q_D(qSlicerCompathPathManagerWidget);

  vtkMRMLAnnotationRulerNode* rulerNode =
    vtkMRMLAnnotationRulerNode::SafeDownCast(ruler);
  if (rulerNode)
    {
    double currentRow = d->getRowFromNode(rulerNode);
    if (currentRow < 0)
      {
      return;
      }

    QCheckBox *rulerVisibility = qobject_cast<QCheckBox *>(d->PathTable->cellWidget(currentRow, 3));
    rulerVisibility->setChecked(rulerNode->GetDisplayVisibility());    
    }
}
