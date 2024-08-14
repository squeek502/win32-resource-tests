#ifndef CUSTOM_H
#define CUSTOM_H

#include <tchar.h>
#include <windows.h>


/* Window class */
#define CUSTOM_WC   _T("CustomControl")

/* Register/unregister the window class */
void CustomRegister(void);
void CustomUnregister(void);


#endif  /* CUSTOM_H */