#pragma once
#include <random>
#include <optional>
#include <nlohmann/json.hpp>

template <typename T>
inline T get(const nlohmann::json& config, const std::string& key, T default_val)
{
    if (config.contains(key))
    {
        return static_cast<T>(config[key]);
    }

    return default_val;
}

template <typename T>
inline std::optional<T> get(const nlohmann::json& config, const std::string& key)
{
    if (config.contains(key))
    {
        return static_cast<T>(config[key]);
    }

    return std::nullopt;
}

inline float generateRandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}