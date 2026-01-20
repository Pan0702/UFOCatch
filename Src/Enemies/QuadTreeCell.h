#include <vector>

template <typename T>
class CQuadTreeCell
{
public:
    void Push(T* object)
    {
        m_objects.push_back(object);
    }
    void Clear()
    {
        m_objects.clear();
    }
    const std::vector<T*>& GetObjects()
    {
        return m_objects;
    }

private:
    std::vector<T*> m_objects;
};
