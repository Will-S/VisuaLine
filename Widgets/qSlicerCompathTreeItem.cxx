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

// --------------------------------------------------------------------------
qSlicerCompathTreeItem
::qSlicerCompathTreeItem(const QVector<QVariant> &data, qSlicerCompathTreeItem *parent)
 {
   this->parentItem = parent;
   this->itemData = data;
   this->checkState = Qt::Checked;
   this->PathNode = NULL;
   this->TargetNode = NULL;
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



