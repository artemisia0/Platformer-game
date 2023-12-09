#include <App.hpp>
#include <stdexcept>
#include <Log.hpp>


int main() {
	try {
		Log("Initialization started").info();
		App app;
		Log("Running").info();
		app.run();
		Log("Exit with zero code").info();
		return 0;
	} catch (std::exception& err) {
		Log(err.what()).fatal();
	} catch (...) {
		Log("Unknown exception has been thrown").fatal();
	}
}
