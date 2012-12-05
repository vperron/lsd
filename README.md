# Shuppan - simple wrapper for ZyRE peer-to-peer library.

The sole purpose of the Shuppan library and its bindings is to enable the most simple ever possible use of the [Zyre library](https://github.com/zeromq/zyre).

## How to build

Shuppan depends on libuuid and Zyre, which also depends on a lot of other libraries (czmq, filemq, libzmq, libcrypto from openssl).

The build phase is a standard autotools procedure and has been successfully tested on Linux in the following conditions:

* Android 4.0.4 on a Samsung Galaxy SIII
* Android 4.2.1 on a Galaxy Nexus
* OpenWRT Barrier Breaker on RaLink RT3050F SoCs
* Linux 3.2.0 on x86\_64 architecture
* Linux 2.6.39 on x86 architecture

## Who owns Shuppan?

Like its core libraries zyre/czmq/libzmq/filemq, shuppan is owned by all its authors and contributors. This is an open source project licensed under the LGPLv3. To contribute to Shuppan please read the [C4 process](http://rfc.zeromq.org/spec:16) that libzmq and its derivatives use.
