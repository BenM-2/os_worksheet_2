typedef struct __attribute__((packed)) {
    unsigned char asci_char;    // Ascii character
    unsigned char FG_BG;        // FG + BG
} FrameBuffer ;

int kernel_main()
{
    // Frame buffer 
    // FrameBuffer *fb = (FrameBuffer *)0x000B800;
    // FrameBuffer test = {'A',0x28};
    // fb[0] = test;

    unsigned char *fb = (unsigned char *)0x000B8000;
    fb[0] = 'A';
    fb[1] = 0x28;
    return 0;
}