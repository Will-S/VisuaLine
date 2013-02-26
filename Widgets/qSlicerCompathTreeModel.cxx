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

#include "qSlicerCompathTreeModel.h"

// --------------------------------------------------------------------------
qSlicerCompathTreeModel::qSlicerCompathTreeModel(QObject* parent) 
  : QAbstractItemModel(parent)
{
  QVector<QVariant> rootData;
  rootData << "Name";
  this->rootItem = new qSlicerCompathTreeItem(rootData);
//  this->setupModelData(this->rootItem);
}

// --------------------------------------------------------------------------
qSlicerCompathTreeModel
::~qSlicerCompathTreeModel()
{
  delete this->rootItem;
}

// --------------------------------------------------------------------------
QModelIndex qSlicerCompathTreeModel
::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() && parent.column() != 0)
    {
    return QModelIndex();
    }

  qSlicerCompathTreeItem* parentItem = this->getItem(parent);
  qSlicerCompathTreeItem* childItem = parentItem->child(row);
  if (childItem)
    {
    return this->createIndex(row, column, childItem);
    }
  else
    {
    return QModelIndex();
    }
}

// --------------------------------------------------------------------------
QModelIndex qSlicerCompathTreeModel
::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    {
    return QModelIndex();
    }

  qSlicerCompathTreeItem* childItem = this->getItem(index);
  qSlicerCompathTreeItem* parentItem = childItem->parent();

  if (parentItem == this->rootItem)
    {
    return QModelIndex();
    }
  return this->createIndex(parentItem->row(), 0, parentItem);
}

// --------------------------------------------------------------------------
int qSlicerCompathTreeModel
::rowCount(const QModelIndex &parent) const
{
  qSlicerCompathTreeItem* parentItem;
  
  if (parent.column() > 0)
    {
    return 0;
    }

  if (!parent.isValid())
    {
    parentItem = this->rootItem;
    }
  else
    {
    parentItem = static_cast<qSlicerCompathTreeItem*>(parent.internalPointer());
    }

  return parentItem->childCount();
}

// --------------------------------------------------------------------------
int qSlicerCompathTreeModel
::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    {
    return static_cast<qSlicerCompathTreeItem*>(parent.internalPointer())->columnCount();
    }
  else
    {
    return this->rootItem->columnCount();
    }
}

// --------------------------------------------------------------------------
QVariant qSlicerCompathTreeModel
::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    {
    return QVariant();
    }

  qSlicerCompathTreeItem* item = static_cast<qSlicerCompathTreeItem*>(index.internalPointer());

  if (role == Qt::CheckStateRole && index.column() == 0)
    {
    return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);
    }

  if (role != Qt::DisplayRole)
    {
    return QVariant();
    }

  return item->data(index.column());
}

// --------------------------------------------------------------------------
Qt::ItemFlags qSlicerCompathTreeModel
::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    {
    return 0;
    }

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  
  if (index.column() == 0)
    {
    flags |= Qt::ItemIsUserCheckable;
    }

  return flags;
}

// --------------------------------------------------------------------------
QVariant qSlicerCompathTreeModel
::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
    return this->rootItem->data(section);
    }
  
  return QVariant();
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeModel
::setupModelData(qSlicerCompathTreeItem* parent)
{
  /*
  QVector<QVariant> myVariant;
  myVariant.append("DTC");
  myVariant.append("MotherFucker");
  qSlicerCompathTreeItem* item = 
    new qSlicerCompathTreeItem(myVariant, parent);
  parent->appendChild(item);
  */
}

// --------------------------------------------------------------------------
qSlicerCompathTreeItem* qSlicerCompathTreeModel
::getItem(const QModelIndex &index) const
{
  if (index.isValid())
    {
    qSlicerCompathTreeItem* item =
      static_cast<qSlicerCompathTreeItem*>(index.internalPointer());
    if (item)
      {
      return item;
      }
    }
  return rootItem;
}

// --------------------------------------------------------------------------
bool qSlicerCompathTreeModel
::setData(const QModelIndex& index, const QVariant& value,
          int role)
{
  if (!index.isValid())
    {
    return false;
    }
  
  qSlicerCompathTreeItem* item = this->getItem(index);
  if (index.column() == 0 && role == Qt::CheckStateRole)
    {
    item->setCheckState((Qt::CheckState)value.toInt());
    
    // Set all children same state as parent
    for (int child = 0; child < item->childCount(); child++)
      {
      QModelIndex childIndex = this->index(child, 0, index);
      if (&childIndex)
        {
        this->setData(childIndex, value, role);        
        emit dataChanged(childIndex, childIndex);
        }
      }

    // Turn on/off widget visibility based on check state
    bool checkStatus = (item->isChecked() != 0);
    if (item)
      {
      if (item->getPathNode())
        {
	// TODO: Turn off projection too ?
        item->getPathNode()->SetDisplayVisibility(checkStatus);
	if (item->getVirtualOffsetNode())
	  {
	  item->getVirtualOffsetNode()->SetDisplayVisibility(checkStatus);
	  }
        }
      if (item->getTargetNode())
        {
	// TODO: Turn off projection too ?
        item->getTargetNode()->SetDisplayVisibility(checkStatus);
        }
      }
    
    return true;
    }
  
  return QAbstractItemModel::setData(index, value, role);
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeModel
::addTrajectory(vtkMRMLAnnotationRulerNode* trajectoryNode, vtkMRMLAnnotationFiducialNode* targetNode)
{
  if (!trajectoryNode || !targetNode)
    {
    return;
    }

  // Top level
  QVector<QVariant> topLevel;
  topLevel.append(trajectoryNode->GetName());
  qSlicerCompathTreeItem *newParent 
    = new qSlicerCompathTreeItem(topLevel, this->rootItem);
  this->beginInsertRows(QModelIndex(), this->rootItem->childCount(), this->rootItem->childCount());
  this->rootItem->appendChild(newParent);
  this->endInsertRows();

  QModelIndex newParentIndex = this->index(this->rootItem->childCount()-1, 0, QModelIndex());

  // Path
  QVector<QVariant> pathItems;
  pathItems.append("Path");
  qSlicerCompathTreeItem* pathItem
    = new qSlicerCompathTreeItem(pathItems, newParent);
  pathItem->setPathNode(trajectoryNode);
  this->beginInsertRows(newParentIndex, newParent->childCount(), newParent->childCount());
  newParent->appendChild(pathItem);
  this->endInsertRows();

  // Fiducial
  std::stringstream tPos;
  tPos.precision(2);
  tPos.setf(std::ios::fixed);
  double* tCoord = targetNode->GetFiducialCoordinates();
  tPos << "Target (R: " <<
    tCoord[0] << ", A: " <<
    tCoord[1] << ", S: " <<
    tCoord[2] << ")";
   
  QVector<QVariant> targetItems;
  targetItems.append(tPos.str().c_str());
  qSlicerCompathTreeItem* targetItem
    = new qSlicerCompathTreeItem(targetItems, newParent);
  targetItem->setTargetNode(targetNode);
  this->beginInsertRows(newParentIndex, newParent->childCount(), newParent->childCount());
  newParent->appendChild(targetItem);
  this->endInsertRows();

  // TODO: Update fiducial with RAS coordinates
}
