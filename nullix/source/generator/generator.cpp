#include "generator/generator.hpp"
#include <expected>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>
#include <system_error>
#include "errika/errika.hpp"
#include "globals.hpp"
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <fstream>
using namespace nullix;

auto
    Base::Generator::computeNextGeneration()
-> std::expected<int, errika::Err_t_>
{
    std::string HOME = std::string(this->userHomePath);

    if
        (!std::filesystem::exists(this->stateFile))
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "state.toml file not found" ,
                std::src_loc()
            }
        );
    }

    toml::parse_result result = toml::parse_file(std::string(this->stateFile));
    if
        (!result)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                std::string(result.error().description()),
                std::src_loc()
            }
        );
    }
    auto state = std::move(result).table();
    auto latest = state["latest_generation"].value<int>();

    if
        (!latest)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "latest_generation missing or invalid",
                std::src_loc()
            }
        );
    }

    return *latest + 1;

}


auto
    Base::Generator::getModules()
-> std::expected<std::vector<std::string>, errika::Err_t_>
{

    std::string HOME = std::string(this->userHomePath);
    std::string nullixModuleFile = HOME + "/.config/nullix/modules.toml";


    toml::parse_result result = toml::parse_file(nullixModuleFile);
    if
        (!result)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                std::string(result.error().description()),
                std::src_loc()
            }
        );
    }

    auto config = std::move(result).table();
    auto modules = config["modules"].as_table();
    if
        (!modules)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "No Modules Found!",
                std::src_loc()
            }
        );
    }
    std::vector<std::string> moduleList;

    for
        (auto&& [name, enabled] : *modules)
    {
        if
            (enabled.value<bool>().value_or(false))
        {
            moduleList.emplace_back(name);
        }
    }
    return moduleList;

}

auto
    Base::Generator::copyConfigs(const std::filesystem::path &genDirPath)
-> std::expected<bool, errika::Err_t_>
{
    std::error_code ec;
    const auto& modules = this->getModules();
    if
        (!modules.has_value())
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                std::string(modules.error().mt_str_errLog()),
                std::src_loc()
            }
        );
    }
    std::vector<std::string> mods = *modules;
    for
        (auto& mod : mods)
    {
        std::filesystem::path source = this->nullixUserConfigPath / mod;
        std::filesystem::path dest = genDirPath / "config" / mod;

        if
            (
                std::filesystem::exists(source) &&
                std::filesystem::is_directory(source)
            )
        {

            std::println
                (
                    "Trying to copy directory from {} to {}",
                    std::string(source) ,
                    std::string(dest)
                )
            ;

            std::filesystem::copy
                (
                    source,
                    dest,
                    std::filesystem::copy_options::recursive |
                    std::filesystem::copy_options::overwrite_existing,
                    ec
                );
            std::println
                (
                    "Successfully copied directory from {} to {}",
                    std::string(source) ,
                    std::string(dest)
                )
            ;
            if
                (ec)
            {
                return std::unexpected
                (
                    errika::Err_t_
                    {
                        errika::error::Err_t_::e_ErrType::Fatal,
                        ec.message() ,
                        std::src_loc()
                    }
                );
            }
            ec.clear();
        }
        else {
            return std::unexpected
            (
                errika::Err_t_
                {
                    errika::error::Err_t_::e_ErrType::Fatal,
                    "Source module directory is not found!",
                    std::src_loc()
                }
            );
        }
    }
    return true;
};

auto
    Base::Generator::copyPackageList(const std::string &genDirName)
-> std::expected<bool, errika::Err_t_>
{
    std::error_code ec;
    //getting packages
    std::filesystem::path srcPkgListPath =
    nullixUserHome/ ".packages" / "pkglist.txt";

    std::filesystem::path srcAurPkgListPath =
    nullixUserHome/ ".packages" / "aurpkglist.txt";

    std::filesystem::path destPkgListPath =
    nullixGeneratedPath / genDirName / "pkglist.txt";

    std::filesystem::path destAurPkgListPath =
    nullixGeneratedPath / genDirName / "aurpkglist.txt";

    if
        (
            (std::filesystem::exists(srcPkgListPath) &&
            std::filesystem::is_regular_file(srcPkgListPath))
        )
    {
        std::println
            (
                "Trying to copy package list from {} to {}",
                std::string(srcPkgListPath) ,
                std::string(destPkgListPath)
            )
        ;

        std::filesystem::
        copy
        (
            srcPkgListPath,
            destPkgListPath,
            std::filesystem::copy_options::overwrite_existing,
            ec
        );
    }
    else
    {
        std::println("{}", "Package list not found");
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "Package list not found!" ,
                std::src_loc()
            }
        );    }

    if
        (ec)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                ec.message(),
                std::src_loc()
            }
        );
    }
    ec.clear();
    std::println
    (
        "Successfully copied package list from {} to {}",
        std::string(srcPkgListPath) ,
        std::string(destPkgListPath)
    )
