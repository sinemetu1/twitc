twitc
===========

[![Build Status](https://travis-ci.org/sinemetu1/twitc.svg)](https://travis-ci.org/sinemetu1/twitc)
[![Coverage Status](https://coveralls.io/repos/sinemetu1/twitc/badge.png)](https://coveralls.io/r/sinemetu1/twitc)

A mini C library for interacting with the Twitter OAuth api.

### Installation:

    ./autogen.sh
    ./configure
    make
    make install

NOTE: There are environment variables that can be set for consumer token,
consumer secret, app token, and app secret. See
[twitc.h](https://github.com/sinemetu1/twitc/blob/master/src/twitc.h)
and the [usage section](https://github.com/sinemetu1/twitc/tree/test-separation#usage) below.

#### Dependencies:

[liboauth](http://liboauth.sourceforge.net/)

[json-c](https://github.com/json-c/json-c)

### Usage:

See [main.c](https://github.com/sinemetu1/twitc/blob/master/src/main.c) for an example.

Environment variables include:

    export TWITC_KEY="Consumer Key (API Key)"
    export TWITC_SECRET="Consumer Secret (API Secret)"
    export TWITC_ACCESS_TOKEN="Access Token"
    export TWITC_ACCESS_SECRET="Access Token Secret"

To generate these values go to
[https://apps.twitter.com/](https://apps.twitter.com/).

### Tests:

Local tests can be run with `make check`.

If you have the environment variables above setup correctly then
you can run `make integration` to test actual integration with
Twitter's API.

### Bugs:

Please file bugs in Github issues.

### License:

[MIT](https://github.com/sinemetu1/twitc/blob/test-separation/LICENSE)
