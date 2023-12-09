#include <App.hpp>
#include <Signal.hpp>
#include <MenuState.hpp>
#include <GameState.hpp>
#include <PauseState.hpp>
#include <OptionsState.hpp>
#include <GameOverState.hpp>
#include <WorkbenchState.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <cassert>


App::App()
	: AppElement(nullptr) {
	// Creating state stack.
	mContext = defaultContext();
	mStateStack = std::make_unique<StateStack>(mContext.get());

	mSignalQueue->push_back(Signal{SignalID::PushState, SignalData::Menu});
	handleSignals();
	
	mRenderWindow->clear();
	mRenderWindow->display();
}

void App::run() {
	sf::Time timeSinceLastUpdate(sf::Time::Zero);
	sf::Clock clock;
	while (mRenderWindow->isOpen()) {
		auto elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		auto fps = 60.0f;
		auto timePerFrame = sf::seconds(1.0)/fps;
		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			handleSignals();
			handleEvents();
			update(timePerFrame);
		}
		if (not mStateStack->size()) {
			break;
		}
		draw(elapsedTime);
	}
}

void App::handleSignals() {
	while (mContext->signalQueue->size()) {
		auto signal = mContext->signalQueue->front();
		mContext->signalQueue->pop_front();
		handleSignal(signal);
	}
}

void App::handleEvents() {
	sf::Event event;
	while (mRenderWindow->pollEvent(event)) {
		handleEvent(event);
	}
}

void App::handleSignal(const Signal& signal) {
	handleStateStackSignal(signal);
	mStateStack->handleSignal(signal);
	mParallax->handleSignal(signal);
}

std::unique_ptr<State> App::stateByID(SignalData data) {
	if (data == SignalData::Menu) {
		return std::make_unique<MenuState>(mContext.get());
	} else if (data == SignalData::Game) {
		return std::make_unique<GameState>(mContext.get());
	} else if (data == SignalData::Pause) {
		return std::make_unique<PauseState>(mContext.get());
	} else if (data == SignalData::Options) {
		return std::make_unique<OptionsState>(mContext.get());
	} else if (data == SignalData::GameOver) {
		return std::make_unique<GameOverState>(mContext.get());
	} else if (data == SignalData::Workbench) {
		return std::make_unique<WorkbenchState>(mContext.get());
	}
	return nullptr;
}

void App::handleStateStackSignal(const Signal& signal) {
	if (signal.ID == SignalID::PushState) {
		mStateStack->push(stateByID(signal.data));
	} else if (signal.ID == SignalID::PopState) {
		mStateStack->pop();
	} else if (signal.ID == SignalID::ClearStack) {
		mStateStack->clear();
	}
}

void App::handleEvent(const sf::Event& event) {
	if (event.key.code == sf::Keyboard::Q) {
		mRenderWindow->close();
	}
	mStateStack->handleEvent(event);
	mParallax->handleEvent(event);
}

void App::update(sf::Time dt) {
	assert(mMusicPlayer);
	if (mMusicPlayer->status(MusicID::Main) == sf::Music::Stopped) {
		mMusicPlayer->reset(MusicID::Main);
		mMusicPlayer->play(MusicID::Main);
	}

	mStateStack->update(dt);
	mParallax->update(dt);
}

void App::draw(sf::Time dt) {
	mRenderWindow->clear();
	auto view = mRenderWindow->getDefaultView();
	if (mContext->player) {
		view.setCenter(mContext->player->position());
	}
	mRenderWindow->setView(view);
	mParallax->draw(dt);
	mStateStack->draw(dt);
	mRenderWindow->display();
}

