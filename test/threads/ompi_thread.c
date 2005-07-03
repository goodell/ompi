#include "ompi_config.h"
#include "support.h"
#include "include/constants.h"
#include "threads/thread.h"
#include "include/sys/atomic.h"


#if !OMPI_HAVE_THREAD_SUPPORT

/* If we don't have thread support, there's no point in running this
   test */

int main(int argc, char *argv[])
{
    printf("OMPI was compiled without thread support -- skipping this test\n");
    return 77;
}

#else

/* Only have the body of this test if we have thread support */

static volatile int count = 0;


static void* thr1_run(opal_object_t* obj)
{
    ompi_atomic_add(&count, 1);
    return NULL;
}

static void* thr2_run(opal_object_t* obj)
{
    ompi_atomic_add(&count, 2);
    return NULL;
}

int main(int argc, char** argv)
{
    int rc;
    ompi_thread_t thr1;
    ompi_thread_t thr2;

    test_init("ompi_thread_t");

    OBJ_CONSTRUCT(&thr1, ompi_thread_t);
    OBJ_CONSTRUCT(&thr2, ompi_thread_t);

    thr1.t_run = thr1_run;
    thr2.t_run = thr2_run;

    rc = ompi_thread_start(&thr1);
    test_verify_int(OMPI_SUCCESS, rc);

    rc = ompi_thread_start(&thr2);
    test_verify_int(OMPI_SUCCESS, rc);
   
    rc = ompi_thread_join(&thr1, NULL);
    test_verify_int(OMPI_SUCCESS, rc);

    rc = ompi_thread_join(&thr2, NULL);
    test_verify_int(OMPI_SUCCESS, rc);

    test_verify_int(3, count);
    return test_finalize();
}

#endif /* OMPI_HAVE_THREAD_SUPPORT */
