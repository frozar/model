package com.example.demo.dao;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.springframework.stereotype.Repository;

import com.example.demo.model.Person;

// The @Repository annotation will tell spring to instanciate,
// create a bean, for the "PersonDao" interface

// The string "fakeDao" is used by the @Qualifier annotation to
// to know which class to instanciate.

@Repository("fakeDao")
public class FakePersonDataAccessService implements PersonDao {
  
  private static List<Person> DB = new ArrayList<>();

  @Override
  public int insertPerson(UUID id, Person person) {
    DB.add(new Person(id, person.getName()));
    return 1;
  }

  @Override
  public List<Person> selectAllPeople() {
    return DB;
  }

}
