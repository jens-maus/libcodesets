# codesets.library

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RAQSDY9YNZVCL)
[![Build Status](https://travis-ci.org/jens-maus/libcodesets.svg?branch=master)](https://travis-ci.org/jens-maus/libcodesets) [![Code Climate](https://codeclimate.com/github/jens-maus/libcodesets/badges/gpa.svg)](https://codeclimate.com/github/jens-maus/libcodesets) [![License](http://img.shields.io/:license-lgpl2-blue.svg?style=flat)](http://www.gnu.org/licenses/lgpl-2.1.html) [![Github Issues](http://githubbadges.herokuapp.com/jens-maus/libcodesets/issues.svg)](https://github.com/jens-maus/libcodesets/issues)

Classic Amiga systems, but even modern AmigaOS versions like AmigaOS4 and
MorphOS don't come with a centralized programming interface for dealing
with different codesets. Especially for the more and more important dealing
with the UTF-8 codeset (e.g. in internet communication programs like YAM or
SimpleMail) a centralized way or API is definitly required and provides
signification advantages. In addition, it should serve as a single interface
point for developers wanting to develop multi-platform applications without
having to implement charset conversion routines on each OS differently.

codesets.library should solve this by providing 3rd party developers an
efficient and easy to use API for dealing with different codesets like
ISO-8859-15, Amiga-1251 but also the mentioned UTF-8 standard, etc. It is
already widly used in known programs like [YAM](https://github.com/jens-maus/yam)
or [SimpleMail](https://github.com/sba1/simplemail) and therefore considered
very stable.

codesets.library is available for AmigaOS3, AmigaOS4, MorphOS and AROS.

## Supported Charsets/Codesets

The currently internally supported (hardcoded) charsets/codesets are:
(conversions are possible from and to each codeset):

* AmigaPL – Polish (Amiga)
* Amiga-1251 – Cyrillic (Amiga)
* ISO-8859-1 – Western European
* ISO-8859-1+Euro – West European (with EURO)
* ISO-8859-2 – Central/East European
* ISO-8859-3 – South European
* ISO-8859-4 – North European
* ISO-8859-5 – Slavic languages
* ISO-8859-9 – Turkish
* ISO-8859-15 – West European II
* ISO-8859-16 – South-Eastern European
* KOI8-R – Russian
* UTF-8 – Unicode

In addition, external charset table files can be stored in LIBS:Charsets
or loaded by an application from `PROGDIR:Charsets`. The charset files included
with this distributions are:

* IBM866 – Cyrillic (cp866)
* ISO-8859-7 – Greek (LatinGreek)
* ISO-8859-10 – Nordic (Latin 6)
* windows-1250 – Central/East Europe (Windows)
* windows-1251 – Cyrillic (Windows)
* windows-1252 – West European (Windows)

More additional, but rarly used charset tables can be found at:
http://www.unicode.org/Public/MAPPINGS/

NOTE: Please note that on operating systems having own codeset/charset
querying support (e.g. AmigaOS4), codesets.library tries to use the system's
functionality as much as possible and will load all additional codesets
supported by the OS itself. Therefore, the above list is just meant to list
the very bare minimum of supported codesets. Please check with your
operating system manual before manually installing additional charset
tables as they might already be available in your OS. In case an internal
charset conflicts with an external charset file, the internal charset is
preferred.

## Downloads/Releases

All releases up to the most current ones can be downloaded from our
[central releases management](https://github.com/jens-maus/libcodesets/releases).

## Bug Reports / Enhancement Requests

To report bugs use the [bug/issue tracker](https://github.com/jens-maus/libcodesets/issues).

## Manual Installation

1. Extract the archive to a temporary directory.
   ```
   > cd RAM:
   > lha x codesets.lha
   ```

2. Go to the `codesets/Libs/<OS>` directory where `<OS>` is the directory
   matching the operating system you want to install the library for:
   ```
   > cd codesets/Libs/AmigaOS4
   ```

3. copy the library out of the `<OS>` directory to your `LIBS:` directory.
   ```
   > copy codesets.library LIBS:
   ```

4. In case you are running an operating system from which codesets.library
   can't directly obtain additional charsets/codesets, you have to copy
   additional charset files to the `LIBS:Charsets` directory. (This is only
   required for AmigaOS3 and MorphOS):
	 ```
   > cd RAM:codesets
   > copy ALL Charsets LIBS:
   ```

5. reboot and enjoy the new version ;)

## License / Copyright

codesets.library was originally written in 2001 and is Copyright (C) 2001-2005 by Alfonso Ranieri.
As of 2005 the library is maintained by an independent open source team and
Copyright (C) 2005-2017 codesets.library Open Source Team.

codesets.library is distributed and licensed under the GNU Lesser General Public License Version 2.1.
See [COPYING](COPYING) for more detailed information.

## Authors

* Alexandre Balaban
* Alfonso Ranieri
* Ilkka Lehtoranta
* Jens Maus
* Matthias Rustler
* Pavel Fedin
* Sebastian Bauer
* Staf Verhaegen
* Thore Böckelmann
