#include "generator/generator.hpp"
#include <cstdlib>
#include <expected>
#include <filesystem>
#include <print>
#include <string>
#include <system_error>
#include <toml++/impl/parse_result.hpp>
#include <toml++/impl/parser.hpp>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <fstream>
using namespace nullix;

auto Generator::computeNextGeneration() -> std::expected<int, std::string>
{
    const char* homeEnv = std::getenv("HOME");
    if (!homeEnv)
        return std::unexpected("HOME environment variable not set");

    std::string HOME = homeEnv;
    std::string stateFile = HOME + "/.local/share/nullix/state.toml";

    if (!std::filesystem::exists(stateFile)) {
        return std::unexpected("state.toml not found");
    }

        toml::parse_result result = toml::parse_file(stateFile);
        if (!result) {
            return std::unexpected(std::string(result.error().description()));
        }
        auto state = std::move(result).table();
        auto latest = state["latest_generation"].value<int>();
        if (!latest) {
            return std::unexpected("latest_generation missing or invalid");
        }
        return *latest + 1;

}


auto Generator::getModules() -> std::expected<std::vector<std::string>, std::string>{

    const char* homeEnv = std::getenv("HOME");
    if (!homeEnv)
        return std::unexpected("HOME environment variable not set");

    std::string HOME = homeEnv;
    std::string nullixModuleFile = HOME + "/.config/nullix/modules.toml";


    toml::parse_result result = toml::parse_file(nullixModuleFile);
    if (!result) {
        return std::unexpected(std::string(result.error().description()));
    }
    auto config = std::move(result).table();
    auto modules = config["modules"].as_table();
    if (!modules) {
        return std::unexpected(std::string{"No Modules Found."});
    }
    std::vector<std::string> moduleList;

    for (auto&& [name, enabled] : *modules) {
        if (enabled.value<bool>().value_or(false)) {
            moduleList.emplace_back(name);
        }
    }
    return moduleList;

}

auto Generator::generate() -> std::expected<bool, std::string>{

    // getting HOME
    const char* homeEnv = std::getenv("HOME");
    if (!homeEnv)
        return std::unexpected("HOME environment variable not set");

    // getting next generation number
    auto genNum = this->computeNextGeneration();
    if (!genNum.has_value()) {
        return std::unexpected(genNum.error());
    }
    int nextGen = *genNum;

    // setting up paths
    std::string HOME = homeEnv;
    std::string genNumStr = std::to_string(nextGen);
    std::string genDirName = "gen" + genNumStr;

    std::filesystem::path nullixDataHome = HOME + "/.local/share/nullix";
    std::filesystem::path nullixUserHome = HOME + "/.config/nullix";
    std::filesystem::path nullixGeneratedPath = nullixDataHome / "generated";
    std::filesystem::path genDirPath = nullixGeneratedPath / genDirName;
    std::filesystem::path nullixUserConfigPath = HOME + "/.config/nullix/config";

    // getting Modules
    auto modules = this->getModules();
    if (!modules.has_value()) {
        return std::unexpected( modules.error());
    }
    std::vector<std::string> mods = *modules;


    // creating directories
    std::error_code ec;
    std::filesystem::create_directories(genDirPath / "config", ec);


    if (ec) {
        return std::unexpected(ec.message());
    }
    ec.clear();
    std::println( "Successfully created directory:{}",std::string(genDirPath / "config") );

    for (auto& mod : mods) {

        std::filesystem::path source =
        nullixUserConfigPath / mod;

        std::filesystem::path dest =
        genDirPath / "config" / mod;

        if ((std::filesystem::exists(source) && std::filesystem::is_directory(source))) {

            std::println( "Trying to copy directory from {} to {}",std::string(source) , std::string(dest));
            std::filesystem::copy(
                source,
                dest,
                std::filesystem::copy_options::recursive |
                std::filesystem::copy_options::overwrite_existing,
                ec
            );
        }
        if (ec) {
            return std::unexpected(ec.message());
        }
        ec.clear();
    }

    //getting packages
    std::filesystem::path srcPkgListPath = nullixUserHome/ ".packages" / "pkglist.txt";
    std::filesystem::path srcAurPkgListPath = nullixUserHome/ ".packages" / "aurpkglist.txt";
    std::filesystem::path destPkgListPath = nullixGeneratedPath / genDirName / "pkglist.txt";
    std::filesystem::path destAurPkgListPath = nullixGeneratedPath / genDirName / "aurpkglist.txt";

    if ((std::filesystem::exists(srcPkgListPath) && std::filesystem::is_regular_file(srcPkgListPath))) {

        std::println( "Trying to copy package list from {} to {}",std::string(srcPkgListPath) , std::string(destPkgListPath));
        std::filesystem::copy(
            srcPkgListPath,
            destPkgListPath,
            std::filesystem::copy_options::overwrite_existing,
            ec
        );
    }
    else {
        std::println("{}", "Package list not found");
        return std::unexpected("Package list not found");
    }

    if (ec) {
        return std::unexpected(ec.message());
    }
    ec.clear();

    if ((std::filesystem::exists(srcAurPkgListPath) && std::filesystem::is_regular_file(srcAurPkgListPath))) {

        std::println( "Trying to copy package list from {} to {}",std::string(srcAurPkgListPath) , std::string(destAurPkgListPath));
        std::filesystem::copy(
            srcAurPkgListPath,
            destAurPkgListPath,
            std::filesystem::copy_options::overwrite_existing,
            ec
        );
    }
    else {
        std::println("{}", "Aur Package list not found");
        return std::unexpected("Aur Package list not found");
    }

    if (ec) {
        return std::unexpected(ec.message());
    }
    ec.clear();

    //updating state

    std::string stateFile = HOME + "/.local/share/nullix/state.toml";

    toml::parse_result result = toml::parse_file(stateFile);
    if (!result) {
        return std::unexpected(std::string(result.error().description()));
    }
    auto state = std::move(result).table();
    state.insert_or_assign("latest_generation" , nextGen);
    std::ofstream out(stateFile);

    if (!out)
        return std::unexpected("Failed to open state.toml for writing");
    out << state;
    if (!out)
        return std::unexpected("Failed writing state.toml");

    std::println("Successfully written to state file");

    return true;
}
