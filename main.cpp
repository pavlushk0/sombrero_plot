
#include "mainApp.h"

int main(int argc, char* argv[]) {
    mainApp_c app;

	app.init_app();

	app.looper();

	return 0;
}
