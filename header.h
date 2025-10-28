#pragma once
#include <cstddef>

namespace mem
{
    class virt
    {
    private:
        static size_t pagesz; // uninit, only at alloc or get_size()
    public:
        static void* alloc(size_t pages, int flags);
        static void* alloc_b(size_t length, int flags);
        static void free(void* ptr, size_t pages);
    };

    class bump
    {
    private:
        void* ptr;
    };
}