#pragma once

#include <cstdint>
#include <mutex>
#include <cassert>
#include <functional>

template<typename T>
class Pool
{
public:
    Pool();
    ~Pool();

    bool Initialize(uint32_t count);    // �������i�m�ۂ���A�C�e�����j
    void Finalize();                    // �I������

    T* Alloc(std::function<void(uint32_t, T*)> Func_ = nullptr);    // �m�ۂ���
    void Free(T* pValue);   // �J������

    [[nodiscard]] uint32_t GetSize() const;   // ���A�C�e�������擾����
    [[nodiscard]] uint32_t GetUsedCount() const;  // �g�p���̃A�C�e�������m�ۂ���
    [[nodiscard]] uint32_t GetAvailableCount() const; // �g�p�\�ȃA�C�e�������擾����

private:
    struct Item
    {
        T           m_Value;        //!< �l�ł�.
        uint32_t    m_Index;        //!< �C���f�b�N�X�ł�.
        Item* m_pNext;        //!< ���̃A�C�e���ւ̃|�C���^.
        Item* m_pPrev;        //!< �O�̃A�C�e���ւ̃|�C���^.

        Item();
        ~Item();
    };

    uint8_t* m_pBuffer;      //!< �o�b�t�@�ł�.
    Item* m_pActive;      //!< �A�N�e�B�u�A�C�e���̐擪�ł�.
    Item* m_pFree;        //!< �t���[�A�C�e���̐擪�ł�.
    uint32_t    m_Capacity;     //!< ���A�C�e�����ł�.
    uint32_t    m_Count;        //!< �m�ۂ����A�C�e�����ł�.
    std::mutex  m_Mutex;        //!< �~���[�e�b�N�X�ł�.

    //-------------------------------------------------------------------------
    //! @brief      �A�C�e�����擾���܂�.
    //!
    //! @param[in]      index       �擾����A�C�e���̃C���f�b�N�X.
    //! @return     �A�C�e���ւ̃|�C���^��ԋp���܂�.
    //-------------------------------------------------------------------------
    Item* GetItem(uint32_t index);

    //-------------------------------------------------------------------------
    //! @brief      �A�C�e���Ƀ����������蓖�Ă܂�.
    //!
    //! @param[in]      index       �擾����A�C�e���̃C���f�b�N�X.
    //! @return     �A�C�e���ւ̃|�C���^��ԋp���܂�.
    //-------------------------------------------------------------------------
    Item* AssignItem(uint32_t index);

    Pool(const Pool&) = delete;
    void operator = (const Pool&) = delete;
};

template <typename T>
Pool<T>::Pool(): m_pBuffer(nullptr)
                 , m_pActive(nullptr)
                 , m_pFree(nullptr)
                 , m_Capacity(0)
                 , m_Count(0)
{ /* DO_NOTHING */
}

template <typename T>
Pool<T>::~Pool()
{
    Finalize();
}

template <typename T>
bool Pool<T>::Initialize(uint32_t count)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    m_pBuffer = static_cast<uint8_t*>(malloc(sizeof(Item) * (count + 2)));
    if (m_pBuffer == nullptr)
    {
        return false;
    }

    m_Capacity = count;

    // �C���f�b�N�X��U��.
    for (auto i = 2u, j = 0u; i < m_Capacity + 2; ++i, ++j)
    {
        auto item = GetItem(i);
        item->m_Index = j;
    }

    m_pActive = GetItem(0);
    m_pActive->m_pPrev = m_pActive->m_pNext = m_pActive;
    m_pActive->m_Index = uint32_t(-1);

    m_pFree = GetItem(1);
    m_pFree->m_Index = uint32_t(-2);

    for (auto i = 1u; i < m_Capacity + 2; ++i)
    {
        GetItem(i)->m_pPrev = nullptr;
        GetItem(i)->m_pNext = GetItem(i + 1);
    }

    GetItem(m_Capacity + 1)->m_pPrev = m_pFree;

    m_Count = 0;

    return true;
}

template <typename T>
void Pool<T>::Finalize()
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    if (m_pBuffer)
    {
        free(m_pBuffer);
        m_pBuffer = nullptr;
    }

    m_pActive = nullptr;
    m_pFree = nullptr;
    m_Capacity = 0;
    m_Count = 0;
}

template <typename T>
T* Pool<T>::Alloc(std::function<void(uint32_t, T*)> Func_)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    if (m_pFree->m_pNext == m_pFree || m_Count + 1 > m_Capacity)
    {
        return nullptr;
    }

    auto item = m_pFree->m_pNext;
    m_pFree->m_pNext = item->m_pNext;

    item->m_pPrev = m_pActive->m_pPrev;
    item->m_pNext = m_pActive;
    item->m_pPrev->m_pNext = item->m_pNext->m_pPrev = item;

    m_Count++;

    // ���������蓖��.
    auto val = new (static_cast<void*>(item)) T();

    // �������̕K�v������ΌĂяo��.
    if (Func_ != nullptr)
    {
        Func_(item->m_Index, val);
    }

    return val;
}

template <typename T>
void Pool<T>::Free(T* pValue)
{
    if (pValue == nullptr)
    {
        return;
    }

    std::lock_guard<std::mutex> guard(m_Mutex);

    auto item = reinterpret_cast<Item*>(pValue);

    item->m_pPrev->m_pNext = item->m_pNext;
    item->m_pNext->m_pPrev = item->m_pPrev;

    item->m_pPrev = nullptr;
    item->m_pNext = m_pFree->m_pNext;

    m_pFree->m_pNext = item;
    m_Count--;
}

template <typename T>
uint32_t Pool<T>::GetSize() const
{
    return m_Capacity;
}

template <typename T>
uint32_t Pool<T>::GetUsedCount() const
{
    return m_Count;
}

template <typename T>
uint32_t Pool<T>::GetAvailableCount() const
{
    return m_Capacity - m_Count;
}

template <typename T>
Pool<T>::Item::Item(): m_Value()
                       , m_Index(0)
                       , m_pNext(nullptr)
                       , m_pPrev(nullptr)
{ /* DO_NOTHING */
}

template <typename T>
Pool<T>::Item::~Item()
{ /* DO_NOTHING */
}

template <typename T>
typename Pool<T>::Item* Pool<T>::GetItem(uint32_t index)
{
    assert(0 <= index && index <= m_Capacity + 2);
    return reinterpret_cast<Item*>(m_pBuffer + sizeof(Item) * index);
}

template <typename T>
typename Pool<T>::Item* Pool<T>::AssignItem(uint32_t index)
{
    assert(0 <= index && index <= m_Capacity + 2);
    auto buf = (m_pBuffer + sizeof(Item) * index);
    return new (buf) Item;
}