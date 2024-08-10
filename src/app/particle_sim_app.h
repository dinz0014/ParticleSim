#pragma once
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "types.h"

class ParticleSimApp
{
public:
    using json = nlohmann::json;

    void ReadConfig(const std::string& filename);

    void Run();

private:
    sim::ValidationResult ValidateConfig();
    void LoadConfig();

private:
    json cfg_;

    static constexpr int WINDOW_WIDTH = 1920;
    static constexpr int WINDOW_HEIGHT = 1080;

    int target_fps_;
    int substeps_;
    float dt_;
};