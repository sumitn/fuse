#include <fuse/fuse_opt.h>

/*!
 * \def FCRYPT_PASS
 * \brief A macro to indicate SUCCESS.
 *
 * Successful returns.
 */

/*!
 * \def FCRYPT_FAIL
 * \brief A macro to indicate FAILURES.
 *
 * Failed returns.
 */

#define FCRYPT_PASS       0 
#define FCRYPT_FAIL       -1

enum {
   KEY_HELP,
   KEY_VERSION,
};

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define FCRYPT_OPT(t, p, v) { t, offsetof(struct fcrypt_fuse, p), v }

/*!
 * \def FCDEBUG_TRACE
 * \brief Macro for tracing enter/exit debug messages.
 */
/*!
 * \def FCDEBUG
 * \brief Macro for debug messags
 */
/*!
 * \def FCDEBUG_ENABLE
 * \brief Enable/Disable Logs
 */
#define FCDEBUG_ENABLE

#ifdef FCDEBUG_ENABLE
#define FCDEBUG_TRACE(fmt,args...) fprintf(stderr, "%s : "fmt,__FUNCTION__,##args)
#define FCDEBUG(fmt,args...) fprintf(stderr, "%s : "fmt,__FUNCTION__,##args)
#else
#define FCDEBUG_TRACE
#define FCDEBUG
#endif

int fcrypt_open(const char *path, struct fuse_file_info *fi);

/*!
 * \fn fcrypt_read()
 * \brief Read function
 *
 * Read function.
 */
int fcrypt_read(const char *path, char *buf, size_t size, off_t offset,
		       struct fuse_file_info *fi);
int fcrypt_write(const char *path, char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi);
