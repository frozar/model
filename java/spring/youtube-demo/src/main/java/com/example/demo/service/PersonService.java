package com.example.demo.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.example.demo.dao.PersonDao;
import com.example.demo.model.Person;

// The @Service annotation is for the data access service.

// The @Autowired annotation tells spring to do the injection 
// of the "PersonDao" interface in the constructor.

// The @Qualifier annotation allow to specify the class to instanciate

@Service
public class PersonService {
  
  private final PersonDao personDao;
  
  @Autowired
  public PersonService(@Qualifier("fakeDao") PersonDao personDao) {
    this.personDao = personDao;
  }
  
  public int addPerson(Person person) {
    return personDao.insertPerson(person);
  }
  
  public List<Person> getAllPeople() {
    return personDao.selectAllPeople();
  }

}
