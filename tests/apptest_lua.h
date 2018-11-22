#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include "IAppEventHandler.h"
#include "IInputEventHandler.h"

#include "LuaStateManager.h"

namespace nc = ncine;

/// My nCine event handler
class MyEventHandler :
	public nc::IAppEventHandler,
	public nc::IInputEventHandler
{
  public:
	MyEventHandler();
	void onPreInit(nc::AppConfiguration &config) override;
	void onInit() override;
	void onFrameStart() override;
	void onShutdown() override;

	void onKeyPressed(const nc::KeyboardEvent &event) override;
	void onKeyReleased(const nc::KeyboardEvent &event) override;

#ifdef __ANDROID__
	void onTouchDown(const nc::TouchEvent &event) override;
	void onTouchUp(const nc::TouchEvent &event) override;
	void onTouchMove(const nc::TouchEvent &event) override;
	void onPointerDown(const nc::TouchEvent &event) override;
	void onPointerUp(const nc::TouchEvent &event) override;
	void onAcceleration(const nc::AccelerometerEvent &event) override;
#endif

	void onMouseButtonPressed(const nc::MouseEvent &event) override;
	void onMouseButtonReleased(const nc::MouseEvent &event) override;
	void onMouseMoved(const nc::MouseState &state) override;
	void onScrollInput(const nc::ScrollEvent &event) override;

	void onJoyButtonPressed(const nc::JoyButtonEvent &event) override;
	void onJoyButtonReleased(const nc::JoyButtonEvent &event) override;
	void onJoyAxisMoved(const nc::JoyAxisEvent &event) override;

	void onJoyMappedButtonPressed(const nc::JoyMappedButtonEvent &event) override;
	void onJoyMappedButtonReleased(const nc::JoyMappedButtonEvent &event) override;
	void onJoyMappedAxisMoved(const nc::JoyMappedAxisEvent &event) override;

	void onJoyConnected(const nc::JoyConnectionEvent &event) override;
	void onJoyDisconnected(const nc::JoyConnectionEvent &event) override;

  private:
	nc::LuaStateManager luaState_;
};

#endif