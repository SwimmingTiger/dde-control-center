/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef FRAMEPROXYINTERFACE_H
#define FRAMEPROXYINTERFACE_H

class ContentWidget;
class ModuleInterface;

namespace dde {

class FrameProxyInterface
{
public:
    // Module request to into next page
    virtual void pushWidget(ModuleInterface * const inter, ContentWidget * const w) = 0;

    virtual void setFrameAutoHide(ModuleInterface * const inter, const bool autoHide) = 0;
};

}

#endif // FRAMEPROXYINTERFACE_H
