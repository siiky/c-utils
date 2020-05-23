/* rc - v2020.05.23-0
 *
 * A RefCount wrapper type inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 */

/*
 * Magic from `sort.h`
 */
# define RC_CFG_CONCAT(A, B)    A ## B
# define RC_CFG_MAKE_STR1(A, B) RC_CFG_CONCAT(A, B)
# define RC_CFG_MAKE_STR(A)     RC_CFG_MAKE_STR1(RC_CFG_PREFIX, A)

# ifndef RC_CFG_TYPE
#   error "Must define RC_CFG_TYPE"
# endif /* RC_CFG_TYPE */

# ifndef RC_CFG_RC
#   define RC_CFG_RC rc
# endif /* RC_CFG_RC */

# ifndef RC_CFG_PREFIX
#   define RC_CFG_PREFIX RC_CFG_MAKE_STR1(RC_CFG_RC, _)
# endif /* RC_CFG_PREFIX */

/**
 * The RC type
 */
struct RC_CFG_RC {
#ifndef RC_CFG_NO_FREE
	/** The free function */
	void (*free) (RC_CFG_TYPE);
#endif /* RC_CFG_NO_FREE */

	/** The refcounted object */
	RC_CFG_TYPE obj;

	/** The refcount */
	unsigned int rc;
};

#include <stdbool.h>

#define RC_FREE RC_CFG_MAKE_STR(free)
#define RC_INC  RC_CFG_MAKE_STR(inc)
#define RC_NEW  RC_CFG_MAKE_STR(new)
#define RC_OBJ  RC_CFG_MAKE_STR(obj)

RC_CFG_TYPE RC_OBJ  (struct RC_CFG_RC * self);
bool        RC_FREE (struct RC_CFG_RC * self);
void        RC_INC  (struct RC_CFG_RC * self);

#ifdef RC_CFG_NO_FREE
struct RC_CFG_RC RC_NEW (RC_CFG_TYPE obj);
#else
struct RC_CFG_RC RC_NEW (RC_CFG_TYPE obj, void (*free) (void*));
#endif /* RC_CFG_NO_FREE */

#ifdef RC_CFG_IMPLEMENTATION

#ifndef RC_CFG_NO_FREE
# include <stddef.h>
#endif /* RC_CFG_NO_FREE */

/**
 * @brief Decrease the refcount of an RC pointer
 * @param self The RC pointer
 */
bool RC_FREE (struct RC_CFG_RC * self)
{
	if (self->rc == 0) return false;
	if (--self->rc == 0) {

#ifndef RC_CFG_NO_FREE
		if (self->free) {
			self->free(self->obj);
		}
		self->free = NULL;
#endif /* RC_CFG_NO_FREE */

		self->obj = RC_CFG_NULL_OBJ;
	}
	return self->rc > 0;
}

/**
 * @brief Increment the refcount of an RC pointer
 * @param self The RC pointer
 */
void RC_INC (struct RC_CFG_RC * self)
{
	self->rc++;
}

/**
 * @brief Initialize an RC pointer
 * @param obj The pointer to associate with this RC pointer
 * @param free The function to free @a obj
 * @returns The new RC pointer
 */
#ifdef RC_CFG_NO_FREE
struct RC_CFG_RC RC_NEW (RC_CFG_TYPE obj)
#else
struct RC_CFG_RC RC_NEW (RC_CFG_TYPE obj, void (*free) (void*))
#endif /* RC_CFG_NO_FREE */
{
	return (struct RC_CFG_RC) {
#ifndef RC_CFG_NO_FREE
		.free = free,
#endif /* RC_CFG_NO_FREE */
		.obj = obj,
		.rc = 1,
	};
}

/**
 * @brief Unwrap the pointer from an RC pointer
 * @param self The RC pointer
 * @returns The wrapper pointer
 */
RC_CFG_TYPE RC_OBJ (struct RC_CFG_RC * self)
{
	return self->obj;
}

#endif /* RC_CFG_IMPLEMENTATION */
