/**
 * =====================================================================================
 *
 *   @file shuppan_tester.c
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/24/2012 03:02:43 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       Shuppan routines tester tool
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */


#include "shuppan.h"


int main (int argc, char *argv [])
{
    //  Initialize context for talking to tasks
    zctx_t *ctx = zctx_new ();
    zctx_set_linger (ctx, 100);
    
    //  Get number of interfaces to simulate, default 10
    int max_interface = 10;
    int nbr_interfaces = 0;
    int max_tries = -1;
    int nbr_tries = 0;
    if (argc > 1)
        max_interface = atoi (argv [1]);
    if (argc > 2)
        max_tries = atoi (argv [2]);

    //  We address interfaces as an array of pipes
    void **pipes = zmalloc (sizeof (void *) * max_interface);

    //  We will randomly start and stop interface threads
    while (!zctx_interrupted) {
        uint index = randof (max_interface);
        //  Toggle interface thread
        if (pipes [index]) {
            zstr_send (pipes [index], "STOP");
            zsocket_destroy (ctx, pipes [index]);
            pipes [index] = NULL;
            zclock_log ("I: Stopped interface (%d running)", --nbr_interfaces);
        }
        else {
						// pipes [index] = zthread_fork (ctx, interface_task, NULL);
            zclock_log ("I: Started interface (%d running)", ++nbr_interfaces);
        }
        if (max_tries > 0 && ++nbr_tries >= max_tries)
            break;
        //  Sleep ~750 msecs randomly so we smooth out activity
        zclock_sleep (randof (500) + 500);
    }
    zclock_log ("I: Stopped tester (%d tries)", nbr_tries);
    zctx_destroy (&ctx);
    free (pipes);
    return 0;
}
