#include "particle_sim_app.h"

int main()
{
    using json = nlohmann::json;

    ParticleSimApp app;
    app.ReadConfig("sim_config.json");
    app.Run();
    return 0;
}