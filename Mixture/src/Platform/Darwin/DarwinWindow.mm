#import "Platform/Darwin/DarwinWindow.h"
#ifdef OPAL_PLATFORM_DARWIN

#include "Mixture/Events/Event.hpp"
#include "Mixture/Events/ApplicationEvent.hpp"
#include "Mixture/Events/MouseEvent.hpp"
#include "Mixture/Events/KeyEvent.hpp"

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

// Define the delegate interface within the .mm file
@interface DarwinWindowDelegate : NSObject <NSWindowDelegate>
{
    Mixture::DarwinWindow* m_Owner; // Reference to the owning MacOSWindow
}
- (instancetype)initWithOwner:(Mixture::DarwinWindow*)owner;
@end

@implementation DarwinWindowDelegate
- (instancetype)initWithOwner:(Mixture::DarwinWindow*)owner
{
    self = [super init];
    if (self)
    {
        m_Owner = owner;
    }
    return self;
}

- (void)windowWillClose:(NSNotification *)notification
{
    m_Owner->OnClose();
}

- (void)windowDidResize:(NSNotification *)notification
{
    NSWindow* window = (NSWindow*)[notification object];
    NSRect frame = [window frame];
    m_Owner->OnResize((int)frame.size.width, (int)frame.size.height);
}
@end

namespace Mixture 
{
    
    DarwinWindow::DarwinWindow(const WindowProps& props)
    {
        Init(props);
    }

    DarwinWindow::~DarwinWindow() 
    {
        Shutdown();
    }

    void DarwinWindow::Init(const WindowProps& props) 
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        NSRect frame = NSMakeRect(0, 0, m_Data.Width, m_Data.Height);
        NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
        m_WindowHandle = [[NSWindow alloc] initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
        
        OPAL_CORE_ASSERT(m_WindowHandle, "Failed to create NSWindow!");

        NSString* title = [NSString stringWithUTF8String:m_Data.Title.c_str()];
        [(NSWindow*)m_WindowHandle setTitle:title];
        [(NSWindow*)m_WindowHandle makeKeyAndOrderFront:nil];
        
        // Assign the delegate
        DarwinWindowDelegate* delegate = [[DarwinWindowDelegate alloc] initWithOwner:this];
        [(NSWindow*)m_WindowHandle setDelegate:delegate];
    }

    void DarwinWindow::Shutdown() 
    {
        @autoreleasepool
        {
            if (m_WindowHandle)
            {
                // Clear the delegate to avoid callbacks to a deallocated object
                [(NSWindow*)m_WindowHandle setDelegate:nil];

                // Close and release the window
                [(NSWindow*)m_WindowHandle close];
                m_WindowHandle = nil;
            }
        }
    }

    void DarwinWindow::OnUpdate() 
    {
        // Process all pending events from the event queue
        @autoreleasepool 
        {
            NSEvent* event;
            while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES]) != nil)
            {
                [NSApp sendEvent:event];
                [NSApp updateWindows];
                
                OnEvent((void*)event);
            }
        }
    }

    void DarwinWindow::OnEvent(void* e)
    {
        if (!m_EventCallback) return;
        
        NSEvent* event = (NSEvent*)e;
        
        switch (event.type)
        {
            case NSEventTypeKeyDown:
            {
                KeyPressedEvent e((int)event.keyCode, 0); // Replace 0 with actual repeat count if needed
                m_EventCallback(e);
                break;
            }
            case NSEventTypeKeyUp:
            {
                KeyReleasedEvent e((int)event.keyCode);
                m_EventCallback(e);
                break;
            }
            case NSEventTypeMouseMoved:
            {
                NSPoint pos = [event locationInWindow];
                MouseMovedEvent e((float)pos.x, (float)pos.y);
                m_EventCallback(e);
                break;
            }
            case NSEventTypeLeftMouseDown:
            case NSEventTypeRightMouseDown:
            case NSEventTypeOtherMouseDown:
            {
                MouseButtonPressedEvent e((int)event.buttonNumber);
                m_EventCallback(e);
                break;
            }
            case NSEventTypeLeftMouseUp:
            case NSEventTypeRightMouseUp:
            case NSEventTypeOtherMouseUp:
            {
                MouseButtonReleasedEvent e((int)event.buttonNumber);
                m_EventCallback(e);
                break;
            }
            case NSEventTypeScrollWheel:
            {
                float deltaX = (float)[event scrollingDeltaX];
                float deltaY = (float)[event scrollingDeltaY];
                MouseScrolledEvent e(deltaX, deltaY);
                m_EventCallback(e);
                break;
            }
            default:
                // Handle other events as needed
                break;
        }
    }

    void DarwinWindow::OnResize(int width, int height)
    {
        m_Data.Width = width;
        m_Data.Height = height;

        if (m_EventCallback)
        {
            WindowResizeEvent event(width, height);
            m_EventCallback(event);
        }
    }

    void DarwinWindow::OnClose()
    {
        if (m_EventCallback)
        {
            WindowCloseEvent event;
            m_EventCallback(event);
        }
    }
}

#endif
