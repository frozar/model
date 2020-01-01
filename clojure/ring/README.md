# ring-example

An example of ring application: simple web-service which computes fibonacci
number.

## Usage

``` bash
lein ring server-headless
```
This command will launch the application on port 3000. The query string must
contains a parameter "n" to trigger the computer of fibo(n):
http://localhost:3000/?n=10

## License

Copyright © 2019 frozar

This program and the accompanying materials are made available under the
terms of the Eclipse Public License 2.0 which is available at
http://www.eclipse.org/legal/epl-2.0.

This Source Code may also be made available under the following Secondary
Licenses when the conditions for such availability set forth in the Eclipse
Public License, v. 2.0 are satisfied: GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or (at your
option) any later version, with the GNU Classpath Exception which is available
at https://www.gnu.org/software/classpath/license.html.
