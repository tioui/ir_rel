IR_REL
======

Copyright (C) 2013 Louis Marchand <prog@tioui.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
This plugin permit to use the remote IR pointer as a relative axes.

This cwiid plugin is base on the ir_ptr and acc plugins from L. Donnie Smith <donnie.smith@gatech.edu>

COMPILATION INSTRUCTION
___________

* Download and extract the cwiid source code (http://abstrakraft.org/cwiid/downloads/cwiid-0.6.00.tgz).
* Execute the configure script in the root directory of the cwiid source code. For more information, see the cwiid README file.
* After the configure has successfully execute, copy the current directory (ir_rel) in the <CWIID_source_code>/wminput/plugins/ directory.
* In the <CWIID_source_code>/wminput/plugins/ir_rel/ directory, execute those command lines:

***

      gcc -g -Wall -W -DHAVE_CONFIG_H -I../../../common/include -I../../../wminput -I../../../libcwiid -fpic   -c -o ir_rel.o ir_rel.c
      gcc -shared   -o ir_rel.so ir_rel.o

***

* To install the plugin for one user execute those command lines:

***

    mkdir -p ~/.cwiid/plugins
    cp -p ir_rel.so ~/.cwiid/plugins/

***

* To install the plugin in the plugin system wide, execute this command line with root privilege (on some system, you will have to use sudo):

***

    cp -p ir_rel.so /usr/local/lib/cwiid/plugins

***

* On some system, the plugins directory is in /usr/lib/cwiid/plugins/. So, if the precedent command give you a "Directory not exist" kind of error, use this command:

***

      cp -p ir_rel.so /usr/lib/cwiid/plugins

***

UTILISATION
___________

In a wminput config file, use those lines:

***

      Plugin.ir_rel.X = REL_X
      Plugin.ir_rel.Y = REL_Y

***
