# bbcalc
boolean & binary calculator

[![Build Status](https://travis-ci.org/madeso/bbcalc.svg?branch=master)](https://travis-ci.org/madeso/bbcalc)
[![Coverage Status](https://coveralls.io/repos/github/madeso/bbcalc/badge.svg?branch=master)](https://coveralls.io/github/madeso/bbcalc?branch=master)

## Current features

Convert between decimal,

    > bbcalc 234
    dec: 234
    hex: 0xea
    bin: 1110 1010

... hexadecimal:

    > bbcalc 0xfe
    dec: 254
    hex: 0xfe
    bin: 1111 1110

... and binary:

    > bbcalc 0b1101
    dec: 13
    hex: 0xd
    bin: 1101

and binary expressions:

    > bbcalc "0xff & 0b100"
    dec: 4
    hex: 0x4
    bin: 100

& is AND

| is OR

## Planned features (no order)

* Adding () to avoid the crappy operator precedence.
* Inverting/negating/complement operator
* Options to reduce the output focusing only on decimal, hex and/or binary
* Smartly align the values when more than one input is specified
* Provide labels when more than 1 input is specifed
* Some automatic testing
* Truth table generation
* csv export of the output
