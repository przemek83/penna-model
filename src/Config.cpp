#include "Config.h"

#include "yaml-cpp/yaml.h"

namespace Config
{
Config::Params loadConfig()
{
    Config::Params params;
    YAML::Node yaml = YAML::LoadFile("config.yaml");
    params.years_ = yaml["years"].as<int>();
    params.livesOnStart_ = yaml["livesOnStart"].as<int>();

    return params;
}
}  // namespace Config
