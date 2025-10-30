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
        static void free_b(void* ptr, size_t length);
    };

    class bump
    {
    private:
        void* ptr;
        size_t length;
        size_t offset;
    public:
        bump(void* base, size_t length);
        ~bump(); // nothing really cause memory is NOT managed by bump but externally
        void* alloc(size_t size);
        void free(size_t size); 
    };
}