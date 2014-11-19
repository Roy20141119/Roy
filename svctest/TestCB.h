#ifndef __SI_TEST_CALLBACK_H__
#define __SI_TEST_CALLBACK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*do_something_callback)(int result, void *userdata);

bool add_callback(do_something_callback cb);

bool remove_callback(do_something_callback cb);

#ifdef __cplusplus
}
#endif
/** @} */
#endif