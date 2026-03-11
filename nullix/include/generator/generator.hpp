#pragma once
#include <expected>
#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <vector>
#include "errika/errika.hpp"
#include "host/host.hpp"

namespace nullix {
    class Base {

        class Generator{
            public:
            auto getModules() -> std::expected<std::vector<std::string>, errika::Err_t_>;
            auto computeNextGeneration() -> std::expected<int, errika::Err_t_>;
            auto createDirectories(const std::filesystem::path &genDirPath) -> std::expected<bool, errika::Err_t_>;
            auto copyConfigs(const std::filesystem::path &genDirPath) -> std::expected<bool, errika::Err_t_>;
            auto updateState(const int &nextGen) -> std::expected<bool, errika::Err_t_>;
            auto copyPackageList(const std::string &genDirPath) -> std::expected<bool, errika::Err_t_>;
            auto generate() -> std::expected<bool, errika::Err_t_>;
            private:
            std::filesystem::path userHomePath = nullix::Host::userHome();
            std::filesystem::path nullixUserConfigPath = nullix::Host::userHome()/".config"/"nullix"/"config";
            std::filesystem::path nullixDataHome = nullix::Host::userHome()/".local"/"share"/"nullix";
            std::filesystem::path nullixUserHome = nullix::Host::userHome()/".config"/"nullix";
            std::filesystem::path nullixGeneratedPath = nullix::Host::userHome()/".local"/"share"/"nullix"/"generated";
            std::filesystem::path stateFile = nullix::Host::userHome()/".local"/"share"/"nullix"/"state.toml";

        };
    };
}
