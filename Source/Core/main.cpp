#include "Application.hpp"

int main() {
    Application app;

    if (!app.Init()) {
        std::cerr << "Falha ao inicializar a aplicação." << std::endl;
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
}