;
    if
        (
            (
                std::filesystem::exists(srcAurPkgListPath) &&
                std::filesystem::is_regular_file(srcAurPkgListPath)
            )
        )
    {

        std::println
            (
                "Trying to copy package list from {} to {}",
                std::string(srcAurPkgListPath) ,
                std::string(destAurPkgListPath)
            )
        ;

        std::filesystem::
        copy
        (
            srcAurPkgListPath,
            destAurPkgListPath,
            std::filesystem::copy_options::overwrite_existing,
            ec
        );
    }
    else
    {
        std::println("{}", "Aur Package list not found");
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "AUR Package list not found!" ,
                std::src_loc()
            }
        );
    }

    if
        (ec)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                ec.message() ,
                std::src_loc()
            }
        );
    }
    ec.clear();
    std::println
        (
            "Successfully copied package list from {} to {}",
            std::string(srcAurPkgListPath) ,
            std::string(destAurPkgListPath)
        )
    ;
    return true;
}

auto
    Base::Generator::updateState(const int &nextGen)
-> std::expected<bool, errika::Err_t_>
{
    //updating state
    toml::parse_result result = toml::parse_file(std::string(this->stateFile));
    if
        (!result)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                std::string(result.error().description()),
                std::src_loc()
            }
        );
    }
    auto state = std::move(result).table();
    state.insert_or_assign("latest_generation" , nextGen);
    std::ofstream out(stateFile);

    if
        (!out)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "Failed to open state.toml file for writing!" ,
                std::src_loc()
            }
        );
    }
    out << state;
    if
        (!out)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                "Failed writing to state.toml" ,
                std::src_loc()
            }
        );
    }
    return true;

};

auto
    Base::Generator::createDirectories(const std::filesystem::path &genDirPath)
-> std::expected<bool, errika::Err_t_>
{
    // creating directories
    std::error_code ec;
    std::filesystem::create_directories(genDirPath / "config", ec);


    if
        (ec)
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                ec.message(),
                std::src_loc()
            }
        );
    }
    ec.clear();

    std::println
    (
        "Successfully created directory:{}",
        std::string(genDirPath / "config")
    );
    return true;
}
auto
    Base::Generator::generate()
-> std::expected<bool, errika::Err_t_>
{

    // getting next generation number
    auto genNum = this->computeNextGeneration();
    if
        (!genNum.has_value())
    {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                genNum.error().mt_str_errLog(),
                std::src_loc()
            }
        );
    }
    int nextGen = *genNum;

    // setting up paths
    std::string genNumStr = std::to_string(nextGen);
    std::string genDirName = "gen" + genNumStr;

    std::filesystem::path genDirPath = this->nullixGeneratedPath / genDirName;

    // creating required directories in nullix data home
    auto createDirectoriesRes = this->createDirectories(genDirPath);
    if (!createDirectoriesRes.has_value()) {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                createDirectoriesRes.error().mt_str_errLog(),
                std::src_loc()
            }
        );
    }

    // copy modules
    auto copyConfigRes = this->copyConfigs(genDirPath);
    if (!copyConfigRes.has_value()) {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                copyConfigRes.error().mt_str_errLog(),
                std::src_loc()
            }
        );
    }

    // copy the pacakge list
    auto copyPackageListRes = this->copyPackageList(genDirName);
    if (!copyPackageListRes.has_value()) {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                copyPackageListRes.error().mt_str_errLog(),
                std::src_loc()
            }
        );
    }

    // updating the state
    auto updateStateRes = this->updateState(nextGen);
    if (!updateStateRes.has_value()) {
        return std::unexpected
        (
            errika::Err_t_
            {
                errika::error::Err_t_::e_ErrType::Fatal,
                updateStateRes.error().mt_str_errLog(),
                std::src_loc()
            }
        );
    }
    std::println("Successfully written to state file");
    std::println("Successfully Generated {}",genDirName);

    return true;
}
