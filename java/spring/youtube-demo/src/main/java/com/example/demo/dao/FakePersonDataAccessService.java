package com.example.demo.dao;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
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

  @Override
  public Optional<Person> selectPersonById(UUID id) {
    return DB.stream()
        .filter(person -> person.getId().equals(id))
        .findFirst();
  }

  @Override
  public int deletePersonById(UUID id) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public int updatePersonById(UUID id, Person person) {
    // TODO Auto-generated method stub
    return 0;
  }

}
