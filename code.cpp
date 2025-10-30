#include "header.h"
#include <sys/mman.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace mem
{
   size_t virt::pagesz = 0;
   void* virt::alloc(size_t pages, int prot_flags)
   {
        if(virt::pagesz == 0)
        {
            virt::pagesz = getpagesize();
        }
        void* x = mmap(NULL, pages * virt::pagesz, prot_flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(x == nullptr)
        {
            throw std::bad_alloc();
        }
        return x;
   }

   void* virt::alloc_b(size_t length, int prot_flags)
   {
    if(virt::pagesz == 0)
    {
        virt::pagesz = getpagesize();
    }
    if(length % virt::pagesz != 0) // round to full page
    {
        length = length - (length % virt::pagesz) + virt::pagesz;
    }
    void* x = mmap(NULL, length, prot_flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(x == nullptr)
    {
        throw std::bad_alloc();
    }
    return x;
   }

   void virt::free(void* ptr, size_t pages)
   {
        munmap(ptr, virt::pagesz * pages);
   }

   void virt::free_b(void* ptr, size_t size)
   {
        munmap(ptr, size);
   }

   // bump allocator
   bump::bump(void* base, size_t length)
   {
        if (!base || length == 0)
            throw std::invalid_argument("bump: invalid region");

        this->ptr = base;
        this->length = length;
        this->offset = 0; // start of bump pointer
   }

   bump::~bump()
   {
        // doesn't really do anything as memory is managed externally
        // through mem::virt::alloc() and mem::virt::free()
        // max just mark it as unused or something
        offset = 0;
        length = 0;
        ptr = nullptr;
   }

   void* bump::alloc(size_t size)
   {
        if(offset + size > length)
        {
            return nullptr; // failed allocation
        }
        char* x = ((char*)ptr) + offset;
        offset += size;
        return x;
   }

   void bump::free(size_t size)
   {
        offset -= size;
   }
}

int main()
{
    void* data = mem::virt::alloc(1, PROT_READ | PROT_WRITE);
    std::cout << "Data[0]: " << ((int*)data)[0];
    mem::virt::free(data, 1);
}