std::unique_ptr<TextureResources> defaultTextureResources() {
	auto textures = std::make_unique<TextureResources>();
	assert(textures->load(TextureID::Player, "./assets/textures/player.png"));
	assert(textures->load(TextureID::Terrain,
						  "./assets/textures/terrain.png"));
	assert(textures->load(TextureID::Lizard,
						  "./assets/textures/lizard.png"));
	assert(textures->load(TextureID::Wizard,
						  "./assets/textures/wizard.png"));
	assert(textures->load(TextureID::Orc,
						  "./assets/textures/orc.png"));
	assert(textures->load(TextureID::ParallaxLayer1,
						  "assets/textures/parallax-mountain-bg.png"));
	assert(textures->load(
			   TextureID::ParallaxLayer2,
			   "assets/textures/parallax-mountain-montain-far.png"));
	assert(textures->load(
			   TextureID::ParallaxLayer3,
			   "assets/textures/parallax-mountain-mountains.png"));
	assert(textures->load(
			   TextureID::ParallaxLayer4,
			   "assets/textures/parallax-mountain-trees.png"));
	assert(textures->load(
			   TextureID::ParallaxLayer5,
			   "assets/textures/parallax-mountain-foreground-trees.png"));
	return textures;
}

std::unique_ptr<FontResources> defaultFontResources() {
	auto fonts = std::make_unique<FontResources>();
	assert(fonts->load(FontID::Menu, "./assets/fonts/DejaVuSans.ttf"));
	return fonts;
}

std::unique_ptr<Context> App::defaultContext() {
	mSignalQueue = std::make_unique<SignalQueue>();
	mRenderWindow = makeRenderWindow();
	mTextureResources = defaultTextureResources();
	mFontResources = defaultFontResources();
	mMusicPlayer = defaultMusicPlayer();
	mSoundPlayer = defaultSoundPlayer();

	auto context = std::make_unique<Context>(Context());
	context->renderTarget = mRenderWindow.get();
	context->window = mRenderWindow.get();
	context->signalQueue = mSignalQueue.get();
	context->textures = mTextureResources.get();
	context->fonts = mFontResources.get();
	context->musicPlayer = mMusicPlayer.get();
	context->soundPlayer = mSoundPlayer.get();

	mParallax = defaultParallax(context.get());
	context->parallax = mParallax.get();

	return context;
}

std::unique_ptr<sf::RenderWindow> App::makeRenderWindow() {
	sf::VideoMode videoMode(sf::VideoMode::getFullscreenModes()[0]);
	auto style = sf::Style::Fullscreen;
	auto window = std::make_unique<sf::RenderWindow>(
		videoMode, "PixelDream", style);
	window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(true);
	window->setKeyRepeatEnabled(false);
	return window;
}

std::unique_ptr<MusicPlayer> App::defaultMusicPlayer() {
	auto musicPlayer = std::make_unique<MusicPlayer>();
	musicPlayer->setVolume(50);
	assert(musicPlayer->load(MusicID::Main, "./assets/audio/mainTheme.ogg"));
	return musicPlayer;
}

std::unique_ptr<SoundPlayer> App::defaultSoundPlayer() {
	auto soundPlayer = std::make_unique<SoundPlayer>();
	soundPlayer->setVolume(90);
	soundPlayer->load(SoundID::Walk, "./assets/audio/playerWalk.ogg");
	soundPlayer->load(SoundID::Jump, "./assets/audio/playerJump.ogg");
	soundPlayer->load(SoundID::AttackByKnife,
					  "assets/audio/playerAttackByKnife.ogg");
	soundPlayer->load(SoundID::AttackByMagic,
					  "assets/audio/playerAttackByMagic.ogg");
	soundPlayer->load(SoundID::CreateBlock,
					  "assets/audio/createBlock.ogg");
	soundPlayer->load(SoundID::DestroyBlock,
					  "assets/audio/destroyBlock.ogg");
	soundPlayer->load(SoundID::ButtonHover,
					  "assets/audio/buttonHover.ogg");
	soundPlayer->load(SoundID::ButtonHit,
					  "assets/audio/buttonHit.ogg");
	return soundPlayer;
}
