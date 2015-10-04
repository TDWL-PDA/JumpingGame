#include "pch.h"
#include "Jumping.h"
#include "BasicTimer.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

Jumping::Jumping() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

void Jumping::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &Jumping::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &Jumping::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &Jumping::OnResuming);

	int highScore = getGameData();
	m_renderer = ref new Renderer(highScore);
}

void Jumping::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &Jumping::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Jumping::OnWindowClosed);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Jumping::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Jumping::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Jumping::OnPointerReleased);

	m_renderer->Initialize(CoreWindow::GetForCurrentThread());
}

void Jumping::Load(Platform::String^ entryPoint)
{
}

void Jumping::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			m_renderer->Update(timer->Total, timer->Delta);
			m_renderer->Render();
			m_renderer->Present(); // This call is synchronized to the display frame rate.
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void Jumping::Uninitialize()
{
}

void Jumping::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void Jumping::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void Jumping::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	Windows::UI::Input::PointerPoint^ currentPoint = args->CurrentPoint;
	m_renderer->HandlePressInput(currentPoint);
}

void Jumping::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
	Windows::UI::Input::PointerPoint^ currentPoint = args->CurrentPoint;
	m_renderer->HandleMoveInput(currentPoint);
}

void Jumping::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	Windows::UI::Input::PointerPoint^ currentPoint = args->CurrentPoint;
	m_renderer->HandleReleaseInput(currentPoint);
}

void Jumping::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void Jumping::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
	m_renderer->ReleaseResourcesForSuspending();

	create_task([this, deferral]()
	{
		// Insert your code here.
		saveGameData();

		deferral->Complete();
	});
}
 
void Jumping::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
	 m_renderer->CreateWindowSizeDependentResources();
	 int highScore = getGameData();
	 m_renderer->setHighScore(highScore);
}

void Jumping::saveGameData()
{
	// get local folder (= isolated storage)
	auto local = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto localFileNamePlatformString = local->Path + "\\game.sav";
	FILE* pFile;

	// Write high score to memory
	auto fdsa = _wfopen_s(&pFile, localFileNamePlatformString->Data(), L"w");	// For writing
	fprintf(pFile, "%d", m_renderer->getHighScore());
	rewind(pFile);
	fclose(pFile);
	//fscanf_s(pFile, "%s", str);	// Doesn't work for some reason...look into later (fscan article on cplusplus.com)
}
int Jumping::getGameData()
{
	// get local folder (= isolated storage)
	auto local = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto localFileNamePlatformString = local->Path + "\\game.sav";

	// NOW IT WORKS!!!!  YESSSSSS!!!
	int highScore;
	FILE* pFile;

	// Read high score
	auto f = _wfopen_s(&pFile, localFileNamePlatformString->Data(), L"r");	// Open the file for reading
	if (f == 0)
	{
		fscanf_s(pFile, "%d", &highScore);
		auto res3 = fclose(pFile);	// Close the file
	}
	else highScore = 0;	// If there is no high score

	return highScore;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new Jumping();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}