# LSD

LSD aims to be a simple high-level wrapper for [zyre](https://github.com/zeromq/zyre) awesome, mesh-oriented, [zeromq](https://github.com/zeromq/libzmq)-powered, P2P library.

It also bears a simple but efficient Python wrapper that proves very useful to test various zyre-based deployments.

The main goal ? Give people the possibility to use zyre with only a remote, bare knowledge of its underneath zeromq magic.

Oh, and LSD stands for Local Service Discovery, which is what it has originally been built for. Please enjoy finding other meanings.

## Dependencies

lsd directly depends on:

* libuuid (from [e2fsprogs](http://e2fsprogs.sourceforge.net/) package)
* libzre, or better named zyre. 

zyre itself depends on:

* libzmq (at least v3.2.0)
* libczmq (latest snapshots)
* libfmq (or filemq, latests snapshot)
* ... and ultimately, libcrypto from openssl package.

## Usage

### C version 

LSD relies on the KISS and zerodoc principles. 
That means that the API should look so awfully OBVIOUS that documentation isn't even remotely needed.

You can find all that API in [lsd.h](https://github.com/vperron/lsd/blob/master/include/lsd.h) file.

There are at least 3 samples of it inside the source tree as well.

Here is a small sample, though:

```c
static void info_callback (lsd_handle_t* handle,
			int event,
			const char *node,
			const char *group, 
			const uint8_t *msg,
			size_t len,
			void* reserved)
{
	printf("Event %d jsut happened !!!\n", event);
}


[...]

lsd_handle_t* handle = lsd_init(NULL, info_callback, NULL);

lsd_join(handle, "MY_GROUP";
lsd_shout(handle, "MY_GROUP", "Hello world !",13);
lsd_publish(handle, "myfancyfile.txt");

sleep (5);

lsd_leave(handle, GROUP_0);
lsd_destroy(handle);
```

As you can notice, we rely on an ultra-simple callback function to read back what zyre internals have to notify:
* New node has arrived
* New guy joined a group
* Some file has been received
* and so on.

It has been designed this way to facilitate _alot_ the binding to other languages.

### Python version

SWIG conversion files along with a test sample are included in the `python/` directory.

## Tested environments

The build phase is a standard autotools procedure and has been successfully tested on Linux in the following conditions:

* Android 4.0.4 on a Samsung Galaxy SIII
* Android 4.2.1 on a Galaxy Nexus
* OpenWRT Barrier Breaker on RaLink RT3050F SoCs
* Linux 3.2.0 on x86\_64 architecture
* Linux 2.6.39 on x86 architecture

## Who owns Lsd?

Like its core libraries zyre/czmq/libzmq/filemq, lsd is owned by all its authors and contributors. This is an open source project licensed under the LGPLv3. To contribute to Lsd please read the [C4 process](http://rfc.zeromq.org/spec:16) that libzmq and its derivatives use.
