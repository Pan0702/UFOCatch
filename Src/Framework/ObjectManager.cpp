#include "ObjectManager.h"
#include "GameObject.h"
#include "../Enemies/AnimalDebug/AnimalDubug.h"
#include "../Enemies/AnimalDebug/AnimalDubug.h"
#include "../Enemies/AnimalDebug/AnimalDubug.h"


namespace
{
    struct UpdateObject
    {
        std::unique_ptr<GameObject> object;
        bool initialized;
        bool destroyMe;
        bool dontDestroy;
        int priority;
        bool active;

        UpdateObject() : object(nullptr), initialized(false), destroyMe(false), dontDestroy(false), priority(0),
                         active(true)
        {
        }
    };

    struct DrawObject
    {
        GameObject* object;
        int order;
        bool visible;

        DrawObject() : object(nullptr), order(0), visible(true)
        {
        }
    };

    std::list<UpdateObject> updateObjects;
    std::list<DrawObject> drawObjects;
    bool needSortUpdate;
    bool needSortDraw;
};

static void deleteDrawObject(GameObject* obj)
{
    for (auto it = drawObjects.begin(); it != drawObjects.end();)
    {
        if ((*it).object == obj)
        {
            it = drawObjects.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ObjectManager::Start()
{
}

void ObjectManager::Update()
{
    if (needSortUpdate)
    {
        updateObjects.sort([](UpdateObject& a, UpdateObject& b) { return a.priority > b.priority; });
        needSortUpdate = false;
    }
    for (auto it = updateObjects.begin(); it != updateObjects.end();)
    {
        UpdateObject& node = *it;
        GameObject* obj = node.object.get();
        if (!node.initialized)
        {
            obj->Start();
            node.initialized = true;
        }
        if (node.active)
        {
            obj->Update();
        }
        it++;
    }

    for (auto it = updateObjects.begin(); it != updateObjects.end();)
    {
        UpdateObject& node = *it;
        if (node.destroyMe)
        {
            deleteDrawObject(node.object.get());
            it = updateObjects.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ObjectManager::Draw()
{
    if (needSortDraw)
    {
        drawObjects.sort([](DrawObject& a, DrawObject& b) { return a.order > b.order; });
        needSortDraw = false;
    }
    for (DrawObject node : drawObjects)
    {
        if (node.visible)
        {
            node.object->Draw();
        }
    }
}

void ObjectManager::Release()
{
    DeleteAllGameObject();
}

void ObjectManager::ChangeScene()
{
    for (auto it = updateObjects.begin(); it != updateObjects.end();)
    {
        const UpdateObject& node = *it;
        if (!node.dontDestroy)
        {
            deleteDrawObject(node.object.get());
            it = updateObjects.erase(it);
        }
        else
            it++;
    }
}

std::list<GameObject*> ObjectManager::GetAllObjects()
{
    std::list<GameObject*> objs;
    for (const UpdateObject& obj : updateObjects)
    {
        objs.push_back(obj.object.get());
    }
    return objs;
}

void ObjectManager::Push(std::unique_ptr<GameObject> obj)
{
    GameObject* raw = obj.get();
    UpdateObject uNode;
    uNode.object = std::move(obj);
    updateObjects.push_back(std::move(uNode));
    needSortUpdate = true;

    DrawObject dNode;
    dNode.object = raw;
    drawObjects.push_back(dNode);
    needSortDraw = true;
}

void ObjectManager::Destroy(GameObject* obj)
{
    for (UpdateObject& ou : updateObjects)
    {
        if (ou.object.get() == obj)
            ou.destroyMe = true;
    }
}

void ObjectManager::SetDrawOrder(const GameObject* obj, int _order)
{
    for (DrawObject& od : drawObjects)
    {
        if (od.object == obj)
        {
            od.order = _order;
        }
    }
    needSortDraw = true;
}

void ObjectManager::SetPriority(const GameObject* obj, int _priority)
{
    for (UpdateObject& ou : updateObjects)
    {
        if (ou.object.get() == obj)
        {
            ou.priority = _priority;
        }
    }
    needSortUpdate = true;
}

void ObjectManager::DeleteGameObject(GameObject* obj)
{
    deleteDrawObject(obj);
    for (auto it = updateObjects.begin(); it != updateObjects.end();)
    {
        UpdateObject& node = (*it);
        if (node.object.get() == obj)
        {
            it = updateObjects.erase(it);
        }
        else
            it++;
    }
}

void ObjectManager::DeleteAllGameObject()
{
    for (auto it = updateObjects.begin(); it != updateObjects.end();)
    {
        it = updateObjects.erase(it);
    }
    updateObjects.clear();

    for (auto it = drawObjects.begin(); it != drawObjects.end();)
    {
        it = drawObjects.erase(it);
    }
    drawObjects.clear();
}

void ObjectManager::DontDestroy(const GameObject* obj, bool dont)
{
    for (auto it = updateObjects.begin(); it != updateObjects.end(); it++)
    {
        UpdateObject& node = *it;
        if (node.object.get() == obj)
        {
            node.dontDestroy = dont;
        }
    }
}

void ObjectManager::SetActive(const GameObject* obj, bool active)
{
    for (auto it = updateObjects.begin(); it != updateObjects.end(); it++)
    {
        UpdateObject& node = *it;
        if (node.object.get() == obj)
        {
            node.active = active;
        }
    }
}

void ObjectManager::SetVisible(const GameObject* obj, bool visible)
{
    for (DrawObject& od : drawObjects)
    {
        if (od.object == obj)
        {
            od.visible = visible;
        }
    }
}

bool ObjectManager::IsExist(GameObject* obj)
{
    for (DrawObject& od : drawObjects)
    {
        if (od.object == obj)
        {
            return true;
        }
    }
    return false;
}
