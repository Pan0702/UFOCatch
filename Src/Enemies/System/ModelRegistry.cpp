#include "ModelRegistry.h"
#include <Windows.h>
#include <tchar.h>

#include "../../Framework/GameObject.h"

CModelRegistry::CModelRegistry()
{
    ObjectManager::DontDestroy(this);
    LoadDefaultModels();
}

CModelRegistry::~CModelRegistry()
{
    for (ModelEntry& entry : m_models)
    {
        delete entry.mesh;
        entry.mesh = nullptr;
    }
    m_models.clear();
}

void CModelRegistry::LoadDefaultModels()
{
    // Dog//
    {
        ModelEntry entry = {};
        entry.name = Animal::Name::DOG;
        entry.mesh = new CFbxMesh();
        entry.mesh->Load(Animal::Path::DOG);
        entry.mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
        entry.mesh->LoadAnimation(A_RUN, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
        entry.mesh->LoadAnimation(A_WALK, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
        m_models.push_back(entry);
    }

    // Human//
    {
        ModelEntry entry = {};
        entry.name = Animal::Name::HUMAN;
        entry.mesh = new CFbxMesh();
        if (entry.mesh->Load(Animal::Path::HUMAN) == false)
            assert(false);
        entry.mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
        entry.mesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
        entry.mesh->LoadAnimation(A_SEACH, "data/NewAnimal/Human/Human_Find.anmx", false);
        m_models.push_back(entry);
    }

    // Chicken//
    {
        ModelEntry entry = {};
        entry.name = Animal::Name::CHICKEN;
        entry.mesh = new CFbxMesh();
        entry.mesh->Load(Animal::Path::CHICKEN);
        entry.mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Chicken/Chicken_Idle.anmx", false);
        entry.mesh->LoadAnimation(A_WALK, "data/NewAnimal/Chicken/Chicken_Walk.anmx", true);
        entry.mesh->LoadAnimation(A_RUN, "data/NewAnimal/Chicken/Chicken_Run.anmx", false);
        m_models.push_back(entry);
    }

    // Sheep
    {
        ModelEntry entry = {};
        entry.name = "Sheep";
        entry.mesh = new CFbxMesh();
        entry.mesh->Load("data/NewAnimal/Sheep/Sheep.mesh");
        entry.mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Sheep/Sheep_Idle.anmx", false);
        entry.mesh->LoadAnimation(A_WALK, "data/NewAnimal/Sheep/Sheep_Walk.anmx", true);
        m_models.push_back(entry);
    }
}

CFbxMesh* CModelRegistry::GetMesh(const std::string& name) const
{
    for (const ModelEntry& entry : m_models)
    {
        if (name == entry.name)
        {
            return entry.mesh;
        }
    }

    MessageBox(nullptr, _T("CModelRegistry::GetMesh()"),
               _T("処理に失敗しました。"), MB_OK);
    return nullptr;
}

void CModelRegistry::Register(const std::string& name, CFbxMesh* mesh)
{
    ModelEntry entry = {};
    entry.name = name;
    entry.mesh = mesh;
    m_models.push_back(entry);
}
