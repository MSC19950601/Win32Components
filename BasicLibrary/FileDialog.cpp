#include "FileDialog.h"

Yupei::FileDialog::FileDialog(HWND _parentWindow)
{
	fileName.hwndOwner = _parentWindow;
}
