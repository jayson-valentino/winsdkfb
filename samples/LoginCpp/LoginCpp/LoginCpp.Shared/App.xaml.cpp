﻿//******************************************************************************
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//******************************************************************************

//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace LoginCpp;

using namespace concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace winsdkfb;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
using namespace Windows::Phone::UI::Input;
#endif

// The Blank Application template is documented at http://go.microsoft.com/fwlink/?LinkId=234227

/// <summary>
/// Initializes the singleton application object. This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
    HardwareButtons::BackPressed += ref new Windows::Foundation::EventHandler<BackPressedEventArgs^>(this, &LoginCpp::App::OnBackRequested);
#endif
}

Frame^ App::CreateRootFrame()
{
    Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context 
        rootFrame = ref new Frame();

        // Set the default language
        rootFrame->Language = Windows::Globalization::ApplicationLanguages::Languages->GetAt(0);

        // Place the frame in the current Window
        Window::Current->Content = rootFrame;
    }

    return rootFrame;
}

/// <summary>
/// Invoked when the application is launched normally by the end user. Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs^ e)
{
#if _DEBUG
	if (IsDebuggerPresent())
	{
		DebugSettings->EnableFrameRateCounter = true;
	}
#endif

	auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

	// Do not repeat app initialization when the Window already has content,
	// just ensure that the window is active.
	if (rootFrame == nullptr)
	{
		// Create a Frame to act as the navigation context and associate it with
		// a SuspensionManager key
		rootFrame = ref new Frame();

		// TODO: Change this value to a cache size that is appropriate for your application.
		rootFrame->CacheSize = 1;

		if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
		{
			// TODO: Restore the saved session state only when appropriate, scheduling the
			// final launch steps after the restore is complete.
		}

		// Place the frame in the current Window
		Window::Current->Content = rootFrame;
	}

	if (rootFrame->Content == nullptr)
	{
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
		// Removes the turnstile navigation for startup.
		if (rootFrame->ContentTransitions != nullptr)
		{
			_transitions = ref new TransitionCollection();
			for (auto transition : rootFrame->ContentTransitions)
			{
				_transitions->Append(transition);
			}
		}

		rootFrame->ContentTransitions = nullptr;
		_firstNavigatedToken = rootFrame->Navigated += ref new NavigatedEventHandler(this, &App::RootFrame_FirstNavigated);
#endif

		// When the navigation stack isn't restored navigate to the first page,
		// configuring the new page by passing required information as a navigation
		// parameter.
		if (!rootFrame->Navigate(MainPage::typeid, e->Arguments))
		{
			throw ref new FailureException("Failed to create initial page");
		}
	}

	// Ensure the current window is active
	Window::Current->Activate();
}

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
/// <summary>
/// Restores the content transitions after the app has launched.
/// </summary>
void App::RootFrame_FirstNavigated(Object^ sender, NavigationEventArgs^ e)
{
	auto rootFrame = safe_cast<Frame^>(sender);

	TransitionCollection^ newTransitions;
	if (_transitions == nullptr)
	{
		newTransitions = ref new TransitionCollection();
		newTransitions->Append(ref new NavigationThemeTransition());
	}
	else
	{
		newTransitions = _transitions;
	}

	rootFrame->ContentTransitions = newTransitions;

	rootFrame->Navigated -= _firstNavigatedToken;
}
#endif

/// <summary>
/// Invoked when application execution is being suspended. Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;		// Unused parameter

	// TODO: Save application state and stop any background activity
}

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
void LoginCpp::App::OnBackRequested(Platform::Object^ sender, BackPressedEventArgs^ args)
{
    Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if (rootFrame->CanGoBack)
    {
        args->Handled = true;
        rootFrame->GoBack();
    }
}
#endif
