#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <utime.h>
#include <unistd.h>

#include <fuse/fuse.h>
#include <fuse/fuse_opt.h>

#include "fcrypt.h"


struct fcrypt_fuse {
      char *fs_spec;
      char *mntpoint;
      unsigned long fsid;
};

struct fcrypt_fuse fcrypt_fuse;

static struct fuse_operations fcrypt_operations = {
   .open          = fcrypt_open,
   .read          = fcrypt_read,
   .write         = fcrypt_write,
};

static struct fuse_opt fcrypt_opts[] = {
   FUSE_OPT_KEY("-h",          KEY_HELP),
   FUSE_OPT_KEY("-V",          KEY_VERSION),
};

static int fcrypt_opt_proc(void *data, const char *arg, int key, 
			   struct fuse_args *out_args)
{
   switch(key)
   {
      case FUSE_OPT_KEY_OPT:
	 return 1;

      case FUSE_OPT_KEY_NONOPT:
	 if(!fcrypt_fuse.mntpoint)
	 {
	    if(!arg)
	    {
	       fprintf(stderr, "FCRYPT requires mountpoint as argument.\n");
	       abort();
	    }

	    fcrypt_fuse.mntpoint = strdup(arg);
	 }
	 return 1;

      case KEY_HELP:
      case KEY_VERSION:
      default:
	 fprintf(stderr, "Unknown error.\n");
	 abort();
   }

   return;
}

int fcrypt_open(const char *path, struct fuse_file_info *fi)
{
   int ret = FCRYPT_PASS;

   fprintf(stderr, "Opening file system.\n");

   return FCRYPT_PASS;
}

int fcrypt_read(const char *path, char *buf, size_t size, off_t offset,
		       struct fuse_file_info *fi)
{
   int ret = FCRYPT_PASS;
   char *retbuf = malloc(size);
   int count;

   FCDEBUG_TRACE("Enter.\n");
   fprintf(stderr, "Opening file system.\n");

   for(count=0; count<size; count++)
     {
       retbuf[count] = (buf[count]-1)%128;
     }
   strcpy(buf, retbuf);

   FCDEBUG_TRACE("Exit.\n");
   return size;
}

int fcrypt_write(const char *path, char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
{
   int ret = FCRYPT_PASS;
   char *retbuf = malloc(size);
   int count;

   FCDEBUG_TRACE("Enter.\n");
   fprintf(stderr, "Opening file system.\n");

   for(count=0; count<size; count++)
     {
       retbuf[count] = (buf[count]+1)%128;
     }
   strcpy(buf, retbuf);

   FCDEBUG_TRACE("Exit.\n");
   return size;
}

int fcrypt_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi)
{
}

static int fcrypt_fuse_main(struct fuse_args *args)
{
   return fuse_main(args->argc, args->argv, &fcrypt_operations);
}

/**
 * Main function
 */
int main(int argc, char *argv[])
{
   int ret = FCRYPT_FAIL;
   char name[200];
   struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
   char *config = NULL;

   /* Not sure if this is needed. */
   umask(0);

   fprintf(stderr, "Starting fcrypt file system.\n");

   if(fuse_opt_parse(&args, &fcrypt_fuse, fcrypt_opts, fcrypt_opt_proc) == -1)
      exit(1);

   fuse_opt_insert_arg(&args, 1, "-odirect_io");
   fuse_opt_insert_arg(&args, 1, "-oattr_timeout=0");
   fuse_opt_insert_arg(&args, 1, "-omax_write=524288");
   if(getpid() == 0)
      fuse_opt_insert_arg(&args, 1, "-oallow_other");
   fuse_opt_insert_arg(&args, 1, "-s");

   snprintf(name, 200, "-ofsname=fcrypt-fs");
   fuse_opt_insert_arg(&args, 1, name);
   
   return fcrypt_fuse_main(&args);
}
