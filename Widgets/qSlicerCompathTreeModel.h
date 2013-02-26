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

#ifndef __qSlicerCompathTreeModel_h
#define __qSlicerCompathTreeModel_h

#include <sstream>

#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QObject>

#include "qSlicerCompathTreeItem.h"

class qSlicerCompathTreeModel : public QAbstractItemModel
{
  Q_OBJECT

 public:
  qSlicerCompathTreeModel(QObject* parent = 0);
  ~qSlicerCompathTreeModel();
  
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role);
  Qt::ItemFlags flags(const QModelIndex &ind) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &parent = QModelIndex()) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  void addTrajectory(vtkMRMLAnnotationRulerNode* trajectoryNode,
                     vtkMRMLAnnotationFiducialNode* targetNode);
  qSlicerCompathTreeItem* getItem(const QModelIndex &index) const;
  bool removeRows(int position, int rows, const QModelIndex& parent);

 private:
  qSlicerCompathTreeItem* rootItem;
};

#endif
