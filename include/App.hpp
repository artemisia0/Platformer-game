#pragma once

#include <AppElement.hpp>
#include <StateStack.hpp>
#include <Context.hpp>
#include <State.hpp>
#include <Signal.hpp>
#include <Resources.hpp>
#include <MusicPlayer.hpp>
#include <SoundPlayer.hpp>
#include <Parallax.hpp>

#include <deque>
#include <memory>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>


class StateStack;
class Context;
enum class SignalData;

class App : public AppElement, private sf::NonCopyable {
public:
	App();
	virtual ~App() = default;

	void run();


private:
	APP_ELEMENT;

	void handleSignals();
	void handleEvents();
	void handleStateStackSignal(const Signal&);

	std::unique_ptr<State> stateByID(SignalData);
	std::unique_ptr<Context> defaultContext();
	std::unique_ptr<sf::RenderWindow> makeRenderWindow();
	std::unique_ptr<MusicPlayer> defaultMusicPlayer();
	std::unique_ptr<SoundPlayer> defaultSoundPlayer();
	

private:
	std::unique_ptr<StateStack> mStateStack;
	std::unique_ptr<Context> mContext;
	std::unique_ptr<sf::RenderWindow> mRenderWindow;
	std::unique_ptr<SignalQueue> mSignalQueue;
	std::unique_ptr<TextureResources> mTextureResources;
	std::unique_ptr<FontResources> mFontResources;
	std::unique_ptr<MusicPlayer> mMusicPlayer;
	std::unique_ptr<SoundPlayer> mSoundPlayer;
	std::unique_ptr<Parallax> mParallax;
};
