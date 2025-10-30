public class Mem {
    static {
        System.loadLibrary("virt"); // libvirt.so
    }

    public native java.nio.ByteBuffer alloc(long pages, int flags);
    public native void free(java.nio.ByteBuffer buffer);
    public native java.nio.ByteBuffer sbrk(long size);

    public static void main(String[] args) {
        Mem vm = new Mem();
        java.nio.ByteBuffer buffer = vm.sbrk(10); // alloc the buffer -> JNI -> mmap()
        buffer.putInt(0, 900);    
        int x = buffer.getInt(0); 
        System.out.println(x); 
        vm.sbrk(-10); // free the 10 bytes allocated
        // vm.free(buffer); // free the buffer -> JNI -> munmap()
    }
}