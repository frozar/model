# Introduction

This projet given an exemple of a complete implementation of a back-end service
using Spring-boot and Postgres SQL database.

This tutorial comes from the video:
https://www.youtube.com/watch?v=9SGDpanrc8U&ab_channel=Amigoscode

# Launch the application

The DB should be created before launching the application.

``` shell
./mvnw spring-boot:run
```

# Configuration of the differents routes

GET api/v1/student

Return every student in the DB.

POST api/v1/student
JSON BODY

``` json
{
    "name": "Bilal",
    "email": "bilal.ahmed@gmail.com",
    "dob": "1995-12-17"
}
```

Add a student to the DB.

PUT api/v1/student/*id*?name=Maria&email=maria@gmail.com

Update a student in DB thanks to the *id* in the route and parameters
of the request.

DELETE api/v1/student/*id*

Delete a person from DB given an *id* in the route.

# Launch the docker Postgres and populate the DB

The user used to connect to the Postgres DB (-U postgres) is the same as the one
declared in "src/main/resources/application.properties" field "spring.datasource.username".

The DB name (student) must match the one declared in "src/main/resources/application.properties"
field "spring.datasource.url".

To get the Postgres "describe" command "\d" or \"dt" to show the list of relations
of a DB, the setup of the project done and run at least once before.

``` shell
docker run --name postgres-spring -e POSTGRES_PASSWORD=password -d -p 5432:5432 postgres:alpine
docker exec -it postgres-spring bin/bash
bash-5.1# psql -U postgres
postgres=# \l
postgres=# CREATE DATABASE student;
postgres=# \c student
student=# \d
student=# \d student
student=# SELECT * FROM student;
```

This previous working session record using Postgres Docker is there as
a quick memo for Docker, Postgres and SQL.

# Generate a standalone executable

In Eclipse, run the clean Maven command: it deletes the target directory.
In Eclipse, run the install Maven command: it create the .jar file in the target
directory.

Launch the .jar file from the target directory:

``` shell
java -jar demo-0.0.1-SNAPSHOT.jar
```

To launch multiple instances of this executable, you must change the
port used by the .jar file:
``` shell
java -jar demo-0.0.1-SNAPSHOT.jar --server.port=8081
```
