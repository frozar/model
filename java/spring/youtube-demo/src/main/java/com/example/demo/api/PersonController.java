package com.example.demo.api;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.demo.model.Person;
import com.example.demo.service.PersonService;

// The controller deals with HTTP requests.

// The @Autowired annotation tells to spring to inject the service 
// to the constructor "PersonController"

// The @RestController annotation tells spring to consider this class
// as a Controller: so it will deal with HTTP requests.

// The @PostMapping annotation tells spring to consider the "addPerson"
// method to deal with POST requests.

// The @RequestBody tells spring to send the JSON payload "as argument"
// to the "Person" constructor.

// The @RequestMapping declare the route handle by the "PersonController" 
// class: it create the spring boot end-point "api/v1/person"

@RequestMapping("api/v1/person")
@RestController
public class PersonController {
  
  private final PersonService personService;
  
  @Autowired
  public PersonController(PersonService personService) {
    this.personService = personService;
  }
  
  @PostMapping
  public void addPerson(@RequestBody Person person) {
    personService.addPerson(person);
  }

}
