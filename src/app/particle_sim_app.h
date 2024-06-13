#include "particle/particle.h"
#include <SFML/Graphics.hpp>

class ParticleSimApp
{
public:
    ParticleSimApp();
    void Run();

private:
    static constexpr int target_fps_ = 100;
    ParticleManager manager_;
};