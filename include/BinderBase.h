#ifndef __SI_BINDER_BASE_H__
#define __SI_BINDER_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

namespace android {

	enum {
	        TEST_START_TRANSACTION = IBinder::FIRST_CALL_TRANSACTION,
			TEST_NUMBER_TRANSACTION,
	        TEST_ADD_CALLBACK_TRANSACTION,
	        TEST_DO_SOMETHING_TRANSACTION,
	    };

	typedef void (*do_something_callback)(int result, void *userdata);

	bool add_callback(do_something_callback cb);

	bool remove_callback(do_something_callback cb);
}

#ifdef __cplusplus
}
#endif
/** @} */
#endif