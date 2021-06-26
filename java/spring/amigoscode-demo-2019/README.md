# Introduction

This projet given an exemple of a complete implementation of a back-end service
using Spring-boot and Postgres SQL database.

This tutorial comes from the video:
https://www.youtube.com/watch?v=vtPkZShrvXQ&ab_channel=freeCodeCamp.org

# Configuration of the differents routes

GET api/v1/person

Return every person in the DB.

GET api/v1/person/*id*

Return one person (or not) given an *id* in the route.

Return every person in the DB.

POST api/v1/person
JSON BODY

``` json
{
  "name": "Toto Tata"
}
```

Add a person to the DB.

PUT api/v1/person/*id*
JSON BODY

``` json
{
  "name": "Update Name"
}
```

Update a person in DB thanks to the *id* in the route and the JSON body of the
request.

DELETE api/v1/person/*id*

Delete a person from DB given an *id* in the route.

# Launch the docker Postgres and populate the DB

The user used to connect to the Postgres DB (-U postgres) is the same as the one
declared in "src/main/resources/application.yml" field "app:datasource:username".

The DB name (demodb) must match the one declared in "src/main/resources/application.yml"
field "app:datasource:jdbc-url".

To get the Postgres "describe" command "\d" or \"dt" to show the list of relations
of a DB, the setup of the project done and run at least once before.

``` shell
docker run --name postgres-spring -e POSTGRES_PASSWORD=password -d -p 5432:5432 postgres:alpine
docker ps
docker port postgres-spring
docker exec -it postgres-spring bin/bash
bash-5.1# psql -U postgres
postgres=# CREATE DATABASE demodb;
postgres=# \l
postgres=# \c demodb
demodb=# \d
demodb=# \dt
demodb=# \d person
demodb=# SELECT * FROM person;
demodb=# CREATE EXTENSION "uuid-ossp";
demodb=# SELECT uuid_generate_v4();
demodb=# INSERT INTO person (id, name) VALUES (uuid_generate_v4(), 'Maria Jones');
demodb=# SELECT * FROM person;
demodb=# INSERT INTO person (id, name) VALUES (uuid_generate_v4(),  'Tom Smith');
demodb=# INSERT INTO person (id, name) VALUES (uuid_generate_v4(),  'Ed Robert');
demodb=# SELECT * FROM person;
demodb=# DELETE FROM person WHERE id = 'fd9b9327-c20f-4d75-857b-98542752f290';
demodb=# SELECT * FROM person;
demodb=# INSERT INTO person (id, name) VALUES (uuid_generate_v4(),  'Maria Jones');
demodb=# INSERT INTO person (id, name) VALUES (uuid_generate_v4(),  'Tom Smith');
demodb=# UPDATE person SET id = '3a5d62ce-19e3-4afc-bb3f-04b2c726fca6', name = 'Tom Payet' WHERE id = '3a5d62ce-19e3-4afc-bb3f-04b2c726fca6';
demodb=# SELECT * FROM person;
demodb=# UPDATE person SET id = 'b8b7946f-6122-4147-933a-7bd3cf5fd3bf', name = 'Tom Payet' WHERE id = 'b8b7946f-6122-4147-933a-7bd3cf5fd3bf';
demodb=# SELECT * FROM person;
demodb=\q
```

This previous working session record using Postgres Docker is there as
a quick memo for Docker, Postgres and SQL.
