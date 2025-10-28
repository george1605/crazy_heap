public class Mem {
    static {
        System.loadLibrary("virt"); // libvirt.so
    }

    public native java.nio.ByteBuffer alloc(long pages, int flags);
    public native void free(java.nio.ByteBuffer buffer);

    public static void main(String[] args) {
        Mem vm = new Mem();
        java.nio.ByteBuffer buffer = vm.alloc(2, 0); // alloc the buffer -> JNI -> mmap()
        buffer.putInt(0, 42);    
        int x = buffer.getInt(0); 
        System.out.println(x); 
        vm.free(buffer); // free the buffer -> JNI -> munmap()
    }
}