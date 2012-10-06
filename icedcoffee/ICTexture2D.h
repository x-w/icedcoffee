//
//  Copyright (C) 2012 Tobias Lensing, Marcus Tillmanns
//  http://icedcoffee-framework.org
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to
//  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is furnished to do
//  so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

/*
 
ORIGINAL LICENSE:

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: Texture2D.h
Abstract: Creates OpenGL 2D textures from images or text.

Version: 1.6

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes aICeptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "icMacros.h"

#ifdef __IC_PLATFORM_IOS
#import <UIKit/UIKit.h>	// for UIImage
#endif

#import <Foundation/Foundation.h>

#import "Platforms/ICGL.h"
#import "Platforms/ICNS.h"
#import "icTypes.h"

/**
 @brief Represents an immutable two-dimensional OpenGL texture
 
 The ICTexture2D class provides methods allowing you to conveniently create and work with
 immutable two-dimensional OpenGL textures.
 */
@interface ICTexture2D : NSObject
{
@protected
	GLuint				_name;
	CGSize				_contentSizeInPixels;
	NSUInteger			_width,
						_height;
	ICPixelFormat		_format;
	GLfloat				_maxS,
						_maxT;
	BOOL				_hasPremultipliedAlpha;
    ICResolutionType    _resolutionType;
}

#pragma mark - Initializing a Texture with Data
/** @name Initializing a Texture with Data */

/** @cond */ // Exclude from docs
- (id)init __attribute__((unavailable));
/** @endcond */

/**
 @brief Initializes a texture with the given data, pixel format, size and resolution type
 
 @param data A buffer containing the data to be uploaded to the OpenGL texture
 @param pixelFormat An ICPixelFormat enumerated value defining the texture's pixel format
 @param textureSizeInPixels The size of the texture in pixels
 @param contentSizeInPixels The size of the texture's contents in pixels
 @param resolutionType An ICResolutionType enumerated value defining the texture's resolution type
 
 The given ``data`` must contain pixels formatted as defined by the specified ``pixelFormat``.
 The most common pixel format in icedcoffee is ICPixelFormatRGBA8888.
 ``textureSizeInPixels`` may differ from ``contentSizeInPixels`` in cases where power of two
 textures must be used to store non-power of two (NPOT) contents. The former defines the size of
 the texture in memory whereas the latter defines the size of the content stored in the texture.
 The ``resolutionType`` argument specifies the native resolution of the texture. If the texture
 represents a high resolution (retina display) image, you should set this to
 ICResolutionTypeRetinaDisplay. Otherwise, this should be set to ICResolutionTypeStandard.
 */
- (id)initWithData:(const void*)data
       pixelFormat:(ICPixelFormat)pixelFormat
       textureSize:(CGSize)textureSizeInPixels
       contentSize:(CGSize)contentSizeInPixels
    resolutionType:(ICResolutionType)resolutionType;

/**
 @brief Initializes a texture with the given data, pixel format, width, height and content size
 in pixels
 
 @param data A buffer containing the data to be uploaded to the OpenGL texture
 @param pixelFormat An ICPixelFormat enumerated value defining the texture's pixel format
 @param pixelsWide The width of the texture in pixels
 @param pixelsHigh The height of the texture in pixels
 @param size The size of the contents in pixels
 
 @deprecated Deprecated as of v0.6.6. Use
 ICTexture2D::initWithData:pixelFormat:textureSize:contentSize:resolutionType: instead.
 */
- (id)initWithData:(const void*)data
       pixelFormat:(ICPixelFormat)pixelFormat
        pixelsWide:(NSUInteger)width
        pixelsHigh:(NSUInteger)height
              size:(CGSize)contentSizeInPixels DEPRECATED_ATTRIBUTE /*v0.6.6*/;


#pragma mark - Initializing a Texture with a CGImage
/** @name Initializing a Texture with a CGImage */

/*
 Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
 */

#ifdef __IC_PLATFORM_MAC

/**
 @brief Initializes a texture with the given ``CGImageRef``
 
 This method internally calls ICTexture2D::initWithCGImage:resolutionType: and specifies
 ICResolutionTypeUnknown as the resolution type.
 */
- (id)initWithCGImage:(CGImageRef)cgImage;

#endif

/**
 @brief Initializes a texture with the given ``CGImageRef`` and resolution type
 */
- (id)initWithCGImage:(CGImageRef)cgImage resolutionType:(ICResolutionType)resolution;


#pragma mark - Initializing a Texture with Text
/** @name Initializing a Texture with Text */

