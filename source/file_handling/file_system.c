// /**
//  * @file file_system.c
//  * @author Ben Marples
//  * @brief
//  */

// //------------------------------------------------------------------------------
// // Includes

// #include "process.h"
// #include "type.h"

// #define MAX_FILE_NAME 64

// static vfs_node_t root;    

// typedef enum
// {
//     FILE_EXECUTABLE,
//     FILE_FOLDER,
//     FILE_TEXT
// }FILE_TYPE_t;

// typedef enum
// {
//     FILE_PERMISSION_READ_ONLY,
//     FILE_PERMISSION_WRITE_ONLY,
//     FILE_PERMISSION_READ_WRITE,
// }FILE_PERMISSIONS_t;

// typedef struct
// {
//     FILE_TYPE_t FT;
//     FILE_PERMISSIONS_t FP;
//     // u8int (*read)(FILE f, void *buff, usize count, usize offeset);
//     // u8int (*write)(FILE f, const void *const buff, usize count, usize offeset);
// } File_info;

// typedef struct vfs_node_t
// {
//     char name[64];
//     vfs_node_t *previous;
//     vfs_node_t *children;
//     usize children_count;
//     File_info file_info;
// } vfs_node_t;

// //------------------------------------------------------------------------------
// // Function Declarations

// void vfs_init();
// vfs_node_t *vfs_lookup(char *path);
// usize add_node(vfs_node_t *parent,vfs_node_t *child);
// usize delete_node(vfs_node_t *);

// //------------------------------------------------------------------------------
// // Function Implementations

// void vfs_init()
// {
//     // Init Root

//     // init stdin,stderr,stdout

// }