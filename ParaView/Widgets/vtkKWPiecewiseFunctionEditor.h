/*=========================================================================

Copyright (c) 1998-2003 Kitware Inc. 469 Clifton Corporate Parkway,
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
// .NAME vtkKWPiecewiseFunctionEditor - a piecewise function editor
// .SECTION Description
// A widget that allows the user to edit a piecewise function.

#ifndef __vtkKWPiecewiseFunctionEditor_h
#define __vtkKWPiecewiseFunctionEditor_h

#include "vtkKWParameterValueFunctionEditor.h"

class vtkPiecewiseFunction;

class VTK_EXPORT vtkKWPiecewiseFunctionEditor : public vtkKWParameterValueFunctionEditor
{
public:
  static vtkKWPiecewiseFunctionEditor* New();
  vtkTypeRevisionMacro(vtkKWPiecewiseFunctionEditor,vtkKWParameterValueFunctionEditor);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get/Set the function
  vtkGetObjectMacro(PiecewiseFunction, vtkPiecewiseFunction);
  virtual void SetPiecewiseFunction(vtkPiecewiseFunction*);

  // Description:
  // Set/Get the window/level mode. In that mode:
  // - the end-points parameter are locked (similar to LockEndPointsParameter)
  // - no point can be added or removed (similar to DisableAddAndRemove)
  // - the first and second point have the same value (they move together)
  // - the last and last-1 point have the same value (they move together) 
  virtual void SetWindowLevelMode(int);
  vtkBooleanMacro(WindowLevelMode, int);
  vtkGetMacro(WindowLevelMode, int);

  // Description:
  // Set/Get the window/level lock mode. In that mode, provided that
  // WindowLevelMode is On:
  // - the last and last-1 points values are locked (expected to be the same)
  vtkSetMacro(WindowLevelModeLockEndPointValue, int);
  vtkBooleanMacro(WindowLevelModeLockEndPointValue, int);
  vtkGetMacro(WindowLevelModeLockEndPointValue, int);

  // Description:
  // Set/Get the window/level.
  virtual void SetWindowLevel(float window, float level);

  // Description:
  // Set commands.
  virtual void InvokeFunctionChangedCommand();
  virtual void InvokeFunctionChangingCommand();

protected:
  vtkKWPiecewiseFunctionEditor();
  ~vtkKWPiecewiseFunctionEditor();

  vtkPiecewiseFunction *PiecewiseFunction;

  int WindowLevelMode;
  int WindowLevelModeLockEndPointValue;
  float Window;
  float Level;

  virtual void UpdateWindowLevelPoints();

  // Description:
  // Is point locked, protected, removable ?
  virtual int FunctionPointCanBeAdded();
  virtual int FunctionPointCanBeRemoved(int id);
  virtual int FunctionPointParameterIsLocked(int id);
  virtual int FunctionPointValueIsLocked(int id);

  // Description:
  // Proxy to the function
  virtual int  HasFunction();
  virtual int  GetFunctionSize();
  virtual int  GetFunctionPointColor(int id, float rgb[3]);
  virtual int  GetFunctionPointParameter(int id, float &parameter);
  virtual int  GetFunctionPointCanvasCoordinates(int id, int &x, int &y);
  virtual int  AddFunctionPointAtCanvasCoordinates(int x, int y, int &id);
  virtual int  AddFunctionPointAtParameter(float parameter, int &id);
  virtual int  MoveFunctionPointToCanvasCoordinates(int id,int x,int y);
  virtual int  MoveFunctionPointToParameter(int id, float parameter, int i=0);
  virtual int  RemoveFunctionPoint(int id);
  virtual void UpdateInfoLabelWithFunctionPoint(int id);
  virtual unsigned long GetFunctionMTime();

private:
  vtkKWPiecewiseFunctionEditor(const vtkKWPiecewiseFunctionEditor&); // Not implemented
  void operator=(const vtkKWPiecewiseFunctionEditor&); // Not implemented
};

#endif

