/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSocketController.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSocketController - Process communication using Sockets
// .SECTION Description
// This is a concrete implementation of vtkMultiProcessController.
// It supports one-to-one communication using sockets. Note that
// process 0 will always correspond to self and process 1 to the
// remote process. This class is best used with ports.

// .SECTION Bugs
// Note that because process 0 will always correspond to self, this class breaks
// assumptions usually implied when using ad-hoc polymorphism.  That is, the
// vtkSocketController will behave differently than other subclasses of
// vtkMultiProcessController.  If you upcast vtkSocketController to
// vtkMultiProcessController and send it to a method that does not know that the
// object is actually a vtkSocketController, the object may not behave as
// intended.  For example, if that oblivious class chose to identify a "root"
// based on the local process id, then both sides of the controller will think
// they are the root (and that will probably lead to deadlock).  If you plan to
// upcast to vtkMultiProcessController, you should probably use the
// CreateCompliantController instead.

// .SECTION see also
// vtkMultiProcessController vtkSocketCommunicator vtkInputPort vtkOutputPort

#ifndef __vtkSocketController_h
#define __vtkSocketController_h

#include "vtkMultiProcessController.h"

class vtkSocketCommunicator;

class VTK_PARALLEL_EXPORT vtkSocketController : public vtkMultiProcessController
{
public:
  static vtkSocketController *New();
  vtkTypeMacro(vtkSocketController,vtkMultiProcessController);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // This method is for initialiazing sockets.
  // One of these is REQUIRED for Windows.
  virtual void Initialize(int* argc, char*** argv, int)
    { this->Initialize(argc,argv); }
  virtual void Initialize(int* argc, char*** argv); 
  virtual void Initialize()
    { this->Initialize(0,0); }

  // Description:
  // Does not apply to sockets. Does nothing.
  void Finalize() {};
  void Finalize(int) {};

  // Description:
  //  Does not apply to sockets. Does nothing.
  void SingleMethodExecute() {};
  
  // Description:
  //  Does not apply to sockets.  Does nothing.
  void MultipleMethodExecute() {};

  // Description:
  //  Does not apply to sockets. Does nothing.
  void CreateOutputWindow() {};

  // Description:
  // Wait for connection on a given port, forwarded
  // to the communicator
  virtual int WaitForConnection(int port);

  // Description:
  // Close a connection, forwarded
  // to the communicator
  virtual void CloseConnection();

  // Description:
  // Open a connection to a give machine, forwarded
  // to the communicator
  virtual int ConnectTo( char* hostName, int port );

  int GetSwapBytesInReceivedData();

  // Description:
  // Set the communicator used in normal and rmi communications.
  void SetCommunicator(vtkSocketCommunicator* comm);

  // Description:
  // FOOLISH MORTALS!  Thou hast forsaken the sacred laws of ad-hoc polymorphism
  // when thou broke a critical assumption of the superclass (namely, each
  // process has thine own id).  The time frame to fix thy error has passed.
  // Too much code has come to rely on this abhorrent behavior.  Instead, we
  // offer this gift: a method for creating an equivalent communicator with
  // correct process id semantics.  The calling code is responsible for
  // deleting this controller.
  vtkMultiProcessController *CreateCompliantController();

//BTX

  enum Consts {
    ENDIAN_TAG=1010580540,      // 0x3c3c3c3c
    IDTYPESIZE_TAG=1027423549,  // 0x3d3d3d3d
    VERSION_TAG=1044266558,     // 0x3e3e3e3e
    HASH_TAG=0x3f3f3f3f
  };
  
//ETX

protected:

  vtkSocketController();
  ~vtkSocketController();

  // Initialize only once, finialize on destruction.
  static int Initialized;
private:
  vtkSocketController(const vtkSocketController&);  // Not implemented.
  void operator=(const vtkSocketController&);  // Not implemented.
};


#endif // __vtkSocketController_h
