# "smallest" rational number that approximates a given floating-point number

## 

`smallest-rational` computes candidates for rational approximations
of given floating-point numbers.

Numbers can be given on the command line or from one or more files
or from `stdin`.

The result is usually a series of approximations,
listed in order, from tighter tolerances down to looser tolerances.
The following information is shown for each candidate fraction:

  1. the fraction: numerator/denominator

  2. the tolerance value

  3. Whether the fraction is greater than, less than or equal
to the given floating-point number.

  4. The absolute value of the difference.

  5. The percent difference.

There are options to show results only for fractions that are over
(>=) or under (<=).

This way of filtering the candidates can be useful,
for example, in cases where approximations that are too small
are not acceptable for the intended use.  For example,
the exact buffer size needed for converting an unsigned integer
of a given bit size is

```
  ceil(log(2)/log(10)) * nbits
```

where |nbits| might typically be 16, 32, or 64.

log(2)/log(10) is 0.301029995663981.

59/196 is a fraction within 0.003 percent,
but is slightly smaller.

28/93 might be a better approximation.
It is within 0.01 percent, but is always larger.

146/485 is closer still and is _always_ long.

Even closer fractions are shown, but they require division
of larger numbers, which for this purpose is completely unnecessary.

Eventually, candidate fractions can require higher precision
arithmetic that the ALU wordsize available.


## Options and commands

`--help`

`--version`

`--debug`

Pretty-print values of interest only for debugging.

`--verbose`

Show some feedback while running.

`--argv`

Input numbers are on the rest of the command line.

`--over`

Show only candidate fractions that are over.
That is, the fraction is always >= the given number.

`--under`

Show only candidate fractions that are under.
That is, the fraction is always <= the given number.

## Exit Status

If there are errors in parsing input numbers,
then exit status is 1.

If all goes well, then exit status is 0.

## Why

`smallest-rational` has been used in applications that
generate code and data and compute ahead of time
the sizes of objects such as conversion buffers,
or single-allocation copies of more complex sets of data.

Also, in very small embedded systems,
it is used to pre-compute sizes needed for arrays,
at run-time.  Such machines may not even have
floating-point arithmetic, or the computation
might need to be done in the kernel,
where use of floating-point arithmetic is forbidden.

## Acknowledgements

The heart of the computation is lifted from the following
code from `comp.programming`,

which is based on a paper
by John Canny, Bruce Donald, and Gene Ressler.

```
From: ressler@cs.cornell.edu (Gene Ressler)
Newsgroups: comp.programming
Subject: Re: Printing float as a fraction?
Date: 13 Mar 93 06:27:37 GMT
Organization: Cornell Univ. CS Dept, Ithaca NY 14853

The following function has the nice property of finding
_the_ shortest fraction in the given interval (subject to
floating point error in the loop termination criterion). 
See ACMCG 92 Proceedings "A Rational Rotation Method for Robust
Geometric Algorithms" by John Canny, Bruce Donald, and myself
for an informal explanation.  This also shows how to compute
"short perfect" sine/cosine pairs using the function below
as a subroutine.
```

## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

##

-- Guy Shaw

   gshaw@acm.org

