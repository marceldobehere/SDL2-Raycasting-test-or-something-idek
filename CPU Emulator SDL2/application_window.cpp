
#include "application_window.h"

#include "SDL.h"
#include "SDL_opengl.h"

namespace TomprogsTutorial_
{

  //--------------------------------------------------------------------
  ApplicationWindow::ApplicationWindow(
                       int width,
                       int height, int bpp ):
                         _width  ( width                  ),
                         _height ( height                 ),
                         _bpp    ( bpp                    ),
                         _caption( "Tomprogs Application" ),
                         _state  ( 0                      )
  {

  }

  //--------------------------------------------------------------------
  ApplicationWindow::~ApplicationWindow()
  {
    destroy();
  }

  //--------------------------------------------------------------------
  bool ApplicationWindow::create()
  {
    // check if allready opened a window
    if( _state & OPENED_WINDOW )
      destroy();

    if(    !SDL_WasInit( SDL_INIT_VIDEO ) )
      SDL_InitSubSystem( SDL_INIT_VIDEO );

    SDL_WM_SetCaption(   _caption.c_str(), NULL );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    _surface =
      SDL_SetVideoMode(
        _width, _height, _bpp,
        SDL_OPENGL | ( (_state & FULL_SCREEN) ? SDL_FULLSCREEN : 0 ) );

    if( !_surface ) return false;

    // disable cursor
    SDL_ShowCursor( 0 );

    _state |= OPENED_WINDOW;

    return true;
  }

  //--------------------------------------------------------------------
  bool ApplicationWindow::destroy()
  {
    // return if no window is opened
    if( !(_state & OPENED_WINDOW) ) return true;

    SDL_FreeSurface( _surface );

    _state &= ~OPENED_WINDOW;

    return true;
  }

  //--------------------------------------------------------------------
  void ApplicationWindow::setResolution( int width, int height )
  {
    _width  = width;
    _height = height;
  }

  //--------------------------------------------------------------------
  void ApplicationWindow::setColorDepth( int bpp )
  {
    _bpp = bpp;
  }

  //--------------------------------------------------------------------
  void ApplicationWindow::setCaption( const std::string& caption )
  {
    _caption = caption;
  }

  //--------------------------------------------------------------------
  void ApplicationWindow::setFullscreen( bool enable )
  {
    if( enable )
    {
      _state |= FULL_SCREEN;
    }
    else
    {
      _state &= ~FULL_SCREEN;
    }
  }

  //--------------------------------------------------------------------
  SDL_Surface* ApplicationWindow::getSurface()
  {
    return _surface;
  }

} // namespace TomprogsTutorial_

