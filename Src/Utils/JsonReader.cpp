#include "JsonReader.h"

#include "../Common/Object3D.h"
#include <fstream>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

void JsonReader::ImportModelData(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) return;

    json root;
    file >> root;
}
