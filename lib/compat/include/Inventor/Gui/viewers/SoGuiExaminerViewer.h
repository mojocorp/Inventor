#ifndef _SO_GUI_EXAMINER_VIEWER_H
#define _SO_GUI_EXAMINER_VIEWER_H

class SoGuiExaminerViewer
{
public:
enum ViewerModes 
{
  PICK_MODE,          // Alt key can take us out of viewing mode
  VIEW_MODE,          // Equivalent to old IDLE_MODE
  SPIN_MODE = VIEW_MODE,
  SPIN_MODE_ACTIVE,   // Equivalent to old SPIN_MODE
  SPINX_MODE_ACTIVE,          // Rotation on X axis.
  SPINY_MODE_ACTIVE,          // Rotation on Y axis.
  SPINZ_MODE_ACTIVE,          // Rotation on Z axis.
  PAN_MODE,           // Just Ctrl key pressed (similar to old ROLL_MODE)
  PAN_MODE_ACTIVE,    // Equivalent to old TRANS_MODE
  DOLLY_MODE,          // 
  DOLLY_MODE_ACTIVE,  // Equivalent to old DOLLY_MODE
  SEEK_MODE,
  SPINX_VIEWER_MODE_ACTIVE,   // Rotation on X viewer axis.
  SPINY_VIEWER_MODE_ACTIVE,   // Rotation on Y viewer axis.
  SPINZ_VIEWER_MODE_ACTIVE,   // Rotation on Z viewer axis.
  REVERSE_DOLLY_MODE_ACTIVE, // A reversed DOLLY MODE
  ROLL_MODE,
  ROLL_MODE_ACTIVE, // Rotation centered around an anchor.
  EMPTY_MODE = -1
};
};

#endif //
