//----------------------------------------------------------------------
// file: application_window.h
// description: Class for initializing an OpenGL window with the SDL
//              used by the OpenGL tutorials at www.tomprogs.at
//----------------------------------------------------------------------
// Copyright (c) 2008-2011, Thomas Geymayer
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
//      copyright notice, this list of conditions and the following
//      disclaimer in the documentation and/or other materials provided
//      with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------


#ifndef APPLICATION_WINDOW_H_
#define APPLICATION_WINDOW_H_

#include "SDL_video.h"
#include <string>

namespace TomprogsTutorial_
{

  //--------------------------------------------------------------------
  ///
  /// ApplicationWindow
  ///
  /// @brief The window the application renders into
  /// @details
  ///
  /// @author <Thomas Geymayer tomgey@gmail.com>
  /// @version <0.0.2>

  class ApplicationWindow
  {
    public:

      //----------------------------------------------------------------
      ///
      /// @brief Constructor
      ///

      ApplicationWindow( int width = 0, int height = 0, int bpp = 0);

      //----------------------------------------------------------------
      ///
      /// @brief Destructor
      ///

      virtual ~ApplicationWindow();

      //----------------------------------------------------------------
      ///
      /// @brief Generates a window with the stored parameters
      ///
      /// Generates a new window with the stored parameters.
      /// If fullscreen is enabled, the new displaymode is set before.
      ///
      /// @return Returns true on success

      bool create();

      //----------------------------------------------------------------
      ///
      /// @brief Destroys this window
      ///
      /// Destroys the window an frees his surface
      ///
      /// @return Returns true on success

      bool destroy();

      //----------------------------------------------------------------
      ///
      /// @brief Set the wished window resolution
      ///
      /// Sets the resolution of the window and changes displaymode if
      /// fullscreen is enabled.

      void setResolution( int width, int height );

      //----------------------------------------------------------------
      ///
      /// @brief Set the wished color depth
      ///
      /// Sets the color depth of the window and changes displaymode if
      /// fullscreen is enabled.

      void setColorDepth( int bpp );

      //----------------------------------------------------------------
      ///
      /// @brief Set the caption of the game window
      ///
      /// Sets the text in the titlebar of the window in windowed mode
      /// and the name of the application in the taskbar or taskmanager.

      void setCaption( const std::string& caption );

      //----------------------------------------------------------------
      ///
      /// @brief En-/Disables fullscreen mode
      ///
      /// Depending on the given parameter enables or disables
      /// fullscreen mode.
      ///
      /// @param enable If true fullscreen will be enabled

      void setFullscreen( bool enable = true );

      //----------------------------------------------------------------
      ///
      /// @brief Get surface of this window
      ///
      /// Get the SDL surface of this window
      ///
      /// @return Pointer to SDL_Surface

      SDL_Surface* getSurface();

      enum
      {
        OPENED_WINDOW = 0x0001,
        FULL_SCREEN   = 0x0002
      };

    private:

      //----------------------------------------------------------------

      int           _width;
      int           _height;
      int           _bpp;

      std::string   _caption;

      SDL_Surface  *_surface;
      char          _state;

      //----------------------------------------------------------------
      ///
      /// Don't allow any copying of this class

      ApplicationWindow(const ApplicationWindow& src);
      ApplicationWindow& operator=(const ApplicationWindow& src);
  };

} // namespace TomprogsTutorial_

#endif /*APPLICATION_WINDOW_H_*/

