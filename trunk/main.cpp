/*
 * Wars: Commando
 * Copyright (C) 2011 Teemu Erkkola
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtGui/QApplication>
#include "gamewindow.h"

#ifdef OPENGL
#include <QGLWidget>
#endif

#ifdef Q_WS_MAC
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFBundle.h>
#endif


int main(int argc, char *argv[])
{
#ifdef Q_WS_MAC
     //resolve .app-bundle path
     CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
     CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                            kCFURLPOSIXPathStyle);
     const char* pathPtr = CFStringGetCStringPtr(macPath,
                                            CFStringGetSystemEncoding());
     std::string path(pathPtr);

     //append resources dir
     path += "/Contents/Resources";
     chdir(path.c_str()); //change working directory

     CFRelease(appUrlRef);
     CFRelease(macPath);
#endif
    QApplication::setGraphicsSystem("raster");
    QApplication a(argc, argv);

    a.setApplicationName("WarsCommando");
    a.setApplicationVersion("0.1");
    GameWindow w;
#ifdef OPENGL
    w.setViewport(new QGLWidget());
#endif
    w.show();
    w.setFocus();
    w.activateWindow();
    w.raise();
    return a.exec();
}
