twitc
===========

[![Build Status](https://travis-ci.org/sinemetu1/twitc.svg)](https://travis-ci.org/sinemetu1/twitc)

A mini C library for interacting with the Twitter OAuth api.

### Installation:


    ./autogen.sh
    ./configure
    make
    make install

NOTE: There are environment variables that can be set for consumer token,
consumer secret, app token, and app secret. See
[twitc.h](https://github.com/sinemetu1/twitc/blob/master/src/twitc.h).

#### Dependencies:

[liboauth](http://liboauth.sourceforge.net/)

[json-c](https://github.com/json-c/json-c)

### Usage:

See [main.c](https://github.com/sinemetu1/twitc/blob/master/src/main.c) for an example.

Environment variables include:

    TWITC_KEY
    TWITC_SECRET
    TWITC_ACCESS_TOKEN
    TWITC_ACCESS_SECRET

To generate these values go to
[https://apps.twitter.com/](https://apps.twitter.com/).

### Tests:

Tests can be run with `make check` if you have the environment variables above
setup correctly and uncommenting test calls in `twitc_all_tests` in
[src/test/test_twitc.c](https://github.com/sinemetu1/twitc/blob/master/src/test/test_twitc.c)
will help you debug any issues.

### Bugs:

Please file bugs in Github issues.
