#ifndef FileSystem_H
#define FileSystem_H

#include "fuse.h"
#include "fuse_opt.h"
#include <cstddef>
#include <memory>
#include <map>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif


struct ext4 {
    // size of each block in the filesystem (4 KB)
    static constexpr std::size_t BLOCK_SIZE = 4096;

    // number of direct block pointers in the inode
    static constexpr std::size_t NDIR_BLOCKS = 12;

    // index for the indirect block pointer
    static constexpr std::size_t IND_BLOCK = NDIR_BLOCKS;

    // index for the double-indirect block pointer
    static constexpr std::size_t DIND_BLOCK = IND_BLOCK + 1;

    // index for the triple-indirect block pointer
    static constexpr std::size_t TIND_BLOCK  = DIND_BLOCK  + 1;

    // total number of block pointers in the inode
    static constexpr std::size_t N_BLOCKS = TIND_BLOCK + 1;
};


class FileSystem {
// singleton
// carefully use smart ptrs since most fuse functions take the ownership of ptr
public:
    using fs_type = ext4;
    static constexpr char mail_address[] = "sijie_kong@ucsb.edu";

    static void init(int argc, char* argv[]) {
        instance = std::make_unique<FileSystem>(argc, argv);
    }

    static FileSystem* get() {
        return instance.get();
    }

    FileSystem(int argc, char* argv[]);
    ~FileSystem();
    // cannot be copied or moved
    FileSystem(const FileSystem& fs) = delete;
    FileSystem(FileSystem&& fs) = delete;

    int launch() {
        return fuse_main(args_ptr->argc, args_ptr->argv, &ops, nullptr);
    }

    // static callbacks
    static int ext4_open(const char *path, struct fuse_file_info *info);
    static int ext4_create(const char *path, mode_t mode, struct fuse_file_info *info);
    static int ext4_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *info);
    static int ext4_release(const char *path, struct fuse_file_info *info);
    static int ext4_sendmail(const char *path);
    static int ext4_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *info);
    static int ext4_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *info, enum fuse_readdir_flags flags);

private:
    static std::unique_ptr<FileSystem> instance;

    std::unique_ptr<fuse_args> args_ptr;
    fuse_operations ops;
    std::map<std::string, std::string> files;
};

#endif