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

#include "qSlicerCompathTreeItem.h"
#include <iostream>

// --------------------------------------------------------------------------
qSlicerCompathTreeItem
::qSlicerCompathTreeItem(const QVector<QVariant> &data, qSlicerCompathTreeItem *parent)
 {
   this->parentItem = parent;
   this->itemData = data;
   this->checkState = Qt::Checked;
   this->PathNode = NULL;
   this->TargetNode = NULL;
   this->VirtualOffsetNode = NULL;
   this->offsetValue = 0;
 }

// --------------------------------------------------------------------------
qSlicerCompathTreeItem
::~qSlicerCompathTreeItem()
{
  qDeleteAll(this->childItems);

  if (this->PathNode)
    {
    this->PathNode->Delete();
    }

  if (this->TargetNode)
    {
    this->TargetNode->Delete();
    }

  if (this->VirtualOffsetNode)
    {
    this->VirtualOffsetNode->Delete();
    }
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::appendChild(qSlicerCompathTreeItem* item)
{
  this->childItems.append(item);
}

// --------------------------------------------------------------------------
qSlicerCompathTreeItem* qSlicerCompathTreeItem
::child(int row)
{
  return this->childItems.value(row);
}

// --------------------------------------------------------------------------
int qSlicerCompathTreeItem
::childCount() const
{
  return this->childItems.count();
}

// --------------------------------------------------------------------------
int qSlicerCompathTreeItem
::row() const
{
  if (this->parentItem)
    {
    return this->parentItem
      ->childItems.indexOf(const_cast<qSlicerCompathTreeItem*>(this));
    }
  return 0;
}

// --------------------------------------------------------------------------
int qSlicerCompathTreeItem
::columnCount() const
{
  return this->itemData.count();
}

// --------------------------------------------------------------------------
QVariant qSlicerCompathTreeItem
::data(int column) const
{
  return this->itemData.value(column);
}

// --------------------------------------------------------------------------
qSlicerCompathTreeItem* qSlicerCompathTreeItem
::parent()
{
  return this->parentItem;
}

// --------------------------------------------------------------------------
Qt::CheckState qSlicerCompathTreeItem
::isChecked() const
{
  return this->checkState;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::setCheckState(Qt::CheckState set)
{
  this->checkState = set;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::setPathNode(vtkMRMLAnnotationRulerNode* rulerNode)
{
  if (rulerNode)
    {
    this->PathNode = rulerNode;
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerCompathTreeItem
::getPathNode()
{
  return this->PathNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::setTargetNode(vtkMRMLAnnotationFiducialNode* fiducialNode)
{
  if (fiducialNode)
    {
    this->TargetNode = fiducialNode;
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* qSlicerCompathTreeItem
::getTargetNode()
{
  return this->TargetNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::setVirtualOffsetNode(vtkMRMLAnnotationRulerNode* virtualTip)
{
  if (virtualTip)
    {
    this->VirtualOffsetNode = virtualTip;

    if (this->VirtualOffsetNode->GetAnnotationLineDisplayNode() &&
        this->VirtualOffsetNode->GetAnnotationPointDisplayNode() &&
        this->VirtualOffsetNode->GetAnnotationTextDisplayNode())
      {
      // Green color
      this->VirtualOffsetNode->GetAnnotationLineDisplayNode()
        ->SetColor(0,1,0);
      this->VirtualOffsetNode->GetAnnotationPointDisplayNode()
        ->SetColor(0,1,0);
      this->VirtualOffsetNode->GetAnnotationTextDisplayNode()
        ->SetColor(0,1,0);
      
      if (this->PathNode)
        {
        double p2[3];
        this->PathNode->GetPosition2(p2);
        this->VirtualOffsetNode->SetPosition1(p2);
        this->VirtualOffsetNode->SetPosition2(p2);
        }
      }
    }
}


// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerCompathTreeItem
::getVirtualOffsetNode()
{
  return this->VirtualOffsetNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem
::setVirtualOffset(double offset)
{
  if (this->VirtualOffsetNode && this->PathNode)
    {
    this->offsetValue = offset;
    if (offset == 0)
      {
      // Turn off node visibility
      this->VirtualOffsetNode->SetDisplayVisibility(0);
      }
    else
      {
      // Set points position
      double p1[3], p2[3], p2minusp1[3];
      this->PathNode->GetPosition1(p1);
      this->PathNode->GetPosition2(p2);
      vtkMath::Subtract(p2,p1,p2minusp1);
      vtkMath::Normalize(p2minusp1);
      
      double offsetCoordinates[3];
      offsetCoordinates[0] = p2[0] + p2minusp1[0]*offset;
      offsetCoordinates[1] = p2[1] + p2minusp1[1]*offset;
      offsetCoordinates[2] = p2[2] + p2minusp1[2]*offset;

      this->VirtualOffsetNode->SetPosition2(offsetCoordinates);

      // Turn on visibility if off
      if (this->VirtualOffsetNode->GetDisplayVisibility() == 0)
        {
        this->VirtualOffsetNode->SetDisplayVisibility(1);
        }
      }
    this->VirtualOffsetNode->Modified();
    }
}

// --------------------------------------------------------------------------
double qSlicerCompathTreeItem
::getVirtualOffset()
{
/*
  if (this->VirtualOffsetNode)
    {
    std::cerr << "Offset: " << this->VirtualOffsetNode->GetDistanceMeasurement() << std::endl;
    return this->VirtualOffsetNode->GetDistanceMeasurement();
    }
  std::cerr << "Fail" << std::endl;
  return 0;
*/
  return this->offsetValue;
}



