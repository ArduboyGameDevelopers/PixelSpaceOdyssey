//
//  image.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef image_h
#define image_h

#include "common.h"

class Image
{
private:
    PgmPtr _data;
    
public:
    Image(PgmPtr data) :
        _data(data)
    {
    }
    
public:
    inline PgmPtr  imagePtr() const { return _data + 2; }
    inline uint8_t width()    const { return pgm_read_byte(_data); }
    inline uint8_t height()   const { return pgm_read_byte(_data + 1); }
};

#endif /* image_h */
