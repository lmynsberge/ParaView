/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPVStringEntry.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2000-2001 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of Kitware nor the names of any contributors may be used
   to endorse or promote products derived from this software without specific 
   prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkPVStringEntry.h"
#include "vtkObjectFactory.h"

//----------------------------------------------------------------------------
vtkPVStringEntry* vtkPVStringEntry::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkPVStringEntry");
  if (ret)
    {
    return (vtkPVStringEntry*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkPVStringEntry;
}

vtkPVStringEntry::vtkPVStringEntry()
{
  this->Label = vtkKWLabel::New();
  this->Label->SetParent(this);
  this->Entry = vtkKWEntry::New();
  this->Entry->SetParent(this);

  this->PVSource = NULL;
}

vtkPVStringEntry::~vtkPVStringEntry()
{
  this->Entry->Delete();
  this->Entry = NULL;
  this->Label->Delete();
  this->Label = NULL;
}

void vtkPVStringEntry::Create(vtkKWApplication *pvApp, char *label,
                              char *setCmd, char *getCmd, char *help,
                              const char *tclName)
{
  const char* wname;
  
  if (this->Application)
    {
    vtkErrorMacro("StringEntry already created");
    return;
    }
  if ( ! this->PVSource)
    {
    vtkErrorMacro("PVSource must be set before calling Create");
    return;
    }

  // For getting the widget in a script.
  this->SetName(label);
  
  this->SetApplication(pvApp);
  
  this->SetSetCommand(setCmd);
  this->SetGetCommand(getCmd);
  
  // create the top level
  wname = this->GetWidgetName();
  this->Script("frame %s -borderwidth 0 -relief flat", wname);
  
  // Now a label
  if (label && label[0] != '\0')
    {
    this->Label->Create(pvApp, "-width 18 -justify right");
    this->Label->SetLabel(label);
    if (help)
      {
      this->Label->SetBalloonHelpString(help);
      }
    this->Script("pack %s -side left", this->Label->GetWidgetName());
    }
  
  // Now the entry
  this->Entry->Create(pvApp, "");
  this->Script("%s configure -xscrollcommand {%s XScrollCallback}",
               this->Entry->GetWidgetName(), this->GetTclName());
  if (help)
    { 
    this->Entry->SetBalloonHelpString(help);
    }
  this->Script("pack %s -side left -fill x -expand t",
               this->Entry->GetWidgetName());
  
  // Command to update the UI.
  this->ResetCommands->AddString("%s SetValue [%s %s]",
                                 this->Entry->GetTclName(), tclName, getCmd); 
  // Format a command to move value from widget to vtkObjects (on all
  // processes).
  // The VTK objects do not yet have to have the same Tcl name!
  //this->AcceptCommands->AddString("%s AcceptHelper2 %s %s [list [%s GetValue]]",
  //                                this->PVSource->GetTclName(), tclName,
  //                                setCmd, this->Entry->GetTclName());
  this->AcceptCommands->AddString("%s %s [%s GetValue]",
                                  tclName, setCmd, this->Entry->GetTclName());
}


void vtkPVStringEntry::XScrollCallback(float x, float y)
{
  this->ModifiedCallback();
}


void vtkPVStringEntry::SetValue(const char* fileName)
{
  const char *old;
  
  if (fileName == NULL)
    {
    fileName = "";
    }

  old = this->Entry->GetValue();
  if (strcmp(old, fileName) == 0)
    {
    return;
    }

  this->Entry->SetValue(fileName); 
  this->ModifiedCallback();
}



void vtkPVStringEntry::Accept()
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  if (this->ModifiedFlag && this->PVSource)
    {  
    if ( ! this->TraceInitialized)
      {
      pvApp->AddTraceEntry("set pv(%s) [$pv(%s) GetPVWidget {%s}]",
                           this->GetTclName(), this->PVSource->GetTclName(),
                           this->Name);
      this->TraceInitialized = 1;
      }

    pvApp->AddTraceEntry("$pv(%s) SetValue {%s}", this->GetTclName(), 
                         this->GetValue());
    }

  this->vtkPVWidget::Accept();
}

