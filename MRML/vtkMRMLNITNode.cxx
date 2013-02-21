#include "vtkMRMLNITNode.h"

#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLNITNode);


//----------------------------------------------------------------------------
vtkMRMLNITNode::vtkMRMLNITNode()
{
  this->HideFromEditors = false;
}

//----------------------------------------------------------------------------
vtkMRMLNITNode::~vtkMRMLNITNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLNITNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
}


//----------------------------------------------------------------------------
void vtkMRMLNITNode::ReadXMLAttributes(const char** atts)
{
  Superclass::ReadXMLAttributes(atts);
}

//----------------------------------------------------------------------------
void vtkMRMLNITNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);
}

//-----------------------------------------------------------
void vtkMRMLNITNode::UpdateScene(vtkMRMLScene *scene)
{
  Superclass::UpdateScene(scene);
}

//---------------------------------------------------------------------------
void vtkMRMLNITNode::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, 
                                           void *callData )
{
  Superclass::ProcessMRMLEvents(caller, event, callData);
}
