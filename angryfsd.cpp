#define FUSE_USE_VERSION 35

#include <string>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <errno.h>
#include <sys/xattr.h>
#include <fuse.h>

int count = 0;
std::string bugger = "sod off mate\n";
std::string nosy = "nosy little shit aren't you\n";
std::string chant = "sunderland till i die\ni'm sunderland till I die\ni know i am\ni'm sure i am\ni'm sunderland till i die!\n";

static void *xmp_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    printf("++++++++++++++++++ xmp_init\n");
    (void)conn;
    cfg->use_ino = 1;

    /* Pick up changes from lower filesystem right away. This is
	   also necessary for better hardlink support. When the kernel
	   calls the unlink() handler, it does not know the inode of
	   the to-be-removed entry and can therefore not invalidate
	   the cache of the associated inode - resulting in an
	   incorrect st_nlink value being reported for any remaining
	   hardlinks to this inode. */
    cfg->entry_timeout = 0;
    cfg->attr_timeout = 0;
    cfg->negative_timeout = 0;

    return NULL;
};

static int xmp_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    (void)fi;
    printf("++++++++++++++++++ xmp_getattr %s\n", path);

    std::string pathStr(path);
    if (pathStr == "/")
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (pathStr == "/bugger" || pathStr == "/plonker")
    {
        printf("++++++++++++++++++ made up results! %s\n", path);
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = bugger.length();
    }
    else if (pathStr == "/off") {
        printf("++++++++++++++++++ made up results! %s\n", path);
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = chant.length();
    }
    else if (pathStr == "/secrets") {
        printf("++++++++++++++++++ made up results! %s\n", path);
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = nosy.length();
    }
    else
        return -ENOENT;

    return 0;
};

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
    printf("++++++++++++++++++ xmp_readdir %s\n", path);
    
    int res;
    switch (count%2) {
        case 0:
            res = filler(buf, "bugger", NULL, 0, FUSE_FILL_DIR_PLUS);
            res = filler(buf, "off", NULL, 0, FUSE_FILL_DIR_PLUS);
            res = filler(buf, "plonker", NULL, 0, FUSE_FILL_DIR_PLUS);
            break;
        case 1:
            res = filler(buf, "secrets", NULL, 0, FUSE_FILL_DIR_PLUS);
            break;
    }
    count++;

    printf("++++++++++++++++++ filler result=%d\n", res);

    return 0;
};

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    printf("++++++++++++++++++ xmp_open %s\n", path);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    printf("++++++++++++++++++ xmp_read path=%s,size=%ld,offset=%ld\n", path, size, offset);

    std::string pathStr(path);
    std::string str;

    if (pathStr == "/bugger" || pathStr == "/plonker") {
        str = bugger;
    } else if (pathStr == "/off") {
        str = chant;
    } else if (pathStr == "/secrets") {
        str = nosy;
    }
    str.copy(buf, str.length());
    size = str.length();
    return size;
};

static const struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .open = xmp_open,
    .read = xmp_read,
    .readdir = xmp_readdir,
    .init = xmp_init,
};

int main(int argc, char *argv[])
{
    printf("++++++++++++++++++ main\n");
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