/**
 @brief Initializes a texture from a string with dimensions, alignment, font name and font size
 */
- (id)initWithString:(NSString*)string
          dimensions:(CGSize)dimensions
           alignment:(ICTextAlignment)alignment
            fontName:(NSString*)name fontSize:(CGFloat)size;

/**
 @brief Initializes a texture from a string with font name and font size
 */
- (id)initWithString:(NSString*)string
            fontName:(NSString*)name
            fontSize:(CGFloat)size;


/**
 @brief Returns the pixel format of the receiver
 */
@property (nonatomic, readonly) ICPixelFormat pixelFormat;

/**
 @brief Returns the width of the receiver in pixels
 
 @deprecated Deprecated as of v0.6.7. Use ICTexture2D::sizeInPixels instead.
 */
@property (nonatomic, readonly) NSUInteger pixelsWide DEPRECATED_ATTRIBUTE /*v0.6.7*/;
/**
 @brief Returns the height of the receiver in pixels

 @deprecated Deprecated as of v0.6.7. Use ICTexture2D::sizeInPixels instead.
 */
@property (nonatomic, readonly) NSUInteger pixelsHigh DEPRECATED_ATTRIBUTE /*v0.6.7*/;

/** 
 @brief Returns the OpenGL texture name of the receiver
 */
@property (nonatomic, readonly) GLuint name;

/**
 @brief Returns the content size of the receiver in pixels
 */
@property (nonatomic, readonly, nonatomic) CGSize contentSizeInPixels;

/** texture max S */
@property (nonatomic, readwrite) GLfloat maxS;
/** texture max T */
@property (nonatomic, readwrite) GLfloat maxT;
/** whether or not the texture has their Alpha premultiplied */
@property (nonatomic, readonly) BOOL hasPremultipliedAlpha;

@property (nonatomic, readwrite) ICResolutionType resolutionType;

/**
 @brief Returns the content size of the receiver in points
 */
- (CGSize)contentSize;

/**
 @brief Returns the content size of the receiver in points scaled with regard to its resolution
 type and the global content scale factor
 */
- (CGSize)displayContentSize;

/**
 @brief Returns the size of the receiver in points
 */
- (CGSize)size DEPRECATED_ATTRIBUTE /*v0.6.6*/;

/**
 @brief Returns the size of the receiver in pixels
 */
- (CGSize)sizeInPixels DEPRECATED_ATTRIBUTE /*v0.6.6*/;

@end


/**
 Extension to set the Min / Mag filter
 */
typedef struct _ICTexParams {
	GLuint	minFilter;
	GLuint	magFilter;
	GLuint	wrapS;
	GLuint	wrapT;
} ICTexParams;

@interface ICTexture2D (GLFilter)
/** sets the min filter, mag filter, wrap s and wrap t texture parameters.
 If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
 @since v0.8
 */
-(void) setTexParameters: (ICTexParams*) texParams;

/** sets antialias texture parameters:
  - GL_TEXTURE_MIN_FILTER = GL_LINEAR
  - GL_TEXTURE_MAG_FILTER = GL_LINEAR

 @since v0.8
 */
- (void) setAntiAliasTexParameters;

/** sets alias texture parameters:
  - GL_TEXTURE_MIN_FILTER = GL_NEAREST
  - GL_TEXTURE_MAG_FILTER = GL_NEAREST
 
 @since v0.8
 */
- (void) setAliasTexParameters;


/** Generates mipmap images for the texture.
 It only works if the texture size is POT (power of 2).
 @since v0.99.0
 */
-(void) generateMipmap;


@end

@interface ICTexture2D (PixelFormat)
/** sets the default pixel format for UIImages that contains alpha channel.
 If the UIImage contains alpha channel, then the options are:
	- generate 32-bit textures: ICPixelFormatRGBA8888 (default one)
	- generate 16-bit textures: ICPixelFormatRGBA4444
	- generate 16-bit textures: ICPixelFormatRGB5A1
	- generate 16-bit textures: ICPixelFormatRGB565
	- generate 8-bit textures: ICPixelFormatA8 (only use it if you use just 1 color)

 How does it work ?
   - If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
   - If the image is an RGB (without Alpha) then an RGB565 texture will be used (16-bit texture)
 
 This parameter is not valid for PVR images.
 
 @since v0.8
 */
+(void) setDefaultAlphaPixelFormat:(ICPixelFormat)format;

/** returns the alpha pixel format
 @since v0.8
 */
+(ICPixelFormat) defaultAlphaPixelFormat;
@end





