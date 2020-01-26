# datomic-tuto

This porject is an update of the amazing tutorial video of M Misophistful:
https://www.youtube.com/watch?v=ao7xEwCjrWQ

Since 2013, the datomic API has slighly changed and the way Datomic is
distributed also evolved.

The purpose of this project is to run a local version of Datomic database.

## Installation

This project should work with the free datomic version avalaible here:
https://my.datomic.com/downloads/free
With datomic free archive, you don't need a license.

But this tutorial project is designed to get ready to use datomic in real
project, so I decided to use the pro version:
https://my.datomic.com/downloads/pro
To use it, you have get the Datomic Pro Starter Edition license which is
free of charge.

More information avalaible at:
https://www.datomic.com/get-datomic.html

This project is configured and launch with the leiningen tool.

Once the license obtained, to be able to use the `[com.datomic/datomic-pro <VERSION>]`
dependency in your project, you have to follow the steps avalaible here to
configure the correct credentials:
https://my.datomic.com/account

The credentials require GPG keys:
https://github.com/technomancy/leiningen/blob/stable/doc/GPG.md

You can also find more informations about leiningen GPG authentication here:
https://github.com/technomancy/leiningen/blob/master/doc/DEPLOY.md#authentication

## Usage

To use this porject, you have to launch a transactor and a peer server (manually).

Once you have download datomic archive, move the uncompress the datomic folder:

``` shell
cd /you/datomic/folder/datomic-pro-<VERSION>
```

To use datomic local, you have to copy/paste the `dev-transactor-template.properties`
file to your project:
``` shell
cp config/sample/dev-transactor-template.properties /your/project/resources/datomic
```
This project provide the `resources/datomic/dev-transactor-template.properties.origin`
file which is the same as the one provided originally. In this file, you have to
fill the field `license-key` with the key you should have received by mail from
the datomic support.

Then you can launch your local transactor:
``` shell
bin/transactor /your/project/resources/datomic/dev-transactor-template.properties
```

And finally, in another terminal, you can launch the peer server:
``` shell
bin/run -m datomic.peer-server -h localhost -p 8998 -a myaccesskey,mysecret -d pet-owners-db,datomic:dev://localhost:4334/pet-owners-db
```

If the setup run fine, you can launch the test of this project:
``` shell
cd /your/project/
lein test
```

Of course, the source code from `src/datomic_tuto/core.clj` constitutes a good
starting point to tinkering around datomic.

## Resources

To build this project I heavy used the following resources.

Documentation datomic on-prem:
https://docs.datomic.com/on-prem/index.html

Learn datomic today:
http://www.learndatalogtoday.org/

Day of datomic:
https://docs.datomic.com/on-prem/day-of-datomic.html

## License

Copyright © 2020 frozar

This program and the accompanying materials are made available under the
terms of the Eclipse Public License 2.0 which is available at
http://www.eclipse.org/legal/epl-2.0.

This Source Code may also be made available under the following Secondary
Licenses when the conditions for such availability set forth in the Eclipse
Public License, v. 2.0 are satisfied: GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or (at your
option) any later version, with the GNU Classpath Exception which is available
at https://www.gnu.org/software/classpath/license.html.
