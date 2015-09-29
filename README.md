# sandboxer
This is a 'proof of concept' of isolating Qt plugins into a separate
process (sandbox). The process is launched by the host application
and communication channel is established between the host and hosted
process (via named pipe).

Hosted process communicates then its main window handle (HWND) so that
the host process could embed it naturally into the application.

Hosted process lifetime is controlled by the host application. In case of
a crash in the hosted process it can be restarted without putting down
the main application.

# Building
CMake is required to configure and build the project.
Build depends on Qt 5.x framework.

#Limitations
Current sanboxing implementation works only on MS Windows system, but
similar technique can be used on X11 as well.