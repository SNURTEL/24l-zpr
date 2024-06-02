#include "common/model_manager.hpp"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

std::string modelManager::pretrainedModelToString(PretrainedModel model) {
    switch (model) {
    case PretrainedModel::style_cezanne:
        return "style_cezanne";
    case PretrainedModel::style_monet:
        return "style_monet";
    case PretrainedModel::style_ukiyoe:
        return "style_ukiyoe";
    case PretrainedModel::style_vangogh:
        return "style_vangogh";
    default:
        throw std::runtime_error("What a terrible failure");
    }
}

std::optional<modelManager::PretrainedModel>
modelManager::stringToPretrainedModel(std::string modelString) {
    if (modelString == "style_cezanne") {
        return PretrainedModel::style_cezanne;
    } else if (modelString == "style_monet") {
        return PretrainedModel::style_monet;
    } else if (modelString == "style_ukiyoe") {
        return PretrainedModel::style_ukiyoe;
    } else if (modelString == "style_vangogh") {
        return PretrainedModel::style_vangogh;
    } else {
        return std::nullopt;
    }
}

std::optional<std::set<modelManager::PretrainedModel>>
modelManager::getDownloadedModels() {
    std::string path = "./models";

    std::set<modelManager::PretrainedModel> availableModels{};

    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            continue;
        }

        auto filename = entry.path().filename();
        size_t lastindex = filename.string().find_last_of(".");
        std::string rawname = filename.string().substr(0, lastindex);

        auto modelFromString = stringToPretrainedModel(rawname);

        if (!modelFromString) {
            return std::nullopt;
        }

        if (modelFromString) {
            availableModels.insert(modelFromString.value());
        }
    }

    return std::make_optional(availableModels);
}

std::optional<std::filesystem::path>
modelManager::_downloadModel(modelManager::PretrainedModel model) {
    std::string command = "python scripts/create_cyclegan_pretrained.py " +
                          modelManager::pretrainedModelToString(model);
    auto returnCode = system(command.c_str());

    if (returnCode) {
        return std::nullopt;
    }

    return modelManager::_getModelPath(model);
}

std::optional<std::filesystem::path>
modelManager::_getModelPath(modelManager::PretrainedModel model) {
    fs::path modelPath =
        "models/" + modelManager::pretrainedModelToString(model) + ".ts";
    if (!fs::exists(modelPath)) {
        return std::nullopt;
    }

    return modelPath;
}

std::optional<std::filesystem::path>
modelManager::getOrDownloadModelPath(modelManager::PretrainedModel model) {
    if (!fs::is_directory("models") || !fs::exists("models")) {
        fs::create_directory("models");
    }

    auto downloadedModels = modelManager::getDownloadedModels();

    if (downloadedModels->contains(model)) {
        auto modelPath = modelManager::_getModelPath(model);
        return modelPath;
    } else {
        auto modelPath = modelManager::_downloadModel(model);
        return modelPath;
    }
}